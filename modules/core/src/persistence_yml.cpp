// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ಗಣನಾ
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
    ಆಭಾಸ ~YAMLEmitter() {}

    FStructData startWriteStruct(ಧ್ರುವ FStructData& parent, ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ,
                                 ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ* type_name=0)
    {
        ವರ್ಣ buf[CV_FS_MAX_LEN + 1024];
        ಧ್ರುವ ವರ್ಣ* data = 0;

        ಯದಿ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_MASK|File_ಗಂಟು::FLOW)) | File_ಗಂಟು::EMPTY;
        ಯದಿ( !File_ಗಂಟು::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ಯದಿ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ಗಂಟು::SEQ;
            snprintf(buf, ಪರಿಮಾಣ(buf), "!!binary |");
            data = buf;
        }
        ಅನ್ಯಥಾ ಯದಿ( File_ಗಂಟು::isFlow(struct_flags))
        {
            ವರ್ಣ c = File_ಗಂಟು::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ಗಂಟು::FLOW;

            ಯದಿ( type_name )
                snprintf( buf, ಪರಿಮಾಣ(buf), "!!%s %c", type_name, c );
            ಅನ್ಯಥಾ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ಅನ್ಯಥಾ ಯದಿ( type_name )
        {
            snprintf( buf, ಪರಿಮಾಣ(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( ಕೀಲಿ, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ಯದಿ( !File_ಗಂಟು::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_ಗಂಟು::isFlow(struct_flags);

        ಪ್ರತ್ಯಾಗಮ fsd;
    }

    ಶೂನ್ಯ endWriteStruct(ಧ್ರುವ FStructData& current_struct)
    {
        ವರ್ಣ* ptr;

        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;

        ಯದಿ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ಯದಿ( ptr > fs->bufferStart() + current_struct.indent && !File_ಗಂಟು::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ಅನ್ಯಥಾ ಯದಿ( File_ಗಂಟು::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_ಗಂಟು::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
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
        writeScalar( ಕೀಲಿ, fs::doubleToString( buf, ಪರಿಮಾಣ(buf), value, ಅಸತ್ಯ ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* str, ತಾರ್ಕಿಕ quote)
    {
        ವರ್ಣ buf[CV_FS_MAX_LEN*4+16];
        ವರ್ಣ* data = (ವರ್ಣ*)str;
        ಪೂರ್ಣಾಙ್ಕ i, len;

        ಯದಿ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಙ್ಕ)strlen(str);
        ಯದಿ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಯದಿ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ಪೂರ್ಣಾಙ್ಕ need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            ಪ್ರತಿ( i = 0; i < len; i++ )
            {
                ವರ್ಣ c = str[i];

                ಯದಿ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ಯದಿ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ಯದಿ( cv_isprint(c) )
                        *data++ = c;
                    ಅನ್ಯಥಾ ಯದಿ( c == '\n' )
                        *data++ = 'n';
                    ಅನ್ಯಥಾ ಯದಿ( c == '\r' )
                        *data++ = 'r';
                    ಅನ್ಯಥಾ ಯದಿ( c == '\t' )
                        *data++ = 't';
                    ಅನ್ಯಥಾ
                    {
                        snprintf( data, ಪರಿಮಾಣ(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ಅನ್ಯಥಾ
                    *data++ = c;
            }
            ಯದಿ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ಯದಿ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ಕೀಲಿ, data);
    }

    ಶೂನ್ಯ writeScalar(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* data)
    {
        fs->check_if_write_struct_is_delayed(ಅಸತ್ಯ);
        ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ಅನ್ಯಥಾ ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ಪೂರ್ಣಾಙ್ಕ i, keylen = 0;
        ಪೂರ್ಣಾಙ್ಕ datalen = 0;
        ವರ್ಣ* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;

        ಯದಿ( ಕೀಲಿ && ಕೀಲಿ[0] == '\0' )
            ಕೀಲಿ = 0;

        ಯದಿ( File_ಗಂಟು::isCollection(struct_flags) )
        {
            ಯದಿ( (File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ಅನ್ಯಥಾ
        {
            fs->setNonEmpty();
            struct_flags = File_ಗಂಟು::EMPTY | (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
        }

        ಯದಿ( ಕೀಲಿ )
        {
            keylen = (ಪೂರ್ಣಾಙ್ಕ)strlen(ಕೀಲಿ);
            ಯದಿ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ಯದಿ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ಯದಿ( data )
            datalen = (ಪೂರ್ಣಾಙ್ಕ)strlen(data);

        ಯದಿ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ಯದಿ( !File_ಗಂಟು::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ಪೂರ್ಣಾಙ್ಕ new_offset = (ಪೂರ್ಣಾಙ್ಕ)(ptr - fs->bufferStart()) + keylen + datalen;
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
            ptr = fs->flush();
            ಯದಿ( !File_ಗಂಟು::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ಯದಿ( data )
                    *ptr++ = ' ';
            }
        }

        ಯದಿ( ಕೀಲಿ )
        {
            ಯದಿ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            ಪ್ರತಿ( i = 0; i < keylen; i++ )
            {
                ವರ್ಣ c = ಕೀಲಿ[i];

                ptr[i] = c;
                ಯದಿ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ಯದಿ( !File_ಗಂಟು::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ಯದಿ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ಗಂಟು::EMPTY;
    }

    ಶೂನ್ಯ writeComment(ಧ್ರುವ ವರ್ಣ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        ಯದಿ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)strlen(comment);
        ಧ್ರುವ ವರ್ಣ* eol = strchr(comment, '\n');
        ತಾರ್ಕಿಕ multiline = eol != 0;
        ವರ್ಣ* ptr = fs->bufferPtr();

        ಯದಿ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ಅನ್ಯಥಾ
            *ptr++ = ' ';

        ಯಾವತ್( comment )
        {
            *ptr++ = '#';
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


ವರ್ಗ YAMLParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ಆಭಾಸ ~YAMLParser() {}

    ವರ್ಣ* skipSpaces( ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ min_indent, ಪೂರ್ಣಾಙ್ಕ max_comment_indent )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಪ್ರತಿ(;;)
        {
            ಯಾವತ್( *ptr == ' ' )
                ptr++;
            ಯದಿ( *ptr == '#' )
            {
                ಯದಿ( ptr - fs->bufferStart() > max_comment_indent )
                    ಪ್ರತ್ಯಾಗಮ ptr;
                *ptr = '\0';
            }
            ಅನ್ಯಥಾ ಯದಿ( cv_isprint(*ptr) )
            {
                ಯದಿ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ಭಙ್ಗ;
            }
            ಅನ್ಯಥಾ ಯದಿ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ಯದಿ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    ಭಙ್ಗ;
                }
                ಅನ್ಯಥಾ
                {
                    ಪೂರ್ಣಾಙ್ಕ l = (ಪೂರ್ಣಾಙ್ಕ)strlen(ptr);
                    ಯದಿ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ಅನ್ಯಥಾ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ತಾರ್ಕಿಕ getBase64Row(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ indent, ವರ್ಣ* &beg, ವರ್ಣ* &end)
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ಯದಿ (!ptr || !*ptr)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; // end of file

        ಯದಿ (ptr - fs->bufferStart() != indent)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; // end of base64 data

        /* find end */
        ಯಾವತ್(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ಯದಿ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }


    ವರ್ಣ* parse_ಕೀಲಿ( ವರ್ಣ* ptr, File_ಗಂಟು& map_ಗಂಟು, File_ಗಂಟು& ಮೌಲ್ಯ_placeholder )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ವರ್ಣ c;
        ವರ್ಣ *endptr = ptr - 1, *saveptr;

        ಯದಿ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        ಕುರು c = *++endptr;
        ಯಾವತ್( cv_isprint(c) && c != ':' );

        ಯದಿ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ಯದಿ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        ಕುರು c = *--endptr;
        ಯಾವತ್( c == ' ' );

        ++endptr;

        ಮೌಲ್ಯ_placeholder = fs->add_ಗಂಟು(map_ಗಂಟು, std::string(ptr, endptr - ptr), File_ಗಂಟು::NONE);
        ptr = saveptr;

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ವರ್ಣ* parse_ಮೌಲ್ಯ( ವರ್ಣ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಙ್ಕ min_indent, ತಾರ್ಕಿಕ is_parent_flow, ಪೂರ್ಣಾಙ್ಕ depth = 0 )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಯದಿ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ವರ್ಣ* endptr = 0;
        ವರ್ಣ c = ptr[0], d = ptr[1];
        ಪೂರ್ಣಾಙ್ಕ value_type = File_ಗಂಟು::NONE;
        ಪೂರ್ಣಾಙ್ಕ len;
        ತಾರ್ಕಿಕ is_ದ್ವಿಮಾನ_string = ಅಸತ್ಯ;
        ತಾರ್ಕಿಕ is_user = ಅಸತ್ಯ;

        ಯದಿ( c == '!' ) // handle explicit type specification
        {
            ಯದಿ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = ಸತ್ಯ;
                //value_type |= FileNode::USER;
            }
            ಯದಿ ( d == '<') //support of full type heading from YAML 1.2
            {
                ಧ್ರುವ ವರ್ಣ* yamlTypeHeading = "<tag:yaml.org,2002:";
                ಧ್ರುವ size_t headingLength = strlen(yamlTypeHeading);

                ವರ್ಣ* typeEndPtr = ++ptr;

                ಕುರು d = *++typeEndPtr;
                ಯಾವತ್( cv_isprint(d) && d != ' ' && d != '>' );

                ಯದಿ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ಯದಿ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = ಸತ್ಯ;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            ಕುರು d = *++endptr;
            ಯಾವತ್( cv_isprint(d) && d != ' ' );
            len = (ಪೂರ್ಣಾಙ್ಕ)(endptr - ptr);
            ಯದಿ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ಯದಿ( len == 3 && !is_user )
            {
                ಯದಿ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ಗಂಟು::STRING;
                ಅನ್ಯಥಾ ಯದಿ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ಗಂಟು::INT;
                ಅನ್ಯಥಾ ಯದಿ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ಗಂಟು::SEQ;
                ಅನ್ಯಥಾ ಯದಿ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ಗಂಟು::MAP;
            }
            ಅನ್ಯಥಾ ಯದಿ( len == 5 && !is_user )
            {
                ಯದಿ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ಗಂಟು::REAL;
            }
            ಅನ್ಯಥಾ ಯದಿ (len == 6 && is_user)
            {
                ಯದಿ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ಗಂಟು::SEQ;
                    is_ದ್ವಿಮಾನ_string = ಸತ್ಯ;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    ಕುರು {
                        d = *++endptr;
                        ಯದಿ (d == '|')
                            ಭಙ್ಗ;
                    } ಯಾವತ್ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ಯದಿ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ಯದಿ( !is_user )
            {
                ಯದಿ (value_type == File_ಗಂಟು::STRING && c != '\'' && c != '\"')
                    ಗಚ್ಛ force_string;
                ಯದಿ( value_type == File_ಗಂಟು::INT )
                    ಗಚ್ಛ force_int;
                ಯದಿ( value_type == File_ಗಂಟು::REAL )
                    ಗಚ್ಛ force_real;
            }
        }

        ಯದಿ (is_ದ್ವಿಮಾನ_string)
        {
            ಪೂರ್ಣಾಙ್ಕ indent = static_cast<ಪೂರ್ಣಾಙ್ಕ>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, ಗಂಟು);
        }
        ಅನ್ಯಥಾ ಯದಿ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ಯಾವತ್( cv_isdigit(*endptr) )
                endptr++;
            ಯದಿ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ದ್ವಿಗುಣ fval = fs->strtod( ptr, &endptr );
                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
            }
            ಅನ್ಯಥಾ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }

            ಯದಿ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ಅನ್ಯಥಾ ಯದಿ( c == '\'' || c == '\"' ) // an explicit string
        {
            ಯದಿ( c == '\'' )
                ಪ್ರತಿ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ಯದಿ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ಅನ್ಯಥಾ ಯದಿ( c == '\'' )
                    {
                        c = *++ptr;
                        ಯದಿ( c != '\'' )
                            ಭಙ್ಗ;
                        buf[len++] = c;
                    }
                    ಅನ್ಯಥಾ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ಅನ್ಯಥಾ
                ಪ್ರತಿ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ಯದಿ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ಅನ್ಯಥಾ ಯದಿ( c == '\"' )
                    {
                        ++ptr;
                        ಭಙ್ಗ;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( c == '\\' )
                    {
                        d = *++ptr;
                        ಯದಿ( d == '\'' )
                            buf[len++] = d;
                        ಅನ್ಯಥಾ ಯದಿ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ಅನ್ಯಥಾ ಯದಿ( d == 'n' )
                            buf[len++] = '\n';
                        ಅನ್ಯಥಾ ಯದಿ( d == 'r' )
                            buf[len++] = '\r';
                        ಅನ್ಯಥಾ ಯದಿ( d == 't' )
                            buf[len++] = '\t';
                        ಅನ್ಯಥಾ ಯದಿ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ಪೂರ್ಣಾಙ್ಕ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ಪೂರ್ಣಾಙ್ಕ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ಯದಿ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ಅನ್ಯಥಾ
                            {
                                buf[len++] = (ವರ್ಣ)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ಅನ್ಯಥಾ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ಯದಿ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, buf, len);
        }
        ಅನ್ಯಥಾ ಯದಿ( c == '[' || c == '{' ) // collection as a flow
        {
            ಪೂರ್ಣಾಙ್ಕ new_min_indent = min_indent + !is_parent_flow;
            ಪೂರ್ಣಾಙ್ಕ struct_type = c == '{' ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ;
            ಪೂರ್ಣಾಙ್ಕ nelems = 0;

            fs->convertToCollection(struct_type, ಗಂಟು);
            d = c == '[' ? ']' : '}';

            ಪ್ರತಿ( ++ptr ;; nelems++ )
            {
                File_ಗಂಟು elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ಯದಿ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ಯದಿ( *ptr == '}' || *ptr == ']' )
                {
                    ಯದಿ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ಭಙ್ಗ;
                }

                ಯದಿ( nelems != 0 )
                {
                    ಯದಿ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ಯದಿ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ಯದಿ( struct_type == File_ಗಂಟು::MAP )
                {
                    ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ಅನ್ಯಥಾ
                {
                    ಯದಿ( *ptr == ']' )
                        ಭಙ್ಗ;
                    elem = fs->add_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE);
                }
                ptr = parse_ಮೌಲ್ಯ( ptr, elem, new_min_indent, ಸತ್ಯ, depth + 1 );
            }
            fs->finalizeCollection(ಗಂಟು);
        }
        ಅನ್ಯಥಾ
        {
            ಪೂರ್ಣಾಙ್ಕ indent, struct_type;

            ಯದಿ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ಯದಿ( !is_parent_flow )
                {
                    ಯದಿ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ಯದಿ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                ಕುರು c = *++endptr;
                ಯಾವತ್( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_ಗಂಟು::STRING));

                ಯದಿ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ಯದಿ( is_parent_flow || c != ':' )
                {
                    ವರ್ಣ* str_end = endptr;
                    ಕುರು c = *--str_end;
                    ಯಾವತ್( str_end > ptr && c == ' ' );
                    str_end++;
                    ಯದಿ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ಯದಿ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                            ptr = endptr;
                            ಪ್ರತ್ಯಾಗಮ ptr;
                        }
                        ಅನ್ಯಥಾ ಯದಿ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                            ptr = endptr;
                            ಪ್ರತ್ಯಾಗಮ ptr;
                        }
                        ಅನ್ಯಥಾ ಯದಿ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ಪ್ರತ್ಯಾಗಮ ptr;
                        }
                    }

                    ಗಂಟು.set_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, ptr, (ಪೂರ್ಣಾಙ್ಕ)(str_end - ptr));
                    ptr = endptr;
                    ಪ್ರತ್ಯಾಗಮ ptr;
                }
                struct_type = File_ಗಂಟು::MAP;
            }
            ಅನ್ಯಥಾ
                struct_type = File_ಗಂಟು::SEQ;

            fs->convertToCollection( struct_type, ಗಂಟು );
            indent = (ಪೂರ್ಣಾಙ್ಕ)(ptr - fs->bufferStart());

            ಪ್ರತಿ(;;)
            {
                File_ಗಂಟು elem;

                ಯದಿ( struct_type == File_ಗಂಟು::MAP )
                {
                    ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, elem );
                }
                ಅನ್ಯಥಾ
                {
                    c = *ptr++;
                    ಯದಿ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_ಮೌಲ್ಯ( ptr, elem, indent + 1, ಅಸತ್ಯ, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ಯದಿ( ptr - fs->bufferStart() != indent )
                {
                    ಯದಿ( ptr - fs->bufferStart() < indent )
                        ಭಙ್ಗ;
                    ಅನ್ಯಥಾ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ಯದಿ( memcmp( ptr, "...", 3 ) == 0 )
                    ಭಙ್ಗ;
            }
            fs->finalizeCollection(ಗಂಟು);
        }

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ತಾರ್ಕಿಕ parse( ವರ್ಣ* ptr )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ತಾರ್ಕಿಕ first = ಸತ್ಯ;
        ತಾರ್ಕಿಕ ok = ಸತ್ಯ;
        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಪ್ರತಿ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            ಪ್ರತಿ(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ಯದಿ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ಭಙ್ಗ;
                }

                ಯದಿ( *ptr == '%' )
                {
                    ಯದಿ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ಯದಿ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ಅನ್ಯಥಾ ಯದಿ( *ptr == '-' )
                {
                    ಯದಿ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ಭಙ್ಗ;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( first )
                        ಭಙ್ಗ;
                }
                ಅನ್ಯಥಾ ಯದಿ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ಯದಿ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ಭಙ್ಗ;
                }
                ಅನ್ಯಥಾ ಯದಿ( fs->eof() )
                    ಭಙ್ಗ;
                ಅನ್ಯಥಾ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ಯದಿ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ಯದಿ( !ptr || !ptr[0] )
                ಭಙ್ಗ;
            ಯದಿ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->add_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::NONE);

                ptr = parse_ಮೌಲ್ಯ( ptr, ಬೇರು_ಗಂಟು, 0, ಅಸತ್ಯ );
                ಯದಿ( !ಬೇರು_ಗಂಟು.isMap() && !ಬೇರು_ಗಂಟು.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ಯದಿ( !ptr )
                    ಭಙ್ಗ;
            }

            ಯದಿ( fs->eof() )
                ಭಙ್ಗ;
            ptr += 3;
            first = ಅಸತ್ಯ;
        }

        ಪ್ರತ್ಯಾಗಮ ok;
    }

    FileStorage_API* fs;
    ವರ್ಣ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<YAMLParser>(fs);
}

}
