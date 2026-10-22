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

    FStruct_दत्त startWriteStruct(ध्रुव FStruct_दत्त& parent, ध्रुव वर्ण* कुञ्चिका,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        वर्ण buf[CV_FS_MAX_LEN + 1024];
        ध्रुव वर्ण* दत्त = 0;

        यदि ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_आवरणम्|File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::रिक्तानि_2;
        यदि( !File_ग्रन्थिः::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        यदि (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ग्रन्थिः::SEQ;
            snprintf(buf, परिमाण(buf), "!!binary |");
            दत्त = buf;
        }
        अन्यथा यदि( File_ग्रन्थिः::isFlow(struct_flags))
        {
            वर्ण c = File_ग्रन्थिः::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ग्रन्थिः::FLOW;

            यदि( type_name )
                snprintf( buf, परिमाण(buf), "!!%s %c", type_name, c );
            अन्यथा
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            दत्त = buf;
        }
        अन्यथा यदि( type_name )
        {
            snprintf( buf, परिमाण(buf), "!!%s", type_name );
            दत्त = buf;
        }

        writeScalar( कुञ्चिका, दत्त );

        FStruct_दत्त fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        यदि( !File_ग्रन्थिः::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_ग्रन्थिः::isFlow(struct_flags);

        प्रत्यागम fsd;
    }

    शून्य अन्तः_Write_Struct(ध्रुव FStruct_दत्त& current_struct)
    {
        वर्ण* ptr;

        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            ptr = fs->बफर_Ptr();
            यदि( ptr > fs->बफर_Start() + current_struct.indent && !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
            fs->समुच्चयः_बफर_Ptr(ptr);
        }
        अन्यथा यदि( File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
        {
            ptr = fs->flush();
            स्मृतिं_प्रतिलिख( ptr, File_ग्रन्थिः::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->समुच्चयः_बफर_Ptr(ptr + 2);
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
        writeScalar( कुञ्चिका, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( ध्रुव वर्ण* कुञ्चिका, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::double_गन्तव्यम्_String( buf, परिमाण(buf), value, असत्य ));
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*4+16];
        वर्ण* दत्त = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णाङ्क need_quote = quote || len == 0 || str[0] == ' ';
            दत्त = buf;
            *दत्त++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                यदि( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                यदि( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *दत्त++ = '\\';
                    यदि( cv_isprint(c) )
                        *दत्त++ = c;
                    अन्यथा यदि( c == '\n' )
                        *दत्त++ = 'n';
                    अन्यथा यदि( c == '\r' )
                        *दत्त++ = 'r';
                    अन्यथा यदि( c == '\t' )
                        *दत्त++ = 't';
                    अन्यथा
                    {
                        snprintf( दत्त, परिमाण(buf) - (दत्त - buf), "x%02x", c );
                        दत्त += 3;
                    }
                }
                अन्यथा
                    *दत्त++ = c;
            }
            यदि( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            यदि( need_quote )
                *दत्त++ = '\"';
            *दत्त++ = '\0';
            दत्त = buf + !need_quote;
        }

        writeScalar( कुञ्चिका, दत्त);
    }

    शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* दत्त)
    {
        fs->check_if_write_struct_is_delayed(असत्य);
        यदि ( fs->get_state_सम्बन्धि_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_गन्तव्यम्_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा यदि ( fs->get_state_सम्बन्धि_writing_base64() == FileStorage_API::अन्तर्_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णाङ्क i, keylen = 0;
        पूर्णाङ्क datalen = 0;
        वर्ण* ptr;

        FStruct_दत्त& current_struct = fs->getCurrentStruct();

        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( कुञ्चिका && कुञ्चिका[0] == '\0' )
            कुञ्चिका = 0;

        यदि( File_ग्रन्थिः::isCollection(struct_flags) )
        {
            यदि( (File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        अन्यथा
        {
            fs->समुच्चयः_Non_रिक्तानि();
            struct_flags = File_ग्रन्थिः::रिक्तानि_2 | (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
        }

        यदि( कुञ्चिका )
        {
            keylen = (पूर्णाङ्क)वाक्यदैर्घ्यम्(कुञ्चिका);
            यदि( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            यदि( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        यदि( दत्त )
            datalen = (पूर्णाङ्क)वाक्यदैर्घ्यम्(दत्त);

        यदि( File_ग्रन्थिः::isFlow(struct_flags) )
        {
            ptr = fs->बफर_Ptr();
            यदि( !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
                *ptr++ = ',';
            पूर्णाङ्क new_विस्थापनम् = (पूर्णाङ्क)(ptr - fs->बफर_Start()) + keylen + datalen;
            यदि( new_विस्थापनम् > fs->wrapMargin() && new_विस्थापनम् - current_struct.indent > 10 )
            {
                fs->समुच्चयः_बफर_Ptr(ptr);
                ptr = fs->flush();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            ptr = fs->flush();
            यदि( !File_ग्रन्थिः::isMap(struct_flags) )
            {
                *ptr++ = '-';
                यदि( दत्त )
                    *ptr++ = ' ';
            }
        }

        यदि( कुञ्चिका )
        {
            यदि( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_बफर( ptr, keylen );

            प्रति( i = 0; i < keylen; i++ )
            {
                वर्ण c = कुञ्चिका[i];

                ptr[i] = c;
                यदि( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            यदि( !File_ग्रन्थिः::isFlow(struct_flags) && दत्त )
                *ptr++ = ' ';
        }

        यदि( दत्त )
        {
            ptr = fs->resize_Write_बफर( ptr, datalen );
            स्मृतिं_प्रतिलिख( ptr, दत्त, datalen );
            ptr += datalen;
        }

        fs->समुच्चयः_बफर_Ptr(ptr);
        current_struct.flags &= ~File_ग्रन्थिः::रिक्तानि_2;
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णाङ्क len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(comment);
        ध्रुव वर्ण* eol = strchr(comment, '\n');
        तार्किक multiline = eol != 0;
        वर्ण* ptr = fs->बफर_Ptr();

        यदि( !eol_comment || multiline ||
            fs->बफर_अन्तः_2() - ptr < len || ptr == fs->बफर_Start() )
            ptr = fs->flush();
        अन्यथा
            *ptr++ = ' ';

        यावत्( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            यदि( eol )
            {
                ptr = fs->resize_Write_बफर( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                स्मृतिं_प्रतिलिख( ptr, comment, eol - comment + 1 );
                fs->समुच्चयः_बफर_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(comment);
                ptr = fs->resize_Write_बफर( ptr, len );
                स्मृतिं_प्रतिलिख( ptr, comment, len );
                fs->समुच्चयः_बफर_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    शून्य start_अग्र_प्रवाहः()
    {
        fs->पङ्क्तिं_लिख( "...\n" );
        fs->पङ्क्तिं_लिख( "---\n" );
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
                यदि( ptr - fs->बफर_Start() > max_comment_indent )
                    प्रत्यागम ptr;
                *ptr = '\0';
            }
            अन्यथा यदि( cv_isprint(*ptr) )
            {
                यदि( ptr - fs->बफर_Start() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                भङ्ग;
            }
            अन्यथा यदि( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                यदि( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->बफर_Start();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->समुच्चयः_Eof();
                    भङ्ग;
                }
                अन्यथा
                {
                    पूर्णाङ्क l = (पूर्णाङ्क)वाक्यदैर्घ्यम्(ptr);
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

        beg = end = ptr = skipSpaces(ptr, 0, अधिकतम_पूर्णाङ्कः);
        यदि (!ptr || !*ptr)
            प्रत्यागम असत्य; // end of file

        यदि (ptr - fs->बफर_Start() != indent)
            प्रत्यागम असत्य; // end of base64 data

        /* find end */
        यावत्(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        यदि (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        प्रत्यागम सत्य;
    }


    वर्ण* parse_कुञ्चिका( वर्ण* ptr, File_ग्रन्थिः& map_ग्रन्थिः, File_ग्रन्थिः& मानम्_placeholder )
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

        मानम्_placeholder = fs->सङ्कलय_ग्रन्थिः(map_ग्रन्थिः, std::string(ptr, endptr - ptr), File_ग्रन्थिः::NONE);
        ptr = saveptr;

        प्रत्यागम ptr;
    }

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क min_indent, तार्किक is_parent_flow, पूर्णाङ्क गभीरता = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( गभीरता > CV_PERSISTENCE_MAX_गभीरता )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        वर्ण* endptr = 0;
        वर्ण c = ptr[0], d = ptr[1];
        पूर्णाङ्क value_type = File_ग्रन्थिः::NONE;
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
                ध्रुव size_t heading_दीर्घता = वाक्यदैर्घ्यम्(yamlTypeHeading);

                वर्ण* type_अन्तः_Ptr = ++ptr;

                कुरु d = *++type_अन्तः_Ptr;
                यावत्( cv_isprint(d) && d != ' ' && d != '>' );

                यदि ( d == '>' && (size_t)(type_अन्तः_Ptr - ptr) > heading_दीर्घता )
                {
                    यदि ( memcmp(ptr, yamlTypeHeading, heading_दीर्घता) == 0 )
                    {
                        *type_अन्तः_Ptr = ' ';
                        ptr += heading_दीर्घता - 1;
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
                    value_type = File_ग्रन्थिः::STRING;
                अन्यथा यदि( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::INT;
                अन्यथा यदि( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::SEQ;
                अन्यथा यदि( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ग्रन्थिः::MAP;
            }
            अन्यथा यदि( len == 5 && !is_user )
            {
                यदि( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ग्रन्थिः::REAL;
            }
            अन्यथा यदि (len == 6 && is_user)
            {
                यदि( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ग्रन्थिः::SEQ;
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
            ptr = skipSpaces( endptr, min_indent, अधिकतम_पूर्णाङ्कः );
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            यदि( !is_user )
            {
                यदि (value_type == File_ग्रन्थिः::STRING && c != '\'' && c != '\"')
                    गच्छ force_string;
                यदि( value_type == File_ग्रन्थिः::INT )
                    गच्छ force_int;
                यदि( value_type == File_ग्रन्थिः::REAL )
                    गच्छ force_real;
            }
        }

        यदि (is_द्विमान_string)
        {
            पूर्णाङ्क indent = static_cast<पूर्णाङ्क>(ptr - fs->बफर_Start());
            ptr = fs->parseBase64(ptr, indent, ग्रन्थिः);
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
                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::REAL, &fval);
            }
            अन्यथा
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
            }

            यदि( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
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

            ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::STRING, buf, len);
        }
        अन्यथा यदि( c == '[' || c == '{' ) // collection as a flow
        {
            पूर्णाङ्क new_min_indent = min_indent + !is_parent_flow;
            पूर्णाङ्क struct_type = c == '{' ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ;
            पूर्णाङ्क nelems = 0;

            fs->convert_गन्तव्यम्_Collection(struct_type, ग्रन्थिः);
            d = c == '[' ? ']' : '}';

            प्रति( ++ptr ;; nelems++ )
            {
                File_ग्रन्थिः elem;

                ptr = skipSpaces( ptr, new_min_indent, अधिकतम_पूर्णाङ्कः );
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
                    ptr = skipSpaces( ptr + 1, new_min_indent, अधिकतम_पूर्णाङ्कः );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                यदि( struct_type == File_ग्रन्थिः::MAP )
                {
                    ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, elem );
                    ptr = skipSpaces( ptr, new_min_indent, अधिकतम_पूर्णाङ्कः );
                }
                अन्यथा
                {
                    यदि( *ptr == ']' )
                        भङ्ग;
                    elem = fs->सङ्कलय_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE);
                }
                ptr = parse_मानम्( ptr, elem, new_min_indent, सत्य, गभीरता + 1 );
            }
            fs->finalizeCollection(ग्रन्थिः);
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
                      (is_parent_flow || c != ':' || value_type == File_ग्रन्थिः::STRING));

                यदि( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                यदि( is_parent_flow || c != ':' )
                {
                    वर्ण* str_अन्तः = endptr;
                    कुरु c = *--str_अन्तः;
                    यावत्( str_अन्तः > ptr && c == ' ' );
                    str_अन्तः++;
                    यदि ((fs->getFormat() & FileStorage::FORMAT_आवरणम्) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_अन्तः - ptr;
                        यदि (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                        अन्यथा यदि (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                        अन्यथा यदि (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            प्रत्यागम ptr;
                        }
                    }

                    ग्रन्थिः.समुच्चयः_मानम्(File_ग्रन्थिः::STRING, ptr, (पूर्णाङ्क)(str_अन्तः - ptr));
                    ptr = endptr;
                    प्रत्यागम ptr;
                }
                struct_type = File_ग्रन्थिः::MAP;
            }
            अन्यथा
                struct_type = File_ग्रन्थिः::SEQ;

            fs->convert_गन्तव्यम्_Collection( struct_type, ग्रन्थिः );
            indent = (पूर्णाङ्क)(ptr - fs->बफर_Start());

            प्रति(;;)
            {
                File_ग्रन्थिः elem;

                यदि( struct_type == File_ग्रन्थिः::MAP )
                {
                    ptr = parse_कुञ्चिका( ptr, ग्रन्थिः, elem );
                }
                अन्यथा
                {
                    c = *ptr++;
                    यदि( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->सङ्कलय_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, अधिकतम_पूर्णाङ्कः );
                ptr = parse_मानम्( ptr, elem, indent + 1, असत्य, गभीरता + 1 );
                ptr = skipSpaces( ptr, 0, अधिकतम_पूर्णाङ्कः );
                यदि( ptr - fs->बफर_Start() != indent )
                {
                    यदि( ptr - fs->बफर_Start() < indent )
                        भङ्ग;
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                यदि( memcmp( ptr, "...", 3 ) == 0 )
                    भङ्ग;
            }
            fs->finalizeCollection(ग्रन्थिः);
        }

        प्रत्यागम ptr;
    }

    तार्किक parse( वर्ण* ptr )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        तार्किक first = सत्य;
        तार्किक ok = सत्य;
        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        प्रति(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            प्रति(;;)
            {
                ptr = skipSpaces( ptr, 0, अधिकतम_पूर्णाङ्कः );
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
                        यदि ((fs->getFormat() & FileStorage::FORMAT_आवरणम्) == FileStorage::FORMAT_YAML_1_0)
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
                ptr = skipSpaces( ptr, 0, अधिकतम_पूर्णाङ्कः );
            यदि( !ptr || !ptr[0] )
                भङ्ग;
            यदि( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ग्रन्थिः मूलम्_ग्रन्थिः = fs->सङ्कलय_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::NONE);

                ptr = parse_मानम्( ptr, मूलम्_ग्रन्थिः, 0, असत्य );
                यदि( !मूलम्_ग्रन्थिः.isMap() && !मूलम्_ग्रन्थिः.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, अधिकतम_पूर्णाङ्कः );
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
