// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#اگر_تعریف_نشده SRC_PERSISTENCE_HPP
#تعریف_کن SRC_PERSISTENCE_HPP

#ضمن <deque>
#ضمن <sstream>
#ضمن <string>
#ضمن <iterator>

#تعریف_کن USE_ZLIB 1
#إذا USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_جابه_جایی_بیتها
#    define FILE_جابه_جایی_بیتها 0
#  endif
#  include <zlib.h>
#وإلا
تعريفنوع فراغ* gzFile;
#پایان_اگر

//=====================================================================================

ساكن ثابت size_t PARSER_BASE64_میانگیر_اندازه = 1024U * 1024U / 8U;
ساكن ثابت صحيح CV_PERSISTENCE_MAX_عمق = 1024;

نطاق base64 {

تعداد
{
    سرآیند_اندازه         = 24,
    ENCODED_سرآیند_اندازه = 32
};

} // base64::

//=====================================================================================

#تعریف_کن CV_FS_MAX_LEN 4096
#تعریف_کن CV_FS_MAX_FMT_جفتها  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#تعریف_کن cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#تعریف_کن cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

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
حرف* itoa( صحيح _val, حرف* میانگیر, صحيح /*radix*/ );
حرف* itoa( int64_t _val, حرف* میانگیر, صحيح /*radix*/, منطقي _signed );
حرف* float_مقصد_String( حرف* buf, size_t buf_اندازه, عائم value, منطقي halfprecision, منطقي explicitZero );
حرف* double_مقصد_String( حرف* buf, size_t buf_اندازه, مزدوج value, منطقي explicitZero );

صحيح calc_Struct_اندازه( ثابت حرف* dt, صحيح initial_اندازه );
صحيح calc_Elem_اندازه( ثابت حرف* dt, صحيح initial_اندازه );
CV_DEPRECATED حرف* encodeFormat( صحيح elem_type, حرف* dt );
حرف* encodeFormat( صحيح elem_type, حرف* dt, size_t dt_len );
صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_جفتها, صحيح max_len );
صحيح decodeSimpleFormat( ثابت حرف* dt );
}


#اگر_تعریف_شده CV_STATIC_ANALYSIS
#تعریف_کن CV_PARSE_ERROR_CPP(errmsg) افعل { (فراغ)fs; abort(); } طالما (0)
#وإلا
#تعریف_کن CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#پایان_اگر


#تعریف_کن CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP() افعل { \
    CV_DbgAssert(ptr); \
    إذا((ptr)[0] == 0 && (ptr) == fs->میانگیر_انتها_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} طالما (0)


صنف FileStorageParser;
صنف FileStorageEmitter;

بنية FStruct_داده
{
    FStruct_داده() { indent = flags = 0; }
    FStruct_داده( ثابت std::string& struct_برچسب,
                 صحيح _struct_flags, صحيح _struct_indent )
    {
        برچسب = struct_برچسب;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string برچسب;
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
    مجازی فراغ مجموعه_Eof() = 0;
    مجازی فراغ closeFile() = 0;
    مجازی فراغ rewind() = 0;
    مجازی حرف* resize_Write_میانگیر( حرف* ptr, صحيح len ) = 0;
    مجازی حرف* میانگیر_Ptr() ثابت = 0;
    مجازی حرف* میانگیر_شروع() ثابت = 0;
    مجازی حرف* میانگیر_انتها_2() ثابت = 0;
    مجازی فراغ مجموعه_میانگیر_Ptr(حرف* ptr) = 0;
    مجازی حرف* flush() = 0;
    مجازی فراغ مجموعه_Non_خالی() = 0;
    مجازی صحيح wrapMargin() ثابت = 0;

    مجازی FStruct_داده& getCurrentStruct() = 0;

    مجازی فراغ convert_مقصد_Collection( صحيح type, File_گره& گره ) = 0;
    مجازی File_گره جمع_کن_گره( File_گره& collection, ثابت std::string& کلید,
                               صحيح type, ثابت فراغ* value=0, صحيح len=-1 ) = 0;
    مجازی فراغ finalizeCollection( File_گره& collection ) = 0;
    مجازی مزدوج strtod(حرف* ptr, حرف** endptr) = 0;

    مجازی حرف* parseBase64(حرف* ptr, صحيح indent, File_گره& collection) = 0;
    CV_NORETURN
    مجازی فراغ parseError(ثابت حرف* funcname, ثابت std::string& پیام,
                            ثابت حرف* filename, صحيح lineno) = 0;

خاص:
    تعداد Base64State{
        Uncertain,
        NotUse,
        درون_Use,
    };

    دوست صنف cv::FileStorage::Impl;
    دوست صنف cv::FileStorage;
    دوست صنف JSONEmitter;
    دوست صنف XMLEmitter;
    دوست صنف YAMLEmitter;

    مجازی فراغ check_if_write_struct_is_delayed(منطقي change_type_مقصد_base64 = خطأ) = 0;
    مجازی فراغ switch_مقصد_Base64_state(Base64State state) = 0;
    مجازی Base64State get_state_از_writing_base64() = 0;
    مجازی صحيح get_space() = 0;
};

صنف FileStorageEmitter
{
عام:
    مجازی ~FileStorageEmitter() {}

    مجازی FStruct_داده شروع_Write_Struct( ثابت FStruct_داده& والد, ثابت حرف* کلید,
                                          صحيح struct_flags, ثابت حرف* type_name=0 ) = 0;
    مجازی فراغ انتها_Write_Struct(ثابت FStruct_داده& current_struct) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, صحيح value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, int64_t value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, مزدوج value) = 0;
    مجازی فراغ اكتب(ثابت حرف* کلید, ثابت حرف* value, منطقي quote) = 0;
    مجازی فراغ writeScalar(ثابت حرف* کلید, ثابت حرف* value) = 0;
    مجازی فراغ writeComment(ثابت حرف* comment, منطقي eol_comment) = 0;
    مجازی فراغ شروع_بعدی_جریان() = 0;
};

صنف FileStorageParser
{
عام:
    مجازی ~FileStorageParser() {}
    مجازی منطقي parse(حرف* ptr) = 0;
    مجازی منطقي get_Base64_ردیف(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#پایان_اگر // SRC_PERSISTENCE_HPP
