// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "persistence.hpp"

क्रमागत
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

namespace cv
{

श्रेणी XMLEmitter : खुला FileStorageEmitter
{
खुला:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    भव ~XMLEmitter() {}

    व्योम writeTag( स्थिर अक्षर* कुंजी, पूर्णांक tag_type, स्थिर std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        अक्षर* ptr = fs->बफर_Ptr();
        पूर्णांक i, len = 0;
        FStruct_तथ्य& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;

        अगर( कुंजी && कुंजी[0] == '\0' )
            कुंजी = 0;

        अगर( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            अगर( File_गाँठ::isCollection(struct_flags) )
            {
                अगर( File_गाँठ::isMap(struct_flags) ^ (कुंजी != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अन्यथा
            {
                struct_flags = File_गाँठ::EMPTY + (कुंजी ? File_गाँठ::MAP : File_गाँठ::SEQ);
                //fs->is_first = 0;
            }

            अगर( !File_गाँठ::isEmptyCollection(struct_flags) )
                ptr = fs->सफाई();
        }

        अगर( !कुंजी )
            कुंजी = "_";
        अन्यथा अगर( कुंजी[0] == '_' && कुंजी[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णांक)म_माप( कुंजी );
        *ptr++ = '<';
        अगर( tag_type == CV_XML_CLOSING_TAG )
        {
            अगर( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        अगर( !cv_isalpha(कुंजी[0]) && कुंजी[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_बफर( ptr, len );
        क्रम( i = 0; i < len; i++ )
        {
            अक्षर c = कुंजी[i];
            अगर( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        पूर्णांक nattr = (पूर्णांक)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        क्रम( i = 0; i < nattr; i += 2 )
        {
            माप_प्रकार len0 = attrlist[i].size();
            माप_प्रकार len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_बफर( ptr, (पूर्णांक)(len0 + len1 + 4) );
            *ptr++ = ' ';

            स_नकल( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            अगर( len1 > 0 )
                स_नकल( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        अगर( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->set_बफर_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_गाँठ::EMPTY;
    }

    FStruct_तथ्य startWriteStruct(स्थिर FStruct_तथ्य& parent, स्थिर अक्षर* कुंजी,
                                 पूर्णांक struct_flags, स्थिर अक्षर* type_name=0)
    {
        std::vector<std::string> attrlist;
        अगर( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( कुंजी, CV_XML_OPENING_TAG, attrlist );

        FStruct_तथ्य current_struct;
        current_struct.tag = कुंजी ? std::string(कुंजी) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        वापस current_struct;
    }

    व्योम endWriteStruct(स्थिर FStruct_तथ्य& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, पूर्णांक value)
    {
        अक्षर buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( कुंजी, ptr);
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, int64_t value)
    {
        अक्षर buf[128], *ptr = fs::itoa( value, buf, 10, true );
        writeScalar( कुंजी, ptr);
    }

    व्योम ग_लिखो( स्थिर अक्षर* कुंजी, द्विगुन value )
    {
        अक्षर buf[128];
        writeScalar( कुंजी, fs::doubleToString( buf, माप(buf), value, false ) );
    }

    व्योम ग_लिखो(स्थिर अक्षर* कुंजी, स्थिर अक्षर* str, bool quote)
    {
        अक्षर buf[CV_FS_MAX_LEN*6+16];
        अक्षर* तथ्य = (अक्षर*)str;
        पूर्णांक i, len;

        अगर( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णांक)म_माप(str);
        अगर( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        अगर( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            तथ्य = buf;
            *तथ्य++ = '\"';
            क्रम( i = 0; i < len; i++ )
            {
                अक्षर c = str[i];

                अगर( (uchar)c >= 128 || c == ' ' )
                {
                    *तथ्य++ = c;
                    need_quote = true;
                }
                अन्यथा अगर( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *तथ्य++ = '&';
                    अगर( c == '<' )
                    {
                        स_नकल(तथ्य, "lt", 2);
                        तथ्य += 2;
                    }
                    अन्यथा अगर( c == '>' )
                    {
                        स_नकल(तथ्य, "gt", 2);
                        तथ्य += 2;
                    }
                    अन्यथा अगर( c == '&' )
                    {
                        स_नकल(तथ्य, "amp", 3);
                        तथ्य += 3;
                    }
                    अन्यथा अगर( c == '\'' )
                    {
                        स_नकल(तथ्य, "apos", 4);
                        तथ्य += 4;
                    }
                    अन्यथा अगर( c == '\"' )
                    {
                        स_नकल( तथ्य, "quot", 4);
                        तथ्य += 4;
                    }
                    अन्यथा
                    {
                        snprintf( तथ्य, माप(buf) - (तथ्य - buf), "#x%02x", (uchar)c );
                        तथ्य += 4;
                    }
                    *तथ्य++ = ';';
                    need_quote = 1;
                }
                अन्यथा
                    *तथ्य++ = c;
            }
            अगर( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            अगर( need_quote )
                *तथ्य++ = '\"';
            len = (पूर्णांक)(तथ्य - buf) - !need_quote;
            *तथ्य++ = '\0';
            तथ्य = buf + (पूर्णांक)!need_quote;
        }

        writeScalar( कुंजी, तथ्य );
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

        पूर्णांक len = (पूर्णांक)म_माप(तथ्य);
        अगर( कुंजी && *कुंजी == '\0' )
            कुंजी = 0;

        FStruct_तथ्य& current_struct = fs->getCurrentStruct();
        पूर्णांक struct_flags = current_struct.flags;

        अगर( File_गाँठ::isMap(struct_flags) ||
           (!File_गाँठ::isCollection(struct_flags) && कुंजी) )
        {
            writeTag( कुंजी, CV_XML_OPENING_TAG );
            अक्षर* ptr = fs->resize_Write_बफर( fs->बफर_Ptr(), len );
            स_नकल( ptr, तथ्य, len );
            fs->set_बफर_Ptr( ptr + len );
            writeTag( कुंजी, CV_XML_CLOSING_TAG );
        }
        अन्यथा
        {
            अक्षर* ptr = fs->बफर_Ptr();
            पूर्णांक new_offset = (पूर्णांक)(ptr - fs->बफर_Start()) + len;

            अगर( कुंजी )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_गाँठ::SEQ;

            अगर( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->बफर_Start() && ptr[-1] == '>') )
            {
                ptr = fs->सफाई();
            }
            अन्यथा अगर( ptr > fs->बफर_Start() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            स_नकल( ptr, तथ्य, len );
            fs->set_बफर_Ptr(ptr + len);
        }
    }

    व्योम writeComment(स्थिर अक्षर* comment, bool eol_comment)
    {
        FStruct_तथ्य& current_struct = fs->getCurrentStruct();
        पूर्णांक len;
        पूर्णांक multiline;
        स्थिर अक्षर* eol;
        अक्षर* ptr;

        अगर( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        अगर( म_माला(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (पूर्णांक)म_माप(comment);
        eol = म_अक्षर(comment, '\n');
        multiline = eol != 0;
        ptr = fs->बफर_Ptr();

        अगर( multiline || !eol_comment || fs->बफर_End() - ptr < len + 5 )
            ptr = fs->सफाई();
        अन्यथा अगर( ptr > fs->बफर_Start() + current_struct.indent )
            *ptr++ = ' ';

        अगर( !multiline )
        {
            ptr = fs->resize_Write_बफर( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (पूर्णांक)म_माप(ptr);
        }
        अन्यथा
        {
            म_नकल( ptr, "<!--" );
            len = 4;
        }

        fs->set_बफर_Ptr(ptr + len);
        ptr = fs->सफाई();

        अगर( multiline )
        {
            जबतक( comment )
            {
                अगर( eol )
                {
                    ptr = fs->resize_Write_बफर( ptr, (पूर्णांक)(eol - comment) + 1 );
                    स_नकल( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = म_अक्षर( comment, '\n' );
                }
                अन्यथा
                {
                    len = (पूर्णांक)म_माप(comment);
                    ptr = fs->resize_Write_बफर( ptr, len );
                    स_नकल( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->set_बफर_Ptr(ptr);
                ptr = fs->सफाई();
            }
            म_नकल( ptr, "-->" );
            fs->set_बफर_Ptr(ptr + 3);
            fs->सफाई();
        }
    }

    व्योम start_अग्र_Stream()
    {
        fs->माला_दो( "\n<!-- next stream -->\n" );
    }

रक्षित:
    FileStorage_API* fs;
};

श्रेणी XMLParser : खुला FileStorageParser
{
खुला:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    भव ~XMLParser() {}

    अक्षर* skipSpaces( अक्षर* ptr, पूर्णांक mode )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        पूर्णांक level = 0;

        क्रम(;;)
        {
            अक्षर c;
            ptr--;

            अगर( mode == CV_XML_INSIDE_COMMENT )
            {
                करो c = *++ptr;
                जबतक( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                अगर( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            अन्यथा अगर( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                करो
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    अगर( level < 0 )
                        वापस ptr;
                } जबतक( cv_isprint_or_tab(c) );
            }
            अन्यथा
            {
                करो c = *++ptr;
                जबतक( c == ' ' || c == '\t' );

                अगर( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    अगर( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अन्यथा अगर( cv_isprint(c) )
                    अवरोध;
            }

            अगर( !cv_isprint(*ptr) )
            {
                अगर( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->माला_लो();
                अगर( !ptr || *ptr == '\0' )
                    अवरोध;
            }
        }
        वापस ptr;
    }

    bool getBase64Row(अक्षर* ptr, पूर्णांक /*indent*/, अक्षर* &beg, अक्षर* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        अगर( !ptr || !*ptr )
            वापस false;

        // closing XML tag
        अगर ( *beg == '<' )
            वापस false;

        // find end of the row
        जबतक( cv_isprint(*ptr) )
            ++ptr;
        अगर ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        वापस true;
    }

    अक्षर* parse_मूल्य( अक्षर* ptr, File_गाँठ& गाँठ, पूर्णांक depth = 0 )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अगर( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_गाँठ new_elem;
        bool have_space = true;
        पूर्णांक value_type = गाँठ.type();
        std::string कुंजी, key2, type_name;

        क्रम(;;)
        {
            अक्षर c = *ptr, d;
            अक्षर* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            अगर( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                अगर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            अगर( c =='<' || c == '\0' )
            {
                पूर्णांक tag_type = 0;
                पूर्णांक elem_type = File_गाँठ::NONE;

                अगर( d == '/' || c == '\0' )
                    अवरोध;

                ptr = parseTag( ptr, कुंजी, type_name, tag_type );

                अगर( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                अगर( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool द्विआधारी_string = false;

                अगर( !type_name.empty() )
                {
                    स्थिर अक्षर* tn = type_name.c_str();
                    अगर( म_भेद(tn, "str") == 0 )
                        elem_type = File_गाँठ::STRING;
                    अन्यथा अगर( म_भेद( tn, "map" ) == 0 )
                        elem_type = File_गाँठ::MAP;
                    अन्यथा अगर( म_भेद( tn, "seq" ) == 0 )
                        elem_type = File_गाँठ::SEQ;
                    अन्यथा अगर( म_भेद( tn, "binary") == 0)
                        द्विआधारी_string = true;
                }

                new_elem = fs->add_गाँठ(गाँठ, कुंजी, elem_type, 0);
                अगर (!द्विआधारी_string)
                    ptr = parse_मूल्य(ptr, new_elem, depth + 1);
                अन्यथा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    अगर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                अगर( tag_type != CV_XML_CLOSING_TAG || key2 != कुंजी )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            अन्यथा
            {
                अगर( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_गाँठ* elem = &गाँठ;
                अगर( गाँठ.type() != File_गाँठ::NONE )
                {
                    fs->convertToCollection( File_गाँठ::SEQ, गाँठ );
                    new_elem = fs->add_गाँठ(गाँठ, std::string(), File_गाँठ::NONE, 0);
                    elem = &new_elem;
                }

                अगर( value_type != File_गाँठ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    जबतक( cv_isdigit(*endptr) )
                        endptr++;
                    अगर( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुन fval = fs->म_से_भग्न( ptr, &endptr );
                        elem->set_मूल्य(File_गाँठ::REAL, &fval);
                    }
                    अन्यथा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_मूल्य(File_गाँठ::INT, &ival);
                    }

                    अगर( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
                }
                अन्यथा
                {
                    // string
                    पूर्णांक i = 0, len, is_quoted = 0;
                    अगर( c == '\"' )
                        is_quoted = 1;
                    अन्यथा
                        --ptr;
                    strbuf[0] = '\0';

                    क्रम( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();

                        अगर( !cv_isalnum(c) )
                        {
                            अगर( c == '\"' )
                            {
                                अगर( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                अवरोध;
                            }
                            अन्यथा अगर( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                अगर( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                अवरोध;
                            }
                            अन्यथा अगर( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            अन्यथा अगर( c == '&' )
                            {
                                अगर( *++ptr == '#' )
                                {
                                    पूर्णांक val, base = 10;
                                    ptr++;
                                    अगर( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (पूर्णांक)म_से_दीर्घ( ptr, &endptr, base );
                                    अगर( (अचिन्हित)val > (अचिन्हित)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (अक्षर)val;
                                }
                                अन्यथा
                                {
                                    endptr = ptr;
                                    करो c = *++endptr;
                                    जबतक( cv_isalnum(c) );
                                    अगर( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (पूर्णांक)(endptr - ptr);
                                    अगर( len == 2 && स_भेद( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    अन्यथा अगर( len == 2 && स_भेद( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    अन्यथा अगर( len == 3 && स_भेद( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    अन्यथा अगर( len == 4 && स_भेद( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    अन्यथा अगर( len == 4 && स_भेद( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    अन्यथा
                                    {
                                        अगर (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        स_नकल( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();
                            }
                        }
                        अगर (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_मूल्य(File_गाँठ::STRING, strbuf, i);
                }

                अगर( value_type != File_गाँठ::NONE && value_type != File_गाँठ::SEQ && value_type != File_गाँठ::MAP )
                    अवरोध;
                have_space = false;
            }
        }
        fs->finalizeCollection(गाँठ);

        वापस ptr;
    }

    अक्षर* parseTag( अक्षर* ptr, std::string& tag_name,
                    std::string& type_name, पूर्णांक& tag_type )
    {
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        अगर( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        अगर( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();

        अगर( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        अन्यथा अगर( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        अन्यथा अगर( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        अन्यथा अगर( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        अन्यथा
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.सफा();
        type_name.सफा();

        क्रम(;;)
        {
            अक्षर c, *endptr;
            अगर( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            करो c = *++endptr;
            जबतक( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (माप_प्रकार)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP();

            अगर( tag_name.empty() )
                tag_name = attrname;
            अन्यथा
            {
                अगर( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                अगर( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    अगर (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    अगर( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                अगर( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    अगर(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    अगर( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                अक्षर quote = *ptr++;
                endptr = ptr;
                क्रम(;;)
                {
                    c = *endptr++;
                    अगर( c == quote )
                        अवरोध;
                    अगर( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                अगर( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (माप_प्रकार)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            अगर( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                अगर (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            अगर( c == '>' )
            {
                अगर( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                अवरोध;
            }
            अन्यथा अगर( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                अगर( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                अवरोध;
            }
            अन्यथा अगर( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                अवरोध;
            }

            अगर( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        वापस ptr;
    }

    bool parse(अक्षर* ptr)
    {
        CV_Assert( fs != 0 );

        std::string कुंजी, key2, type_name;
        पूर्णांक tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        अगर (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        अगर( स_भेद( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, कुंजी, type_name, tag_type );
        File_गाँठ मूल_collection(fs->getFS(), 0, 0);

        जबतक( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            अगर (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            अगर( *ptr != '\0' )
            {
                ptr = parseTag( ptr, कुंजी, type_name, tag_type );
                अगर( tag_type != CV_XML_OPENING_TAG || कुंजी != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_गाँठ मूल = fs->add_गाँठ(मूल_collection, std::string(), File_गाँठ::MAP, 0);
                ptr = parse_मूल्य( ptr, मूल );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                अगर( tag_type != CV_XML_CLOSING_TAG || कुंजी != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->खप() );
        वापस ok;
    }

    FileStorage_API* fs;
    अक्षर strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    वापस makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    वापस makePtr<XMLParser>(fs);
}

}
