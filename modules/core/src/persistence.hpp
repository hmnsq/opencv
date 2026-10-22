// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#که_نه_تعریف_شوی SRC_PERSISTENCE_HPP
#تعریف_کړه SRC_PERSISTENCE_HPP

#ضمن <deque>
#ضمن <sstream>
#ضمن <string>
#ضمن <iterator>

#تعریف_کړه USE_ZLIB 1
#إذا USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_دننه_واټن_بټونه
#    define FILE_دننه_واټن_بټونه 0
#  endif
#  include <zlib.h>
#وإلا
تعريفنوع فراغ* gzFile;
#د_که_پای

//=====================================================================================

ساكن ثابت size_t PARSER_BASE64_بفر_اندازه = 1024U * 1024U / 8U;
ساكن ثابت صحيح CV_PERSISTENCE_MAX_ژوروالی = 1024;

نطاق base64 {

تعداد
{
    سرلیک_اندازه         = 24,
    ENCODED_سرلیک_اندازه = 32
};

} // base64::

//=====================================================================================

#تعریف_کړه CV_FS_MAX_LEN 4096
#تعریف_کړه CV_FS_MAX_FMT_جوړې  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#تعریف_کړه cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#تعریف_کړه cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

مضمن منطقي cv_isalnum(حرف c)
{
    أرجع ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

مضمن منطقي cv_isalpha(حرف c)
{
    أرجع ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

مضمن منطقي cv_isdigit(حرف c)
{
    أرجع '0' <= c && c <= '9';
}

مضمن منطقي cv_isspace(حرف c)
{
    أرجع (9 <= c && c <= 13) || c == ' ';
}

مضمن حرف* cv_skip_BOM(حرف* ptr)
{
    إذا((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      أرجع ptr + 3;
    }
    أرجع ptr;
}

نطاق cv
{
نطاق fs
{
صحيح strcasecmp(ثابت حرف* str1, ثابت حرف* str2);
حرف* itoa( صحيح _val, حرف* بفر, صحيح /*radix*/ );
حرف* itoa( int64_t _val, حرف* بفر, صحيح /*radix*/, منطقي _signed );
حرف* float_منزل_String( حرف* buf, size_t buf_اندازه, عائم value, منطقي halfprecision, منطقي explicitZero );
حرف* double_منزل_String( حرف* buf, size_t buf_اندازه, مزدوج value, منطقي explicitZero );

صحيح calc_Struct_اندازه( ثابت حرف* dt, صحيح initial_اندازه );
صحيح calc_Elem_اندازه( ثابت حرف* dt, صحيح initial_اندازه );
CV_DEPRECATED حرف* encodeFormat( صحيح elem_type, حرف* dt );
حرف* encodeFormat( صحيح elem_type, حرف* dt, size_t dt_len );
صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_جوړې, صحيح max_len );
صحيح decodeSimpleFormat( ثابت حرف* dt );
}


#که_تعریف_شوی CV_STATIC_ANALYSIS
#تعریف_کړه CV_PARSE_ERROR_CPP(errmsg) افعل { (فراغ)fs; abort(); } طالما (0)
#وإلا
#تعریف_کړه CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#د_که_پای


#تعریف_کړه CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP() افعل { \
    CV_DbgAssert(ptr); \
    إذا((ptr)[0] == 0 && (ptr) == fs->بفر_پای_ټکی_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} طالما (0)


صنف FileStorageParser;
صنف FileStorageEmitter;

بنية FStruct_معلومات
{
    FStruct_معلومات() { indent = flags = 0; }
    FStruct_معلومات( ثابت std::string& struct_نښان,
                 صحيح _struct_flags, صحيح _struct_indent )
    {
        نښان = struct_نښان;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string نښان;
    صحيح flags;
    صحيح indent;
};

صنف FileStorage_API
{
عام:
    مجازي ~FileStorage_API();
    مجازي FileStorage* getFS() = 0;

    مجازي صحيح getFormat() ثابت = 0;

    مجازي فراغ puts( ثابت حرف* str ) = 0;
    مجازي حرف* gets() = 0;
    مجازي منطقي eof() = 0;
    مجازي فراغ سېټ_Eof() = 0;
    مجازي فراغ closeFile() = 0;
    مجازي فراغ rewind() = 0;
    مجازي حرف* resize_Write_بفر( حرف* ptr, صحيح len ) = 0;
    مجازي حرف* بفر_Ptr() ثابت = 0;
    مجازي حرف* بفر_پیل() ثابت = 0;
    مجازي حرف* بفر_پای_ټکی_2() ثابت = 0;
    مجازي فراغ سېټ_بفر_Ptr(حرف* ptr) = 0;
    مجازي حرف* flush() = 0;
    مجازي فراغ سېټ_Non_تشه() = 0;
    مجازي صحيح wrapMargin() ثابت = 0;

    مجازي FStruct_معلومات& getCurrentStruct() = 0;

    مجازي فراغ convert_منزل_Collection( صحيح type, File_غوټه& غوټه ) = 0;
    مجازي File_غوټه جمع_کړه_غوټه( File_غوټه& collection, ثابت std::string& کلی,
                               صحيح type, ثابت فراغ* value=0, صحيح len=-1 ) = 0;
    مجازي فراغ finalizeCollection( File_غوټه& collection ) = 0;
    مجازي مزدوج strtod(حرف* ptr, حرف** endptr) = 0;

    مجازي حرف* parseBase64(حرف* ptr, صحيح indent, File_غوټه& collection) = 0;
    CV_NORETURN
    مجازي فراغ parseError(ثابت حرف* funcname, ثابت std::string& پیغام,
                            ثابت حرف* filename, صحيح lineno) = 0;

خاص:
    تعداد Base64State{
        Uncertain,
        NotUse,
        دننه_Use,
    };

    ملګری صنف cv::FileStorage::Impl;
    ملګری صنف cv::FileStorage;
    ملګری صنف JSONEmitter;
    ملګری صنف XMLEmitter;
    ملګری صنف YAMLEmitter;

    مجازي فراغ check_if_write_struct_is_delayed(منطقي change_type_منزل_base64 = خطأ) = 0;
    مجازي فراغ switch_منزل_Base64_state(Base64State state) = 0;
    مجازي Base64State get_state_د_writing_base64() = 0;
    مجازي صحيح get_space() = 0;
};

صنف FileStorageEmitter
{
عام:
    مجازي ~FileStorageEmitter() {}

    مجازي FStruct_معلومات پیل_Write_Struct( ثابت FStruct_معلومات& پلار, ثابت حرف* کلی,
                                          صحيح struct_flags, ثابت حرف* type_name=0 ) = 0;
    مجازي فراغ پای_ټکی_Write_Struct(ثابت FStruct_معلومات& current_struct) = 0;
    مجازي فراغ اكتب(ثابت حرف* کلی, صحيح value) = 0;
    مجازي فراغ اكتب(ثابت حرف* کلی, int64_t value) = 0;
    مجازي فراغ اكتب(ثابت حرف* کلی, مزدوج value) = 0;
    مجازي فراغ اكتب(ثابت حرف* کلی, ثابت حرف* value, منطقي quote) = 0;
    مجازي فراغ writeScalar(ثابت حرف* کلی, ثابت حرف* value) = 0;
    مجازي فراغ writeComment(ثابت حرف* comment, منطقي eol_comment) = 0;
    مجازي فراغ پیل_بل_بهیر() = 0;
};

صنف FileStorageParser
{
عام:
    مجازي ~FileStorageParser() {}
    مجازي منطقي parse(حرف* ptr) = 0;
    مجازي منطقي get_Base64_لیکه(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#د_که_پای // SRC_PERSISTENCE_HPP
