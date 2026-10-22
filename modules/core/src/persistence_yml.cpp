// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

गणना
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

नामाकाश cv
{

वग्ग YAMLEmitter : सार्वजनिक FileStorageEmitter
{
सार्वजनिक:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    आभास ~YAMLEmitter() {}

    FStructData startWriteStruct(ध्रुव FStructData& parent, ध्रुव वर्ण* कुञ्चिका,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        वर्ण buf[CV_FS_MAX_LEN + 1024];
        ध्रुव वर्ण* data = 0;

        सचे ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_MASK|File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::EMPTY;
        सचे( !File_ग्रन्थिः::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        सचे (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ग्रन्थिः::SEQ;
            snprintf(buf, परिमाण(buf), "!!binary |");
            data = buf;
        }
        अञ्ञथा सचे( File_ग्रन्थिः::isFlow(struct_flags))
        {
            वर्ण c = File_ग्रन्थिः::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ग्रन्थिः::FLOW;

            सचे( type_name )
                snprintf( buf, परिमाण(buf), "!!%s %c", type_name, c );
            अञ्ञथा
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        अञ्ञथा सचे( type_name )
        {
            snprintf( buf, परिमाण(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( कुञ्चिका, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        सचे( !File_ग्रन्थिः::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_ग्रन्थिः::isFlow(struct_flags);

        पटिनिवत्त fsd;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        वर्ण* ptr;

        पूर्णाङ्क struct_flags = current_struct.flags;

        सचे( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            सचे( ptr > fs->bufferStart() + current_struct.indent && !File_ग्रन्थिः::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        अञ्ञथा सचे( File_ग्रन्थिः::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_ग्रन्थिः::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
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
        writeScalar( कुञ्चिका, fs::doubleToString( buf, परिमाण(buf), value, मिच्छा ));
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*4+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        सचे( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)strlen(str);
        सचे( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        सचे( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णाङ्क need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            अत्थाय( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                सचे( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                सचे( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    सचे( cv_isprint(c) )
                        *data++ = c;
                    अञ्ञथा सचे( c == '\n' )
                        *data++ = 'n';
                    अञ्ञथा सचे( c == '\r' )
                        *data++ = 'r';
                    अञ्ञथा सचे( c == '\t' )
                        *data++ = 't';
                    अञ्ञथा
                    {
                        snprintf( data, परिमाण(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                अञ्ञथा
                    *data++ = c;
            }
            सचे( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            सचे( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( कुञ्चिका, data);
    }

    शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* data)
    {
        fs->check_if_write_struct_is_delayed(मिच्छा);
        सचे ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अञ्ञथा सचे ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णाङ्क i, keylen = 0;
        पूर्णाङ्क datalen = 0;
        वर्ण* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        पूर्णाङ्क struct_flags = current_struct.flags;

        सचे( कुञ्चिका && कुञ्चिका[0] == '\0' )
            कुञ्चिका = 0;

        सचे( File_ग्रन्थिः::isCollection(struct_flags) )
        {
            सचे( (File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        अञ्ञथा
        {
            fs->setNonEmpty();
            struct_flags = File_ग्रन्थिः::EMPTY | (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
        }

        सचे( कुञ्चिका )
        {
            keylen = (पूर्णाङ्क)strlen(कुञ्चिका);
            सचे( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            सचे( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        सचे( data )
            datalen = (पूर्णाङ्क)strlen(data);

        सचे( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            सचे( !File_ग्रन्थिः::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            पूर्णाङ्क new_offset = (पूर्णाङ्क)(ptr - fs->bufferStart()) + keylen + datalen;
            सचे( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            अञ्ञथा
                *ptr++ = ' ';
        }
        अञ्ञथा
        {
            ptr = fs->flush();
            सचे( !File_ग्रन्थिः::isMap(struct_flags) )
            {
                *ptr++ = '-';
                सचे( data )
                    *ptr++ = ' ';
            }
        }

        सचे( कुञ्चिका )
        {
            सचे( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            अत्थाय( i = 0; i < keylen; i++ )
            {
                वर्ण c = कुञ्चिका[i];

                ptr[i] = c;
                सचे( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            सचे( !File_ग्रन्थिः::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        सचे( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ग्रन्थिः::EMPTY;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        सचे( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = (पूर्णाङ्क)strlen(comment);
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;
        वर्ण* ptr = fs->bufferPtr();

        सचे( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        अञ्ञथा
            *ptr++ = ' ';

        याव( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            सचे( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अञ्ञथा
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


वग्ग YAMLParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभास ~YAMLParser() {}

    वर्ण* skipSpaces( वर्ण* ptr, पूर्णाङ्क min_indent, पूर्णाङ्क max_comment_indent )
    {
        सचे (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अत्थाय(;;)
        {
            याव( *ptr == ' ' )
                ptr++;
            सचे( *ptr == '#' )
            {
                सचे( ptr - fs->bufferStart() > max_comment_indent )
                    पटिनिवत्त ptr;
                *ptr = '\0';
            }
            अञ्ञथा सचे( cv_isprint(*ptr) )
            {
                सचे( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                भङ्ग;
            }
            अञ्ञथा सचे( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                सचे( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    भङ्ग;
                }
                अञ्ञथा
                {
                    पूर्णाङ्क l = (पूर्णाङ्क)strlen(ptr);
                    सचे( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            अञ्ञथा
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        पटिनिवत्त ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क indent, वर्ण* &beg, वर्ण* &end)
    {
        सचे (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        सचे (!ptr || !*ptr)
            पटिनिवत्त मिच्छा; // end of file

        सचे (ptr - fs->bufferStart() != indent)
            पटिनिवत्त मिच्छा; // end of base64 data

        /* find end */
        याव(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        सचे (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        पटिनिवत्त सच्च;
    }


    वर्ण* parse_कुञ्चिका( वर्ण* ptr, File_ग्रन्थिः& map_ग्रन्थिः, File_ग्रन्थिः& मानम्_placeholder )
    {
        सचे (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        वर्ण c;
        वर्ण *endptr = ptr - 1, *saveptr;

        सचे( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        करोहि c = *++endptr;
        याव( cv_isprint(c) && c != ':' );

        सचे( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        सचे( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        करोहि c = *--endptr;
        याव( c == ' ' );

        ++endptr;

        मानम्_placeholder = fs->add_ग्रन्थिः(map_ग्रन्थिः, std::string(ptr, endptr - ptr), File_ग्रन्थिः::NONE);
        ptr = saveptr;

        पटिनिवत्त ptr;
    }

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क min_indent, तार्किक is_parent_flow, पूर्णाङ्क depth = 0 )
    {
        सचे (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        सचे( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        वर्ण* endptr = 0;
        वर्ण c = ptr[0], d = ptr[1];
        पूर्णाङ्क value_type = File_ग्रन्थिः::NONE;
        पूर्णाङ्क len;
        तार्किक is_द्विमान_string = मिच्छा;
        तार्किक is_user = मिच्छा;

        सचे( c == '!' ) // handle explicit type specification
        {
            सचे( d == '!' || d == '^' )
            {
                ptr++;
                is_user = सच्च;
                //value_type |= FileNode::USER;
            }
            सचे ( d == '<') //support of full type heading from YAML 1.2
            {
                ध्रुव वर्ण* yamlTypeHeading = "<tag:yaml.org,2002:";
                ध्रुव size_t headingLength = strlen(yamlTypeHeading);

                वर्ण* typeEndPtr = ++ptr;

                करोहि d = *++typeEndPtr;
                याव( cv_isprint(d) && d != ' ' && d != '>' );

                सचे ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    सचे ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = सच्च;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            करोहि d = *++endptr;
            याव( cv_isprint(d) && d != ' ' );
            len = (पूर्णाङ्क)(endptr - ptr);
            सचे( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            सचे( len == 3 && !is_user )
            {
                सचे( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::STRING;
                अञ्ञथा सचे( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::INT;
                अञ्ञथा सचे( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::SEQ;
                अञ्ञथा सचे( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::MAP;
            }
            अञ्ञथा सचे( len == 5 && !is_user )
            {
                सचे( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ग्रन्थिः::REAL;
            }
            अञ्ञथा सचे (len == 6 && is_user)
            {
                सचे( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ग्रन्थिः::SEQ;
                    is_द्विमान_string = सच्च;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    करोहि {
                        d = *++endptr;
                        सचे (d == '|')
                            भङ्ग;
                    } याव (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            सचे (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            सचे( !is_user )
            {
                सचे (value_type == File_ग्रन्थिः::STRING && c != '\'' && c != '\"')
                    गच्छ force_string;
                सचे( value_type == File_ग्रन्थिः::INT )
                    गच्छ force_int;
                सचे( value_type == File_ग्रन्थिः::REAL )
                    गच्छ force_real;
            }
        }

        सचे (is_द्विमान_string)
        {
            पूर्णाङ्क indent = static_cast<पूर्णाङ्क>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, ग्रन्थिः);
        }
        अञ्ञथा सचे( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            याव( cv_isdigit(*endptr) )
                endptr++;
            सचे( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                द्विगुण fval = fs->strtod( ptr, &endptr );
                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::REAL, &fval);
            }
            अञ्ञथा
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ग्रन्थिः.set_मानम्(File_ग्रन्थिः::INT, &ival);
            }

            सचे( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        अञ्ञथा सचे( c == '\'' || c == '\"' ) // an explicit string
        {
            सचे( c == '\'' )
                अत्थाय( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    सचे( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    अञ्ञथा सचे( c == '\'' )
                    {
                        c = *++ptr;
                        सचे( c != '\'' )
                            भङ्ग;
                        buf[len++] = c;
                    }
                    अञ्ञथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            अञ्ञथा
                अत्थाय( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    सचे( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    अञ्ञथा सचे( c == '\"' )
                    {
                        ++ptr;
                        भङ्ग;
                    }
                    अञ्ञथा सचे( c == '\\' )
                    {
                        d = *++ptr;
                        सचे( d == '\'' )
                            buf[len++] = d;
                        अञ्ञथा सचे( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        अञ्ञथा सचे( d == 'n' )
                            buf[len++] = '\n';
                        अञ्ञथा सचे( d == 'r' )
                            buf[len++] = '\r';
                        अञ्ञथा सचे( d == 't' )
                            buf[len++] = '\t';
                        अञ्ञथा सचे( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            पूर्णाङ्क val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (पूर्णाङ्क)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            सचे( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            अञ्ञथा
                            {
                                buf[len++] = (वर्ण)val;
                                ptr = endptr;
                            }
                        }
                    }
                    अञ्ञथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            सचे( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ग्रन्थिः.set_मानम्(File_ग्रन्थिः::STRING, buf, len);
        }
        अञ्ञथा सचे( c == '[' || c == '{' ) // collection as a flow
        {
            पूर्णाङ्क new_min_indent = min_indent + !is_parent_flow;
            पूर्णाङ्क struct_type = c == '{' ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ;
            पूर्णाङ्क nelems = 0;

            fs->convertToCollection(struct_type, ग्रन्थिः);
            d = c == '[' ? ']' : '}';

            अत्थाय( ++ptr ;; nelems++ )
            {
                File_ग्रन्थिः elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                सचे (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                सचे( *ptr == '}' || *ptr == ']' )
                {
                    सचे( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    भङ्ग;
                }

                सचे( nelems != 0 )
                {
                    सचे( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    सचे (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                सचे( struct_type == File_ग्रन्थिः::MAP )
                {
                    ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                अञ्ञथा
                {
                    सचे( *ptr == ']' )
                        भङ्ग;
                    elem = fs->add_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE);
                }
                ptr = parse_मानम्( ptr, elem, new_min_indent, सच्च, depth + 1 );
            }
            fs->finalizeCollection(ग्रन्थिः);
        }
        अञ्ञथा
        {
            पूर्णाङ्क indent, struct_type;

            सचे( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                सचे( !is_parent_flow )
                {
                    सचे( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    सचे( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                करोहि c = *++endptr;
                याव( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_ग्रन्थिः::STRING));

                सचे( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                सचे( is_parent_flow || c != ':' )
                {
                    वर्ण* str_end = endptr;
                    करोहि c = *--str_end;
                    याव( str_end > ptr && c == ' ' );
                    str_end++;
                    सचे ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        सचे (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ग्रन्थिः.set_मानम्(File_ग्रन्थिः::INT, &ival);
                            ptr = endptr;
                            पटिनिवत्त ptr;
                        }
                        अञ्ञथा सचे (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ग्रन्थिः.set_मानम्(File_ग्रन्थिः::INT, &ival);
                            ptr = endptr;
                            पटिनिवत्त ptr;
                        }
                        अञ्ञथा सचे (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            पटिनिवत्त ptr;
                        }
                    }

                    ग्रन्थिः.set_मानम्(File_ग्रन्थिः::STRING, ptr, (पूर्णाङ्क)(str_end - ptr));
                    ptr = endptr;
                    पटिनिवत्त ptr;
                }
                struct_type = File_ग्रन्थिः::MAP;
            }
            अञ्ञथा
                struct_type = File_ग्रन्थिः::SEQ;

            fs->convertToCollection( struct_type, ग्रन्थिः );
            indent = (पूर्णाङ्क)(ptr - fs->bufferStart());

            अत्थाय(;;)
            {
                File_ग्रन्थिः elem;

                सचे( struct_type == File_ग्रन्थिः::MAP )
                {
                    ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, elem );
                }
                अञ्ञथा
                {
                    c = *ptr++;
                    सचे( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_मानम्( ptr, elem, indent + 1, मिच्छा, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                सचे( ptr - fs->bufferStart() != indent )
                {
                    सचे( ptr - fs->bufferStart() < indent )
                        भङ्ग;
                    अञ्ञथा
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                सचे( memcmp( ptr, "...", 3 ) == 0 )
                    भङ्ग;
            }
            fs->finalizeCollection(ग्रन्थिः);
        }

        पटिनिवत्त ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        सचे (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        तार्किक first = सच्च;
        तार्किक ok = सच्च;
        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        अत्थाय(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            अत्थाय(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                सचे( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    भङ्ग;
                }

                सचे( *ptr == '%' )
                {
                    सचे( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        सचे ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                अञ्ञथा सचे( *ptr == '-' )
                {
                    सचे( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        भङ्ग;
                    }
                    अञ्ञथा सचे( first )
                        भङ्ग;
                }
                अञ्ञथा सचे( cv_isalnum(*ptr) || *ptr=='_')
                {
                    सचे( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    भङ्ग;
                }
                अञ्ञथा सचे( fs->eof() )
                    भङ्ग;
                अञ्ञथा
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            सचे( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            सचे( !ptr || !ptr[0] )
                भङ्ग;
            सचे( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->add_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::NONE);

                ptr = parse_मानम्( ptr, मूलम्_ग्रन्थिः, 0, मिच्छा );
                सचे( !मूलम्_ग्रन्थिः.isMap() && !मूलम्_ग्रन्थिः.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                सचे( !ptr )
                    भङ्ग;
            }

            सचे( fs->eof() )
                भङ्ग;
            ptr += 3;
            first = मिच्छा;
        }

        पटिनिवत्त ok;
    }

    FileStorage_API* fs;
    वर्ण buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    पटिनिवत्त makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    पटिनिवत्त makePtr<YAMLParser>(fs);
}

}
