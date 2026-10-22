// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

नामाकाश cv
{

वर्ग JSONEmitter : सार्वजनिक FileStorageEmitter
{
सार्वजनिक:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    आभासी ~JSONEmitter() {}

    FStructData startWriteStruct( स्थिरांक FStructData& parent, स्थिरांक अक्षर* किल्ली,
                                  पूर्णांक struct_flags, स्थिरांक अक्षर* type_name=0 )
    {
        अक्षर data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_गाठ::TYPE_MASK|File_गाठ::FLOW)) | File_गाठ::EMPTY;
        जर( !File_गाठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        जर( type_name && *type_name == '\0' )
            type_name = 0;

        तार्किक is_real_collection = सत्य;
        जर (type_name && स_भेद(type_name, "binary", 6) == 0)
        {
            struct_flags = File_गाठ::STR;
            data[0] = '\0';
            is_real_collection = असत्य;
        }

        जर ( is_real_collection )
        {
            अक्षर c = File_गाठ::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( किल्ली, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        परत current_struct;
    }

    शून्य endWriteStruct(स्थिरांक FStructData& current_struct)
    {
        पूर्णांक struct_flags = current_struct.flags;

        जर (File_गाठ::isCollection(struct_flags)) {
            जर (!File_गाठ::isFlow(struct_flags)) {
                जर (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    अक्षर *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->माला_दो(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            अक्षर *ptr = fs->bufferPtr();
            जर (ptr > fs->bufferStart() + current_struct.indent && !File_गाठ::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_गाठ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, पूर्णांक value)
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10 ));
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, int64_t value)
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( स्थिरांक अक्षर* किल्ली, द्विगुण value )
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::doubleToString( buf, आकार(buf), value, सत्य ));
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* str, तार्किक quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*6+16];
        अक्षर* data = (अक्षर*)str;
        पूर्णांक i, len;

        जर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        जर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        जर( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णांक need_quote = 1;
            data = buf;
            *data++ = '\"';
            साठी( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];
                स्थिरांक अचिन्हित अक्षर byte = static_cast<अचिन्हित अक्षर>(c);

                निवड ( c )
                {
                स्थिती '\\':
                स्थिती '\"': { *data++ = '\\'; *data++ = c;   थांबा; }
                स्थिती '\n': { *data++ = '\\'; *data++ = 'n'; थांबा; }
                स्थिती '\r': { *data++ = '\\'; *data++ = 'r'; थांबा; }
                स्थिती '\t': { *data++ = '\\'; *data++ = 't'; थांबा; }
                स्थिती '\b': { *data++ = '\\'; *data++ = 'b'; थांबा; }
                स्थिती '\f': { *data++ = '\\'; *data++ = 'f'; थांबा; }
                पूर्वनिर्धारित  :
                    जर (byte < 0x20)
                    {
                        स्थिर स्थिरांक अक्षर hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    अन्यथा
                        *data++ = c;
                    थांबा;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( किल्ली, data);
    }

    शून्य writeScalar(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(असत्य);
        जर ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा जर ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        माप_प्रकार किल्ली_len = 0u;
        जर( किल्ली && *किल्ली == '\0' )
            किल्ली = 0;
        जर ( किल्ली )
        {
            किल्ली_len = म_माप(किल्ली);
            जर ( किल्ली_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            अन्यथा जर ( static_cast<पूर्णांक>(किल्ली_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        माप_प्रकार data_len = 0u;
        जर ( data )
            data_len = म_माप(data);

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;
        जर( File_गाठ::isCollection(struct_flags) )
        {
            जर ( (File_गाठ::isMap(struct_flags) ^ (किल्ली != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } अन्यथा {
            fs->setNonEmpty();
            struct_flags = File_गाठ::EMPTY | (किल्ली ? File_गाठ::MAP : File_गाठ::SEQ);
        }

        // start to write
        अक्षर* ptr = 0;

        जर( File_गाठ::isFlow(struct_flags) )
        {
            पूर्णांक new_offset;
            ptr = fs->bufferPtr();
            जर( !File_गाठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<पूर्णांक>(ptr - fs->bufferStart() + किल्ली_len + data_len);
            जर( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            जर ( !File_गाठ::isEmptyCollection(struct_flags) )
            {
                ptr = fs->bufferPtr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->माला_दो( fs->bufferStart() );
                fs->setBufferPtr(fs->bufferStart());
            }
            ptr = fs->flush();
        }

        जर( किल्ली )
        {
            जर( !cv_isalpha(किल्ली[0]) && किल्ली[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णांक>(किल्ली_len) );
            *ptr++ = '\"';

            साठी( माप_प्रकार i = 0u; i < किल्ली_len; i++ )
            {
                अक्षर c = किल्ली[i];

                ptr[i] = c;
                जर( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += किल्ली_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        जर( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णांक>(data_len) );
            स_नकल( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_गाठ::EMPTY;
    }

    शून्य writeComment(स्थिरांक अक्षर* comment, तार्किक eol_comment)
    {
        जर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णांक len = static_cast<पूर्णांक>(म_माप(comment));
        अक्षर* ptr = fs->bufferPtr();
        स्थिरांक अक्षर* eol = म_अक्षर(comment, '\n');
        तार्किक multiline = eol != 0;

        जर( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        अन्यथा
            *ptr++ = ' ';

        पर्यंत( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            जर( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णांक)(eol - comment) + 1 );
                स_नकल( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = म_अक्षर( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णांक)म_माप(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                स_नकल( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    शून्य startNextStream()
    {
        fs->माला_दो( "...\n" );
        fs->माला_दो( "---\n" );
    }

संरक्षित:
    FileStorage_API* fs;
};

वर्ग JSONParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभासी ~JSONParser() {}

    अक्षर* skipSpaces( अक्षर* ptr )
    {
        तार्किक is_eof = असत्य;
        तार्किक is_completed = असत्य;

        पर्यंत ( is_eof == असत्य && is_completed == असत्य )
        {
            जर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            निवड ( *ptr )
            {
                /* comment */
                स्थिती '/' : {
                    ptr++;
                    जर ( *ptr == '\0' )
                    {
                        ptr = fs->माला_लो();
                        जर( !ptr || !*ptr ) { is_eof = सत्य; थांबा; }
                    }

                    जर ( *ptr == '/' )
                    {
                        पर्यंत ( *ptr != '\n' && *ptr != '\r' )
                        {
                            जर ( *ptr == '\0' )
                            {
                                ptr = fs->माला_लो();
                                जर( !ptr || !*ptr ) { is_eof = सत्य; थांबा; }
                            }
                            अन्यथा
                            {
                                ptr++;
                            }
                        }
                    }
                    अन्यथा जर ( *ptr == '*' )
                    {
                        ptr++;
                        साठी (;;)
                        {
                            जर ( *ptr == '\0' )
                            {
                                ptr = fs->माला_लो();
                                जर( !ptr || !*ptr ) { is_eof = सत्य; थांबा; }
                            }
                            अन्यथा जर ( *ptr == '*' )
                            {
                                ptr++;
                                जर ( *ptr == '\0' )
                                {
                                    ptr = fs->माला_लो();
                                    जर( !ptr || !*ptr ) { is_eof = सत्य; थांबा; }
                                }
                                जर ( *ptr == '/' )
                                {
                                    ptr++;
                                    थांबा;
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
                } थांबा;
                    /* whitespace */
                स्थिती '\t':
                स्थिती ' ' : {
                    ptr++;
                } थांबा;
                    /* newline || end mark */
                स्थिती '\0':
                स्थिती '\n':
                स्थिती '\r': {
                    ptr = fs->माला_लो();
                    जर( !ptr || !*ptr ) { is_eof = सत्य; थांबा; }
                } थांबा;
                    /* other character */
                पूर्वनिर्धारित: {
                    जर( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = सत्य;
                } थांबा;
            }
        }

        जर ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            जर( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        परत ptr;
    }

    uint32_t parseUnicodeEscape(अक्षर*& ptr)
    {
        uint32_t codepoint = 0;
        साठी (पूर्णांक k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            अक्षर hex = *ptr;
            uint32_t digit = 0;
            जर      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            अन्यथा जर (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            अन्यथा जर (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            अन्यथा CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        परत codepoint;
    }

    स्थिर शून्य appendUtf8(std::string& out, uint32_t codepoint)
    {
        जर (codepoint < 0x80) {
            out += (अक्षर)codepoint;
        } अन्यथा जर (codepoint < 0x800) {
            out += (अक्षर)(0xC0 | (codepoint >> 6));
            out += (अक्षर)(0x80 | (codepoint & 0x3F));
        } अन्यथा जर (codepoint < 0x10000) {
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
    शून्य parseUnicodeEscapeToUtf8(अक्षर*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        जर (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            जर (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t खालचा = parseUnicodeEscape(ptr);
            जर (खालचा < 0xDC00 || खालचा > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (खालचा - 0xDC00u);
        }
        अन्यथा जर (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    अक्षर* parse_किल्ली( अक्षर* ptr, File_गाठ& collection, File_गाठ& मूल्य_placeholder )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जर( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        अक्षर * beg = ptr + 1;
        std::string किल्ली_name;
        करा {
            जर (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                जर (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, किल्ली_name);
                    सुरूठेवा;
                }
                किल्ली_name += *ptr;
            } अन्यथा जर (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                किल्ली_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } पर्यंत( cv_isprint(*ptr) && *ptr != '"' );

        जर( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        जर( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        मूल्य_placeholder = fs->add_गाठ(collection, किल्ली_name, File_गाठ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        जर( !ptr || !*ptr )
            परत 0;

        जर( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        परत ++ptr;
    }

    तार्किक getBase64Row(अक्षर* ptr, पूर्णांक /*indent*/, अक्षर* &beg, अक्षर* &end)
    {
        beg = end = ptr;
        जर( !ptr || !*ptr )
            परत असत्य;

        // find end of the row
        पर्यंत( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        जर ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        परत सत्य;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाठ& गाठ )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        जर( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        जर( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            अक्षर * beg = ptr;
            माप_प्रकार len = 0u;
            साठी ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            जर ((len >= 8u) && (स_भेद( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, गाठ);

                जर ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;
            }
            अन्यथा
            {   /**************** normal string ****************/
                पूर्णांक i = 0, sz;

                ptr = beg;
                तार्किक is_matching = असत्य;
                पर्यंत ( !is_matching )
                {
                    निवड ( *ptr )
                    {
                        स्थिती '\\':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            जर( sz > 0 )
                            {
                                जर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            जर (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            निवड ( *ptr )
                            {
                            स्थिती '\\':
                            स्थिती '\"':
                            स्थिती '\'': { buf[i++] = *ptr; थांबा; }
                            स्थिती 'n' : { buf[i++] = '\n'; थांबा; }
                            स्थिती 'r' : { buf[i++] = '\r'; थांबा; }
                            स्थिती 't' : { buf[i++] = '\t'; थांबा; }
                            स्थिती 'b' : { buf[i++] = '\b'; थांबा; }
                            स्थिती 'f' : { buf[i++] = '\f'; थांबा; }
                            स्थिती 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                जर (i + (पूर्णांक)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, utf8.data(), utf8.size());
                                i += (पूर्णांक)utf8.size();
                                beg = ptr;
                                सुरूठेवा;
                            }
                            पूर्वनिर्धारित  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            थांबा;
                            }
                            ptr++;
                            beg = ptr;
                            थांबा;
                        }
                        स्थिती '\0':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            जर( sz > 0 )
                            {
                                जर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->माला_लो();
                            जर ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            थांबा;
                        }
                        स्थिती '\"':
                        {
                            sz = (पूर्णांक)(ptr - beg);
                            जर( sz > 0 )
                            {
                                जर (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स_नकल(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = सत्य;
                            थांबा;
                        }
                        स्थिती '\n':
                        स्थिती '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            थांबा;
                        }
                        पूर्वनिर्धारित:
                        {
                            ptr++;
                            थांबा;
                        }
                    }
                }

                जर ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;

                गाठ.set_मूल्य(File_गाठ::STRING, buf, i);
            }
        }
        अन्यथा जर ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            अक्षर * beg = ptr;
            जर ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            पर्यंत( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            जर (*ptr == '.' || *ptr == 'e')
            {
                द्विगुण fval = fs->म_से_भग्न( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                गाठ.set_मूल्य(File_गाठ::REAL, &fval);
            }
            अन्यथा
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                गाठ.set_मूल्य(File_गाठ::INT, &ival);
            }

            जर ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        अन्यथा
        {    /**************** other data ****************/
            स्थिरांक अक्षर* beg = ptr;
            पूर्णांक len = 0;
            साठी ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            जर( len == 4 && स_भेद( beg, "null", 4 ) == 0 )
                ;
            अन्यथा जर( (len == 4 && स_भेद( beg, "true", 4 ) == 0) ||
                     (len == 5 && स_भेद( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                गाठ.set_मूल्य(File_गाठ::INT, &ival);
            }
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        परत ptr;
    }

    अक्षर* parseSeq( अक्षर* ptr, File_गाठ& गाठ, पूर्णांक depth = 0 )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        जर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        जर ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाठ::SEQ, गाठ);

        साठी (;;)
        {
            ptr = skipSpaces( ptr );
            जर( !ptr || !*ptr )
                थांबा;

            जर ( *ptr != ']' )
            {
                File_गाठ child = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE );

                जर ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा जर ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
            }

            ptr = skipSpaces( ptr );
            जर( !ptr || !*ptr )
                थांबा;

            जर ( *ptr == ',' )
                ptr++;
            अन्यथा जर ( *ptr == ']' )
                थांबा;
            अन्यथा
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        जर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जर ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(गाठ);
        परत ptr;
    }

    अक्षर* parseMap( अक्षर* ptr, File_गाठ& गाठ, पूर्णांक depth = 0 )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        जर ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाठ::MAP, गाठ);

        साठी( ;; )
        {
            ptr = skipSpaces( ptr );
            जर( !ptr || !*ptr )
                थांबा;

            जर ( *ptr == '"' )
            {
                File_गाठ child;
                ptr = parse_किल्ली( ptr, गाठ, child );
                जर( !ptr || !*ptr )
                    थांबा;
                ptr = skipSpaces( ptr );
                जर( !ptr || !*ptr )
                    थांबा;

                जर ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा जर ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
            }

            ptr = skipSpaces( ptr );
            जर( !ptr || !*ptr )
                थांबा;

            जर ( *ptr == ',' )
                ptr++;
            अन्यथा जर ( *ptr == '}' )
                थांबा;
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        जर (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जर ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(गाठ);
        परत ptr;
    }

    तार्किक parse( अक्षर* ptr )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        जर ( !ptr || !*ptr )
            परत असत्य;

        File_गाठ मूळ_collection(fs->getFS(), 0, 0);

        जर( *ptr == '{' )
        {
            File_गाठ मूळ_गाठ = fs->add_गाठ(मूळ_collection, std::string(), File_गाठ::MAP);
            parseMap( ptr, मूळ_गाठ );
        }
        अन्यथा जर ( *ptr == '[' )
        {
            File_गाठ मूळ_गाठ = fs->add_गाठ(मूळ_collection, std::string(), File_गाठ::SEQ);
            parseSeq( ptr, मूळ_गाठ );
        }
        अन्यथा
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        परत सत्य;
    }

    FileStorage_API* fs;
    अक्षर buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    परत makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    परत makePtr<JSONParser>(fs);
}

}
