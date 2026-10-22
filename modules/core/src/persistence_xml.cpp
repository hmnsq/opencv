// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

تعداد
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_ٹیگ = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_ٹیگ = 1,
    CV_XML_CLOSING_ٹیگ = 2,
    CV_XML_خالی_تھاواں_ٹیگ = 3,
    CV_XML_سرخی_ٹیگ = 4,
    CV_XML_DIRECTIVE_ٹیگ = 5
};

نطاق cv
{

صنف XMLEmitter : عام FileStorageEmitter
{
عام:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~XMLEmitter() {}

    فراغ write_ٹیگ( ثابت حرف* کلید, صحيح ٹیگ_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->بفر_Ptr();
        صحيح i, len = 0;
        FStruct_انکڑے& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( کلید && کلید[0] == '\0' )
            کلید = 0;

        إذا( ٹیگ_type == CV_XML_OPENING_ٹیگ || ٹیگ_type == CV_XML_خالی_تھاواں_ٹیگ )
        {
            إذا( File_گرہ::isCollection(struct_flags) )
            {
                إذا( File_گرہ::isMap(struct_flags) ^ (کلید != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وإلا
            {
                struct_flags = File_گرہ::خالی_تھاواں_2 + (کلید ? File_گرہ::MAP : File_گرہ::SEQ);
                //fs->is_first = 0;
            }

            إذا( !File_گرہ::is_خالی_تھاواں_Collection(struct_flags) )
                ptr = fs->flush();
        }

        إذا( !کلید )
            کلید = "_";
        وإلا إذا( کلید[0] == '_' && کلید[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)لڑی_لمبائی( کلید );
        *ptr++ = '<';
        إذا( ٹیگ_type == CV_XML_CLOSING_ٹیگ )
        {
            إذا( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        إذا( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_بفر( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = کلید[i];
            إذا( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        صحيح nattr = (صحيح)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        لكل( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_بفر( ptr, (صحيح)(len0 + len1 + 4) );
            *ptr++ = ' ';

            حافظہ_نقل( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            إذا( len1 > 0 )
                حافظہ_نقل( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        إذا( ٹیگ_type == CV_XML_خالی_تھاواں_ٹیگ )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->سیٹ_بفر_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_گرہ::خالی_تھاواں_2;
    }

    FStruct_انکڑے شروع_Write_Struct(ثابت FStruct_انکڑے& ماپا, ثابت حرف* کلید,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        إذا( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_ٹیگ( کلید, CV_XML_OPENING_ٹیگ, attrlist );

        FStruct_انکڑے current_struct;
        current_struct.ٹیگ = کلید ? std::string(کلید) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = ماپا.indent + CV_XML_INDENT;

        أرجع current_struct;
    }

    فراغ سرا_Write_Struct(ثابت FStruct_انکڑے& current_struct)
    {
        write_ٹیگ( current_struct.ٹیگ.c_str(), CV_XML_CLOSING_ٹیگ );
    }

    فراغ اكتب(ثابت حرف* کلید, صحيح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلید, ptr);
    }

    فراغ اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, سچ );
        writeScalar( کلید, ptr);
    }

    فراغ اكتب( ثابت حرف* کلید, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_منزل_String( buf, حجم(buf), value, خطأ ) );
    }

    فراغ اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* انکڑے = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)لڑی_لمبائی(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            انکڑے = buf;
            *انکڑے++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( (uchar)c >= 128 || c == ' ' )
                {
                    *انکڑے++ = c;
                    need_quote = سچ;
                }
                وإلا إذا( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *انکڑے++ = '&';
                    إذا( c == '<' )
                    {
                        حافظہ_نقل(انکڑے, "lt", 2);
                        انکڑے += 2;
                    }
                    وإلا إذا( c == '>' )
                    {
                        حافظہ_نقل(انکڑے, "gt", 2);
                        انکڑے += 2;
                    }
                    وإلا إذا( c == '&' )
                    {
                        حافظہ_نقل(انکڑے, "amp", 3);
                        انکڑے += 3;
                    }
                    وإلا إذا( c == '\'' )
                    {
                        حافظہ_نقل(انکڑے, "apos", 4);
                        انکڑے += 4;
                    }
                    وإلا إذا( c == '\"' )
                    {
                        حافظہ_نقل( انکڑے, "quot", 4);
                        انکڑے += 4;
                    }
                    وإلا
                    {
                        snprintf( انکڑے, حجم(buf) - (انکڑے - buf), "#x%02x", (uchar)c );
                        انکڑے += 4;
                    }
                    *انکڑے++ = ';';
                    need_quote = 1;
                }
                وإلا
                    *انکڑے++ = c;
            }
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = سچ;

            إذا( need_quote )
                *انکڑے++ = '\"';
            len = (صحيح)(انکڑے - buf) - !need_quote;
            *انکڑے++ = '\0';
            انکڑے = buf + (صحيح)!need_quote;
        }

        writeScalar( کلید, انکڑے );
    }

    فراغ writeScalar(ثابت حرف* کلید, ثابت حرف* انکڑے)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_دا_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_منزل_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_دا_writing_base64() == FileStorage_API::وچ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح len = (صحيح)لڑی_لمبائی(انکڑے);
        إذا( کلید && *کلید == '\0' )
            کلید = 0;

        FStruct_انکڑے& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( File_گرہ::isMap(struct_flags) ||
           (!File_گرہ::isCollection(struct_flags) && کلید) )
        {
            write_ٹیگ( کلید, CV_XML_OPENING_ٹیگ );
            حرف* ptr = fs->resize_Write_بفر( fs->بفر_Ptr(), len );
            حافظہ_نقل( ptr, انکڑے, len );
            fs->سیٹ_بفر_Ptr( ptr + len );
            write_ٹیگ( کلید, CV_XML_CLOSING_ٹیگ );
        }
        وإلا
        {
            حرف* ptr = fs->بفر_Ptr();
            صحيح new_وتھ = (صحيح)(ptr - fs->بفر_شروع()) + len;

            إذا( کلید )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_گرہ::SEQ;

            إذا( (new_وتھ > fs->wrapMargin() && new_وتھ - current_struct.indent > 10) ||
               (ptr > fs->بفر_شروع() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وإلا إذا( ptr > fs->بفر_شروع() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            حافظہ_نقل( ptr, انکڑے, len );
            fs->سیٹ_بفر_Ptr(ptr + len);
        }
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        FStruct_انکڑے& current_struct = fs->getCurrentStruct();
        صحيح len;
        صحيح multiline;
        ثابت حرف* eol;
        حرف* ptr;

        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        إذا( لڑی_کھوج(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)لڑی_لمبائی(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->بفر_Ptr();

        إذا( multiline || !eol_comment || fs->بفر_سرا_2() - ptr < len + 5 )
            ptr = fs->flush();
        وإلا إذا( ptr > fs->بفر_شروع() + current_struct.indent )
            *ptr++ = ' ';

        إذا( !multiline )
        {
            ptr = fs->resize_Write_بفر( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحيح)لڑی_لمبائی(ptr);
        }
        وإلا
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->سیٹ_بفر_Ptr(ptr + len);
        ptr = fs->flush();

        إذا( multiline )
        {
            طالما( comment )
            {
                إذا( eol )
                {
                    ptr = fs->resize_Write_بفر( ptr, (صحيح)(eol - comment) + 1 );
                    حافظہ_نقل( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                وإلا
                {
                    len = (صحيح)لڑی_لمبائی(comment);
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

    فراغ شروع_اگلا_دھارا()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

محمي:
    FileStorage_API* fs;
};

صنف XMLParser : عام FileStorageParser
{
عام:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~XMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح منوال )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            إذا( منوال == CV_XML_INSIDE_COMMENT )
            {
                افعل c = *++ptr;
                طالما( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                إذا( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    منوال = 0;
                    ptr += 3;
                }
            }
            وإلا إذا( منوال == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                افعل
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    إذا( level < 0 )
                        أرجع ptr;
                } طالما( cv_isprint_or_tab(c) );
            }
            وإلا
            {
                افعل c = *++ptr;
                طالما( c == ' ' || c == '\t' );

                إذا( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    إذا( منوال != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    منوال = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                وإلا إذا( cv_isprint(c) )
                    اكسر;
            }

            إذا( !cv_isprint(*ptr) )
            {
                إذا( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                إذا( !ptr || *ptr == '\0' )
                    اكسر;
            }
        }
        أرجع ptr;
    }

    منطقي get_Base64_صف(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_ٹیگ);
        إذا( !ptr || !*ptr )
            أرجع خطأ;

        // closing XML tag
        إذا ( *beg == '<' )
            أرجع خطأ;

        // find end of the row
        طالما( cv_isprint(*ptr) )
            ++ptr;
        إذا ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        أرجع سچ;
    }

    حرف* parse_قدر( حرف* ptr, File_گرہ& گرہ, صحيح ڈونگھائی = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( ڈونگھائی > CV_PERSISTENCE_MAX_ڈونگھائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_گرہ new_elem;
        منطقي have_space = سچ;
        صحيح value_type = گرہ.type();
        std::string کلید, key2, type_name;

        لكل(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            إذا( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = سچ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            إذا( c =='<' || c == '\0' )
            {
                صحيح ٹیگ_type = 0;
                صحيح elem_type = File_گرہ::NONE;

                إذا( d == '/' || c == '\0' )
                    اكسر;

                ptr = parse_ٹیگ( ptr, کلید, type_name, ٹیگ_type );

                إذا( ٹیگ_type == CV_XML_DIRECTIVE_ٹیگ )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                إذا( ٹیگ_type == CV_XML_خالی_تھاواں_ٹیگ )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(ٹیگ_type == CV_XML_OPENING_ٹیگ);

                /* for base64 string */
                منطقي ثنائی_string = خطأ;

                إذا( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    إذا( strcmp(tn, "str") == 0 )
                        elem_type = File_گرہ::STRING;
                    وإلا إذا( strcmp( tn, "map" ) == 0 )
                        elem_type = File_گرہ::MAP;
                    وإلا إذا( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_گرہ::SEQ;
                    وإلا إذا( strcmp( tn, "binary") == 0)
                        ثنائی_string = سچ;
                }

                new_elem = fs->جوڑو_گرہ(گرہ, کلید, elem_type, 0);
                إذا (!ثنائی_string)
                    ptr = parse_قدر(ptr, new_elem, ڈونگھائی + 1);
                وإلا
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_ٹیگ( ptr, key2, type_name, ٹیگ_type );
                إذا( ٹیگ_type != CV_XML_CLOSING_ٹیگ || key2 != کلید )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = سچ;
            }
            وإلا
            {
                إذا( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_گرہ* elem = &گرہ;
                إذا( گرہ.type() != File_گرہ::NONE )
                {
                    fs->convert_منزل_Collection( File_گرہ::SEQ, گرہ );
                    new_elem = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE, 0);
                    elem = &new_elem;
                }

                إذا( value_type != File_گرہ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    طالما( cv_isdigit(*endptr) )
                        endptr++;
                    إذا( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->سیٹ_قدر(File_گرہ::REAL, &fval);
                    }
                    وإلا
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->سیٹ_قدر(File_گرہ::INT, &ival);
                    }

                    إذا( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
                }
                وإلا
                {
                    // string
                    صحيح i = 0, len, is_quoted = 0;
                    إذا( c == '\"' )
                        is_quoted = 1;
                    وإلا
                        --ptr;
                    strbuf[0] = '\0';

                    لكل( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();

                        إذا( !cv_isalnum(c) )
                        {
                            إذا( c == '\"' )
                            {
                                إذا( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                اكسر;
                            }
                            وإلا إذا( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                إذا( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                اكسر;
                            }
                            وإلا إذا( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            وإلا إذا( c == '&' )
                            {
                                إذا( *++ptr == '#' )
                                {
                                    صحيح val, ادھار = 10;
                                    ptr++;
                                    إذا( *ptr == 'x' )
                                    {
                                        ادھار = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, ادھار );
                                    إذا( (بنانشانی)val > (بنانشانی)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                وإلا
                                {
                                    endptr = ptr;
                                    افعل c = *++endptr;
                                    طالما( cv_isalnum(c) );
                                    إذا( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحيح)(endptr - ptr);
                                    إذا( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    وإلا إذا( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    وإلا إذا( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    وإلا إذا( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    وإلا إذا( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    وإلا
                                    {
                                        إذا (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        حافظہ_نقل( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
                            }
                        }
                        إذا (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->سیٹ_قدر(File_گرہ::STRING, strbuf, i);
                }

                إذا( value_type != File_گرہ::NONE && value_type != File_گرہ::SEQ && value_type != File_گرہ::MAP )
                    اكسر;
                have_space = خطأ;
            }
        }
        fs->finalizeCollection(گرہ);

        أرجع ptr;
    }

    حرف* parse_ٹیگ( حرف* ptr, std::string& ٹیگ_name,
                    std::string& type_name, صحيح& ٹیگ_type )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        إذا( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        إذا( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();

        إذا( cv_isalnum(*ptr) || *ptr == '_' )
            ٹیگ_type = CV_XML_OPENING_ٹیگ;
        وإلا إذا( *ptr == '/' )
        {
            ٹیگ_type = CV_XML_CLOSING_ٹیگ;
            ptr++;
        }
        وإلا إذا( *ptr == '?' )
        {
            ٹیگ_type = CV_XML_سرخی_ٹیگ;
            ptr++;
        }
        وإلا إذا( *ptr == '!' )
        {
            ٹیگ_type = CV_XML_DIRECTIVE_ٹیگ;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        وإلا
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        ٹیگ_name.clear();
        type_name.clear();

        لكل(;;)
        {
            حرف c, *endptr;
            إذا( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            افعل c = *++endptr;
            طالما( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();

            إذا( ٹیگ_name.empty() )
                ٹیگ_name = attrname;
            وإلا
            {
                إذا( ٹیگ_type == CV_XML_CLOSING_ٹیگ )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                إذا( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ٹیگ );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    إذا( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                إذا( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ٹیگ );
                    إذا(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    إذا( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                لكل(;;)
                {
                    c = *endptr++;
                    إذا( c == quote )
                        اكسر;
                    إذا( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                إذا( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقي have_space = cv_isspace(c) || c == '\0';

            إذا( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_ٹیگ );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            إذا( c == '>' )
            {
                إذا( ٹیگ_type == CV_XML_سرخی_ٹیگ )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            وإلا إذا( c == '?' && ٹیگ_type == CV_XML_سرخی_ٹیگ )
            {
                إذا( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            وإلا إذا( c == '/' && ptr[1] == '>' && ٹیگ_type == CV_XML_OPENING_ٹیگ )  // FIXIT ptr[1] - out of bounds read without check
            {
                ٹیگ_type = CV_XML_خالی_تھاواں_ٹیگ;
                ptr += 2;
                اكسر;
            }

            إذا( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        أرجع ptr;
    }

    منطقي parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string کلید, key2, type_name;
        صحيح ٹیگ_type = 0;
        منطقي ok = خطأ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_ٹیگ );
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_ٹیگ( ptr, کلید, type_name, ٹیگ_type );
        File_گرہ جڑ_collection(fs->getFS(), 0, 0);

        طالما( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            إذا( *ptr != '\0' )
            {
                ptr = parse_ٹیگ( ptr, کلید, type_name, ٹیگ_type );
                إذا( ٹیگ_type != CV_XML_OPENING_ٹیگ || کلید != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_گرہ جڑ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::MAP, 0);
                ptr = parse_قدر( ptr, جڑ );
                ptr = parse_ٹیگ( ptr, key2, type_name, ٹیگ_type );
                إذا( ٹیگ_type != CV_XML_CLOSING_ٹیگ || کلید != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = سچ;
            }
        }
        CV_Assert( fs->eof() );
        أرجع ok;
    }

    FileStorage_API* fs;
    حرف strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    أرجع makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    أرجع makePtr<XMLParser>(fs);
}

}
