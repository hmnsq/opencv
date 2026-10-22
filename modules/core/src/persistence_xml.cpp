// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ಎಣಿಕೆ
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_ಟ್ಯಾಗ್ = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_ಟ್ಯಾಗ್ = 1,
    CV_XML_CLOSING_ಟ್ಯಾಗ್ = 2,
    CV_XML_ಖಾಲಿಗಳು_ಟ್ಯಾಗ್ = 3,
    CV_XML_ಶೀರ್ಷಿಕೆ_ಟ್ಯಾಗ್ = 4,
    CV_XML_DIRECTIVE_ಟ್ಯಾಗ್ = 5
};

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ XMLEmitter : ಸಾರ್ವಜನಿಕ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ವಾಸ್ತವಿಕ ~XMLEmitter() {}

    ಶೂನ್ಯ write_ಟ್ಯಾಗ್( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್_type, ಸ್ಥಿರಾಂಕ std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        ಅಕ್ಷರ* ptr = fs->ಬಫರ್_Ptr();
        ಪೂರ್ಣಾಂಕ i, len = 0;
        FStruct_ದತ್ತಾಂಶ& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;

        ಆದರೆ( ಕೀಲಿ && ಕೀಲಿ[0] == '\0' )
            ಕೀಲಿ = 0;

        ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_OPENING_ಟ್ಯಾಗ್ || ಟ್ಯಾಗ್_type == CV_XML_ಖಾಲಿಗಳು_ಟ್ಯಾಗ್ )
        {
            ಆದರೆ( File_ಗಂಟು::isCollection(struct_flags) )
            {
                ಆದರೆ( File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                struct_flags = File_ಗಂಟು::ಖಾಲಿಗಳು_2 + (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
                //fs->is_first = 0;
            }

            ಆದರೆ( !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
                ptr = fs->flush();
        }

        ಆದರೆ( !ಕೀಲಿ )
            ಕೀಲಿ = "_";
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಕೀಲಿ[0] == '_' && ಕೀಲಿ[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ( ಕೀಲಿ );
        *ptr++ = '<';
        ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_CLOSING_ಟ್ಯಾಗ್ )
        {
            ಆದರೆ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ಆದರೆ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_ಬಫರ್( ptr, len );
        ಗಾಗಿ( i = 0; i < len; i++ )
        {
            ಅಕ್ಷರ c = ಕೀಲಿ[i];
            ಆದರೆ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ಪೂರ್ಣಾಂಕ nattr = (ಪೂರ್ಣಾಂಕ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        ಗಾಗಿ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_ಬಫರ್( ptr, (ಪೂರ್ಣಾಂಕ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            ಸ್ಮರಣೆ_ನಕಲು( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ಆದರೆ( len1 > 0 )
                ಸ್ಮರಣೆ_ನಕಲು( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_ಖಾಲಿಗಳು_ಟ್ಯಾಗ್ )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->ಗಣ_ಬಫರ್_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_ಗಂಟು::ಖಾಲಿಗಳು_2;
    }

    FStruct_ದತ್ತಾಂಶ ಆರಂಭ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ,
                                 ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name=0)
    {
        std::vector<std::string> attrlist;
        ಆದರೆ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_ಟ್ಯಾಗ್( ಕೀಲಿ, CV_XML_OPENING_ಟ್ಯಾಗ್, attrlist );

        FStruct_ದತ್ತಾಂಶ current_struct;
        current_struct.ಟ್ಯಾಗ್ = ಕೀಲಿ ? std::string(ಕೀಲಿ) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = ಪೋಷಕ.indent + CV_XML_INDENT;

        ಹಿಂತಿರುಗಿ current_struct;
    }

    ಶೂನ್ಯ ಕೊನೆ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& current_struct)
    {
        write_ಟ್ಯಾಗ್( current_struct.ಟ್ಯಾಗ್.c_str(), CV_XML_CLOSING_ಟ್ಯಾಗ್ );
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value)
    {
        ಅಕ್ಷರ buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( ಕೀಲಿ, ptr);
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, int64_t value)
    {
        ಅಕ್ಷರ buf[128], *ptr = fs::itoa( value, buf, 10, ನಿಜ );
        writeScalar( ಕೀಲಿ, ptr);
    }

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ದ್ವಿಗುಣ value )
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::double_ಗಮ್ಯ_String( buf, ಗಾತ್ರ(buf), value, ಸುಳ್ಳು ) );
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str, ತಾರ್ಕಿಕ quote)
    {
        ಅಕ್ಷರ buf[CV_FS_MAX_LEN*6+16];
        ಅಕ್ಷರ* ದತ್ತಾಂಶ = (ಅಕ್ಷರ*)str;
        ಪೂರ್ಣಾಂಕ i, len;

        ಆದರೆ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str);
        ಆದರೆ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಆದರೆ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            ತಾರ್ಕಿಕ need_quote = quote || len == 0;
            ದತ್ತಾಂಶ = buf;
            *ದತ್ತಾಂಶ++ = '\"';
            ಗಾಗಿ( i = 0; i < len; i++ )
            {
                ಅಕ್ಷರ c = str[i];

                ಆದರೆ( (uchar)c >= 128 || c == ' ' )
                {
                    *ದತ್ತಾಂಶ++ = c;
                    need_quote = ನಿಜ;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *ದತ್ತಾಂಶ++ = '&';
                    ಆದರೆ( c == '<' )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ, "lt", 2);
                        ದತ್ತಾಂಶ += 2;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '>' )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ, "gt", 2);
                        ದತ್ತಾಂಶ += 2;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '&' )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ, "amp", 3);
                        ದತ್ತಾಂಶ += 3;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\'' )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ, "apos", 4);
                        ದತ್ತಾಂಶ += 4;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\"' )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು( ದತ್ತಾಂಶ, "quot", 4);
                        ದತ್ತಾಂಶ += 4;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        snprintf( ದತ್ತಾಂಶ, ಗಾತ್ರ(buf) - (ದತ್ತಾಂಶ - buf), "#x%02x", (uchar)c );
                        ದತ್ತಾಂಶ += 4;
                    }
                    *ದತ್ತಾಂಶ++ = ';';
                    need_quote = 1;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    *ದತ್ತಾಂಶ++ = c;
            }
            ಆದರೆ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ನಿಜ;

            ಆದರೆ( need_quote )
                *ದತ್ತಾಂಶ++ = '\"';
            len = (ಪೂರ್ಣಾಂಕ)(ದತ್ತಾಂಶ - buf) - !need_quote;
            *ದತ್ತಾಂಶ++ = '\0';
            ದತ್ತಾಂಶ = buf + (ಪೂರ್ಣಾಂಕ)!need_quote;
        }

        writeScalar( ಕೀಲಿ, ದತ್ತಾಂಶ );
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

        ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ದತ್ತಾಂಶ);
        ಆದರೆ( ಕೀಲಿ && *ಕೀಲಿ == '\0' )
            ಕೀಲಿ = 0;

        FStruct_ದತ್ತಾಂಶ& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;

        ಆದರೆ( File_ಗಂಟು::isMap(struct_flags) ||
           (!File_ಗಂಟು::isCollection(struct_flags) && ಕೀಲಿ) )
        {
            write_ಟ್ಯಾಗ್( ಕೀಲಿ, CV_XML_OPENING_ಟ್ಯಾಗ್ );
            ಅಕ್ಷರ* ptr = fs->resize_Write_ಬಫರ್( fs->ಬಫರ್_Ptr(), len );
            ಸ್ಮರಣೆ_ನಕಲು( ptr, ದತ್ತಾಂಶ, len );
            fs->ಗಣ_ಬಫರ್_Ptr( ptr + len );
            write_ಟ್ಯಾಗ್( ಕೀಲಿ, CV_XML_CLOSING_ಟ್ಯಾಗ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಅಕ್ಷರ* ptr = fs->ಬಫರ್_Ptr();
            ಪೂರ್ಣಾಂಕ new_ಸ್ಥಾನಾಂತರ = (ಪೂರ್ಣಾಂಕ)(ptr - fs->ಬಫರ್_ಆರಂಭ()) + len;

            ಆದರೆ( ಕೀಲಿ )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ಗಂಟು::SEQ;

            ಆದರೆ( (new_ಸ್ಥಾನಾಂತರ > fs->wrapMargin() && new_ಸ್ಥಾನಾಂತರ - current_struct.indent > 10) ||
               (ptr > fs->ಬಫರ್_ಆರಂಭ() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ptr > fs->ಬಫರ್_ಆರಂಭ() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            ಸ್ಮರಣೆ_ನಕಲು( ptr, ದತ್ತಾಂಶ, len );
            fs->ಗಣ_ಬಫರ್_Ptr(ptr + len);
        }
    }

    ಶೂನ್ಯ writeComment(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        FStruct_ದತ್ತಾಂಶ& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಂಕ len;
        ಪೂರ್ಣಾಂಕ multiline;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* eol;
        ಅಕ್ಷರ* ptr;

        ಆದರೆ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಆದರೆ( ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->ಬಫರ್_Ptr();

        ಆದರೆ( multiline || !eol_comment || fs->ಬಫರ್_ಕೊನೆ_2() - ptr < len + 5 )
            ptr = fs->flush();
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ptr > fs->ಬಫರ್_ಆರಂಭ() + current_struct.indent )
            *ptr++ = ' ';

        ಆದರೆ( !multiline )
        {
            ptr = fs->resize_Write_ಬಫರ್( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ptr);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->ಗಣ_ಬಫರ್_Ptr(ptr + len);
        ptr = fs->flush();

        ಆದರೆ( multiline )
        {
            ಆಗ( comment )
            {
                ಆದರೆ( eol )
                {
                    ptr = fs->resize_Write_ಬಫರ್( ptr, (ಪೂರ್ಣಾಂಕ)(eol - comment) + 1 );
                    ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment);
                    ptr = fs->resize_Write_ಬಫರ್( ptr, len );
                    ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->ಗಣ_ಬಫರ್_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->ಗಣ_ಬಫರ್_Ptr(ptr + 3);
            fs->flush();
        }
    }

    ಶೂನ್ಯ ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

ಸಂರಕ್ಷಿತ:
    FileStorage_API* fs;
};

ವರ್ಗ XMLParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ವಾಸ್ತವಿಕ ~XMLParser() {}

    ಅಕ್ಷರ* skipSpaces( ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಪೂರ್ಣಾಂಕ level = 0;

        ಗಾಗಿ(;;)
        {
            ಅಕ್ಷರ c;
            ptr--;

            ಆದರೆ( ಬಹುಲಕ == CV_XML_INSIDE_COMMENT )
            {
                ಮಾಡು c = *++ptr;
                ಆಗ( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ಆದರೆ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    ಬಹುಲಕ = 0;
                    ptr += 3;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಬಹುಲಕ == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                ಮಾಡು
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ಆದರೆ( level < 0 )
                        ಹಿಂತಿರುಗಿ ptr;
                } ಆಗ( cv_isprint_or_tab(c) );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಮಾಡು c = *++ptr;
                ಆಗ( c == ' ' || c == '\t' );

                ಆದರೆ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ಆದರೆ( ಬಹುಲಕ != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    ಬಹುಲಕ = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cv_isprint(c) )
                    ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ( !cv_isprint(*ptr) )
            {
                ಆದರೆ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ಆದರೆ( !ptr || *ptr == '\0' )
                    ನಿಲ್ಲಿಸು;
            }
        }
        ಹಿಂತಿರುಗಿ ptr;
    }

    ತಾರ್ಕಿಕ get_Base64_ಅಡ್ಡಸಾಲು(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ /*indent*/, ಅಕ್ಷರ* &beg, ಅಕ್ಷರ* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_ಟ್ಯಾಗ್);
        ಆದರೆ( !ptr || !*ptr )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        // closing XML tag
        ಆದರೆ ( *beg == '<' )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        // find end of the row
        ಆಗ( cv_isprint(*ptr) )
            ++ptr;
        ಆದರೆ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಅಕ್ಷರ* parse_ಮೌಲ್ಯ( ಅಕ್ಷರ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಂಕ ಆಳ = 0 )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಆದರೆ( ಆಳ > CV_PERSISTENCE_MAX_ಆಳ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ಗಂಟು new_elem;
        ತಾರ್ಕಿಕ have_space = ನಿಜ;
        ಪೂರ್ಣಾಂಕ value_type = ಗಂಟು.type();
        std::string ಕೀಲಿ, key2, type_name;

        ಗಾಗಿ(;;)
        {
            ಅಕ್ಷರ c = *ptr, d;
            ಅಕ್ಷರ* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ಆದರೆ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ಆದರೆ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = ನಿಜ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ಆದರೆ( c =='<' || c == '\0' )
            {
                ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್_type = 0;
                ಪೂರ್ಣಾಂಕ elem_type = File_ಗಂಟು::NONE;

                ಆದರೆ( d == '/' || c == '\0' )
                    ನಿಲ್ಲಿಸು;

                ptr = parse_ಟ್ಯಾಗ್( ptr, ಕೀಲಿ, type_name, ಟ್ಯಾಗ್_type );

                ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_DIRECTIVE_ಟ್ಯಾಗ್ )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_ಖಾಲಿಗಳು_ಟ್ಯಾಗ್ )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(ಟ್ಯಾಗ್_type == CV_XML_OPENING_ಟ್ಯಾಗ್);

                /* for base64 string */
                ತಾರ್ಕಿಕ ದ್ವಿಮಾನ_string = ಸುಳ್ಳು;

                ಆದರೆ( !type_name.empty() )
                {
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* tn = type_name.c_str();
                    ಆದರೆ( strcmp(tn, "str") == 0 )
                        elem_type = File_ಗಂಟು::STRING;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ಗಂಟು::MAP;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ಗಂಟು::SEQ;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( strcmp( tn, "binary") == 0)
                        ದ್ವಿಮಾನ_string = ನಿಜ;
                }

                new_elem = fs->ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, ಕೀಲಿ, elem_type, 0);
                ಆದರೆ (!ದ್ವಿಮಾನ_string)
                    ptr = parse_ಮೌಲ್ಯ(ptr, new_elem, ಆಳ + 1);
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ಆದರೆ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_ಟ್ಯಾಗ್( ptr, key2, type_name, ಟ್ಯಾಗ್_type );
                ಆದರೆ( ಟ್ಯಾಗ್_type != CV_XML_CLOSING_ಟ್ಯಾಗ್ || key2 != ಕೀಲಿ )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ನಿಜ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ಗಂಟು* elem = &ಗಂಟು;
                ಆದರೆ( ಗಂಟು.type() != File_ಗಂಟು::NONE )
                {
                    fs->convert_ಗಮ್ಯ_Collection( File_ಗಂಟು::SEQ, ಗಂಟು );
                    new_elem = fs->ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE, 0);
                    elem = &new_elem;
                }

                ಆದರೆ( value_type != File_ಗಂಟು::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ಆಗ( cv_isdigit(*endptr) )
                        endptr++;
                    ಆದರೆ( *endptr == '.' || *endptr == 'e' )
                    {
                        ದ್ವಿಗುಣ fval = fs->strtod( ptr, &endptr );
                        elem->ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                    }

                    ಆದರೆ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    // string
                    ಪೂರ್ಣಾಂಕ i = 0, len, is_quoted = 0;
                    ಆದರೆ( c == '\"' )
                        is_quoted = 1;
                    ಇಲ್ಲದಿದ್ದರೆ
                        --ptr;
                    strbuf[0] = '\0';

                    ಗಾಗಿ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();

                        ಆದರೆ( !cv_isalnum(c) )
                        {
                            ಆದರೆ( c == '\"' )
                            {
                                ಆದರೆ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                ನಿಲ್ಲಿಸು;
                            }
                            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ಆದರೆ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                ನಿಲ್ಲಿಸು;
                            }
                            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '&' )
                            {
                                ಆದರೆ( *++ptr == '#' )
                                {
                                    ಪೂರ್ಣಾಂಕ val, ತಳ = 10;
                                    ptr++;
                                    ಆದರೆ( *ptr == 'x' )
                                    {
                                        ತಳ = 16;
                                        ptr++;
                                    }
                                    val = (ಪೂರ್ಣಾಂಕ)strtol( ptr, &endptr, ತಳ );
                                    ಆದರೆ( (ಚಿಹ್ನೆರಹಿತ)val > (ಚಿಹ್ನೆರಹಿತ)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (ಅಕ್ಷರ)val;
                                }
                                ಇಲ್ಲದಿದ್ದರೆ
                                {
                                    endptr = ptr;
                                    ಮಾಡು c = *++endptr;
                                    ಆಗ( cv_isalnum(c) );
                                    ಆದರೆ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ಪೂರ್ಣಾಂಕ)(endptr - ptr);
                                    ಆದರೆ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ಇಲ್ಲದಿದ್ದರೆ
                                    {
                                        ಆದರೆ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        ಸ್ಮರಣೆ_ನಕಲು( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
                            }
                        }
                        ಆದರೆ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, strbuf, i);
                }

                ಆದರೆ( value_type != File_ಗಂಟು::NONE && value_type != File_ಗಂಟು::SEQ && value_type != File_ಗಂಟು::MAP )
                    ನಿಲ್ಲಿಸು;
                have_space = ಸುಳ್ಳು;
            }
        }
        fs->finalizeCollection(ಗಂಟು);

        ಹಿಂತಿರುಗಿ ptr;
    }

    ಅಕ್ಷರ* parse_ಟ್ಯಾಗ್( ಅಕ್ಷರ* ptr, std::string& ಟ್ಯಾಗ್_name,
                    std::string& type_name, ಪೂರ್ಣಾಂಕ& ಟ್ಯಾಗ್_type )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ಆದರೆ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ಆದರೆ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();

        ಆದರೆ( cv_isalnum(*ptr) || *ptr == '_' )
            ಟ್ಯಾಗ್_type = CV_XML_OPENING_ಟ್ಯಾಗ್;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( *ptr == '/' )
        {
            ಟ್ಯಾಗ್_type = CV_XML_CLOSING_ಟ್ಯಾಗ್;
            ptr++;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( *ptr == '?' )
        {
            ಟ್ಯಾಗ್_type = CV_XML_ಶೀರ್ಷಿಕೆ_ಟ್ಯಾಗ್;
            ptr++;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( *ptr == '!' )
        {
            ಟ್ಯಾಗ್_type = CV_XML_DIRECTIVE_ಟ್ಯಾಗ್;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        ಟ್ಯಾಗ್_name.clear();
        type_name.clear();

        ಗಾಗಿ(;;)
        {
            ಅಕ್ಷರ c, *endptr;
            ಆದರೆ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            ಮಾಡು c = *++endptr;
            ಆಗ( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();

            ಆದರೆ( ಟ್ಯಾಗ್_name.empty() )
                ಟ್ಯಾಗ್_name = attrname;
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_CLOSING_ಟ್ಯಾಗ್ )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ಆದರೆ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ಟ್ಯಾಗ್ );
                    ಆದರೆ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ಆದರೆ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ಆದರೆ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ಟ್ಯಾಗ್ );
                    ಆದರೆ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ಆದರೆ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                ಅಕ್ಷರ quote = *ptr++;
                endptr = ptr;
                ಗಾಗಿ(;;)
                {
                    c = *endptr++;
                    ಆದರೆ( c == quote )
                        ನಿಲ್ಲಿಸು;
                    ಆದರೆ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ಆದರೆ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            ತಾರ್ಕಿಕ have_space = cv_isspace(c) || c == '\0';

            ಆದರೆ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_ಟ್ಯಾಗ್ );
                ಆದರೆ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ಆದರೆ( c == '>' )
            {
                ಆದರೆ( ಟ್ಯಾಗ್_type == CV_XML_ಶೀರ್ಷಿಕೆ_ಟ್ಯಾಗ್ )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                ನಿಲ್ಲಿಸು;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '?' && ಟ್ಯಾಗ್_type == CV_XML_ಶೀರ್ಷಿಕೆ_ಟ್ಯಾಗ್ )
            {
                ಆದರೆ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                ನಿಲ್ಲಿಸು;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c == '/' && ptr[1] == '>' && ಟ್ಯಾಗ್_type == CV_XML_OPENING_ಟ್ಯಾಗ್ )  // FIXIT ptr[1] - out of bounds read without check
            {
                ಟ್ಯಾಗ್_type = CV_XML_ಖಾಲಿಗಳು_ಟ್ಯಾಗ್;
                ptr += 2;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ಹಿಂತಿರುಗಿ ptr;
    }

    ತಾರ್ಕಿಕ parse(ಅಕ್ಷರ* ptr)
    {
        CV_Assert( fs != 0 );

        std::string ಕೀಲಿ, key2, type_name;
        ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್_type = 0;
        ತಾರ್ಕಿಕ ok = ಸುಳ್ಳು;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_ಟ್ಯಾಗ್ );
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಆದರೆ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_ಟ್ಯಾಗ್( ptr, ಕೀಲಿ, type_name, ಟ್ಯಾಗ್_type );
        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಆಗ( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ಆದರೆ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ಆದರೆ( *ptr != '\0' )
            {
                ptr = parse_ಟ್ಯಾಗ್( ptr, ಕೀಲಿ, type_name, ಟ್ಯಾಗ್_type );
                ಆದರೆ( ಟ್ಯಾಗ್_type != CV_XML_OPENING_ಟ್ಯಾಗ್ || ಕೀಲಿ != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ಗಂಟು ಬೇರು = fs->ಕೂಡಿಸು_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::MAP, 0);
                ptr = parse_ಮೌಲ್ಯ( ptr, ಬೇರು );
                ptr = parse_ಟ್ಯಾಗ್( ptr, key2, type_name, ಟ್ಯಾಗ್_type );
                ಆದರೆ( ಟ್ಯಾಗ್_type != CV_XML_CLOSING_ಟ್ಯಾಗ್ || ಕೀಲಿ != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ನಿಜ;
            }
        }
        CV_Assert( fs->eof() );
        ಹಿಂತಿರುಗಿ ok;
    }

    FileStorage_API* fs;
    ಅಕ್ಷರ strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<XMLParser>(fs);
}

}
