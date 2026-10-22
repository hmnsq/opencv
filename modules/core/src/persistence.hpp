// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#अन्तर्भाव <deque>
#अन्तर्भाव <sstream>
#अन्तर्भाव <string>
#अन्तर्भाव <iterator>

#define USE_ZLIB 1
#यदि USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#अन्यथा
प्रकारनिर्देश शून्य* gzFile;
#endif

//=====================================================================================

स्थिर अचर माप_प्रकार PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
स्थिर अचर पूर्णाङ्क CV_PERSISTENCE_MAX_DEPTH = 1024;

नामाकाश base64 {

गणना
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

अन्तःस्थ तार्किक cv_isalnum(वर्ण c)
{
    फर्काऊ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अन्तःस्थ तार्किक cv_isalpha(वर्ण c)
{
    फर्काऊ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अन्तःस्थ तार्किक cv_isdigit(वर्ण c)
{
    फर्काऊ '0' <= c && c <= '9';
}

अन्तःस्थ तार्किक cv_isspace(वर्ण c)
{
    फर्काऊ (9 <= c && c <= 13) || c == ' ';
}

अन्तःस्थ वर्ण* cv_skip_BOM(वर्ण* ptr)
{
    यदि((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      फर्काऊ ptr + 3;
    }
    फर्काऊ ptr;
}

नामाकाश cv
{
नामाकाश fs
{
पूर्णाङ्क strcasecmp(अचर वर्ण* str1, अचर वर्ण* str2);
वर्ण* itoa( पूर्णाङ्क _val, वर्ण* buffer, पूर्णाङ्क /*radix*/ );
वर्ण* itoa( int64_t _val, वर्ण* buffer, पूर्णाङ्क /*radix*/, तार्किक _signed );
वर्ण* floatToString( वर्ण* buf, माप_प्रकार bufSize, भिन्न value, तार्किक halfprecision, तार्किक explicitZero );
वर्ण* doubleToString( वर्ण* buf, माप_प्रकार bufSize, द्विगुण value, तार्किक explicitZero );

पूर्णाङ्क calcStructSize( अचर वर्ण* dt, पूर्णाङ्क initial_size );
पूर्णाङ्क calcElemSize( अचर वर्ण* dt, पूर्णाङ्क initial_size );
CV_DEPRECATED वर्ण* encodeFormat( पूर्णाङ्क elem_type, वर्ण* dt );
वर्ण* encodeFormat( पूर्णाङ्क elem_type, वर्ण* dt, माप_प्रकार dt_len );
पूर्णाङ्क decodeFormat( अचर वर्ण* dt, पूर्णाङ्क* fmt_pairs, पूर्णाङ्क max_len );
पूर्णाङ्क decodeSimpleFormat( अचर वर्ण* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) गर { (शून्य)fs; पात(); } जबसम्म (0)
#अन्यथा
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() गर { \
    CV_DbgAssert(ptr); \
    यदि((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} जबसम्म (0)


वर्ग FileStorageParser;
वर्ग FileStorageEmitter;

संरचना FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( अचर std::string& _struct_tag,
                 पूर्णाङ्क _struct_flags, पूर्णाङ्क _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    पूर्णाङ्क flags;
    पूर्णाङ्क indent;
};

वर्ग FileStorage_API
{
सार्वजनिक:
    आभास ~FileStorage_API();
    आभास FileStorage* getFS() = 0;

    आभास पूर्णाङ्क getFormat() अचर = 0;

    आभास शून्य माला_दो( अचर वर्ण* str ) = 0;
    आभास वर्ण* माला_लो() = 0;
    आभास तार्किक eof() = 0;
    आभास शून्य setEof() = 0;
    आभास शून्य closeFile() = 0;
    आभास शून्य शुरुआत() = 0;
    आभास वर्ण* resizeWriteBuffer( वर्ण* ptr, पूर्णाङ्क len ) = 0;
    आभास वर्ण* bufferPtr() अचर = 0;
    आभास वर्ण* bufferStart() अचर = 0;
    आभास वर्ण* bufferEnd() अचर = 0;
    आभास शून्य setBufferPtr(वर्ण* ptr) = 0;
    आभास वर्ण* flush() = 0;
    आभास शून्य setNonEmpty() = 0;
    आभास पूर्णाङ्क wrapMargin() अचर = 0;

    आभास FStructData& getCurrentStruct() = 0;

    आभास शून्य convertToCollection( पूर्णाङ्क type, File_गाँठो& गाँठो ) = 0;
    आभास File_गाँठो add_गाँठो( File_गाँठो& collection, अचर std::string& साँचो,
                               पूर्णाङ्क type, अचर शून्य* value=0, पूर्णाङ्क len=-1 ) = 0;
    आभास शून्य finalizeCollection( File_गाँठो& collection ) = 0;
    आभास द्विगुण म_से_भग्न(वर्ण* ptr, वर्ण** endptr) = 0;

    आभास वर्ण* parseBase64(वर्ण* ptr, पूर्णाङ्क indent, File_गाँठो& collection) = 0;
    CV_NORETURN
    आभास शून्य parseError(अचर वर्ण* funcname, अचर std::string& सन्देश,
                            अचर वर्ण* filename, पूर्णाङ्क lineno) = 0;

गुप्त:
    गणना Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    मित्र वर्ग cv::FileStorage::Impl;
    मित्र वर्ग cv::FileStorage;
    मित्र वर्ग JSONEmitter;
    मित्र वर्ग XMLEmitter;
    मित्र वर्ग YAMLEmitter;

    आभास शून्य check_if_write_struct_is_delayed(तार्किक change_type_to_base64 = असत्य) = 0;
    आभास शून्य switch_to_Base64_state(Base64State state) = 0;
    आभास Base64State get_state_of_writing_base64() = 0;
    आभास पूर्णाङ्क get_space() = 0;
};

वर्ग FileStorageEmitter
{
सार्वजनिक:
    आभास ~FileStorageEmitter() {}

    आभास FStructData startWriteStruct( अचर FStructData& parent, अचर वर्ण* साँचो,
                                          पूर्णाङ्क struct_flags, अचर वर्ण* type_name=0 ) = 0;
    आभास शून्य endWriteStruct(अचर FStructData& current_struct) = 0;
    आभास शून्य लिख(अचर वर्ण* साँचो, पूर्णाङ्क value) = 0;
    आभास शून्य लिख(अचर वर्ण* साँचो, int64_t value) = 0;
    आभास शून्य लिख(अचर वर्ण* साँचो, द्विगुण value) = 0;
    आभास शून्य लिख(अचर वर्ण* साँचो, अचर वर्ण* value, तार्किक quote) = 0;
    आभास शून्य writeScalar(अचर वर्ण* साँचो, अचर वर्ण* value) = 0;
    आभास शून्य writeComment(अचर वर्ण* comment, तार्किक eol_comment) = 0;
    आभास शून्य startNextStream() = 0;
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

#endif // SRC_PERSISTENCE_HPP
