// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

شمارش
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

فضاینام cv
{

رده YAMLEmitter : عمومی FileStorageEmitter
{
عمومی:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~YAMLEmitter() {}

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت نویسه* کلید,
                                 صحیح struct_flags, ثابت نویسه* type_name=0)
    {
        نویسه buf[CV_FS_MAX_LEN + 1024];
        ثابت نویسه* data = 0;

        اگر ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_گره::TYPE_MASK|File_گره::FLOW)) | File_گره::EMPTY;
        اگر( !File_گره::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        اگر (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_گره::SEQ;
            snprintf(buf, اندازه(buf), "!!binary |");
            data = buf;
        }
        وگرنه اگر( File_گره::isFlow(struct_flags))
        {
            نویسه c = File_گره::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_گره::FLOW;

            اگر( type_name )
                snprintf( buf, اندازه(buf), "!!%s %c", type_name, c );
            وگرنه
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        وگرنه اگر( type_name )
        {
            snprintf( buf, اندازه(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( کلید, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        اگر( !File_گره::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_گره::isFlow(struct_flags);

        بازگشت fsd;
    }

    تهی endWriteStruct(ثابت FStructData& current_struct)
    {
        نویسه* ptr;

        صحیح struct_flags = current_struct.flags;

        اگر( File_گره::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            اگر( ptr > fs->bufferStart() + current_struct.indent && !File_گره::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_گره::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        وگرنه اگر( File_گره::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_گره::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    تهی اكتب(ثابت نویسه* کلید, صحیح value)
    {
        نویسه buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10 ));
    }

    تهی اكتب(ثابت نویسه* کلید, int64_t value)
    {
        نویسه buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10, درست ));
    }

    تهی اكتب( ثابت نویسه* کلید, دوگانه value )
    {
        نویسه buf[128];
        writeScalar( کلید, fs::doubleToString( buf, اندازه(buf), value, نادرست ));
    }

    تهی اكتب(ثابت نویسه* کلید, ثابت نویسه* str, منطقی quote)
    {
        نویسه buf[CV_FS_MAX_LEN*4+16];
        نویسه* data = (نویسه*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)strlen(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحیح need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            برای( i = 0; i < len; i++ )
            {
                نویسه c = str[i];

                اگر( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                اگر( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    اگر( cv_isprint(c) )
                        *data++ = c;
                    وگرنه اگر( c == '\n' )
                        *data++ = 'n';
                    وگرنه اگر( c == '\r' )
                        *data++ = 'r';
                    وگرنه اگر( c == '\t' )
                        *data++ = 't';
                    وگرنه
                    {
                        snprintf( data, اندازه(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                وگرنه
                    *data++ = c;
            }
            اگر( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            اگر( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( کلید, data);
    }

    تهی writeScalar(ثابت نویسه* کلید, ثابت نویسه* data)
    {
        fs->check_if_write_struct_is_delayed(نادرست);
        اگر ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وگرنه اگر ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحیح i, keylen = 0;
        صحیح datalen = 0;
        نویسه* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        صحیح struct_flags = current_struct.flags;

        اگر( کلید && کلید[0] == '\0' )
            کلید = 0;

        اگر( File_گره::isCollection(struct_flags) )
        {
            اگر( (File_گره::isMap(struct_flags) ^ (کلید != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        وگرنه
        {
            fs->setNonEmpty();
            struct_flags = File_گره::EMPTY | (کلید ? File_گره::MAP : File_گره::SEQ);
        }

        اگر( کلید )
        {
            keylen = (صحیح)strlen(کلید);
            اگر( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            اگر( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        اگر( data )
            datalen = (صحیح)strlen(data);

        اگر( File_گره::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            اگر( !File_گره::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            صحیح new_offset = (صحیح)(ptr - fs->bufferStart()) + keylen + datalen;
            اگر( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            وگرنه
                *ptr++ = ' ';
        }
        وگرنه
        {
            ptr = fs->flush();
            اگر( !File_گره::isMap(struct_flags) )
            {
                *ptr++ = '-';
                اگر( data )
                    *ptr++ = ' ';
            }
        }

        اگر( کلید )
        {
            اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            برای( i = 0; i < keylen; i++ )
            {
                نویسه c = کلید[i];

                ptr[i] = c;
                اگر( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            اگر( !File_گره::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        اگر( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_گره::EMPTY;
    }

    تهی writeComment(ثابت نویسه* comment, منطقی eol_comment)
    {
        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحیح len = (صحیح)strlen(comment);
        ثابت نویسه* eol = strchr(comment, '\n');
        منطقی multiline = eol != 0;
        نویسه* ptr = fs->bufferPtr();

        اگر( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        وگرنه
            *ptr++ = ' ';

        وقتی( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            اگر( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحیح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وگرنه
            {
                len = (صحیح)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    تهی startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محافظتشده:
    FileStorage_API* fs;
};


رده YAMLParser : عمومی FileStorageParser
{
عمومی:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~YAMLParser() {}

    نویسه* skipSpaces( نویسه* ptr, صحیح min_indent, صحیح max_comment_indent )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        برای(;;)
        {
            وقتی( *ptr == ' ' )
                ptr++;
            اگر( *ptr == '#' )
            {
                اگر( ptr - fs->bufferStart() > max_comment_indent )
                    بازگشت ptr;
                *ptr = '\0';
            }
            وگرنه اگر( cv_isprint(*ptr) )
            {
                اگر( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                بشکن;
            }
            وگرنه اگر( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                اگر( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    بشکن;
                }
                وگرنه
                {
                    صحیح l = (صحیح)strlen(ptr);
                    اگر( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            وگرنه
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        بازگشت ptr;
    }

    منطقی getBase64Row(نویسه* ptr, صحیح indent, نویسه* &beg, نویسه* &end)
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        اگر (!ptr || !*ptr)
            بازگشت نادرست; // end of file

        اگر (ptr - fs->bufferStart() != indent)
            بازگشت نادرست; // end of base64 data

        /* find end */
        وقتی(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        اگر (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        بازگشت درست;
    }


    نویسه* parse_کلید( نویسه* ptr, File_گره& map_گره, File_گره& مقدار_placeholder )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        نویسه c;
        نویسه *endptr = ptr - 1, *saveptr;

        اگر( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        انجامبده c = *++endptr;
        وقتی( cv_isprint(c) && c != ':' );

        اگر( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        اگر( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        انجامبده c = *--endptr;
        وقتی( c == ' ' );

        ++endptr;

        مقدار_placeholder = fs->add_گره(map_گره, std::string(ptr, endptr - ptr), File_گره::NONE);
        ptr = saveptr;

        بازگشت ptr;
    }

    نویسه* parse_مقدار( نویسه* ptr, File_گره& گره, صحیح min_indent, منطقی is_parent_flow, صحیح depth = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        نویسه* endptr = 0;
        نویسه c = ptr[0], d = ptr[1];
        صحیح value_type = File_گره::NONE;
        صحیح len;
        منطقی is_دودویی_string = نادرست;
        منطقی is_user = نادرست;

        اگر( c == '!' ) // handle explicit type specification
        {
            اگر( d == '!' || d == '^' )
            {
                ptr++;
                is_user = درست;
                //value_type |= FileNode::USER;
            }
            اگر ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت نویسه* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t headingLength = strlen(yamlTypeHeading);

                نویسه* typeEndPtr = ++ptr;

                انجامبده d = *++typeEndPtr;
                وقتی( cv_isprint(d) && d != ' ' && d != '>' );

                اگر ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    اگر ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = درست;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            انجامبده d = *++endptr;
            وقتی( cv_isprint(d) && d != ' ' );
            len = (صحیح)(endptr - ptr);
            اگر( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            اگر( len == 3 && !is_user )
            {
                اگر( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_گره::STRING;
                وگرنه اگر( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_گره::INT;
                وگرنه اگر( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_گره::SEQ;
                وگرنه اگر( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_گره::MAP;
            }
            وگرنه اگر( len == 5 && !is_user )
            {
                اگر( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_گره::REAL;
            }
            وگرنه اگر (len == 6 && is_user)
            {
                اگر( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_گره::SEQ;
                    is_دودویی_string = درست;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    انجامبده {
                        d = *++endptr;
                        اگر (d == '|')
                            بشکن;
                    } وقتی (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            اگر (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            اگر( !is_user )
            {
                اگر (value_type == File_گره::STRING && c != '\'' && c != '\"')
                    برو force_string;
                اگر( value_type == File_گره::INT )
                    برو force_int;
                اگر( value_type == File_گره::REAL )
                    برو force_real;
            }
        }

        اگر (is_دودویی_string)
        {
            صحیح indent = static_cast<صحیح>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, گره);
        }
        وگرنه اگر( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            وقتی( cv_isdigit(*endptr) )
                endptr++;
            اگر( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                دوگانه fval = fs->strtod( ptr, &endptr );
                گره.set_مقدار(File_گره::REAL, &fval);
            }
            وگرنه
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                گره.set_مقدار(File_گره::INT, &ival);
            }

            اگر( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        وگرنه اگر( c == '\'' || c == '\"' ) // an explicit string
        {
            اگر( c == '\'' )
                برای( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    اگر( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    وگرنه اگر( c == '\'' )
                    {
                        c = *++ptr;
                        اگر( c != '\'' )
                            بشکن;
                        buf[len++] = c;
                    }
                    وگرنه
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            وگرنه
                برای( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    اگر( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    وگرنه اگر( c == '\"' )
                    {
                        ++ptr;
                        بشکن;
                    }
                    وگرنه اگر( c == '\\' )
                    {
                        d = *++ptr;
                        اگر( d == '\'' )
                            buf[len++] = d;
                        وگرنه اگر( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        وگرنه اگر( d == 'n' )
                            buf[len++] = '\n';
                        وگرنه اگر( d == 'r' )
                            buf[len++] = '\r';
                        وگرنه اگر( d == 't' )
                            buf[len++] = '\t';
                        وگرنه اگر( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            صحیح val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (صحیح)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            اگر( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            وگرنه
                            {
                                buf[len++] = (نویسه)val;
                                ptr = endptr;
                            }
                        }
                    }
                    وگرنه
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            اگر( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            گره.set_مقدار(File_گره::STRING, buf, len);
        }
        وگرنه اگر( c == '[' || c == '{' ) // collection as a flow
        {
            صحیح new_min_indent = min_indent + !is_parent_flow;
            صحیح struct_type = c == '{' ? File_گره::MAP : File_گره::SEQ;
            صحیح nelems = 0;

            fs->convertToCollection(struct_type, گره);
            d = c == '[' ? ']' : '}';

            برای( ++ptr ;; nelems++ )
            {
                File_گره elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                اگر( *ptr == '}' || *ptr == ']' )
                {
                    اگر( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    بشکن;
                }

                اگر( nelems != 0 )
                {
                    اگر( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                اگر( struct_type == File_گره::MAP )
                {
                    ptr = parse_کلید( ptr, گره, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                وگرنه
                {
                    اگر( *ptr == ']' )
                        بشکن;
                    elem = fs->add_گره(گره, std::string(), File_گره::NONE);
                }
                ptr = parse_مقدار( ptr, elem, new_min_indent, درست, depth + 1 );
            }
            fs->finalizeCollection(گره);
        }
        وگرنه
        {
            صحیح indent, struct_type;

            اگر( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                اگر( !is_parent_flow )
                {
                    اگر( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    اگر( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                انجامبده c = *++endptr;
                وقتی( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_گره::STRING));

                اگر( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                اگر( is_parent_flow || c != ':' )
                {
                    نویسه* str_end = endptr;
                    انجامبده c = *--str_end;
                    وقتی( str_end > ptr && c == ' ' );
                    str_end++;
                    اگر ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        اگر (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            گره.set_مقدار(File_گره::INT, &ival);
                            ptr = endptr;
                            بازگشت ptr;
                        }
                        وگرنه اگر (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            گره.set_مقدار(File_گره::INT, &ival);
                            ptr = endptr;
                            بازگشت ptr;
                        }
                        وگرنه اگر (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            بازگشت ptr;
                        }
                    }

                    گره.set_مقدار(File_گره::STRING, ptr, (صحیح)(str_end - ptr));
                    ptr = endptr;
                    بازگشت ptr;
                }
                struct_type = File_گره::MAP;
            }
            وگرنه
                struct_type = File_گره::SEQ;

            fs->convertToCollection( struct_type, گره );
            indent = (صحیح)(ptr - fs->bufferStart());

            برای(;;)
            {
                File_گره elem;

                اگر( struct_type == File_گره::MAP )
                {
                    ptr = parse_کلید( ptr, گره, elem );
                }
                وگرنه
                {
                    c = *ptr++;
                    اگر( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_گره(گره, std::string(), File_گره::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_مقدار( ptr, elem, indent + 1, نادرست, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                اگر( ptr - fs->bufferStart() != indent )
                {
                    اگر( ptr - fs->bufferStart() < indent )
                        بشکن;
                    وگرنه
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                اگر( memcmp( ptr, "...", 3 ) == 0 )
                    بشکن;
            }
            fs->finalizeCollection(گره);
        }

        بازگشت ptr;
    }

    منطقی parse( نویسه* ptr )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقی first = درست;
        منطقی ok = درست;
        File_گره ریشه_collection(fs->getFS(), 0, 0);

        برای(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            برای(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                اگر( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    بشکن;
                }

                اگر( *ptr == '%' )
                {
                    اگر( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        اگر ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                وگرنه اگر( *ptr == '-' )
                {
                    اگر( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        بشکن;
                    }
                    وگرنه اگر( first )
                        بشکن;
                }
                وگرنه اگر( cv_isalnum(*ptr) || *ptr=='_')
                {
                    اگر( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    بشکن;
                }
                وگرنه اگر( fs->eof() )
                    بشکن;
                وگرنه
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            اگر( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            اگر( !ptr || !ptr[0] )
                بشکن;
            اگر( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_گره ریشه_گره = fs->add_گره(ریشه_collection, std::string(), File_گره::NONE);

                ptr = parse_مقدار( ptr, ریشه_گره, 0, نادرست );
                اگر( !ریشه_گره.isMap() && !ریشه_گره.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                اگر( !ptr )
                    بشکن;
            }

            اگر( fs->eof() )
                بشکن;
            ptr += 3;
            first = نادرست;
        }

        بازگشت ok;
    }

    FileStorage_API* fs;
    نویسه buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    بازگشت makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    بازگشت makePtr<YAMLParser>(fs);
}

}
