// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"

गणना
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_चिह्नकम् = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_चिह्नकम् = 1,
    CV_XML_CLOSING_चिह्नकम् = 2,
    CV_XML_रिक्तानि_चिह्नकम् = 3,
    CV_XML_शीर्षकम्_चिह्नकम् = 4,
    CV_XML_DIRECTIVE_चिह्नकम् = 5
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

    शून्य write_चिह्नकम्( ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क चिह्नकम्_type, ध्रुव std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        वर्ण* ptr = fs->बफर_Ptr();
        पूर्णाङ्क i, len = 0;
        FStruct_दत्त& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( कुञ्चिका && कुञ्चिका[0] == '\0' )
            कुञ्चिका = 0;

        यदि( चिह्नकम्_type == CV_XML_OPENING_चिह्नकम् || चिह्नकम्_type == CV_XML_रिक्तानि_चिह्नकम् )
        {
            यदि( File_ग्रन्थिः::isCollection(struct_flags) )
            {
                यदि( File_ग्रन्थिः::isMap(struct_flags) ^ (कुञ्चिका != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            अन्यथा
            {
                struct_flags = File_ग्रन्थिः::रिक्तानि_2 + (कुञ्चिका ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ);
                //fs->is_first = 0;
            }

            यदि( !File_ग्रन्थिः::is_रिक्तानि_Collection(struct_flags) )
                ptr = fs->flush();
        }

        यदि( !कुञ्चिका )
            कुञ्चिका = "_";
        अन्यथा यदि( कुञ्चिका[0] == '_' && कुञ्चिका[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (पूर्णाङ्क)वाक्यदैर्घ्यम्( कुञ्चिका );
        *ptr++ = '<';
        यदि( चिह्नकम्_type == CV_XML_CLOSING_चिह्नकम् )
        {
            यदि( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        यदि( !cv_isalpha(कुञ्चिका[0]) && कुञ्चिका[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_बफर( ptr, len );
        प्रति( i = 0; i < len; i++ )
        {
            वर्ण c = कुञ्चिका[i];
            यदि( !cv_isalnum(c) && c != '_' && c != '-' )
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

            ptr = fs->resize_Write_बफर( ptr, (पूर्णाङ्क)(len0 + len1 + 4) );
            *ptr++ = ' ';

            स्मृतिं_प्रतिलिख( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            यदि( len1 > 0 )
                स्मृतिं_प्रतिलिख( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        यदि( चिह्नकम्_type == CV_XML_रिक्तानि_चिह्नकम् )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->समुच्चयः_बफर_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_ग्रन्थिः::रिक्तानि_2;
    }

    FStruct_दत्त startWriteStruct(ध्रुव FStruct_दत्त& parent, ध्रुव वर्ण* कुञ्चिका,
                                 पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name=0)
    {
        std::vector<std::string> attrlist;
        यदि( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_चिह्नकम्( कुञ्चिका, CV_XML_OPENING_चिह्नकम्, attrlist );

        FStruct_दत्त current_struct;
        current_struct.चिह्नकम् = कुञ्चिका ? std::string(कुञ्चिका) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        प्रत्यागम current_struct;
    }

    शून्य अन्तः_Write_Struct(ध्रुव FStruct_दत्त& current_struct)
    {
        write_चिह्नकम्( current_struct.चिह्नकम्.c_str(), CV_XML_CLOSING_चिह्नकम् );
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( कुञ्चिका, ptr);
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, int64_t value)
    {
        वर्ण buf[128], *ptr = fs::itoa( value, buf, 10, सत्य );
        writeScalar( कुञ्चिका, ptr);
    }

    शून्य लिख( ध्रुव वर्ण* कुञ्चिका, द्विगुण value )
    {
        वर्ण buf[128];
        writeScalar( कुञ्चिका, fs::double_गन्तव्यम्_String( buf, परिमाण(buf), value, असत्य ) );
    }

    शून्य लिख(ध्रुव वर्ण* कुञ्चिका, ध्रुव वर्ण* str, तार्किक quote)
    {
        वर्ण buf[CV_FS_MAX_LEN*6+16];
        वर्ण* दत्त = (वर्ण*)str;
        पूर्णाङ्क i, len;

        यदि( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(str);
        यदि( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        यदि( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            तार्किक need_quote = quote || len == 0;
            दत्त = buf;
            *दत्त++ = '\"';
            प्रति( i = 0; i < len; i++ )
            {
                वर्ण c = str[i];

                यदि( (uchar)c >= 128 || c == ' ' )
                {
                    *दत्त++ = c;
                    need_quote = सत्य;
                }
                अन्यथा यदि( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *दत्त++ = '&';
                    यदि( c == '<' )
                    {
                        स्मृतिं_प्रतिलिख(दत्त, "lt", 2);
                        दत्त += 2;
                    }
                    अन्यथा यदि( c == '>' )
                    {
                        स्मृतिं_प्रतिलिख(दत्त, "gt", 2);
                        दत्त += 2;
                    }
                    अन्यथा यदि( c == '&' )
                    {
                        स्मृतिं_प्रतिलिख(दत्त, "amp", 3);
                        दत्त += 3;
                    }
                    अन्यथा यदि( c == '\'' )
                    {
                        स्मृतिं_प्रतिलिख(दत्त, "apos", 4);
                        दत्त += 4;
                    }
                    अन्यथा यदि( c == '\"' )
                    {
                        स्मृतिं_प्रतिलिख( दत्त, "quot", 4);
                        दत्त += 4;
                    }
                    अन्यथा
                    {
                        snprintf( दत्त, परिमाण(buf) - (दत्त - buf), "#x%02x", (uchar)c );
                        दत्त += 4;
                    }
                    *दत्त++ = ';';
                    need_quote = 1;
                }
                अन्यथा
                    *दत्त++ = c;
            }
            यदि( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = सत्य;

            यदि( need_quote )
                *दत्त++ = '\"';
            len = (पूर्णाङ्क)(दत्त - buf) - !need_quote;
            *दत्त++ = '\0';
            दत्त = buf + (पूर्णाङ्क)!need_quote;
        }

        writeScalar( कुञ्चिका, दत्त );
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

        पूर्णाङ्क len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(दत्त);
        यदि( कुञ्चिका && *कुञ्चिका == '\0' )
            कुञ्चिका = 0;

        FStruct_दत्त& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क struct_flags = current_struct.flags;

        यदि( File_ग्रन्थिः::isMap(struct_flags) ||
           (!File_ग्रन्थिः::isCollection(struct_flags) && कुञ्चिका) )
        {
            write_चिह्नकम्( कुञ्चिका, CV_XML_OPENING_चिह्नकम् );
            वर्ण* ptr = fs->resize_Write_बफर( fs->बफर_Ptr(), len );
            स्मृतिं_प्रतिलिख( ptr, दत्त, len );
            fs->समुच्चयः_बफर_Ptr( ptr + len );
            write_चिह्नकम्( कुञ्चिका, CV_XML_CLOSING_चिह्नकम् );
        }
        अन्यथा
        {
            वर्ण* ptr = fs->बफर_Ptr();
            पूर्णाङ्क new_विस्थापनम् = (पूर्णाङ्क)(ptr - fs->बफर_Start()) + len;

            यदि( कुञ्चिका )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ग्रन्थिः::SEQ;

            यदि( (new_विस्थापनम् > fs->wrapMargin() && new_विस्थापनम् - current_struct.indent > 10) ||
               (ptr > fs->बफर_Start() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            अन्यथा यदि( ptr > fs->बफर_Start() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            स्मृतिं_प्रतिलिख( ptr, दत्त, len );
            fs->समुच्चयः_बफर_Ptr(ptr + len);
        }
    }

    शून्य writeComment(ध्रुव वर्ण* comment, तार्किक eol_comment)
    {
        FStruct_दत्त& current_struct = fs->getCurrentStruct();
        पूर्णाङ्क len;
        पूर्णाङ्क multiline;
        ध्रुव वर्ण* eol;
        वर्ण* ptr;

        यदि( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        यदि( वाक्ये_अन्विष(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->बफर_Ptr();

        यदि( multiline || !eol_comment || fs->बफर_अन्तः_2() - ptr < len + 5 )
            ptr = fs->flush();
        अन्यथा यदि( ptr > fs->बफर_Start() + current_struct.indent )
            *ptr++ = ' ';

        यदि( !multiline )
        {
            ptr = fs->resize_Write_बफर( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(ptr);
        }
        अन्यथा
        {
            वाक्यं_प्रतिलिख( ptr, "<!--" );
            len = 4;
        }

        fs->समुच्चयः_बफर_Ptr(ptr + len);
        ptr = fs->flush();

        यदि( multiline )
        {
            यावत्( comment )
            {
                यदि( eol )
                {
                    ptr = fs->resize_Write_बफर( ptr, (पूर्णाङ्क)(eol - comment) + 1 );
                    स्मृतिं_प्रतिलिख( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                अन्यथा
                {
                    len = (पूर्णाङ्क)वाक्यदैर्घ्यम्(comment);
                    ptr = fs->resize_Write_बफर( ptr, len );
                    स्मृतिं_प्रतिलिख( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->समुच्चयः_बफर_Ptr(ptr);
                ptr = fs->flush();
            }
            वाक्यं_प्रतिलिख( ptr, "-->" );
            fs->समुच्चयः_बफर_Ptr(ptr + 3);
            fs->flush();
        }
    }

    शून्य start_अग्र_प्रवाहः()
    {
        fs->पङ्क्तिं_लिख( "\n<!-- next stream -->\n" );
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

    वर्ण* skipSpaces( वर्ण* ptr, पूर्णाङ्क बहुलकम् )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        पूर्णाङ्क level = 0;

        प्रति(;;)
        {
            वर्ण c;
            ptr--;

            यदि( बहुलकम् == CV_XML_INSIDE_COMMENT )
            {
                कुरु c = *++ptr;
                यावत्( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                यदि( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    बहुलकम् = 0;
                    ptr += 3;
                }
            }
            अन्यथा यदि( बहुलकम् == CV_XML_INSIDE_DIRECTIVE )
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
                    यदि( बहुलकम् != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    बहुलकम् = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                अन्यथा यदि( cv_isprint(c) )
                    भङ्ग;
            }

            यदि( !cv_isprint(*ptr) )
            {
                यदि( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                यदि( !ptr || *ptr == '\0' )
                    भङ्ग;
            }
        }
        प्रत्यागम ptr;
    }

    तार्किक getBase64Row(वर्ण* ptr, पूर्णाङ्क /*indent*/, वर्ण* &beg, वर्ण* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_चिह्नकम्);
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

    वर्ण* parse_मानम्( वर्ण* ptr, File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क गभीरता = 0 )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( गभीरता > CV_PERSISTENCE_MAX_गभीरता )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ग्रन्थिः new_elem;
        तार्किक have_space = सत्य;
        पूर्णाङ्क value_type = ग्रन्थिः.type();
        std::string कुञ्चिका, key2, type_name;

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
                पूर्णाङ्क चिह्नकम्_type = 0;
                पूर्णाङ्क elem_type = File_ग्रन्थिः::NONE;

                यदि( d == '/' || c == '\0' )
                    भङ्ग;

                ptr = parse_चिह्नकम्( ptr, कुञ्चिका, type_name, चिह्नकम्_type );

                यदि( चिह्नकम्_type == CV_XML_DIRECTIVE_चिह्नकम् )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                यदि( चिह्नकम्_type == CV_XML_रिक्तानि_चिह्नकम् )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(चिह्नकम्_type == CV_XML_OPENING_चिह्नकम्);

                /* for base64 string */
                तार्किक द्विमान_string = असत्य;

                यदि( !type_name.empty() )
                {
                    ध्रुव वर्ण* tn = type_name.c_str();
                    यदि( वाक्यं_तुलय(tn, "str") == 0 )
                        elem_type = File_ग्रन्थिः::STRING;
                    अन्यथा यदि( वाक्यं_तुलय( tn, "map" ) == 0 )
                        elem_type = File_ग्रन्थिः::MAP;
                    अन्यथा यदि( वाक्यं_तुलय( tn, "seq" ) == 0 )
                        elem_type = File_ग्रन्थिः::SEQ;
                    अन्यथा यदि( वाक्यं_तुलय( tn, "binary") == 0)
                        द्विमान_string = सत्य;
                }

                new_elem = fs->सङ्कलय_ग्रन्थिः(ग्रन्थिः, कुञ्चिका, elem_type, 0);
                यदि (!द्विमान_string)
                    ptr = parse_मानम्(ptr, new_elem, गभीरता + 1);
                अन्यथा
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_चिह्नकम्( ptr, key2, type_name, चिह्नकम्_type );
                यदि( चिह्नकम्_type != CV_XML_CLOSING_चिह्नकम् || key2 != कुञ्चिका )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = सत्य;
            }
            अन्यथा
            {
                यदि( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ग्रन्थिः* elem = &ग्रन्थिः;
                यदि( ग्रन्थिः.type() != File_ग्रन्थिः::NONE )
                {
                    fs->convert_गन्तव्यम्_Collection( File_ग्रन्थिः::SEQ, ग्रन्थिः );
                    new_elem = fs->सङ्कलय_ग्रन्थिः(ग्रन्थिः, std::string(), File_ग्रन्थिः::NONE, 0);
                    elem = &new_elem;
                }

                यदि( value_type != File_ग्रन्थिः::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    यावत्( cv_isdigit(*endptr) )
                        endptr++;
                    यदि( *endptr == '.' || *endptr == 'e' )
                    {
                        द्विगुण fval = fs->strtod( ptr, &endptr );
                        elem->समुच्चयः_मानम्(File_ग्रन्थिः::REAL, &fval);
                    }
                    अन्यथा
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->समुच्चयः_मानम्(File_ग्रन्थिः::INT, &ival);
                    }

                    यदि( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
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
                        CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();

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
                                    पूर्णाङ्क val, आधारः = 10;
                                    ptr++;
                                    यदि( *ptr == 'x' )
                                    {
                                        आधारः = 16;
                                        ptr++;
                                    }
                                    val = (पूर्णाङ्क)strtol( ptr, &endptr, आधारः );
                                    यदि( (अचिह्नित)val > (अचिह्नित)255 ||
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
                                    यदि( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    अन्यथा यदि( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    अन्यथा यदि( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    अन्यथा यदि( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    अन्यथा यदि( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    अन्यथा
                                    {
                                        यदि (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        स्मृतिं_प्रतिलिख( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();
                            }
                        }
                        यदि (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->समुच्चयः_मानम्(File_ग्रन्थिः::STRING, strbuf, i);
                }

                यदि( value_type != File_ग्रन्थिः::NONE && value_type != File_ग्रन्थिः::SEQ && value_type != File_ग्रन्थिः::MAP )
                    भङ्ग;
                have_space = असत्य;
            }
        }
        fs->finalizeCollection(ग्रन्थिः);

        प्रत्यागम ptr;
    }

    वर्ण* parse_चिह्नकम्( वर्ण* ptr, std::string& चिह्नकम्_name,
                    std::string& type_name, पूर्णाङ्क& चिह्नकम्_type )
    {
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        यदि( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        यदि( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();

        यदि( cv_isalnum(*ptr) || *ptr == '_' )
            चिह्नकम्_type = CV_XML_OPENING_चिह्नकम्;
        अन्यथा यदि( *ptr == '/' )
        {
            चिह्नकम्_type = CV_XML_CLOSING_चिह्नकम्;
            ptr++;
        }
        अन्यथा यदि( *ptr == '?' )
        {
            चिह्नकम्_type = CV_XML_शीर्षकम्_चिह्नकम्;
            ptr++;
        }
        अन्यथा यदि( *ptr == '!' )
        {
            चिह्नकम्_type = CV_XML_DIRECTIVE_चिह्नकम्;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        अन्यथा
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        चिह्नकम्_name.clear();
        type_name.clear();

        प्रति(;;)
        {
            वर्ण c, *endptr;
            यदि( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            कुरु c = *++endptr;
            यावत्( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_अन्तः_सम्बन्धि_बफर_BUG_CPP();

            यदि( चिह्नकम्_name.empty() )
                चिह्नकम्_name = attrname;
            अन्यथा
            {
                यदि( चिह्नकम्_type == CV_XML_CLOSING_चिह्नकम् )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                यदि( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_चिह्नकम् );
                    यदि (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    यदि( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                यदि( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_चिह्नकम् );
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
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            तार्किक have_space = cv_isspace(c) || c == '\0';

            यदि( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_चिह्नकम् );
                यदि (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            यदि( c == '>' )
            {
                यदि( चिह्नकम्_type == CV_XML_शीर्षकम्_चिह्नकम् )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                भङ्ग;
            }
            अन्यथा यदि( c == '?' && चिह्नकम्_type == CV_XML_शीर्षकम्_चिह्नकम् )
            {
                यदि( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                भङ्ग;
            }
            अन्यथा यदि( c == '/' && ptr[1] == '>' && चिह्नकम्_type == CV_XML_OPENING_चिह्नकम् )  // FIXIT ptr[1] - out of bounds read without check
            {
                चिह्नकम्_type = CV_XML_रिक्तानि_चिह्नकम्;
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

        std::string कुञ्चिका, key2, type_name;
        पूर्णाङ्क चिह्नकम्_type = 0;
        तार्किक ok = असत्य;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_चिह्नकम् );
        यदि (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        यदि( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_चिह्नकम्( ptr, कुञ्चिका, type_name, चिह्नकम्_type );
        File_ग्रन्थिः मूलम्_collection(fs->getFS(), 0, 0);

        यावत्( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            यदि (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            यदि( *ptr != '\0' )
            {
                ptr = parse_चिह्नकम्( ptr, कुञ्चिका, type_name, चिह्नकम्_type );
                यदि( चिह्नकम्_type != CV_XML_OPENING_चिह्नकम् || कुञ्चिका != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ग्रन्थिः मूलम् = fs->सङ्कलय_ग्रन्थिः(मूलम्_collection, std::string(), File_ग्रन्थिः::MAP, 0);
                ptr = parse_मानम्( ptr, मूलम् );
                ptr = parse_चिह्नकम्( ptr, key2, type_name, चिह्नकम्_type );
                यदि( चिह्नकम्_type != CV_XML_CLOSING_चिह्नकम् || कुञ्चिका != key2 )
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
