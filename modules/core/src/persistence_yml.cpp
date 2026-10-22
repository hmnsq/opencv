// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence.hpp"

எண்ணிக்கை
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

நாமாகாஶ cv
{

வகுப்பு YAMLEmitter : பொது FileStorageEmitter
{
பொது:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    மெய்நிகர் ~YAMLEmitter() {}

    FStructData startWriteStruct(மாறிலி FStructData& parent, மாறிலி எழுத்து* சாவி,
                                 முழு struct_flags, மாறிலி எழுத்து* type_name=0)
    {
        எழுத்து buf[CV_FS_MAX_LEN + 1024];
        மாறிலி எழுத்து* data = 0;

        எனில் ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_முனை::TYPE_MASK|File_முனை::FLOW)) | File_முனை::EMPTY;
        எனில்( !File_முனை::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        எனில் (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_முனை::SEQ;
            snprintf(buf, அளவு(buf), "!!binary |");
            data = buf;
        }
        இல்லையெனில் எனில்( File_முனை::isFlow(struct_flags))
        {
            எழுத்து c = File_முனை::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_முனை::FLOW;

            எனில்( type_name )
                snprintf( buf, அளவு(buf), "!!%s %c", type_name, c );
            இல்லையெனில்
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        இல்லையெனில் எனில்( type_name )
        {
            snprintf( buf, அளவு(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( சாவி, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        எனில்( !File_முனை::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_முனை::isFlow(struct_flags);

        திரும்பு fsd;
    }

    வெறுமை endWriteStruct(மாறிலி FStructData& current_struct)
    {
        எழுத்து* ptr;

        முழு struct_flags = current_struct.flags;

        எனில்( File_முனை::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            எனில்( ptr > fs->bufferStart() + current_struct.indent && !File_முனை::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_முனை::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        இல்லையெனில் எனில்( File_முனை::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_முனை::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, முழு value)
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::itoa( value, buf, 10 ));
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, int64_t value)
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::itoa( value, buf, 10, உண்மை ));
    }

    வெறுமை லிக( மாறிலி எழுத்து* சாவி, இரட்டை value )
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::doubleToString( buf, அளவு(buf), value, பொய் ));
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* str, தருக்க quote)
    {
        எழுத்து buf[CV_FS_MAX_LEN*4+16];
        எழுத்து* data = (எழுத்து*)str;
        முழு i, len;

        எனில்( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (முழு)strlen(str);
        எனில்( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        எனில்( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            முழு need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            க்கு( i = 0; i < len; i++ )
            {
                எழுத்து c = str[i];

                எனில்( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                எனில்( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    எனில்( cv_isprint(c) )
                        *data++ = c;
                    இல்லையெனில் எனில்( c == '\n' )
                        *data++ = 'n';
                    இல்லையெனில் எனில்( c == '\r' )
                        *data++ = 'r';
                    இல்லையெனில் எனில்( c == '\t' )
                        *data++ = 't';
                    இல்லையெனில்
                    {
                        snprintf( data, அளவு(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                இல்லையெனில்
                    *data++ = c;
            }
            எனில்( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            எனில்( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( சாவி, data);
    }

    வெறுமை writeScalar(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* data)
    {
        fs->check_if_write_struct_is_delayed(பொய்);
        எனில் ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        இல்லையெனில் எனில் ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        முழு i, keylen = 0;
        முழு datalen = 0;
        எழுத்து* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        முழு struct_flags = current_struct.flags;

        எனில்( சாவி && சாவி[0] == '\0' )
            சாவி = 0;

        எனில்( File_முனை::isCollection(struct_flags) )
        {
            எனில்( (File_முனை::isMap(struct_flags) ^ (சாவி != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        இல்லையெனில்
        {
            fs->setNonEmpty();
            struct_flags = File_முனை::EMPTY | (சாவி ? File_முனை::MAP : File_முனை::SEQ);
        }

        எனில்( சாவி )
        {
            keylen = (முழு)strlen(சாவி);
            எனில்( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            எனில்( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        எனில்( data )
            datalen = (முழு)strlen(data);

        எனில்( File_முனை::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            எனில்( !File_முனை::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            முழு new_offset = (முழு)(ptr - fs->bufferStart()) + keylen + datalen;
            எனில்( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            இல்லையெனில்
                *ptr++ = ' ';
        }
        இல்லையெனில்
        {
            ptr = fs->flush();
            எனில்( !File_முனை::isMap(struct_flags) )
            {
                *ptr++ = '-';
                எனில்( data )
                    *ptr++ = ' ';
            }
        }

        எனில்( சாவி )
        {
            எனில்( !cv_isalpha(சாவி[0]) && சாவி[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            க்கு( i = 0; i < keylen; i++ )
            {
                எழுத்து c = சாவி[i];

                ptr[i] = c;
                எனில்( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            எனில்( !File_முனை::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        எனில்( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_முனை::EMPTY;
    }

    வெறுமை writeComment(மாறிலி எழுத்து* comment, தருக்க eol_comment)
    {
        எனில்( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        முழு len = (முழு)strlen(comment);
        மாறிலி எழுத்து* eol = strchr(comment, '\n');
        தருக்க multiline = eol != 0;
        எழுத்து* ptr = fs->bufferPtr();

        எனில்( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        இல்லையெனில்
            *ptr++ = ' ';

        போது( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            எனில்( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (முழு)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            இல்லையெனில்
            {
                len = (முழு)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    வெறுமை startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

பாதுகாக்கப்பட்ட:
    FileStorage_API* fs;
};


வகுப்பு YAMLParser : பொது FileStorageParser
{
பொது:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    மெய்நிகர் ~YAMLParser() {}

    எழுத்து* skipSpaces( எழுத்து* ptr, முழு min_indent, முழு max_comment_indent )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        க்கு(;;)
        {
            போது( *ptr == ' ' )
                ptr++;
            எனில்( *ptr == '#' )
            {
                எனில்( ptr - fs->bufferStart() > max_comment_indent )
                    திரும்பு ptr;
                *ptr = '\0';
            }
            இல்லையெனில் எனில்( cv_isprint(*ptr) )
            {
                எனில்( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                நிறுத்து;
            }
            இல்லையெனில் எனில்( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                எனில்( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    நிறுத்து;
                }
                இல்லையெனில்
                {
                    முழு l = (முழு)strlen(ptr);
                    எனில்( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            இல்லையெனில்
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        திரும்பு ptr;
    }

    தருக்க getBase64Row(எழுத்து* ptr, முழு indent, எழுத்து* &beg, எழுத்து* &end)
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        எனில் (!ptr || !*ptr)
            திரும்பு பொய்; // end of file

        எனில் (ptr - fs->bufferStart() != indent)
            திரும்பு பொய்; // end of base64 data

        /* find end */
        போது(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        எனில் (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        திரும்பு உண்மை;
    }


    எழுத்து* parse_சாவி( எழுத்து* ptr, File_முனை& map_முனை, File_முனை& மதிப்பு_placeholder )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        எழுத்து c;
        எழுத்து *endptr = ptr - 1, *saveptr;

        எனில்( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        செய் c = *++endptr;
        போது( cv_isprint(c) && c != ':' );

        எனில்( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        எனில்( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        செய் c = *--endptr;
        போது( c == ' ' );

        ++endptr;

        மதிப்பு_placeholder = fs->add_முனை(map_முனை, std::string(ptr, endptr - ptr), File_முனை::NONE);
        ptr = saveptr;

        திரும்பு ptr;
    }

    எழுத்து* parse_மதிப்பு( எழுத்து* ptr, File_முனை& முனை, முழு min_indent, தருக்க is_parent_flow, முழு depth = 0 )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        எனில்( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        எழுத்து* endptr = 0;
        எழுத்து c = ptr[0], d = ptr[1];
        முழு value_type = File_முனை::NONE;
        முழு len;
        தருக்க is_இருமம்_string = பொய்;
        தருக்க is_user = பொய்;

        எனில்( c == '!' ) // handle explicit type specification
        {
            எனில்( d == '!' || d == '^' )
            {
                ptr++;
                is_user = உண்மை;
                //value_type |= FileNode::USER;
            }
            எனில் ( d == '<') //support of full type heading from YAML 1.2
            {
                மாறிலி எழுத்து* yamlTypeHeading = "<tag:yaml.org,2002:";
                மாறிலி size_t headingLength = strlen(yamlTypeHeading);

                எழுத்து* typeEndPtr = ++ptr;

                செய் d = *++typeEndPtr;
                போது( cv_isprint(d) && d != ' ' && d != '>' );

                எனில் ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    எனில் ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = உண்மை;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            செய் d = *++endptr;
            போது( cv_isprint(d) && d != ' ' );
            len = (முழு)(endptr - ptr);
            எனில்( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            எனில்( len == 3 && !is_user )
            {
                எனில்( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_முனை::STRING;
                இல்லையெனில் எனில்( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_முனை::INT;
                இல்லையெனில் எனில்( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_முனை::SEQ;
                இல்லையெனில் எனில்( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_முனை::MAP;
            }
            இல்லையெனில் எனில்( len == 5 && !is_user )
            {
                எனில்( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_முனை::REAL;
            }
            இல்லையெனில் எனில் (len == 6 && is_user)
            {
                எனில்( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_முனை::SEQ;
                    is_இருமம்_string = உண்மை;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    செய் {
                        d = *++endptr;
                        எனில் (d == '|')
                            நிறுத்து;
                    } போது (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            எனில் (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            எனில்( !is_user )
            {
                எனில் (value_type == File_முனை::STRING && c != '\'' && c != '\"')
                    செல் force_string;
                எனில்( value_type == File_முனை::INT )
                    செல் force_int;
                எனில்( value_type == File_முனை::REAL )
                    செல் force_real;
            }
        }

        எனில் (is_இருமம்_string)
        {
            முழு indent = static_cast<முழு>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, முனை);
        }
        இல்லையெனில் எனில்( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            போது( cv_isdigit(*endptr) )
                endptr++;
            எனில்( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                இரட்டை fval = fs->strtod( ptr, &endptr );
                முனை.set_மதிப்பு(File_முனை::REAL, &fval);
            }
            இல்லையெனில்
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                முனை.set_மதிப்பு(File_முனை::INT, &ival);
            }

            எனில்( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        இல்லையெனில் எனில்( c == '\'' || c == '\"' ) // an explicit string
        {
            எனில்( c == '\'' )
                க்கு( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    எனில்( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    இல்லையெனில் எனில்( c == '\'' )
                    {
                        c = *++ptr;
                        எனில்( c != '\'' )
                            நிறுத்து;
                        buf[len++] = c;
                    }
                    இல்லையெனில்
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            இல்லையெனில்
                க்கு( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    எனில்( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    இல்லையெனில் எனில்( c == '\"' )
                    {
                        ++ptr;
                        நிறுத்து;
                    }
                    இல்லையெனில் எனில்( c == '\\' )
                    {
                        d = *++ptr;
                        எனில்( d == '\'' )
                            buf[len++] = d;
                        இல்லையெனில் எனில்( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        இல்லையெனில் எனில்( d == 'n' )
                            buf[len++] = '\n';
                        இல்லையெனில் எனில்( d == 'r' )
                            buf[len++] = '\r';
                        இல்லையெனில் எனில்( d == 't' )
                            buf[len++] = '\t';
                        இல்லையெனில் எனில்( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            முழு val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (முழு)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            எனில்( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            இல்லையெனில்
                            {
                                buf[len++] = (எழுத்து)val;
                                ptr = endptr;
                            }
                        }
                    }
                    இல்லையெனில்
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            எனில்( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            முனை.set_மதிப்பு(File_முனை::STRING, buf, len);
        }
        இல்லையெனில் எனில்( c == '[' || c == '{' ) // collection as a flow
        {
            முழு new_min_indent = min_indent + !is_parent_flow;
            முழு struct_type = c == '{' ? File_முனை::MAP : File_முனை::SEQ;
            முழு nelems = 0;

            fs->convertToCollection(struct_type, முனை);
            d = c == '[' ? ']' : '}';

            க்கு( ++ptr ;; nelems++ )
            {
                File_முனை elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                எனில் (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                எனில்( *ptr == '}' || *ptr == ']' )
                {
                    எனில்( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    நிறுத்து;
                }

                எனில்( nelems != 0 )
                {
                    எனில்( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    எனில் (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                எனில்( struct_type == File_முனை::MAP )
                {
                    ptr = parse_சாவி( ptr, முனை, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                இல்லையெனில்
                {
                    எனில்( *ptr == ']' )
                        நிறுத்து;
                    elem = fs->add_முனை(முனை, std::string(), File_முனை::NONE);
                }
                ptr = parse_மதிப்பு( ptr, elem, new_min_indent, உண்மை, depth + 1 );
            }
            fs->finalizeCollection(முனை);
        }
        இல்லையெனில்
        {
            முழு indent, struct_type;

            எனில்( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                எனில்( !is_parent_flow )
                {
                    எனில்( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    எனில்( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                செய் c = *++endptr;
                போது( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_முனை::STRING));

                எனில்( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                எனில்( is_parent_flow || c != ':' )
                {
                    எழுத்து* str_end = endptr;
                    செய் c = *--str_end;
                    போது( str_end > ptr && c == ' ' );
                    str_end++;
                    எனில் ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        எனில் (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            முனை.set_மதிப்பு(File_முனை::INT, &ival);
                            ptr = endptr;
                            திரும்பு ptr;
                        }
                        இல்லையெனில் எனில் (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            முனை.set_மதிப்பு(File_முனை::INT, &ival);
                            ptr = endptr;
                            திரும்பு ptr;
                        }
                        இல்லையெனில் எனில் (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            திரும்பு ptr;
                        }
                    }

                    முனை.set_மதிப்பு(File_முனை::STRING, ptr, (முழு)(str_end - ptr));
                    ptr = endptr;
                    திரும்பு ptr;
                }
                struct_type = File_முனை::MAP;
            }
            இல்லையெனில்
                struct_type = File_முனை::SEQ;

            fs->convertToCollection( struct_type, முனை );
            indent = (முழு)(ptr - fs->bufferStart());

            க்கு(;;)
            {
                File_முனை elem;

                எனில்( struct_type == File_முனை::MAP )
                {
                    ptr = parse_சாவி( ptr, முனை, elem );
                }
                இல்லையெனில்
                {
                    c = *ptr++;
                    எனில்( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_முனை(முனை, std::string(), File_முனை::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_மதிப்பு( ptr, elem, indent + 1, பொய், depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                எனில்( ptr - fs->bufferStart() != indent )
                {
                    எனில்( ptr - fs->bufferStart() < indent )
                        நிறுத்து;
                    இல்லையெனில்
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                எனில்( memcmp( ptr, "...", 3 ) == 0 )
                    நிறுத்து;
            }
            fs->finalizeCollection(முனை);
        }

        திரும்பு ptr;
    }

    தருக்க parse( எழுத்து* ptr )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        தருக்க first = உண்மை;
        தருக்க ok = உண்மை;
        File_முனை வேர்_collection(fs->getFS(), 0, 0);

        க்கு(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            க்கு(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                எனில்( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    நிறுத்து;
                }

                எனில்( *ptr == '%' )
                {
                    எனில்( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        எனில் ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                இல்லையெனில் எனில்( *ptr == '-' )
                {
                    எனில்( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        நிறுத்து;
                    }
                    இல்லையெனில் எனில்( first )
                        நிறுத்து;
                }
                இல்லையெனில் எனில்( cv_isalnum(*ptr) || *ptr=='_')
                {
                    எனில்( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    நிறுத்து;
                }
                இல்லையெனில் எனில்( fs->eof() )
                    நிறுத்து;
                இல்லையெனில்
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            எனில்( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            எனில்( !ptr || !ptr[0] )
                நிறுத்து;
            எனில்( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_முனை வேர்_முனை = fs->add_முனை(வேர்_collection, std::string(), File_முனை::NONE);

                ptr = parse_மதிப்பு( ptr, வேர்_முனை, 0, பொய் );
                எனில்( !வேர்_முனை.isMap() && !வேர்_முனை.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                எனில்( !ptr )
                    நிறுத்து;
            }

            எனில்( fs->eof() )
                நிறுத்து;
            ptr += 3;
            first = பொய்;
        }

        திரும்பு ok;
    }

    FileStorage_API* fs;
    எழுத்து buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    திரும்பு makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    திரும்பு makePtr<YAMLParser>(fs);
}

}
