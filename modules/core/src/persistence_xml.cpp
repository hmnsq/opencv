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
    आभासी ~XMLEmitter() {}

    शून्य writeTag( स्थिरांक अक्षर* किल्ली, पूर्णांक tag_type, स्थिरांक std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        अक्षर* ptr = fs->bufferPtr();
        पूर्णांक i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;

        जर( किल्ली && किल्ली[0] == '\0' )
            किल्ली = 0;

        जर( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            जर( File_गाठ::isCollection(struct_flags) )
            {
                जर( File_गाठ::isMap(struct_flags) ^ (किल्ली != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अन्यथा
            {
                struct_flags = File_गाठ::EMPTY + (किल्ली ? File_गाठ::MAP : File_गाठ::SEQ);
                //fs->is_first = 0;
            }

            जर( !File_गाठ::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        जर( !किल्ली )
            किल्ली = "_";
        अन्यथा जर( किल्ली[0] == '_' && किल्ली[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णांक)म_माप( किल्ली );
        *ptr++ = '<';
        जर( tag_type == CV_XML_CLOSING_TAG )
        {
            जर( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        जर( !cv_isalpha(किल्ली[0]) && किल्ली[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        साठी( i = 0; i < len; i++ )
        {
            अक्षर c = किल्ली[i];
            जर( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        पूर्णांक nattr = (पूर्णांक)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        साठी( i = 0; i < nattr; i += 2 )
        {
            माप_प्रकार len0 = attrlist[i].size();
            माप_प्रकार len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (पूर्णांक)(len0 + len1 + 4) );
            *ptr++ = ' ';

            स_नकल( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            जर( len1 > 0 )
                स_नकल( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        जर( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_गाठ::EMPTY;
    }

    FStructData startWriteStruct(स्थिरांक FStructData& parent, स्थिरांक अक्षर* किल्ली,
                                 पूर्णांक struct_flags, स्थिरांक अक्षर* type_name=0)
    {
        std::vector<std::string> attrlist;
        जर( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( किल्ली, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = किल्ली ? std::string(किल्ली) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        परत current_struct;
    }

    शून्य endWriteStruct(स्थिरांक FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, पूर्णांक value)
    {
        अक्षर buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( किल्ली, ptr);
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, int64_t value)
    {
        अक्षर buf[128], *ptr = fs::itoa( value, buf, 10, सत्य );
        writeScalar( किल्ली, ptr);
    }

    शून्य लिख( स्थिरांक अक्षर* किल्ली, द्विगुण value )
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::doubleToString( buf, आकार(buf), value, असत्य ) );
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* str, तार्किक quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*6+16];
        अक्षर* data = (अक्षर*)str;
        पूर्णांक i, len;

        जर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        जर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        जर( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            तार्किक need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            साठी( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];

                जर( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = सत्य;
                }
                अन्यथा जर( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    जर( c == '<' )
                    {
                        स_नकल(data, "lt", 2);
                        data += 2;
                    }
                    अन्यथा जर( c == '>' )
                    {
                        स_नकल(data, "gt", 2);
                        data += 2;
                    }
                    अन्यथा जर( c == '&' )
                    {
                        स_नकल(data, "amp", 3);
                        data += 3;
                    }
                    अन्यथा जर( c == '\'' )
                    {
                        स_नकल(data, "apos", 4);
                        data += 4;
                    }
                    अन्यथा जर( c == '\"' )
                    {
                        स_नकल( data, "quot", 4);
                        data += 4;
                    }
                    अन्यथा
                    {
                        snprintf( data, आकार(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                अन्यथा
                    *data++ = c;
            }
            जर( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = सत्य;

            जर( need_quote )
                *data++ = '\"';
            len = (पूर्णांक)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (पूर्णांक)!need_quote;
        }

        writeScalar( किल्ली, data );
    }

    शून्य writeScalar(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* data)
    {
        fs->check_if_write_struct_is_delayed(असत्य);
        जर ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा जर ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णांक len = (पूर्णांक)म_माप(data);
        जर( किल्ली && *किल्ली == '\0' )
            किल्ली = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;

        जर( File_गाठ::isMap(struct_flags) ||
           (!File_गाठ::isCollection(struct_flags) && किल्ली) )
        {
            writeTag( किल्ली, CV_XML_OPENING_TAG );
            अक्षर* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            स_नकल( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( किल्ली, CV_XML_CLOSING_TAG );
        }
        अन्यथा
        {
            अक्षर* ptr = fs->bufferPtr();
            पूर्णांक new_offset = (पूर्णांक)(ptr - fs->bufferStart()) + len;

            जर( किल्ली )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_गाठ::SEQ;

            जर( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            अन्यथा जर( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            स_नकल( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    शून्य writeComment(स्थिरांक अक्षर* comment, तार्किक eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        पूर्णांक len;
        पूर्णांक multiline;
        स्थिरांक अक्षर* eol;
        अक्षर* ptr;

        जर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        जर( म_माला(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (पूर्णांक)म_माप(comment);
        eol = म_अक्षर(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        जर( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        अन्यथा जर( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        जर( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (पूर्णांक)म_माप(ptr);
        }
        अन्यथा
        {
            म_नकल( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        जर( multiline )
        {
            पर्यंत( comment )
            {
                जर( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (पूर्णांक)(eol - comment) + 1 );
                    स_नकल( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = म_अक्षर( comment, '\n' );
                }
                अन्यथा
                {
                    len = (पूर्णांक)म_माप(comment);
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

संरक्षित:
    FileStorage_API* fs;
};

वर्ग XMLParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभासी ~XMLParser() {}

    अक्षर* skipSpaces( अक्षर* ptr, पूर्णांक mode )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        पूर्णांक level = 0;

        साठी(;;)
        {
            अक्षर c;
            ptr--;

            जर( mode == CV_XML_INSIDE_COMMENT )
            {
                करा c = *++ptr;
                पर्यंत( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                जर( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            अन्यथा जर( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                करा
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    जर( level < 0 )
                        परत ptr;
                } पर्यंत( cv_isprint_or_tab(c) );
            }
            अन्यथा
            {
                करा c = *++ptr;
                पर्यंत( c == ' ' || c == '\t' );

                जर( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    जर( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अन्यथा जर( cv_isprint(c) )
                    थांबा;
            }

            जर( !cv_isprint(*ptr) )
            {
                जर( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->माला_लो();
                जर( !ptr || *ptr == '\0' )
                    थांबा;
            }
        }
        परत ptr;
    }

    तार्किक getBase64Row(अक्षर* ptr, पूर्णांक /*indent*/, अक्षर* &beg, अक्षर* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        जर( !ptr || !*ptr )
            परत असत्य;

        // closing XML tag
        जर ( *beg == '<' )
            परत असत्य;

        // find end of the row
        पर्यंत( cv_isprint(*ptr) )
            ++ptr;
        जर ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        परत सत्य;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाठ& गाठ, पूर्णांक depth = 0 )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_गाठ new_elem;
        तार्किक have_space = सत्य;
        पूर्णांक value_type = गाठ.type();
        std::string किल्ली, key2, type_name;

        साठी(;;)
        {
            अक्षर c = *ptr, d;
            अक्षर* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            जर( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                जर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = सत्य;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            जर( c =='<' || c == '\0' )
            {
                पूर्णांक tag_type = 0;
                पूर्णांक elem_type = File_गाठ::NONE;

                जर( d == '/' || c == '\0' )
                    थांबा;

                ptr = parseTag( ptr, किल्ली, type_name, tag_type );

                जर( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                जर( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                तार्किक द्विमान_string = असत्य;

                जर( !type_name.empty() )
                {
                    स्थिरांक अक्षर* tn = type_name.c_str();
                    जर( म_भेद(tn, "str") == 0 )
                        elem_type = File_गाठ::STRING;
                    अन्यथा जर( म_भेद( tn, "map" ) == 0 )
                        elem_type = File_गाठ::MAP;
                    अन्यथा जर( म_भेद( tn, "seq" ) == 0 )
                        elem_type = File_गाठ::SEQ;
                    अन्यथा जर( म_भेद( tn, "binary") == 0)
                        द्विमान_string = सत्य;
                }

                new_elem = fs->add_गाठ(गाठ, किल्ली, elem_type, 0);
                जर (!द्विमान_string)
                    ptr = parse_मूल्य(ptr, new_elem, depth + 1);
                अन्यथा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    जर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                जर( tag_type != CV_XML_CLOSING_TAG || key2 != किल्ली )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = सत्य;
            }
            अन्यथा
            {
                जर( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_गाठ* elem = &गाठ;
                जर( गाठ.type() != File_गाठ::NONE )
                {
                    fs->convertToCollection( File_गाठ::SEQ, गाठ );
                    new_elem = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE, 0);
                    elem = &new_elem;
                }

                जर( value_type != File_गाठ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    पर्यंत( cv_isdigit(*endptr) )
                        endptr++;
                    जर( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुण fval = fs->म_से_भग्न( ptr, &endptr );
                        elem->set_मूल्य(File_गाठ::REAL, &fval);
                    }
                    अन्यथा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_मूल्य(File_गाठ::INT, &ival);
                    }

                    जर( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                अन्यथा
                {
                    // string
                    पूर्णांक i = 0, len, is_quoted = 0;
                    जर( c == '\"' )
                        is_quoted = 1;
                    अन्यथा
                        --ptr;
                    strbuf[0] = '\0';

                    साठी( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        जर( !cv_isalnum(c) )
                        {
                            जर( c == '\"' )
                            {
                                जर( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                थांबा;
                            }
                            अन्यथा जर( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                जर( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                थांबा;
                            }
                            अन्यथा जर( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            अन्यथा जर( c == '&' )
                            {
                                जर( *++ptr == '#' )
                                {
                                    पूर्णांक val, base = 10;
                                    ptr++;
                                    जर( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (पूर्णांक)म_से_दीर्घ( ptr, &endptr, base );
                                    जर( (अचिन्हित)val > (अचिन्हित)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (अक्षर)val;
                                }
                                अन्यथा
                                {
                                    endptr = ptr;
                                    करा c = *++endptr;
                                    पर्यंत( cv_isalnum(c) );
                                    जर( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (पूर्णांक)(endptr - ptr);
                                    जर( len == 2 && स_भेद( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    अन्यथा जर( len == 2 && स_भेद( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    अन्यथा जर( len == 3 && स_भेद( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    अन्यथा जर( len == 4 && स_भेद( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    अन्यथा जर( len == 4 && स_भेद( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    अन्यथा
                                    {
                                        जर (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        स_नकल( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        जर (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_मूल्य(File_गाठ::STRING, strbuf, i);
                }

                जर( value_type != File_गाठ::NONE && value_type != File_गाठ::SEQ && value_type != File_गाठ::MAP )
                    थांबा;
                have_space = असत्य;
            }
        }
        fs->finalizeCollection(गाठ);

        परत ptr;
    }

    अक्षर* parseTag( अक्षर* ptr, std::string& tag_name,
                    std::string& type_name, पूर्णांक& tag_type )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        जर( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        जर( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        जर( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        अन्यथा जर( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        अन्यथा जर( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        अन्यथा जर( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        अन्यथा
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        साठी(;;)
        {
            अक्षर c, *endptr;
            जर( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            करा c = *++endptr;
            पर्यंत( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (माप_प्रकार)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            जर( tag_name.empty() )
                tag_name = attrname;
            अन्यथा
            {
                जर( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                जर( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    जर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    जर( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                जर( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    जर(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    जर( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                अक्षर quote = *ptr++;
                endptr = ptr;
                साठी(;;)
                {
                    c = *endptr++;
                    जर( c == quote )
                        थांबा;
                    जर( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                जर( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (माप_प्रकार)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            तार्किक have_space = cv_isspace(c) || c == '\0';

            जर( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                जर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            जर( c == '>' )
            {
                जर( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                थांबा;
            }
            अन्यथा जर( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                जर( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                थांबा;
            }
            अन्यथा जर( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                थांबा;
            }

            जर( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        परत ptr;
    }

    तार्किक parse(अक्षर* ptr)
    {
        CV_Assert( fs != 0 );

        std::string किल्ली, key2, type_name;
        पूर्णांक tag_type = 0;
        तार्किक ok = असत्य;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जर( स_भेद( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, किल्ली, type_name, tag_type );
        File_गाठ मूळ_collection(fs->getFS(), 0, 0);

        पर्यंत( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            जर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            जर( *ptr != '\0' )
            {
                ptr = parseTag( ptr, किल्ली, type_name, tag_type );
                जर( tag_type != CV_XML_OPENING_TAG || किल्ली != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_गाठ मूळ = fs->add_गाठ(मूळ_collection, std::string(), File_गाठ::MAP, 0);
                ptr = parse_मूल्य( ptr, मूळ );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                जर( tag_type != CV_XML_CLOSING_TAG || किल्ली != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = सत्य;
            }
        }
        CV_Assert( fs->eof() );
        परत ok;
    }

    FileStorage_API* fs;
    अक्षर strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    परत makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    परत makePtr<XMLParser>(fs);
}

}
