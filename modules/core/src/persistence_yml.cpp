// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

شمار
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

نامخلا cv
{

جماعت YAMLEmitter : عوامی FileStorageEmitter
{
عوامی:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~YAMLEmitter() {}

    FStruct_ڈیٹا آغاز_Write_Struct(ثابت FStruct_ڈیٹا& والد, ثابت حرف* کلید,
                                 صحیح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* ڈیٹا = 0;

        اگر ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_گرہ::TYPE_نقاب|File_گرہ::FLOW)) | File_گرہ::خالی_خانے_2;
        اگر( !File_گرہ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        اگر (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_گرہ::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            ڈیٹا = buf;
        }
        ورنہ اگر( File_گرہ::isFlow(struct_flags))
        {
            حرف c = File_گرہ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_گرہ::FLOW;

            اگر( type_name )
                snprintf( buf, حجم(buf), "!!%s %c", type_name, c );
            ورنہ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            ڈیٹا = buf;
        }
        ورنہ اگر( type_name )
        {
            snprintf( buf, حجم(buf), "!!%s", type_name );
            ڈیٹا = buf;
        }

        writeScalar( کلید, ڈیٹا );

        FStruct_ڈیٹا fsd;
        fsd.indent = والد.indent;
        fsd.flags = struct_flags;

        اگر( !File_گرہ::isFlow(والد.flags) )
            fsd.indent += CV_YML_INDENT + File_گرہ::isFlow(struct_flags);

        واپس fsd;
    }

    خالی انتہا_Write_Struct(ثابت FStruct_ڈیٹا& current_struct)
    {
        حرف* ptr;

        صحیح struct_flags = current_struct.flags;

        اگر( File_گرہ::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            اگر( ptr > fs->بفر_آغاز() + current_struct.indent && !File_گرہ::is_خالی_خانے_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_گرہ::isMap(struct_flags) ? '}' : ']';
            fs->سیٹ_بفر_Ptr(ptr);
        }
        ورنہ اگر( File_گرہ::is_خالی_خانے_Collection(struct_flags) )
        {
            ptr = fs->flush();
            حافظہ_نقل( ptr, File_گرہ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->سیٹ_بفر_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    خالی اكتب(ثابت حرف* کلید, صحیح value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10 ));
    }

    خالی اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10, سچ ));
    }

    خالی اكتب( ثابت حرف* کلید, دوہرا value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_مقصد_String( buf, حجم(buf), value, جھوٹ ));
    }

    خالی اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقی quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* ڈیٹا = (حرف*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)لڑی_لمبائی(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحیح need_quote = quote || len == 0 || str[0] == ' ';
            ڈیٹا = buf;
            *ڈیٹا++ = '\"';
            برائے( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                اگر( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                اگر( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *ڈیٹا++ = '\\';
                    اگر( cv_isprint(c) )
                        *ڈیٹا++ = c;
                    ورنہ اگر( c == '\n' )
                        *ڈیٹا++ = 'n';
                    ورنہ اگر( c == '\r' )
                        *ڈیٹا++ = 'r';
                    ورنہ اگر( c == '\t' )
                        *ڈیٹا++ = 't';
                    ورنہ
                    {
                        snprintf( ڈیٹا, حجم(buf) - (ڈیٹا - buf), "x%02x", c );
                        ڈیٹا += 3;
                    }
                }
                ورنہ
                    *ڈیٹا++ = c;
            }
            اگر( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            اگر( need_quote )
                *ڈیٹا++ = '\"';
            *ڈیٹا++ = '\0';
            ڈیٹا = buf + !need_quote;
        }

        writeScalar( کلید, ڈیٹا);
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

        صحیح i, keylen = 0;
        صحیح datalen = 0;
        حرف* ptr;

        FStruct_ڈیٹا& current_struct = fs->getCurrentStruct();

        صحیح struct_flags = current_struct.flags;

        اگر( کلید && کلید[0] == '\0' )
            کلید = 0;

        اگر( File_گرہ::isCollection(struct_flags) )
        {
            اگر( (File_گرہ::isMap(struct_flags) ^ (کلید != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ورنہ
        {
            fs->سیٹ_Non_خالی_خانے();
            struct_flags = File_گرہ::خالی_خانے_2 | (کلید ? File_گرہ::MAP : File_گرہ::SEQ);
        }

        اگر( کلید )
        {
            keylen = (صحیح)لڑی_لمبائی(کلید);
            اگر( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            اگر( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        اگر( ڈیٹا )
            datalen = (صحیح)لڑی_لمبائی(ڈیٹا);

        اگر( File_گرہ::isFlow(struct_flags) )
        {
            ptr = fs->بفر_Ptr();
            اگر( !File_گرہ::is_خالی_خانے_Collection(struct_flags) )
                *ptr++ = ',';
            صحیح new_انحراف = (صحیح)(ptr - fs->بفر_آغاز()) + keylen + datalen;
            اگر( new_انحراف > fs->wrapMargin() && new_انحراف - current_struct.indent > 10 )
            {
                fs->سیٹ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            ورنہ
                *ptr++ = ' ';
        }
        ورنہ
        {
            ptr = fs->flush();
            اگر( !File_گرہ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                اگر( ڈیٹا )
                    *ptr++ = ' ';
            }
        }

        اگر( کلید )
        {
            اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, keylen );

            برائے( i = 0; i < keylen; i++ )
            {
                حرف c = کلید[i];

                ptr[i] = c;
                اگر( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            اگر( !File_گرہ::isFlow(struct_flags) && ڈیٹا )
                *ptr++ = ' ';
        }

        اگر( ڈیٹا )
        {
            ptr = fs->resize_Write_بفر( ptr, datalen );
            حافظہ_نقل( ptr, ڈیٹا, datalen );
            ptr += datalen;
        }

        fs->سیٹ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_گرہ::خالی_خانے_2;
    }

    خالی writeComment(ثابت حرف* comment, منطقی eol_comment)
    {
        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحیح len = (صحیح)لڑی_لمبائی(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقی multiline = eol != 0;
        حرف* ptr = fs->بفر_Ptr();

        اگر( !eol_comment || multiline ||
            fs->بفر_انتہا_2() - ptr < len || ptr == fs->بفر_آغاز() )
            ptr = fs->flush();
        ورنہ
            *ptr++ = ' ';

        جب( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            اگر( eol )
            {
                ptr = fs->resize_Write_بفر( ptr, (صحیح)(eol - comment) + 1 );
                حافظہ_نقل( ptr, comment, eol - comment + 1 );
                fs->سیٹ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ورنہ
            {
                len = (صحیح)لڑی_لمبائی(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                حافظہ_نقل( ptr, comment, len );
                fs->سیٹ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    خالی آغاز_اگلا_دھارا()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محفوظ:
    FileStorage_API* fs;
};


جماعت YAMLParser : عوامی FileStorageParser
{
عوامی:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~YAMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحیح min_indent, صحیح max_comment_indent )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        برائے(;;)
        {
            جب( *ptr == ' ' )
                ptr++;
            اگر( *ptr == '#' )
            {
                اگر( ptr - fs->بفر_آغاز() > max_comment_indent )
                    واپس ptr;
                *ptr = '\0';
            }
            ورنہ اگر( cv_isprint(*ptr) )
            {
                اگر( ptr - fs->بفر_آغاز() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                توڑو;
            }
            ورنہ اگر( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                اگر( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->بفر_آغاز();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->سیٹ_Eof();
                    توڑو;
                }
                ورنہ
                {
                    صحیح l = (صحیح)لڑی_لمبائی(ptr);
                    اگر( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ورنہ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        واپس ptr;
    }

    منطقی get_Base64_صف(حرف* ptr, صحیح indent, حرف* &beg, حرف* &end)
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, زیادہ_سے_زیادہ_عدد);
        اگر (!ptr || !*ptr)
            واپس جھوٹ; // end of file

        اگر (ptr - fs->بفر_آغاز() != indent)
            واپس جھوٹ; // end of base64 data

        /* find end */
        جب(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        اگر (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        واپس سچ;
    }


    حرف* parse_کلید( حرف* ptr, File_گرہ& map_گرہ, File_گرہ& قدر_placeholder )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        حرف c;
        حرف *endptr = ptr - 1, *saveptr;

        اگر( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        کرو c = *++endptr;
        جب( cv_isprint(c) && c != ':' );

        اگر( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        اگر( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        کرو c = *--endptr;
        جب( c == ' ' );

        ++endptr;

        قدر_placeholder = fs->جوڑو_گرہ(map_گرہ, std::string(ptr, endptr - ptr), File_گرہ::NONE);
        ptr = saveptr;

        واپس ptr;
    }

    حرف* parse_قدر( حرف* ptr, File_گرہ& گرہ, صحیح min_indent, منطقی is_والد_flow, صحیح گہرائی = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( گہرائی > CV_PERSISTENCE_MAX_گہرائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحیح value_type = File_گرہ::NONE;
        صحیح len;
        منطقی is_ثنائی_string = جھوٹ;
        منطقی is_user = جھوٹ;

        اگر( c == '!' ) // handle explicit type specification
        {
            اگر( d == '!' || d == '^' )
            {
                ptr++;
                is_user = سچ;
                //value_type |= FileNode::USER;
            }
            اگر ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t heading_لمبائی = لڑی_لمبائی(yamlTypeHeading);

                حرف* type_انتہا_Ptr = ++ptr;

                کرو d = *++type_انتہا_Ptr;
                جب( cv_isprint(d) && d != ' ' && d != '>' );

                اگر ( d == '>' && (size_t)(type_انتہا_Ptr - ptr) > heading_لمبائی )
                {
                    اگر ( memcmp(ptr, yamlTypeHeading, heading_لمبائی) == 0 )
                    {
                        *type_انتہا_Ptr = ' ';
                        ptr += heading_لمبائی - 1;
                        is_user = سچ;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            کرو d = *++endptr;
            جب( cv_isprint(d) && d != ' ' );
            len = (صحیح)(endptr - ptr);
            اگر( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            اگر( len == 3 && !is_user )
            {
                اگر( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_گرہ::STRING;
                ورنہ اگر( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_گرہ::INT;
                ورنہ اگر( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_گرہ::SEQ;
                ورنہ اگر( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_گرہ::MAP;
            }
            ورنہ اگر( len == 5 && !is_user )
            {
                اگر( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_گرہ::REAL;
            }
            ورنہ اگر (len == 6 && is_user)
            {
                اگر( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_گرہ::SEQ;
                    is_ثنائی_string = سچ;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    کرو {
                        d = *++endptr;
                        اگر (d == '|')
                            توڑو;
                    } جب (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, زیادہ_سے_زیادہ_عدد );
            اگر (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            اگر( !is_user )
            {
                اگر (value_type == File_گرہ::STRING && c != '\'' && c != '\"')
                    جاؤ force_string;
                اگر( value_type == File_گرہ::INT )
                    جاؤ force_int;
                اگر( value_type == File_گرہ::REAL )
                    جاؤ force_real;
            }
        }

        اگر (is_ثنائی_string)
        {
            صحیح indent = static_cast<صحیح>(ptr - fs->بفر_آغاز());
            ptr = fs->parseBase64(ptr, indent, گرہ);
        }
        ورنہ اگر( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            جب( cv_isdigit(*endptr) )
                endptr++;
            اگر( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                دوہرا fval = fs->strtod( ptr, &endptr );
                گرہ.سیٹ_قدر(File_گرہ::REAL, &fval);
            }
            ورنہ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
            }

            اگر( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
        }
        ورنہ اگر( c == '\'' || c == '\"' ) // an explicit string
        {
            اگر( c == '\'' )
                برائے( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    اگر( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ورنہ اگر( c == '\'' )
                    {
                        c = *++ptr;
                        اگر( c != '\'' )
                            توڑو;
                        buf[len++] = c;
                    }
                    ورنہ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ورنہ
                برائے( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    اگر( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ورنہ اگر( c == '\"' )
                    {
                        ++ptr;
                        توڑو;
                    }
                    ورنہ اگر( c == '\\' )
                    {
                        d = *++ptr;
                        اگر( d == '\'' )
                            buf[len++] = d;
                        ورنہ اگر( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ورنہ اگر( d == 'n' )
                            buf[len++] = '\n';
                        ورنہ اگر( d == 'r' )
                            buf[len++] = '\r';
                        ورنہ اگر( d == 't' )
                            buf[len++] = '\t';
                        ورنہ اگر( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            صحیح val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (صحیح)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            اگر( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ورنہ
                            {
                                buf[len++] = (حرف)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ورنہ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            اگر( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            گرہ.سیٹ_قدر(File_گرہ::STRING, buf, len);
        }
        ورنہ اگر( c == '[' || c == '{' ) // collection as a flow
        {
            صحیح new_min_indent = min_indent + !is_والد_flow;
            صحیح struct_type = c == '{' ? File_گرہ::MAP : File_گرہ::SEQ;
            صحیح nelems = 0;

            fs->convert_مقصد_Collection(struct_type, گرہ);
            d = c == '[' ? ']' : '}';

            برائے( ++ptr ;; nelems++ )
            {
                File_گرہ elem;

                ptr = skipSpaces( ptr, new_min_indent, زیادہ_سے_زیادہ_عدد );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                اگر( *ptr == '}' || *ptr == ']' )
                {
                    اگر( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    توڑو;
                }

                اگر( nelems != 0 )
                {
                    اگر( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, زیادہ_سے_زیادہ_عدد );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                اگر( struct_type == File_گرہ::MAP )
                {
                    ptr = parse_کلید( ptr, گرہ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, زیادہ_سے_زیادہ_عدد );
                }
                ورنہ
                {
                    اگر( *ptr == ']' )
                        توڑو;
                    elem = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE);
                }
                ptr = parse_قدر( ptr, elem, new_min_indent, سچ, گہرائی + 1 );
            }
            fs->finalizeCollection(گرہ);
        }
        ورنہ
        {
            صحیح indent, struct_type;

            اگر( is_والد_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                اگر( !is_والد_flow )
                {
                    اگر( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    اگر( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                کرو c = *++endptr;
                جب( cv_isprint(c) &&
                      (!is_والد_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_والد_flow || c != ':' || value_type == File_گرہ::STRING));

                اگر( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                اگر( is_والد_flow || c != ':' )
                {
                    حرف* str_انتہا = endptr;
                    کرو c = *--str_انتہا;
                    جب( str_انتہا > ptr && c == ' ' );
                    str_انتہا++;
                    اگر ((fs->getFormat() & FileStorage::FORMAT_نقاب) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_انتہا - ptr;
                        اگر (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
                            ptr = endptr;
                            واپس ptr;
                        }
                        ورنہ اگر (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
                            ptr = endptr;
                            واپس ptr;
                        }
                        ورنہ اگر (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            واپس ptr;
                        }
                    }

                    گرہ.سیٹ_قدر(File_گرہ::STRING, ptr, (صحیح)(str_انتہا - ptr));
                    ptr = endptr;
                    واپس ptr;
                }
                struct_type = File_گرہ::MAP;
            }
            ورنہ
                struct_type = File_گرہ::SEQ;

            fs->convert_مقصد_Collection( struct_type, گرہ );
            indent = (صحیح)(ptr - fs->بفر_آغاز());

            برائے(;;)
            {
                File_گرہ elem;

                اگر( struct_type == File_گرہ::MAP )
                {
                    ptr = parse_کلید( ptr, گرہ, elem );
                }
                ورنہ
                {
                    c = *ptr++;
                    اگر( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, زیادہ_سے_زیادہ_عدد );
                ptr = parse_قدر( ptr, elem, indent + 1, جھوٹ, گہرائی + 1 );
                ptr = skipSpaces( ptr, 0, زیادہ_سے_زیادہ_عدد );
                اگر( ptr - fs->بفر_آغاز() != indent )
                {
                    اگر( ptr - fs->بفر_آغاز() < indent )
                        توڑو;
                    ورنہ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                اگر( memcmp( ptr, "...", 3 ) == 0 )
                    توڑو;
            }
            fs->finalizeCollection(گرہ);
        }

        واپس ptr;
    }

    منطقی parse( حرف* ptr )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقی first = سچ;
        منطقی ok = سچ;
        File_گرہ جڑ_collection(fs->getFS(), 0, 0);

        برائے(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            برائے(;;)
            {
                ptr = skipSpaces( ptr, 0, زیادہ_سے_زیادہ_عدد );
                اگر( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    توڑو;
                }

                اگر( *ptr == '%' )
                {
                    اگر( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        اگر ((fs->getFormat() & FileStorage::FORMAT_نقاب) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ورنہ اگر( *ptr == '-' )
                {
                    اگر( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        توڑو;
                    }
                    ورنہ اگر( first )
                        توڑو;
                }
                ورنہ اگر( cv_isalnum(*ptr) || *ptr=='_')
                {
                    اگر( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    توڑو;
                }
                ورنہ اگر( fs->eof() )
                    توڑو;
                ورنہ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            اگر( ptr )
                ptr = skipSpaces( ptr, 0, زیادہ_سے_زیادہ_عدد );
            اگر( !ptr || !ptr[0] )
                توڑو;
            اگر( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_گرہ جڑ_گرہ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::NONE);

                ptr = parse_قدر( ptr, جڑ_گرہ, 0, جھوٹ );
                اگر( !جڑ_گرہ.isMap() && !جڑ_گرہ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, زیادہ_سے_زیادہ_عدد );
                اگر( !ptr )
                    توڑو;
            }

            اگر( fs->eof() )
                توڑو;
            ptr += 3;
            first = جھوٹ;
        }

        واپس ok;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    واپس makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    واپس makePtr<YAMLParser>(fs);
}

}
