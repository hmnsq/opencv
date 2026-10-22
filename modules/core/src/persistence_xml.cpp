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

    शून्य writeTag( अचर वर्ण* साँचो, पूर्णाङ्क tag_type, अचर std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        वर्ण* ptr = fs->bufferPtr();
        पूर्णाङ्क i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( साँचो && साँचो[0] == '\0' )
            साँचो = 0;

        यदि( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            यदि( File_गाँठो::isCollection(struct_flags) )
            {
                यदि( File_गाँठो::isMap(struct_flags) ^ (साँचो != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अन्यथा
            {
                struct_flags = File_गाँठो::EMPTY + (साँचो ? File_गाँठो::MAP : File_गाँठो::SEQ);
                //fs->is_first = 0;
            }

            यदि( !File_गाँठो::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        यदि( !साँचो )
            साँचो = "_";
        अन्यथा यदि( साँचो[0] == '_' && साँचो[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णाङ्क)म_माप( साँचो );
        *ptr++ = '<';
        यदि( tag_type == CV_XML_CLOSING_TAG )
        {
            यदि( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        यदि( !cv_isalpha(साँचो[0]) && साँचो[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        लागि( i = 0; i < len; i++ )
        {
            वर्ण c = साँचो[i];
            यदि( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        पूर्णाङ्क nattr = (पूर्णाङ्क)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        लागि( i = 0; i < nattr; i += 2 )
        {
            माप_प्रकार len0 = attrlist[i].size();
            माप_प्रकार len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(len0 + len1 + 4) );
            *ptr++ = ' ';

            स_नकल( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            यदि( len1 > 0 )
                स_नकल( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        यदि( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_गाँठो::EMPTY;
    }

    FStructData startWriteStruct(अचर FStructData& parent, अचर वर्ण* साँचो,
                                 पूर्णाङ्क struct_flags, अचर वर्ण* type_name=0)
    {
        std::vector<std::string> attrlist;
        यदि( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( साँचो, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = साँचो ? std::string(साँचो) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        फर्काऊ current_struct;
    }

    शून्य endWriteStruct(अचर FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    शून्य लिख(अचर वर्ण* साँचो, पूर्णाङ्क value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( साँचो, ptr);
    }

    शून्य लिख(अचर वर्ण* साँचो, int64_t value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10, सत्य );
        writeScalar( साँचो, ptr);
    }

    शून्य लिख( अचर वर्ण* साँचो, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( साँचो, fs::doubleToString( buf, परिमाण(buf), value, असत्य ) );
    }

    शून्य लिख(अचर वर्ण* साँचो, अचर वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* data = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)म_माप(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            तार्किक need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            लागि( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                यदि( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = सत्य;
                }
                अन्यथा यदि( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    यदि( c == '<' )
                    {
                        स_नकल(data, "lt", 2);
                        data += 2;
                    }
                    अन्यथा यदि( c == '>' )
                    {
                        स_नकल(data, "gt", 2);
                        data += 2;
                    }
                    अन्यथा यदि( c == '&' )
                    {
                        स_नकल(data, "amp", 3);
                        data += 3;
                    }
                    अन्यथा यदि( c == '\'' )
                    {
                        स_नकल(data, "apos", 4);
                        data += 4;
                    }
                    अन्यथा यदि( c == '\"' )
                    {
                        स_नकल( data, "quot", 4);
                        data += 4;
                    }
                    अन्यथा
                    {
                        snprintf( data, परिमाण(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                अन्यथा
                    *data++ = c;
            }
            यदि( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = सत्य;

            यदि( need_quote )
                *data++ = '\"';
            len = (पूर्णाङ्क)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (पूर्णाङ्क)!need_quote;
        }

        writeScalar( साँचो, data );
    }

    शून्य writeScalar(अचर वर्ण* साँचो, अचर वर्ण* data)
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

        पूर्णाङ्क len = (पूर्णाङ्क)म_माप(data);
        यदि( साँचो && *साँचो == '\0' )
            साँचो = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( File_गाँठो::isMap(struct_flags) ||
           (!File_गाँठो::isCollection(struct_flags) && साँचो) )
        {
            writeTag( साँचो, CV_XML_OPENING_TAG );
            वर्ण* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            स_नकल( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( साँचो, CV_XML_CLOSING_TAG );
        }
        अन्यथा
        {
            वर्ण* ptr = fs->bufferPtr();
            पूर्णाङ्क new_offset = (पूर्णाङ्क)(ptr - fs->bufferStart()) + len;

            यदि( साँचो )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_गाँठो::SEQ;

            यदि( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            अन्यथा यदि( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            स_नकल( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    शून्य writeComment(अचर वर्ण* comment, तार्किक eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क len;
        पूर्णाङ्क multiline;
        अचर वर्ण* eol;
        वर्ण* ptr;

        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        यदि( म_माला(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (पूर्णाङ्क)म_माप(comment);
        eol = म_अक्षर(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        यदि( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        अन्यथा यदि( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        यदि( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (पूर्णाङ्क)म_माप(ptr);
        }
        अन्यथा
        {
            म_नकल( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        यदि( multiline )
        {
            जबसम्म( comment )
            {
                यदि( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                    स_नकल( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = म_अक्षर( comment, '\n' );
                }
                अन्यथा
                {
                    len = (पूर्णाङ्क)म_माप(comment);
                    ptr = fs->resizeWriteBuffer( ptr, len );
                    स_नकल( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            म_नकल( ptr, "-->" );
            fs->setBufferPtr(ptr + 3);
            fs->flush();
        }
    }

    शून्य startNextStream()
    {
        fs->माला_दो( "\n<!-- next stream -->\n" );
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
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        पूर्णाङ्क level = 0;

        लागि(;;)
        {
            वर्ण c;
            ptr--;

            यदि( mode == CV_XML_INSIDE_COMMENT )
            {
                गर c = *++ptr;
                जबसम्म( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                यदि( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            अन्यथा यदि( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                गर
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    यदि( level < 0 )
                        फर्काऊ ptr;
                } जबसम्म( cv_isprint_or_tab(c) );
            }
            अन्यथा
            {
                गर c = *++ptr;
                जबसम्म( c == ' ' || c == '\t' );

                यदि( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    यदि( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अन्यथा यदि( cv_isprint(c) )
                    रोक;
            }

            यदि( !cv_isprint(*ptr) )
            {
                यदि( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->माला_लो();
                यदि( !ptr || *ptr == '\0' )
                    रोक;
            }
        }
        फर्काऊ ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        यदि( !ptr || !*ptr )
            फर्काऊ असत्य;

        // closing XML tag
        यदि ( *beg == '<' )
            फर्काऊ असत्य;

        // find end of the row
        जबसम्म( cv_isprint(*ptr) )
            ++ptr;
        यदि ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        फर्काऊ सत्य;
    }

    वर्ण* parse_मान( वर्ण* ptr, File_गाँठो& गाँठो, पूर्णाङ्क depth = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_गाँठो new_elem;
        तार्किक have_space = सत्य;
        पूर्णाङ्क value_type = गाँठो.type();
        std::string साँचो, key2, type_name;

        लागि(;;)
        {
            वर्ण c = *ptr, d;
            वर्ण* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            यदि( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                यदि (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = सत्य;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            यदि( c =='<' || c == '\0' )
            {
                पूर्णाङ्क tag_type = 0;
                पूर्णाङ्क elem_type = File_गाँठो::NONE;

                यदि( d == '/' || c == '\0' )
                    रोक;

                ptr = parseTag( ptr, साँचो, type_name, tag_type );

                यदि( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                यदि( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                तार्किक द्विआधारी_string = असत्य;

                यदि( !type_name.empty() )
                {
                    अचर वर्ण* tn = type_name.c_str();
                    यदि( म_भेद(tn, "str") == 0 )
                        elem_type = File_गाँठो::STRING;
                    अन्यथा यदि( म_भेद( tn, "map" ) == 0 )
                        elem_type = File_गाँठो::MAP;
                    अन्यथा यदि( म_भेद( tn, "seq" ) == 0 )
                        elem_type = File_गाँठो::SEQ;
                    अन्यथा यदि( म_भेद( tn, "binary") == 0)
                        द्विआधारी_string = सत्य;
                }

                new_elem = fs->add_गाँठो(गाँठो, साँचो, elem_type, 0);
                यदि (!द्विआधारी_string)
                    ptr = parse_मान(ptr, new_elem, depth + 1);
                अन्यथा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                यदि( tag_type != CV_XML_CLOSING_TAG || key2 != साँचो )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = सत्य;
            }
            अन्यथा
            {
                यदि( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_गाँठो* elem = &गाँठो;
                यदि( गाँठो.type() != File_गाँठो::NONE )
                {
                    fs->convertToCollection( File_गाँठो::SEQ, गाँठो );
                    new_elem = fs->add_गाँठो(गाँठो, std::string(), File_गाँठो::NONE, 0);
                    elem = &new_elem;
                }

                यदि( value_type != File_गाँठो::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    जबसम्म( cv_isdigit(*endptr) )
                        endptr++;
                    यदि( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुण fval = fs->म_से_भग्न( ptr, &endptr );
                        elem->set_मान(File_गाँठो::REAL, &fval);
                    }
                    अन्यथा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_मान(File_गाँठो::INT, &ival);
                    }

                    यदि( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                अन्यथा
                {
                    // string
                    पूर्णाङ्क i = 0, len, is_quoted = 0;
                    यदि( c == '\"' )
                        is_quoted = 1;
                    अन्यथा
                        --ptr;
                    strbuf[0] = '\0';

                    लागि( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        यदि( !cv_isalnum(c) )
                        {
                            यदि( c == '\"' )
                            {
                                यदि( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                रोक;
                            }
                            अन्यथा यदि( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                यदि( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                रोक;
                            }
                            अन्यथा यदि( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            अन्यथा यदि( c == '&' )
                            {
                                यदि( *++ptr == '#' )
                                {
                                    पूर्णाङ्क val, base = 10;
                                    ptr++;
                                    यदि( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (पूर्णाङ्क)म_से_दीर्घ( ptr, &endptr, base );
                                    यदि( (अचिन्हित)val > (अचिन्हित)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (वर्ण)val;
                                }
                                अन्यथा
                                {
                                    endptr = ptr;
                                    गर c = *++endptr;
                                    जबसम्म( cv_isalnum(c) );
                                    यदि( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (पूर्णाङ्क)(endptr - ptr);
                                    यदि( len == 2 && स_भेद( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    अन्यथा यदि( len == 2 && स_भेद( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    अन्यथा यदि( len == 3 && स_भेद( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    अन्यथा यदि( len == 4 && स_भेद( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    अन्यथा यदि( len == 4 && स_भेद( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    अन्यथा
                                    {
                                        यदि (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        स_नकल( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        यदि (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_मान(File_गाँठो::STRING, strbuf, i);
                }

                यदि( value_type != File_गाँठो::NONE && value_type != File_गाँठो::SEQ && value_type != File_गाँठो::MAP )
                    रोक;
                have_space = असत्य;
            }
        }
        fs->finalizeCollection(गाँठो);

        फर्काऊ ptr;
    }

    वर्ण* parseTag( वर्ण* ptr, std::string& tag_name,
                    std::string& type_name, पूर्णाङ्क& tag_type )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        यदि( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        यदि( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        यदि( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        अन्यथा यदि( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        अन्यथा यदि( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        अन्यथा यदि( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        अन्यथा
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        लागि(;;)
        {
            वर्ण c, *endptr;
            यदि( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            गर c = *++endptr;
            जबसम्म( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (माप_प्रकार)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            यदि( tag_name.empty() )
                tag_name = attrname;
            अन्यथा
            {
                यदि( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                यदि( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    यदि( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                यदि( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    यदि(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    यदि( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                वर्ण quote = *ptr++;
                endptr = ptr;
                लागि(;;)
                {
                    c = *endptr++;
                    यदि( c == quote )
                        रोक;
                    यदि( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                यदि( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (माप_प्रकार)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            तार्किक have_space = cv_isspace(c) || c == '\0';

            यदि( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                यदि (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            यदि( c == '>' )
            {
                यदि( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                रोक;
            }
            अन्यथा यदि( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                यदि( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                रोक;
            }
            अन्यथा यदि( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                रोक;
            }

            यदि( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        फर्काऊ ptr;
    }

    तार्किक parse(वर्ण* ptr)
    {
        CV_Assert( fs != 0 );

        std::string साँचो, key2, type_name;
        पूर्णाङ्क tag_type = 0;
        तार्किक ok = असत्य;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( स_भेद( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, साँचो, type_name, tag_type );
        File_गाँठो जरा_collection(fs->getFS(), 0, 0);

        जबसम्म( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            यदि( *ptr != '\0' )
            {
                ptr = parseTag( ptr, साँचो, type_name, tag_type );
                यदि( tag_type != CV_XML_OPENING_TAG || साँचो != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_गाँठो जरा = fs->add_गाँठो(जरा_collection, std::string(), File_गाँठो::MAP, 0);
                ptr = parse_मान( ptr, जरा );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                यदि( tag_type != CV_XML_CLOSING_TAG || साँचो != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = सत्य;
            }
        }
        CV_Assert( fs->eof() );
        फर्काऊ ok;
    }

    FileStorage_API* fs;
    वर्ण strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    फर्काऊ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    फर्काऊ makePtr<XMLParser>(fs);
}

}
