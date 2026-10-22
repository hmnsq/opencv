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

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* key,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        حرف buf[CV_FS_MAX_LEN + 1024];
        ثابت حرف* data = 0;

        𐡄𐡍 ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        𐡄𐡍( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        𐡄𐡍 (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, حجم(buf), "!!binary |");
            data = buf;
        }
        وإلا 𐡄𐡍( FileNode::isFlow(struct_flags))
        {
            حرف c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            𐡄𐡍( type_name )
                snprintf( buf, حجم(buf), "!!%s %c", type_name, c );
            وإلا
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        وإلا 𐡄𐡍( type_name )
        {
            snprintf( buf, حجم(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        𐡄𐡍( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        أرجع fsd;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        حرف* ptr;

        صحيح struct_flags = current_struct.flags;

        𐡄𐡍( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            𐡄𐡍( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        وإلا 𐡄𐡍( FileNode::isEmptyCollection(struct_flags) )
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
        writeScalar( key, fs::itoa( value, buf, 10, 𐡒𐡔𐡈 ));
    }

    فراغ اكتب( ثابت حرف* key, مزدوج value )
    {
        حرف buf[128];
        writeScalar( key, fs::doubleToString( buf, حجم(buf), value, خطأ ));
    }

    فراغ اكتب(ثابت حرف* key, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*4+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        𐡄𐡍( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        𐡄𐡍( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        𐡄𐡍( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                𐡄𐡍( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                𐡄𐡍( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    𐡄𐡍( cv_isprint(c) )
                        *data++ = c;
                    وإلا 𐡄𐡍( c == '\n' )
                        *data++ = 'n';
                    وإلا 𐡄𐡍( c == '\r' )
                        *data++ = 'r';
                    وإلا 𐡄𐡍( c == '\t' )
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
            𐡄𐡍( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            𐡄𐡍( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    فراغ writeScalar(ثابت حرف* key, ثابت حرف* data)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        𐡄𐡍 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وإلا 𐡄𐡍 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح i, keylen = 0;
        صحيح datalen = 0;
        حرف* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        صحيح struct_flags = current_struct.flags;

        𐡄𐡍( key && key[0] == '\0' )
            key = 0;

        𐡄𐡍( FileNode::isCollection(struct_flags) )
        {
            𐡄𐡍( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        وإلا
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        𐡄𐡍( key )
        {
            keylen = (صحيح)strlen(key);
            𐡄𐡍( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            𐡄𐡍( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        𐡄𐡍( data )
            datalen = (صحيح)strlen(data);

        𐡄𐡍( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            𐡄𐡍( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + keylen + datalen;
            𐡄𐡍( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
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
            𐡄𐡍( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                𐡄𐡍( data )
                    *ptr++ = ' ';
            }
        }

        𐡄𐡍( key )
        {
            𐡄𐡍( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            لكل( i = 0; i < keylen; i++ )
            {
                حرف c = key[i];

                ptr[i] = c;
                𐡄𐡍( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            𐡄𐡍( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        𐡄𐡍( data )
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
        𐡄𐡍( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = (صحيح)strlen(comment);
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;
        حرف* ptr = fs->bufferPtr();

        𐡄𐡍( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        وإلا
            *ptr++ = ' ';

        طالما( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            𐡄𐡍( eol )
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
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        لكل(;;)
        {
            طالما( *ptr == ' ' )
                ptr++;
            𐡄𐡍( *ptr == '#' )
            {
                𐡄𐡍( ptr - fs->bufferStart() > max_comment_indent )
                    أرجع ptr;
                *ptr = '\0';
            }
            وإلا 𐡄𐡍( cv_isprint(*ptr) )
            {
                𐡄𐡍( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                اكسر;
            }
            وإلا 𐡄𐡍( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                𐡄𐡍( !ptr )
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
                    𐡄𐡍( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
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
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        𐡄𐡍 (!ptr || !*ptr)
            أرجع خطأ; // end of file

        𐡄𐡍 (ptr - fs->bufferStart() != indent)
            أرجع خطأ; // end of base64 data

        /* find end */
        طالما(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        𐡄𐡍 (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        أرجع 𐡒𐡔𐡈;
    }


    حرف* parseKey( حرف* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        حرف c;
        حرف *endptr = ptr - 1, *saveptr;

        𐡄𐡍( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        𐡏𐡁𐡃 c = *++endptr;
        طالما( cv_isprint(c) && c != ':' );

        𐡄𐡍( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        𐡄𐡍( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        𐡏𐡁𐡃 c = *--endptr;
        طالما( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        أرجع ptr;
    }

    حرف* parseValue( حرف* ptr, FileNode& node, صحيح min_indent, منطقي is_parent_flow, صحيح depth = 0 )
    {
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        𐡄𐡍( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        حرف* endptr = 0;
        حرف c = ptr[0], d = ptr[1];
        صحيح value_type = FileNode::NONE;
        صحيح len;
        منطقي is_binary_string = خطأ;
        منطقي is_user = خطأ;

        𐡄𐡍( c == '!' ) // handle explicit type specification
        {
            𐡄𐡍( d == '!' || d == '^' )
            {
                ptr++;
                is_user = 𐡒𐡔𐡈;
                //value_type |= FileNode::USER;
            }
            𐡄𐡍 ( d == '<') //support of full type heading from YAML 1.2
            {
                ثابت حرف* yamlTypeHeading = "<tag:yaml.org,2002:";
                ثابت size_t headingLength = strlen(yamlTypeHeading);

                حرف* typeEndPtr = ++ptr;

                𐡏𐡁𐡃 d = *++typeEndPtr;
                طالما( cv_isprint(d) && d != ' ' && d != '>' );

                𐡄𐡍 ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    𐡄𐡍 ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = 𐡒𐡔𐡈;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            𐡏𐡁𐡃 d = *++endptr;
            طالما( cv_isprint(d) && d != ' ' );
            len = (صحيح)(endptr - ptr);
            𐡄𐡍( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            𐡄𐡍( len == 3 && !is_user )
            {
                𐡄𐡍( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                وإلا 𐡄𐡍( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                وإلا 𐡄𐡍( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                وإلا 𐡄𐡍( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            وإلا 𐡄𐡍( len == 5 && !is_user )
            {
                𐡄𐡍( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            وإلا 𐡄𐡍 (len == 6 && is_user)
            {
                𐡄𐡍( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = 𐡒𐡔𐡈;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    𐡏𐡁𐡃 {
                        d = *++endptr;
                        𐡄𐡍 (d == '|')
                            اكسر;
                    } طالما (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            𐡄𐡍 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            𐡄𐡍( !is_user )
            {
                𐡄𐡍 (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    اذهب force_string;
                𐡄𐡍( value_type == FileNode::INT )
                    اذهب force_int;
                𐡄𐡍( value_type == FileNode::REAL )
                    اذهب force_real;
            }
        }

        𐡄𐡍 (is_binary_string)
        {
            صحيح indent = static_cast<صحيح>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        وإلا 𐡄𐡍( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            طالما( cv_isdigit(*endptr) )
                endptr++;
            𐡄𐡍( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                مزدوج fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            وإلا
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            𐡄𐡍( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        وإلا 𐡄𐡍( c == '\'' || c == '\"' ) // an explicit string
        {
            𐡄𐡍( c == '\'' )
                لكل( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    𐡄𐡍( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    وإلا 𐡄𐡍( c == '\'' )
                    {
                        c = *++ptr;
                        𐡄𐡍( c != '\'' )
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
                    𐡄𐡍( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    وإلا 𐡄𐡍( c == '\"' )
                    {
                        ++ptr;
                        اكسر;
                    }
                    وإلا 𐡄𐡍( c == '\\' )
                    {
                        d = *++ptr;
                        𐡄𐡍( d == '\'' )
                            buf[len++] = d;
                        وإلا 𐡄𐡍( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        وإلا 𐡄𐡍( d == 'n' )
                            buf[len++] = '\n';
                        وإلا 𐡄𐡍( d == 'r' )
                            buf[len++] = '\r';
                        وإلا 𐡄𐡍( d == 't' )
                            buf[len++] = '\t';
                        وإلا 𐡄𐡍( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            صحيح val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (صحيح)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            𐡄𐡍( endptr == ptr + is_hex )
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

            𐡄𐡍( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        وإلا 𐡄𐡍( c == '[' || c == '{' ) // collection as a flow
        {
            صحيح new_min_indent = min_indent + !is_parent_flow;
            صحيح struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            صحيح nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            لكل( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                𐡄𐡍 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                𐡄𐡍( *ptr == '}' || *ptr == ']' )
                {
                    𐡄𐡍( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    اكسر;
                }

                𐡄𐡍( nelems != 0 )
                {
                    𐡄𐡍( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    𐡄𐡍 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                𐡄𐡍( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                وإلا
                {
                    𐡄𐡍( *ptr == ']' )
                        اكسر;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, 𐡒𐡔𐡈, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        وإلا
        {
            صحيح indent, struct_type;

            𐡄𐡍( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                𐡄𐡍( !is_parent_flow )
                {
                    𐡄𐡍( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    𐡄𐡍( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                𐡏𐡁𐡃 c = *++endptr;
                طالما( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                𐡄𐡍( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                𐡄𐡍( is_parent_flow || c != ':' )
                {
                    حرف* str_end = endptr;
                    𐡏𐡁𐡃 c = *--str_end;
                    طالما( str_end > ptr && c == ' ' );
                    str_end++;
                    𐡄𐡍 ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        𐡄𐡍 (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا 𐡄𐡍 (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            أرجع ptr;
                        }
                        وإلا 𐡄𐡍 (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            أرجع ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (صحيح)(str_end - ptr));
                    ptr = endptr;
                    أرجع ptr;
                }
                struct_type = FileNode::MAP;
            }
            وإلا
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (صحيح)(ptr - fs->bufferStart());

            لكل(;;)
            {
                FileNode elem;

                𐡄𐡍( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                وإلا
                {
                    c = *ptr++;
                    𐡄𐡍( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, خطأ, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                𐡄𐡍( ptr - fs->bufferStart() != indent )
                {
                    𐡄𐡍( ptr - fs->bufferStart() < indent )
                        اكسر;
                    وإلا
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                𐡄𐡍( memcmp( ptr, "...", 3 ) == 0 )
                    اكسر;
            }
            fs->finalizeCollection(node);
        }

        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        منطقي first = 𐡒𐡔𐡈;
        منطقي ok = 𐡒𐡔𐡈;
        FileNode root_collection(fs->getFS(), 0, 0);

        لكل(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            لكل(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                𐡄𐡍( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    اكسر;
                }

                𐡄𐡍( *ptr == '%' )
                {
                    𐡄𐡍( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        𐡄𐡍 ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                وإلا 𐡄𐡍( *ptr == '-' )
                {
                    𐡄𐡍( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        اكسر;
                    }
                    وإلا 𐡄𐡍( first )
                        اكسر;
                }
                وإلا 𐡄𐡍( cv_isalnum(*ptr) || *ptr=='_')
                {
                    𐡄𐡍( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    اكسر;
                }
                وإلا 𐡄𐡍( fs->eof() )
                    اكسر;
                وإلا
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            𐡄𐡍( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            𐡄𐡍( !ptr || !ptr[0] )
                اكسر;
            𐡄𐡍( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, خطأ );
                𐡄𐡍( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                𐡄𐡍( !ptr )
                    اكسر;
            }

            𐡄𐡍( fs->eof() )
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
