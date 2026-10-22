// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#جے_بنا_تعریف SRC_PERSISTENCE_HPP
#تعریف_کرو SRC_PERSISTENCE_HPP

#ضمن <deque>
#ضمن <sstream>
#ضمن <string>
#ضمن <iterator>

#تعریف_کرو USE_ZLIB 1
#إذا USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_وتھ_بٹ
#    define FILE_وتھ_بٹ 0
#  endif
#  include <zlib.h>
#وإلا
تعريفنوع فراغ* gzFile;
#جے_انت

//=====================================================================================

ساكن ثابت size_t PARSER_BASE64_بفر_ناپ = 1024U * 1024U / 8U;
ساكن ثابت صحيح CV_PERSISTENCE_MAX_ڈونگھائی = 1024;

نطاق base64 {

تعداد
{
    سرخی_ناپ         = 24,
    ENCODED_سرخی_ناپ = 32
};

} // base64::

//=====================================================================================

#تعریف_کرو CV_FS_MAX_LEN 4096
#تعریف_کرو CV_FS_MAX_FMT_جوڑے  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#تعریف_کرو cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#تعریف_کرو cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

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
حرف* float_منزل_String( حرف* buf, size_t buf_ناپ, عائم value, منطقي halfprecision, منطقي explicitZero );
حرف* double_منزل_String( حرف* buf, size_t buf_ناپ, مزدوج value, منطقي explicitZero );

صحيح calc_Struct_ناپ( ثابت حرف* dt, صحيح initial_ناپ );
صحيح calc_Elem_ناپ( ثابت حرف* dt, صحيح initial_ناپ );
CV_DEPRECATED حرف* encodeFormat( صحيح elem_type, حرف* dt );
حرف* encodeFormat( صحيح elem_type, حرف* dt, size_t dt_len );
صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_جوڑے, صحيح max_len );
صحيح decodeSimpleFormat( ثابت حرف* dt );
}


#جے_تعریف_شدہ CV_STATIC_ANALYSIS
#تعریف_کرو CV_PARSE_ERROR_CPP(errmsg) افعل { (فراغ)fs; abort(); } طالما (0)
#وإلا
#تعریف_کرو CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#جے_انت


#تعریف_کرو CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP() افعل { \
    CV_DbgAssert(ptr); \
    إذا((ptr)[0] == 0 && (ptr) == fs->بفر_سرا_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} طالما (0)


صنف FileStorageParser;
صنف FileStorageEmitter;

بنية FStruct_انکڑے
{
    FStruct_انکڑے() { indent = flags = 0; }
    FStruct_انکڑے( ثابت std::string& struct_ٹیگ,
                 صحيح _struct_flags, صحيح _struct_indent )
    {
        ٹیگ = struct_ٹیگ;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string ٹیگ;
    صحيح flags;
    صحيح indent;
};

صنف FileStorage_API
{
عام:
    مجازی ~FileStorage_API();
    مجازی FileStorage* getFS() = 0;

    مجازی صحيح getFormat() ثابت = 0;

    مجازی فراغ puts( ثابت حرف* str ) = 0;
    مجازی حرف* gets() = 0;
    مجازی منطقي eof() = 0;
    مجازی فراغ سیٹ_Eof() = 0;
    مجازی فراغ closeFile() = 0;
    مجازی فراغ rewind() = 0;
    مجازی حرف* resize_Write_بفر( حرف* ptr, صحيح len ) = 0;
    مجازی حرف* بفر_Ptr() ثابت = 0;
    مجازی حرف* بفر_شروع() ثابت = 0;
    مجازی حرف* بفر_سرا_2() ثابت = 0;
    مجازی فراغ سیٹ_بفر_Ptr(حرف* ptr) = 0;
    مجازی حرف* flush() = 0;
    مجازی فراغ سیٹ_Non_خالی_تھاواں() = 0;
    مجازی صحيح wrapMargin() ثابت = 0;

    مجازی FStruct_انکڑے& getCurrentStruct() = 0;

    مجازی فراغ convert_منزل_Collection( صحيح type, File_گرہ& گرہ ) = 0;
    مجازی File_گرہ جوڑو_گرہ( File_گرہ& collection, ثابت std::string& کلید,
                               صحيح type, ثابت فراغ* value=0, صحيح len=-1 ) = 0;
    مجازی فراغ finalizeCollection( File_گرہ& collection ) = 0;
    مجازی مزدوج strtod(حرف* ptr, حرف** endptr) = 0;

    مجازی حرف* parseBase64(حرف* ptr, صحيح indent, File_گرہ& collection) = 0;
    CV_NORETURN
    مجازی فراغ parseError(ثابت حرف* funcname, ثابت std::string& پیغام,
                            ثابت حرف* filename, صحيح lineno) = 0;

خاص:
    تعداد Base64State{
        Uncertain,
        NotUse,
        وچ_Use,
    };

    یار صنف cv::FileStorage::Impl;
    یار صنف cv::FileStorage;
    یار صنف JSONEmitter;
    یار صنف XMLEmitter;
    یار صنف YAMLEmitter;

    مجازی فراغ check_if_write_struct_is_delayed(منطقي change_type_منزل_base64 = خطأ) = 0;
    مجازی فراغ switch_منزل_Base64_state(Base64State state) = 0;
    مجازی Base64State get_state_دا_writing_base64() = 0;
    مجازی صحيح get_space() = 0;
};

صنف FileStorageEmitter
{
عام:
    مجازی ~FileStorageEmitter() {}

    مجازی FStruct_انکڑے شروع_Write_Struct( ثابت FStruct_انکڑے& ماپا, ثابت حرف* کلید,
                                          صحيح struct_flags, ثابت حرف* type_name=0 ) = 0;
    مجازی فراغ سرا_Write_Struct(ثابت FStruct_انکڑے& current_struct) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, صحيح value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, int64_t value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, مزدوج value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, ثابت حرف* value, منطقي quote) = 0;
    مجازی فراغ writeScalar(ثابت حرف* کلید, ثابت حرف* value) = 0;
    مجازی فراغ writeComment(ثابت حرف* comment, منطقي eol_comment) = 0;
    مجازی فراغ شروع_اگلا_دھارا() = 0;
};

صنف FileStorageParser
{
عام:
    مجازی ~FileStorageParser() {}
    مجازی منطقي parse(حرف* ptr) = 0;
    مجازی منطقي get_Base64_صف(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#جے_انت // SRC_PERSISTENCE_HPP
