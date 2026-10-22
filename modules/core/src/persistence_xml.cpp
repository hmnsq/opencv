// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

गणना
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_TAG = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_TAG = 1,
    CV_XML_CLOSING_TAG = 2,
    CV_XML_EMPTY_TAG = 3,
    CV_XML_HEADER_TAG = 4,
    CV_XML_DIRECTIVE_TAG = 5
};

नामाकाश cv
{

वर्ग XMLEmitter : सार्वजनिक FileStorageEmitter
{
सार्वजनिक:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    आभास ~XMLEmitter() {}

    शून्य writeTag( ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क tag_type, ध्रुव std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        वर्ण* ptr = fs->bufferPtr();
        पूर्णाङ्क i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        जइ( कुञ्चिका && कुञ्चिका[0] == '\0' )
            कुञ्चिका = 0;

        जइ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            जइ( File_ग्रन्थिः::isCollection(struct_flags) )
            {
                जइ( File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अण्णहा
            {
                struct_flags = File_ग्रन्थिः::EMPTY + (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
                //fs->is_first = 0;
            }

            जइ( !File_ग्रन्थिः::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        जइ( !कुञ्चिका )
            कुञ्चिका = "_";
        अण्णहा जइ( कुञ्चिका[0] == '_' && कुञ्चिका[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णाङ्क)strlen( कुञ्चिका );
        *ptr++ = '<';
        जइ( tag_type == CV_XML_CLOSING_TAG )
        {
            जइ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        जइ( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        प्रति( i = 0; i < len; i++ )
        {
            वर्ण c = कुञ्चिका[i];
            जइ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        पूर्णाङ्क nattr = (पूर्णाङ्क)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        प्रति( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            जइ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        जइ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_ग्रन्थिः::EMPTY;
    }

    FStructData startWriteStruct(ध्रुव FStructData& parent, ध्रुव वर्ण* कुञ्चिका,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        std::vector<std::string> attrlist;
        जइ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( कुञ्चिका, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = कुञ्चिका ? std::string(कुञ्चिका) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        प्रत्यागम current_struct;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( कुञ्चिका, ptr);
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, int64_t value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10, सच्च );
        writeScalar( कुञ्चिका, ptr);
    }

    शून्य लिख( ध्रुव वर्ण* कुञ्चिका, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::doubleToString( buf, परिमाण(buf), value, मिच्छ ) );
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        जइ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)strlen(str);
        जइ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        जइ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            तार्किक need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                जइ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = सच्च;
                }
                अण्णहा जइ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    जइ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    अण्णहा जइ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    अण्णहा जइ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    अण्णहा जइ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    अण्णहा जइ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    अण्णहा
                    {
                        snprintf( data, परिमाण(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                अण्णहा
                    *data++ = c;
            }
            जइ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = सच्च;

            जइ( need_quote )
                *data++ = '\"';
            len = (पूर्णाङ्क)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (पूर्णाङ्क)!need_quote;
        }

        writeScalar( कुञ्चिका, data );
    }

    शून्य writeScalar(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* data)
    {
        fs->check_if_write_struct_is_delayed(मिच्छ);
        जइ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अण्णहा जइ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णाङ्क len = (पूर्णाङ्क)strlen(data);
        जइ( कुञ्चिका && *कुञ्चिका == '\0' )
            कुञ्चिका = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        जइ( File_ग्रन्थिः::isMap(struct_flags) ||
           (!File_ग्रन्थिः::isCollection(struct_flags) && कुञ्चिका) )
        {
            writeTag( कुञ्चिका, CV_XML_OPENING_TAG );
            वर्ण* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( कुञ्चिका, CV_XML_CLOSING_TAG );
        }
        अण्णहा
        {
            वर्ण* ptr = fs->bufferPtr();
            पूर्णाङ्क new_offset = (पूर्णाङ्क)(ptr - fs->bufferStart()) + len;

            जइ( कुञ्चिका )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ग्रन्थिः::SEQ;

            जइ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            अण्णहा जइ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क len;
        पूर्णाङ्क multiline;
        ध्रुव वर्ण* eol;
        वर्ण* ptr;

        जइ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        जइ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (पूर्णाङ्क)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        जइ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        अण्णहा जइ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        जइ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (पूर्णाङ्क)strlen(ptr);
        }
        अण्णहा
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        जइ( multiline )
        {
            जाव( comment )
            {
                जइ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                अण्णहा
                {
                    len = (पूर्णाङ्क)strlen(comment);
                    ptr = fs->resizeWriteBuffer( ptr, len );
                    memcpy( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->setBufferPtr(ptr + 3);
            fs->flush();
        }
    }

    शून्य startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

रक्षित:
    FileStorage_API* fs;
};

वर्ग XMLParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभास ~XMLParser() {}

    वर्ण* skipSpaces( वर्ण* ptr, पूर्णाङ्क mode )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        पूर्णाङ्क level = 0;

        प्रति(;;)
        {
            वर्ण c;
            ptr--;

            जइ( mode == CV_XML_INSIDE_COMMENT )
            {
                करेहि c = *++ptr;
                जाव( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                जइ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            अण्णहा जइ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                करेहि
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    जइ( level < 0 )
                        प्रत्यागम ptr;
                } जाव( cv_isprint_or_tab(c) );
            }
            अण्णहा
            {
                करेहि c = *++ptr;
                जाव( c == ' ' || c == '\t' );

                जइ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    जइ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अण्णहा जइ( cv_isprint(c) )
                    भङ्ग;
            }

            जइ( !cv_isprint(*ptr) )
            {
                जइ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                जइ( !ptr || *ptr == '\0' )
                    भङ्ग;
            }
        }
        प्रत्यागम ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        जइ( !ptr || !*ptr )
            प्रत्यागम मिच्छ;

        // closing XML tag
        जइ ( *beg == '<' )
            प्रत्यागम मिच्छ;

        // find end of the row
        जाव( cv_isprint(*ptr) )
            ++ptr;
        जइ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        प्रत्यागम सच्च;
    }

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क depth = 0 )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जइ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ग्रन्थिः new_elem;
        तार्किक have_space = सच्च;
        पूर्णाङ्क value_type = ग्रन्थिः.type();
        std::string कुञ्चिका, key2, type_name;

        प्रति(;;)
        {
            वर्ण c = *ptr, d;
            वर्ण* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            जइ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                जइ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = सच्च;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            जइ( c =='<' || c == '\0' )
            {
                पूर्णाङ्क tag_type = 0;
                पूर्णाङ्क elem_type = File_ग्रन्थिः::NONE;

                जइ( d == '/' || c == '\0' )
                    भङ्ग;

                ptr = parseTag( ptr, कुञ्चिका, type_name, tag_type );

                जइ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                जइ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                तार्किक द्विमान_string = मिच्छ;

                जइ( !type_name.empty() )
                {
                    ध्रुव वर्ण* tn = type_name.c_str();
                    जइ( strcmp(tn, "str") == 0 )
                        elem_type = File_ग्रन्थिः::STRING;
                    अण्णहा जइ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ग्रन्थिः::MAP;
                    अण्णहा जइ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ग्रन्थिः::SEQ;
                    अण्णहा जइ( strcmp( tn, "binary") == 0)
                        द्विमान_string = सच्च;
                }

                new_elem = fs->add_ग्रन्थिः(ग्रन्थिः, कुञ्चिका, elem_type, 0);
                जइ (!द्विमान_string)
                    ptr = parse_मानम्(ptr, new_elem, depth + 1);
                अण्णहा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    जइ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                जइ( tag_type != CV_XML_CLOSING_TAG || key2 != कुञ्चिका )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = सच्च;
            }
            अण्णहा
            {
                जइ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ग्रन्थिः* elem = &ग्रन्थिः;
                जइ( ग्रन्थिः.type() != File_ग्रन्थिः::NONE )
                {
                    fs->convertToCollection( File_ग्रन्थिः::SEQ, ग्रन्थिः );
                    new_elem = fs->add_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE, 0);
                    elem = &new_elem;
                }

                जइ( value_type != File_ग्रन्थिः::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    जाव( cv_isdigit(*endptr) )
                        endptr++;
                    जइ( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुण fval = fs->strtod( ptr, &endptr );
                        elem->set_मानम्(File_ग्रन्थिः::REAL, &fval);
                    }
                    अण्णहा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_मानम्(File_ग्रन्थिः::INT, &ival);
                    }

                    जइ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                अण्णहा
                {
                    // string
                    पूर्णाङ्क i = 0, len, is_quoted = 0;
                    जइ( c == '\"' )
                        is_quoted = 1;
                    अण्णहा
                        --ptr;
                    strbuf[0] = '\0';

                    प्रति( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        जइ( !cv_isalnum(c) )
                        {
                            जइ( c == '\"' )
                            {
                                जइ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                भङ्ग;
                            }
                            अण्णहा जइ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                जइ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                भङ्ग;
                            }
                            अण्णहा जइ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            अण्णहा जइ( c == '&' )
                            {
                                जइ( *++ptr == '#' )
                                {
                                    पूर्णाङ्क val, base = 10;
                                    ptr++;
                                    जइ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (पूर्णाङ्क)strtol( ptr, &endptr, base );
                                    जइ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (वर्ण)val;
                                }
                                अण्णहा
                                {
                                    endptr = ptr;
                                    करेहि c = *++endptr;
                                    जाव( cv_isalnum(c) );
                                    जइ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (पूर्णाङ्क)(endptr - ptr);
                                    जइ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    अण्णहा जइ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    अण्णहा जइ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    अण्णहा जइ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    अण्णहा जइ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    अण्णहा
                                    {
                                        जइ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        जइ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_मानम्(File_ग्रन्थिः::STRING, strbuf, i);
                }

                जइ( value_type != File_ग्रन्थिः::NONE && value_type != File_ग्रन्थिः::SEQ && value_type != File_ग्रन्थिः::MAP )
                    भङ्ग;
                have_space = मिच्छ;
            }
        }
        fs->finalizeCollection(ग्रन्थिः);

        प्रत्यागम ptr;
    }

    वर्ण* parseTag( वर्ण* ptr, std::string& tag_name,
                    std::string& type_name, पूर्णाङ्क& tag_type )
    {
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        जइ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        जइ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        जइ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        अण्णहा जइ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        अण्णहा जइ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        अण्णहा जइ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        अण्णहा
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        प्रति(;;)
        {
            वर्ण c, *endptr;
            जइ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            करेहि c = *++endptr;
            जाव( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            जइ( tag_name.empty() )
                tag_name = attrname;
            अण्णहा
            {
                जइ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                जइ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    जइ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    जइ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                जइ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    जइ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    जइ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                वर्ण quote = *ptr++;
                endptr = ptr;
                प्रति(;;)
                {
                    c = *endptr++;
                    जइ( c == quote )
                        भङ्ग;
                    जइ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                जइ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            तार्किक have_space = cv_isspace(c) || c == '\0';

            जइ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                जइ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            जइ( c == '>' )
            {
                जइ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                भङ्ग;
            }
            अण्णहा जइ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                जइ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                भङ्ग;
            }
            अण्णहा जइ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                भङ्ग;
            }

            जइ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        प्रत्यागम ptr;
    }

    तार्किक parse(वर्ण* ptr)
    {
        CV_Assert( fs != 0 );

        std::string कुञ्चिका, key2, type_name;
        पूर्णाङ्क tag_type = 0;
        तार्किक ok = मिच्छ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        जइ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जइ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, कुञ्चिका, type_name, tag_type );
        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        जाव( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            जइ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            जइ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, कुञ्चिका, type_name, tag_type );
                जइ( tag_type != CV_XML_OPENING_TAG || कुञ्चिका != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ग्रन्थिः मूलम् = fs->add_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::MAP, 0);
                ptr = parse_मानम्( ptr, मूलम् );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                जइ( tag_type != CV_XML_CLOSING_TAG || कुञ्चिका != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = सच्च;
            }
        }
        CV_Assert( fs->eof() );
        प्रत्यागम ok;
    }

    FileStorage_API* fs;
    वर्ण strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    प्रत्यागम makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    प्रत्यागम makePtr<XMLParser>(fs);
}

}
