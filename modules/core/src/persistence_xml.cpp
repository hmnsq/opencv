// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

تعداد
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_نښان = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_نښان = 1,
    CV_XML_CLOSING_نښان = 2,
    CV_XML_تشه_نښان = 3,
    CV_XML_سرلیک_نښان = 4,
    CV_XML_DIRECTIVE_نښان = 5
};

نطاق cv
{

صنف XMLEmitter : عام FileStorageEmitter
{
عام:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازي ~XMLEmitter() {}

    فراغ write_نښان( ثابت حرف* کلی, صحيح نښان_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->بفر_Ptr();
        صحيح i, len = 0;
        FStruct_معلومات& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( کلی && کلی[0] == '\0' )
            کلی = 0;

        إذا( نښان_type == CV_XML_OPENING_نښان || نښان_type == CV_XML_تشه_نښان )
        {
            إذا( File_غوټه::isCollection(struct_flags) )
            {
                إذا( File_غوټه::isMap(struct_flags) ^ (کلی != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وإلا
            {
                struct_flags = File_غوټه::تشه_2 + (کلی ? File_غوټه::MAP : File_غوټه::SEQ);
                //fs->is_first = 0;
            }

            إذا( !File_غوټه::is_تشه_Collection(struct_flags) )
                ptr = fs->flush();
        }

        إذا( !کلی )
            کلی = "_";
        وإلا إذا( کلی[0] == '_' && کلی[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)مزي_اوږدوالی( کلی );
        *ptr++ = '<';
        إذا( نښان_type == CV_XML_CLOSING_نښان )
        {
            إذا( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        إذا( !cv_isalpha(کلی[0]) && کلی[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_بفر( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = کلی[i];
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

            حافظه_نقل( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            إذا( len1 > 0 )
                حافظه_نقل( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        إذا( نښان_type == CV_XML_تشه_نښان )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->سېټ_بفر_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_غوټه::تشه_2;
    }

    FStruct_معلومات پیل_Write_Struct(ثابت FStruct_معلومات& پلار, ثابت حرف* کلی,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        إذا( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_نښان( کلی, CV_XML_OPENING_نښان, attrlist );

        FStruct_معلومات current_struct;
        current_struct.نښان = کلی ? std::string(کلی) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = پلار.indent + CV_XML_INDENT;

        أرجع current_struct;
    }

    فراغ پای_ټکی_Write_Struct(ثابت FStruct_معلومات& current_struct)
    {
        write_نښان( current_struct.نښان.c_str(), CV_XML_CLOSING_نښان );
    }

    فراغ اكتب(ثابت حرف* کلی, صحيح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلی, ptr);
    }

    فراغ اكتب(ثابت حرف* کلی, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, ریښتیا );
        writeScalar( کلی, ptr);
    }

    فراغ اكتب( ثابت حرف* کلی, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلی, fs::double_منزل_String( buf, حجم(buf), value, خطأ ) );
    }

    فراغ اكتب(ثابت حرف* کلی, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* معلومات = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)مزي_اوږدوالی(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            معلومات = buf;
            *معلومات++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( (uchar)c >= 128 || c == ' ' )
                {
                    *معلومات++ = c;
                    need_quote = ریښتیا;
                }
                وإلا إذا( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *معلومات++ = '&';
                    إذا( c == '<' )
                    {
                        حافظه_نقل(معلومات, "lt", 2);
                        معلومات += 2;
                    }
                    وإلا إذا( c == '>' )
                    {
                        حافظه_نقل(معلومات, "gt", 2);
                        معلومات += 2;
                    }
                    وإلا إذا( c == '&' )
                    {
                        حافظه_نقل(معلومات, "amp", 3);
                        معلومات += 3;
                    }
                    وإلا إذا( c == '\'' )
                    {
                        حافظه_نقل(معلومات, "apos", 4);
                        معلومات += 4;
                    }
                    وإلا إذا( c == '\"' )
                    {
                        حافظه_نقل( معلومات, "quot", 4);
                        معلومات += 4;
                    }
                    وإلا
                    {
                        snprintf( معلومات, حجم(buf) - (معلومات - buf), "#x%02x", (uchar)c );
                        معلومات += 4;
                    }
                    *معلومات++ = ';';
                    need_quote = 1;
                }
                وإلا
                    *معلومات++ = c;
            }
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ریښتیا;

            إذا( need_quote )
                *معلومات++ = '\"';
            len = (صحيح)(معلومات - buf) - !need_quote;
            *معلومات++ = '\0';
            معلومات = buf + (صحيح)!need_quote;
        }

        writeScalar( کلی, معلومات );
    }

    فراغ writeScalar(ثابت حرف* کلی, ثابت حرف* معلومات)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_د_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_منزل_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_د_writing_base64() == FileStorage_API::دننه_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح len = (صحيح)مزي_اوږدوالی(معلومات);
        إذا( کلی && *کلی == '\0' )
            کلی = 0;

        FStruct_معلومات& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( File_غوټه::isMap(struct_flags) ||
           (!File_غوټه::isCollection(struct_flags) && کلی) )
        {
            write_نښان( کلی, CV_XML_OPENING_نښان );
            حرف* ptr = fs->resize_Write_بفر( fs->بفر_Ptr(), len );
            حافظه_نقل( ptr, معلومات, len );
            fs->سېټ_بفر_Ptr( ptr + len );
            write_نښان( کلی, CV_XML_CLOSING_نښان );
        }
        وإلا
        {
            حرف* ptr = fs->بفر_Ptr();
            صحيح new_دننه_واټن = (صحيح)(ptr - fs->بفر_پیل()) + len;

            إذا( کلی )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_غوټه::SEQ;

            إذا( (new_دننه_واټن > fs->wrapMargin() && new_دننه_واټن - current_struct.indent > 10) ||
               (ptr > fs->بفر_پیل() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وإلا إذا( ptr > fs->بفر_پیل() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            حافظه_نقل( ptr, معلومات, len );
            fs->سېټ_بفر_Ptr(ptr + len);
        }
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        FStruct_معلومات& current_struct = fs->getCurrentStruct();
        صحيح len;
        صحيح multiline;
        ثابت حرف* eol;
        حرف* ptr;

        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        إذا( مزي_پلټنه(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)مزي_اوږدوالی(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->بفر_Ptr();

        إذا( multiline || !eol_comment || fs->بفر_پای_ټکی_2() - ptr < len + 5 )
            ptr = fs->flush();
        وإلا إذا( ptr > fs->بفر_پیل() + current_struct.indent )
            *ptr++ = ' ';

        إذا( !multiline )
        {
            ptr = fs->resize_Write_بفر( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحيح)مزي_اوږدوالی(ptr);
        }
        وإلا
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->سېټ_بفر_Ptr(ptr + len);
        ptr = fs->flush();

        إذا( multiline )
        {
            طالما( comment )
            {
                إذا( eol )
                {
                    ptr = fs->resize_Write_بفر( ptr, (صحيح)(eol - comment) + 1 );
                    حافظه_نقل( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                وإلا
                {
                    len = (صحيح)مزي_اوږدوالی(comment);
                    ptr = fs->resize_Write_بفر( ptr, len );
                    حافظه_نقل( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->سېټ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->سېټ_بفر_Ptr(ptr + 3);
            fs->flush();
        }
    }

    فراغ پیل_بل_بهیر()
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

    مجازي ~XMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح ډېرپېښ )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            إذا( ډېرپېښ == CV_XML_INSIDE_COMMENT )
            {
                افعل c = *++ptr;
                طالما( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                إذا( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    ډېرپېښ = 0;
                    ptr += 3;
                }
            }
            وإلا إذا( ډېرپېښ == CV_XML_INSIDE_DIRECTIVE )
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
                    إذا( ډېرپېښ != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    ډېرپېښ = CV_XML_INSIDE_COMMENT;
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

    منطقي get_Base64_لیکه(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_نښان);
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
        أرجع ریښتیا;
    }

    حرف* parse_ارزښت( حرف* ptr, File_غوټه& غوټه, صحيح ژوروالی = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( ژوروالی > CV_PERSISTENCE_MAX_ژوروالی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_غوټه new_elem;
        منطقي have_space = ریښتیا;
        صحيح value_type = غوټه.type();
        std::string کلی, key2, type_name;

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
                have_space = ریښتیا;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            إذا( c =='<' || c == '\0' )
            {
                صحيح نښان_type = 0;
                صحيح elem_type = File_غوټه::NONE;

                إذا( d == '/' || c == '\0' )
                    اكسر;

                ptr = parse_نښان( ptr, کلی, type_name, نښان_type );

                إذا( نښان_type == CV_XML_DIRECTIVE_نښان )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                إذا( نښان_type == CV_XML_تشه_نښان )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(نښان_type == CV_XML_OPENING_نښان);

                /* for base64 string */
                منطقي دوګونی_string = خطأ;

                إذا( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    إذا( strcmp(tn, "str") == 0 )
                        elem_type = File_غوټه::STRING;
                    وإلا إذا( strcmp( tn, "map" ) == 0 )
                        elem_type = File_غوټه::MAP;
                    وإلا إذا( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_غوټه::SEQ;
                    وإلا إذا( strcmp( tn, "binary") == 0)
                        دوګونی_string = ریښتیا;
                }

                new_elem = fs->جمع_کړه_غوټه(غوټه, کلی, elem_type, 0);
                إذا (!دوګونی_string)
                    ptr = parse_ارزښت(ptr, new_elem, ژوروالی + 1);
                وإلا
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_نښان( ptr, key2, type_name, نښان_type );
                إذا( نښان_type != CV_XML_CLOSING_نښان || key2 != کلی )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ریښتیا;
            }
            وإلا
            {
                إذا( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_غوټه* elem = &غوټه;
                إذا( غوټه.type() != File_غوټه::NONE )
                {
                    fs->convert_منزل_Collection( File_غوټه::SEQ, غوټه );
                    new_elem = fs->جمع_کړه_غوټه(غوټه, std::string(), File_غوټه::NONE, 0);
                    elem = &new_elem;
                }

                إذا( value_type != File_غوټه::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    طالما( cv_isdigit(*endptr) )
                        endptr++;
                    إذا( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->سېټ_ارزښت(File_غوټه::REAL, &fval);
                    }
                    وإلا
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->سېټ_ارزښت(File_غوټه::INT, &ival);
                    }

                    إذا( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();
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
                        CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();

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
                                    صحيح val, بنسټ = 10;
                                    ptr++;
                                    إذا( *ptr == 'x' )
                                    {
                                        بنسټ = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, بنسټ );
                                    إذا( (بینښه)val > (بینښه)255 ||
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
                                        حافظه_نقل( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();
                            }
                        }
                        إذا (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->سېټ_ارزښت(File_غوټه::STRING, strbuf, i);
                }

                إذا( value_type != File_غوټه::NONE && value_type != File_غوټه::SEQ && value_type != File_غوټه::MAP )
                    اكسر;
                have_space = خطأ;
            }
        }
        fs->finalizeCollection(غوټه);

        أرجع ptr;
    }

    حرف* parse_نښان( حرف* ptr, std::string& نښان_name,
                    std::string& type_name, صحيح& نښان_type )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        إذا( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        إذا( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();

        إذا( cv_isalnum(*ptr) || *ptr == '_' )
            نښان_type = CV_XML_OPENING_نښان;
        وإلا إذا( *ptr == '/' )
        {
            نښان_type = CV_XML_CLOSING_نښان;
            ptr++;
        }
        وإلا إذا( *ptr == '?' )
        {
            نښان_type = CV_XML_سرلیک_نښان;
            ptr++;
        }
        وإلا إذا( *ptr == '!' )
        {
            نښان_type = CV_XML_DIRECTIVE_نښان;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        وإلا
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        نښان_name.clear();
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
            CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();

            إذا( نښان_name.empty() )
                نښان_name = attrname;
            وإلا
            {
                إذا( نښان_type == CV_XML_CLOSING_نښان )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                إذا( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_نښان );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    إذا( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                إذا( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_نښان );
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
                ptr = skipSpaces( ptr, CV_XML_INSIDE_نښان );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            إذا( c == '>' )
            {
                إذا( نښان_type == CV_XML_سرلیک_نښان )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            وإلا إذا( c == '?' && نښان_type == CV_XML_سرلیک_نښان )
            {
                إذا( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            وإلا إذا( c == '/' && ptr[1] == '>' && نښان_type == CV_XML_OPENING_نښان )  // FIXIT ptr[1] - out of bounds read without check
            {
                نښان_type = CV_XML_تشه_نښان;
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

        std::string کلی, key2, type_name;
        صحيح نښان_type = 0;
        منطقي ok = خطأ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_نښان );
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_نښان( ptr, کلی, type_name, نښان_type );
        File_غوټه ریښه_collection(fs->getFS(), 0, 0);

        طالما( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            إذا( *ptr != '\0' )
            {
                ptr = parse_نښان( ptr, کلی, type_name, نښان_type );
                إذا( نښان_type != CV_XML_OPENING_نښان || کلی != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_غوټه ریښه = fs->جمع_کړه_غوټه(ریښه_collection, std::string(), File_غوټه::MAP, 0);
                ptr = parse_ارزښت( ptr, ریښه );
                ptr = parse_نښان( ptr, key2, type_name, نښان_type );
                إذا( نښان_type != CV_XML_CLOSING_نښان || کلی != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ریښتیا;
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
