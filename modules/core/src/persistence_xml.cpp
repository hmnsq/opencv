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

    शून्य writeTag( ध्रुव वर्ण* कुंजी, पूर्णाङ्क tag_type, ध्रुव std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        वर्ण* ptr = fs->bufferPtr();
        पूर्णाङ्क i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( कुंजी && कुंजी[0] == '\0' )
            कुंजी = 0;

        यदि( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            यदि( File_गाँठ::isCollection(struct_flags) )
            {
                यदि( File_गाँठ::isMap(struct_flags) ^ (कुंजी != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अन्यथा
            {
                struct_flags = File_गाँठ::EMPTY + (कुंजी ? File_गाँठ::MAP : File_गाँठ::SEQ);
                //fs->is_first = 0;
            }

            यदि( !File_गाँठ::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        यदि( !कुंजी )
            कुंजी = "_";
        अन्यथा यदि( कुंजी[0] == '_' && कुंजी[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णाङ्क)म_माप( कुंजी );
        *ptr++ = '<';
        यदि( tag_type == CV_XML_CLOSING_TAG )
        {
            यदि( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        यदि( !cv_isalpha(कुंजी[0]) && कुंजी[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        प्रति( i = 0; i < len; i++ )
        {
            वर्ण c = कुंजी[i];
            यदि( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        पूर्णाङ्क nattr = (पूर्णाङ्क)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        प्रति( i = 0; i < nattr; i += 2 )
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
        current_struct.flags = struct_flags & ~File_गाँठ::EMPTY;
    }

    FStructData startWriteStruct(ध्रुव FStructData& parent, ध्रुव वर्ण* कुंजी,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        std::vector<std::string> attrlist;
        यदि( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( कुंजी, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = कुंजी ? std::string(कुंजी) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        प्रत्यागम current_struct;
    }

    शून्य endWriteStruct(ध्रुव FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, पूर्णाङ्क value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( कुंजी, ptr);
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, int64_t value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10, सत्य );
        writeScalar( कुंजी, ptr);
    }

    शून्य लिख( ध्रुव वर्ण* कुंजी, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुंजी, fs::doubleToString( buf, परिमाण(buf), value, असत्य ) );
    }

    शून्य लिख(ध्रुव वर्ण* कुंजी, ध्रुव वर्ण* str, तार्किक quote)
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
            प्रति( i = 0; i < len; i++ )
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

        writeScalar( कुंजी, data );
    }

    शून्य writeScalar(ध्रुव वर्ण* कुंजी, ध्रुव वर्ण* data)
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
        यदि( कुंजी && *कुंजी == '\0' )
            कुंजी = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( File_गाँठ::isMap(struct_flags) ||
           (!File_गाँठ::isCollection(struct_flags) && कुंजी) )
        {
            writeTag( कुंजी, CV_XML_OPENING_TAG );
            वर्ण* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            स_नकल( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( कुंजी, CV_XML_CLOSING_TAG );
        }
        अन्यथा
        {
            वर्ण* ptr = fs->bufferPtr();
            पूर्णाङ्क new_offset = (पूर्णाङ्क)(ptr - fs->bufferStart()) + len;

            यदि( कुंजी )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_गाँठ::SEQ;

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

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क len;
        पूर्णाङ्क multiline;
        ध्रुव वर्ण* eol;
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
            यावत्( comment )
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

        प्रति(;;)
        {
            वर्ण c;
            ptr--;

            यदि( mode == CV_XML_INSIDE_COMMENT )
            {
                कुरु c = *++ptr;
                यावत्( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

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
                कुरु
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    यदि( level < 0 )
                        प्रत्यागम ptr;
                } यावत्( cv_isprint_or_tab(c) );
            }
            अन्यथा
            {
                कुरु c = *++ptr;
                यावत्( c == ' ' || c == '\t' );

                यदि( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    यदि( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अन्यथा यदि( cv_isprint(c) )
                    भङ्ग;
            }

            यदि( !cv_isprint(*ptr) )
            {
                यदि( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->माला_लो();
                यदि( !ptr || *ptr == '\0' )
                    भङ्ग;
            }
        }
        प्रत्यागम ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        यदि( !ptr || !*ptr )
            प्रत्यागम असत्य;

        // closing XML tag
        यदि ( *beg == '<' )
            प्रत्यागम असत्य;

        // find end of the row
        यावत्( cv_isprint(*ptr) )
            ++ptr;
        यदि ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        प्रत्यागम सत्य;
    }

    वर्ण* parse_मूल्य( वर्ण* ptr, File_गाँठ& गाँठ, पूर्णाङ्क depth = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_गाँठ new_elem;
        तार्किक have_space = सत्य;
        पूर्णाङ्क value_type = गाँठ.type();
        std::string कुंजी, key2, type_name;

        प्रति(;;)
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
                पूर्णाङ्क elem_type = File_गाँठ::NONE;

                यदि( d == '/' || c == '\0' )
                    भङ्ग;

                ptr = parseTag( ptr, कुंजी, type_name, tag_type );

                यदि( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                यदि( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                तार्किक द्विआधारी_string = असत्य;

                यदि( !type_name.empty() )
                {
                    ध्रुव वर्ण* tn = type_name.c_str();
                    यदि( म_भेद(tn, "str") == 0 )
                        elem_type = File_गाँठ::STRING;
                    अन्यथा यदि( म_भेद( tn, "map" ) == 0 )
                        elem_type = File_गाँठ::MAP;
                    अन्यथा यदि( म_भेद( tn, "seq" ) == 0 )
                        elem_type = File_गाँठ::SEQ;
                    अन्यथा यदि( म_भेद( tn, "binary") == 0)
                        द्विआधारी_string = सत्य;
                }

                new_elem = fs->add_गाँठ(गाँठ, कुंजी, elem_type, 0);
                यदि (!द्विआधारी_string)
                    ptr = parse_मूल्य(ptr, new_elem, depth + 1);
                अन्यथा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                यदि( tag_type != CV_XML_CLOSING_TAG || key2 != कुंजी )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = सत्य;
            }
            अन्यथा
            {
                यदि( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_गाँठ* elem = &गाँठ;
                यदि( गाँठ.type() != File_गाँठ::NONE )
                {
                    fs->convertToCollection( File_गाँठ::SEQ, गाँठ );
                    new_elem = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE, 0);
                    elem = &new_elem;
                }

                यदि( value_type != File_गाँठ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    यावत्( cv_isdigit(*endptr) )
                        endptr++;
                    यदि( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुण fval = fs->म_से_भग्न( ptr, &endptr );
                        elem->set_मूल्य(File_गाँठ::REAL, &fval);
                    }
                    अन्यथा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_मूल्य(File_गाँठ::INT, &ival);
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

                    प्रति( ;; )
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
                                भङ्ग;
                            }
                            अन्यथा यदि( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                यदि( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                भङ्ग;
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
                                    कुरु c = *++endptr;
                                    यावत्( cv_isalnum(c) );
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
                    elem->set_मूल्य(File_गाँठ::STRING, strbuf, i);
                }

                यदि( value_type != File_गाँठ::NONE && value_type != File_गाँठ::SEQ && value_type != File_गाँठ::MAP )
                    भङ्ग;
                have_space = असत्य;
            }
        }
        fs->finalizeCollection(गाँठ);

        प्रत्यागम ptr;
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

        प्रति(;;)
        {
            वर्ण c, *endptr;
            यदि( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            कुरु c = *++endptr;
            यावत्( cv_isalnum(c) || c == '_' || c == '-' );

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
                प्रति(;;)
                {
                    c = *endptr++;
                    यदि( c == quote )
                        भङ्ग;
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
                भङ्ग;
            }
            अन्यथा यदि( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                यदि( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                भङ्ग;
            }
            अन्यथा यदि( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                भङ्ग;
            }

            यदि( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        प्रत्यागम ptr;
    }

    तार्किक parse(वर्ण* ptr)
    {
        CV_Assert( fs != 0 );

        std::string कुंजी, key2, type_name;
        पूर्णाङ्क tag_type = 0;
        तार्किक ok = असत्य;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( स_भेद( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, कुंजी, type_name, tag_type );
        File_गाँठ मूल_collection(fs->getFS(), 0, 0);

        यावत्( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            यदि( *ptr != '\0' )
            {
                ptr = parseTag( ptr, कुंजी, type_name, tag_type );
                यदि( tag_type != CV_XML_OPENING_TAG || कुंजी != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_गाँठ मूल = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::MAP, 0);
                ptr = parse_मूल्य( ptr, मूल );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                यदि( tag_type != CV_XML_CLOSING_TAG || कुंजी != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = सत्य;
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
