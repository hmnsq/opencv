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
    आभासी ~YAMLEmitter() {}

    FStructData startWriteStruct(स्थिरांक FStructData& parent, स्थिरांक अक्षर* किल्ली,
                                 पूर्णांक struct_flags, स्थिरांक अक्षर* type_name=0)
    {
        अक्षर buf[CV_FS_MAX_LEN + 1024];
        स्थिरांक अक्षर* data = 0;

        जर ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_गाठ::TYPE_MASK|File_गाठ::FLOW)) | File_गाठ::EMPTY;
        जर( !File_गाठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        जर (type_name && स_भेद(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_गाठ::SEQ;
            snprintf(buf, आकार(buf), "!!binary |");
            data = buf;
        }
        अन्यथा जर( File_गाठ::isFlow(struct_flags))
        {
            अक्षर c = File_गाठ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_गाठ::FLOW;

            जर( type_name )
                snprintf( buf, आकार(buf), "!!%s %c", type_name, c );
            अन्यथा
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        अन्यथा जर( type_name )
        {
            snprintf( buf, आकार(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( किल्ली, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        जर( !File_गाठ::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_गाठ::isFlow(struct_flags);

        परत fsd;
    }

    शून्य endWriteStruct(स्थिरांक FStructData& current_struct)
    {
        अक्षर* ptr;

        पूर्णांक struct_flags = current_struct.flags;

        जर( File_गाठ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            जर( ptr > fs->bufferStart() + current_struct.indent && !File_गाठ::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_गाठ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        अन्यथा जर( File_गाठ::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            स_नकल( ptr, File_गाठ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, पूर्णांक value)
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10 ));
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, int64_t value)
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::itoa( value, buf, 10, सत्य ));
    }

    शून्य लिख( स्थिरांक अक्षर* किल्ली, द्विगुण value )
    {
        अक्षर buf[128];
        writeScalar( किल्ली, fs::doubleToString( buf, आकार(buf), value, असत्य ));
    }

    शून्य लिख(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* str, तार्किक quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*4+16];
        अक्षर* data = (अक्षर*)str;
        पूर्णांक i, len;

        जर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        जर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        जर( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णांक need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            साठी( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];

                जर( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                जर( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    जर( cv_isprint(c) )
                        *data++ = c;
                    अन्यथा जर( c == '\n' )
                        *data++ = 'n';
                    अन्यथा जर( c == '\r' )
                        *data++ = 'r';
                    अन्यथा जर( c == '\t' )
                        *data++ = 't';
                    अन्यथा
                    {
                        snprintf( data, आकार(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                अन्यथा
                    *data++ = c;
            }
            जर( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            जर( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( किल्ली, data);
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

        पूर्णांक i, keylen = 0;
        पूर्णांक datalen = 0;
        अक्षर* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        पूर्णांक struct_flags = current_struct.flags;

        जर( किल्ली && किल्ली[0] == '\0' )
            किल्ली = 0;

        जर( File_गाठ::isCollection(struct_flags) )
        {
            जर( (File_गाठ::isMap(struct_flags) ^ (किल्ली != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        अन्यथा
        {
            fs->setNonEmpty();
            struct_flags = File_गाठ::EMPTY | (किल्ली ? File_गाठ::MAP : File_गाठ::SEQ);
        }

        जर( किल्ली )
        {
            keylen = (पूर्णांक)म_माप(किल्ली);
            जर( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            जर( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        जर( data )
            datalen = (पूर्णांक)म_माप(data);

        जर( File_गाठ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            जर( !File_गाठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            पूर्णांक new_offset = (पूर्णांक)(ptr - fs->bufferStart()) + keylen + datalen;
            जर( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
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
            जर( !File_गाठ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                जर( data )
                    *ptr++ = ' ';
            }
        }

        जर( किल्ली )
        {
            जर( !cv_isalpha(किल्ली[0]) && किल्ली[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            साठी( i = 0; i < keylen; i++ )
            {
                अक्षर c = किल्ली[i];

                ptr[i] = c;
                जर( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            जर( !File_गाठ::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        जर( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            स_नकल( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_गाठ::EMPTY;
    }

    शून्य writeComment(स्थिरांक अक्षर* comment, तार्किक eol_comment)
    {
        जर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णांक len = (पूर्णांक)म_माप(comment);
        स्थिरांक अक्षर* eol = म_अक्षर(comment, '\n');
        तार्किक multiline = eol != 0;
        अक्षर* ptr = fs->bufferPtr();

        जर( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        अन्यथा
            *ptr++ = ' ';

        पर्यंत( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            जर( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (पूर्णांक)(eol - comment) + 1 );
                स_नकल( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = म_अक्षर( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णांक)म_माप(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                स_नकल( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    शून्य startNextStream()
    {
        fs->माला_दो( "...\n" );
        fs->माला_दो( "---\n" );
    }

संरक्षित:
    FileStorage_API* fs;
};


वर्ग YAMLParser : सार्वजनिक FileStorageParser
{
सार्वजनिक:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    आभासी ~YAMLParser() {}

    अक्षर* skipSpaces( अक्षर* ptr, पूर्णांक min_indent, पूर्णांक max_comment_indent )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        साठी(;;)
        {
            पर्यंत( *ptr == ' ' )
                ptr++;
            जर( *ptr == '#' )
            {
                जर( ptr - fs->bufferStart() > max_comment_indent )
                    परत ptr;
                *ptr = '\0';
            }
            अन्यथा जर( cv_isprint(*ptr) )
            {
                जर( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                थांबा;
            }
            अन्यथा जर( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->माला_लो();
                जर( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    थांबा;
                }
                अन्यथा
                {
                    पूर्णांक l = (पूर्णांक)म_माप(ptr);
                    जर( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            अन्यथा
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        परत ptr;
    }

    तार्किक getBase64Row(अक्षर* ptr, पूर्णांक indent, अक्षर* &beg, अक्षर* &end)
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, पूर्णांक_उच्च);
        जर (!ptr || !*ptr)
            परत असत्य; // end of file

        जर (ptr - fs->bufferStart() != indent)
            परत असत्य; // end of base64 data

        /* find end */
        पर्यंत(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        जर (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        परत सत्य;
    }


    अक्षर* parse_किल्ली( अक्षर* ptr, File_गाठ& map_गाठ, File_गाठ& मूल्य_placeholder )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अक्षर c;
        अक्षर *endptr = ptr - 1, *saveptr;

        जर( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        करा c = *++endptr;
        पर्यंत( cv_isprint(c) && c != ':' );

        जर( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        जर( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        करा c = *--endptr;
        पर्यंत( c == ' ' );

        ++endptr;

        मूल्य_placeholder = fs->add_गाठ(map_गाठ, std::string(ptr, endptr - ptr), File_गाठ::NONE);
        ptr = saveptr;

        परत ptr;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाठ& गाठ, पूर्णांक min_indent, तार्किक is_parent_flow, पूर्णांक depth = 0 )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        जर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        अक्षर* endptr = 0;
        अक्षर c = ptr[0], d = ptr[1];
        पूर्णांक value_type = File_गाठ::NONE;
        पूर्णांक len;
        तार्किक is_द्विमान_string = असत्य;
        तार्किक is_user = असत्य;

        जर( c == '!' ) // handle explicit type specification
        {
            जर( d == '!' || d == '^' )
            {
                ptr++;
                is_user = सत्य;
                //value_type |= FileNode::USER;
            }
            जर ( d == '<') //support of full type heading from YAML 1.2
            {
                स्थिरांक अक्षर* yamlTypeHeading = "<tag:yaml.org,2002:";
                स्थिरांक माप_प्रकार headingLength = म_माप(yamlTypeHeading);

                अक्षर* typeEndPtr = ++ptr;

                करा d = *++typeEndPtr;
                पर्यंत( cv_isprint(d) && d != ' ' && d != '>' );

                जर ( d == '>' && (माप_प्रकार)(typeEndPtr - ptr) > headingLength )
                {
                    जर ( स_भेद(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = सत्य;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            करा d = *++endptr;
            पर्यंत( cv_isprint(d) && d != ' ' );
            len = (पूर्णांक)(endptr - ptr);
            जर( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            जर( len == 3 && !is_user )
            {
                जर( स_भेद( ptr, "str", 3 ) == 0 )
                    value_type = File_गाठ::STRING;
                अन्यथा जर( स_भेद( ptr, "int", 3 ) == 0 )
                    value_type = File_गाठ::INT;
                अन्यथा जर( स_भेद( ptr, "seq", 3 ) == 0 )
                    value_type = File_गाठ::SEQ;
                अन्यथा जर( स_भेद( ptr, "map", 3 ) == 0 )
                    value_type = File_गाठ::MAP;
            }
            अन्यथा जर( len == 5 && !is_user )
            {
                जर( स_भेद( ptr, "float", 5 ) == 0 )
                    value_type = File_गाठ::REAL;
            }
            अन्यथा जर (len == 6 && is_user)
            {
                जर( स_भेद( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_गाठ::SEQ;
                    is_द्विमान_string = सत्य;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    करा {
                        d = *++endptr;
                        जर (d == '|')
                            थांबा;
                    } पर्यंत (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, पूर्णांक_उच्च );
            जर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            जर( !is_user )
            {
                जर (value_type == File_गाठ::STRING && c != '\'' && c != '\"')
                    जा force_string;
                जर( value_type == File_गाठ::INT )
                    जा force_int;
                जर( value_type == File_गाठ::REAL )
                    जा force_real;
            }
        }

        जर (is_द्विमान_string)
        {
            पूर्णांक indent = static_cast<पूर्णांक>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, गाठ);
        }
        अन्यथा जर( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            पर्यंत( cv_isdigit(*endptr) )
                endptr++;
            जर( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                द्विगुण fval = fs->म_से_भग्न( ptr, &endptr );
                गाठ.set_मूल्य(File_गाठ::REAL, &fval);
            }
            अन्यथा
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                गाठ.set_मूल्य(File_गाठ::INT, &ival);
            }

            जर( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        अन्यथा जर( c == '\'' || c == '\"' ) // an explicit string
        {
            जर( c == '\'' )
                साठी( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    जर( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा जर( c == '\'' )
                    {
                        c = *++ptr;
                        जर( c != '\'' )
                            थांबा;
                        buf[len++] = c;
                    }
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            अन्यथा
                साठी( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    जर( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा जर( c == '\"' )
                    {
                        ++ptr;
                        थांबा;
                    }
                    अन्यथा जर( c == '\\' )
                    {
                        d = *++ptr;
                        जर( d == '\'' )
                            buf[len++] = d;
                        अन्यथा जर( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        अन्यथा जर( d == 'n' )
                            buf[len++] = '\n';
                        अन्यथा जर( d == 'r' )
                            buf[len++] = '\r';
                        अन्यथा जर( d == 't' )
                            buf[len++] = '\t';
                        अन्यथा जर( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            पूर्णांक val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (पूर्णांक)म_से_दीर्घ( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            जर( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            अन्यथा
                            {
                                buf[len++] = (अक्षर)val;
                                ptr = endptr;
                            }
                        }
                    }
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            जर( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            गाठ.set_मूल्य(File_गाठ::STRING, buf, len);
        }
        अन्यथा जर( c == '[' || c == '{' ) // collection as a flow
        {
            पूर्णांक new_min_indent = min_indent + !is_parent_flow;
            पूर्णांक struct_type = c == '{' ? File_गाठ::MAP : File_गाठ::SEQ;
            पूर्णांक nelems = 0;

            fs->convertToCollection(struct_type, गाठ);
            d = c == '[' ? ']' : '}';

            साठी( ++ptr ;; nelems++ )
            {
                File_गाठ elem;

                ptr = skipSpaces( ptr, new_min_indent, पूर्णांक_उच्च );
                जर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                जर( *ptr == '}' || *ptr == ']' )
                {
                    जर( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    थांबा;
                }

                जर( nelems != 0 )
                {
                    जर( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, पूर्णांक_उच्च );
                    जर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                जर( struct_type == File_गाठ::MAP )
                {
                    ptr = parse_किल्ली( ptr, गाठ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, पूर्णांक_उच्च );
                }
                अन्यथा
                {
                    जर( *ptr == ']' )
                        थांबा;
                    elem = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE);
                }
                ptr = parse_मूल्य( ptr, elem, new_min_indent, सत्य, depth + 1 );
            }
            fs->finalizeCollection(गाठ);
        }
        अन्यथा
        {
            पूर्णांक indent, struct_type;

            जर( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                जर( !is_parent_flow )
                {
                    जर( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    जर( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                करा c = *++endptr;
                पर्यंत( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_गाठ::STRING));

                जर( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                जर( is_parent_flow || c != ':' )
                {
                    अक्षर* str_end = endptr;
                    करा c = *--str_end;
                    पर्यंत( str_end > ptr && c == ' ' );
                    str_end++;
                    जर ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        माप_प्रकार llen = str_end - ptr;
                        जर (llen == 4 && स_भेद(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            गाठ.set_मूल्य(File_गाठ::INT, &ival);
                            ptr = endptr;
                            परत ptr;
                        }
                        अन्यथा जर (llen == 5 && स_भेद(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            गाठ.set_मूल्य(File_गाठ::INT, &ival);
                            ptr = endptr;
                            परत ptr;
                        }
                        अन्यथा जर (llen == 4 && स_भेद(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            परत ptr;
                        }
                    }

                    गाठ.set_मूल्य(File_गाठ::STRING, ptr, (पूर्णांक)(str_end - ptr));
                    ptr = endptr;
                    परत ptr;
                }
                struct_type = File_गाठ::MAP;
            }
            अन्यथा
                struct_type = File_गाठ::SEQ;

            fs->convertToCollection( struct_type, गाठ );
            indent = (पूर्णांक)(ptr - fs->bufferStart());

            साठी(;;)
            {
                File_गाठ elem;

                जर( struct_type == File_गाठ::MAP )
                {
                    ptr = parse_किल्ली( ptr, गाठ, elem );
                }
                अन्यथा
                {
                    c = *ptr++;
                    जर( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_गाठ(गाठ, std::string(), File_गाठ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, पूर्णांक_उच्च );
                ptr = parse_मूल्य( ptr, elem, indent + 1, असत्य, depth + 1 );
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                जर( ptr - fs->bufferStart() != indent )
                {
                    जर( ptr - fs->bufferStart() < indent )
                        थांबा;
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                जर( स_भेद( ptr, "...", 3 ) == 0 )
                    थांबा;
            }
            fs->finalizeCollection(गाठ);
        }

        परत ptr;
    }

    तार्किक parse( अक्षर* ptr )
    {
        जर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        तार्किक first = सत्य;
        तार्किक ok = सत्य;
        File_गाठ मूळ_collection(fs->getFS(), 0, 0);

        साठी(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            साठी(;;)
            {
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                जर( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    थांबा;
                }

                जर( *ptr == '%' )
                {
                    जर( स_भेद( ptr, "%YAML", 5 ) == 0 &&
                        स_भेद( ptr, "%YAML:1.", 8 ) != 0 &&
                        स_भेद( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        जर ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                अन्यथा जर( *ptr == '-' )
                {
                    जर( स_भेद(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        थांबा;
                    }
                    अन्यथा जर( first )
                        थांबा;
                }
                अन्यथा जर( cv_isalnum(*ptr) || *ptr=='_')
                {
                    जर( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    थांबा;
                }
                अन्यथा जर( fs->eof() )
                    थांबा;
                अन्यथा
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            जर( ptr )
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
            जर( !ptr || !ptr[0] )
                थांबा;
            जर( स_भेद( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_गाठ मूळ_गाठ = fs->add_गाठ(मूळ_collection, std::string(), File_गाठ::NONE);

                ptr = parse_मूल्य( ptr, मूळ_गाठ, 0, असत्य );
                जर( !मूळ_गाठ.isMap() && !मूळ_गाठ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                जर( !ptr )
                    थांबा;
            }

            जर( fs->eof() )
                थांबा;
            ptr += 3;
            first = असत्य;
        }

        परत ok;
    }

    FileStorage_API* fs;
    अक्षर buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    परत makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    परत makePtr<YAMLParser>(fs);
}

}
