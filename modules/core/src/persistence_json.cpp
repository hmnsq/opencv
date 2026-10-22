// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਗ JSONEmitter : ਜਨਤਕ FileStorageEmitter
{
ਜਨਤਕ:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ਆਭਾਸੀ ~JSONEmitter() {}

    FStruct_ਅੰਕੜੇ ਸ਼ੁਰੂ_Write_Struct( ਅਚਲ FStruct_ਅੰਕੜੇ& ਮਾਪਾ, ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ,
                                  ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ* type_name=0 )
    {
        ਅੱਖਰ ਅੰਕੜੇ[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ਗੰਢ::TYPE_ਨਕਾਬ|File_ਗੰਢ::FLOW)) | File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
        ਜੇ( !File_ਗੰਢ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ਜੇ( type_name && *type_name == '\0' )
            type_name = 0;

        ਤਾਰਕਿਕ is_real_collection = ਸੱਚ;
        ਜੇ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ਗੰਢ::STR;
            ਅੰਕੜੇ[0] = '\0';
            is_real_collection = ਝੂਠ;
        }

        ਜੇ ( is_real_collection )
        {
            ਅੱਖਰ c = File_ਗੰਢ::isMap(struct_flags) ? '{' : '[';
            ਅੰਕੜੇ[0] = c;
            ਅੰਕੜੇ[1] = '\0';
        }

        writeScalar( ਕੁੰਜੀ, ਅੰਕੜੇ );
        FStruct_ਅੰਕੜੇ current_struct("", struct_flags, ਮਾਪਾ.indent + 4);

        ਵਾਪਸ current_struct;
    }

    ਖਾਲੀ ਸਿਰਾ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& current_struct)
    {
        ਪੂਰਨ struct_flags = current_struct.flags;

        ਜੇ (File_ਗੰਢ::isCollection(struct_flags)) {
            ਜੇ (!File_ਗੰਢ::isFlow(struct_flags)) {
                ਜੇ (fs->ਬਫਰ_Ptr() <= fs->ਬਫਰ_ਸ਼ੁਰੂ() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    ਅੱਖਰ *ptr = fs->ਬਫਰ_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->ਬਫਰ_ਸ਼ੁਰੂ());
                    fs->ਸੈੱਟ_ਬਫਰ_Ptr(fs->ਬਫਰ_ਸ਼ੁਰੂ());
                }
                fs->flush();
            }

            ਅੱਖਰ *ptr = fs->ਬਫਰ_Ptr();
            ਜੇ (ptr > fs->ਬਫਰ_ਸ਼ੁਰੂ() + current_struct.indent && !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ਗੰਢ::isMap(struct_flags) ? '}' : ']';
            fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
        }
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ value)
    {
        ਅੱਖਰ buf[128];
        writeScalar( ਕੁੰਜੀ, fs::itoa( value, buf, 10 ));
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, int64_t value)
    {
        ਅੱਖਰ buf[128];
        writeScalar( ਕੁੰਜੀ, fs::itoa( value, buf, 10, ਸੱਚ ));
    }

    ਖਾਲੀ ਲਿਖ( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਦੁੱਗਣਾ value )
    {
        ਅੱਖਰ buf[128];
        writeScalar( ਕੁੰਜੀ, fs::double_ਮੰਜ਼ਿਲ_String( buf, ਆਕਾਰ(buf), value, ਸੱਚ ));
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* str, ਤਾਰਕਿਕ quote)
    {
        ਅੱਖਰ buf[CV_FS_MAX_LEN*6+16];
        ਅੱਖਰ* ਅੰਕੜੇ = (ਅੱਖਰ*)str;
        ਪੂਰਨ i, len;

        ਜੇ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(str);
        ਜੇ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ਜੇ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ਪੂਰਨ need_quote = 1;
            ਅੰਕੜੇ = buf;
            *ਅੰਕੜੇ++ = '\"';
            ਲਈ( i = 0; i < len; i++ )
            {
                ਅੱਖਰ c = str[i];
                ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ byte = static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(c);

                ਚੋਣ ( c )
                {
                ਹਾਲਤ '\\':
                ਹਾਲਤ '\"': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = c;   ਰੋਕੋ; }
                ਹਾਲਤ '\n': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 'n'; ਰੋਕੋ; }
                ਹਾਲਤ '\r': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 'r'; ਰੋਕੋ; }
                ਹਾਲਤ '\t': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 't'; ਰੋਕੋ; }
                ਹਾਲਤ '\b': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 'b'; ਰੋਕੋ; }
                ਹਾਲਤ '\f': { *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 'f'; ਰੋਕੋ; }
                ਮੂਲ  :
                    ਜੇ (byte < 0x20)
                    {
                        ਸਥਿਰ ਅਚਲ ਅੱਖਰ hex[] = "0123456789abcdef";
                        *ਅੰਕੜੇ++ = '\\'; *ਅੰਕੜੇ++ = 'u'; *ਅੰਕੜੇ++ = '0'; *ਅੰਕੜੇ++ = '0';
                        *ਅੰਕੜੇ++ = hex[byte >> 4]; *ਅੰਕੜੇ++ = hex[byte & 0xf];
                    }
                    ਨਹੀਂਤਾਂ
                        *ਅੰਕੜੇ++ = c;
                    ਰੋਕੋ;
                }
            }

            *ਅੰਕੜੇ++ = '\"';
            *ਅੰਕੜੇ++ = '\0';
            ਅੰਕੜੇ = buf + !need_quote;
        }

        writeScalar( ਕੁੰਜੀ, ਅੰਕੜੇ);
    }

    ਖਾਲੀ writeScalar(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* ਅੰਕੜੇ)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ਝੂਠ);
        ਜੇ ( fs->get_state_ਦਾ_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_ਮੰਜ਼ਿਲ_Base64_state( FileStorage_API::NotUse );
        }
        ਨਹੀਂਤਾਂ ਜੇ ( fs->get_state_ਦਾ_writing_base64() == FileStorage_API::ਵਿੱਚ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ਕੁੰਜੀ_len = 0u;
        ਜੇ( ਕੁੰਜੀ && *ਕੁੰਜੀ == '\0' )
            ਕੁੰਜੀ = 0;
        ਜੇ ( ਕੁੰਜੀ )
        {
            ਕੁੰਜੀ_len = ਲੜੀ_ਲੰਬਾਈ(ਕੁੰਜੀ);
            ਜੇ ( ਕੁੰਜੀ_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ਨਹੀਂਤਾਂ ਜੇ ( static_cast<ਪੂਰਨ>(ਕੁੰਜੀ_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t ਅੰਕੜੇ_len = 0u;
        ਜੇ ( ਅੰਕੜੇ )
            ਅੰਕੜੇ_len = ਲੜੀ_ਲੰਬਾਈ(ਅੰਕੜੇ);

        FStruct_ਅੰਕੜੇ& current_struct = fs->getCurrentStruct();
        ਪੂਰਨ struct_flags = current_struct.flags;
        ਜੇ( File_ਗੰਢ::isCollection(struct_flags) )
        {
            ਜੇ ( (File_ਗੰਢ::isMap(struct_flags) ^ (ਕੁੰਜੀ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ਨਹੀਂਤਾਂ {
            fs->ਸੈੱਟ_Non_ਖਾਲੀ_ਥਾਵਾਂ();
            struct_flags = File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2 | (ਕੁੰਜੀ ? File_ਗੰਢ::MAP : File_ਗੰਢ::SEQ);
        }

        // start to write
        ਅੱਖਰ* ptr = 0;

        ਜੇ( File_ਗੰਢ::isFlow(struct_flags) )
        {
            ਪੂਰਨ new_ਵਿੱਥ;
            ptr = fs->ਬਫਰ_Ptr();
            ਜੇ( !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
                *ptr++ = ',';
            new_ਵਿੱਥ = static_cast<ਪੂਰਨ>(ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() + ਕੁੰਜੀ_len + ਅੰਕੜੇ_len);
            ਜੇ( new_ਵਿੱਥ > fs->wrapMargin() && new_ਵਿੱਥ - current_struct.indent > 10 )
            {
                fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
                ptr = fs->flush();
            }
            ਨਹੀਂਤਾਂ
                *ptr++ = ' ';
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ ( !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
            {
                ptr = fs->ਬਫਰ_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->ਬਫਰ_ਸ਼ੁਰੂ() );
                fs->ਸੈੱਟ_ਬਫਰ_Ptr(fs->ਬਫਰ_ਸ਼ੁਰੂ());
            }
            ptr = fs->flush();
        }

        ਜੇ( ਕੁੰਜੀ )
        {
            ਜੇ( !cv_isalpha(ਕੁੰਜੀ[0]) && ਕੁੰਜੀ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_ਬਫਰ( ptr, static_cast<ਪੂਰਨ>(ਕੁੰਜੀ_len) );
            *ptr++ = '\"';

            ਲਈ( size_t i = 0u; i < ਕੁੰਜੀ_len; i++ )
            {
                ਅੱਖਰ c = ਕੁੰਜੀ[i];

                ptr[i] = c;
                ਜੇ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ਕੁੰਜੀ_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ਜੇ( ਅੰਕੜੇ )
        {
            ptr = fs->resize_Write_ਬਫਰ( ptr, static_cast<ਪੂਰਨ>(ਅੰਕੜੇ_len) );
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, ਅੰਕੜੇ, ਅੰਕੜੇ_len );
            ptr += ਅੰਕੜੇ_len;
        }

        fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
        current_struct.flags &= ~File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
    }

    ਖਾਲੀ writeComment(ਅਚਲ ਅੱਖਰ* comment, ਤਾਰਕਿਕ eol_comment)
    {
        ਜੇ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ਪੂਰਨ len = static_cast<ਪੂਰਨ>(ਲੜੀ_ਲੰਬਾਈ(comment));
        ਅੱਖਰ* ptr = fs->ਬਫਰ_Ptr();
        ਅਚਲ ਅੱਖਰ* eol = strchr(comment, '\n');
        ਤਾਰਕਿਕ multiline = eol != 0;

        ਜੇ( !eol_comment || multiline || fs->ਬਫਰ_ਸਿਰਾ_2() - ptr < len || ptr == fs->ਬਫਰ_ਸ਼ੁਰੂ() )
            ptr = fs->flush();
        ਨਹੀਂਤਾਂ
            *ptr++ = ' ';

        ਜਦੋਂ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ਜੇ( eol )
            {
                ptr = fs->resize_Write_ਬਫਰ( ptr, (ਪੂਰਨ)(eol - comment) + 1 );
                ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, comment, eol - comment + 1 );
                fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ਨਹੀਂਤਾਂ
            {
                len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(comment);
                ptr = fs->resize_Write_ਬਫਰ( ptr, len );
                ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, comment, len );
                fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ਖਾਲੀ ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

ਸੁਰੱਖਿਅਤ:
    FileStorage_API* fs;
};

ਵਰਗ JSONParser : ਜਨਤਕ FileStorageParser
{
ਜਨਤਕ:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ਆਭਾਸੀ ~JSONParser() {}

    ਅੱਖਰ* skipSpaces( ਅੱਖਰ* ptr )
    {
        ਤਾਰਕਿਕ is_eof = ਝੂਠ;
        ਤਾਰਕਿਕ is_completed = ਝੂਠ;

        ਜਦੋਂ ( is_eof == ਝੂਠ && is_completed == ਝੂਠ )
        {
            ਜੇ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            ਚੋਣ ( *ptr )
            {
                /* comment */
                ਹਾਲਤ '/' : {
                    ptr++;
                    ਜੇ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ਜੇ( !ptr || !*ptr ) { is_eof = ਸੱਚ; ਰੋਕੋ; }
                    }

                    ਜੇ ( *ptr == '/' )
                    {
                        ਜਦੋਂ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ਜੇ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ਜੇ( !ptr || !*ptr ) { is_eof = ਸੱਚ; ਰੋਕੋ; }
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                ptr++;
                            }
                        }
                    }
                    ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '*' )
                    {
                        ptr++;
                        ਲਈ (;;)
                        {
                            ਜੇ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ਜੇ( !ptr || !*ptr ) { is_eof = ਸੱਚ; ਰੋਕੋ; }
                            }
                            ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '*' )
                            {
                                ptr++;
                                ਜੇ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ਜੇ( !ptr || !*ptr ) { is_eof = ਸੱਚ; ਰੋਕੋ; }
                                }
                                ਜੇ ( *ptr == '/' )
                                {
                                    ptr++;
                                    ਰੋਕੋ;
                                }
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                ptr++;
                            }
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ਰੋਕੋ;
                    /* whitespace */
                ਹਾਲਤ '\t':
                ਹਾਲਤ ' ' : {
                    ptr++;
                } ਰੋਕੋ;
                    /* newline || end mark */
                ਹਾਲਤ '\0':
                ਹਾਲਤ '\n':
                ਹਾਲਤ '\r': {
                    ptr = fs->gets();
                    ਜੇ( !ptr || !*ptr ) { is_eof = ਸੱਚ; ਰੋਕੋ; }
                } ਰੋਕੋ;
                    /* other character */
                ਮੂਲ: {
                    ਜੇ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = ਸੱਚ;
                } ਰੋਕੋ;
            }
        }

        ਜੇ ( is_eof || !is_completed )
        {
            ptr = fs->ਬਫਰ_ਸ਼ੁਰੂ();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->ਸੈੱਟ_Eof();
            ਜੇ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ਵਾਪਸ ptr;
    }

    uint32_t parseUnicodeEscape(ਅੱਖਰ*& ptr)
    {
        uint32_t codepoint = 0;
        ਲਈ (ਪੂਰਨ k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
            ਅੱਖਰ hex = *ptr;
            uint32_t digit = 0;
            ਜੇ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ਨਹੀਂਤਾਂ ਜੇ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ਨਹੀਂਤਾਂ ਜੇ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ਨਹੀਂਤਾਂ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ਵਾਪਸ codepoint;
    }

    ਸਥਿਰ ਖਾਲੀ appendUtf8(std::string& out, uint32_t codepoint)
    {
        ਜੇ (codepoint < 0x80) {
            out += (ਅੱਖਰ)codepoint;
        } ਨਹੀਂਤਾਂ ਜੇ (codepoint < 0x800) {
            out += (ਅੱਖਰ)(0xC0 | (codepoint >> 6));
            out += (ਅੱਖਰ)(0x80 | (codepoint & 0x3F));
        } ਨਹੀਂਤਾਂ ਜੇ (codepoint < 0x10000) {
            out += (ਅੱਖਰ)(0xE0 | (codepoint >> 12));
            out += (ਅੱਖਰ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ਅੱਖਰ)(0x80 | (codepoint & 0x3F));
        } ਨਹੀਂਤਾਂ {
            out += (ਅੱਖਰ)(0xF0 | (codepoint >> 18));
            out += (ਅੱਖਰ)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (ਅੱਖਰ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ਅੱਖਰ)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    ਖਾਲੀ parse_Unicode_Escape_ਮੰਜ਼ਿਲ_Utf8(ਅੱਖਰ*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ਜੇ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ਜੇ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t ਨੀਵਾਂ = parseUnicodeEscape(ptr);
            ਜੇ (ਨੀਵਾਂ < 0xDC00 || ਨੀਵਾਂ > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (ਨੀਵਾਂ - 0xDC00u);
        }
        ਨਹੀਂਤਾਂ ਜੇ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    ਅੱਖਰ* parse_ਕੁੰਜੀ( ਅੱਖਰ* ptr, File_ਗੰਢ& collection, File_ਗੰਢ& ਮੁੱਲ_placeholder )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਜੇ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        ਅੱਖਰ * beg = ptr + 1;
        std::string ਕੁੰਜੀ_name;
        ਕਰੋ {
            ਜੇ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
                ਜੇ (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_ਮੰਜ਼ਿਲ_Utf8(ptr, ਕੁੰਜੀ_name);
                    ਜਾਰੀ;
                }
                ਕੁੰਜੀ_name += *ptr;
            } ਨਹੀਂਤਾਂ ਜੇ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ਕੁੰਜੀ_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
        } ਜਦੋਂ( cv_isprint(*ptr) && *ptr != '"' );

        ਜੇ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ਜੇ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        ਮੁੱਲ_placeholder = fs->ਜੋੜੋ_ਗੰਢ(collection, ਕੁੰਜੀ_name, File_ਗੰਢ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ਜੇ( !ptr || !*ptr )
            ਵਾਪਸ 0;

        ਜੇ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ਵਾਪਸ ++ptr;
    }

    ਤਾਰਕਿਕ get_Base64_ਪੰਕਤੀ(ਅੱਖਰ* ptr, ਪੂਰਨ /*indent*/, ਅੱਖਰ* &beg, ਅੱਖਰ* &end)
    {
        beg = end = ptr;
        ਜੇ( !ptr || !*ptr )
            ਵਾਪਸ ਝੂਠ;

        // find end of the row
        ਜਦੋਂ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ਜੇ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ਵਾਪਸ ਸੱਚ;
    }

    ਅੱਖਰ* parse_ਮੁੱਲ( ਅੱਖਰ* ptr, File_ਗੰਢ& ਗੰਢ )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ਜੇ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ਜੇ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            ਅੱਖਰ * beg = ptr;
            size_t len = 0u;
            ਲਈ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ਜੇ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ਗੰਢ);

                ਜੇ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ਨਹੀਂਤਾਂ
                    ptr++;
            }
            ਨਹੀਂਤਾਂ
            {   /**************** normal string ****************/
                ਪੂਰਨ i = 0, sz;

                ptr = beg;
                ਤਾਰਕਿਕ is_matching = ਝੂਠ;
                ਜਦੋਂ ( !is_matching )
                {
                    ਚੋਣ ( *ptr )
                    {
                        ਹਾਲਤ '\\':
                        {
                            sz = (ਪੂਰਨ)(ptr - beg);
                            ਜੇ( sz > 0 )
                            {
                                ਜੇ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ਜੇ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            ਚੋਣ ( *ptr )
                            {
                            ਹਾਲਤ '\\':
                            ਹਾਲਤ '\"':
                            ਹਾਲਤ '\'': { buf[i++] = *ptr; ਰੋਕੋ; }
                            ਹਾਲਤ 'n' : { buf[i++] = '\n'; ਰੋਕੋ; }
                            ਹਾਲਤ 'r' : { buf[i++] = '\r'; ਰੋਕੋ; }
                            ਹਾਲਤ 't' : { buf[i++] = '\t'; ਰੋਕੋ; }
                            ਹਾਲਤ 'b' : { buf[i++] = '\b'; ਰੋਕੋ; }
                            ਹਾਲਤ 'f' : { buf[i++] = '\f'; ਰੋਕੋ; }
                            ਹਾਲਤ 'u' : {
                                ptr++;
                                std::string utf8;
                                parse_Unicode_Escape_ਮੰਜ਼ਿਲ_Utf8(ptr, utf8);
                                ਜੇ (i + (ਪੂਰਨ)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf + i, utf8.ਅੰਕੜੇ(), utf8.size());
                                i += (ਪੂਰਨ)utf8.size();
                                beg = ptr;
                                ਜਾਰੀ;
                            }
                            ਮੂਲ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ਰੋਕੋ;
                            }
                            ptr++;
                            beg = ptr;
                            ਰੋਕੋ;
                        }
                        ਹਾਲਤ '\0':
                        {
                            sz = (ਪੂਰਨ)(ptr - beg);
                            ਜੇ( sz > 0 )
                            {
                                ਜੇ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ਜੇ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ਰੋਕੋ;
                        }
                        ਹਾਲਤ '\"':
                        {
                            sz = (ਪੂਰਨ)(ptr - beg);
                            ਜੇ( sz > 0 )
                            {
                                ਜੇ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = ਸੱਚ;
                            ਰੋਕੋ;
                        }
                        ਹਾਲਤ '\n':
                        ਹਾਲਤ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ਰੋਕੋ;
                        }
                        ਮੂਲ:
                        {
                            ptr++;
                            ਰੋਕੋ;
                        }
                    }
                }

                ਜੇ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ਨਹੀਂਤਾਂ
                    ptr++;

                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::STRING, buf, i);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            ਅੱਖਰ * beg = ptr;
            ਜੇ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
            }
            ਜਦੋਂ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
            }
            ਜੇ (*ptr == '.' || *ptr == 'e')
            {
                ਦੁੱਗਣਾ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();

                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::REAL, &fval);
            }
            ਨਹੀਂਤਾਂ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();

                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
            }

            ਜੇ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ਨਹੀਂਤਾਂ
        {    /**************** other data ****************/
            ਅਚਲ ਅੱਖਰ* beg = ptr;
            ਪੂਰਨ len = 0;
            ਲਈ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
            }

            ਜੇ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ਨਹੀਂਤਾਂ ਜੇ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
            }
            ਨਹੀਂਤਾਂ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ਵਾਪਸ ptr;
    }

    ਅੱਖਰ* parseSeq( ਅੱਖਰ* ptr, File_ਗੰਢ& ਗੰਢ, ਪੂਰਨ ਡੂੰਘਾਈ = 0 )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ਜੇ( ਡੂੰਘਾਈ > CV_PERSISTENCE_MAX_ਡੂੰਘਾਈ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ਜੇ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ਨਹੀਂਤਾਂ
            ptr++;

        fs->convert_ਮੰਜ਼ਿਲ_Collection(File_ਗੰਢ::SEQ, ਗੰਢ);

        ਲਈ (;;)
        {
            ptr = skipSpaces( ptr );
            ਜੇ( !ptr || !*ptr )
                ਰੋਕੋ;

            ਜੇ ( *ptr != ']' )
            {
                File_ਗੰਢ ਬੱਚਾ = fs->ਜੋੜੋ_ਗੰਢ(ਗੰਢ, std::string(), File_ਗੰਢ::NONE );

                ਜੇ ( *ptr == '[' )
                    ptr = parseSeq( ptr, ਬੱਚਾ, ਡੂੰਘਾਈ + 1 );
                ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '{' )
                    ptr = parseMap( ptr, ਬੱਚਾ, ਡੂੰਘਾਈ + 1 );
                ਨਹੀਂਤਾਂ
                    ptr = parse_ਮੁੱਲ( ptr, ਬੱਚਾ );
            }

            ptr = skipSpaces( ptr );
            ਜੇ( !ptr || !*ptr )
                ਰੋਕੋ;

            ਜੇ ( *ptr == ',' )
                ptr++;
            ਨਹੀਂਤਾਂ ਜੇ ( *ptr == ']' )
                ਰੋਕੋ;
            ਨਹੀਂਤਾਂ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ਜੇ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ਨਹੀਂਤਾਂ
            ptr++;

        fs->finalizeCollection(ਗੰਢ);
        ਵਾਪਸ ptr;
    }

    ਅੱਖਰ* parseMap( ਅੱਖਰ* ptr, File_ਗੰਢ& ਗੰਢ, ਪੂਰਨ ਡੂੰਘਾਈ = 0 )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ਜੇ( ਡੂੰਘਾਈ > CV_PERSISTENCE_MAX_ਡੂੰਘਾਈ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ਜੇ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ਨਹੀਂਤਾਂ
            ptr++;

        fs->convert_ਮੰਜ਼ਿਲ_Collection(File_ਗੰਢ::MAP, ਗੰਢ);

        ਲਈ( ;; )
        {
            ptr = skipSpaces( ptr );
            ਜੇ( !ptr || !*ptr )
                ਰੋਕੋ;

            ਜੇ ( *ptr == '"' )
            {
                File_ਗੰਢ ਬੱਚਾ;
                ptr = parse_ਕੁੰਜੀ( ptr, ਗੰਢ, ਬੱਚਾ );
                ਜੇ( !ptr || !*ptr )
                    ਰੋਕੋ;
                ptr = skipSpaces( ptr );
                ਜੇ( !ptr || !*ptr )
                    ਰੋਕੋ;

                ਜੇ ( *ptr == '[' )
                    ptr = parseSeq( ptr, ਬੱਚਾ, ਡੂੰਘਾਈ + 1 );
                ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '{' )
                    ptr = parseMap( ptr, ਬੱਚਾ, ਡੂੰਘਾਈ + 1 );
                ਨਹੀਂਤਾਂ
                    ptr = parse_ਮੁੱਲ( ptr, ਬੱਚਾ );
            }

            ptr = skipSpaces( ptr );
            ਜੇ( !ptr || !*ptr )
                ਰੋਕੋ;

            ਜੇ ( *ptr == ',' )
                ptr++;
            ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '}' )
                ਰੋਕੋ;
            ਨਹੀਂਤਾਂ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ਜੇ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ਨਹੀਂਤਾਂ
            ptr++;

        fs->finalizeCollection(ਗੰਢ);
        ਵਾਪਸ ptr;
    }

    ਤਾਰਕਿਕ parse( ਅੱਖਰ* ptr )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ਜੇ ( !ptr || !*ptr )
            ਵਾਪਸ ਝੂਠ;

        File_ਗੰਢ ਜੜ੍ਹ_collection(fs->getFS(), 0, 0);

        ਜੇ( *ptr == '{' )
        {
            File_ਗੰਢ ਜੜ੍ਹ_ਗੰਢ = fs->ਜੋੜੋ_ਗੰਢ(ਜੜ੍ਹ_collection, std::string(), File_ਗੰਢ::MAP);
            parseMap( ptr, ਜੜ੍ਹ_ਗੰਢ );
        }
        ਨਹੀਂਤਾਂ ਜੇ ( *ptr == '[' )
        {
            File_ਗੰਢ ਜੜ੍ਹ_ਗੰਢ = fs->ਜੋੜੋ_ਗੰਢ(ਜੜ੍ਹ_collection, std::string(), File_ਗੰਢ::SEQ);
            parseSeq( ptr, ਜੜ੍ਹ_ਗੰਢ );
        }
        ਨਹੀਂਤਾਂ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ਵਾਪਸ ਸੱਚ;
    }

    FileStorage_API* fs;
    ਅੱਖਰ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<JSONParser>(fs);
}

}
