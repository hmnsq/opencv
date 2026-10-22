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

מחלקה YAMLEmitter : عام FileStorageEmitter
{
عام:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~YAMLEmitter() {}

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* key,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* data = 0;

        אם ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        אם( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        אם (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            data = buf;
        }
        אחרת אם( FileNode::isFlow(struct_flags))
        {
            حرف c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            אם( type_name )
                snprintf( buf, حجم(buf), "!!%s %c", type_name, c );
            אחרת
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        אחרת אם( type_name )
        {
            snprintf( buf, حجم(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        אם( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        החזר fsd;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        حرف* ptr;

        صحيح struct_flags = current_struct.flags;

        אם( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            אם( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        אחרת אם( FileNode::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, FileNode::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    فراغ اكتب(ثابت حرف* key, صحيح value)
    {
        حرف buf[128];
        writeScalar( key, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* key, int64_t value)
    {
        حرف buf[128];
        writeScalar( key, fs::itoa( value, buf, 10, אמת ));
    }

    فراغ اكتب( ثابت حرف* key, مزدوج value )
    {
        حرف buf[128];
        writeScalar( key, fs::doubleToString( buf, حجم(buf), value, שקר ));
    }

    فراغ اكتب(ثابت حرف* key, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        אם( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        אם( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        אם( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            עבור( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                אם( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                אם( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    אם( cv_isprint(c) )
                        *data++ = c;
                    אחרת אם( c == '\n' )
                        *data++ = 'n';
                    אחרת אם( c == '\r' )
                        *data++ = 'r';
                    אחרת אם( c == '\t' )
                        *data++ = 't';
                    אחרת
                    {
                        snprintf( data, حجم(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                אחרת
                    *data++ = c;
            }
            אם( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            אם( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    فراغ writeScalar(ثابت حرف* key, ثابت حرف* data)
    {
        fs->check_if_write_struct_is_delayed(שקר);
        אם ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        אחרת אם ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح i, keylen = 0;
        صحيح datalen = 0;
        حرف* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        صحيح struct_flags = current_struct.flags;

        אם( key && key[0] == '\0' )
            key = 0;

        אם( FileNode::isCollection(struct_flags) )
        {
            אם( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        אחרת
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        אם( key )
        {
            keylen = (صحيح)strlen(key);
            אם( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            אם( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        אם( data )
            datalen = (صحيح)strlen(data);

        אם( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            אם( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + keylen + datalen;
            אם( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            אחרת
                *ptr++ = ' ';
        }
        אחרת
        {
            ptr = fs->flush();
            אם( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                אם( data )
                    *ptr++ = ' ';
            }
        }

        אם( key )
        {
            אם( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            עבור( i = 0; i < keylen; i++ )
            {
                حرف c = key[i];

                ptr[i] = c;
                אם( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            אם( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        אם( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        אם( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = (صحيح)strlen(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;
        حرف* ptr = fs->bufferPtr();

        אם( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        אחרת
            *ptr++ = ' ';

        כלעוד( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            אם( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            אחרת
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


מחלקה YAMLParser : عام FileStorageParser
{
عام:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~YAMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح min_indent, صحيح max_comment_indent )
    {
        אם (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        עבור(;;)
        {
            כלעוד( *ptr == ' ' )
                ptr++;
            אם( *ptr == '#' )
            {
                אם( ptr - fs->bufferStart() > max_comment_indent )
                    החזר ptr;
                *ptr = '\0';
            }
            אחרת אם( cv_isprint(*ptr) )
            {
                אם( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                اكسر;
            }
            אחרת אם( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                אם( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    اكسر;
                }
                אחרת
                {
                    صحيح l = (صحيح)strlen(ptr);
                    אם( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            אחרת
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        החזר ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end)
    {
        אם (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        אם (!ptr || !*ptr)
            החזר שקר; // end of file

        אם (ptr - fs->bufferStart() != indent)
            החזר שקר; // end of base64 data

        /* find end */
        כלעוד(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        אם (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        החזר אמת;
    }


    حرف* parseKey( حرف* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        אם (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        حرف c;
        حرف *endptr = ptr - 1, *saveptr;

        אם( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        עשה c = *++endptr;
        כלעוד( cv_isprint(c) && c != ':' );

        אם( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        אם( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        עשה c = *--endptr;
        כלעוד( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        החזר ptr;
    }

    حرف* parseValue( حرف* ptr, FileNode& node, صحيح min_indent, منطقي is_parent_flow, صحيح depth = 0 )
    {
        אם (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        אם( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحيح value_type = FileNode::NONE;
        صحيح len;
        منطقي is_binary_string = שקר;
        منطقي is_user = שקר;

        אם( c == '!' ) // handle explicit type specification
        {
            אם( d == '!' || d == '^' )
            {
                ptr++;
                is_user = אמת;
                //value_type |= FileNode::USER;
            }
            אם ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t headingLength = strlen(yamlTypeHeading);

                حرف* typeEndPtr = ++ptr;

                עשה d = *++typeEndPtr;
                כלעוד( cv_isprint(d) && d != ' ' && d != '>' );

                אם ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    אם ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = אמת;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            עשה d = *++endptr;
            כלעוד( cv_isprint(d) && d != ' ' );
            len = (صحيح)(endptr - ptr);
            אם( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            אם( len == 3 && !is_user )
            {
                אם( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                אחרת אם( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                אחרת אם( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                אחרת אם( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            אחרת אם( len == 5 && !is_user )
            {
                אם( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            אחרת אם (len == 6 && is_user)
            {
                אם( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = אמת;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    עשה {
                        d = *++endptr;
                        אם (d == '|')
                            اكسر;
                    } כלעוד (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            אם (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            אם( !is_user )
            {
                אם (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    اذهب force_string;
                אם( value_type == FileNode::INT )
                    اذهب force_int;
                אם( value_type == FileNode::REAL )
                    اذهب force_real;
            }
        }

        אם (is_binary_string)
        {
            صحيح indent = static_cast<صحيح>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        אחרת אם( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            כלעוד( cv_isdigit(*endptr) )
                endptr++;
            אם( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                مزدوج fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            אחרת
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            אם( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        אחרת אם( c == '\'' || c == '\"' ) // an explicit string
        {
            אם( c == '\'' )
                עבור( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    אם( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    אחרת אם( c == '\'' )
                    {
                        c = *++ptr;
                        אם( c != '\'' )
                            اكسر;
                        buf[len++] = c;
                    }
                    אחרת
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            אחרת
                עבור( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    אם( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    אחרת אם( c == '\"' )
                    {
                        ++ptr;
                        اكسر;
                    }
                    אחרת אם( c == '\\' )
                    {
                        d = *++ptr;
                        אם( d == '\'' )
                            buf[len++] = d;
                        אחרת אם( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        אחרת אם( d == 'n' )
                            buf[len++] = '\n';
                        אחרת אם( d == 'r' )
                            buf[len++] = '\r';
                        אחרת אם( d == 't' )
                            buf[len++] = '\t';
                        אחרת אם( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            صحيح val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (صحيح)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            אם( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            אחרת
                            {
                                buf[len++] = (حرف)val;
                                ptr = endptr;
                            }
                        }
                    }
                    אחרת
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            אם( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        אחרת אם( c == '[' || c == '{' ) // collection as a flow
        {
            صحيح new_min_indent = min_indent + !is_parent_flow;
            صحيح struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            صحيح nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            עבור( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                אם (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                אם( *ptr == '}' || *ptr == ']' )
                {
                    אם( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    اكسر;
                }

                אם( nelems != 0 )
                {
                    אם( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    אם (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                אם( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                אחרת
                {
                    אם( *ptr == ']' )
                        اكسر;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, אמת, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        אחרת
        {
            صحيح indent, struct_type;

            אם( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                אם( !is_parent_flow )
                {
                    אם( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    אם( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                עשה c = *++endptr;
                כלעוד( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                אם( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                אם( is_parent_flow || c != ':' )
                {
                    حرف* str_end = endptr;
                    עשה c = *--str_end;
                    כלעוד( str_end > ptr && c == ' ' );
                    str_end++;
                    אם ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        אם (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            החזר ptr;
                        }
                        אחרת אם (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            החזר ptr;
                        }
                        אחרת אם (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            החזר ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (صحيح)(str_end - ptr));
                    ptr = endptr;
                    החזר ptr;
                }
                struct_type = FileNode::MAP;
            }
            אחרת
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (صحيح)(ptr - fs->bufferStart());

            עבור(;;)
            {
                FileNode elem;

                אם( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                אחרת
                {
                    c = *ptr++;
                    אם( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, שקר, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                אם( ptr - fs->bufferStart() != indent )
                {
                    אם( ptr - fs->bufferStart() < indent )
                        اكسر;
                    אחרת
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                אם( memcmp( ptr, "...", 3 ) == 0 )
                    اكسر;
            }
            fs->finalizeCollection(node);
        }

        החזר ptr;
    }

    منطقي parse( حرف* ptr )
    {
        אם (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقي first = אמת;
        منطقي ok = אמת;
        FileNode root_collection(fs->getFS(), 0, 0);

        עבור(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            עבור(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                אם( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    اكسر;
                }

                אם( *ptr == '%' )
                {
                    אם( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        אם ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                אחרת אם( *ptr == '-' )
                {
                    אם( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        اكسر;
                    }
                    אחרת אם( first )
                        اكسر;
                }
                אחרת אם( cv_isalnum(*ptr) || *ptr=='_')
                {
                    אם( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    اكسر;
                }
                אחרת אם( fs->eof() )
                    اكسر;
                אחרת
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            אם( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            אם( !ptr || !ptr[0] )
                اكسر;
            אם( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, שקר );
                אם( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                אם( !ptr )
                    اكسر;
            }

            אם( fs->eof() )
                اكسر;
            ptr += 3;
            first = שקר;
        }

        החזר ok;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    החזר makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    החזר makePtr<YAMLParser>(fs);
}

}
