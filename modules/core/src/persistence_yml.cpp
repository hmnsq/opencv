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
    virtual ~YAMLEmitter() {}

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* کلیل,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* data = 0;

        إذا ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_گرێ::TYPE_MASK|File_گرێ::FLOW)) | File_گرێ::EMPTY;
        إذا( !File_گرێ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_گرێ::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            data = buf;
        }
        وإلا إذا( File_گرێ::isFlow(struct_flags))
        {
            حرف c = File_گرێ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_گرێ::FLOW;

            إذا( type_name )
                snprintf( buf, حجم(buf), "!!%s %c", type_name, c );
            وإلا
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        وإلا إذا( type_name )
        {
            snprintf( buf, حجم(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( کلیل, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        إذا( !File_گرێ::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_گرێ::isFlow(struct_flags);

        أرجع fsd;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        حرف* ptr;

        صحيح struct_flags = current_struct.flags;

        إذا( File_گرێ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            إذا( ptr > fs->bufferStart() + current_struct.indent && !File_گرێ::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_گرێ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        وإلا إذا( File_گرێ::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_گرێ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    فراغ اكتب(ثابت حرف* کلیل, صحيح value)
    {
        حرف buf[128];
        writeScalar( کلیل, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* کلیل, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلیل, fs::itoa( value, buf, 10, true ));
    }

    فراغ اكتب( ثابت حرف* کلیل, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلیل, fs::doubleToString( buf, حجم(buf), value, خطأ ));
    }

    فراغ اكتب(ثابت حرف* کلیل, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                إذا( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    إذا( cv_isprint(c) )
                        *data++ = c;
                    وإلا إذا( c == '\n' )
                        *data++ = 'n';
                    وإلا إذا( c == '\r' )
                        *data++ = 'r';
                    وإلا إذا( c == '\t' )
                        *data++ = 't';
                    وإلا
                    {
                        snprintf( data, حجم(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                وإلا
                    *data++ = c;
            }
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            إذا( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( کلیل, data);
    }

    فراغ writeScalar(ثابت حرف* کلیل, ثابت حرف* data)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح i, keylen = 0;
        صحيح datalen = 0;
        حرف* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        صحيح struct_flags = current_struct.flags;

        إذا( کلیل && کلیل[0] == '\0' )
            کلیل = 0;

        إذا( File_گرێ::isCollection(struct_flags) )
        {
            إذا( (File_گرێ::isMap(struct_flags) ^ (کلیل != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        وإلا
        {
            fs->setNonEmpty();
            struct_flags = File_گرێ::EMPTY | (کلیل ? File_گرێ::MAP : File_گرێ::SEQ);
        }

        إذا( کلیل )
        {
            keylen = (صحيح)strlen(کلیل);
            إذا( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            إذا( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        إذا( data )
            datalen = (صحيح)strlen(data);

        إذا( File_گرێ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            إذا( !File_گرێ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + keylen + datalen;
            إذا( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            ptr = fs->flush();
            إذا( !File_گرێ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                إذا( data )
                    *ptr++ = ' ';
            }
        }

        إذا( کلیل )
        {
            إذا( !cv_isalpha(کلیل[0]) && کلیل[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            لكل( i = 0; i < keylen; i++ )
            {
                حرف c = کلیل[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            إذا( !File_گرێ::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        إذا( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_گرێ::EMPTY;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = (صحيح)strlen(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;
        حرف* ptr = fs->bufferPtr();

        إذا( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        وإلا
            *ptr++ = ' ';

        طالما( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            إذا( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
            {
                len = (صحيح)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ startNextStream()
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

    virtual ~YAMLParser() {}

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
                إذا( ptr - fs->bufferStart() > max_comment_indent )
                    أرجع ptr;
                *ptr = '\0';
            }
            وإلا إذا( cv_isprint(*ptr) )
            {
                إذا( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                اكسر;
            }
            وإلا إذا( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                إذا( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    اكسر;
                }
                وإلا
                {
                    صحيح l = (صحيح)strlen(ptr);
                    إذا( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            وإلا
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        أرجع ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end)
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        إذا (!ptr || !*ptr)
            أرجع خطأ; // end of file

        إذا (ptr - fs->bufferStart() != indent)
            أرجع خطأ; // end of base64 data

        /* find end */
        طالما(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        إذا (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        أرجع true;
    }


    حرف* parse_کلیل( حرف* ptr, File_گرێ& map_گرێ, File_گرێ& بەها_placeholder )
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

        بەها_placeholder = fs->add_گرێ(map_گرێ, std::string(ptr, endptr - ptr), File_گرێ::NONE);
        ptr = saveptr;

        أرجع ptr;
    }

    حرف* parse_بەها( حرف* ptr, File_گرێ& گرێ, صحيح min_indent, منطقي is_parent_flow, صحيح depth = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحيح value_type = File_گرێ::NONE;
        صحيح len;
        منطقي is_دووانەیی_string = خطأ;
        منطقي is_user = خطأ;

        إذا( c == '!' ) // handle explicit type specification
        {
            إذا( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            إذا ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t headingLength = strlen(yamlTypeHeading);

                حرف* typeEndPtr = ++ptr;

                افعل d = *++typeEndPtr;
                طالما( cv_isprint(d) && d != ' ' && d != '>' );

                إذا ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    إذا ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = true;
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
                    value_type = File_گرێ::STRING;
                وإلا إذا( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_گرێ::INT;
                وإلا إذا( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_گرێ::SEQ;
                وإلا إذا( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_گرێ::MAP;
            }
            وإلا إذا( len == 5 && !is_user )
            {
                إذا( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_گرێ::REAL;
            }
            وإلا إذا (len == 6 && is_user)
            {
                إذا( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_گرێ::SEQ;
                    is_دووانەیی_string = true;

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
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            إذا( !is_user )
            {
                إذا (value_type == File_گرێ::STRING && c != '\'' && c != '\"')
                    اذهب force_string;
                إذا( value_type == File_گرێ::INT )
                    اذهب force_int;
                إذا( value_type == File_گرێ::REAL )
                    اذهب force_real;
            }
        }

        إذا (is_دووانەیی_string)
        {
            صحيح indent = static_cast<صحيح>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, گرێ);
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
                گرێ.set_بەها(File_گرێ::REAL, &fval);
            }
            وإلا
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                گرێ.set_بەها(File_گرێ::INT, &ival);
            }

            إذا( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
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

            گرێ.set_بەها(File_گرێ::STRING, buf, len);
        }
        وإلا إذا( c == '[' || c == '{' ) // collection as a flow
        {
            صحيح new_min_indent = min_indent + !is_parent_flow;
            صحيح struct_type = c == '{' ? File_گرێ::MAP : File_گرێ::SEQ;
            صحيح nelems = 0;

            fs->convertToCollection(struct_type, گرێ);
            d = c == '[' ? ']' : '}';

            لكل( ++ptr ;; nelems++ )
            {
                File_گرێ elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
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
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                إذا( struct_type == File_گرێ::MAP )
                {
                    ptr = parse_کلیل( ptr, گرێ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                وإلا
                {
                    إذا( *ptr == ']' )
                        اكسر;
                    elem = fs->add_گرێ(گرێ, std::string(), File_گرێ::NONE);
                }
                ptr = parse_بەها( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(گرێ);
        }
        وإلا
        {
            صحيح indent, struct_type;

            إذا( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                إذا( !is_parent_flow )
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
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_گرێ::STRING));

                إذا( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                إذا( is_parent_flow || c != ':' )
                {
                    حرف* str_end = endptr;
                    افعل c = *--str_end;
                    طالما( str_end > ptr && c == ' ' );
                    str_end++;
                    إذا ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        إذا (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            گرێ.set_بەها(File_گرێ::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            گرێ.set_بەها(File_گرێ::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا إذا (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            أرجع ptr;
                        }
                    }

                    گرێ.set_بەها(File_گرێ::STRING, ptr, (صحيح)(str_end - ptr));
                    ptr = endptr;
                    أرجع ptr;
                }
                struct_type = File_گرێ::MAP;
            }
            وإلا
                struct_type = File_گرێ::SEQ;

            fs->convertToCollection( struct_type, گرێ );
            indent = (صحيح)(ptr - fs->bufferStart());

            لكل(;;)
            {
                File_گرێ elem;

                إذا( struct_type == File_گرێ::MAP )
                {
                    ptr = parse_کلیل( ptr, گرێ, elem );
                }
                وإلا
                {
                    c = *ptr++;
                    إذا( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_گرێ(گرێ, std::string(), File_گرێ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_بەها( ptr, elem, indent + 1, خطأ, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                إذا( ptr - fs->bufferStart() != indent )
                {
                    إذا( ptr - fs->bufferStart() < indent )
                        اكسر;
                    وإلا
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                إذا( memcmp( ptr, "...", 3 ) == 0 )
                    اكسر;
            }
            fs->finalizeCollection(گرێ);
        }

        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقي first = true;
        منطقي ok = true;
        File_گرێ ڕەگ_collection(fs->getFS(), 0, 0);

        لكل(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            لكل(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
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
                        إذا ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
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
                ptr = skipSpaces( ptr, 0, INT_MAX );
            إذا( !ptr || !ptr[0] )
                اكسر;
            إذا( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_گرێ ڕەگ_گرێ = fs->add_گرێ(ڕەگ_collection, std::string(), File_گرێ::NONE);

                ptr = parse_بەها( ptr, ڕەگ_گرێ, 0, خطأ );
                إذا( !ڕەگ_گرێ.isMap() && !ڕەگ_گرێ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
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
