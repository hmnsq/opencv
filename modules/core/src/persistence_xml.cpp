// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "persistence.hpp"

شمار
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_لیبل = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_لیبل = 1,
    CV_XML_CLOSING_لیبل = 2,
    CV_XML_خالی_خانے_لیبل = 3,
    CV_XML_سرخی_لیبل = 4,
    CV_XML_DIRECTIVE_لیبل = 5
};

نامخلا cv
{

جماعت XMLEmitter : عوامی FileStorageEmitter
{
عوامی:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~XMLEmitter() {}

    خالی write_لیبل( ثابت حرف* کلید, صحیح لیبل_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->بفر_Ptr();
        صحیح i, len = 0;
        FStruct_ڈیٹا& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;

        اگر( کلید && کلید[0] == '\0' )
            کلید = 0;

        اگر( لیبل_type == CV_XML_OPENING_لیبل || لیبل_type == CV_XML_خالی_خانے_لیبل )
        {
            اگر( File_گرہ::isCollection(struct_flags) )
            {
                اگر( File_گرہ::isMap(struct_flags) ^ (کلید != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ورنہ
            {
                struct_flags = File_گرہ::خالی_خانے_2 + (کلید ? File_گرہ::MAP : File_گرہ::SEQ);
                //fs->is_first = 0;
            }

            اگر( !File_گرہ::is_خالی_خانے_Collection(struct_flags) )
                ptr = fs->flush();
        }

        اگر( !کلید )
            کلید = "_";
        ورنہ اگر( کلید[0] == '_' && کلید[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحیح)لڑی_لمبائی( کلید );
        *ptr++ = '<';
        اگر( لیبل_type == CV_XML_CLOSING_لیبل )
        {
            اگر( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_بفر( ptr, len );
        برائے( i = 0; i < len; i++ )
        {
            حرف c = کلید[i];
            اگر( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        صحیح nattr = (صحیح)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        برائے( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_بفر( ptr, (صحیح)(len0 + len1 + 4) );
            *ptr++ = ' ';

            حافظہ_نقل( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            اگر( len1 > 0 )
                حافظہ_نقل( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        اگر( لیبل_type == CV_XML_خالی_خانے_لیبل )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->سیٹ_بفر_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_گرہ::خالی_خانے_2;
    }

    FStruct_ڈیٹا آغاز_Write_Struct(ثابت FStruct_ڈیٹا& والد, ثابت حرف* کلید,
                                 صحیح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        اگر( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_لیبل( کلید, CV_XML_OPENING_لیبل, attrlist );

        FStruct_ڈیٹا current_struct;
        current_struct.لیبل = کلید ? std::string(کلید) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = والد.indent + CV_XML_INDENT;

        واپس current_struct;
    }

    خالی انتہا_Write_Struct(ثابت FStruct_ڈیٹا& current_struct)
    {
        write_لیبل( current_struct.لیبل.c_str(), CV_XML_CLOSING_لیبل );
    }

    خالی اكتب(ثابت حرف* کلید, صحیح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلید, ptr);
    }

    خالی اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, سچ );
        writeScalar( کلید, ptr);
    }

    خالی اكتب( ثابت حرف* کلید, دوہرا value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_مقصد_String( buf, حجم(buf), value, جھوٹ ) );
    }

    خالی اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقی quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* ڈیٹا = (حرف*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)لڑی_لمبائی(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقی need_quote = quote || len == 0;
            ڈیٹا = buf;
            *ڈیٹا++ = '\"';
            برائے( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                اگر( (uchar)c >= 128 || c == ' ' )
                {
                    *ڈیٹا++ = c;
                    need_quote = سچ;
                }
                ورنہ اگر( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *ڈیٹا++ = '&';
                    اگر( c == '<' )
                    {
                        حافظہ_نقل(ڈیٹا, "lt", 2);
                        ڈیٹا += 2;
                    }
                    ورنہ اگر( c == '>' )
                    {
                        حافظہ_نقل(ڈیٹا, "gt", 2);
                        ڈیٹا += 2;
                    }
                    ورنہ اگر( c == '&' )
                    {
                        حافظہ_نقل(ڈیٹا, "amp", 3);
                        ڈیٹا += 3;
                    }
                    ورنہ اگر( c == '\'' )
                    {
                        حافظہ_نقل(ڈیٹا, "apos", 4);
                        ڈیٹا += 4;
                    }
                    ورنہ اگر( c == '\"' )
                    {
                        حافظہ_نقل( ڈیٹا, "quot", 4);
                        ڈیٹا += 4;
                    }
                    ورنہ
                    {
                        snprintf( ڈیٹا, حجم(buf) - (ڈیٹا - buf), "#x%02x", (uchar)c );
                        ڈیٹا += 4;
                    }
                    *ڈیٹا++ = ';';
                    need_quote = 1;
                }
                ورنہ
                    *ڈیٹا++ = c;
            }
            اگر( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = سچ;

            اگر( need_quote )
                *ڈیٹا++ = '\"';
            len = (صحیح)(ڈیٹا - buf) - !need_quote;
            *ڈیٹا++ = '\0';
            ڈیٹا = buf + (صحیح)!need_quote;
        }

        writeScalar( کلید, ڈیٹا );
    }

    خالی writeScalar(ثابت حرف* کلید, ثابت حرف* ڈیٹا)
    {
        fs->check_if_write_struct_is_delayed(جھوٹ);
        اگر ( fs->get_state_کا_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_مقصد_Base64_state( FileStorage_API::NotUse );
        }
        ورنہ اگر ( fs->get_state_کا_writing_base64() == FileStorage_API::اندر_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحیح len = (صحیح)لڑی_لمبائی(ڈیٹا);
        اگر( کلید && *کلید == '\0' )
            کلید = 0;

        FStruct_ڈیٹا& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;

        اگر( File_گرہ::isMap(struct_flags) ||
           (!File_گرہ::isCollection(struct_flags) && کلید) )
        {
            write_لیبل( کلید, CV_XML_OPENING_لیبل );
            حرف* ptr = fs->resize_Write_بفر( fs->بفر_Ptr(), len );
            حافظہ_نقل( ptr, ڈیٹا, len );
            fs->سیٹ_بفر_Ptr( ptr + len );
            write_لیبل( کلید, CV_XML_CLOSING_لیبل );
        }
        ورنہ
        {
            حرف* ptr = fs->بفر_Ptr();
            صحیح new_انحراف = (صحیح)(ptr - fs->بفر_آغاز()) + len;

            اگر( کلید )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_گرہ::SEQ;

            اگر( (new_انحراف > fs->wrapMargin() && new_انحراف - current_struct.indent > 10) ||
               (ptr > fs->بفر_آغاز() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ورنہ اگر( ptr > fs->بفر_آغاز() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            حافظہ_نقل( ptr, ڈیٹا, len );
            fs->سیٹ_بفر_Ptr(ptr + len);
        }
    }

    خالی writeComment(ثابت حرف* comment, منطقی eol_comment)
    {
        FStruct_ڈیٹا& current_struct = fs->getCurrentStruct();
        صحیح len;
        صحیح multiline;
        ثابت حرف* eol;
        حرف* ptr;

        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        اگر( لڑی_تلاش(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحیح)لڑی_لمبائی(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->بفر_Ptr();

        اگر( multiline || !eol_comment || fs->بفر_انتہا_2() - ptr < len + 5 )
            ptr = fs->flush();
        ورنہ اگر( ptr > fs->بفر_آغاز() + current_struct.indent )
            *ptr++ = ' ';

        اگر( !multiline )
        {
            ptr = fs->resize_Write_بفر( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحیح)لڑی_لمبائی(ptr);
        }
        ورنہ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->سیٹ_بفر_Ptr(ptr + len);
        ptr = fs->flush();

        اگر( multiline )
        {
            جب( comment )
            {
                اگر( eol )
                {
                    ptr = fs->resize_Write_بفر( ptr, (صحیح)(eol - comment) + 1 );
                    حافظہ_نقل( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ورنہ
                {
                    len = (صحیح)لڑی_لمبائی(comment);
                    ptr = fs->resize_Write_بفر( ptr, len );
                    حافظہ_نقل( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->سیٹ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->سیٹ_بفر_Ptr(ptr + 3);
            fs->flush();
        }
    }

    خالی آغاز_اگلا_دھارا()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

محفوظ:
    FileStorage_API* fs;
};

جماعت XMLParser : عوامی FileStorageParser
{
عوامی:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~XMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحیح منوال )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحیح level = 0;

        برائے(;;)
        {
            حرف c;
            ptr--;

            اگر( منوال == CV_XML_INSIDE_COMMENT )
            {
                کرو c = *++ptr;
                جب( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                اگر( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    منوال = 0;
                    ptr += 3;
                }
            }
            ورنہ اگر( منوال == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                کرو
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    اگر( level < 0 )
                        واپس ptr;
                } جب( cv_isprint_or_tab(c) );
            }
            ورنہ
            {
                کرو c = *++ptr;
                جب( c == ' ' || c == '\t' );

                اگر( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    اگر( منوال != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    منوال = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ورنہ اگر( cv_isprint(c) )
                    توڑو;
            }

            اگر( !cv_isprint(*ptr) )
            {
                اگر( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                اگر( !ptr || *ptr == '\0' )
                    توڑو;
            }
        }
        واپس ptr;
    }

    منطقی get_Base64_صف(حرف* ptr, صحیح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_لیبل);
        اگر( !ptr || !*ptr )
            واپس جھوٹ;

        // closing XML tag
        اگر ( *beg == '<' )
            واپس جھوٹ;

        // find end of the row
        جب( cv_isprint(*ptr) )
            ++ptr;
        اگر ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        واپس سچ;
    }

    حرف* parse_قدر( حرف* ptr, File_گرہ& گرہ, صحیح گہرائی = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( گہرائی > CV_PERSISTENCE_MAX_گہرائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_گرہ new_elem;
        منطقی have_space = سچ;
        صحیح value_type = گرہ.type();
        std::string کلید, key2, type_name;

        برائے(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            اگر( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = سچ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            اگر( c =='<' || c == '\0' )
            {
                صحیح لیبل_type = 0;
                صحیح elem_type = File_گرہ::NONE;

                اگر( d == '/' || c == '\0' )
                    توڑو;

                ptr = parse_لیبل( ptr, کلید, type_name, لیبل_type );

                اگر( لیبل_type == CV_XML_DIRECTIVE_لیبل )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                اگر( لیبل_type == CV_XML_خالی_خانے_لیبل )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(لیبل_type == CV_XML_OPENING_لیبل);

                /* for base64 string */
                منطقی ثنائی_string = جھوٹ;

                اگر( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    اگر( strcmp(tn, "str") == 0 )
                        elem_type = File_گرہ::STRING;
                    ورنہ اگر( strcmp( tn, "map" ) == 0 )
                        elem_type = File_گرہ::MAP;
                    ورنہ اگر( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_گرہ::SEQ;
                    ورنہ اگر( strcmp( tn, "binary") == 0)
                        ثنائی_string = سچ;
                }

                new_elem = fs->جوڑو_گرہ(گرہ, کلید, elem_type, 0);
                اگر (!ثنائی_string)
                    ptr = parse_قدر(ptr, new_elem, گہرائی + 1);
                ورنہ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_لیبل( ptr, key2, type_name, لیبل_type );
                اگر( لیبل_type != CV_XML_CLOSING_لیبل || key2 != کلید )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = سچ;
            }
            ورنہ
            {
                اگر( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_گرہ* elem = &گرہ;
                اگر( گرہ.type() != File_گرہ::NONE )
                {
                    fs->convert_مقصد_Collection( File_گرہ::SEQ, گرہ );
                    new_elem = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE, 0);
                    elem = &new_elem;
                }

                اگر( value_type != File_گرہ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    جب( cv_isdigit(*endptr) )
                        endptr++;
                    اگر( *endptr == '.' || *endptr == 'e' )
                    {
                        دوہرا fval = fs->strtod( ptr, &endptr );
                        elem->سیٹ_قدر(File_گرہ::REAL, &fval);
                    }
                    ورنہ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->سیٹ_قدر(File_گرہ::INT, &ival);
                    }

                    اگر( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
                }
                ورنہ
                {
                    // string
                    صحیح i = 0, len, is_quoted = 0;
                    اگر( c == '\"' )
                        is_quoted = 1;
                    ورنہ
                        --ptr;
                    strbuf[0] = '\0';

                    برائے( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();

                        اگر( !cv_isalnum(c) )
                        {
                            اگر( c == '\"' )
                            {
                                اگر( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                توڑو;
                            }
                            ورنہ اگر( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                اگر( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                توڑو;
                            }
                            ورنہ اگر( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ورنہ اگر( c == '&' )
                            {
                                اگر( *++ptr == '#' )
                                {
                                    صحیح val, بنیاد = 10;
                                    ptr++;
                                    اگر( *ptr == 'x' )
                                    {
                                        بنیاد = 16;
                                        ptr++;
                                    }
                                    val = (صحیح)strtol( ptr, &endptr, بنیاد );
                                    اگر( (غیرعلامتی)val > (غیرعلامتی)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                ورنہ
                                {
                                    endptr = ptr;
                                    کرو c = *++endptr;
                                    جب( cv_isalnum(c) );
                                    اگر( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحیح)(endptr - ptr);
                                    اگر( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ورنہ اگر( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ورنہ اگر( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ورنہ اگر( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ورنہ اگر( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ورنہ
                                    {
                                        اگر (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        حافظہ_نقل( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
                            }
                        }
                        اگر (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->سیٹ_قدر(File_گرہ::STRING, strbuf, i);
                }

                اگر( value_type != File_گرہ::NONE && value_type != File_گرہ::SEQ && value_type != File_گرہ::MAP )
                    توڑو;
                have_space = جھوٹ;
            }
        }
        fs->finalizeCollection(گرہ);

        واپس ptr;
    }

    حرف* parse_لیبل( حرف* ptr, std::string& لیبل_name,
                    std::string& type_name, صحیح& لیبل_type )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        اگر( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        اگر( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();

        اگر( cv_isalnum(*ptr) || *ptr == '_' )
            لیبل_type = CV_XML_OPENING_لیبل;
        ورنہ اگر( *ptr == '/' )
        {
            لیبل_type = CV_XML_CLOSING_لیبل;
            ptr++;
        }
        ورنہ اگر( *ptr == '?' )
        {
            لیبل_type = CV_XML_سرخی_لیبل;
            ptr++;
        }
        ورنہ اگر( *ptr == '!' )
        {
            لیبل_type = CV_XML_DIRECTIVE_لیبل;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ورنہ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        لیبل_name.clear();
        type_name.clear();

        برائے(;;)
        {
            حرف c, *endptr;
            اگر( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            کرو c = *++endptr;
            جب( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();

            اگر( لیبل_name.empty() )
                لیبل_name = attrname;
            ورنہ
            {
                اگر( لیبل_type == CV_XML_CLOSING_لیبل )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                اگر( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_لیبل );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    اگر( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                اگر( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_لیبل );
                    اگر(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    اگر( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                برائے(;;)
                {
                    c = *endptr++;
                    اگر( c == quote )
                        توڑو;
                    اگر( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                اگر( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقی have_space = cv_isspace(c) || c == '\0';

            اگر( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_لیبل );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            اگر( c == '>' )
            {
                اگر( لیبل_type == CV_XML_سرخی_لیبل )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                توڑو;
            }
            ورنہ اگر( c == '?' && لیبل_type == CV_XML_سرخی_لیبل )
            {
                اگر( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                توڑو;
            }
            ورنہ اگر( c == '/' && ptr[1] == '>' && لیبل_type == CV_XML_OPENING_لیبل )  // FIXIT ptr[1] - out of bounds read without check
            {
                لیبل_type = CV_XML_خالی_خانے_لیبل;
                ptr += 2;
                توڑو;
            }

            اگر( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        واپس ptr;
    }

    منطقی parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string کلید, key2, type_name;
        صحیح لیبل_type = 0;
        منطقی ok = جھوٹ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_لیبل );
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_لیبل( ptr, کلید, type_name, لیبل_type );
        File_گرہ جڑ_collection(fs->getFS(), 0, 0);

        جب( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            اگر (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            اگر( *ptr != '\0' )
            {
                ptr = parse_لیبل( ptr, کلید, type_name, لیبل_type );
                اگر( لیبل_type != CV_XML_OPENING_لیبل || کلید != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_گرہ جڑ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::MAP, 0);
                ptr = parse_قدر( ptr, جڑ );
                ptr = parse_لیبل( ptr, key2, type_name, لیبل_type );
                اگر( لیبل_type != CV_XML_CLOSING_لیبل || کلید != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = سچ;
            }
        }
        CV_Assert( fs->eof() );
        واپس ok;
    }

    FileStorage_API* fs;
    حرف strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    واپس makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    واپس makePtr<XMLParser>(fs);
}

}
