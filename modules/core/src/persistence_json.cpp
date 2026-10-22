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

    FStructData startWriteStruct( ध्रुव FStructData& parent, ध्रुव वर्ण* कुञ्चिका,
                                  पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0 )
    {
        वर्ण data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_MASK|File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::EMPTY;
        जइ( !File_ग्रन्थिः::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        जइ( type_name && *type_name == '\0' )
            type_name = 0;

        तार्किक is_real_collection = सच्च;
        जइ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ग्रन्थिः::STR;
            data[0] = '\0';
            is_real_collection = मिच्छ;
        }

        जइ ( is_real_collection )
        {
            वर्ण c = File_ग्रन्थिः::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( कुञ्चिका, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        प्रत्यागम current_struct;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        पूर्णाङ्क struct_flags = current_struct.flags;

        जइ (File_ग्रन्थिः::isCollection(struct_flags)) {
            जइ (!File_ग्रन्थिः::isFlow(struct_flags)) {
                जइ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
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
            जइ (ptr > fs->bufferStart() + current_struct.indent && !File_ग्रन्थिः::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
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
        writeScalar( कुञ्चिका, fs::itoa( value, buf, 10, सच्च ));
    }

    शून्य लिख( ध्रुव वर्ण* कुञ्चिका, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::doubleToString( buf, परिमाण(buf), value, सच्च ));
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        जइ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)strlen(str);
        जइ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        जइ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
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
                    जइ (byte < 0x20)
                    {
                        स्थिर ध्रुव वर्ण hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    अण्णहा
                        *data++ = c;
                    भङ्ग;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( कुञ्चिका, data);
    }

    शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(मिच्छ);
        जइ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अण्णहा जइ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t कुञ्चिका_len = 0u;
        जइ( कुञ्चिका && *कुञ्चिका == '\0' )
            कुञ्चिका = 0;
        जइ ( कुञ्चिका )
        {
            कुञ्चिका_len = strlen(कुञ्चिका);
            जइ ( कुञ्चिका_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            अण्णहा जइ ( static_cast<पूर्णाङ्क>(कुञ्चिका_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        जइ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;
        जइ( File_ग्रन्थिः::isCollection(struct_flags) )
        {
            जइ ( (File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } अण्णहा {
            fs->setNonEmpty();
            struct_flags = File_ग्रन्थिः::EMPTY | (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
        }

        // start to write
        वर्ण* ptr = 0;

        जइ( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            पूर्णाङ्क new_offset;
            ptr = fs->bufferPtr();
            जइ( !File_ग्रन्थिः::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<पूर्णाङ्क>(ptr - fs->bufferStart() + कुञ्चिका_len + data_len);
            जइ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            अण्णहा
                *ptr++ = ' ';
        }
        अण्णहा
        {
            जइ ( !File_ग्रन्थिः::isEmptyCollection(struct_flags) )
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

        जइ( कुञ्चिका )
        {
            जइ( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णाङ्क>(कुञ्चिका_len) );
            *ptr++ = '\"';

            प्रति( size_t i = 0u; i < कुञ्चिका_len; i++ )
            {
                वर्ण c = कुञ्चिका[i];

                ptr[i] = c;
                जइ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += कुञ्चिका_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        जइ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<पूर्णाङ्क>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ग्रन्थिः::EMPTY;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        जइ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = static_cast<पूर्णाङ्क>(strlen(comment));
        वर्ण* ptr = fs->bufferPtr();
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;

        जइ( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        अण्णहा
            *ptr++ = ' ';

        जाव( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            जइ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अण्णहा
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
        तार्किक is_eof = मिच्छ;
        तार्किक is_completed = मिच्छ;

        जाव ( is_eof == मिच्छ && is_completed == मिच्छ )
        {
            जइ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            विकल्प ( *ptr )
            {
                /* comment */
                पक्ष '/' : {
                    ptr++;
                    जइ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        जइ( !ptr || !*ptr ) { is_eof = सच्च; भङ्ग; }
                    }

                    जइ ( *ptr == '/' )
                    {
                        जाव ( *ptr != '\n' && *ptr != '\r' )
                        {
                            जइ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                जइ( !ptr || !*ptr ) { is_eof = सच्च; भङ्ग; }
                            }
                            अण्णहा
                            {
                                ptr++;
                            }
                        }
                    }
                    अण्णहा जइ ( *ptr == '*' )
                    {
                        ptr++;
                        प्रति (;;)
                        {
                            जइ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                जइ( !ptr || !*ptr ) { is_eof = सच्च; भङ्ग; }
                            }
                            अण्णहा जइ ( *ptr == '*' )
                            {
                                ptr++;
                                जइ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    जइ( !ptr || !*ptr ) { is_eof = सच्च; भङ्ग; }
                                }
                                जइ ( *ptr == '/' )
                                {
                                    ptr++;
                                    भङ्ग;
                                }
                            }
                            अण्णहा
                            {
                                ptr++;
                            }
                        }
                    }
                    अण्णहा
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
                    जइ( !ptr || !*ptr ) { is_eof = सच्च; भङ्ग; }
                } भङ्ग;
                    /* other character */
                स्वभाव: {
                    जइ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = सच्च;
                } भङ्ग;
            }
        }

        जइ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            जइ( !is_completed )
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
            जइ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            अण्णहा जइ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            अण्णहा जइ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            अण्णहा CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        प्रत्यागम codepoint;
    }

    स्थिर शून्य appendUtf8(std::string& out, uint32_t codepoint)
    {
        जइ (codepoint < 0x80) {
            out += (वर्ण)codepoint;
        } अण्णहा जइ (codepoint < 0x800) {
            out += (वर्ण)(0xC0 | (codepoint >> 6));
            out += (वर्ण)(0x80 | (codepoint & 0x3F));
        } अण्णहा जइ (codepoint < 0x10000) {
            out += (वर्ण)(0xE0 | (codepoint >> 12));
            out += (वर्ण)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (वर्ण)(0x80 | (codepoint & 0x3F));
        } अण्णहा {
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
        जइ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            जइ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t नीचः = parseUnicodeEscape(ptr);
            जइ (नीचः < 0xDC00 || नीचः > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (नीचः - 0xDC00u);
        }
        अण्णहा जइ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    वर्ण* parse_कुञ्चिका( वर्ण* ptr, File_ग्रन्थिः& collection, File_ग्रन्थिः& मानम्_placeholder )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जइ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        वर्ण * beg = ptr + 1;
        std::string कुञ्चिका_name;
        करेहि {
            जइ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                जइ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, कुञ्चिका_name);
                    अनुवर्त;
                }
                कुञ्चिका_name += *ptr;
            } अण्णहा जइ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                कुञ्चिका_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } जाव( cv_isprint(*ptr) && *ptr != '"' );

        जइ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        जइ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        मानम्_placeholder = fs->add_ग्रन्थिः(collection, कुञ्चिका_name, File_ग्रन्थिः::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        जइ( !ptr || !*ptr )
            प्रत्यागम 0;

        जइ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        प्रत्यागम ++ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr;
        जइ( !ptr || !*ptr )
            प्रत्यागम मिच्छ;

        // find end of the row
        जाव( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        जइ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        प्रत्यागम सच्च;
    }

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        जइ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        जइ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            वर्ण * beg = ptr;
            size_t len = 0u;
            प्रति ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            जइ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ग्रन्थिः);

                जइ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अण्णहा
                    ptr++;
            }
            अण्णहा
            {   /**************** normal string ****************/
                पूर्णाङ्क i = 0, sz;

                ptr = beg;
                तार्किक is_matching = मिच्छ;
                जाव ( !is_matching )
                {
                    विकल्प ( *ptr )
                    {
                        पक्ष '\\':
                        {
                            sz = (पूर्णाङ्क)(ptr - beg);
                            जइ( sz > 0 )
                            {
                                जइ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            जइ (i + 1 >= CV_FS_MAX_LEN)
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
                                जइ (i + (पूर्णाङ्क)utf8.size() >= CV_FS_MAX_LEN)
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
                            जइ( sz > 0 )
                            {
                                जइ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            जइ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            भङ्ग;
                        }
                        पक्ष '\"':
                        {
                            sz = (पूर्णाङ्क)(ptr - beg);
                            जइ( sz > 0 )
                            {
                                जइ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = सच्च;
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

                जइ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                अण्णहा
                    ptr++;

                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::STRING, buf, i);
            }
        }
        अण्णहा जइ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            वर्ण * beg = ptr;
            जइ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            जाव( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            जइ (*ptr == '.' || *ptr == 'e')
            {
                द्विगुण fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::REAL, &fval);
            }
            अण्णहा
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::INT, &ival);
            }

            जइ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        अण्णहा
        {    /**************** other data ****************/
            ध्रुव वर्ण* beg = ptr;
            पूर्णाङ्क len = 0;
            प्रति ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            जइ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            अण्णहा जइ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::INT, &ival);
            }
            अण्णहा
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        प्रत्यागम ptr;
    }

    वर्ण* parseSeq( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क depth = 0 )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        जइ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        जइ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        अण्णहा
            ptr++;

        fs->convertToCollection(File_ग्रन्थिः::SEQ, ग्रन्थिः);

        प्रति (;;)
        {
            ptr = skipSpaces( ptr );
            जइ( !ptr || !*ptr )
                भङ्ग;

            जइ ( *ptr != ']' )
            {
                File_ग्रन्थिः child = fs->add_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE );

                जइ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अण्णहा जइ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अण्णहा
                    ptr = parse_मानम्( ptr, child );
            }

            ptr = skipSpaces( ptr );
            जइ( !ptr || !*ptr )
                भङ्ग;

            जइ ( *ptr == ',' )
                ptr++;
            अण्णहा जइ ( *ptr == ']' )
                भङ्ग;
            अण्णहा
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        जइ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जइ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        अण्णहा
            ptr++;

        fs->finalizeCollection(ग्रन्थिः);
        प्रत्यागम ptr;
    }

    वर्ण* parseMap( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क depth = 0 )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जइ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        जइ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        अण्णहा
            ptr++;

        fs->convertToCollection(File_ग्रन्थिः::MAP, ग्रन्थिः);

        प्रति( ;; )
        {
            ptr = skipSpaces( ptr );
            जइ( !ptr || !*ptr )
                भङ्ग;

            जइ ( *ptr == '"' )
            {
                File_ग्रन्थिः child;
                ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, child );
                जइ( !ptr || !*ptr )
                    भङ्ग;
                ptr = skipSpaces( ptr );
                जइ( !ptr || !*ptr )
                    भङ्ग;

                जइ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                अण्णहा जइ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                अण्णहा
                    ptr = parse_मानम्( ptr, child );
            }

            ptr = skipSpaces( ptr );
            जइ( !ptr || !*ptr )
                भङ्ग;

            जइ ( *ptr == ',' )
                ptr++;
            अण्णहा जइ ( *ptr == '}' )
                भङ्ग;
            अण्णहा
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        जइ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        जइ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        अण्णहा
            ptr++;

        fs->finalizeCollection(ग्रन्थिः);
        प्रत्यागम ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        जइ ( !ptr || !*ptr )
            प्रत्यागम मिच्छ;

        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        जइ( *ptr == '{' )
        {
            File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->add_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::MAP);
            parseMap( ptr, मूलम्_ग्रन्थिः );
        }
        अण्णहा जइ ( *ptr == '[' )
        {
            File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->add_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::SEQ);
            parseSeq( ptr, मूलम्_ग्रन्थिः );
        }
        अण्णहा
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        प्रत्यागम सच्च;
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
