// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"

ਗਿਣਤੀ
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਗ YAMLEmitter : ਜਨਤਕ FileStorageEmitter
{
ਜਨਤਕ:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ਆਭਾਸੀ ~YAMLEmitter() {}

    FStruct_ਅੰਕੜੇ ਸ਼ੁਰੂ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& ਮਾਪਾ, ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ,
                                 ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ* type_name=0)
    {
        ਅੱਖਰ buf[CV_FS_MAX_LEN + 1024];
        ਅਚਲ ਅੱਖਰ* ਅੰਕੜੇ = 0;

        ਜੇ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ਗੰਢ::TYPE_ਨਕਾਬ|File_ਗੰਢ::FLOW)) | File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
        ਜੇ( !File_ਗੰਢ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ਜੇ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ਗੰਢ::SEQ;
            snprintf(buf, ਆਕਾਰ(buf), "!!binary |");
            ਅੰਕੜੇ = buf;
        }
        ਨਹੀਂਤਾਂ ਜੇ( File_ਗੰਢ::isFlow(struct_flags))
        {
            ਅੱਖਰ c = File_ਗੰਢ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ਗੰਢ::FLOW;

            ਜੇ( type_name )
                snprintf( buf, ਆਕਾਰ(buf), "!!%s %c", type_name, c );
            ਨਹੀਂਤਾਂ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            ਅੰਕੜੇ = buf;
        }
        ਨਹੀਂਤਾਂ ਜੇ( type_name )
        {
            snprintf( buf, ਆਕਾਰ(buf), "!!%s", type_name );
            ਅੰਕੜੇ = buf;
        }

        writeScalar( ਕੁੰਜੀ, ਅੰਕੜੇ );

        FStruct_ਅੰਕੜੇ fsd;
        fsd.indent = ਮਾਪਾ.indent;
        fsd.flags = struct_flags;

        ਜੇ( !File_ਗੰਢ::isFlow(ਮਾਪਾ.flags) )
            fsd.indent += CV_YML_INDENT + File_ਗੰਢ::isFlow(struct_flags);

        ਵਾਪਸ fsd;
    }

    ਖਾਲੀ ਸਿਰਾ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& current_struct)
    {
        ਅੱਖਰ* ptr;

        ਪੂਰਨ struct_flags = current_struct.flags;

        ਜੇ( File_ਗੰਢ::isFlow(struct_flags) )
        {
            ptr = fs->ਬਫਰ_Ptr();
            ਜੇ( ptr > fs->ਬਫਰ_ਸ਼ੁਰੂ() + current_struct.indent && !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ਗੰਢ::isMap(struct_flags) ? '}' : ']';
            fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
        }
        ਨਹੀਂਤਾਂ ਜੇ( File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
        {
            ptr = fs->flush();
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, File_ਗੰਢ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
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
        writeScalar( ਕੁੰਜੀ, fs::double_ਮੰਜ਼ਿਲ_String( buf, ਆਕਾਰ(buf), value, ਝੂਠ ));
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* str, ਤਾਰਕਿਕ quote)
    {
        ਅੱਖਰ buf[CV_FS_MAX_LEN*4+16];
        ਅੱਖਰ* ਅੰਕੜੇ = (ਅੱਖਰ*)str;
        ਪੂਰਨ i, len;

        ਜੇ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(str);
        ਜੇ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ਜੇ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ਪੂਰਨ need_quote = quote || len == 0 || str[0] == ' ';
            ਅੰਕੜੇ = buf;
            *ਅੰਕੜੇ++ = '\"';
            ਲਈ( i = 0; i < len; i++ )
            {
                ਅੱਖਰ c = str[i];

                ਜੇ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ਜੇ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *ਅੰਕੜੇ++ = '\\';
                    ਜੇ( cv_isprint(c) )
                        *ਅੰਕੜੇ++ = c;
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\n' )
                        *ਅੰਕੜੇ++ = 'n';
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\r' )
                        *ਅੰਕੜੇ++ = 'r';
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\t' )
                        *ਅੰਕੜੇ++ = 't';
                    ਨਹੀਂਤਾਂ
                    {
                        snprintf( ਅੰਕੜੇ, ਆਕਾਰ(buf) - (ਅੰਕੜੇ - buf), "x%02x", c );
                        ਅੰਕੜੇ += 3;
                    }
                }
                ਨਹੀਂਤਾਂ
                    *ਅੰਕੜੇ++ = c;
            }
            ਜੇ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ਜੇ( need_quote )
                *ਅੰਕੜੇ++ = '\"';
            *ਅੰਕੜੇ++ = '\0';
            ਅੰਕੜੇ = buf + !need_quote;
        }

        writeScalar( ਕੁੰਜੀ, ਅੰਕੜੇ);
    }

    ਖਾਲੀ writeScalar(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* ਅੰਕੜੇ)
    {
        fs->check_if_write_struct_is_delayed(ਝੂਠ);
        ਜੇ ( fs->get_state_ਦਾ_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_ਮੰਜ਼ਿਲ_Base64_state( FileStorage_API::NotUse );
        }
        ਨਹੀਂਤਾਂ ਜੇ ( fs->get_state_ਦਾ_writing_base64() == FileStorage_API::ਵਿੱਚ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ਪੂਰਨ i, keylen = 0;
        ਪੂਰਨ datalen = 0;
        ਅੱਖਰ* ptr;

        FStruct_ਅੰਕੜੇ& current_struct = fs->getCurrentStruct();

        ਪੂਰਨ struct_flags = current_struct.flags;

        ਜੇ( ਕੁੰਜੀ && ਕੁੰਜੀ[0] == '\0' )
            ਕੁੰਜੀ = 0;

        ਜੇ( File_ਗੰਢ::isCollection(struct_flags) )
        {
            ਜੇ( (File_ਗੰਢ::isMap(struct_flags) ^ (ਕੁੰਜੀ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ਨਹੀਂਤਾਂ
        {
            fs->ਸੈੱਟ_Non_ਖਾਲੀ_ਥਾਵਾਂ();
            struct_flags = File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2 | (ਕੁੰਜੀ ? File_ਗੰਢ::MAP : File_ਗੰਢ::SEQ);
        }

        ਜੇ( ਕੁੰਜੀ )
        {
            keylen = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(ਕੁੰਜੀ);
            ਜੇ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ਜੇ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ਜੇ( ਅੰਕੜੇ )
            datalen = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(ਅੰਕੜੇ);

        ਜੇ( File_ਗੰਢ::isFlow(struct_flags) )
        {
            ptr = fs->ਬਫਰ_Ptr();
            ਜੇ( !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
                *ptr++ = ',';
            ਪੂਰਨ new_ਵਿੱਥ = (ਪੂਰਨ)(ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ()) + keylen + datalen;
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
            ptr = fs->flush();
            ਜੇ( !File_ਗੰਢ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ਜੇ( ਅੰਕੜੇ )
                    *ptr++ = ' ';
            }
        }

        ਜੇ( ਕੁੰਜੀ )
        {
            ਜੇ( !cv_isalpha(ਕੁੰਜੀ[0]) && ਕੁੰਜੀ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_ਬਫਰ( ptr, keylen );

            ਲਈ( i = 0; i < keylen; i++ )
            {
                ਅੱਖਰ c = ਕੁੰਜੀ[i];

                ptr[i] = c;
                ਜੇ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ਜੇ( !File_ਗੰਢ::isFlow(struct_flags) && ਅੰਕੜੇ )
                *ptr++ = ' ';
        }

        ਜੇ( ਅੰਕੜੇ )
        {
            ptr = fs->resize_Write_ਬਫਰ( ptr, datalen );
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, ਅੰਕੜੇ, datalen );
            ptr += datalen;
        }

        fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
        current_struct.flags &= ~File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
    }

    ਖਾਲੀ writeComment(ਅਚਲ ਅੱਖਰ* comment, ਤਾਰਕਿਕ eol_comment)
    {
        ਜੇ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ਪੂਰਨ len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(comment);
        ਅਚਲ ਅੱਖਰ* eol = strchr(comment, '\n');
        ਤਾਰਕਿਕ multiline = eol != 0;
        ਅੱਖਰ* ptr = fs->ਬਫਰ_Ptr();

        ਜੇ( !eol_comment || multiline ||
            fs->ਬਫਰ_ਸਿਰਾ_2() - ptr < len || ptr == fs->ਬਫਰ_ਸ਼ੁਰੂ() )
            ptr = fs->flush();
        ਨਹੀਂਤਾਂ
            *ptr++ = ' ';

        ਜਦੋਂ( comment )
        {
            *ptr++ = '#';
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


ਵਰਗ YAMLParser : ਜਨਤਕ FileStorageParser
{
ਜਨਤਕ:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ਆਭਾਸੀ ~YAMLParser() {}

    ਅੱਖਰ* skipSpaces( ਅੱਖਰ* ptr, ਪੂਰਨ min_indent, ਪੂਰਨ max_comment_indent )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਲਈ(;;)
        {
            ਜਦੋਂ( *ptr == ' ' )
                ptr++;
            ਜੇ( *ptr == '#' )
            {
                ਜੇ( ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() > max_comment_indent )
                    ਵਾਪਸ ptr;
                *ptr = '\0';
            }
            ਨਹੀਂਤਾਂ ਜੇ( cv_isprint(*ptr) )
            {
                ਜੇ( ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ਰੋਕੋ;
            }
            ਨਹੀਂਤਾਂ ਜੇ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ਜੇ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->ਬਫਰ_ਸ਼ੁਰੂ();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->ਸੈੱਟ_Eof();
                    ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਪੂਰਨ l = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(ptr);
                    ਜੇ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ਨਹੀਂਤਾਂ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ਵਾਪਸ ptr;
    }

    ਤਾਰਕਿਕ get_Base64_ਪੰਕਤੀ(ਅੱਖਰ* ptr, ਪੂਰਨ indent, ਅੱਖਰ* &beg, ਅੱਖਰ* &end)
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
        ਜੇ (!ptr || !*ptr)
            ਵਾਪਸ ਝੂਠ; // end of file

        ਜੇ (ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() != indent)
            ਵਾਪਸ ਝੂਠ; // end of base64 data

        /* find end */
        ਜਦੋਂ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ਜੇ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ਵਾਪਸ ਸੱਚ;
    }


    ਅੱਖਰ* parse_ਕੁੰਜੀ( ਅੱਖਰ* ptr, File_ਗੰਢ& map_ਗੰਢ, File_ਗੰਢ& ਮੁੱਲ_placeholder )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਅੱਖਰ c;
        ਅੱਖਰ *endptr = ptr - 1, *saveptr;

        ਜੇ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        ਕਰੋ c = *++endptr;
        ਜਦੋਂ( cv_isprint(c) && c != ':' );

        ਜੇ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ਜੇ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        ਕਰੋ c = *--endptr;
        ਜਦੋਂ( c == ' ' );

        ++endptr;

        ਮੁੱਲ_placeholder = fs->ਜੋੜੋ_ਗੰਢ(map_ਗੰਢ, std::string(ptr, endptr - ptr), File_ਗੰਢ::NONE);
        ptr = saveptr;

        ਵਾਪਸ ptr;
    }

    ਅੱਖਰ* parse_ਮੁੱਲ( ਅੱਖਰ* ptr, File_ਗੰਢ& ਗੰਢ, ਪੂਰਨ min_indent, ਤਾਰਕਿਕ is_ਮਾਪਾ_flow, ਪੂਰਨ ਡੂੰਘਾਈ = 0 )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਜੇ( ਡੂੰਘਾਈ > CV_PERSISTENCE_MAX_ਡੂੰਘਾਈ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ਅੱਖਰ* endptr = 0;
        ਅੱਖਰ c = ptr[0], d = ptr[1];
        ਪੂਰਨ value_type = File_ਗੰਢ::NONE;
        ਪੂਰਨ len;
        ਤਾਰਕਿਕ is_ਦੁਆਧਾਰੀ_string = ਝੂਠ;
        ਤਾਰਕਿਕ is_user = ਝੂਠ;

        ਜੇ( c == '!' ) // handle explicit type specification
        {
            ਜੇ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = ਸੱਚ;
                //value_type |= FileNode::USER;
            }
            ਜੇ ( d == '<') //support of full type heading from YAML 1.2
            {
                ਅਚਲ ਅੱਖਰ* yamlTypeHeading = "<tag:yaml.org,2002:";
                ਅਚਲ size_t heading_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(yamlTypeHeading);

                ਅੱਖਰ* type_ਸਿਰਾ_Ptr = ++ptr;

                ਕਰੋ d = *++type_ਸਿਰਾ_Ptr;
                ਜਦੋਂ( cv_isprint(d) && d != ' ' && d != '>' );

                ਜੇ ( d == '>' && (size_t)(type_ਸਿਰਾ_Ptr - ptr) > heading_ਲੰਮਾਈ )
                {
                    ਜੇ ( memcmp(ptr, yamlTypeHeading, heading_ਲੰਮਾਈ) == 0 )
                    {
                        *type_ਸਿਰਾ_Ptr = ' ';
                        ptr += heading_ਲੰਮਾਈ - 1;
                        is_user = ਸੱਚ;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            ਕਰੋ d = *++endptr;
            ਜਦੋਂ( cv_isprint(d) && d != ' ' );
            len = (ਪੂਰਨ)(endptr - ptr);
            ਜੇ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ਜੇ( len == 3 && !is_user )
            {
                ਜੇ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ਗੰਢ::STRING;
                ਨਹੀਂਤਾਂ ਜੇ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ਗੰਢ::INT;
                ਨਹੀਂਤਾਂ ਜੇ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ਗੰਢ::SEQ;
                ਨਹੀਂਤਾਂ ਜੇ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ਗੰਢ::MAP;
            }
            ਨਹੀਂਤਾਂ ਜੇ( len == 5 && !is_user )
            {
                ਜੇ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ਗੰਢ::REAL;
            }
            ਨਹੀਂਤਾਂ ਜੇ (len == 6 && is_user)
            {
                ਜੇ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ਗੰਢ::SEQ;
                    is_ਦੁਆਧਾਰੀ_string = ਸੱਚ;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    ਕਰੋ {
                        d = *++endptr;
                        ਜੇ (d == '|')
                            ਰੋਕੋ;
                    } ਜਦੋਂ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
            ਜੇ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ਜੇ( !is_user )
            {
                ਜੇ (value_type == File_ਗੰਢ::STRING && c != '\'' && c != '\"')
                    ਜਾਓ force_string;
                ਜੇ( value_type == File_ਗੰਢ::INT )
                    ਜਾਓ force_int;
                ਜੇ( value_type == File_ਗੰਢ::REAL )
                    ਜਾਓ force_real;
            }
        }

        ਜੇ (is_ਦੁਆਧਾਰੀ_string)
        {
            ਪੂਰਨ indent = static_cast<ਪੂਰਨ>(ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ());
            ptr = fs->parseBase64(ptr, indent, ਗੰਢ);
        }
        ਨਹੀਂਤਾਂ ਜੇ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ਜਦੋਂ( cv_isdigit(*endptr) )
                endptr++;
            ਜੇ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ਦੁੱਗਣਾ fval = fs->strtod( ptr, &endptr );
                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::REAL, &fval);
            }
            ਨਹੀਂਤਾਂ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
            }

            ਜੇ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
        }
        ਨਹੀਂਤਾਂ ਜੇ( c == '\'' || c == '\"' ) // an explicit string
        {
            ਜੇ( c == '\'' )
                ਲਈ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ਜੇ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\'' )
                    {
                        c = *++ptr;
                        ਜੇ( c != '\'' )
                            ਰੋਕੋ;
                        buf[len++] = c;
                    }
                    ਨਹੀਂਤਾਂ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ਨਹੀਂਤਾਂ
                ਲਈ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ਜੇ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\"' )
                    {
                        ++ptr;
                        ਰੋਕੋ;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\\' )
                    {
                        d = *++ptr;
                        ਜੇ( d == '\'' )
                            buf[len++] = d;
                        ਨਹੀਂਤਾਂ ਜੇ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ਨਹੀਂਤਾਂ ਜੇ( d == 'n' )
                            buf[len++] = '\n';
                        ਨਹੀਂਤਾਂ ਜੇ( d == 'r' )
                            buf[len++] = '\r';
                        ਨਹੀਂਤਾਂ ਜੇ( d == 't' )
                            buf[len++] = '\t';
                        ਨਹੀਂਤਾਂ ਜੇ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ਪੂਰਨ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ਪੂਰਨ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ਜੇ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ਨਹੀਂਤਾਂ
                            {
                                buf[len++] = (ਅੱਖਰ)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ਨਹੀਂਤਾਂ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ਜੇ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::STRING, buf, len);
        }
        ਨਹੀਂਤਾਂ ਜੇ( c == '[' || c == '{' ) // collection as a flow
        {
            ਪੂਰਨ new_min_indent = min_indent + !is_ਮਾਪਾ_flow;
            ਪੂਰਨ struct_type = c == '{' ? File_ਗੰਢ::MAP : File_ਗੰਢ::SEQ;
            ਪੂਰਨ nelems = 0;

            fs->convert_ਮੰਜ਼ਿਲ_Collection(struct_type, ਗੰਢ);
            d = c == '[' ? ']' : '}';

            ਲਈ( ++ptr ;; nelems++ )
            {
                File_ਗੰਢ elem;

                ptr = skipSpaces( ptr, new_min_indent, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                ਜੇ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ਜੇ( *ptr == '}' || *ptr == ']' )
                {
                    ਜੇ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ਰੋਕੋ;
                }

                ਜੇ( nelems != 0 )
                {
                    ਜੇ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                    ਜੇ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ਜੇ( struct_type == File_ਗੰਢ::MAP )
                {
                    ptr = parse_ਕੁੰਜੀ( ptr, ਗੰਢ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                }
                ਨਹੀਂਤਾਂ
                {
                    ਜੇ( *ptr == ']' )
                        ਰੋਕੋ;
                    elem = fs->ਜੋੜੋ_ਗੰਢ(ਗੰਢ, std::string(), File_ਗੰਢ::NONE);
                }
                ptr = parse_ਮੁੱਲ( ptr, elem, new_min_indent, ਸੱਚ, ਡੂੰਘਾਈ + 1 );
            }
            fs->finalizeCollection(ਗੰਢ);
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ indent, struct_type;

            ਜੇ( is_ਮਾਪਾ_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ਜੇ( !is_ਮਾਪਾ_flow )
                {
                    ਜੇ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ਜੇ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                ਕਰੋ c = *++endptr;
                ਜਦੋਂ( cv_isprint(c) &&
                      (!is_ਮਾਪਾ_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_ਮਾਪਾ_flow || c != ':' || value_type == File_ਗੰਢ::STRING));

                ਜੇ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ਜੇ( is_ਮਾਪਾ_flow || c != ':' )
                {
                    ਅੱਖਰ* str_ਸਿਰਾ = endptr;
                    ਕਰੋ c = *--str_ਸਿਰਾ;
                    ਜਦੋਂ( str_ਸਿਰਾ > ptr && c == ' ' );
                    str_ਸਿਰਾ++;
                    ਜੇ ((fs->getFormat() & FileStorage::FORMAT_ਨਕਾਬ) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_ਸਿਰਾ - ptr;
                        ਜੇ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
                            ptr = endptr;
                            ਵਾਪਸ ptr;
                        }
                        ਨਹੀਂਤਾਂ ਜੇ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
                            ptr = endptr;
                            ਵਾਪਸ ptr;
                        }
                        ਨਹੀਂਤਾਂ ਜੇ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ਵਾਪਸ ptr;
                        }
                    }

                    ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::STRING, ptr, (ਪੂਰਨ)(str_ਸਿਰਾ - ptr));
                    ptr = endptr;
                    ਵਾਪਸ ptr;
                }
                struct_type = File_ਗੰਢ::MAP;
            }
            ਨਹੀਂਤਾਂ
                struct_type = File_ਗੰਢ::SEQ;

            fs->convert_ਮੰਜ਼ਿਲ_Collection( struct_type, ਗੰਢ );
            indent = (ਪੂਰਨ)(ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ());

            ਲਈ(;;)
            {
                File_ਗੰਢ elem;

                ਜੇ( struct_type == File_ਗੰਢ::MAP )
                {
                    ptr = parse_ਕੁੰਜੀ( ptr, ਗੰਢ, elem );
                }
                ਨਹੀਂਤਾਂ
                {
                    c = *ptr++;
                    ਜੇ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->ਜੋੜੋ_ਗੰਢ(ਗੰਢ, std::string(), File_ਗੰਢ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                ptr = parse_ਮੁੱਲ( ptr, elem, indent + 1, ਝੂਠ, ਡੂੰਘਾਈ + 1 );
                ptr = skipSpaces( ptr, 0, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                ਜੇ( ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() != indent )
                {
                    ਜੇ( ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ() < indent )
                        ਰੋਕੋ;
                    ਨਹੀਂਤਾਂ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ਜੇ( memcmp( ptr, "...", 3 ) == 0 )
                    ਰੋਕੋ;
            }
            fs->finalizeCollection(ਗੰਢ);
        }

        ਵਾਪਸ ptr;
    }

    ਤਾਰਕਿਕ parse( ਅੱਖਰ* ptr )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਤਾਰਕਿਕ first = ਸੱਚ;
        ਤਾਰਕਿਕ ok = ਸੱਚ;
        File_ਗੰਢ ਜੜ੍ਹ_collection(fs->getFS(), 0, 0);

        ਲਈ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            ਲਈ(;;)
            {
                ptr = skipSpaces( ptr, 0, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                ਜੇ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ਰੋਕੋ;
                }

                ਜੇ( *ptr == '%' )
                {
                    ਜੇ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ਜੇ ((fs->getFormat() & FileStorage::FORMAT_ਨਕਾਬ) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ਨਹੀਂਤਾਂ ਜੇ( *ptr == '-' )
                {
                    ਜੇ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ਰੋਕੋ;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( first )
                        ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ਜੇ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( fs->eof() )
                    ਰੋਕੋ;
                ਨਹੀਂਤਾਂ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ਜੇ( ptr )
                ptr = skipSpaces( ptr, 0, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
            ਜੇ( !ptr || !ptr[0] )
                ਰੋਕੋ;
            ਜੇ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ਗੰਢ ਜੜ੍ਹ_ਗੰਢ = fs->ਜੋੜੋ_ਗੰਢ(ਜੜ੍ਹ_collection, std::string(), File_ਗੰਢ::NONE);

                ptr = parse_ਮੁੱਲ( ptr, ਜੜ੍ਹ_ਗੰਢ, 0, ਝੂਠ );
                ਜੇ( !ਜੜ੍ਹ_ਗੰਢ.isMap() && !ਜੜ੍ਹ_ਗੰਢ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                ਜੇ( !ptr )
                    ਰੋਕੋ;
            }

            ਜੇ( fs->eof() )
                ਰੋਕੋ;
            ptr += 3;
            first = ਝੂਠ;
        }

        ਵਾਪਸ ok;
    }

    FileStorage_API* fs;
    ਅੱਖਰ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<YAMLParser>(fs);
}

}
