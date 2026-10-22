// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#यदि_अपरिभाषितम् SRC_PERSISTENCE_HPP
#परिभाषय SRC_PERSISTENCE_HPP

#अन्तर्भाव <deque>
#अन्तर्भाव <sstream>
#अन्तर्भाव <string>
#अन्तर्भाव <iterator>

#परिभाषय USE_ZLIB 1
#यदि USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_विस्थापनम्_द्वयङ्काः
#    define FILE_विस्थापनम्_द्वयङ्काः 0
#  endif
#  include <zlib.h>
#अन्यथा
प्रकारनिर्देश शून्य* gzFile;
#यदि_अन्तः

//=====================================================================================

स्थिर ध्रुव size_t PARSER_BASE64_बफर_आकार = 1024U * 1024U / 8U;
स्थिर ध्रुव पूर्णाङ्क CV_PERSISTENCE_MAX_गभीरता = 1024;

नामाकाश base64 {

गणना
{
    शीर्षकम्_आकार         = 24,
    ENCODED_शीर्षकम्_आकार = 32
};

} // base64::

//=====================================================================================

#परिभाषय CV_FS_MAX_LEN 4096
#परिभाषय CV_FS_MAX_FMT_युग्मानि  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#परिभाषय cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#परिभाषय cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

अन्तःस्थ तार्किक cv_isalnum(वर्ण c)
{
    प्रत्यागम ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अन्तःस्थ तार्किक cv_isalpha(वर्ण c)
{
    प्रत्यागम ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अन्तःस्थ तार्किक cv_isdigit(वर्ण c)
{
    प्रत्यागम '0' <= c && c <= '9';
}

अन्तःस्थ तार्किक cv_isspace(वर्ण c)
{
    प्रत्यागम (9 <= c && c <= 13) || c == ' ';
}

अन्तःस्थ वर्ण* cv_skip_BOM(वर्ण* ptr)
{
    यदि((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      प्रत्यागम ptr + 3;
    }
    प्रत्यागम ptr;
}

नामाकाश cv
{
नामाकाश fs
{
पूर्णाङ्क strcasecmp(ध्रुव वर्ण* str1, ध्रुव वर्ण* str2);
वर्ण* itoa( पूर्णाङ्क _val, वर्ण* बफर, पूर्णाङ्क /*radix*/ );
वर्ण* itoa( int64_t _val, वर्ण* बफर, पूर्णाङ्क /*radix*/, तार्किक _signed );
वर्ण* float_गन्तव्यम्_String( वर्ण* buf, size_t buf_आकार, भिन्न value, तार्किक halfprecision, तार्किक explicitZero );
वर्ण* double_गन्तव्यम्_String( वर्ण* buf, size_t buf_आकार, द्विगुण value, तार्किक explicitZero );

पूर्णाङ्क calc_Struct_आकार( ध्रुव वर्ण* dt, पूर्णाङ्क initial_आकार );
पूर्णाङ्क calc_Elem_आकार( ध्रुव वर्ण* dt, पूर्णाङ्क initial_आकार );
CV_DEPRECATED वर्ण* encodeFormat( पूर्णाङ्क elem_type, वर्ण* dt );
वर्ण* encodeFormat( पूर्णाङ्क elem_type, वर्ण* dt, size_t dt_len );
पूर्णाङ्क decodeFormat( ध्रुव वर्ण* dt, पूर्णाङ्क* fmt_युग्मानि, पूर्णाङ्क max_len );
पूर्णाङ्क decodeSimpleFormat( ध्रुव वर्ण* dt );
}


#यदि_परिभाषितम् CV_STATIC_ANALYSIS
#परिभाषय CV_PARSE_ERROR_CPP(errmsg) कुरु { (शून्य)fs; abort(); } यावत् (0)
#अन्यथा
#परिभाषय CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#यदि_अन्तः


#परिभाषय CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP() कुरु { \
    CV_DbgAssert(ptr); \
    यदि((ptr)[0] == 0 && (ptr) == fs->बफर_अन्तः_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} यावत् (0)


वर्ग FileStorageParser;
वर्ग FileStorageEmitter;

संरचना FStruct_दत्त
{
    FStruct_दत्त() { indent = flags = 0; }
    FStruct_दत्त( ध्रुव std::string& struct_चिह्नकम्,
                 पूर्णाङ्क _struct_flags, पूर्णाङ्क _struct_indent )
    {
        चिह्नकम् = struct_चिह्नकम्;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string चिह्नकम्;
    पूर्णाङ्क flags;
    पूर्णाङ्क indent;
};

वर्ग FileStorage_API
{
सार्वजनिक:
    आभास ~FileStorage_API();
    आभास FileStorage* getFS() = 0;

    आभास पूर्णाङ्क getFormat() ध्रुव = 0;

    आभास शून्य पङ्क्तिं_लिख( ध्रुव वर्ण* str ) = 0;
    आभास वर्ण* gets() = 0;
    आभास तार्किक eof() = 0;
    आभास शून्य समुच्चयः_Eof() = 0;
    आभास शून्य closeFile() = 0;
    आभास शून्य rewind() = 0;
    आभास वर्ण* resize_Write_बफर( वर्ण* ptr, पूर्णाङ्क len ) = 0;
    आभास वर्ण* बफर_Ptr() ध्रुव = 0;
    आभास वर्ण* बफर_Start() ध्रुव = 0;
    आभास वर्ण* बफर_अन्तः_2() ध्रुव = 0;
    आभास शून्य समुच्चयः_बफर_Ptr(वर्ण* ptr) = 0;
    आभास वर्ण* flush() = 0;
    आभास शून्य समुच्चयः_Non_रिक्तानि() = 0;
    आभास पूर्णाङ्क wrapMargin() ध्रुव = 0;

    आभास FStruct_दत्त& getCurrentStruct() = 0;

    आभास शून्य convert_गन्तव्यम्_Collection( पूर्णाङ्क type, File_ग्रन्थिः& ग्रन्थिः ) = 0;
    आभास File_ग्रन्थिः सङ्कलय_ग्रन्थिः( File_ग्रन्थिः& collection, ध्रुव std::string& कुञ्चिका,
                               पूर्णाङ्क type, ध्रुव शून्य* value=0, पूर्णाङ्क len=-1 ) = 0;
    आभास शून्य finalizeCollection( File_ग्रन्थिः& collection ) = 0;
    आभास द्विगुण strtod(वर्ण* ptr, वर्ण** endptr) = 0;

    आभास वर्ण* parseBase64(वर्ण* ptr, पूर्णाङ्क indent, File_ग्रन्थिः& collection) = 0;
    CV_NORETURN
    आभास शून्य parseError(ध्रुव वर्ण* funcname, ध्रुव std::string& सन्देशः,
                            ध्रुव वर्ण* filename, पूर्णाङ्क lineno) = 0;

गुप्त:
    गणना Base64State{
        Uncertain,
        NotUse,
        अन्तर्_Use,
    };

    मित्रम् वर्ग cv::FileStorage::Impl;
    मित्रम् वर्ग cv::FileStorage;
    मित्रम् वर्ग JSONEmitter;
    मित्रम् वर्ग XMLEmitter;
    मित्रम् वर्ग YAMLEmitter;

    आभास शून्य check_if_write_struct_is_delayed(तार्किक change_type_गन्तव्यम्_base64 = असत्य) = 0;
    आभास शून्य switch_गन्तव्यम्_Base64_state(Base64State state) = 0;
    आभास Base64State get_state_सम्बन्धि_writing_base64() = 0;
    आभास पूर्णाङ्क get_space() = 0;
};

वर्ग FileStorageEmitter
{
सार्वजनिक:
    आभास ~FileStorageEmitter() {}

    आभास FStruct_दत्त startWriteStruct( ध्रुव FStruct_दत्त& parent, ध्रुव वर्ण* कुञ्चिका,
                                          पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0 ) = 0;
    आभास शून्य अन्तः_Write_Struct(ध्रुव FStruct_दत्त& current_struct) = 0;
    आभास शून्य लिख(ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क value) = 0;
    आभास शून्य लिख(ध्रुव वर्ण* कुञ्चिका, int64_t value) = 0;
    आभास शून्य लिख(ध्रुव वर्ण* कुञ्चिका, द्विगुण value) = 0;
    आभास शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* value, तार्किक quote) = 0;
    आभास शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* value) = 0;
    आभास शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment) = 0;
    आभास शून्य start_अग्र_प्रवाहः() = 0;
};

वर्ग FileStorageParser
{
सार्वजनिक:
    आभास ~FileStorageParser() {}
    आभास तार्किक parse(वर्ण* ptr) = 0;
    आभास तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क indent, वर्ण* &beg, वर्ण* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#यदि_अन्तः // SRC_PERSISTENCE_HPP
