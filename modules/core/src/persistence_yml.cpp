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

वर्ग YAMLEmitter : सार्वजनिक FileStorageEmitter
{
सार्वजनिक:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    आभास ~YAMLEmitter() {}

    FStructData startWriteStruct(ध्रुव FStructData& parent, ध्रुव वर्ण* किल्ली,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        वर्ण buf[CV_FS_MAX_LEN + 1024];
        ध्रुव वर्ण* data = 0;

        यदि ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_गाठ::TYPE_MASK|File_गाठ::FLOW)) | File_गाठ::EMPTY;
        यदि( !File_गाठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        यदि (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_गाठ::SEQ;
            snprintf(buf, परिमाण(buf), "!!binary |");
            data = buf;
        }
        अन्यथा यदि( File_गाठ::isFlow(struct_flags))
        {
            वर्ण c = File_गाठ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_गाठ::FLOW;

            यदि( type_name )
                snprintf( buf, परिमाण(buf), "!!%s %c", type_name, c );
            अन्यथा
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        अन्यथा यदि( type_name )
        {
            snprintf( buf, परिमाण(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( किल्ली, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        यदि( !File_गाठ::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_गाठ::isFlow(struct_flags);

        प्रत्यागम fsd;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        वर्ण* ptr;

        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( File_गाठ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            यदि( ptr > fs->bufferStart() + current_struct.indent && !File_गाठ::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_गाठ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        अन्यथा यदि( File_गाठ::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_गाठ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    शून्य लिख(ध्रुव वर्ण* किल्ली, पूर्णाङ्क value)
    {
        वर्ण buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10 ));
    }

    शून्य लिख(ध्रुव वर्ण* किल्ली, int64_t value)
    {
        वर्ण buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( ध्रुव वर्ण* किल्ली, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( किल्ली, fs::doubleToString( buf, परिमाण(buf), value, असत्य ));
    }

    शून्य लिख(ध्रुव वर्ण* किल्ली, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*4+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)strlen(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णाङ्क need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                यदि( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                यदि( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    यदि( cv_isprint(c) )
                        *data++ = c;
                    अन्यथा यदि( c == '\n' )
                        *data++ = 'n';
                    अन्यथा यदि( c == '\r' )
                        *data++ = 'r';
                    अन्यथा यदि( c == '\t' )
                        *data++ = 't';
                    अन्यथा
                    {
                        snprintf( data, परिमाण(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                अन्यथा
                    *data++ = c;
            }
            यदि( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            यदि( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( किल्ली, data);
    }

    शून्य writeScalar(ध्रुव वर्ण* किल्ली, ध्रुव वर्ण* data)
    {
        fs->check_if_write_struct_is_delayed(असत्य);
        यदि ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा यदि ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णाङ्क i, keylen = 0;
        पूर्णाङ्क datalen = 0;
        वर्ण* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( किल्ली && किल्ली[0] == '\0' )
            किल्ली = 0;

        यदि( File_गाठ::isCollection(struct_flags) )
        {
            यदि( (File_गाठ::isMap(struct_flags) ^ (किल्ली != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        अन्यथा
        {
            fs->setNonEmpty();
            struct_flags = File_गाठ::EMPTY | (किल्ली ? File_गाठ::MAP : File_गाठ::SEQ);
        }

        यदि( किल्ली )
        {
            keylen = (पूर्णाङ्क)strlen(किल्ली);
            यदि( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            यदि( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        यदि( data )
            datalen = (पूर्णाङ्क)strlen(data);

        यदि( File_गाठ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            यदि( !File_गाठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            पूर्णाङ्क new_offset = (पूर्णाङ्क)(ptr - fs->bufferStart()) + keylen + datalen;
            यदि( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            ptr = fs->flush();
            यदि( !File_गाठ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                यदि( data )
                    *ptr++ = ' ';
            }
        }

        यदि( किल्ली )
        {
            यदि( !cv_isalpha(किल्ली[0]) && किल्ली[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            प्रति( i = 0; i < keylen; i++ )
            {
                वर्ण c = किल्ली[i];

                ptr[i] = c;
                यदि( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            यदि( !File_गाठ::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        यदि( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_गाठ::EMPTY;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = (पूर्णाङ्क)strlen(comment);
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;
        वर्ण* ptr = fs->bufferPtr();

        यदि( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        अन्यथा
            *ptr++ = ' ';

        यावत्( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            यदि( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अन्यथा
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


वर्ग YAMLParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभास ~YAMLParser() {}

    वर्ण* skipSpaces( वर्ण* ptr, पूर्णाङ्क min_indent, पूर्णाङ्क max_comment_indent )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        प्रति(;;)
        {
            यावत्( *ptr == ' ' )
                ptr++;
            यदि( *ptr == '#' )
            {
                यदि( ptr - fs->bufferStart() > max_comment_indent )
                    प्रत्यागम ptr;
                *ptr = '\0';
            }
            अन्यथा यदि( cv_isprint(*ptr) )
            {
                यदि( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                भङ्ग;
            }
            अन्यथा यदि( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                यदि( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    भङ्ग;
                }
                अन्यथा
                {
                    पूर्णाङ्क l = (पूर्णाङ्क)strlen(ptr);
                    यदि( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            अन्यथा
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        प्रत्यागम ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क indent, वर्ण* &beg, वर्ण* &end)
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        यदि (!ptr || !*ptr)
            प्रत्यागम असत्य; // end of file

        यदि (ptr - fs->bufferStart() != indent)
            प्रत्यागम असत्य; // end of base64 data

        /* find end */
        यावत्(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        यदि (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        प्रत्यागम सत्य;
    }


    वर्ण* parse_किल्ली( वर्ण* ptr, File_गाठ& map_गाठ, File_गाठ& मूल्य_placeholder )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        वर्ण c;
        वर्ण *endptr = ptr - 1, *saveptr;

        यदि( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        कुरु c = *++endptr;
        यावत्( cv_isprint(c) && c != ':' );

        यदि( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        यदि( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        कुरु c = *--endptr;
        यावत्( c == ' ' );

        ++endptr;

        मूल्य_placeholder = fs->add_गाठ(map_गाठ, std::string(ptr, endptr - ptr), File_गाठ::NONE);
        ptr = saveptr;

        प्रत्यागम ptr;
    }

    वर्ण* parse_मूल्य( वर्ण* ptr, File_गाठ& गाठ, पूर्णाङ्क min_indent, तार्किक is_parent_flow, पूर्णाङ्क depth = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        वर्ण* endptr = 0;
        वर्ण c = ptr[0], d = ptr[1];
        पूर्णाङ्क value_type = File_गाठ::NONE;
        पूर्णाङ्क len;
        तार्किक is_द्विमान_string = असत्य;
        तार्किक is_user = असत्य;

        यदि( c == '!' ) // handle explicit type specification
        {
            यदि( d == '!' || d == '^' )
            {
                ptr++;
                is_user = सत्य;
                //value_type |= FileNode::USER;
            }
            यदि ( d == '<') //support of full type heading from YAML 1.2
            {
                ध्रुव वर्ण* yamlTypeHeading = "<tag:yaml.org,2002:";
                ध्रुव size_t headingLength = strlen(yamlTypeHeading);

                वर्ण* typeEndPtr = ++ptr;

                कुरु d = *++typeEndPtr;
                यावत्( cv_isprint(d) && d != ' ' && d != '>' );

                यदि ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    यदि ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = सत्य;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            कुरु d = *++endptr;
            यावत्( cv_isprint(d) && d != ' ' );
            len = (पूर्णाङ्क)(endptr - ptr);
            यदि( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            यदि( len == 3 && !is_user )
            {
                यदि( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_गाठ::STRING;
                अन्यथा यदि( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_गाठ::INT;
                अन्यथा यदि( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_गाठ::SEQ;
                अन्यथा यदि( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_गाठ::MAP;
            }
            अन्यथा यदि( len == 5 && !is_user )
            {
                यदि( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_गाठ::REAL;
            }
            अन्यथा यदि (len == 6 && is_user)
            {
                यदि( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_गाठ::SEQ;
                    is_द्विमान_string = सत्य;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    कुरु {
                        d = *++endptr;
                        यदि (d == '|')
                            भङ्ग;
                    } यावत् (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            यदि( !is_user )
            {
                यदि (value_type == File_गाठ::STRING && c != '\'' && c != '\"')
                    गच्छ force_string;
                यदि( value_type == File_गाठ::INT )
                    गच्छ force_int;
                यदि( value_type == File_गाठ::REAL )
                    गच्छ force_real;
            }
        }

        यदि (is_द्विमान_string)
        {
            पूर्णाङ्क indent = static_cast<पूर्णाङ्क>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, गाठ);
        }
        अन्यथा यदि( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            यावत्( cv_isdigit(*endptr) )
                endptr++;
            यदि( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                द्विगुण fval = fs->strtod( ptr, &endptr );
                गाठ.set_मूल्य(File_गाठ::REAL, &fval);
            }
            अन्यथा
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                गाठ.set_मूल्य(File_गाठ::INT, &ival);
            }

            यदि( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        अन्यथा यदि( c == '\'' || c == '\"' ) // an explicit string
        {
            यदि( c == '\'' )
                प्रति( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    यदि( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा यदि( c == '\'' )
                    {
                        c = *++ptr;
                        यदि( c != '\'' )
                            भङ्ग;
                        buf[len++] = c;
                    }
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            अन्यथा
                प्रति( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    यदि( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा यदि( c == '\"' )
                    {
                        ++ptr;
                        भङ्ग;
                    }
                    अन्यथा यदि( c == '\\' )
                    {
                        d = *++ptr;
                        यदि( d == '\'' )
                            buf[len++] = d;
                        अन्यथा यदि( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        अन्यथा यदि( d == 'n' )
                            buf[len++] = '\n';
                        अन्यथा यदि( d == 'r' )
                            buf[len++] = '\r';
                        अन्यथा यदि( d == 't' )
                            buf[len++] = '\t';
                        अन्यथा यदि( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            पूर्णाङ्क val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (पूर्णाङ्क)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            यदि( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            अन्यथा
                            {
                                buf[len++] = (वर्ण)val;
                                ptr = endptr;
                            }
                        }
                    }
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            यदि( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            गाठ.set_मूल्य(File_गाठ::STRING, buf, len);
        }
        अन्यथा यदि( c == '[' || c == '{' ) // collection as a flow
        {
            पूर्णाङ्क new_min_indent = min_indent + !is_parent_flow;
            पूर्णाङ्क struct_type = c == '{' ? File_गाठ::MAP : File_गाठ::SEQ;
            पूर्णाङ्क nelems = 0;

            fs->convertToCollection(struct_type, गाठ);
            d = c == '[' ? ']' : '}';

            प्रति( ++ptr ;; nelems++ )
            {
                File_गाठ elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                यदि (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                यदि( *ptr == '}' || *ptr == ']' )
                {
                    यदि( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    भङ्ग;
                }

                यदि( nelems != 0 )
                {
                    यदि( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                यदि( struct_type == File_गाठ::MAP )
                {
                    ptr = parse_किल्ली( ptr, गाठ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                अन्यथा
                {
                    यदि( *ptr == ']' )
                        भङ्ग;
                    elem = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE);
                }
                ptr = parse_मूल्य( ptr, elem, new_min_indent, सत्य, depth + 1 );
            }
            fs->finalizeCollection(गाठ);
        }
        अन्यथा
        {
            पूर्णाङ्क indent, struct_type;

            यदि( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                यदि( !is_parent_flow )
                {
                    यदि( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    यदि( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                कुरु c = *++endptr;
                यावत्( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_गाठ::STRING));

                यदि( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                यदि( is_parent_flow || c != ':' )
                {
                    वर्ण* str_end = endptr;
                    कुरु c = *--str_end;
                    यावत्( str_end > ptr && c == ' ' );
                    str_end++;
                    यदि ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        यदि (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            गाठ.set_मूल्य(File_गाठ::INT, &ival);
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                        अन्यथा यदि (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            गाठ.set_मूल्य(File_गाठ::INT, &ival);
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                        अन्यथा यदि (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                    }

                    गाठ.set_मूल्य(File_गाठ::STRING, ptr, (पूर्णाङ्क)(str_end - ptr));
                    ptr = endptr;
                    प्रत्यागम ptr;
                }
                struct_type = File_गाठ::MAP;
            }
            अन्यथा
                struct_type = File_गाठ::SEQ;

            fs->convertToCollection( struct_type, गाठ );
            indent = (पूर्णाङ्क)(ptr - fs->bufferStart());

            प्रति(;;)
            {
                File_गाठ elem;

                यदि( struct_type == File_गाठ::MAP )
                {
                    ptr = parse_किल्ली( ptr, गाठ, elem );
                }
                अन्यथा
                {
                    c = *ptr++;
                    यदि( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_मूल्य( ptr, elem, indent + 1, असत्य, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                यदि( ptr - fs->bufferStart() != indent )
                {
                    यदि( ptr - fs->bufferStart() < indent )
                        भङ्ग;
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                यदि( memcmp( ptr, "...", 3 ) == 0 )
                    भङ्ग;
            }
            fs->finalizeCollection(गाठ);
        }

        प्रत्यागम ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        तार्किक first = सत्य;
        तार्किक ok = सत्य;
        File_गाठ मूळ_collection(fs->getFS(), 0, 0);

        प्रति(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            प्रति(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                यदि( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    भङ्ग;
                }

                यदि( *ptr == '%' )
                {
                    यदि( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        यदि ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                अन्यथा यदि( *ptr == '-' )
                {
                    यदि( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        भङ्ग;
                    }
                    अन्यथा यदि( first )
                        भङ्ग;
                }
                अन्यथा यदि( cv_isalnum(*ptr) || *ptr=='_')
                {
                    यदि( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    भङ्ग;
                }
                अन्यथा यदि( fs->eof() )
                    भङ्ग;
                अन्यथा
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            यदि( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            यदि( !ptr || !ptr[0] )
                भङ्ग;
            यदि( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_गाठ मूळ_गाठ = fs->add_गाठ(मूळ_collection, std::string(), File_गाठ::NONE);

                ptr = parse_मूल्य( ptr, मूळ_गाठ, 0, असत्य );
                यदि( !मूळ_गाठ.isMap() && !मूळ_गाठ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                यदि( !ptr )
                    भङ्ग;
            }

            यदि( fs->eof() )
                भङ्ग;
            ptr += 3;
            first = असत्य;
        }

        प्रत्यागम ok;
    }

    FileStorage_API* fs;
    वर्ण buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    प्रत्यागम makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    प्रत्यागम makePtr<YAMLParser>(fs);
}

}
