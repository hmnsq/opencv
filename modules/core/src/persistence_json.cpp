// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ JSONEmitter : ಸಾರ್ವಜನಿಕ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ಆಭಾಸ ~JSONEmitter() {}

    FStructData startWriteStruct( ಧ್ರುವ FStructData& parent, ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ,
                                  ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ* type_name=0 )
    {
        ವರ್ಣ data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_MASK|File_ಗಂಟು::FLOW)) | File_ಗಂಟು::EMPTY;
        ಯದಿ( !File_ಗಂಟು::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ಯದಿ( type_name && *type_name == '\0' )
            type_name = 0;

        ತಾರ್ಕಿಕ is_real_collection = ಸತ್ಯ;
        ಯದಿ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ಗಂಟು::STR;
            data[0] = '\0';
            is_real_collection = ಅಸತ್ಯ;
        }

        ಯದಿ ( is_real_collection )
        {
            ವರ್ಣ c = File_ಗಂಟು::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( ಕೀಲಿ, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        ಪ್ರತ್ಯಾಗಮ current_struct;
    }

    ಶೂನ್ಯ endWriteStruct(ಧ್ರುವ FStructData& current_struct)
    {
        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;

        ಯದಿ (File_ಗಂಟು::isCollection(struct_flags)) {
            ಯದಿ (!File_ಗಂಟು::isFlow(struct_flags)) {
                ಯದಿ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    ವರ್ಣ *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            ವರ್ಣ *ptr = fs->bufferPtr();
            ಯದಿ (ptr > fs->bufferStart() + current_struct.indent && !File_ಗಂಟು::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ value)
    {
        ವರ್ಣ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10 ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, int64_t value)
    {
        ವರ್ಣ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10, ಸತ್ಯ ));
    }

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ದ್ವಿಗುಣ value )
    {
        ವರ್ಣ buf[128];
        writeScalar( ಕೀಲಿ, fs::doubleToString( buf, ಪರಿಮಾಣ(buf), value, ಸತ್ಯ ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* str, ತಾರ್ಕಿಕ quote)
    {
        ವರ್ಣ buf[CV_FS_MAX_LEN*6+16];
        ವರ್ಣ* data = (ವರ್ಣ*)str;
        ಪೂರ್ಣಾಙ್ಕ i, len;

        ಯದಿ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಙ್ಕ)strlen(str);
        ಯದಿ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಯದಿ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ಪೂರ್ಣಾಙ್ಕ need_quote = 1;
            data = buf;
            *data++ = '\"';
            ಪ್ರತಿ( i = 0; i < len; i++ )
            {
                ವರ್ಣ c = str[i];
                ಧ್ರುವ unsigned ವರ್ಣ byte = static_cast<unsigned ವರ್ಣ>(c);

                ವಿಕಲ್ಪ ( c )
                {
                ಪಕ್ಷ '\\':
                ಪಕ್ಷ '\"': { *data++ = '\\'; *data++ = c;   ಭಙ್ಗ; }
                ಪಕ್ಷ '\n': { *data++ = '\\'; *data++ = 'n'; ಭಙ್ಗ; }
                ಪಕ್ಷ '\r': { *data++ = '\\'; *data++ = 'r'; ಭಙ್ಗ; }
                ಪಕ್ಷ '\t': { *data++ = '\\'; *data++ = 't'; ಭಙ್ಗ; }
                ಪಕ್ಷ '\b': { *data++ = '\\'; *data++ = 'b'; ಭಙ್ಗ; }
                ಪಕ್ಷ '\f': { *data++ = '\\'; *data++ = 'f'; ಭಙ್ಗ; }
                ಸ್ವಭಾವ  :
                    ಯದಿ (byte < 0x20)
                    {
                        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    ಅನ್ಯಥಾ
                        *data++ = c;
                    ಭಙ್ಗ;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ಕೀಲಿ, data);
    }

    ಶೂನ್ಯ writeScalar(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ಅಸತ್ಯ);
        ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ಅನ್ಯಥಾ ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ಕೀಲಿ_len = 0u;
        ಯದಿ( ಕೀಲಿ && *ಕೀಲಿ == '\0' )
            ಕೀಲಿ = 0;
        ಯದಿ ( ಕೀಲಿ )
        {
            ಕೀಲಿ_len = strlen(ಕೀಲಿ);
            ಯದಿ ( ಕೀಲಿ_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ಅನ್ಯಥಾ ಯದಿ ( static_cast<ಪೂರ್ಣಾಙ್ಕ>(ಕೀಲಿ_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ಯದಿ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;
        ಯದಿ( File_ಗಂಟು::isCollection(struct_flags) )
        {
            ಯದಿ ( (File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ಅನ್ಯಥಾ {
            fs->setNonEmpty();
            struct_flags = File_ಗಂಟು::EMPTY | (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
        }

        // start to write
        ವರ್ಣ* ptr = 0;

        ಯದಿ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ಪೂರ್ಣಾಙ್ಕ new_offset;
            ptr = fs->bufferPtr();
            ಯದಿ( !File_ಗಂಟು::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<ಪೂರ್ಣಾಙ್ಕ>(ptr - fs->bufferStart() + ಕೀಲಿ_len + data_len);
            ಯದಿ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ಅನ್ಯಥಾ
                *ptr++ = ' ';
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ ( !File_ಗಂಟು::isEmptyCollection(struct_flags) )
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

        ಯದಿ( ಕೀಲಿ )
        {
            ಯದಿ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<ಪೂರ್ಣಾಙ್ಕ>(ಕೀಲಿ_len) );
            *ptr++ = '\"';

            ಪ್ರತಿ( size_t i = 0u; i < ಕೀಲಿ_len; i++ )
            {
                ವರ್ಣ c = ಕೀಲಿ[i];

                ptr[i] = c;
                ಯದಿ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ಕೀಲಿ_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ಯದಿ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<ಪೂರ್ಣಾಙ್ಕ>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ಗಂಟು::EMPTY;
    }

    ಶೂನ್ಯ writeComment(ಧ್ರುವ ವರ್ಣ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        ಯದಿ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಪೂರ್ಣಾಙ್ಕ len = static_cast<ಪೂರ್ಣಾಙ್ಕ>(strlen(comment));
        ವರ್ಣ* ptr = fs->bufferPtr();
        ಧ್ರುವ ವರ್ಣ* eol = strchr(comment, '\n');
        ತಾರ್ಕಿಕ multiline = eol != 0;

        ಯದಿ( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ಅನ್ಯಥಾ
            *ptr++ = ' ';

        ಯಾವತ್( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ಯದಿ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ಪೂರ್ಣಾಙ್ಕ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ಅನ್ಯಥಾ
            {
                len = (ಪೂರ್ಣಾಙ್ಕ)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ಶೂನ್ಯ startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

ರಕ್ಷಿತ:
    FileStorage_API* fs;
};

ವರ್ಗ JSONParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ಆಭಾಸ ~JSONParser() {}

    ವರ್ಣ* skipSpaces( ವರ್ಣ* ptr )
    {
        ತಾರ್ಕಿಕ is_eof = ಅಸತ್ಯ;
        ತಾರ್ಕಿಕ is_completed = ಅಸತ್ಯ;

        ಯಾವತ್ ( is_eof == ಅಸತ್ಯ && is_completed == ಅಸತ್ಯ )
        {
            ಯದಿ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            ವಿಕಲ್ಪ ( *ptr )
            {
                /* comment */
                ಪಕ್ಷ '/' : {
                    ptr++;
                    ಯದಿ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ಯದಿ( !ptr || !*ptr ) { is_eof = ಸತ್ಯ; ಭಙ್ಗ; }
                    }

                    ಯದಿ ( *ptr == '/' )
                    {
                        ಯಾವತ್ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ಯದಿ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ಯದಿ( !ptr || !*ptr ) { is_eof = ಸತ್ಯ; ಭಙ್ಗ; }
                            }
                            ಅನ್ಯಥಾ
                            {
                                ptr++;
                            }
                        }
                    }
                    ಅನ್ಯಥಾ ಯದಿ ( *ptr == '*' )
                    {
                        ptr++;
                        ಪ್ರತಿ (;;)
                        {
                            ಯದಿ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ಯದಿ( !ptr || !*ptr ) { is_eof = ಸತ್ಯ; ಭಙ್ಗ; }
                            }
                            ಅನ್ಯಥಾ ಯದಿ ( *ptr == '*' )
                            {
                                ptr++;
                                ಯದಿ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ಯದಿ( !ptr || !*ptr ) { is_eof = ಸತ್ಯ; ಭಙ್ಗ; }
                                }
                                ಯದಿ ( *ptr == '/' )
                                {
                                    ptr++;
                                    ಭಙ್ಗ;
                                }
                            }
                            ಅನ್ಯಥಾ
                            {
                                ptr++;
                            }
                        }
                    }
                    ಅನ್ಯಥಾ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ಭಙ್ಗ;
                    /* whitespace */
                ಪಕ್ಷ '\t':
                ಪಕ್ಷ ' ' : {
                    ptr++;
                } ಭಙ್ಗ;
                    /* newline || end mark */
                ಪಕ್ಷ '\0':
                ಪಕ್ಷ '\n':
                ಪಕ್ಷ '\r': {
                    ptr = fs->gets();
                    ಯದಿ( !ptr || !*ptr ) { is_eof = ಸತ್ಯ; ಭಙ್ಗ; }
                } ಭಙ್ಗ;
                    /* other character */
                ಸ್ವಭಾವ: {
                    ಯದಿ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = ಸತ್ಯ;
                } ಭಙ್ಗ;
            }
        }

        ಯದಿ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ಯದಿ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    uint32_t parseUnicodeEscape(ವರ್ಣ*& ptr)
    {
        uint32_t codepoint = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            ವರ್ಣ hex = *ptr;
            uint32_t digit = 0;
            ಯದಿ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ಅನ್ಯಥಾ ಯದಿ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ಅನ್ಯಥಾ ಯದಿ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ಅನ್ಯಥಾ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ಪ್ರತ್ಯಾಗಮ codepoint;
    }

    ಸ್ಥಿರ ಶೂನ್ಯ appendUtf8(std::string& out, uint32_t codepoint)
    {
        ಯದಿ (codepoint < 0x80) {
            out += (ವರ್ಣ)codepoint;
        } ಅನ್ಯಥಾ ಯದಿ (codepoint < 0x800) {
            out += (ವರ್ಣ)(0xC0 | (codepoint >> 6));
            out += (ವರ್ಣ)(0x80 | (codepoint & 0x3F));
        } ಅನ್ಯಥಾ ಯದಿ (codepoint < 0x10000) {
            out += (ವರ್ಣ)(0xE0 | (codepoint >> 12));
            out += (ವರ್ಣ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ವರ್ಣ)(0x80 | (codepoint & 0x3F));
        } ಅನ್ಯಥಾ {
            out += (ವರ್ಣ)(0xF0 | (codepoint >> 18));
            out += (ವರ್ಣ)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (ವರ್ಣ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ವರ್ಣ)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    ಶೂನ್ಯ parseUnicodeEscapeToUtf8(ವರ್ಣ*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ಯದಿ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ಯದಿ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t ಕೆಳಗಿನ = parseUnicodeEscape(ptr);
            ಯದಿ (ಕೆಳಗಿನ < 0xDC00 || ಕೆಳಗಿನ > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (ಕೆಳಗಿನ - 0xDC00u);
        }
        ಅನ್ಯಥಾ ಯದಿ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    ವರ್ಣ* parse_ಕೀಲಿ( ವರ್ಣ* ptr, File_ಗಂಟು& collection, File_ಗಂಟು& ಮೌಲ್ಯ_placeholder )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಯದಿ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        ವರ್ಣ * beg = ptr + 1;
        std::string ಕೀಲಿ_name;
        ಕುರು {
            ಯದಿ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ಯದಿ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, ಕೀಲಿ_name);
                    ಅನುವರ್ತ;
                }
                ಕೀಲಿ_name += *ptr;
            } ಅನ್ಯಥಾ ಯದಿ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ಕೀಲಿ_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ಯಾವತ್( cv_isprint(*ptr) && *ptr != '"' );

        ಯದಿ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ಯದಿ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        ಮೌಲ್ಯ_placeholder = fs->add_ಗಂಟು(collection, ಕೀಲಿ_name, File_ಗಂಟು::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ಯದಿ( !ptr || !*ptr )
            ಪ್ರತ್ಯಾಗಮ 0;

        ಯದಿ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ಪ್ರತ್ಯಾಗಮ ++ptr;
    }

    ತಾರ್ಕಿಕ getBase64Row(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ /*indent*/, ವರ್ಣ* &beg, ವರ್ಣ* &end)
    {
        beg = end = ptr;
        ಯದಿ( !ptr || !*ptr )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        // find end of the row
        ಯಾವತ್( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ಯದಿ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ವರ್ಣ* parse_ಮೌಲ್ಯ( ವರ್ಣ* ptr, File_ಗಂಟು& ಗಂಟು )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ಯದಿ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ಯದಿ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            ವರ್ಣ * beg = ptr;
            size_t len = 0u;
            ಪ್ರತಿ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ಯದಿ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ಗಂಟು);

                ಯದಿ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ಅನ್ಯಥಾ
                    ptr++;
            }
            ಅನ್ಯಥಾ
            {   /**************** normal string ****************/
                ಪೂರ್ಣಾಙ್ಕ i = 0, sz;

                ptr = beg;
                ತಾರ್ಕಿಕ is_matching = ಅಸತ್ಯ;
                ಯಾವತ್ ( !is_matching )
                {
                    ವಿಕಲ್ಪ ( *ptr )
                    {
                        ಪಕ್ಷ '\\':
                        {
                            sz = (ಪೂರ್ಣಾಙ್ಕ)(ptr - beg);
                            ಯದಿ( sz > 0 )
                            {
                                ಯದಿ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ಯದಿ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            ವಿಕಲ್ಪ ( *ptr )
                            {
                            ಪಕ್ಷ '\\':
                            ಪಕ್ಷ '\"':
                            ಪಕ್ಷ '\'': { buf[i++] = *ptr; ಭಙ್ಗ; }
                            ಪಕ್ಷ 'n' : { buf[i++] = '\n'; ಭಙ್ಗ; }
                            ಪಕ್ಷ 'r' : { buf[i++] = '\r'; ಭಙ್ಗ; }
                            ಪಕ್ಷ 't' : { buf[i++] = '\t'; ಭಙ್ಗ; }
                            ಪಕ್ಷ 'b' : { buf[i++] = '\b'; ಭಙ್ಗ; }
                            ಪಕ್ಷ 'f' : { buf[i++] = '\f'; ಭಙ್ಗ; }
                            ಪಕ್ಷ 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                ಯದಿ (i + (ಪೂರ್ಣಾಙ್ಕ)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (ಪೂರ್ಣಾಙ್ಕ)utf8.size();
                                beg = ptr;
                                ಅನುವರ್ತ;
                            }
                            ಸ್ವಭಾವ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ಭಙ್ಗ;
                            }
                            ptr++;
                            beg = ptr;
                            ಭಙ್ಗ;
                        }
                        ಪಕ್ಷ '\0':
                        {
                            sz = (ಪೂರ್ಣಾಙ್ಕ)(ptr - beg);
                            ಯದಿ( sz > 0 )
                            {
                                ಯದಿ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ಯದಿ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ಭಙ್ಗ;
                        }
                        ಪಕ್ಷ '\"':
                        {
                            sz = (ಪೂರ್ಣಾಙ್ಕ)(ptr - beg);
                            ಯದಿ( sz > 0 )
                            {
                                ಯದಿ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = ಸತ್ಯ;
                            ಭಙ್ಗ;
                        }
                        ಪಕ್ಷ '\n':
                        ಪಕ್ಷ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ಭಙ್ಗ;
                        }
                        ಸ್ವಭಾವ:
                        {
                            ptr++;
                            ಭಙ್ಗ;
                        }
                    }
                }

                ಯದಿ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ಅನ್ಯಥಾ
                    ptr++;

                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, buf, i);
            }
        }
        ಅನ್ಯಥಾ ಯದಿ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            ವರ್ಣ * beg = ptr;
            ಯದಿ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ಯಾವತ್( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ಯದಿ (*ptr == '.' || *ptr == 'e')
            {
                ದ್ವಿಗುಣ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
            }
            ಅನ್ಯಥಾ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }

            ಯದಿ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ಅನ್ಯಥಾ
        {    /**************** other data ****************/
            ಧ್ರುವ ವರ್ಣ* beg = ptr;
            ಪೂರ್ಣಾಙ್ಕ len = 0;
            ಪ್ರತಿ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ಯದಿ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ಅನ್ಯಥಾ ಯದಿ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }
            ಅನ್ಯಥಾ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ವರ್ಣ* parseSeq( ವರ್ಣ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಙ್ಕ depth = 0 )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ಯದಿ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ಯದಿ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ಅನ್ಯಥಾ
            ptr++;

        fs->convertToCollection(File_ಗಂಟು::SEQ, ಗಂಟು);

        ಪ್ರತಿ (;;)
        {
            ptr = skipSpaces( ptr );
            ಯದಿ( !ptr || !*ptr )
                ಭಙ್ಗ;

            ಯದಿ ( *ptr != ']' )
            {
                File_ಗಂಟು child = fs->add_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE );

                ಯದಿ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ಅನ್ಯಥಾ ಯದಿ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ಅನ್ಯಥಾ
                    ptr = parse_ಮೌಲ್ಯ( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ಯದಿ( !ptr || !*ptr )
                ಭಙ್ಗ;

            ಯದಿ ( *ptr == ',' )
                ptr++;
            ಅನ್ಯಥಾ ಯದಿ ( *ptr == ']' )
                ಭಙ್ಗ;
            ಅನ್ಯಥಾ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಯದಿ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ಅನ್ಯಥಾ
            ptr++;

        fs->finalizeCollection(ಗಂಟು);
        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ವರ್ಣ* parseMap( ವರ್ಣ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಙ್ಕ depth = 0 )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಯದಿ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ಯದಿ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ಅನ್ಯಥಾ
            ptr++;

        fs->convertToCollection(File_ಗಂಟು::MAP, ಗಂಟು);

        ಪ್ರತಿ( ;; )
        {
            ptr = skipSpaces( ptr );
            ಯದಿ( !ptr || !*ptr )
                ಭಙ್ಗ;

            ಯದಿ ( *ptr == '"' )
            {
                File_ಗಂಟು child;
                ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, child );
                ಯದಿ( !ptr || !*ptr )
                    ಭಙ್ಗ;
                ptr = skipSpaces( ptr );
                ಯದಿ( !ptr || !*ptr )
                    ಭಙ್ಗ;

                ಯದಿ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ಅನ್ಯಥಾ ಯದಿ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ಅನ್ಯಥಾ
                    ptr = parse_ಮೌಲ್ಯ( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ಯದಿ( !ptr || !*ptr )
                ಭಙ್ಗ;

            ಯದಿ ( *ptr == ',' )
                ptr++;
            ಅನ್ಯಥಾ ಯದಿ ( *ptr == '}' )
                ಭಙ್ಗ;
            ಅನ್ಯಥಾ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಯದಿ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ಅನ್ಯಥಾ
            ptr++;

        fs->finalizeCollection(ಗಂಟು);
        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ತಾರ್ಕಿಕ parse( ವರ್ಣ* ptr )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ಯದಿ ( !ptr || !*ptr )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಯದಿ( *ptr == '{' )
        {
            File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->add_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::MAP);
            parseMap( ptr, ಬೇರು_ಗಂಟು );
        }
        ಅನ್ಯಥಾ ಯದಿ ( *ptr == '[' )
        {
            File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->add_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::SEQ);
            parseSeq( ptr, ಬೇರು_ಗಂಟು );
        }
        ಅನ್ಯಥಾ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    FileStorage_API* fs;
    ವರ್ಣ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<JSONParser>(fs);
}

}
