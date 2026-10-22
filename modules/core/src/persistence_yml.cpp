// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ಎಣಿಕೆ
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ YAMLEmitter : ಸಾರ್ವಜನಿಕ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ವಾಸ್ತವಿಕ ~YAMLEmitter() {}

    FStruct_ದತ್ತಾಂಶ ಆರಂಭ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ,
                                 ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name=0)
    {
        ಅಕ್ಷರ buf[CV_FS_MAX_LEN + 1024];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ದತ್ತಾಂಶ = 0;

        ಆದರೆ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_ಮುಖವಾಡ|File_ಗಂಟು::FLOW)) | File_ಗಂಟು::ಖಾಲಿಗಳು_2;
        ಆದರೆ( !File_ಗಂಟು::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ಆದರೆ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ಗಂಟು::SEQ;
            snprintf(buf, ಗಾತ್ರ(buf), "!!binary |");
            ದತ್ತಾಂಶ = buf;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( File_ಗಂಟು::isFlow(struct_flags))
        {
            ಅಕ್ಷರ c = File_ಗಂಟು::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ಗಂಟು::FLOW;

            ಆದರೆ( type_name )
                snprintf( buf, ಗಾತ್ರ(buf), "!!%s %c", type_name, c );
            ಇಲ್ಲದಿದ್ದರೆ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            ದತ್ತಾಂಶ = buf;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type_name )
        {
            snprintf( buf, ಗಾತ್ರ(buf), "!!%s", type_name );
            ದತ್ತಾಂಶ = buf;
        }

        writeScalar( ಕೀಲಿ, ದತ್ತಾಂಶ );

        FStruct_ದತ್ತಾಂಶ fsd;
        fsd.indent = ಪೋಷಕ.indent;
        fsd.flags = struct_flags;

        ಆದರೆ( !File_ಗಂಟು::isFlow(ಪೋಷಕ.flags) )
            fsd.indent += CV_YML_INDENT + File_ಗಂಟು::isFlow(struct_flags);

        ಹಿಂತಿರುಗಿ fsd;
    }

    ಶೂನ್ಯ ಕೊನೆ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& current_struct)
    {
        ಅಕ್ಷರ* ptr;

        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;

        ಆದರೆ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ptr = fs->ಬಫರ್_Ptr();
            ಆದರೆ( ptr > fs->ಬಫರ್_ಆರಂಭ() + current_struct.indent && !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
            fs->ಗಣ_ಬಫರ್_Ptr(ptr);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
        {
            ptr = fs->flush();
            ಸ್ಮರಣೆ_ನಕಲು( ptr, File_ಗಂಟು::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->ಗಣ_ಬಫರ್_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value)
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10 ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, int64_t value)
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10, ನಿಜ ));
    }

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ದ್ವಿಗುಣ value )
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::double_ಗಮ್ಯ_String( buf, ಗಾತ್ರ(buf), value, ಸುಳ್ಳು ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str, ತಾರ್ಕಿಕ quote)
    {
        ಅಕ್ಷರ buf[CV_FS_MAX_LEN*4+16];
        ಅಕ್ಷರ* ದತ್ತಾಂಶ = (ಅಕ್ಷರ*)str;
        ಪೂರ್ಣಾಂಕ i, len;

        ಆದರೆ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str);
        ಆದರೆ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಆದರೆ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ಪೂರ್ಣಾಂಕ need_quote = quote || len == 0 || str[0] == ' ';
            ದತ್ತಾಂಶ = buf;
            *ದತ್ತಾಂಶ++ = '\"';
            ಗಾಗಿ( i = 0; i < len; i++ )
            {
                ಅಕ್ಷರ c = str[i];

                ಆದರೆ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ಆದರೆ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *ದತ್ತಾಂಶ++ = '\\';
                    ಆದರೆ( cv_isprint(c) )
                        *ದತ್ತಾಂಶ++ = c;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\n' )
                        *ದತ್ತಾಂಶ++ = 'n';
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\r' )
                        *ದತ್ತಾಂಶ++ = 'r';
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\t' )
                        *ದತ್ತಾಂಶ++ = 't';
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        snprintf( ದತ್ತಾಂಶ, ಗಾತ್ರ(buf) - (ದತ್ತಾಂಶ - buf), "x%02x", c );
                        ದತ್ತಾಂಶ += 3;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    *ದತ್ತಾಂಶ++ = c;
            }
            ಆದರೆ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ಆದರೆ( need_quote )
                *ದತ್ತಾಂಶ++ = '\"';
            *ದತ್ತಾಂಶ++ = '\0';
            ದತ್ತಾಂಶ = buf + !need_quote;
        }

        writeScalar( ಕೀಲಿ, ದತ್ತಾಂಶ);
    }

    ಶೂನ್ಯ writeScalar(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ದತ್ತಾಂಶ)
    {
        fs->check_if_write_struct_is_delayed(ಸುಳ್ಳು);
        ಆದರೆ ( fs->get_state_ದ_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_ಗಮ್ಯ_Base64_state( FileStorage_API::NotUse );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( fs->get_state_ದ_writing_base64() == FileStorage_API::ಒಳ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ಪೂರ್ಣಾಂಕ i, keylen = 0;
        ಪೂರ್ಣಾಂಕ datalen = 0;
        ಅಕ್ಷರ* ptr;

        FStruct_ದತ್ತಾಂಶ& current_struct = fs->getCurrentStruct();

        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;

        ಆದರೆ( ಕೀಲಿ && ಕೀಲಿ[0] == '\0' )
            ಕೀಲಿ = 0;

        ಆದರೆ( File_ಗಂಟು::isCollection(struct_flags) )
        {
            ಆದರೆ( (File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            fs->ಗಣ_Non_ಖಾಲಿಗಳು();
            struct_flags = File_ಗಂಟು::ಖಾಲಿಗಳು_2 | (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
        }

        ಆದರೆ( ಕೀಲಿ )
        {
            keylen = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಕೀಲಿ);
            ಆದರೆ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ಆದರೆ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ಆದರೆ( ದತ್ತಾಂಶ )
            datalen = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ದತ್ತಾಂಶ);

        ಆದರೆ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ptr = fs->ಬಫರ್_Ptr();
            ಆದರೆ( !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
                *ptr++ = ',';
            ಪೂರ್ಣಾಂಕ new_ಸ್ಥಾನಾಂತರ = (ಪೂರ್ಣಾಂಕ)(ptr - fs->ಬಫರ್_ಆರಂಭ()) + keylen + datalen;
            ಆದರೆ( new_ಸ್ಥಾನಾಂತರ > fs->wrapMargin() && new_ಸ್ಥಾನಾಂತರ - current_struct.indent > 10 )
            {
                fs->ಗಣ_ಬಫರ್_Ptr(ptr);
                ptr = fs->flush();
            }
            ಇಲ್ಲದಿದ್ದರೆ
                *ptr++ = ' ';
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ptr = fs->flush();
            ಆದರೆ( !File_ಗಂಟು::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ಆದರೆ( ದತ್ತಾಂಶ )
                    *ptr++ = ' ';
            }
        }

        ಆದರೆ( ಕೀಲಿ )
        {
            ಆದರೆ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_ಬಫರ್( ptr, keylen );

            ಗಾಗಿ( i = 0; i < keylen; i++ )
            {
                ಅಕ್ಷರ c = ಕೀಲಿ[i];

                ptr[i] = c;
                ಆದರೆ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ಆದರೆ( !File_ಗಂಟು::isFlow(struct_flags) && ದತ್ತಾಂಶ )
                *ptr++ = ' ';
        }

        ಆದರೆ( ದತ್ತಾಂಶ )
        {
            ptr = fs->resize_Write_ಬಫರ್( ptr, datalen );
            ಸ್ಮರಣೆ_ನಕಲು( ptr, ದತ್ತಾಂಶ, datalen );
            ptr += datalen;
        }

        fs->ಗಣ_ಬಫರ್_Ptr(ptr);
        current_struct.flags &= ~File_ಗಂಟು::ಖಾಲಿಗಳು_2;
    }

    ಶೂನ್ಯ writeComment(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        ಆದರೆ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* eol = strchr(comment, '\n');
        ತಾರ್ಕಿಕ multiline = eol != 0;
        ಅಕ್ಷರ* ptr = fs->ಬಫರ್_Ptr();

        ಆದರೆ( !eol_comment || multiline ||
            fs->ಬಫರ್_ಕೊನೆ_2() - ptr < len || ptr == fs->ಬಫರ್_ಆರಂಭ() )
            ptr = fs->flush();
        ಇಲ್ಲದಿದ್ದರೆ
            *ptr++ = ' ';

        ಆಗ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ಆದರೆ( eol )
            {
                ptr = fs->resize_Write_ಬಫರ್( ptr, (ಪೂರ್ಣಾಂಕ)(eol - comment) + 1 );
                ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, eol - comment + 1 );
                fs->ಗಣ_ಬಫರ್_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment);
                ptr = fs->resize_Write_ಬಫರ್( ptr, len );
                ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, len );
                fs->ಗಣ_ಬಫರ್_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ಶೂನ್ಯ ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

ಸಂರಕ್ಷಿತ:
    FileStorage_API* fs;
};


ವರ್ಗ YAMLParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ವಾಸ್ತವಿಕ ~YAMLParser() {}

    ಅಕ್ಷರ* skipSpaces( ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ min_indent, ಪೂರ್ಣಾಂಕ max_comment_indent )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಗಾಗಿ(;;)
        {
            ಆಗ( *ptr == ' ' )
                ptr++;
            ಆದರೆ( *ptr == '#' )
            {
                ಆದರೆ( ptr - fs->ಬಫರ್_ಆರಂಭ() > max_comment_indent )
                    ಹಿಂತಿರುಗಿ ptr;
                *ptr = '\0';
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cv_isprint(*ptr) )
            {
                ಆದರೆ( ptr - fs->ಬಫರ್_ಆರಂಭ() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ನಿಲ್ಲಿಸು;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ಆದರೆ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->ಬಫರ್_ಆರಂಭ();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->ಗಣ_Eof();
                    ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಪೂರ್ಣಾಂಕ l = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ptr);
                    ಆದರೆ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ಹಿಂತಿರುಗಿ ptr;
    }

    ತಾರ್ಕಿಕ get_Base64_ಅಡ್ಡಸಾಲು(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ indent, ಅಕ್ಷರ* &beg, ಅಕ್ಷರ* &end)
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
        ಆದರೆ (!ptr || !*ptr)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; // end of file

        ಆದರೆ (ptr - fs->ಬಫರ್_ಆರಂಭ() != indent)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; // end of base64 data

        /* find end */
        ಆಗ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ಆದರೆ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }


    ಅಕ್ಷರ* parse_ಕೀಲಿ( ಅಕ್ಷರ* ptr, File_ಗಂಟು& map_ಗಂಟು, File_ಗಂಟು& ಮೌಲ್ಯ_placeholder )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಅಕ್ಷರ c;
        ಅಕ್ಷರ *endptr = ptr - 1, *saveptr;

        ಆದರೆ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        ಮಾಡು c = *++endptr;
        ಆಗ( cv_isprint(c) && c != ':' );

        ಆದರೆ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ಆದರೆ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        ಮಾಡು c = *--endptr;
        ಆಗ( c == ' ' );

        ++endptr;

        ಮೌಲ್ಯ_placeholder = fs->ಕೂಡಿಸು_ಗಂಟು(map_ಗಂಟು, std::string(ptr, endptr - ptr), File_ಗಂಟು::NONE);
        ptr = saveptr;

        ಹಿಂತಿರುಗಿ ptr;
    }

    ಅಕ್ಷರ* parse_ಮೌಲ್ಯ( ಅಕ್ಷರ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಂಕ min_indent, ತಾರ್ಕಿಕ is_ಪೋಷಕ_flow, ಪೂರ್ಣಾಂಕ ಆಳ = 0 )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಆದರೆ( ಆಳ > CV_PERSISTENCE_MAX_ಆಳ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ಅಕ್ಷರ* endptr = 0;
        ಅಕ್ಷರ c = ptr[0], d = ptr[1];
        ಪೂರ್ಣಾಂಕ value_type = File_ಗಂಟು::NONE;
        ಪೂರ್ಣಾಂಕ len;
        ತಾರ್ಕಿಕ is_ದ್ವಿಮಾನ_string = ಸುಳ್ಳು;
        ತಾರ್ಕಿಕ is_user = ಸುಳ್ಳು;

        ಆದರೆ( c == '!' ) // handle explicit type specification
        {
            ಆದರೆ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = ನಿಜ;
                //value_type |= FileNode::USER;
            }
            ಆದರೆ ( d == '<') //support of full type heading from YAML 1.2
            {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* yamlTypeHeading = "<tag:yaml.org,2002:";
                ಸ್ಥಿರಾಂಕ size_t heading_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(yamlTypeHeading);

                ಅಕ್ಷರ* type_ಕೊನೆ_Ptr = ++ptr;

                ಮಾಡು d = *++type_ಕೊನೆ_Ptr;
                ಆಗ( cv_isprint(d) && d != ' ' && d != '>' );

                ಆದರೆ ( d == '>' && (size_t)(type_ಕೊನೆ_Ptr - ptr) > heading_ಉದ್ದಳತೆ )
                {
                    ಆದರೆ ( memcmp(ptr, yamlTypeHeading, heading_ಉದ್ದಳತೆ) == 0 )
                    {
                        *type_ಕೊನೆ_Ptr = ' ';
                        ptr += heading_ಉದ್ದಳತೆ - 1;
                        is_user = ನಿಜ;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            ಮಾಡು d = *++endptr;
            ಆಗ( cv_isprint(d) && d != ' ' );
            len = (ಪೂರ್ಣಾಂಕ)(endptr - ptr);
            ಆದರೆ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ಆದರೆ( len == 3 && !is_user )
            {
                ಆದರೆ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ಗಂಟು::STRING;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ಗಂಟು::INT;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ಗಂಟು::SEQ;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ಗಂಟು::MAP;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len == 5 && !is_user )
            {
                ಆದರೆ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ಗಂಟು::REAL;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 6 && is_user)
            {
                ಆದರೆ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ಗಂಟು::SEQ;
                    is_ದ್ವಿಮಾನ_string = ನಿಜ;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    ಮಾಡು {
                        d = *++endptr;
                        ಆದರೆ (d == '|')
                            ನಿಲ್ಲಿಸು;
                    } ಆಗ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
            ಆದರೆ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ಆದರೆ( !is_user )
            {
                ಆದರೆ (value_type == File_ಗಂಟು::STRING && c != '\'' && c != '\"')
                    ಹೋಗು force_string;
                ಆದರೆ( value_type == File_ಗಂಟು::INT )
                    ಹೋಗು force_int;
                ಆದರೆ( value_type == File_ಗಂಟು::REAL )
                    ಹೋಗು force_real;
            }
        }

        ಆದರೆ (is_ದ್ವಿಮಾನ_string)
        {
            ಪೂರ್ಣಾಂಕ indent = static_cast<ಪೂರ್ಣಾಂಕ>(ptr - fs->ಬಫರ್_ಆರಂಭ());
            ptr = fs->parseBase64(ptr, indent, ಗಂಟು);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ಆಗ( cv_isdigit(*endptr) )
                endptr++;
            ಆದರೆ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ದ್ವಿಗುಣ fval = fs->strtod( ptr, &endptr );
                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }

            ಆದರೆ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\'' || c == '\"' ) // an explicit string
        {
            ಆದರೆ( c == '\'' )
                ಗಾಗಿ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ಆದರೆ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\'' )
                    {
                        c = *++ptr;
                        ಆದರೆ( c != '\'' )
                            ನಿಲ್ಲಿಸು;
                        buf[len++] = c;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ಆದರೆ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\"' )
                    {
                        ++ptr;
                        ನಿಲ್ಲಿಸು;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\\' )
                    {
                        d = *++ptr;
                        ಆದರೆ( d == '\'' )
                            buf[len++] = d;
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 'n' )
                            buf[len++] = '\n';
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 'r' )
                            buf[len++] = '\r';
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 't' )
                            buf[len++] = '\t';
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ಪೂರ್ಣಾಂಕ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ಪೂರ್ಣಾಂಕ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ಆದರೆ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                buf[len++] = (ಅಕ್ಷರ)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ಆದರೆ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, buf, len);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '[' || c == '{' ) // collection as a flow
        {
            ಪೂರ್ಣಾಂಕ new_min_indent = min_indent + !is_ಪೋಷಕ_flow;
            ಪೂರ್ಣಾಂಕ struct_type = c == '{' ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ;
            ಪೂರ್ಣಾಂಕ nelems = 0;

            fs->convert_ಗಮ್ಯ_Collection(struct_type, ಗಂಟು);
            d = c == '[' ? ']' : '}';

            ಗಾಗಿ( ++ptr ;; nelems++ )
            {
                File_ಗಂಟು elem;

                ptr = skipSpaces( ptr, new_min_indent, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                ಆದರೆ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ಆದರೆ( *ptr == '}' || *ptr == ']' )
                {
                    ಆದರೆ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ( nelems != 0 )
                {
                    ಆದರೆ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                    ಆದರೆ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ಆದರೆ( struct_type == File_ಗಂಟು::MAP )
                {
                    ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, elem );
                    ptr = skipSpaces( ptr, new_min_indent, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಆದರೆ( *ptr == ']' )
                        ನಿಲ್ಲಿಸು;
                    elem = fs->ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE);
                }
                ptr = parse_ಮೌಲ್ಯ( ptr, elem, new_min_indent, ನಿಜ, ಆಳ + 1 );
            }
            fs->finalizeCollection(ಗಂಟು);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ indent, struct_type;

            ಆದರೆ( is_ಪೋಷಕ_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ಆದರೆ( !is_ಪೋಷಕ_flow )
                {
                    ಆದರೆ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ಆದರೆ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                ಮಾಡು c = *++endptr;
                ಆಗ( cv_isprint(c) &&
                      (!is_ಪೋಷಕ_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_ಪೋಷಕ_flow || c != ':' || value_type == File_ಗಂಟು::STRING));

                ಆದರೆ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ಆದರೆ( is_ಪೋಷಕ_flow || c != ':' )
                {
                    ಅಕ್ಷರ* str_ಕೊನೆ = endptr;
                    ಮಾಡು c = *--str_ಕೊನೆ;
                    ಆಗ( str_ಕೊನೆ > ptr && c == ' ' );
                    str_ಕೊನೆ++;
                    ಆದರೆ ((fs->getFormat() & FileStorage::FORMAT_ಮುಖವಾಡ) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_ಕೊನೆ - ptr;
                        ಆದರೆ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                            ptr = endptr;
                            ಹಿಂತಿರುಗಿ ptr;
                        }
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                            ptr = endptr;
                            ಹಿಂತಿರುಗಿ ptr;
                        }
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ಹಿಂತಿರುಗಿ ptr;
                        }
                    }

                    ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, ptr, (ಪೂರ್ಣಾಂಕ)(str_ಕೊನೆ - ptr));
                    ptr = endptr;
                    ಹಿಂತಿರುಗಿ ptr;
                }
                struct_type = File_ಗಂಟು::MAP;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                struct_type = File_ಗಂಟು::SEQ;

            fs->convert_ಗಮ್ಯ_Collection( struct_type, ಗಂಟು );
            indent = (ಪೂರ್ಣಾಂಕ)(ptr - fs->ಬಫರ್_ಆರಂಭ());

            ಗಾಗಿ(;;)
            {
                File_ಗಂಟು elem;

                ಆದರೆ( struct_type == File_ಗಂಟು::MAP )
                {
                    ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, elem );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    c = *ptr++;
                    ಆದರೆ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                ptr = parse_ಮೌಲ್ಯ( ptr, elem, indent + 1, ಸುಳ್ಳು, ಆಳ + 1 );
                ptr = skipSpaces( ptr, 0, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                ಆದರೆ( ptr - fs->ಬಫರ್_ಆರಂಭ() != indent )
                {
                    ಆದರೆ( ptr - fs->ಬಫರ್_ಆರಂಭ() < indent )
                        ನಿಲ್ಲಿಸು;
                    ಇಲ್ಲದಿದ್ದರೆ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ಆದರೆ( memcmp( ptr, "...", 3 ) == 0 )
                    ನಿಲ್ಲಿಸು;
            }
            fs->finalizeCollection(ಗಂಟು);
        }

        ಹಿಂತಿರುಗಿ ptr;
    }

    ತಾರ್ಕಿಕ parse( ಅಕ್ಷರ* ptr )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ತಾರ್ಕಿಕ first = ನಿಜ;
        ತಾರ್ಕಿಕ ok = ನಿಜ;
        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಗಾಗಿ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            ಗಾಗಿ(;;)
            {
                ptr = skipSpaces( ptr, 0, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                ಆದರೆ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ( *ptr == '%' )
                {
                    ಆದರೆ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ಆದರೆ ((fs->getFormat() & FileStorage::FORMAT_ಮುಖವಾಡ) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( *ptr == '-' )
                {
                    ಆದರೆ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ನಿಲ್ಲಿಸು;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( first )
                        ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ಆದರೆ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( fs->eof() )
                    ನಿಲ್ಲಿಸು;
                ಇಲ್ಲದಿದ್ದರೆ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ಆದರೆ( ptr )
                ptr = skipSpaces( ptr, 0, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
            ಆದರೆ( !ptr || !ptr[0] )
                ನಿಲ್ಲಿಸು;
            ಆದರೆ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->ಕೂಡಿಸು_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::NONE);

                ptr = parse_ಮೌಲ್ಯ( ptr, ಬೇರು_ಗಂಟು, 0, ಸುಳ್ಳು );
                ಆದರೆ( !ಬೇರು_ಗಂಟು.isMap() && !ಬೇರು_ಗಂಟು.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                ಆದರೆ( !ptr )
                    ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ( fs->eof() )
                ನಿಲ್ಲಿಸು;
            ptr += 3;
            first = ಸುಳ್ಳು;
        }

        ಹಿಂತಿರುಗಿ ok;
    }

    FileStorage_API* fs;
    ಅಕ್ಷರ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<YAMLParser>(fs);
}

}
