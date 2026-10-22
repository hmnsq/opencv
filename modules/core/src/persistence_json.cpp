// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence.hpp"

നാമാകാശ cv
{

വർഗ്ഗം JSONEmitter : പൊതു FileStorageEmitter
{
പൊതു:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    മായാ ~JSONEmitter() {}

    FStructData startWriteStruct( സ്ഥിരം FStructData& parent, സ്ഥിരം അക്ഷരം* താക്കോൽ,
                                  പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം* type_name=0 )
    {
        അക്ഷരം data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_കണ്ണി::TYPE_MASK|File_കണ്ണി::FLOW)) | File_കണ്ണി::EMPTY;
        എങ്കിൽ( !File_കണ്ണി::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        എങ്കിൽ( type_name && *type_name == '\0' )
            type_name = 0;

        തർക്കം is_real_collection = സത്യം;
        എങ്കിൽ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_കണ്ണി::STR;
            data[0] = '\0';
            is_real_collection = അസത്യം;
        }

        എങ്കിൽ ( is_real_collection )
        {
            അക്ഷരം c = File_കണ്ണി::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( താക്കോൽ, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        തിരികെ current_struct;
    }

    ശൂന്യം endWriteStruct(സ്ഥിരം FStructData& current_struct)
    {
        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;

        എങ്കിൽ (File_കണ്ണി::isCollection(struct_flags)) {
            എങ്കിൽ (!File_കണ്ണി::isFlow(struct_flags)) {
                എങ്കിൽ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    അക്ഷരം *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            അക്ഷരം *ptr = fs->bufferPtr();
            എങ്കിൽ (ptr > fs->bufferStart() + current_struct.indent && !File_കണ്ണി::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_കണ്ണി::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ value)
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::itoa( value, buf, 10 ));
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, int64_t value)
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::itoa( value, buf, 10, സത്യം ));
    }

    ശൂന്യം ലിഖ( സ്ഥിരം അക്ഷരം* താക്കോൽ, ഇരട്ട value )
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::doubleToString( buf, വലുപ്പം(buf), value, സത്യം ));
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* str, തർക്കം quote)
    {
        അക്ഷരം buf[CV_FS_MAX_LEN*6+16];
        അക്ഷരം* data = (അക്ഷരം*)str;
        പൂർണ്ണസംഖ്യ i, len;

        എങ്കിൽ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (പൂർണ്ണസംഖ്യ)strlen(str);
        എങ്കിൽ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        എങ്കിൽ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            പൂർണ്ണസംഖ്യ need_quote = 1;
            data = buf;
            *data++ = '\"';
            വേണ്ടി( i = 0; i < len; i++ )
            {
                അക്ഷരം c = str[i];
                സ്ഥിരം unsigned അക്ഷരം byte = static_cast<unsigned അക്ഷരം>(c);

                തിരഞ്ഞെടുപ്പ് ( c )
                {
                അവസ്ഥ '\\':
                അവസ്ഥ '\"': { *data++ = '\\'; *data++ = c;   നിർത്തുക; }
                അവസ്ഥ '\n': { *data++ = '\\'; *data++ = 'n'; നിർത്തുക; }
                അവസ്ഥ '\r': { *data++ = '\\'; *data++ = 'r'; നിർത്തുക; }
                അവസ്ഥ '\t': { *data++ = '\\'; *data++ = 't'; നിർത്തുക; }
                അവസ്ഥ '\b': { *data++ = '\\'; *data++ = 'b'; നിർത്തുക; }
                അവസ്ഥ '\f': { *data++ = '\\'; *data++ = 'f'; നിർത്തുക; }
                സ്ഥിരസ്ഥിതി  :
                    എങ്കിൽ (byte < 0x20)
                    {
                        സ്ഥിതം സ്ഥിരം അക്ഷരം hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    അല്ലെങ്കിൽ
                        *data++ = c;
                    നിർത്തുക;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( താക്കോൽ, data);
    }

    ശൂന്യം writeScalar(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(അസത്യം);
        എങ്കിൽ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        അല്ലെങ്കിൽ എങ്കിൽ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t താക്കോൽ_len = 0u;
        എങ്കിൽ( താക്കോൽ && *താക്കോൽ == '\0' )
            താക്കോൽ = 0;
        എങ്കിൽ ( താക്കോൽ )
        {
            താക്കോൽ_len = strlen(താക്കോൽ);
            എങ്കിൽ ( താക്കോൽ_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            അല്ലെങ്കിൽ എങ്കിൽ ( static_cast<പൂർണ്ണസംഖ്യ>(താക്കോൽ_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        എങ്കിൽ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;
        എങ്കിൽ( File_കണ്ണി::isCollection(struct_flags) )
        {
            എങ്കിൽ ( (File_കണ്ണി::isMap(struct_flags) ^ (താക്കോൽ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } അല്ലെങ്കിൽ {
            fs->setNonEmpty();
            struct_flags = File_കണ്ണി::EMPTY | (താക്കോൽ ? File_കണ്ണി::MAP : File_കണ്ണി::SEQ);
        }

        // start to write
        അക്ഷരം* ptr = 0;

        എങ്കിൽ( File_കണ്ണി::isFlow(struct_flags) )
        {
            പൂർണ്ണസംഖ്യ new_offset;
            ptr = fs->bufferPtr();
            എങ്കിൽ( !File_കണ്ണി::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<പൂർണ്ണസംഖ്യ>(ptr - fs->bufferStart() + താക്കോൽ_len + data_len);
            എങ്കിൽ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            അല്ലെങ്കിൽ
                *ptr++ = ' ';
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ ( !File_കണ്ണി::isEmptyCollection(struct_flags) )
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

        എങ്കിൽ( താക്കോൽ )
        {
            എങ്കിൽ( !cv_isalpha(താക്കോൽ[0]) && താക്കോൽ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<പൂർണ്ണസംഖ്യ>(താക്കോൽ_len) );
            *ptr++ = '\"';

            വേണ്ടി( size_t i = 0u; i < താക്കോൽ_len; i++ )
            {
                അക്ഷരം c = താക്കോൽ[i];

                ptr[i] = c;
                എങ്കിൽ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += താക്കോൽ_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        എങ്കിൽ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<പൂർണ്ണസംഖ്യ>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_കണ്ണി::EMPTY;
    }

    ശൂന്യം writeComment(സ്ഥിരം അക്ഷരം* comment, തർക്കം eol_comment)
    {
        എങ്കിൽ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        പൂർണ്ണസംഖ്യ len = static_cast<പൂർണ്ണസംഖ്യ>(strlen(comment));
        അക്ഷരം* ptr = fs->bufferPtr();
        സ്ഥിരം അക്ഷരം* eol = strchr(comment, '\n');
        തർക്കം multiline = eol != 0;

        എങ്കിൽ( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        അല്ലെങ്കിൽ
            *ptr++ = ' ';

        ആകുമ്പോൾ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            എങ്കിൽ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (പൂർണ്ണസംഖ്യ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            അല്ലെങ്കിൽ
            {
                len = (പൂർണ്ണസംഖ്യ)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ശൂന്യം startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

സംരക്ഷിത:
    FileStorage_API* fs;
};

വർഗ്ഗം JSONParser : പൊതു FileStorageParser
{
പൊതു:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    മായാ ~JSONParser() {}

    അക്ഷരം* skipSpaces( അക്ഷരം* ptr )
    {
        തർക്കം is_eof = അസത്യം;
        തർക്കം is_completed = അസത്യം;

        ആകുമ്പോൾ ( is_eof == അസത്യം && is_completed == അസത്യം )
        {
            എങ്കിൽ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            തിരഞ്ഞെടുപ്പ് ( *ptr )
            {
                /* comment */
                അവസ്ഥ '/' : {
                    ptr++;
                    എങ്കിൽ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        എങ്കിൽ( !ptr || !*ptr ) { is_eof = സത്യം; നിർത്തുക; }
                    }

                    എങ്കിൽ ( *ptr == '/' )
                    {
                        ആകുമ്പോൾ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            എങ്കിൽ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                എങ്കിൽ( !ptr || !*ptr ) { is_eof = സത്യം; നിർത്തുക; }
                            }
                            അല്ലെങ്കിൽ
                            {
                                ptr++;
                            }
                        }
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '*' )
                    {
                        ptr++;
                        വേണ്ടി (;;)
                        {
                            എങ്കിൽ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                എങ്കിൽ( !ptr || !*ptr ) { is_eof = സത്യം; നിർത്തുക; }
                            }
                            അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '*' )
                            {
                                ptr++;
                                എങ്കിൽ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    എങ്കിൽ( !ptr || !*ptr ) { is_eof = സത്യം; നിർത്തുക; }
                                }
                                എങ്കിൽ ( *ptr == '/' )
                                {
                                    ptr++;
                                    നിർത്തുക;
                                }
                            }
                            അല്ലെങ്കിൽ
                            {
                                ptr++;
                            }
                        }
                    }
                    അല്ലെങ്കിൽ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } നിർത്തുക;
                    /* whitespace */
                അവസ്ഥ '\t':
                അവസ്ഥ ' ' : {
                    ptr++;
                } നിർത്തുക;
                    /* newline || end mark */
                അവസ്ഥ '\0':
                അവസ്ഥ '\n':
                അവസ്ഥ '\r': {
                    ptr = fs->gets();
                    എങ്കിൽ( !ptr || !*ptr ) { is_eof = സത്യം; നിർത്തുക; }
                } നിർത്തുക;
                    /* other character */
                സ്ഥിരസ്ഥിതി: {
                    എങ്കിൽ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = സത്യം;
                } നിർത്തുക;
            }
        }

        എങ്കിൽ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            എങ്കിൽ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        തിരികെ ptr;
    }

    uint32_t parseUnicodeEscape(അക്ഷരം*& ptr)
    {
        uint32_t codepoint = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            അക്ഷരം hex = *ptr;
            uint32_t digit = 0;
            എങ്കിൽ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            അല്ലെങ്കിൽ എങ്കിൽ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            അല്ലെങ്കിൽ എങ്കിൽ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            അല്ലെങ്കിൽ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        തിരികെ codepoint;
    }

    സ്ഥിതം ശൂന്യം appendUtf8(std::string& out, uint32_t codepoint)
    {
        എങ്കിൽ (codepoint < 0x80) {
            out += (അക്ഷരം)codepoint;
        } അല്ലെങ്കിൽ എങ്കിൽ (codepoint < 0x800) {
            out += (അക്ഷരം)(0xC0 | (codepoint >> 6));
            out += (അക്ഷരം)(0x80 | (codepoint & 0x3F));
        } അല്ലെങ്കിൽ എങ്കിൽ (codepoint < 0x10000) {
            out += (അക്ഷരം)(0xE0 | (codepoint >> 12));
            out += (അക്ഷരം)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (അക്ഷരം)(0x80 | (codepoint & 0x3F));
        } അല്ലെങ്കിൽ {
            out += (അക്ഷരം)(0xF0 | (codepoint >> 18));
            out += (അക്ഷരം)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (അക്ഷരം)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (അക്ഷരം)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    ശൂന്യം parseUnicodeEscapeToUtf8(അക്ഷരം*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        എങ്കിൽ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            എങ്കിൽ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t താഴെയുള്ള = parseUnicodeEscape(ptr);
            എങ്കിൽ (താഴെയുള്ള < 0xDC00 || താഴെയുള്ള > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (താഴെയുള്ള - 0xDC00u);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    അക്ഷരം* parse_താക്കോൽ( അക്ഷരം* ptr, File_കണ്ണി& collection, File_കണ്ണി& മൂല്യം_placeholder )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        എങ്കിൽ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        അക്ഷരം * beg = ptr + 1;
        std::string താക്കോൽ_name;
        ചെയ്യുക {
            എങ്കിൽ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                എങ്കിൽ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, താക്കോൽ_name);
                    തുടരുക;
                }
                താക്കോൽ_name += *ptr;
            } അല്ലെങ്കിൽ എങ്കിൽ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                താക്കോൽ_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ആകുമ്പോൾ( cv_isprint(*ptr) && *ptr != '"' );

        എങ്കിൽ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        എങ്കിൽ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        മൂല്യം_placeholder = fs->add_കണ്ണി(collection, താക്കോൽ_name, File_കണ്ണി::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        എങ്കിൽ( !ptr || !*ptr )
            തിരികെ 0;

        എങ്കിൽ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        തിരികെ ++ptr;
    }

    തർക്കം getBase64Row(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ /*indent*/, അക്ഷരം* &beg, അക്ഷരം* &end)
    {
        beg = end = ptr;
        എങ്കിൽ( !ptr || !*ptr )
            തിരികെ അസത്യം;

        // find end of the row
        ആകുമ്പോൾ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        എങ്കിൽ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        തിരികെ സത്യം;
    }

    അക്ഷരം* parse_മൂല്യം( അക്ഷരം* ptr, File_കണ്ണി& കണ്ണി )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        എങ്കിൽ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        എങ്കിൽ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            അക്ഷരം * beg = ptr;
            size_t len = 0u;
            വേണ്ടി ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            എങ്കിൽ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, കണ്ണി);

                എങ്കിൽ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                അല്ലെങ്കിൽ
                    ptr++;
            }
            അല്ലെങ്കിൽ
            {   /**************** normal string ****************/
                പൂർണ്ണസംഖ്യ i = 0, sz;

                ptr = beg;
                തർക്കം is_matching = അസത്യം;
                ആകുമ്പോൾ ( !is_matching )
                {
                    തിരഞ്ഞെടുപ്പ് ( *ptr )
                    {
                        അവസ്ഥ '\\':
                        {
                            sz = (പൂർണ്ണസംഖ്യ)(ptr - beg);
                            എങ്കിൽ( sz > 0 )
                            {
                                എങ്കിൽ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            എങ്കിൽ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            തിരഞ്ഞെടുപ്പ് ( *ptr )
                            {
                            അവസ്ഥ '\\':
                            അവസ്ഥ '\"':
                            അവസ്ഥ '\'': { buf[i++] = *ptr; നിർത്തുക; }
                            അവസ്ഥ 'n' : { buf[i++] = '\n'; നിർത്തുക; }
                            അവസ്ഥ 'r' : { buf[i++] = '\r'; നിർത്തുക; }
                            അവസ്ഥ 't' : { buf[i++] = '\t'; നിർത്തുക; }
                            അവസ്ഥ 'b' : { buf[i++] = '\b'; നിർത്തുക; }
                            അവസ്ഥ 'f' : { buf[i++] = '\f'; നിർത്തുക; }
                            അവസ്ഥ 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                എങ്കിൽ (i + (പൂർണ്ണസംഖ്യ)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (പൂർണ്ണസംഖ്യ)utf8.size();
                                beg = ptr;
                                തുടരുക;
                            }
                            സ്ഥിരസ്ഥിതി  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            നിർത്തുക;
                            }
                            ptr++;
                            beg = ptr;
                            നിർത്തുക;
                        }
                        അവസ്ഥ '\0':
                        {
                            sz = (പൂർണ്ണസംഖ്യ)(ptr - beg);
                            എങ്കിൽ( sz > 0 )
                            {
                                എങ്കിൽ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            എങ്കിൽ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            നിർത്തുക;
                        }
                        അവസ്ഥ '\"':
                        {
                            sz = (പൂർണ്ണസംഖ്യ)(ptr - beg);
                            എങ്കിൽ( sz > 0 )
                            {
                                എങ്കിൽ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = സത്യം;
                            നിർത്തുക;
                        }
                        അവസ്ഥ '\n':
                        അവസ്ഥ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            നിർത്തുക;
                        }
                        സ്ഥിരസ്ഥിതി:
                        {
                            ptr++;
                            നിർത്തുക;
                        }
                    }
                }

                എങ്കിൽ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                അല്ലെങ്കിൽ
                    ptr++;

                കണ്ണി.set_മൂല്യം(File_കണ്ണി::STRING, buf, i);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            അക്ഷരം * beg = ptr;
            എങ്കിൽ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ആകുമ്പോൾ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            എങ്കിൽ (*ptr == '.' || *ptr == 'e')
            {
                ഇരട്ട fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                കണ്ണി.set_മൂല്യം(File_കണ്ണി::REAL, &fval);
            }
            അല്ലെങ്കിൽ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                കണ്ണി.set_മൂല്യം(File_കണ്ണി::INT, &ival);
            }

            എങ്കിൽ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        അല്ലെങ്കിൽ
        {    /**************** other data ****************/
            സ്ഥിരം അക്ഷരം* beg = ptr;
            പൂർണ്ണസംഖ്യ len = 0;
            വേണ്ടി ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            എങ്കിൽ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            അല്ലെങ്കിൽ എങ്കിൽ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                കണ്ണി.set_മൂല്യം(File_കണ്ണി::INT, &ival);
            }
            അല്ലെങ്കിൽ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        തിരികെ ptr;
    }

    അക്ഷരം* parseSeq( അക്ഷരം* ptr, File_കണ്ണി& കണ്ണി, പൂർണ്ണസംഖ്യ depth = 0 )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        എങ്കിൽ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        എങ്കിൽ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        അല്ലെങ്കിൽ
            ptr++;

        fs->convertToCollection(File_കണ്ണി::SEQ, കണ്ണി);

        വേണ്ടി (;;)
        {
            ptr = skipSpaces( ptr );
            എങ്കിൽ( !ptr || !*ptr )
                നിർത്തുക;

            എങ്കിൽ ( *ptr != ']' )
            {
                File_കണ്ണി child = fs->add_കണ്ണി(കണ്ണി, std::string(), File_കണ്ണി::NONE );

                എങ്കിൽ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                അല്ലെങ്കിൽ
                    ptr = parse_മൂല്യം( ptr, child );
            }

            ptr = skipSpaces( ptr );
            എങ്കിൽ( !ptr || !*ptr )
                നിർത്തുക;

            എങ്കിൽ ( *ptr == ',' )
                ptr++;
            അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == ']' )
                നിർത്തുക;
            അല്ലെങ്കിൽ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        എങ്കിൽ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        അല്ലെങ്കിൽ
            ptr++;

        fs->finalizeCollection(കണ്ണി);
        തിരികെ ptr;
    }

    അക്ഷരം* parseMap( അക്ഷരം* ptr, File_കണ്ണി& കണ്ണി, പൂർണ്ണസംഖ്യ depth = 0 )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        എങ്കിൽ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        എങ്കിൽ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        അല്ലെങ്കിൽ
            ptr++;

        fs->convertToCollection(File_കണ്ണി::MAP, കണ്ണി);

        വേണ്ടി( ;; )
        {
            ptr = skipSpaces( ptr );
            എങ്കിൽ( !ptr || !*ptr )
                നിർത്തുക;

            എങ്കിൽ ( *ptr == '"' )
            {
                File_കണ്ണി child;
                ptr = parse_താക്കോൽ( ptr, കണ്ണി, child );
                എങ്കിൽ( !ptr || !*ptr )
                    നിർത്തുക;
                ptr = skipSpaces( ptr );
                എങ്കിൽ( !ptr || !*ptr )
                    നിർത്തുക;

                എങ്കിൽ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                അല്ലെങ്കിൽ
                    ptr = parse_മൂല്യം( ptr, child );
            }

            ptr = skipSpaces( ptr );
            എങ്കിൽ( !ptr || !*ptr )
                നിർത്തുക;

            എങ്കിൽ ( *ptr == ',' )
                ptr++;
            അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '}' )
                നിർത്തുക;
            അല്ലെങ്കിൽ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        എങ്കിൽ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        അല്ലെങ്കിൽ
            ptr++;

        fs->finalizeCollection(കണ്ണി);
        തിരികെ ptr;
    }

    തർക്കം parse( അക്ഷരം* ptr )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        എങ്കിൽ ( !ptr || !*ptr )
            തിരികെ അസത്യം;

        File_കണ്ണി വേര്_collection(fs->getFS(), 0, 0);

        എങ്കിൽ( *ptr == '{' )
        {
            File_കണ്ണി വേര്_കണ്ണി = fs->add_കണ്ണി(വേര്_collection, std::string(), File_കണ്ണി::MAP);
            parseMap( ptr, വേര്_കണ്ണി );
        }
        അല്ലെങ്കിൽ എങ്കിൽ ( *ptr == '[' )
        {
            File_കണ്ണി വേര്_കണ്ണി = fs->add_കണ്ണി(വേര്_collection, std::string(), File_കണ്ണി::SEQ);
            parseSeq( ptr, വേര്_കണ്ണി );
        }
        അല്ലെങ്കിൽ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        തിരികെ സത്യം;
    }

    FileStorage_API* fs;
    അക്ഷരം buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    തിരികെ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    തിരികെ makePtr<JSONParser>(fs);
}

}
