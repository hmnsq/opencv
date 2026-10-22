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

    FStruct_معلومات شروعات_Write_Struct(ثابت FStruct_معلومات& والد, ثابت حرف* ڪنجي,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* معلومات = 0;

        إذا ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ڳنڍ::TYPE_نقاب|File_ڳنڍ::FLOW)) | File_ڳنڍ::خالي_جايون_2;
        إذا( !File_ڳنڍ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ڳنڍ::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            معلومات = buf;
        }
        وإلا إذا( File_ڳنڍ::isFlow(struct_flags))
        {
            حرف c = File_ڳنڍ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ڳنڍ::FLOW;

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

        writeScalar( ڪنجي, معلومات );

        FStruct_معلومات fsd;
        fsd.indent = والد.indent;
        fsd.flags = struct_flags;

        إذا( !File_ڳنڍ::isFlow(والد.flags) )
            fsd.indent += CV_YML_INDENT + File_ڳنڍ::isFlow(struct_flags);

        أرجع fsd;
    }

    فراغ پڇاڙي_Write_Struct(ثابت FStruct_معلومات& current_struct)
    {
        حرف* ptr;

        صحيح struct_flags = current_struct.flags;

        إذا( File_ڳنڍ::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            إذا( ptr > fs->بفر_شروعات() + current_struct.indent && !File_ڳنڍ::is_خالي_جايون_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ڳنڍ::isMap(struct_flags) ? '}' : ']';
            fs->سيٽ_بفر_Ptr(ptr);
        }
        وإلا إذا( File_ڳنڍ::is_خالي_جايون_Collection(struct_flags) )
        {
            ptr = fs->flush();
            يادگيري_نقل( ptr, File_ڳنڍ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->سيٽ_بفر_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    فراغ اكتب(ثابت حرف* ڪنجي, صحيح value)
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* ڪنجي, int64_t value)
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::itoa( value, buf, 10, سچ ));
    }

    فراغ اكتب( ثابت حرف* ڪنجي, مزدوج value )
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::double_منزل_String( buf, حجم(buf), value, خطأ ));
    }

    فراغ اكتب(ثابت حرف* ڪنجي, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* معلومات = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)لڙي_ڊيگهه(str);
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

        writeScalar( ڪنجي, معلومات);
    }

    فراغ writeScalar(ثابت حرف* ڪنجي, ثابت حرف* معلومات)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_جو_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_منزل_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_جو_writing_base64() == FileStorage_API::اندريون_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح i, keylen = 0;
        صحيح datalen = 0;
        حرف* ptr;

        FStruct_معلومات& current_struct = fs->getCurrentStruct();

        صحيح struct_flags = current_struct.flags;

        إذا( ڪنجي && ڪنجي[0] == '\0' )
            ڪنجي = 0;

        إذا( File_ڳنڍ::isCollection(struct_flags) )
        {
            إذا( (File_ڳنڍ::isMap(struct_flags) ^ (ڪنجي != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        وإلا
        {
            fs->سيٽ_Non_خالي_جايون();
            struct_flags = File_ڳنڍ::خالي_جايون_2 | (ڪنجي ? File_ڳنڍ::MAP : File_ڳنڍ::SEQ);
        }

        إذا( ڪنجي )
        {
            keylen = (صحيح)لڙي_ڊيگهه(ڪنجي);
            إذا( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            إذا( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        إذا( معلومات )
            datalen = (صحيح)لڙي_ڊيگهه(معلومات);

        إذا( File_ڳنڍ::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            إذا( !File_ڳنڍ::is_خالي_جايون_Collection(struct_flags) )
                *ptr++ = ',';
            صحيح new_وٿي = (صحيح)(ptr - fs->بفر_شروعات()) + keylen + datalen;
            إذا( new_وٿي > fs->wrapMargin() && new_وٿي - current_struct.indent > 10 )
            {
                fs->سيٽ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            ptr = fs->flush();
            إذا( !File_ڳنڍ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                إذا( معلومات )
                    *ptr++ = ' ';
            }
        }

        إذا( ڪنجي )
        {
            إذا( !cv_isalpha(ڪنجي[0]) && ڪنجي[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, keylen );

            لكل( i = 0; i < keylen; i++ )
            {
                حرف c = ڪنجي[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            إذا( !File_ڳنڍ::isFlow(struct_flags) && معلومات )
                *ptr++ = ' ';
        }

        إذا( معلومات )
        {
            ptr = fs->resize_Write_بفر( ptr, datalen );
            يادگيري_نقل( ptr, معلومات, datalen );
            ptr += datalen;
        }

        fs->سيٽ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_ڳنڍ::خالي_جايون_2;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = (صحيح)لڙي_ڊيگهه(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;
        حرف* ptr = fs->بفر_Ptr();

        إذا( !eol_comment || multiline ||
            fs->بفر_پڇاڙي_2() - ptr < len || ptr == fs->بفر_شروعات() )
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
                يادگيري_نقل( ptr, comment, eol - comment + 1 );
                fs->سيٽ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
            {
                len = (صحيح)لڙي_ڊيگهه(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                يادگيري_نقل( ptr, comment, len );
                fs->سيٽ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ شروعات_ايندڙ_وهڪرو()
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
                إذا( ptr - fs->بفر_شروعات() > max_comment_indent )
                    أرجع ptr;
                *ptr = '\0';
            }
            وإلا إذا( cv_isprint(*ptr) )
            {
                إذا( ptr - fs->بفر_شروعات() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                اكسر;
            }
            وإلا إذا( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                إذا( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->بفر_شروعات();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->سيٽ_Eof();
                    اكسر;
                }
                وإلا
                {
                    صحيح l = (صحيح)لڙي_ڊيگهه(ptr);
                    إذا( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            وإلا
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        أرجع ptr;
    }

    منطقي get_Base64_صف(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end)
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, وڌ_کان_وڌ_عدد);
        إذا (!ptr || !*ptr)
            أرجع خطأ; // end of file

        إذا (ptr - fs->بفر_شروعات() != indent)
            أرجع خطأ; // end of base64 data

        /* find end */
        طالما(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        إذا (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        أرجع سچ;
    }


    حرف* parse_ڪنجي( حرف* ptr, File_ڳنڍ& map_ڳنڍ, File_ڳنڍ& قدر_placeholder )
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

        قدر_placeholder = fs->جوڙيو_ڳنڍ(map_ڳنڍ, std::string(ptr, endptr - ptr), File_ڳنڍ::NONE);
        ptr = saveptr;

        أرجع ptr;
    }

    حرف* parse_قدر( حرف* ptr, File_ڳنڍ& ڳنڍ, صحيح min_indent, منطقي is_والد_flow, صحيح اونهائي = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( اونهائي > CV_PERSISTENCE_MAX_اونهائي )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحيح value_type = File_ڳنڍ::NONE;
        صحيح len;
        منطقي is_ٻٽو_string = خطأ;
        منطقي is_user = خطأ;

        إذا( c == '!' ) // handle explicit type specification
        {
            إذا( d == '!' || d == '^' )
            {
                ptr++;
                is_user = سچ;
                //value_type |= FileNode::USER;
            }
            إذا ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t heading_ڊيگهائي = لڙي_ڊيگهه(yamlTypeHeading);

                حرف* type_پڇاڙي_Ptr = ++ptr;

                افعل d = *++type_پڇاڙي_Ptr;
                طالما( cv_isprint(d) && d != ' ' && d != '>' );

                إذا ( d == '>' && (size_t)(type_پڇاڙي_Ptr - ptr) > heading_ڊيگهائي )
                {
                    إذا ( memcmp(ptr, yamlTypeHeading, heading_ڊيگهائي) == 0 )
                    {
                        *type_پڇاڙي_Ptr = ' ';
                        ptr += heading_ڊيگهائي - 1;
                        is_user = سچ;
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
                    value_type = File_ڳنڍ::STRING;
                وإلا إذا( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ڳنڍ::INT;
                وإلا إذا( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ڳنڍ::SEQ;
                وإلا إذا( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ڳنڍ::MAP;
            }
            وإلا إذا( len == 5 && !is_user )
            {
                إذا( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ڳنڍ::REAL;
            }
            وإلا إذا (len == 6 && is_user)
            {
                إذا( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ڳنڍ::SEQ;
                    is_ٻٽو_string = سچ;

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
            ptr = skipSpaces( endptr, min_indent, وڌ_کان_وڌ_عدد );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            إذا( !is_user )
            {
                إذا (value_type == File_ڳنڍ::STRING && c != '\'' && c != '\"')
                    اذهب force_string;
                إذا( value_type == File_ڳنڍ::INT )
                    اذهب force_int;
                إذا( value_type == File_ڳنڍ::REAL )
                    اذهب force_real;
            }
        }

        إذا (is_ٻٽو_string)
        {
            صحيح indent = static_cast<صحيح>(ptr - fs->بفر_شروعات());
            ptr = fs->parseBase64(ptr, indent, ڳنڍ);
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
                ڳنڍ.سيٽ_قدر(File_ڳنڍ::REAL, &fval);
            }
            وإلا
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ڳنڍ.سيٽ_قدر(File_ڳنڍ::INT, &ival);
            }

            إذا( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
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

            ڳنڍ.سيٽ_قدر(File_ڳنڍ::STRING, buf, len);
        }
        وإلا إذا( c == '[' || c == '{' ) // collection as a flow
        {
            صحيح new_min_indent = min_indent + !is_والد_flow;
            صحيح struct_type = c == '{' ? File_ڳنڍ::MAP : File_ڳنڍ::SEQ;
            صحيح nelems = 0;

            fs->convert_منزل_Collection(struct_type, ڳنڍ);
            d = c == '[' ? ']' : '}';

            لكل( ++ptr ;; nelems++ )
            {
                File_ڳنڍ elem;

                ptr = skipSpaces( ptr, new_min_indent, وڌ_کان_وڌ_عدد );
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
                    ptr = skipSpaces( ptr + 1, new_min_indent, وڌ_کان_وڌ_عدد );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                إذا( struct_type == File_ڳنڍ::MAP )
                {
                    ptr = parse_ڪنجي( ptr, ڳنڍ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, وڌ_کان_وڌ_عدد );
                }
                وإلا
                {
                    إذا( *ptr == ']' )
                        اكسر;
                    elem = fs->جوڙيو_ڳنڍ(ڳنڍ, std::string(), File_ڳنڍ::NONE);
                }
                ptr = parse_قدر( ptr, elem, new_min_indent, سچ, اونهائي + 1 );
            }
            fs->finalizeCollection(ڳنڍ);
        }
        وإلا
        {
            صحيح indent, struct_type;

            إذا( is_والد_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                إذا( !is_والد_flow )
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
                      (!is_والد_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_والد_flow || c != ':' || value_type == File_ڳنڍ::STRING));

                إذا( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                إذا( is_والد_flow || c != ':' )
                {
                    حرف* str_پڇاڙي = endptr;
                    افعل c = *--str_پڇاڙي;
                    طالما( str_پڇاڙي > ptr && c == ' ' );
                    str_پڇاڙي++;
                    إذا ((fs->getFormat() & FileStorage::FORMAT_نقاب) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_پڇاڙي - ptr;
                        إذا (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ڳنڍ.سيٽ_قدر(File_ڳنڍ::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ڳنڍ.سيٽ_قدر(File_ڳنڍ::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            أرجع ptr;
                        }
                    }

                    ڳنڍ.سيٽ_قدر(File_ڳنڍ::STRING, ptr, (صحيح)(str_پڇاڙي - ptr));
                    ptr = endptr;
                    أرجع ptr;
                }
                struct_type = File_ڳنڍ::MAP;
            }
            وإلا
                struct_type = File_ڳنڍ::SEQ;

            fs->convert_منزل_Collection( struct_type, ڳنڍ );
            indent = (صحيح)(ptr - fs->بفر_شروعات());

            لكل(;;)
            {
                File_ڳنڍ elem;

                إذا( struct_type == File_ڳنڍ::MAP )
                {
                    ptr = parse_ڪنجي( ptr, ڳنڍ, elem );
                }
                وإلا
                {
                    c = *ptr++;
                    إذا( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->جوڙيو_ڳنڍ(ڳنڍ, std::string(), File_ڳنڍ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, وڌ_کان_وڌ_عدد );
                ptr = parse_قدر( ptr, elem, indent + 1, خطأ, اونهائي + 1 );
                ptr = skipSpaces( ptr, 0, وڌ_کان_وڌ_عدد );
                إذا( ptr - fs->بفر_شروعات() != indent )
                {
                    إذا( ptr - fs->بفر_شروعات() < indent )
                        اكسر;
                    وإلا
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                إذا( memcmp( ptr, "...", 3 ) == 0 )
                    اكسر;
            }
            fs->finalizeCollection(ڳنڍ);
        }

        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقي first = سچ;
        منطقي ok = سچ;
        File_ڳنڍ پاڙ_collection(fs->getFS(), 0, 0);

        لكل(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            لكل(;;)
            {
                ptr = skipSpaces( ptr, 0, وڌ_کان_وڌ_عدد );
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
                        إذا ((fs->getFormat() & FileStorage::FORMAT_نقاب) == FileStorage::FORMAT_YAML_1_0)
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
                ptr = skipSpaces( ptr, 0, وڌ_کان_وڌ_عدد );
            إذا( !ptr || !ptr[0] )
                اكسر;
            إذا( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ڳنڍ پاڙ_ڳنڍ = fs->جوڙيو_ڳنڍ(پاڙ_collection, std::string(), File_ڳنڍ::NONE);

                ptr = parse_قدر( ptr, پاڙ_ڳنڍ, 0, خطأ );
                إذا( !پاڙ_ڳنڍ.isMap() && !پاڙ_ڳنڍ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, وڌ_کان_وڌ_عدد );
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
