// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "persistence.hpp"

namespace cv
{

श्रेणी JSONEmitter : खुला FileStorageEmitter
{
खुला:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    भव ~JSONEmitter() {}

    FStruct_तथ्य startWriteStruct( स्थिर FStruct_तथ्य& parent, स्थिर अक्षर* कुंजी,
                                  पूर्णांक struct_flags, स्थिर अक्षर* type_name=0 )
    {
        अक्षर तथ्य[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_गाँठ::TYPE_MASK|File_गाँठ::FLOW)) | File_गाँठ::EMPTY;
        अगर( !File_गाँठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        अगर( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        अगर (type_name && स_भेद(type_name, "binary", 6) == 0)
        {
            struct_flags = File_गाँठ::STR;
            तथ्य[0] = '\0';
            is_real_collection = false;
        }

        अगर ( is_real_collection )
        {
            अक्षर c = File_गाँठ::isMap(struct_flags) ? '{' : '[';
            तथ्य[0] = c;
            तथ्य[1] = '\0';
        }

        writeScalar( कुंजी, तथ्य );
        FStruct_तथ्य current_struct("", struct_flags, parent.indent + 4);

        वापस current_struct;
    }

    व्योम endWriteStruct(स्थिर FStruct_तथ्य& current_struct)
    {
        पूर्णांक struct_flags = current_struct.flags;

        अगर (File_गाँठ::isCollection(struct_flags)) {
            अगर (!File_गाँठ::isFlow(struct_flags)) {
                अगर (fs->बफर_Ptr() <= fs->बफर_Start() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    अक्षर *ptr = fs->बफर_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->माला_दो(fs->बफर_Start());
                    fs->set_बफर_Ptr(fs->बफर_Start());
                }
                fs->सफाई();
            }

            अक्षर *ptr = fs->बफर_Ptr();
            अगर (ptr > fs->बफर_Start() + current_struct.indent && !File_गाँठ::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_गाँठ::isMap(struct_flags) ? '}' : ']';
            fs->set_बफर_Ptr(ptr);
        }
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, पूर्णांक value)
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10 ));
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, int64_t value)
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10, true ));
    }

    व्योम ग_लिखो( स्थिर अक्षर* कुंजी, द्विगुन value )
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::doubleToString( buf, माप(buf), value, true ));
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, स्थिर अक्षर* str, bool quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*6+16];
        अक्षर* तथ्य = (अक्षर*)str;
        पूर्णांक i, len;

        अगर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        अगर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        अगर( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णांक need_quote = 1;
            तथ्य = buf;
            *तथ्य++ = '\"';
            क्रम( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];
                स्थिर अचिन्हित अक्षर byte = static_cast<अचिन्हित अक्षर>(c);

                चयन ( c )
                {
                हाल '\\':
                हाल '\"': { *तथ्य++ = '\\'; *तथ्य++ = c;   अवरोध; }
                हाल '\n': { *तथ्य++ = '\\'; *तथ्य++ = 'n'; अवरोध; }
                हाल '\r': { *तथ्य++ = '\\'; *तथ्य++ = 'r'; अवरोध; }
                हाल '\t': { *तथ्य++ = '\\'; *तथ्य++ = 't'; अवरोध; }
                हाल '\b': { *तथ्य++ = '\\'; *तथ्य++ = 'b'; अवरोध; }
                हाल '\f': { *तथ्य++ = '\\'; *तथ्य++ = 'f'; अवरोध; }
                शेष  :
                    अगर (byte < 0x20)
                    {
                        अटल स्थिर अक्षर षष्ठ[] = "0123456789abcdef";
                        *तथ्य++ = '\\'; *तथ्य++ = 'u'; *तथ्य++ = '0'; *तथ्य++ = '0';
                        *तथ्य++ = षष्ठ[byte >> 4]; *तथ्य++ = षष्ठ[byte & 0xf];
                    }
                    अन्यथा
                        *तथ्य++ = c;
                    अवरोध;
                }
            }

            *तथ्य++ = '\"';
            *तथ्य++ = '\0';
            तथ्य = buf + !need_quote;
        }

        writeScalar( कुंजी, तथ्य);
    }

    व्योम writeScalar(स्थिर अक्षर* कुंजी, स्थिर अक्षर* तथ्य)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(false);
        अगर ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा अगर ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        माप_प्रकार कुंजी_len = 0u;
        अगर( कुंजी && *कुंजी == '\0' )
            कुंजी = 0;
        अगर ( कुंजी )
        {
            कुंजी_len = म_माप(कुंजी);
            अगर ( कुंजी_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            अन्यथा अगर ( static_cast<पूर्णांक>(कुंजी_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        माप_प्रकार तथ्य_len = 0u;
        अगर ( तथ्य )
            तथ्य_len = म_माप(तथ्य);

        FStruct_तथ्य& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;
        अगर( File_गाँठ::isCollection(struct_flags) )
        {
            अगर ( (File_गाँठ::isMap(struct_flags) ^ (कुंजी != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } अन्यथा {
            fs->setNonEmpty();
            struct_flags = File_गाँठ::EMPTY | (कुंजी ? File_गाँठ::MAP : File_गाँठ::SEQ);
        }

        // start to write
        अक्षर* ptr = 0;

        अगर( File_गाँठ::isFlow(struct_flags) )
        {
            पूर्णांक new_offset;
            ptr = fs->बफर_Ptr();
            अगर( !File_गाँठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<पूर्णांक>(ptr - fs->बफर_Start() + कुंजी_len + तथ्य_len);
            अगर( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->set_बफर_Ptr(ptr);
                ptr = fs->सफाई();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            अगर ( !File_गाँठ::isEmptyCollection(struct_flags) )
            {
                ptr = fs->बफर_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->माला_दो( fs->बफर_Start() );
                fs->set_बफर_Ptr(fs->बफर_Start());
            }
            ptr = fs->सफाई();
        }

        अगर( कुंजी )
        {
            अगर( !cv_isalpha(कुंजी[0]) && कुंजी[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_बफर( ptr, static_cast<पूर्णांक>(कुंजी_len) );
            *ptr++ = '\"';

            क्रम( माप_प्रकार i = 0u; i < कुंजी_len; i++ )
            {
                अक्षर c = कुंजी[i];

                ptr[i] = c;
                अगर( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += कुंजी_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        अगर( तथ्य )
        {
            ptr = fs->resize_Write_बफर( ptr, static_cast<पूर्णांक>(तथ्य_len) );
            स_नकल( ptr, तथ्य, तथ्य_len );
            ptr += तथ्य_len;
        }

        fs->set_बफर_Ptr(ptr);
        current_struct.flags &= ~File_गाँठ::EMPTY;
    }

    व्योम writeComment(स्थिर अक्षर* comment, bool eol_comment)
    {
        अगर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णांक len = static_cast<पूर्णांक>(म_माप(comment));
        अक्षर* ptr = fs->बफर_Ptr();
        स्थिर अक्षर* eol = म_अक्षर(comment, '\n');
        bool multiline = eol != 0;

        अगर( !eol_comment || multiline || fs->बफर_End() - ptr < len || ptr == fs->बफर_Start() )
            ptr = fs->सफाई();
        अन्यथा
            *ptr++ = ' ';

        जबतक( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            अगर( eol )
            {
                ptr = fs->resize_Write_बफर( ptr, (पूर्णांक)(eol - comment) + 1 );
                स_नकल( ptr, comment, eol - comment + 1 );
                fs->set_बफर_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = म_अक्षर( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णांक)म_माप(comment);
                ptr = fs->resize_Write_बफर( ptr, len );
                स_नकल( ptr, comment, len );
                fs->set_बफर_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->सफाई();
        }
    }

    व्योम start_अग्र_Stream()
    {
        fs->माला_दो( "...\n" );
        fs->माला_दो( "---\n" );
    }

रक्षित:
    FileStorage_API* fs;
};

श्रेणी JSONParser : खुला FileStorageParser
{
खुला:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    भव ~JSONParser() {}

    अक्षर* skipSpaces( अक्षर* ptr )
    {
        bool is_eof = false;
        bool is_completed = false;

        जबतक ( is_eof == false && is_completed == false )
        {
            अगर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            चयन ( *ptr )
            {
                /* comment */
                हाल '/' : {
                    ptr++;
                    अगर ( *ptr == '\0' )
                    {
                        ptr = fs->माला_लो();
                        अगर( !ptr || !*ptr ) { is_eof = true; अवरोध; }
                    }

                    अगर ( *ptr == '/' )
                    {
                        जबतक ( *ptr != '\n' && *ptr != '\r' )
                        {
                            अगर ( *ptr == '\0' )
                            {
                                ptr = fs->माला_लो();
                                अगर( !ptr || !*ptr ) { is_eof = true; अवरोध; }
                            }
                            अन्यथा
                            {
                                ptr++;
                            }
                        }
                    }
                    अन्यथा अगर ( *ptr == '*' )
                    {
                        ptr++;
                        क्रम (;;)
                        {
                            अगर ( *ptr == '\0' )
                            {
                                ptr = fs->माला_लो();
                                अगर( !ptr || !*ptr ) { is_eof = true; अवरोध; }
                            }
                            अन्यथा अगर ( *ptr == '*' )
                            {
                                ptr++;
                                अगर ( *ptr == '\0' )
                                {
                                    ptr = fs->माला_लो();
                                    अगर( !ptr || !*ptr ) { is_eof = true; अवरोध; }
                                }
                                अगर ( *ptr == '/' )
                                {
                                    ptr++;
                                    अवरोध;
                                }
                            }
                            अन्यथा
                            {
                                ptr++;
                            }
                        }
                    }
                    अन्यथा
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } अवरोध;
                    /* whitespace */
                हाल '\t':
                हाल ' ' : {
                    ptr++;
                } अवरोध;
                    /* newline || end mark */
                हाल '\0':
                हाल '\n':
                हाल '\r': {
                    ptr = fs->माला_लो();
                    अगर( !ptr || !*ptr ) { is_eof = true; अवरोध; }
                } अवरोध;
                    /* other character */
                शेष: {
                    अगर( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } अवरोध;
            }
        }

        अगर ( is_eof || !is_completed )
        {
            ptr = fs->बफर_Start();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            अगर( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        वापस ptr;
    }

    uint32_t parseUnicodeEscape(अक्षर*& ptr)
    {
        uint32_t codepoint = 0;
        क्रम (पूर्णांक k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
            अक्षर षष्ठ = *ptr;
            uint32_t digit = 0;
            अगर      (षष्ठ >= '0' && षष्ठ <= '9') digit = (uint32_t)(षष्ठ - '0');
            अन्यथा अगर (षष्ठ >= 'a' && षष्ठ <= 'f') digit = (uint32_t)(षष्ठ - 'a') + 10u;
            अन्यथा अगर (षष्ठ >= 'A' && षष्ठ <= 'F') digit = (uint32_t)(षष्ठ - 'A') + 10u;
            अन्यथा CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        वापस codepoint;
    }

    अटल व्योम appendUtf8(std::string& out, uint32_t codepoint)
    {
        अगर (codepoint < 0x80) {
            out += (अक्षर)codepoint;
        } अन्यथा अगर (codepoint < 0x800) {
            out += (अक्षर)(0xC0 | (codepoint >> 6));
            out += (अक्षर)(0x80 | (codepoint & 0x3F));
        } अन्यथा अगर (codepoint < 0x10000) {
            out += (अक्षर)(0xE0 | (codepoint >> 12));
            out += (अक्षर)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (अक्षर)(0x80 | (codepoint & 0x3F));
        } अन्यथा {
            out += (अक्षर)(0xF0 | (codepoint >> 18));
            out += (अक्षर)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (अक्षर)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (अक्षर)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    व्योम parseUnicodeEscapeToUtf8(अक्षर*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        अगर (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            अगर (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t निचला = parseUnicodeEscape(ptr);
            अगर (निचला < 0xDC00 || निचला > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (निचला - 0xDC00u);
        }
        अन्यथा अगर (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    अक्षर* parse_कुंजी( अक्षर* ptr, File_गाँठ& collection, File_गाँठ& मूल्य_placeholder )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अगर( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        अक्षर * beg = ptr + 1;
        std::string कुंजी_name;
        करो {
            अगर (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
                अगर (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, कुंजी_name);
                    जारी;
                }
                कुंजी_name += *ptr;
            } अन्यथा अगर (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                कुंजी_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
        } जबतक( cv_isprint(*ptr) && *ptr != '"' );

        अगर( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        अगर( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        मूल्य_placeholder = fs->add_गाँठ(collection, कुंजी_name, File_गाँठ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        अगर( !ptr || !*ptr )
            वापस 0;

        अगर( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        वापस ++ptr;
    }

    bool getBase64Row(अक्षर* ptr, पूर्णांक /*indent*/, अक्षर* &beg, अक्षर* &end)
    {
        beg = end = ptr;
        अगर( !ptr || !*ptr )
            वापस false;

        // find end of the row
        जबतक( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        अगर ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        वापस true;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाँठ& गाँठ )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        अगर( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        अगर( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            अक्षर * beg = ptr;
            माप_प्रकार len = 0u;
            क्रम ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            अगर ((len >= 8u) && (स_भेद( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, गाँठ);

                अगर ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;
            }
            अन्यथा
            {   /**************** normal string ****************/
                पूर्णांक i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                जबतक ( !is_matching )
                {
                    चयन ( *ptr )
                    {
                        हाल '\\':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            अगर( sz > 0 )
                            {
                                अगर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            अगर (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            चयन ( *ptr )
                            {
                            हाल '\\':
                            हाल '\"':
                            हाल '\'': { buf[i++] = *ptr; अवरोध; }
                            हाल 'n' : { buf[i++] = '\n'; अवरोध; }
                            हाल 'r' : { buf[i++] = '\r'; अवरोध; }
                            हाल 't' : { buf[i++] = '\t'; अवरोध; }
                            हाल 'b' : { buf[i++] = '\b'; अवरोध; }
                            हाल 'f' : { buf[i++] = '\f'; अवरोध; }
                            हाल 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                अगर (i + (पूर्णांक)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, utf8.तथ्य(), utf8.size());
                                i += (पूर्णांक)utf8.size();
                                beg = ptr;
                                जारी;
                            }
                            शेष  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            अवरोध;
                            }
                            ptr++;
                            beg = ptr;
                            अवरोध;
                        }
                        हाल '\0':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            अगर( sz > 0 )
                            {
                                अगर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->माला_लो();
                            अगर ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            अवरोध;
                        }
                        हाल '\"':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            अगर( sz > 0 )
                            {
                                अगर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = true;
                            अवरोध;
                        }
                        हाल '\n':
                        हाल '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            अवरोध;
                        }
                        शेष:
                        {
                            ptr++;
                            अवरोध;
                        }
                    }
                }

                अगर ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;

                गाँठ.set_मूल्य(File_गाँठ::STRING, buf, i);
            }
        }
        अन्यथा अगर ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            अक्षर * beg = ptr;
            अगर ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
            }
            जबतक( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
            }
            अगर (*ptr == '.' || *ptr == 'e')
            {
                द्विगुन fval = fs->म_से_भग्न( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();

                गाँठ.set_मूल्य(File_गाँठ::REAL, &fval);
            }
            अन्यथा
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();

                गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
            }

            अगर ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        अन्यथा
        {    /**************** other data ****************/
            स्थिर अक्षर* beg = ptr;
            पूर्णांक len = 0;
            क्रम ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
            }

            अगर( len == 4 && स_भेद( beg, "null", 4 ) == 0 )
                ;
            अन्यथा अगर( (len == 4 && स_भेद( beg, "true", 4 ) == 0) ||
                     (len == 5 && स_भेद( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
            }
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        वापस ptr;
    }

    अक्षर* parseSeq( अक्षर* ptr, File_गाँठ& गाँठ, पूर्णांक depth = 0 )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        अगर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        अगर ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाँठ::SEQ, गाँठ);

        क्रम (;;)
        {
            ptr = skipSpaces( ptr );
            अगर( !ptr || !*ptr )
                अवरोध;

            अगर ( *ptr != ']' )
            {
                File_गाँठ child = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE );

                अगर ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा अगर ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
            }

            ptr = skipSpaces( ptr );
            अगर( !ptr || !*ptr )
                अवरोध;

            अगर ( *ptr == ',' )
                ptr++;
            अन्यथा अगर ( *ptr == ']' )
                अवरोध;
            अन्यथा
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        अगर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        अगर ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(गाँठ);
        वापस ptr;
    }

    अक्षर* parseMap( अक्षर* ptr, File_गाँठ& गाँठ, पूर्णांक depth = 0 )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        अगर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        अगर ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाँठ::MAP, गाँठ);

        क्रम( ;; )
        {
            ptr = skipSpaces( ptr );
            अगर( !ptr || !*ptr )
                अवरोध;

            अगर ( *ptr == '"' )
            {
                File_गाँठ child;
                ptr = parse_कुंजी( ptr, गाँठ, child );
                अगर( !ptr || !*ptr )
                    अवरोध;
                ptr = skipSpaces( ptr );
                अगर( !ptr || !*ptr )
                    अवरोध;

                अगर ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा अगर ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
            }

            ptr = skipSpaces( ptr );
            अगर( !ptr || !*ptr )
                अवरोध;

            अगर ( *ptr == ',' )
                ptr++;
            अन्यथा अगर ( *ptr == '}' )
                अवरोध;
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        अगर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        अगर ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(गाँठ);
        वापस ptr;
    }

    bool parse( अक्षर* ptr )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        अगर ( !ptr || !*ptr )
            वापस false;

        File_गाँठ मूल_collection(fs->getFS(), 0, 0);

        अगर( *ptr == '{' )
        {
            File_गाँठ मूल_गाँठ = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::MAP);
            parseMap( ptr, मूल_गाँठ );
        }
        अन्यथा अगर ( *ptr == '[' )
        {
            File_गाँठ मूल_गाँठ = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::SEQ);
            parseSeq( ptr, मूल_गाँठ );
        }
        अन्यथा
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        वापस true;
    }

    FileStorage_API* fs;
    अक्षर buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    वापस makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    वापस makePtr<JSONParser>(fs);
}

}
