// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"

ਗਿਣਤੀ
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_ਟੈਗ = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_ਟੈਗ = 1,
    CV_XML_CLOSING_ਟੈਗ = 2,
    CV_XML_ਖਾਲੀ_ਥਾਵਾਂ_ਟੈਗ = 3,
    CV_XML_ਸਿਰਲੇਖ_ਟੈਗ = 4,
    CV_XML_DIRECTIVE_ਟੈਗ = 5
};

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਗ XMLEmitter : ਜਨਤਕ FileStorageEmitter
{
ਜਨਤਕ:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ਆਭਾਸੀ ~XMLEmitter() {}

    ਖਾਲੀ write_ਟੈਗ( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ ਟੈਗ_type, ਅਚਲ std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        ਅੱਖਰ* ptr = fs->ਬਫਰ_Ptr();
        ਪੂਰਨ i, len = 0;
        FStruct_ਅੰਕੜੇ& current_struct = fs->getCurrentStruct();
        ਪੂਰਨ struct_flags = current_struct.flags;

        ਜੇ( ਕੁੰਜੀ && ਕੁੰਜੀ[0] == '\0' )
            ਕੁੰਜੀ = 0;

        ਜੇ( ਟੈਗ_type == CV_XML_OPENING_ਟੈਗ || ਟੈਗ_type == CV_XML_ਖਾਲੀ_ਥਾਵਾਂ_ਟੈਗ )
        {
            ਜੇ( File_ਗੰਢ::isCollection(struct_flags) )
            {
                ਜੇ( File_ਗੰਢ::isMap(struct_flags) ^ (ਕੁੰਜੀ != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ਨਹੀਂਤਾਂ
            {
                struct_flags = File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2 + (ਕੁੰਜੀ ? File_ਗੰਢ::MAP : File_ਗੰਢ::SEQ);
                //fs->is_first = 0;
            }

            ਜੇ( !File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(struct_flags) )
                ptr = fs->flush();
        }

        ਜੇ( !ਕੁੰਜੀ )
            ਕੁੰਜੀ = "_";
        ਨਹੀਂਤਾਂ ਜੇ( ਕੁੰਜੀ[0] == '_' && ਕੁੰਜੀ[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ( ਕੁੰਜੀ );
        *ptr++ = '<';
        ਜੇ( ਟੈਗ_type == CV_XML_CLOSING_ਟੈਗ )
        {
            ਜੇ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ਜੇ( !cv_isalpha(ਕੁੰਜੀ[0]) && ਕੁੰਜੀ[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_ਬਫਰ( ptr, len );
        ਲਈ( i = 0; i < len; i++ )
        {
            ਅੱਖਰ c = ਕੁੰਜੀ[i];
            ਜੇ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ਪੂਰਨ nattr = (ਪੂਰਨ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        ਲਈ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_ਬਫਰ( ptr, (ਪੂਰਨ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ਜੇ( len1 > 0 )
                ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ਜੇ( ਟੈਗ_type == CV_XML_ਖਾਲੀ_ਥਾਵਾਂ_ਟੈਗ )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
    }

    FStruct_ਅੰਕੜੇ ਸ਼ੁਰੂ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& ਮਾਪਾ, ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ,
                                 ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ* type_name=0)
    {
        std::vector<std::string> attrlist;
        ਜੇ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_ਟੈਗ( ਕੁੰਜੀ, CV_XML_OPENING_ਟੈਗ, attrlist );

        FStruct_ਅੰਕੜੇ current_struct;
        current_struct.ਟੈਗ = ਕੁੰਜੀ ? std::string(ਕੁੰਜੀ) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = ਮਾਪਾ.indent + CV_XML_INDENT;

        ਵਾਪਸ current_struct;
    }

    ਖਾਲੀ ਸਿਰਾ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& current_struct)
    {
        write_ਟੈਗ( current_struct.ਟੈਗ.c_str(), CV_XML_CLOSING_ਟੈਗ );
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ value)
    {
        ਅੱਖਰ buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( ਕੁੰਜੀ, ptr);
    }

    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, int64_t value)
    {
        ਅੱਖਰ buf[128], *ptr = fs::itoa( value, buf, 10, ਸੱਚ );
        writeScalar( ਕੁੰਜੀ, ptr);
    }

    ਖਾਲੀ ਲਿਖ( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਦੁੱਗਣਾ value )
    {
        ਅੱਖਰ buf[128];
        writeScalar( ਕੁੰਜੀ, fs::double_ਮੰਜ਼ਿਲ_String( buf, ਆਕਾਰ(buf), value, ਝੂਠ ) );
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

        ਜੇ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            ਤਾਰਕਿਕ need_quote = quote || len == 0;
            ਅੰਕੜੇ = buf;
            *ਅੰਕੜੇ++ = '\"';
            ਲਈ( i = 0; i < len; i++ )
            {
                ਅੱਖਰ c = str[i];

                ਜੇ( (uchar)c >= 128 || c == ' ' )
                {
                    *ਅੰਕੜੇ++ = c;
                    need_quote = ਸੱਚ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *ਅੰਕੜੇ++ = '&';
                    ਜੇ( c == '<' )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ, "lt", 2);
                        ਅੰਕੜੇ += 2;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( c == '>' )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ, "gt", 2);
                        ਅੰਕੜੇ += 2;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( c == '&' )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ, "amp", 3);
                        ਅੰਕੜੇ += 3;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\'' )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ, "apos", 4);
                        ਅੰਕੜੇ += 4;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( c == '\"' )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ( ਅੰਕੜੇ, "quot", 4);
                        ਅੰਕੜੇ += 4;
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        snprintf( ਅੰਕੜੇ, ਆਕਾਰ(buf) - (ਅੰਕੜੇ - buf), "#x%02x", (uchar)c );
                        ਅੰਕੜੇ += 4;
                    }
                    *ਅੰਕੜੇ++ = ';';
                    need_quote = 1;
                }
                ਨਹੀਂਤਾਂ
                    *ਅੰਕੜੇ++ = c;
            }
            ਜੇ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ਸੱਚ;

            ਜੇ( need_quote )
                *ਅੰਕੜੇ++ = '\"';
            len = (ਪੂਰਨ)(ਅੰਕੜੇ - buf) - !need_quote;
            *ਅੰਕੜੇ++ = '\0';
            ਅੰਕੜੇ = buf + (ਪੂਰਨ)!need_quote;
        }

        writeScalar( ਕੁੰਜੀ, ਅੰਕੜੇ );
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

        ਪੂਰਨ len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(ਅੰਕੜੇ);
        ਜੇ( ਕੁੰਜੀ && *ਕੁੰਜੀ == '\0' )
            ਕੁੰਜੀ = 0;

        FStruct_ਅੰਕੜੇ& current_struct = fs->getCurrentStruct();
        ਪੂਰਨ struct_flags = current_struct.flags;

        ਜੇ( File_ਗੰਢ::isMap(struct_flags) ||
           (!File_ਗੰਢ::isCollection(struct_flags) && ਕੁੰਜੀ) )
        {
            write_ਟੈਗ( ਕੁੰਜੀ, CV_XML_OPENING_ਟੈਗ );
            ਅੱਖਰ* ptr = fs->resize_Write_ਬਫਰ( fs->ਬਫਰ_Ptr(), len );
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, ਅੰਕੜੇ, len );
            fs->ਸੈੱਟ_ਬਫਰ_Ptr( ptr + len );
            write_ਟੈਗ( ਕੁੰਜੀ, CV_XML_CLOSING_ਟੈਗ );
        }
        ਨਹੀਂਤਾਂ
        {
            ਅੱਖਰ* ptr = fs->ਬਫਰ_Ptr();
            ਪੂਰਨ new_ਵਿੱਥ = (ਪੂਰਨ)(ptr - fs->ਬਫਰ_ਸ਼ੁਰੂ()) + len;

            ਜੇ( ਕੁੰਜੀ )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ਗੰਢ::SEQ;

            ਜੇ( (new_ਵਿੱਥ > fs->wrapMargin() && new_ਵਿੱਥ - current_struct.indent > 10) ||
               (ptr > fs->ਬਫਰ_ਸ਼ੁਰੂ() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ਨਹੀਂਤਾਂ ਜੇ( ptr > fs->ਬਫਰ_ਸ਼ੁਰੂ() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, ਅੰਕੜੇ, len );
            fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + len);
        }
    }

    ਖਾਲੀ writeComment(ਅਚਲ ਅੱਖਰ* comment, ਤਾਰਕਿਕ eol_comment)
    {
        FStruct_ਅੰਕੜੇ& current_struct = fs->getCurrentStruct();
        ਪੂਰਨ len;
        ਪੂਰਨ multiline;
        ਅਚਲ ਅੱਖਰ* eol;
        ਅੱਖਰ* ptr;

        ਜੇ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ਜੇ( ਲੜੀ_ਖੋਜ(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->ਬਫਰ_Ptr();

        ਜੇ( multiline || !eol_comment || fs->ਬਫਰ_ਸਿਰਾ_2() - ptr < len + 5 )
            ptr = fs->flush();
        ਨਹੀਂਤਾਂ ਜੇ( ptr > fs->ਬਫਰ_ਸ਼ੁਰੂ() + current_struct.indent )
            *ptr++ = ' ';

        ਜੇ( !multiline )
        {
            ptr = fs->resize_Write_ਬਫਰ( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(ptr);
        }
        ਨਹੀਂਤਾਂ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + len);
        ptr = fs->flush();

        ਜੇ( multiline )
        {
            ਜਦੋਂ( comment )
            {
                ਜੇ( eol )
                {
                    ptr = fs->resize_Write_ਬਫਰ( ptr, (ਪੂਰਨ)(eol - comment) + 1 );
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ਨਹੀਂਤਾਂ
                {
                    len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(comment);
                    ptr = fs->resize_Write_ਬਫਰ( ptr, len );
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->ਸੈੱਟ_ਬਫਰ_Ptr(ptr + 3);
            fs->flush();
        }
    }

    ਖਾਲੀ ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

ਸੁਰੱਖਿਅਤ:
    FileStorage_API* fs;
};

ਵਰਗ XMLParser : ਜਨਤਕ FileStorageParser
{
ਜਨਤਕ:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ਆਭਾਸੀ ~XMLParser() {}

    ਅੱਖਰ* skipSpaces( ਅੱਖਰ* ptr, ਪੂਰਨ ਬਹੁਲਕ )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਪੂਰਨ level = 0;

        ਲਈ(;;)
        {
            ਅੱਖਰ c;
            ptr--;

            ਜੇ( ਬਹੁਲਕ == CV_XML_INSIDE_COMMENT )
            {
                ਕਰੋ c = *++ptr;
                ਜਦੋਂ( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ਜੇ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    ਬਹੁਲਕ = 0;
                    ptr += 3;
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ( ਬਹੁਲਕ == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                ਕਰੋ
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ਜੇ( level < 0 )
                        ਵਾਪਸ ptr;
                } ਜਦੋਂ( cv_isprint_or_tab(c) );
            }
            ਨਹੀਂਤਾਂ
            {
                ਕਰੋ c = *++ptr;
                ਜਦੋਂ( c == ' ' || c == '\t' );

                ਜੇ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ਜੇ( ਬਹੁਲਕ != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    ਬਹੁਲਕ = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ਨਹੀਂਤਾਂ ਜੇ( cv_isprint(c) )
                    ਰੋਕੋ;
            }

            ਜੇ( !cv_isprint(*ptr) )
            {
                ਜੇ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ਜੇ( !ptr || *ptr == '\0' )
                    ਰੋਕੋ;
            }
        }
        ਵਾਪਸ ptr;
    }

    ਤਾਰਕਿਕ get_Base64_ਪੰਕਤੀ(ਅੱਖਰ* ptr, ਪੂਰਨ /*indent*/, ਅੱਖਰ* &beg, ਅੱਖਰ* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_ਟੈਗ);
        ਜੇ( !ptr || !*ptr )
            ਵਾਪਸ ਝੂਠ;

        // closing XML tag
        ਜੇ ( *beg == '<' )
            ਵਾਪਸ ਝੂਠ;

        // find end of the row
        ਜਦੋਂ( cv_isprint(*ptr) )
            ++ptr;
        ਜੇ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ਵਾਪਸ ਸੱਚ;
    }

    ਅੱਖਰ* parse_ਮੁੱਲ( ਅੱਖਰ* ptr, File_ਗੰਢ& ਗੰਢ, ਪੂਰਨ ਡੂੰਘਾਈ = 0 )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਜੇ( ਡੂੰਘਾਈ > CV_PERSISTENCE_MAX_ਡੂੰਘਾਈ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ਗੰਢ new_elem;
        ਤਾਰਕਿਕ have_space = ਸੱਚ;
        ਪੂਰਨ value_type = ਗੰਢ.type();
        std::string ਕੁੰਜੀ, key2, type_name;

        ਲਈ(;;)
        {
            ਅੱਖਰ c = *ptr, d;
            ਅੱਖਰ* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ਜੇ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ਜੇ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = ਸੱਚ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ਜੇ( c =='<' || c == '\0' )
            {
                ਪੂਰਨ ਟੈਗ_type = 0;
                ਪੂਰਨ elem_type = File_ਗੰਢ::NONE;

                ਜੇ( d == '/' || c == '\0' )
                    ਰੋਕੋ;

                ptr = parse_ਟੈਗ( ptr, ਕੁੰਜੀ, type_name, ਟੈਗ_type );

                ਜੇ( ਟੈਗ_type == CV_XML_DIRECTIVE_ਟੈਗ )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ਜੇ( ਟੈਗ_type == CV_XML_ਖਾਲੀ_ਥਾਵਾਂ_ਟੈਗ )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(ਟੈਗ_type == CV_XML_OPENING_ਟੈਗ);

                /* for base64 string */
                ਤਾਰਕਿਕ ਦੁਆਧਾਰੀ_string = ਝੂਠ;

                ਜੇ( !type_name.empty() )
                {
                    ਅਚਲ ਅੱਖਰ* tn = type_name.c_str();
                    ਜੇ( strcmp(tn, "str") == 0 )
                        elem_type = File_ਗੰਢ::STRING;
                    ਨਹੀਂਤਾਂ ਜੇ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ਗੰਢ::MAP;
                    ਨਹੀਂਤਾਂ ਜੇ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ਗੰਢ::SEQ;
                    ਨਹੀਂਤਾਂ ਜੇ( strcmp( tn, "binary") == 0)
                        ਦੁਆਧਾਰੀ_string = ਸੱਚ;
                }

                new_elem = fs->ਜੋੜੋ_ਗੰਢ(ਗੰਢ, ਕੁੰਜੀ, elem_type, 0);
                ਜੇ (!ਦੁਆਧਾਰੀ_string)
                    ptr = parse_ਮੁੱਲ(ptr, new_elem, ਡੂੰਘਾਈ + 1);
                ਨਹੀਂਤਾਂ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ਜੇ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_ਟੈਗ( ptr, key2, type_name, ਟੈਗ_type );
                ਜੇ( ਟੈਗ_type != CV_XML_CLOSING_ਟੈਗ || key2 != ਕੁੰਜੀ )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ਸੱਚ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ਗੰਢ* elem = &ਗੰਢ;
                ਜੇ( ਗੰਢ.type() != File_ਗੰਢ::NONE )
                {
                    fs->convert_ਮੰਜ਼ਿਲ_Collection( File_ਗੰਢ::SEQ, ਗੰਢ );
                    new_elem = fs->ਜੋੜੋ_ਗੰਢ(ਗੰਢ, std::string(), File_ਗੰਢ::NONE, 0);
                    elem = &new_elem;
                }

                ਜੇ( value_type != File_ਗੰਢ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ਜਦੋਂ( cv_isdigit(*endptr) )
                        endptr++;
                    ਜੇ( *endptr == '.' || *endptr == 'e' )
                    {
                        ਦੁੱਗਣਾ fval = fs->strtod( ptr, &endptr );
                        elem->ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::REAL, &fval);
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::INT, &ival);
                    }

                    ਜੇ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
                }
                ਨਹੀਂਤਾਂ
                {
                    // string
                    ਪੂਰਨ i = 0, len, is_quoted = 0;
                    ਜੇ( c == '\"' )
                        is_quoted = 1;
                    ਨਹੀਂਤਾਂ
                        --ptr;
                    strbuf[0] = '\0';

                    ਲਈ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();

                        ਜੇ( !cv_isalnum(c) )
                        {
                            ਜੇ( c == '\"' )
                            {
                                ਜੇ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                ਰੋਕੋ;
                            }
                            ਨਹੀਂਤਾਂ ਜੇ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ਜੇ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                ਰੋਕੋ;
                            }
                            ਨਹੀਂਤਾਂ ਜੇ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ਨਹੀਂਤਾਂ ਜੇ( c == '&' )
                            {
                                ਜੇ( *++ptr == '#' )
                                {
                                    ਪੂਰਨ val, ਅਧਾਰ = 10;
                                    ptr++;
                                    ਜੇ( *ptr == 'x' )
                                    {
                                        ਅਧਾਰ = 16;
                                        ptr++;
                                    }
                                    val = (ਪੂਰਨ)strtol( ptr, &endptr, ਅਧਾਰ );
                                    ਜੇ( (ਬਿਨਾਨਿਸ਼ਾਨੀ)val > (ਬਿਨਾਨਿਸ਼ਾਨੀ)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (ਅੱਖਰ)val;
                                }
                                ਨਹੀਂਤਾਂ
                                {
                                    endptr = ptr;
                                    ਕਰੋ c = *++endptr;
                                    ਜਦੋਂ( cv_isalnum(c) );
                                    ਜੇ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ਪੂਰਨ)(endptr - ptr);
                                    ਜੇ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ਨਹੀਂਤਾਂ ਜੇ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ਨਹੀਂਤਾਂ ਜੇ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ਨਹੀਂਤਾਂ ਜੇ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ਨਹੀਂਤਾਂ ਜੇ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ਨਹੀਂਤਾਂ
                                    {
                                        ਜੇ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        ਸਮ੍ਰਿਤੀ_ਨਕਲ( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();
                            }
                        }
                        ਜੇ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->ਸੈੱਟ_ਮੁੱਲ(File_ਗੰਢ::STRING, strbuf, i);
                }

                ਜੇ( value_type != File_ਗੰਢ::NONE && value_type != File_ਗੰਢ::SEQ && value_type != File_ਗੰਢ::MAP )
                    ਰੋਕੋ;
                have_space = ਝੂਠ;
            }
        }
        fs->finalizeCollection(ਗੰਢ);

        ਵਾਪਸ ptr;
    }

    ਅੱਖਰ* parse_ਟੈਗ( ਅੱਖਰ* ptr, std::string& ਟੈਗ_name,
                    std::string& type_name, ਪੂਰਨ& ਟੈਗ_type )
    {
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ਜੇ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ਜੇ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();

        ਜੇ( cv_isalnum(*ptr) || *ptr == '_' )
            ਟੈਗ_type = CV_XML_OPENING_ਟੈਗ;
        ਨਹੀਂਤਾਂ ਜੇ( *ptr == '/' )
        {
            ਟੈਗ_type = CV_XML_CLOSING_ਟੈਗ;
            ptr++;
        }
        ਨਹੀਂਤਾਂ ਜੇ( *ptr == '?' )
        {
            ਟੈਗ_type = CV_XML_ਸਿਰਲੇਖ_ਟੈਗ;
            ptr++;
        }
        ਨਹੀਂਤਾਂ ਜੇ( *ptr == '!' )
        {
            ਟੈਗ_type = CV_XML_DIRECTIVE_ਟੈਗ;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ਨਹੀਂਤਾਂ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        ਟੈਗ_name.clear();
        type_name.clear();

        ਲਈ(;;)
        {
            ਅੱਖਰ c, *endptr;
            ਜੇ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            ਕਰੋ c = *++endptr;
            ਜਦੋਂ( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP();

            ਜੇ( ਟੈਗ_name.empty() )
                ਟੈਗ_name = attrname;
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( ਟੈਗ_type == CV_XML_CLOSING_ਟੈਗ )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ਜੇ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ਟੈਗ );
                    ਜੇ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ਜੇ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ਜੇ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ਟੈਗ );
                    ਜੇ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ਜੇ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                ਅੱਖਰ quote = *ptr++;
                endptr = ptr;
                ਲਈ(;;)
                {
                    c = *endptr++;
                    ਜੇ( c == quote )
                        ਰੋਕੋ;
                    ਜੇ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ਜੇ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            ਤਾਰਕਿਕ have_space = cv_isspace(c) || c == '\0';

            ਜੇ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_ਟੈਗ );
                ਜੇ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ਜੇ( c == '>' )
            {
                ਜੇ( ਟੈਗ_type == CV_XML_ਸਿਰਲੇਖ_ਟੈਗ )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                ਰੋਕੋ;
            }
            ਨਹੀਂਤਾਂ ਜੇ( c == '?' && ਟੈਗ_type == CV_XML_ਸਿਰਲੇਖ_ਟੈਗ )
            {
                ਜੇ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                ਰੋਕੋ;
            }
            ਨਹੀਂਤਾਂ ਜੇ( c == '/' && ptr[1] == '>' && ਟੈਗ_type == CV_XML_OPENING_ਟੈਗ )  // FIXIT ptr[1] - out of bounds read without check
            {
                ਟੈਗ_type = CV_XML_ਖਾਲੀ_ਥਾਵਾਂ_ਟੈਗ;
                ptr += 2;
                ਰੋਕੋ;
            }

            ਜੇ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ਵਾਪਸ ptr;
    }

    ਤਾਰਕਿਕ parse(ਅੱਖਰ* ptr)
    {
        CV_Assert( fs != 0 );

        std::string ਕੁੰਜੀ, key2, type_name;
        ਪੂਰਨ ਟੈਗ_type = 0;
        ਤਾਰਕਿਕ ok = ਝੂਠ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_ਟੈਗ );
        ਜੇ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ਜੇ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_ਟੈਗ( ptr, ਕੁੰਜੀ, type_name, ਟੈਗ_type );
        File_ਗੰਢ ਜੜ੍ਹ_collection(fs->getFS(), 0, 0);

        ਜਦੋਂ( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ਜੇ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ਜੇ( *ptr != '\0' )
            {
                ptr = parse_ਟੈਗ( ptr, ਕੁੰਜੀ, type_name, ਟੈਗ_type );
                ਜੇ( ਟੈਗ_type != CV_XML_OPENING_ਟੈਗ || ਕੁੰਜੀ != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ਗੰਢ ਜੜ੍ਹ = fs->ਜੋੜੋ_ਗੰਢ(ਜੜ੍ਹ_collection, std::string(), File_ਗੰਢ::MAP, 0);
                ptr = parse_ਮੁੱਲ( ptr, ਜੜ੍ਹ );
                ptr = parse_ਟੈਗ( ptr, key2, type_name, ਟੈਗ_type );
                ਜੇ( ਟੈਗ_type != CV_XML_CLOSING_ਟੈਗ || ਕੁੰਜੀ != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ਸੱਚ;
            }
        }
        CV_Assert( fs->eof() );
        ਵਾਪਸ ok;
    }

    FileStorage_API* fs;
    ਅੱਖਰ strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ਵਾਪਸ makePtr<XMLParser>(fs);
}

}
