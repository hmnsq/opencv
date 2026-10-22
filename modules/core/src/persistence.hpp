// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#اگر_غیر_تعریف_شدہ SRC_PERSISTENCE_HPP
#تعریف_کرو SRC_PERSISTENCE_HPP

#شامل <deque>
#شامل <sstream>
#شامل <string>
#شامل <iterator>

#تعریف_کرو USE_ZLIB 1
#اگر USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_انحراف_بٹ
#    define FILE_انحراف_بٹ 0
#  endif
#  include <zlib.h>
#ورنہ
قسمتعریف خالی* gzFile;
#اگر_ختم

//=====================================================================================

ساکن ثابت size_t PARSER_BASE64_بفر_سائز = 1024U * 1024U / 8U;
ساکن ثابت صحیح CV_PERSISTENCE_MAX_گہرائی = 1024;

نامخلا base64 {

شمار
{
    سرخی_سائز         = 24,
    ENCODED_سرخی_سائز = 32
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

اندرونی منطقی cv_isalnum(حرف c)
{
    واپس ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

اندرونی منطقی cv_isalpha(حرف c)
{
    واپس ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

اندرونی منطقی cv_isdigit(حرف c)
{
    واپس '0' <= c && c <= '9';
}

اندرونی منطقی cv_isspace(حرف c)
{
    واپس (9 <= c && c <= 13) || c == ' ';
}

اندرونی حرف* cv_skip_BOM(حرف* ptr)
{
    اگر((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      واپس ptr + 3;
    }
    واپس ptr;
}

نامخلا cv
{
نامخلا fs
{
صحیح strcasecmp(ثابت حرف* str1, ثابت حرف* str2);
حرف* itoa( صحیح _val, حرف* بفر, صحیح /*radix*/ );
حرف* itoa( int64_t _val, حرف* بفر, صحیح /*radix*/, منطقی _signed );
حرف* float_مقصد_String( حرف* buf, size_t buf_سائز, اعشاری value, منطقی halfprecision, منطقی explicitZero );
حرف* double_مقصد_String( حرف* buf, size_t buf_سائز, دوہرا value, منطقی explicitZero );

صحیح calc_Struct_سائز( ثابت حرف* dt, صحیح initial_سائز );
صحیح calc_Elem_سائز( ثابت حرف* dt, صحیح initial_سائز );
CV_DEPRECATED حرف* encodeFormat( صحیح elem_type, حرف* dt );
حرف* encodeFormat( صحیح elem_type, حرف* dt, size_t dt_len );
صحیح decodeFormat( ثابت حرف* dt, صحیح* fmt_جوڑے, صحیح max_len );
صحیح decodeSimpleFormat( ثابت حرف* dt );
}


#اگر_تعریف_شدہ CV_STATIC_ANALYSIS
#تعریف_کرو CV_PARSE_ERROR_CPP(errmsg) کرو { (خالی)fs; abort(); } جب (0)
#ورنہ
#تعریف_کرو CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#اگر_ختم


#تعریف_کرو CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP() کرو { \
    CV_DbgAssert(ptr); \
    اگر((ptr)[0] == 0 && (ptr) == fs->بفر_انتہا_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} جب (0)


جماعت FileStorageParser;
جماعت FileStorageEmitter;

ساخت FStruct_ڈیٹا
{
    FStruct_ڈیٹا() { indent = flags = 0; }
    FStruct_ڈیٹا( ثابت std::string& struct_لیبل,
                 صحیح _struct_flags, صحیح _struct_indent )
    {
        لیبل = struct_لیبل;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string لیبل;
    صحیح flags;
    صحیح indent;
};

جماعت FileStorage_API
{
عوامی:
    مجازی ~FileStorage_API();
    مجازی FileStorage* getFS() = 0;

    مجازی صحیح getFormat() ثابت = 0;

    مجازی خالی puts( ثابت حرف* str ) = 0;
    مجازی حرف* gets() = 0;
    مجازی منطقی eof() = 0;
    مجازی خالی سیٹ_Eof() = 0;
    مجازی خالی closeFile() = 0;
    مجازی خالی rewind() = 0;
    مجازی حرف* resize_Write_بفر( حرف* ptr, صحیح len ) = 0;
    مجازی حرف* بفر_Ptr() ثابت = 0;
    مجازی حرف* بفر_آغاز() ثابت = 0;
    مجازی حرف* بفر_انتہا_2() ثابت = 0;
    مجازی خالی سیٹ_بفر_Ptr(حرف* ptr) = 0;
    مجازی حرف* flush() = 0;
    مجازی خالی سیٹ_Non_خالی_خانے() = 0;
    مجازی صحیح wrapMargin() ثابت = 0;

    مجازی FStruct_ڈیٹا& getCurrentStruct() = 0;

    مجازی خالی convert_مقصد_Collection( صحیح type, File_گرہ& گرہ ) = 0;
    مجازی File_گرہ جوڑو_گرہ( File_گرہ& collection, ثابت std::string& کلید,
                               صحیح type, ثابت خالی* value=0, صحیح len=-1 ) = 0;
    مجازی خالی finalizeCollection( File_گرہ& collection ) = 0;
    مجازی دوہرا strtod(حرف* ptr, حرف** endptr) = 0;

    مجازی حرف* parseBase64(حرف* ptr, صحیح indent, File_گرہ& collection) = 0;
    CV_NORETURN
    مجازی خالی parseError(ثابت حرف* funcname, ثابت std::string& پیغام,
                            ثابت حرف* filename, صحیح lineno) = 0;

نجی:
    شمار Base64State{
        Uncertain,
        NotUse,
        اندر_Use,
    };

    دوست جماعت cv::FileStorage::Impl;
    دوست جماعت cv::FileStorage;
    دوست جماعت JSONEmitter;
    دوست جماعت XMLEmitter;
    دوست جماعت YAMLEmitter;

    مجازی خالی check_if_write_struct_is_delayed(منطقی change_type_مقصد_base64 = جھوٹ) = 0;
    مجازی خالی switch_مقصد_Base64_state(Base64State state) = 0;
    مجازی Base64State get_state_کا_writing_base64() = 0;
    مجازی صحیح get_space() = 0;
};

جماعت FileStorageEmitter
{
عوامی:
    مجازی ~FileStorageEmitter() {}

    مجازی FStruct_ڈیٹا آغاز_Write_Struct( ثابت FStruct_ڈیٹا& والد, ثابت حرف* کلید,
                                          صحیح struct_flags, ثابت حرف* type_name=0 ) = 0;
    مجازی خالی انتہا_Write_Struct(ثابت FStruct_ڈیٹا& current_struct) = 0;
    مجازی خالی اكتب(ثابت حرف* کلید, صحیح value) = 0;
    مجازی خالی اكتب(ثابت حرف* کلید, int64_t value) = 0;
    مجازی خالی اكتب(ثابت حرف* کلید, دوہرا value) = 0;
    مجازی خالی اكتب(ثابت حرف* کلید, ثابت حرف* value, منطقی quote) = 0;
    مجازی خالی writeScalar(ثابت حرف* کلید, ثابت حرف* value) = 0;
    مجازی خالی writeComment(ثابت حرف* comment, منطقی eol_comment) = 0;
    مجازی خالی آغاز_اگلا_دھارا() = 0;
};

جماعت FileStorageParser
{
عوامی:
    مجازی ~FileStorageParser() {}
    مجازی منطقی parse(حرف* ptr) = 0;
    مجازی منطقی get_Base64_صف(حرف* ptr, صحیح indent, حرف* &beg, حرف* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#اگر_ختم // SRC_PERSISTENCE_HPP
