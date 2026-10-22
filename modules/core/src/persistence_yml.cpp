// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "persistence.hpp"

क्रमागत
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

namespace cv
{

श्रेणी YAMLEmitter : खुला FileStorageEmitter
{
खुला:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    भव ~YAMLEmitter() {}

    FStruct_तथ्य startWriteStruct(स्थिर FStruct_तथ्य& parent, स्थिर अक्षर* कुंजी,
                                 पूर्णांक struct_flags, स्थिर अक्षर* type_name=0)
    {
        अक्षर buf[CV_FS_MAX_LEN + 1024];
        स्थिर अक्षर* तथ्य = 0;

        अगर ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_गाँठ::TYPE_MASK|File_गाँठ::FLOW)) | File_गाँठ::EMPTY;
        अगर( !File_गाँठ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        अगर (type_name && स_भेद(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_गाँठ::SEQ;
            snprintf(buf, माप(buf), "!!binary |");
            तथ्य = buf;
        }
        अन्यथा अगर( File_गाँठ::isFlow(struct_flags))
        {
            अक्षर c = File_गाँठ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_गाँठ::FLOW;

            अगर( type_name )
                snprintf( buf, माप(buf), "!!%s %c", type_name, c );
            अन्यथा
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            तथ्य = buf;
        }
        अन्यथा अगर( type_name )
        {
            snprintf( buf, माप(buf), "!!%s", type_name );
            तथ्य = buf;
        }

        writeScalar( कुंजी, तथ्य );

        FStruct_तथ्य fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        अगर( !File_गाँठ::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_गाँठ::isFlow(struct_flags);

        वापस fsd;
    }

    व्योम endWriteStruct(स्थिर FStruct_तथ्य& current_struct)
    {
        अक्षर* ptr;

        पूर्णांक struct_flags = current_struct.flags;

        अगर( File_गाँठ::isFlow(struct_flags) )
        {
            ptr = fs->बफर_Ptr();
            अगर( ptr > fs->बफर_Start() + current_struct.indent && !File_गाँठ::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_गाँठ::isMap(struct_flags) ? '}' : ']';
            fs->set_बफर_Ptr(ptr);
        }
        अन्यथा अगर( File_गाँठ::isEmptyCollection(struct_flags) )
        {
            ptr = fs->सफाई();
            स_नकल( ptr, File_गाँठ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->set_बफर_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, पूर्णांक value)
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10 ));
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, int64_t value)
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::itoa( value, buf, 10, true ));
    }

    व्योम ग_लिखो( स्थिर अक्षर* कुंजी, द्विगुन value )
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::doubleToString( buf, माप(buf), value, false ));
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, स्थिर अक्षर* str, bool quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*4+16];
        अक्षर* तथ्य = (अक्षर*)str;
        पूर्णांक i, len;

        अगर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        अगर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        अगर( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            पूर्णांक need_quote = quote || len == 0 || str[0] == ' ';
            तथ्य = buf;
            *तथ्य++ = '\"';
            क्रम( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];

                अगर( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                अगर( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *तथ्य++ = '\\';
                    अगर( cv_isprint(c) )
                        *तथ्य++ = c;
                    अन्यथा अगर( c == '\n' )
                        *तथ्य++ = 'n';
                    अन्यथा अगर( c == '\r' )
                        *तथ्य++ = 'r';
                    अन्यथा अगर( c == '\t' )
                        *तथ्य++ = 't';
                    अन्यथा
                    {
                        snprintf( तथ्य, माप(buf) - (तथ्य - buf), "x%02x", c );
                        तथ्य += 3;
                    }
                }
                अन्यथा
                    *तथ्य++ = c;
            }
            अगर( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            अगर( need_quote )
                *तथ्य++ = '\"';
            *तथ्य++ = '\0';
            तथ्य = buf + !need_quote;
        }

        writeScalar( कुंजी, तथ्य);
    }

    व्योम writeScalar(स्थिर अक्षर* कुंजी, स्थिर अक्षर* तथ्य)
    {
        fs->check_if_write_struct_is_delayed(false);
        अगर ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        अन्यथा अगर ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        पूर्णांक i, keylen = 0;
        पूर्णांक datalen = 0;
        अक्षर* ptr;

        FStruct_तथ्य& current_struct = fs->getCurrentStruct();

        पूर्णांक struct_flags = current_struct.flags;

        अगर( कुंजी && कुंजी[0] == '\0' )
            कुंजी = 0;

        अगर( File_गाँठ::isCollection(struct_flags) )
        {
            अगर( (File_गाँठ::isMap(struct_flags) ^ (कुंजी != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        अन्यथा
        {
            fs->setNonEmpty();
            struct_flags = File_गाँठ::EMPTY | (कुंजी ? File_गाँठ::MAP : File_गाँठ::SEQ);
        }

        अगर( कुंजी )
        {
            keylen = (पूर्णांक)म_माप(कुंजी);
            अगर( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            अगर( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        अगर( तथ्य )
            datalen = (पूर्णांक)म_माप(तथ्य);

        अगर( File_गाँठ::isFlow(struct_flags) )
        {
            ptr = fs->बफर_Ptr();
            अगर( !File_गाँठ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            पूर्णांक new_offset = (पूर्णांक)(ptr - fs->बफर_Start()) + keylen + datalen;
            अगर( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->set_बफर_Ptr(ptr);
                ptr = fs->सफाई();
            }
            अन्यथा
                *ptr++ = ' ';
        }
        अन्यथा
        {
            ptr = fs->सफाई();
            अगर( !File_गाँठ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                अगर( तथ्य )
                    *ptr++ = ' ';
            }
        }

        अगर( कुंजी )
        {
            अगर( !cv_isalpha(कुंजी[0]) && कुंजी[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_बफर( ptr, keylen );

            क्रम( i = 0; i < keylen; i++ )
            {
                अक्षर c = कुंजी[i];

                ptr[i] = c;
                अगर( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            अगर( !File_गाँठ::isFlow(struct_flags) && तथ्य )
                *ptr++ = ' ';
        }

        अगर( तथ्य )
        {
            ptr = fs->resize_Write_बफर( ptr, datalen );
            स_नकल( ptr, तथ्य, datalen );
            ptr += datalen;
        }

        fs->set_बफर_Ptr(ptr);
        current_struct.flags &= ~File_गाँठ::EMPTY;
    }

    व्योम writeComment(स्थिर अक्षर* comment, bool eol_comment)
    {
        अगर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        पूर्णांक len = (पूर्णांक)म_माप(comment);
        स्थिर अक्षर* eol = म_अक्षर(comment, '\n');
        bool multiline = eol != 0;
        अक्षर* ptr = fs->बफर_Ptr();

        अगर( !eol_comment || multiline ||
            fs->बफर_End() - ptr < len || ptr == fs->बफर_Start() )
            ptr = fs->सफाई();
        अन्यथा
            *ptr++ = ' ';

        जबतक( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            अगर( eol )
            {
                ptr = fs->resize_Write_बफर( ptr, (पूर्णांक)(eol - comment) + 1 );
                स_नकल( ptr, comment, eol - comment + 1 );
                fs->set_बफर_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = म_अक्षर( comment, '\n' );
            }
            अन्यथा
            {
                len = (पूर्णांक)म_माप(comment);
                ptr = fs->resize_Write_बफर( ptr, len );
                स_नकल( ptr, comment, len );
                fs->set_बफर_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->सफाई();
        }
    }

    व्योम start_अग्र_Stream()
    {
        fs->माला_दो( "...\n" );
        fs->माला_दो( "---\n" );
    }

रक्षित:
    FileStorage_API* fs;
};


श्रेणी YAMLParser : खुला FileStorageParser
{
खुला:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    भव ~YAMLParser() {}

    अक्षर* skipSpaces( अक्षर* ptr, पूर्णांक min_indent, पूर्णांक max_comment_indent )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        क्रम(;;)
        {
            जबतक( *ptr == ' ' )
                ptr++;
            अगर( *ptr == '#' )
            {
                अगर( ptr - fs->बफर_Start() > max_comment_indent )
                    वापस ptr;
                *ptr = '\0';
            }
            अन्यथा अगर( cv_isprint(*ptr) )
            {
                अगर( ptr - fs->बफर_Start() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                अवरोध;
            }
            अन्यथा अगर( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->माला_लो();
                अगर( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->बफर_Start();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    अवरोध;
                }
                अन्यथा
                {
                    पूर्णांक l = (पूर्णांक)म_माप(ptr);
                    अगर( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->खप() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            अन्यथा
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        वापस ptr;
    }

    bool getBase64Row(अक्षर* ptr, पूर्णांक indent, अक्षर* &beg, अक्षर* &end)
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, पूर्णांक_उच्च);
        अगर (!ptr || !*ptr)
            वापस false; // end of file

        अगर (ptr - fs->बफर_Start() != indent)
            वापस false; // end of base64 data

        /* find end */
        जबतक(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        अगर (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        वापस true;
    }


    अक्षर* parse_कुंजी( अक्षर* ptr, File_गाँठ& map_गाँठ, File_गाँठ& मूल्य_placeholder )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अक्षर c;
        अक्षर *endptr = ptr - 1, *saveptr;

        अगर( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        करो c = *++endptr;
        जबतक( cv_isprint(c) && c != ':' );

        अगर( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        अगर( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        करो c = *--endptr;
        जबतक( c == ' ' );

        ++endptr;

        मूल्य_placeholder = fs->add_गाँठ(map_गाँठ, std::string(ptr, endptr - ptr), File_गाँठ::NONE);
        ptr = saveptr;

        वापस ptr;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाँठ& गाँठ, पूर्णांक min_indent, bool is_parent_flow, पूर्णांक depth = 0 )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अगर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        अक्षर* endptr = 0;
        अक्षर c = ptr[0], d = ptr[1];
        पूर्णांक value_type = File_गाँठ::NONE;
        पूर्णांक len;
        bool is_द्विआधारी_string = false;
        bool is_user = false;

        अगर( c == '!' ) // handle explicit type specification
        {
            अगर( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            अगर ( d == '<') //support of full type heading from YAML 1.2
            {
                स्थिर अक्षर* yamlTypeHeading = "<tag:yaml.org,2002:";
                स्थिर माप_प्रकार heading_लंबाई = म_माप(yamlTypeHeading);

                अक्षर* typeEndPtr = ++ptr;

                करो d = *++typeEndPtr;
                जबतक( cv_isprint(d) && d != ' ' && d != '>' );

                अगर ( d == '>' && (माप_प्रकार)(typeEndPtr - ptr) > heading_लंबाई )
                {
                    अगर ( स_भेद(ptr, yamlTypeHeading, heading_लंबाई) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += heading_लंबाई - 1;
                        is_user = true;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            करो d = *++endptr;
            जबतक( cv_isprint(d) && d != ' ' );
            len = (पूर्णांक)(endptr - ptr);
            अगर( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            अगर( len == 3 && !is_user )
            {
                अगर( स_भेद( ptr, "str", 3 ) == 0 )
                    value_type = File_गाँठ::STRING;
                अन्यथा अगर( स_भेद( ptr, "int", 3 ) == 0 )
                    value_type = File_गाँठ::INT;
                अन्यथा अगर( स_भेद( ptr, "seq", 3 ) == 0 )
                    value_type = File_गाँठ::SEQ;
                अन्यथा अगर( स_भेद( ptr, "map", 3 ) == 0 )
                    value_type = File_गाँठ::MAP;
            }
            अन्यथा अगर( len == 5 && !is_user )
            {
                अगर( स_भेद( ptr, "float", 5 ) == 0 )
                    value_type = File_गाँठ::REAL;
            }
            अन्यथा अगर (len == 6 && is_user)
            {
                अगर( स_भेद( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_गाँठ::SEQ;
                    is_द्विआधारी_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    करो {
                        d = *++endptr;
                        अगर (d == '|')
                            अवरोध;
                    } जबतक (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, पूर्णांक_उच्च );
            अगर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            अगर( !is_user )
            {
                अगर (value_type == File_गाँठ::STRING && c != '\'' && c != '\"')
                    जाओ force_string;
                अगर( value_type == File_गाँठ::INT )
                    जाओ force_int;
                अगर( value_type == File_गाँठ::REAL )
                    जाओ force_real;
            }
        }

        अगर (is_द्विआधारी_string)
        {
            पूर्णांक indent = static_cast<पूर्णांक>(ptr - fs->बफर_Start());
            ptr = fs->parseBase64(ptr, indent, गाँठ);
        }
        अन्यथा अगर( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            जबतक( cv_isdigit(*endptr) )
                endptr++;
            अगर( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                द्विगुन fval = fs->म_से_भग्न( ptr, &endptr );
                गाँठ.set_मूल्य(File_गाँठ::REAL, &fval);
            }
            अन्यथा
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
            }

            अगर( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
        }
        अन्यथा अगर( c == '\'' || c == '\"' ) // an explicit string
        {
            अगर( c == '\'' )
                क्रम( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    अगर( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा अगर( c == '\'' )
                    {
                        c = *++ptr;
                        अगर( c != '\'' )
                            अवरोध;
                        buf[len++] = c;
                    }
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            अन्यथा
                क्रम( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    अगर( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    अन्यथा अगर( c == '\"' )
                    {
                        ++ptr;
                        अवरोध;
                    }
                    अन्यथा अगर( c == '\\' )
                    {
                        d = *++ptr;
                        अगर( d == '\'' )
                            buf[len++] = d;
                        अन्यथा अगर( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        अन्यथा अगर( d == 'n' )
                            buf[len++] = '\n';
                        अन्यथा अगर( d == 'r' )
                            buf[len++] = '\r';
                        अन्यथा अगर( d == 't' )
                            buf[len++] = '\t';
                        अन्यथा अगर( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            पूर्णांक val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (पूर्णांक)म_से_दीर्घ( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            अगर( endptr == ptr + is_hex )
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

            अगर( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            गाँठ.set_मूल्य(File_गाँठ::STRING, buf, len);
        }
        अन्यथा अगर( c == '[' || c == '{' ) // collection as a flow
        {
            पूर्णांक new_min_indent = min_indent + !is_parent_flow;
            पूर्णांक struct_type = c == '{' ? File_गाँठ::MAP : File_गाँठ::SEQ;
            पूर्णांक nelems = 0;

            fs->convertToCollection(struct_type, गाँठ);
            d = c == '[' ? ']' : '}';

            क्रम( ++ptr ;; nelems++ )
            {
                File_गाँठ elem;

                ptr = skipSpaces( ptr, new_min_indent, पूर्णांक_उच्च );
                अगर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                अगर( *ptr == '}' || *ptr == ']' )
                {
                    अगर( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    अवरोध;
                }

                अगर( nelems != 0 )
                {
                    अगर( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, पूर्णांक_उच्च );
                    अगर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                अगर( struct_type == File_गाँठ::MAP )
                {
                    ptr = parse_कुंजी( ptr, गाँठ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, पूर्णांक_उच्च );
                }
                अन्यथा
                {
                    अगर( *ptr == ']' )
                        अवरोध;
                    elem = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE);
                }
                ptr = parse_मूल्य( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(गाँठ);
        }
        अन्यथा
        {
            पूर्णांक indent, struct_type;

            अगर( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                अगर( !is_parent_flow )
                {
                    अगर( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    अगर( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                करो c = *++endptr;
                जबतक( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_गाँठ::STRING));

                अगर( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                अगर( is_parent_flow || c != ':' )
                {
                    अक्षर* str_end = endptr;
                    करो c = *--str_end;
                    जबतक( str_end > ptr && c == ' ' );
                    str_end++;
                    अगर ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        माप_प्रकार llen = str_end - ptr;
                        अगर (llen == 4 && स_भेद(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
                            ptr = endptr;
                            वापस ptr;
                        }
                        अन्यथा अगर (llen == 5 && स_भेद(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            गाँठ.set_मूल्य(File_गाँठ::INT, &ival);
                            ptr = endptr;
                            वापस ptr;
                        }
                        अन्यथा अगर (llen == 4 && स_भेद(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            वापस ptr;
                        }
                    }

                    गाँठ.set_मूल्य(File_गाँठ::STRING, ptr, (पूर्णांक)(str_end - ptr));
                    ptr = endptr;
                    वापस ptr;
                }
                struct_type = File_गाँठ::MAP;
            }
            अन्यथा
                struct_type = File_गाँठ::SEQ;

            fs->convertToCollection( struct_type, गाँठ );
            indent = (पूर्णांक)(ptr - fs->बफर_Start());

            क्रम(;;)
            {
                File_गाँठ elem;

                अगर( struct_type == File_गाँठ::MAP )
                {
                    ptr = parse_कुंजी( ptr, गाँठ, elem );
                }
                अन्यथा
                {
                    c = *ptr++;
                    अगर( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, पूर्णांक_उच्च );
                ptr = parse_मूल्य( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                अगर( ptr - fs->बफर_Start() != indent )
                {
                    अगर( ptr - fs->बफर_Start() < indent )
                        अवरोध;
                    अन्यथा
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                अगर( स_भेद( ptr, "...", 3 ) == 0 )
                    अवरोध;
            }
            fs->finalizeCollection(गाँठ);
        }

        वापस ptr;
    }

    bool parse( अक्षर* ptr )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        File_गाँठ मूल_collection(fs->getFS(), 0, 0);

        क्रम(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            क्रम(;;)
            {
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                अगर( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    अवरोध;
                }

                अगर( *ptr == '%' )
                {
                    अगर( स_भेद( ptr, "%YAML", 5 ) == 0 &&
                        स_भेद( ptr, "%YAML:1.", 8 ) != 0 &&
                        स_भेद( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        अगर ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                अन्यथा अगर( *ptr == '-' )
                {
                    अगर( स_भेद(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        अवरोध;
                    }
                    अन्यथा अगर( first )
                        अवरोध;
                }
                अन्यथा अगर( cv_isalnum(*ptr) || *ptr=='_')
                {
                    अगर( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    अवरोध;
                }
                अन्यथा अगर( fs->खप() )
                    अवरोध;
                अन्यथा
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            अगर( ptr )
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
            अगर( !ptr || !ptr[0] )
                अवरोध;
            अगर( स_भेद( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_गाँठ मूल_गाँठ = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::NONE);

                ptr = parse_मूल्य( ptr, मूल_गाँठ, 0, false );
                अगर( !मूल_गाँठ.isMap() && !मूल_गाँठ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, पूर्णांक_उच्च );
                अगर( !ptr )
                    अवरोध;
            }

            अगर( fs->खप() )
                अवरोध;
            ptr += 3;
            first = false;
        }

        वापस ok;
    }

    FileStorage_API* fs;
    अक्षर buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    वापस makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    वापस makePtr<YAMLParser>(fs);
}

}
