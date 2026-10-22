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
    आभास ~JSONEmitter() {}

    FStruct_दत्त startWriteStruct( ध्रुव FStruct_दत्त& parent, ध्रुव वर्ण* कुञ्चिका,
                                  पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0 )
    {
        वर्ण दत्त[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_आवरणम्|File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::रिक्तानि_2;
        यदि( !File_ग्रन्थिः::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        यदि( type_name && *type_name == '\0' )
            type_name = 0;

        तार्किक is_real_collection = सत्य;
        यदि (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ग्रन्थिः::STR;
            दत्त[0] = '\0';
            is_real_collection = असत्य;
        }

        यदि ( is_real_collection )
        {
            वर्ण c = File_ग्रन्थिः::isMap(struct_flags) ? '{' : '[';
            दत्त[0] = c;
            दत्त[1] = '\0';
        }

        writeScalar( कुञ्चिका, दत्त );
        FStruct_दत्त current_struct("", struct_flags, parent.indent + 4);

        प्रत्यागम current_struct;
    }

    शून्य अन्तः_Write_Struct(ध्रुव FStruct_दत्त& current_struct)
    {
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि (File_ग्रन्थिः::isCollection(struct_flags)) {
            यदि (!File_ग्रन्थिः::isFlow(struct_flags)) {
                यदि (fs->बफर_Ptr() <= fs->बफर_Start() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    वर्ण *ptr = fs->बफर_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->पङ्क्तिं_लिख(fs->बफर_Start());
                    fs->समुच्चयः_बफर_Ptr(fs->बफर_Start());
                }
                fs->flush();
            }

            वर्ण *ptr = fs->बफर_Ptr();
            यदि (ptr > fs->बफर_Start() + current_struct.indent && !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
            fs->समुच्चयः_बफर_Ptr(ptr);
        }
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क value)
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::itoa( value, buf, 10 ));
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, int64_t value)
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( ध्रुव वर्ण* कुञ्चिका, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::double_गन्तव्यम्_String( buf, परिमाण(buf), value, सत्य ));
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* दत्त = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णाङ्क need_quote = 1;
            दत्त = buf;
            *दत्त++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];
                ध्रुव अचिह्नित वर्ण byte = static_cast<अचिह्नित वर्ण>(c);

                विकल्प ( c )
                {
                पक्ष '\\':
                पक्ष '\"': { *दत्त++ = '\\'; *दत्त++ = c;   भङ्ग; }
                पक्ष '\n': { *दत्त++ = '\\'; *दत्त++ = 'n'; भङ्ग; }
                पक्ष '\r': { *दत्त++ = '\\'; *दत्त++ = 'r'; भङ्ग; }
                पक्ष '\t': { *दत्त++ = '\\'; *दत्त++ = 't'; भङ्ग; }
                पक्ष '\b': { *दत्त++ = '\\'; *दत्त++ = 'b'; भङ्ग; }
                पक्ष '\f': { *दत्त++ = '\\'; *दत्त++ = 'f'; भङ्ग; }
                स्वभाव  :
                    यदि (byte < 0x20)
                    {
                        स्थिर ध्रुव वर्ण hex[] = "0123456789abcdef";
                        *दत्त++ = '\\'; *दत्त++ = 'u'; *दत्त++ = '0'; *दत्त++ = '0';
                        *दत्त++ = hex[byte >> 4]; *दत्त++ = hex[byte & 0xf];
                    }
                    अन्यथा
                        *दत्त++ = c;
                    भङ्ग;
                }
            }

            *दत्त++ = '\"';
            *दत्त++ = '\0';
            दत्त = buf + !need_quote;
        }

        writeScalar( कुञ्चिका, दत्त);
    }

    शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* दत्त)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(असत्य);
        यदि ( fs->get_state_सम्बन्धि_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_गन्तव्यम्_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा यदि ( fs->get_state_सम्बन्धि_writing_base64() == FileStorage_API::अन्तर्_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t कुञ्चिका_len = 0u;
        यदि( कुञ्चिका && *कुञ्चिका == '\0' )
            कुञ्चिका = 0;
        यदि ( कुञ्चिका )
        {
            कुञ्चिका_len = वाक्यदैर्घ्यम्(कुञ्चिका);
            यदि ( कुञ्चिका_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            अन्यथा यदि ( static_cast<पूर्णाङ्क>(कुञ्चिका_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t दत्त_len = 0u;
        यदि ( दत्त )
            दत्त_len = वाक्यदैर्घ्यम्(दत्त);

        FStruct_दत्त& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;
        यदि( File_ग्रन्थिः::isCollection(struct_flags) )
        {
            यदि ( (File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } अन्यथा {
            fs->समुच्चयः_Non_रिक्तानि();
            struct_flags = File_ग्रन्थिः::रिक्तानि_2 | (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
        }

        // start to write
        वर्ण* ptr = 0;

        यदि( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            पूर्णाङ्क new_विस्थापनम्;
            ptr = fs->बफर_Ptr();
            यदि( !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
                *ptr++ = ',';
            new_विस्थापनम् = static_cast<पूर्णाङ्क>(ptr - fs->बफर_Start() + कुञ्चिका_len + दत्त_len);
            यदि( new_विस्थापनम् > fs->wrapMargin() && new_विस्थापनम् - current_struct.indent > 10 )
            {
                fs->समुच्चयः_बफर_Ptr(ptr);
                ptr = fs->flush();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            यदि ( !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
            {
                ptr = fs->बफर_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->पङ्क्तिं_लिख( fs->बफर_Start() );
                fs->समुच्चयः_बफर_Ptr(fs->बफर_Start());
            }
            ptr = fs->flush();
        }

        यदि( कुञ्चिका )
        {
            यदि( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_बफर( ptr, static_cast<पूर्णाङ्क>(कुञ्चिका_len) );
            *ptr++ = '\"';

            प्रति( size_t i = 0u; i < कुञ्चिका_len; i++ )
            {
                वर्ण c = कुञ्चिका[i];

                ptr[i] = c;
                यदि( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += कुञ्चिका_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        यदि( दत्त )
        {
            ptr = fs->resize_Write_बफर( ptr, static_cast<पूर्णाङ्क>(दत्त_len) );
            स्मृतिं_प्रतिलिख( ptr, दत्त, दत्त_len );
            ptr += दत्त_len;
        }

        fs->समुच्चयः_बफर_Ptr(ptr);
        current_struct.flags &= ~File_ग्रन्थिः::रिक्तानि_2;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = static_cast<पूर्णाङ्क>(वाक्यदैर्घ्यम्(comment));
        वर्ण* ptr = fs->बफर_Ptr();
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;

        यदि( !eol_comment || multiline || fs->बफर_अन्तः_2() - ptr < len || ptr == fs->बफर_Start() )
            ptr = fs->flush();
        अन्यथा
            *ptr++ = ' ';

        यावत्( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            यदि( eol )
            {
                ptr = fs->resize_Write_बफर( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                स्मृतिं_प्रतिलिख( ptr, comment, eol - comment + 1 );
                fs->समुच्चयः_बफर_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(comment);
                ptr = fs->resize_Write_बफर( ptr, len );
                स्मृतिं_प्रतिलिख( ptr, comment, len );
                fs->समुच्चयः_बफर_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    शून्य start_अग्र_प्रवाहः()
    {
        fs->पङ्क्तिं_लिख( "...\n" );
        fs->पङ्क्तिं_लिख( "---\n" );
    }

रक्षित:
    FileStorage_API* fs;
};

वर्ग JSONParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभास ~JSONParser() {}

    वर्ण* skipSpaces( वर्ण* ptr )
    {
        तार्किक is_eof = असत्य;
        तार्किक is_completed = असत्य;

        यावत् ( is_eof == असत्य && is_completed == असत्य )
        {
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            विकल्प ( *ptr )
            {
                /* comment */
                पक्ष '/' : {
                    ptr++;
                    यदि ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        यदि( !ptr || !*ptr ) { is_eof = सत्य; भङ्ग; }
                    }

                    यदि ( *ptr == '/' )
                    {
                        यावत् ( *ptr != '\n' && *ptr != '\r' )
                        {
                            यदि ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                यदि( !ptr || !*ptr ) { is_eof = सत्य; भङ्ग; }
                            }
                            अन्यथा
                            {
                                ptr++;
                            }
                        }
                    }
                    अन्यथा यदि ( *ptr == '*' )
                    {
                        ptr++;
                        प्रति (;;)
                        {
                            यदि ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                यदि( !ptr || !*ptr ) { is_eof = सत्य; भङ्ग; }
                            }
                            अन्यथा यदि ( *ptr == '*' )
                            {
                                ptr++;
                                यदि ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    यदि( !ptr || !*ptr ) { is_eof = सत्य; भङ्ग; }
                                }
                                यदि ( *ptr == '/' )
                                {
                                    ptr++;
                                    भङ्ग;
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
                } भङ्ग;
                    /* whitespace */
                पक्ष '\t':
                पक्ष ' ' : {
                    ptr++;
                } भङ्ग;
                    /* newline || end mark */
                पक्ष '\0':
                पक्ष '\n':
                पक्ष '\r': {
                    ptr = fs->gets();
                    यदि( !ptr || !*ptr ) { is_eof = सत्य; भङ्ग; }
                } भङ्ग;
                    /* other character */
                स्वभाव: {
                    यदि( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = सत्य;
                } भङ्ग;
            }
        }

        यदि ( is_eof || !is_completed )
        {
            ptr = fs->बफर_Start();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->समुच्चयः_Eof();
            यदि( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        प्रत्यागम ptr;
    }

    uint32_t parseUnicodeEscape(वर्ण*& ptr)
    {
        uint32_t codepoint = 0;
        प्रति (पूर्णाङ्क k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
            वर्ण hex = *ptr;
            uint32_t digit = 0;
            यदि      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            अन्यथा यदि (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            अन्यथा यदि (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            अन्यथा CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        प्रत्यागम codepoint;
    }

    स्थिर शून्य appendUtf8(std::string& out, uint32_t codepoint)
    {
        यदि (codepoint < 0x80) {
            out += (वर्ण)codepoint;
        } अन्यथा यदि (codepoint < 0x800) {
            out += (वर्ण)(0xC0 | (codepoint >> 6));
            out += (वर्ण)(0x80 | (codepoint & 0x3F));
        } अन्यथा यदि (codepoint < 0x10000) {
            out += (वर्ण)(0xE0 | (codepoint >> 12));
            out += (वर्ण)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (वर्ण)(0x80 | (codepoint & 0x3F));
        } अन्यथा {
            out += (वर्ण)(0xF0 | (codepoint >> 18));
            out += (वर्ण)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (वर्ण)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (वर्ण)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    शून्य parse_Unicode_Escape_गन्तव्यम्_Utf8(वर्ण*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        यदि (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            यदि (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t नीचः = parseUnicodeEscape(ptr);
            यदि (नीचः < 0xDC00 || नीचः > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (नीचः - 0xDC00u);
        }
        अन्यथा यदि (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    वर्ण* parse_कुञ्चिका( वर्ण* ptr, File_ग्रन्थिः& collection, File_ग्रन्थिः& मानम्_placeholder )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        वर्ण * beg = ptr + 1;
        std::string कुञ्चिका_name;
        कुरु {
            यदि (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
                यदि (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_गन्तव्यम्_Utf8(ptr, कुञ्चिका_name);
                    अनुवर्त;
                }
                कुञ्चिका_name += *ptr;
            } अन्यथा यदि (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                कुञ्चिका_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
        } यावत्( cv_isprint(*ptr) && *ptr != '"' );

        यदि( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        यदि( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        मानम्_placeholder = fs->सङ्कलय_ग्रन्थिः(collection, कुञ्चिका_name, File_ग्रन्थिः::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        यदि( !ptr || !*ptr )
            प्रत्यागम 0;

        यदि( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        प्रत्यागम ++ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr;
        यदि( !ptr || !*ptr )
            प्रत्यागम असत्य;

        // find end of the row
        यावत्( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        यदि ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        प्रत्यागम सत्य;
    }

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        यदि( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        यदि( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            वर्ण * beg = ptr;
            size_t len = 0u;
            प्रति ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            यदि ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ग्रन्थिः);

                यदि ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;
            }
            अन्यथा
            {   /**************** normal string ****************/
                पूर्णाङ्क i = 0, sz;

                ptr = beg;
                तार्किक is_matching = असत्य;
                यावत् ( !is_matching )
                {
                    विकल्प ( *ptr )
                    {
                        पक्ष '\\':
                        {
                            sz = (पूर्णाङ्क)(ptr - beg);
                            यदि( sz > 0 )
                            {
                                यदि (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स्मृतिं_प्रतिलिख(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            यदि (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            विकल्प ( *ptr )
                            {
                            पक्ष '\\':
                            पक्ष '\"':
                            पक्ष '\'': { buf[i++] = *ptr; भङ्ग; }
                            पक्ष 'n' : { buf[i++] = '\n'; भङ्ग; }
                            पक्ष 'r' : { buf[i++] = '\r'; भङ्ग; }
                            पक्ष 't' : { buf[i++] = '\t'; भङ्ग; }
                            पक्ष 'b' : { buf[i++] = '\b'; भङ्ग; }
                            पक्ष 'f' : { buf[i++] = '\f'; भङ्ग; }
                            पक्ष 'u' : {
                                ptr++;
                                std::string utf8;
                                parse_Unicode_Escape_गन्तव्यम्_Utf8(ptr, utf8);
                                यदि (i + (पूर्णाङ्क)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स्मृतिं_प्रतिलिख(buf + i, utf8.दत्त(), utf8.size());
                                i += (पूर्णाङ्क)utf8.size();
                                beg = ptr;
                                अनुवर्त;
                            }
                            स्वभाव  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            भङ्ग;
                            }
                            ptr++;
                            beg = ptr;
                            भङ्ग;
                        }
                        पक्ष '\0':
                        {
                            sz = (पूर्णाङ्क)(ptr - beg);
                            यदि( sz > 0 )
                            {
                                यदि (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स्मृतिं_प्रतिलिख(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            यदि ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            भङ्ग;
                        }
                        पक्ष '\"':
                        {
                            sz = (पूर्णाङ्क)(ptr - beg);
                            यदि( sz > 0 )
                            {
                                यदि (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                स्मृतिं_प्रतिलिख(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = सत्य;
                            भङ्ग;
                        }
                        पक्ष '\n':
                        पक्ष '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            भङ्ग;
                        }
                        स्वभाव:
                        {
                            ptr++;
                            भङ्ग;
                        }
                    }
                }

                यदि ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अन्यथा
                    ptr++;

                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::STRING, buf, i);
            }
        }
        अन्यथा यदि ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            वर्ण * beg = ptr;
            यदि ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
            }
            यावत्( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
            }
            यदि (*ptr == '.' || *ptr == 'e')
            {
                द्विगुण fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();

                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::REAL, &fval);
            }
            अन्यथा
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();

                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
            }

            यदि ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        अन्यथा
        {    /**************** other data ****************/
            ध्रुव वर्ण* beg = ptr;
            पूर्णाङ्क len = 0;
            प्रति ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
            }

            यदि( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            अन्यथा यदि( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
            }
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        प्रत्यागम ptr;
    }

    वर्ण* parseSeq( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क गभीरता = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        यदि( गभीरता > CV_PERSISTENCE_MAX_गभीरता )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        यदि ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->convert_गन्तव्यम्_Collection(File_ग्रन्थिः::SEQ, ग्रन्थिः);

        प्रति (;;)
        {
            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr != ']' )
            {
                File_ग्रन्थिः child = fs->सङ्कलय_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE );

                यदि ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, गभीरता + 1 );
                अन्यथा यदि ( *ptr == '{' )
                    ptr = parseMap( ptr, child, गभीरता + 1 );
                अन्यथा
                    ptr = parse_मानम्( ptr, child );
            }

            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr == ',' )
                ptr++;
            अन्यथा यदि ( *ptr == ']' )
                भङ्ग;
            अन्यथा
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        यदि (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        यदि ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(ग्रन्थिः);
        प्रत्यागम ptr;
    }

    वर्ण* parseMap( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क गभीरता = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        यदि( गभीरता > CV_PERSISTENCE_MAX_गभीरता )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        यदि ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->convert_गन्तव्यम्_Collection(File_ग्रन्थिः::MAP, ग्रन्थिः);

        प्रति( ;; )
        {
            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr == '"' )
            {
                File_ग्रन्थिः child;
                ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, child );
                यदि( !ptr || !*ptr )
                    भङ्ग;
                ptr = skipSpaces( ptr );
                यदि( !ptr || !*ptr )
                    भङ्ग;

                यदि ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, गभीरता + 1 );
                अन्यथा यदि ( *ptr == '{' )
                    ptr = parseMap( ptr, child, गभीरता + 1 );
                अन्यथा
                    ptr = parse_मानम्( ptr, child );
            }

            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr == ',' )
                ptr++;
            अन्यथा यदि ( *ptr == '}' )
                भङ्ग;
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        यदि (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        यदि ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->finalizeCollection(ग्रन्थिः);
        प्रत्यागम ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        यदि ( !ptr || !*ptr )
            प्रत्यागम असत्य;

        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        यदि( *ptr == '{' )
        {
            File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->सङ्कलय_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::MAP);
            parseMap( ptr, मूलम्_ग्रन्थिः );
        }
        अन्यथा यदि ( *ptr == '[' )
        {
            File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->सङ्कलय_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::SEQ);
            parseSeq( ptr, मूलम्_ग्रन्थिः );
        }
        अन्यथा
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        प्रत्यागम सत्य;
    }

    FileStorage_API* fs;
    वर्ण buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    प्रत्यागम makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    प्रत्यागम makePtr<JSONParser>(fs);
}

}
