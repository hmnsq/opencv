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

    FStructData startWriteStruct( ध्रुव FStructData& parent, ध्रुव वर्ण* कुंजी,
                                  पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0 )
    {
        वर्ण data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_गाँठ::TYPE_MASK|File_गाँठ::FLOW)) | File_गाँठ::EMPTY;
        यदि( !File_गाँठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        यदि( type_name && *type_name == '\0' )
            type_name = 0;

        तार्किक is_real_collection = सत्य;
        यदि (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_गाँठ::STR;
            data[0] = '\0';
            is_real_collection = असत्य;
        }

        यदि ( is_real_collection )
        {
            वर्ण c = File_गाँठ::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( कुंजी, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        प्रत्यागम current_struct;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि (File_गाँठ::isCollection(struct_flags)) {
            यदि (!File_गाँठ::isFlow(struct_flags)) {
                यदि (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    वर्ण *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            वर्ण *ptr = fs->bufferPtr();
            यदि (ptr > fs->bufferStart() + current_struct.indent && !File_गाँठ::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_गाँठ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, पूर्णाङ्क value)
    {
        वर्ण buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10 ));
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, int64_t value)
    {
        वर्ण buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( ध्रुव वर्ण* कुंजी, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुंजी, fs::doubleToString( buf, परिमाण(buf), value, सत्य ));
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)strlen(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णाङ्क need_quote = 1;
            data = buf;
            *data++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];
                ध्रुव unsigned वर्ण byte = static_cast<unsigned वर्ण>(c);

                विकल्प ( c )
                {
                पक्ष '\\':
                पक्ष '\"': { *data++ = '\\'; *data++ = c;   भङ्ग; }
                पक्ष '\n': { *data++ = '\\'; *data++ = 'n'; भङ्ग; }
                पक्ष '\r': { *data++ = '\\'; *data++ = 'r'; भङ्ग; }
                पक्ष '\t': { *data++ = '\\'; *data++ = 't'; भङ्ग; }
                पक्ष '\b': { *data++ = '\\'; *data++ = 'b'; भङ्ग; }
                पक्ष '\f': { *data++ = '\\'; *data++ = 'f'; भङ्ग; }
                स्वभाव  :
                    यदि (byte < 0x20)
                    {
                        स्थिर ध्रुव वर्ण hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    अन्यथा
                        *data++ = c;
                    भङ्ग;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( कुंजी, data);
    }

    शून्य writeScalar(ध्रुव वर्ण* कुंजी, ध्रुव वर्ण* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(असत्य);
        यदि ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा यदि ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t कुंजी_len = 0u;
        यदि( कुंजी && *कुंजी == '\0' )
            कुंजी = 0;
        यदि ( कुंजी )
        {
            कुंजी_len = strlen(कुंजी);
            यदि ( कुंजी_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            अन्यथा यदि ( static_cast<पूर्णाङ्क>(कुंजी_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        यदि ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;
        यदि( File_गाँठ::isCollection(struct_flags) )
        {
            यदि ( (File_गाँठ::isMap(struct_flags) ^ (कुंजी != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } अन्यथा {
            fs->setNonEmpty();
            struct_flags = File_गाँठ::EMPTY | (कुंजी ? File_गाँठ::MAP : File_गाँठ::SEQ);
        }

        // start to write
        वर्ण* ptr = 0;

        यदि( File_गाँठ::isFlow(struct_flags) )
        {
            पूर्णाङ्क new_offset;
            ptr = fs->bufferPtr();
            यदि( !File_गाँठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<पूर्णाङ्क>(ptr - fs->bufferStart() + कुंजी_len + data_len);
            यदि( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            यदि ( !File_गाँठ::isEmptyCollection(struct_flags) )
            {
                ptr = fs->bufferPtr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->bufferStart() );
                fs->setBufferPtr(fs->bufferStart());
            }
            ptr = fs->flush();
        }

        यदि( कुंजी )
        {
            यदि( !cv_isalpha(कुंजी[0]) && कुंजी[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णाङ्क>(कुंजी_len) );
            *ptr++ = '\"';

            प्रति( size_t i = 0u; i < कुंजी_len; i++ )
            {
                वर्ण c = कुंजी[i];

                ptr[i] = c;
                यदि( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += कुंजी_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        यदि( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णाङ्क>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_गाँठ::EMPTY;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = static_cast<पूर्णाङ्क>(strlen(comment));
        वर्ण* ptr = fs->bufferPtr();
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;

        यदि( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
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
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णाङ्क)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    शून्य startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
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
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            यदि( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        प्रत्यागम ptr;
    }

    uint32_t parseUnicodeEscape(वर्ण*& ptr)
    {
        uint32_t codepoint = 0;
        प्रति (पूर्णाङ्क k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
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
    शून्य parseUnicodeEscapeToUtf8(वर्ण*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        यदि (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            यदि (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t निचला = parseUnicodeEscape(ptr);
            यदि (निचला < 0xDC00 || निचला > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (निचला - 0xDC00u);
        }
        अन्यथा यदि (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    वर्ण* parse_कुंजी( वर्ण* ptr, File_गाँठ& collection, File_गाँठ& मूल्य_placeholder )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        वर्ण * beg = ptr + 1;
        std::string कुंजी_name;
        कुरु {
            यदि (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                यदि (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, कुंजी_name);
                    अनुवर्त;
                }
                कुंजी_name += *ptr;
            } अन्यथा यदि (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                कुंजी_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } यावत्( cv_isprint(*ptr) && *ptr != '"' );

        यदि( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        यदि( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        मूल्य_placeholder = fs->add_गाँठ(collection, कुंजी_name, File_गाँठ::NONE);

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

    वर्ण* parse_मूल्य( वर्ण* ptr, File_गाँठ& गाँठ )
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
                ptr = fs->parseBase64(ptr, 0, गाँठ);

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
                                memcpy(buf + i, beg, sz);
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
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                यदि (i + (पूर्णाङ्क)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
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
                                memcpy(buf + i, beg, sz);
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
                                memcpy(buf + i, beg, sz);
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

                गाँठ.set_मूल्य(File_गाँठ::STRING, buf, i);
            }
        }
        अन्यथा यदि ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            वर्ण * beg = ptr;
            यदि ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            यावत्( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            यदि (*ptr == '.' || *ptr == 'e')
            {
                द्विगुण fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                गाँठ.set_मूल्य(File_गाँठ::REAL, &fval);
            }
            अन्यथा
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
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
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            यदि( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            अन्यथा यदि( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
            }
            अन्यथा
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        प्रत्यागम ptr;
    }

    वर्ण* parseSeq( वर्ण* ptr, File_गाँठ& गाँठ, पूर्णाङ्क depth = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        यदि( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        यदि ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाँठ::SEQ, गाँठ);

        प्रति (;;)
        {
            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr != ']' )
            {
                File_गाँठ child = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE );

                यदि ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा यदि ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
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

        fs->finalizeCollection(गाँठ);
        प्रत्यागम ptr;
    }

    वर्ण* parseMap( वर्ण* ptr, File_गाँठ& गाँठ, पूर्णाङ्क depth = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        यदि( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        यदि ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        अन्यथा
            ptr++;

        fs->convertToCollection(File_गाँठ::MAP, गाँठ);

        प्रति( ;; )
        {
            ptr = skipSpaces( ptr );
            यदि( !ptr || !*ptr )
                भङ्ग;

            यदि ( *ptr == '"' )
            {
                File_गाँठ child;
                ptr = parse_कुंजी( ptr, गाँठ, child );
                यदि( !ptr || !*ptr )
                    भङ्ग;
                ptr = skipSpaces( ptr );
                यदि( !ptr || !*ptr )
                    भङ्ग;

                यदि ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अन्यथा यदि ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अन्यथा
                    ptr = parse_मूल्य( ptr, child );
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

        fs->finalizeCollection(गाँठ);
        प्रत्यागम ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        यदि ( !ptr || !*ptr )
            प्रत्यागम असत्य;

        File_गाँठ मूल_collection(fs->getFS(), 0, 0);

        यदि( *ptr == '{' )
        {
            File_गाँठ मूल_गाँठ = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::MAP);
            parseMap( ptr, मूल_गाँठ );
        }
        अन्यथा यदि ( *ptr == '[' )
        {
            File_गाँठ मूल_गाँठ = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::SEQ);
            parseSeq( ptr, मूल_गाँठ );
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
