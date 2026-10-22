// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#నిర్వచితం_కాకపోతే SRC_PERSISTENCE_HPP
#నిర్వచించు SRC_PERSISTENCE_HPP

#అన్తర్భావ <deque>
#అన్తర్భావ <sstream>
#అన్తర్భావ <string>
#అన్తర్భావ <iterator>

#నిర్వచించు USE_ZLIB 1
#ఒకవేళ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_స్థానభ్రంశం_బిట్లు
#    define FILE_స్థానభ్రంశం_బిట్లు 0
#  endif
#  include <zlib.h>
#లేకపోతే
రకనిర్వచనం శూన్య* gzFile;
#అయితే_ముగింపు

//=====================================================================================

స్థిర స్థిరరాశి size_t PARSER_BASE64_బఫరు_కొలత = 1024U * 1024U / 8U;
స్థిర స్థిరరాశి పూర్ణాంకం CV_PERSISTENCE_MAX_లోతు = 1024;

నామాకాశ base64 {

గణన
{
    శీర్షిక_కొలత         = 24,
    ENCODED_శీర్షిక_కొలత = 32
};

} // base64::

//=====================================================================================

#నిర్వచించు CV_FS_MAX_LEN 4096
#నిర్వచించు CV_FS_MAX_FMT_జతలు  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#నిర్వచించు cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#నిర్వచించు cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

అంతఃవరుస తార్కిక cv_isalnum(అక్షరం c)
{
    తిరిగి ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

అంతఃవరుస తార్కిక cv_isalpha(అక్షరం c)
{
    తిరిగి ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

అంతఃవరుస తార్కిక cv_isdigit(అక్షరం c)
{
    తిరిగి '0' <= c && c <= '9';
}

అంతఃవరుస తార్కిక cv_isspace(అక్షరం c)
{
    తిరిగి (9 <= c && c <= 13) || c == ' ';
}

అంతఃవరుస అక్షరం* cv_skip_BOM(అక్షరం* ptr)
{
    ఒకవేళ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      తిరిగి ptr + 3;
    }
    తిరిగి ptr;
}

నామాకాశ cv
{
నామాకాశ fs
{
పూర్ణాంకం strcasecmp(స్థిరరాశి అక్షరం* str1, స్థిరరాశి అక్షరం* str2);
అక్షరం* itoa( పూర్ణాంకం _val, అక్షరం* బఫరు, పూర్ణాంకం /*radix*/ );
అక్షరం* itoa( int64_t _val, అక్షరం* బఫరు, పూర్ణాంకం /*radix*/, తార్కిక _signed );
అక్షరం* float_గమ్యం_String( అక్షరం* buf, size_t buf_కొలత, దశాంశం value, తార్కిక halfprecision, తార్కిక explicitZero );
అక్షరం* double_గమ్యం_String( అక్షరం* buf, size_t buf_కొలత, ద్విగుణ value, తార్కిక explicitZero );

పూర్ణాంకం calc_Struct_కొలత( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం initial_కొలత );
పూర్ణాంకం calc_Elem_కొలత( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం initial_కొలత );
CV_DEPRECATED అక్షరం* encodeFormat( పూర్ణాంకం elem_type, అక్షరం* dt );
అక్షరం* encodeFormat( పూర్ణాంకం elem_type, అక్షరం* dt, size_t dt_len );
పూర్ణాంకం decodeFormat( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం* fmt_జతలు, పూర్ణాంకం max_len );
పూర్ణాంకం decodeSimpleFormat( స్థిరరాశి అక్షరం* dt );
}


#నిర్వచితమైతే CV_STATIC_ANALYSIS
#నిర్వచించు CV_PARSE_ERROR_CPP(errmsg) చేయి { (శూన్య)fs; abort(); } అయితే (0)
#లేకపోతే
#నిర్వచించు CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#అయితే_ముగింపు


#నిర్వచించు CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP() చేయి { \
    CV_DbgAssert(ptr); \
    ఒకవేళ((ptr)[0] == 0 && (ptr) == fs->బఫరు_చివర_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} అయితే (0)


తరగతి FileStorageParser;
తరగతి FileStorageEmitter;

నిర్మాణం FStruct_దత్తాంశం
{
    FStruct_దత్తాంశం() { indent = flags = 0; }
    FStruct_దత్తాంశం( స్థిరరాశి std::string& struct_ట్యాగ్,
                 పూర్ణాంకం _struct_flags, పూర్ణాంకం _struct_indent )
    {
        ట్యాగ్ = struct_ట్యాగ్;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string ట్యాగ్;
    పూర్ణాంకం flags;
    పూర్ణాంకం indent;
};

తరగతి FileStorage_API
{
బహిరంగ:
    మాయా ~FileStorage_API();
    మాయా FileStorage* getFS() = 0;

    మాయా పూర్ణాంకం getFormat() స్థిరరాశి = 0;

    మాయా శూన్య puts( స్థిరరాశి అక్షరం* str ) = 0;
    మాయా అక్షరం* gets() = 0;
    మాయా తార్కిక eof() = 0;
    మాయా శూన్య సమితి_Eof() = 0;
    మాయా శూన్య closeFile() = 0;
    మాయా శూన్య rewind() = 0;
    మాయా అక్షరం* resize_Write_బఫరు( అక్షరం* ptr, పూర్ణాంకం len ) = 0;
    మాయా అక్షరం* బఫరు_Ptr() స్థిరరాశి = 0;
    మాయా అక్షరం* బఫరు_ప్రారంభం() స్థిరరాశి = 0;
    మాయా అక్షరం* బఫరు_చివర_2() స్థిరరాశి = 0;
    మాయా శూన్య సమితి_బఫరు_Ptr(అక్షరం* ptr) = 0;
    మాయా అక్షరం* flush() = 0;
    మాయా శూన్య సమితి_Non_ఖాళీలు() = 0;
    మాయా పూర్ణాంకం wrapMargin() స్థిరరాశి = 0;

    మాయా FStruct_దత్తాంశం& getCurrentStruct() = 0;

    మాయా శూన్య convert_గమ్యం_Collection( పూర్ణాంకం type, File_కణుపు& కణుపు ) = 0;
    మాయా File_కణుపు కూడు_కణుపు( File_కణుపు& collection, స్థిరరాశి std::string& కీలకం,
                               పూర్ణాంకం type, స్థిరరాశి శూన్య* value=0, పూర్ణాంకం len=-1 ) = 0;
    మాయా శూన్య finalizeCollection( File_కణుపు& collection ) = 0;
    మాయా ద్విగుణ strtod(అక్షరం* ptr, అక్షరం** endptr) = 0;

    మాయా అక్షరం* parseBase64(అక్షరం* ptr, పూర్ణాంకం indent, File_కణుపు& collection) = 0;
    CV_NORETURN
    మాయా శూన్య parseError(స్థిరరాశి అక్షరం* funcname, స్థిరరాశి std::string& సందేశం,
                            స్థిరరాశి అక్షరం* filename, పూర్ణాంకం lineno) = 0;

అంతరంగ:
    గణన Base64State{
        Uncertain,
        NotUse,
        లోపల_Use,
    };

    మిత్ర తరగతి cv::FileStorage::Impl;
    మిత్ర తరగతి cv::FileStorage;
    మిత్ర తరగతి JSONEmitter;
    మిత్ర తరగతి XMLEmitter;
    మిత్ర తరగతి YAMLEmitter;

    మాయా శూన్య check_if_write_struct_is_delayed(తార్కిక change_type_గమ్యం_base64 = అబద్ధం) = 0;
    మాయా శూన్య switch_గమ్యం_Base64_state(Base64State state) = 0;
    మాయా Base64State get_state_యొక్క_writing_base64() = 0;
    మాయా పూర్ణాంకం get_space() = 0;
};

తరగతి FileStorageEmitter
{
బహిరంగ:
    మాయా ~FileStorageEmitter() {}

    మాయా FStruct_దత్తాంశం ప్రారంభం_Write_Struct( స్థిరరాశి FStruct_దత్తాంశం& మాతృ, స్థిరరాశి అక్షరం* కీలకం,
                                          పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం* type_name=0 ) = 0;
    మాయా శూన్య చివర_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& current_struct) = 0;
    మాయా శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం value) = 0;
    మాయా శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, int64_t value) = 0;
    మాయా శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, ద్విగుణ value) = 0;
    మాయా శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* value, తార్కిక quote) = 0;
    మాయా శూన్య writeScalar(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* value) = 0;
    మాయా శూన్య writeComment(స్థిరరాశి అక్షరం* comment, తార్కిక eol_comment) = 0;
    మాయా శూన్య ప్రారంభం_తదుపరి_ప్రవాహం() = 0;
};

తరగతి FileStorageParser
{
బహిరంగ:
    మాయా ~FileStorageParser() {}
    మాయా తార్కిక parse(అక్షరం* ptr) = 0;
    మాయా తార్కిక get_Base64_అడ్డువరుస(అక్షరం* ptr, పూర్ణాంకం indent, అక్షరం* &beg, అక్షరం* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#అయితే_ముగింపు // SRC_PERSISTENCE_HPP
