// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

تعداد
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

نطاق cv
{

صنف YAMLEmitter : عام FileStorageEmitter
{
عام:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازي ~YAMLEmitter() {}

    FStruct_معلومات پیل_Write_Struct(ثابت FStruct_معلومات& پلار, ثابت حرف* کلی,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* معلومات = 0;

        إذا ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_غوټه::TYPE_ماسک|File_غوټه::FLOW)) | File_غوټه::تشه_2;
        إذا( !File_غوټه::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_غوټه::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            معلومات = buf;
        }
        وإلا إذا( File_غوټه::isFlow(struct_flags))
        {
            حرف c = File_غوټه::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_غوټه::FLOW;

            إذا( type_name )
                snprintf( buf, حجم(buf), "!!%s %c", type_name, c );
            وإلا
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            معلومات = buf;
        }
        وإلا إذا( type_name )
        {
            snprintf( buf, حجم(buf), "!!%s", type_name );
            معلومات = buf;
        }

        writeScalar( کلی, معلومات );

        FStruct_معلومات fsd;
        fsd.indent = پلار.indent;
        fsd.flags = struct_flags;

        إذا( !File_غوټه::isFlow(پلار.flags) )
            fsd.indent += CV_YML_INDENT + File_غوټه::isFlow(struct_flags);

        أرجع fsd;
    }

    فراغ پای_ټکی_Write_Struct(ثابت FStruct_معلومات& current_struct)
    {
        حرف* ptr;

        صحيح struct_flags = current_struct.flags;

        إذا( File_غوټه::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            إذا( ptr > fs->بفر_پیل() + current_struct.indent && !File_غوټه::is_تشه_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_غوټه::isMap(struct_flags) ? '}' : ']';
            fs->سېټ_بفر_Ptr(ptr);
        }
        وإلا إذا( File_غوټه::is_تشه_Collection(struct_flags) )
        {
            ptr = fs->flush();
            حافظه_نقل( ptr, File_غوټه::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->سېټ_بفر_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    فراغ اكتب(ثابت حرف* کلی, صحيح value)
    {
        حرف buf[128];
        writeScalar( کلی, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* کلی, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلی, fs::itoa( value, buf, 10, ریښتیا ));
    }

    فراغ اكتب( ثابت حرف* کلی, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلی, fs::double_منزل_String( buf, حجم(buf), value, خطأ ));
    }

    فراغ اكتب(ثابت حرف* کلی, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* معلومات = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)مزي_اوږدوالی(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = quote || len == 0 || str[0] == ' ';
            معلومات = buf;
            *معلومات++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                إذا( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *معلومات++ = '\\';
                    إذا( cv_isprint(c) )
                        *معلومات++ = c;
                    وإلا إذا( c == '\n' )
                        *معلومات++ = 'n';
                    وإلا إذا( c == '\r' )
                        *معلومات++ = 'r';
                    وإلا إذا( c == '\t' )
                        *معلومات++ = 't';
                    وإلا
                    {
                        snprintf( معلومات, حجم(buf) - (معلومات - buf), "x%02x", c );
                        معلومات += 3;
                    }
                }
                وإلا
                    *معلومات++ = c;
            }
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            إذا( need_quote )
                *معلومات++ = '\"';
            *معلومات++ = '\0';
            معلومات = buf + !need_quote;
        }

        writeScalar( کلی, معلومات);
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

        صحيح i, keylen = 0;
        صحيح datalen = 0;
        حرف* ptr;

        FStruct_معلومات& current_struct = fs->getCurrentStruct();

        صحيح struct_flags = current_struct.flags;

        إذا( کلی && کلی[0] == '\0' )
            کلی = 0;

        إذا( File_غوټه::isCollection(struct_flags) )
        {
            إذا( (File_غوټه::isMap(struct_flags) ^ (کلی != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        وإلا
        {
            fs->سېټ_Non_تشه();
            struct_flags = File_غوټه::تشه_2 | (کلی ? File_غوټه::MAP : File_غوټه::SEQ);
        }

        إذا( کلی )
        {
            keylen = (صحيح)مزي_اوږدوالی(کلی);
            إذا( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            إذا( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        إذا( معلومات )
            datalen = (صحيح)مزي_اوږدوالی(معلومات);

        إذا( File_غوټه::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            إذا( !File_غوټه::is_تشه_Collection(struct_flags) )
                *ptr++ = ',';
            صحيح new_دننه_واټن = (صحيح)(ptr - fs->بفر_پیل()) + keylen + datalen;
            إذا( new_دننه_واټن > fs->wrapMargin() && new_دننه_واټن - current_struct.indent > 10 )
            {
                fs->سېټ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            ptr = fs->flush();
            إذا( !File_غوټه::isMap(struct_flags) )
            {
                *ptr++ = '-';
                إذا( معلومات )
                    *ptr++ = ' ';
            }
        }

        إذا( کلی )
        {
            إذا( !cv_isalpha(کلی[0]) && کلی[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, keylen );

            لكل( i = 0; i < keylen; i++ )
            {
                حرف c = کلی[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            إذا( !File_غوټه::isFlow(struct_flags) && معلومات )
                *ptr++ = ' ';
        }

        إذا( معلومات )
        {
            ptr = fs->resize_Write_بفر( ptr, datalen );
            حافظه_نقل( ptr, معلومات, datalen );
            ptr += datalen;
        }

        fs->سېټ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_غوټه::تشه_2;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = (صحيح)مزي_اوږدوالی(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;
        حرف* ptr = fs->بفر_Ptr();

        إذا( !eol_comment || multiline ||
            fs->بفر_پای_ټکی_2() - ptr < len || ptr == fs->بفر_پیل() )
            ptr = fs->flush();
        وإلا
            *ptr++ = ' ';

        طالما( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            إذا( eol )
            {
                ptr = fs->resize_Write_بفر( ptr, (صحيح)(eol - comment) + 1 );
                حافظه_نقل( ptr, comment, eol - comment + 1 );
                fs->سېټ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
            {
                len = (صحيح)مزي_اوږدوالی(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                حافظه_نقل( ptr, comment, len );
                fs->سېټ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ پیل_بل_بهیر()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محمي:
    FileStorage_API* fs;
};


صنف YAMLParser : عام FileStorageParser
{
عام:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازي ~YAMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح min_indent, صحيح max_comment_indent )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        لكل(;;)
        {
            طالما( *ptr == ' ' )
                ptr++;
            إذا( *ptr == '#' )
            {
                إذا( ptr - fs->بفر_پیل() > max_comment_indent )
                    أرجع ptr;
                *ptr = '\0';
            }
            وإلا إذا( cv_isprint(*ptr) )
            {
                إذا( ptr - fs->بفر_پیل() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                اكسر;
            }
            وإلا إذا( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                إذا( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->بفر_پیل();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->سېټ_Eof();
                    اكسر;
                }
                وإلا
                {
                    صحيح l = (صحيح)مزي_اوږدوالی(ptr);
                    إذا( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            وإلا
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        أرجع ptr;
    }

    منطقي get_Base64_لیکه(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end)
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, لوی_عدد);
        إذا (!ptr || !*ptr)
            أرجع خطأ; // end of file

        إذا (ptr - fs->بفر_پیل() != indent)
            أرجع خطأ; // end of base64 data

        /* find end */
        طالما(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        إذا (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        أرجع ریښتیا;
    }


    حرف* parse_کلی( حرف* ptr, File_غوټه& map_غوټه, File_غوټه& ارزښت_placeholder )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        حرف c;
        حرف *endptr = ptr - 1, *saveptr;

        إذا( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        افعل c = *++endptr;
        طالما( cv_isprint(c) && c != ':' );

        إذا( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        إذا( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        افعل c = *--endptr;
        طالما( c == ' ' );

        ++endptr;

        ارزښت_placeholder = fs->جمع_کړه_غوټه(map_غوټه, std::string(ptr, endptr - ptr), File_غوټه::NONE);
        ptr = saveptr;

        أرجع ptr;
    }

    حرف* parse_ارزښت( حرف* ptr, File_غوټه& غوټه, صحيح min_indent, منطقي is_پلار_flow, صحيح ژوروالی = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( ژوروالی > CV_PERSISTENCE_MAX_ژوروالی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحيح value_type = File_غوټه::NONE;
        صحيح len;
        منطقي is_دوګونی_string = خطأ;
        منطقي is_user = خطأ;

        إذا( c == '!' ) // handle explicit type specification
        {
            إذا( d == '!' || d == '^' )
            {
                ptr++;
                is_user = ریښتیا;
                //value_type |= FileNode::USER;
            }
            إذا ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t heading_اوږدتیا = مزي_اوږدوالی(yamlTypeHeading);

                حرف* type_پای_ټکی_Ptr = ++ptr;

                افعل d = *++type_پای_ټکی_Ptr;
                طالما( cv_isprint(d) && d != ' ' && d != '>' );

                إذا ( d == '>' && (size_t)(type_پای_ټکی_Ptr - ptr) > heading_اوږدتیا )
                {
                    إذا ( memcmp(ptr, yamlTypeHeading, heading_اوږدتیا) == 0 )
                    {
                        *type_پای_ټکی_Ptr = ' ';
                        ptr += heading_اوږدتیا - 1;
                        is_user = ریښتیا;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            افعل d = *++endptr;
            طالما( cv_isprint(d) && d != ' ' );
            len = (صحيح)(endptr - ptr);
            إذا( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            إذا( len == 3 && !is_user )
            {
                إذا( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_غوټه::STRING;
                وإلا إذا( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_غوټه::INT;
                وإلا إذا( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_غوټه::SEQ;
                وإلا إذا( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_غوټه::MAP;
            }
            وإلا إذا( len == 5 && !is_user )
            {
                إذا( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_غوټه::REAL;
            }
            وإلا إذا (len == 6 && is_user)
            {
                إذا( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_غوټه::SEQ;
                    is_دوګونی_string = ریښتیا;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    افعل {
                        d = *++endptr;
                        إذا (d == '|')
                            اكسر;
                    } طالما (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, لوی_عدد );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            إذا( !is_user )
            {
                إذا (value_type == File_غوټه::STRING && c != '\'' && c != '\"')
                    اذهب force_string;
                إذا( value_type == File_غوټه::INT )
                    اذهب force_int;
                إذا( value_type == File_غوټه::REAL )
                    اذهب force_real;
            }
        }

        إذا (is_دوګونی_string)
        {
            صحيح indent = static_cast<صحيح>(ptr - fs->بفر_پیل());
            ptr = fs->parseBase64(ptr, indent, غوټه);
        }
        وإلا إذا( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            طالما( cv_isdigit(*endptr) )
                endptr++;
            إذا( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                مزدوج fval = fs->strtod( ptr, &endptr );
                غوټه.سېټ_ارزښت(File_غوټه::REAL, &fval);
            }
            وإلا
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                غوټه.سېټ_ارزښت(File_غوټه::INT, &ival);
            }

            إذا( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_پای_ټکی_د_بفر_BUG_CPP();
        }
        وإلا إذا( c == '\'' || c == '\"' ) // an explicit string
        {
            إذا( c == '\'' )
                لكل( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    إذا( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    وإلا إذا( c == '\'' )
                    {
                        c = *++ptr;
                        إذا( c != '\'' )
                            اكسر;
                        buf[len++] = c;
                    }
                    وإلا
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            وإلا
                لكل( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    إذا( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    وإلا إذا( c == '\"' )
                    {
                        ++ptr;
                        اكسر;
                    }
                    وإلا إذا( c == '\\' )
                    {
                        d = *++ptr;
                        إذا( d == '\'' )
                            buf[len++] = d;
                        وإلا إذا( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        وإلا إذا( d == 'n' )
                            buf[len++] = '\n';
                        وإلا إذا( d == 'r' )
                            buf[len++] = '\r';
                        وإلا إذا( d == 't' )
                            buf[len++] = '\t';
                        وإلا إذا( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            صحيح val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (صحيح)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            إذا( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            وإلا
                            {
                                buf[len++] = (حرف)val;
                                ptr = endptr;
                            }
                        }
                    }
                    وإلا
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            إذا( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            غوټه.سېټ_ارزښت(File_غوټه::STRING, buf, len);
        }
        وإلا إذا( c == '[' || c == '{' ) // collection as a flow
        {
            صحيح new_min_indent = min_indent + !is_پلار_flow;
            صحيح struct_type = c == '{' ? File_غوټه::MAP : File_غوټه::SEQ;
            صحيح nelems = 0;

            fs->convert_منزل_Collection(struct_type, غوټه);
            d = c == '[' ? ']' : '}';

            لكل( ++ptr ;; nelems++ )
            {
                File_غوټه elem;

                ptr = skipSpaces( ptr, new_min_indent, لوی_عدد );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                إذا( *ptr == '}' || *ptr == ']' )
                {
                    إذا( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    اكسر;
                }

                إذا( nelems != 0 )
                {
                    إذا( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, لوی_عدد );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                إذا( struct_type == File_غوټه::MAP )
                {
                    ptr = parse_کلی( ptr, غوټه, elem );
                    ptr = skipSpaces( ptr, new_min_indent, لوی_عدد );
                }
                وإلا
                {
                    إذا( *ptr == ']' )
                        اكسر;
                    elem = fs->جمع_کړه_غوټه(غوټه, std::string(), File_غوټه::NONE);
                }
                ptr = parse_ارزښت( ptr, elem, new_min_indent, ریښتیا, ژوروالی + 1 );
            }
            fs->finalizeCollection(غوټه);
        }
        وإلا
        {
            صحيح indent, struct_type;

            إذا( is_پلار_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                إذا( !is_پلار_flow )
                {
                    إذا( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    إذا( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                افعل c = *++endptr;
                طالما( cv_isprint(c) &&
                      (!is_پلار_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_پلار_flow || c != ':' || value_type == File_غوټه::STRING));

                إذا( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                إذا( is_پلار_flow || c != ':' )
                {
                    حرف* str_پای_ټکی = endptr;
                    افعل c = *--str_پای_ټکی;
                    طالما( str_پای_ټکی > ptr && c == ' ' );
                    str_پای_ټکی++;
                    إذا ((fs->getFormat() & FileStorage::FORMAT_ماسک) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_پای_ټکی - ptr;
                        إذا (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            غوټه.سېټ_ارزښت(File_غوټه::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            غوټه.سېټ_ارزښت(File_غوټه::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            أرجع ptr;
                        }
                    }

                    غوټه.سېټ_ارزښت(File_غوټه::STRING, ptr, (صحيح)(str_پای_ټکی - ptr));
                    ptr = endptr;
                    أرجع ptr;
                }
                struct_type = File_غوټه::MAP;
            }
            وإلا
                struct_type = File_غوټه::SEQ;

            fs->convert_منزل_Collection( struct_type, غوټه );
            indent = (صحيح)(ptr - fs->بفر_پیل());

            لكل(;;)
            {
                File_غوټه elem;

                إذا( struct_type == File_غوټه::MAP )
                {
                    ptr = parse_کلی( ptr, غوټه, elem );
                }
                وإلا
                {
                    c = *ptr++;
                    إذا( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->جمع_کړه_غوټه(غوټه, std::string(), File_غوټه::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, لوی_عدد );
                ptr = parse_ارزښت( ptr, elem, indent + 1, خطأ, ژوروالی + 1 );
                ptr = skipSpaces( ptr, 0, لوی_عدد );
                إذا( ptr - fs->بفر_پیل() != indent )
                {
                    إذا( ptr - fs->بفر_پیل() < indent )
                        اكسر;
                    وإلا
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                إذا( memcmp( ptr, "...", 3 ) == 0 )
                    اكسر;
            }
            fs->finalizeCollection(غوټه);
        }

        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقي first = ریښتیا;
        منطقي ok = ریښتیا;
        File_غوټه ریښه_collection(fs->getFS(), 0, 0);

        لكل(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            لكل(;;)
            {
                ptr = skipSpaces( ptr, 0, لوی_عدد );
                إذا( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    اكسر;
                }

                إذا( *ptr == '%' )
                {
                    إذا( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        إذا ((fs->getFormat() & FileStorage::FORMAT_ماسک) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                وإلا إذا( *ptr == '-' )
                {
                    إذا( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        اكسر;
                    }
                    وإلا إذا( first )
                        اكسر;
                }
                وإلا إذا( cv_isalnum(*ptr) || *ptr=='_')
                {
                    إذا( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    اكسر;
                }
                وإلا إذا( fs->eof() )
                    اكسر;
                وإلا
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            إذا( ptr )
                ptr = skipSpaces( ptr, 0, لوی_عدد );
            إذا( !ptr || !ptr[0] )
                اكسر;
            إذا( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_غوټه ریښه_غوټه = fs->جمع_کړه_غوټه(ریښه_collection, std::string(), File_غوټه::NONE);

                ptr = parse_ارزښت( ptr, ریښه_غوټه, 0, خطأ );
                إذا( !ریښه_غوټه.isMap() && !ریښه_غوټه.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, لوی_عدد );
                إذا( !ptr )
                    اكسر;
            }

            إذا( fs->eof() )
                اكسر;
            ptr += 3;
            first = خطأ;
        }

        أرجع ok;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    أرجع makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    أرجع makePtr<YAMLParser>(fs);
}

}
