// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "persistence.hpp"

গণনা
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

নামাকাশ cv
{

শ্ৰেণী YAMLEmitter : সার্ৱজনিক FileStorageEmitter
{
সার্ৱজনিক:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    আভাস ~YAMLEmitter() {}

    FStructData startWriteStruct(ধ্রুৱ FStructData& parent, ধ্রুৱ ৱর্ণ* চাবি,
                                 পূর্ণাঙ্ক struct_flags, ধ্রুৱ ৱর্ণ* type_name=0)
    {
        ৱর্ণ buf[CV_FS_MAX_LEN + 1024];
        ধ্রুৱ ৱর্ণ* data = 0;

        যদি ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_গ্ৰন্থি::TYPE_MASK|File_গ্ৰন্থি::FLOW)) | File_গ্ৰন্থি::EMPTY;
        যদি( !File_গ্ৰন্থি::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        যদি (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_গ্ৰন্থি::SEQ;
            snprintf(buf, পরিমাণ(buf), "!!binary |");
            data = buf;
        }
        অন্যথা যদি( File_গ্ৰন্থি::isFlow(struct_flags))
        {
            ৱর্ণ c = File_গ্ৰন্থি::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_গ্ৰন্থি::FLOW;

            যদি( type_name )
                snprintf( buf, পরিমাণ(buf), "!!%s %c", type_name, c );
            অন্যথা
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        অন্যথা যদি( type_name )
        {
            snprintf( buf, পরিমাণ(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( চাবি, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        যদি( !File_গ্ৰন্থি::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_গ্ৰন্থি::isFlow(struct_flags);

        ঘূৰাই fsd;
    }

    শূন্য endWriteStruct(ধ্রুৱ FStructData& current_struct)
    {
        ৱর্ণ* ptr;

        পূর্ণাঙ্ক struct_flags = current_struct.flags;

        যদি( File_গ্ৰন্থি::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            যদি( ptr > fs->bufferStart() + current_struct.indent && !File_গ্ৰন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_গ্ৰন্থি::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        অন্যথা যদি( File_গ্ৰন্থি::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_গ্ৰন্থি::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, পূর্ণাঙ্ক value)
    {
        ৱর্ণ buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10 ));
    }

    শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, int64_t value)
    {
        ৱর্ণ buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10, সঁচা ));
    }

    শূন্য লিখ( ধ্রুৱ ৱর্ণ* চাবি, দ্ৱিগুণ value )
    {
        ৱর্ণ buf[128];
        writeScalar( চাবি, fs::doubleToString( buf, পরিমাণ(buf), value, মিছা ));
    }

    শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, ধ্রুৱ ৱর্ণ* str, তার্কিক quote)
    {
        ৱর্ণ buf[CV_FS_MAX_LEN*4+16];
        ৱর্ণ* data = (ৱর্ণ*)str;
        পূর্ণাঙ্ক i, len;

        যদি( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (পূর্ণাঙ্ক)strlen(str);
        যদি( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        যদি( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            পূর্ণাঙ্ক need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            বাবে( i = 0; i < len; i++ )
            {
                ৱর্ণ c = str[i];

                যদি( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                যদি( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    যদি( cv_isprint(c) )
                        *data++ = c;
                    অন্যথা যদি( c == '\n' )
                        *data++ = 'n';
                    অন্যথা যদি( c == '\r' )
                        *data++ = 'r';
                    অন্যথা যদি( c == '\t' )
                        *data++ = 't';
                    অন্যথা
                    {
                        snprintf( data, পরিমাণ(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                অন্যথা
                    *data++ = c;
            }
            যদি( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            যদি( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( চাবি, data);
    }

    শূন্য writeScalar(ধ্রুৱ ৱর্ণ* চাবি, ধ্রুৱ ৱর্ণ* data)
    {
        fs->check_if_write_struct_is_delayed(মিছা);
        যদি ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        অন্যথা যদি ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        পূর্ণাঙ্ক i, keylen = 0;
        পূর্ণাঙ্ক datalen = 0;
        ৱর্ণ* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        পূর্ণাঙ্ক struct_flags = current_struct.flags;

        যদি( চাবি && চাবি[0] == '\0' )
            চাবি = 0;

        যদি( File_গ্ৰন্থি::isCollection(struct_flags) )
        {
            যদি( (File_গ্ৰন্থি::isMap(struct_flags) ^ (চাবি != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        অন্যথা
        {
            fs->setNonEmpty();
            struct_flags = File_গ্ৰন্থি::EMPTY | (চাবি ? File_গ্ৰন্থি::MAP : File_গ্ৰন্থি::SEQ);
        }

        যদি( চাবি )
        {
            keylen = (পূর্ণাঙ্ক)strlen(চাবি);
            যদি( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            যদি( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        যদি( data )
            datalen = (পূর্ণাঙ্ক)strlen(data);

        যদি( File_গ্ৰন্থি::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            যদি( !File_গ্ৰন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            পূর্ণাঙ্ক new_offset = (পূর্ণাঙ্ক)(ptr - fs->bufferStart()) + keylen + datalen;
            যদি( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            অন্যথা
                *ptr++ = ' ';
        }
        অন্যথা
        {
            ptr = fs->flush();
            যদি( !File_গ্ৰন্থি::isMap(struct_flags) )
            {
                *ptr++ = '-';
                যদি( data )
                    *ptr++ = ' ';
            }
        }

        যদি( চাবি )
        {
            যদি( !cv_isalpha(চাবি[0]) && চাবি[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            বাবে( i = 0; i < keylen; i++ )
            {
                ৱর্ণ c = চাবি[i];

                ptr[i] = c;
                যদি( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            যদি( !File_গ্ৰন্থি::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        যদি( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_গ্ৰন্থি::EMPTY;
    }

    শূন্য writeComment(ধ্রুৱ ৱর্ণ* comment, তার্কিক eol_comment)
    {
        যদি( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)strlen(comment);
        ধ্রুৱ ৱর্ণ* eol = strchr(comment, '\n');
        তার্কিক multiline = eol != 0;
        ৱর্ণ* ptr = fs->bufferPtr();

        যদি( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        অন্যথা
            *ptr++ = ' ';

        যেতিয়ালৈ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            যদি( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (পূর্ণাঙ্ক)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            অন্যথা
            {
                len = (পূর্ণাঙ্ক)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    শূন্য startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

রক্ষিত:
    FileStorage_API* fs;
};


শ্ৰেণী YAMLParser : সার্ৱজনিক FileStorageParser
{
সার্ৱজনিক:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    আভাস ~YAMLParser() {}

    ৱর্ণ* skipSpaces( ৱর্ণ* ptr, পূর্ণাঙ্ক min_indent, পূর্ণাঙ্ক max_comment_indent )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        বাবে(;;)
        {
            যেতিয়ালৈ( *ptr == ' ' )
                ptr++;
            যদি( *ptr == '#' )
            {
                যদি( ptr - fs->bufferStart() > max_comment_indent )
                    ঘূৰাই ptr;
                *ptr = '\0';
            }
            অন্যথা যদি( cv_isprint(*ptr) )
            {
                যদি( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ভঙ্গ;
            }
            অন্যথা যদি( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                যদি( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    ভঙ্গ;
                }
                অন্যথা
                {
                    পূর্ণাঙ্ক l = (পূর্ণাঙ্ক)strlen(ptr);
                    যদি( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            অন্যথা
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ঘূৰাই ptr;
    }

    তার্কিক getBase64Row(ৱর্ণ* ptr, পূর্ণাঙ্ক indent, ৱর্ণ* &beg, ৱর্ণ* &end)
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        যদি (!ptr || !*ptr)
            ঘূৰাই মিছা; // end of file

        যদি (ptr - fs->bufferStart() != indent)
            ঘূৰাই মিছা; // end of base64 data

        /* find end */
        যেতিয়ালৈ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        যদি (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ঘূৰাই সঁচা;
    }


    ৱর্ণ* parse_চাবি( ৱর্ণ* ptr, File_গ্ৰন্থি& map_গ্ৰন্থি, File_গ্ৰন্থি& মান_placeholder )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ৱর্ণ c;
        ৱর্ণ *endptr = ptr - 1, *saveptr;

        যদি( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        কৰা c = *++endptr;
        যেতিয়ালৈ( cv_isprint(c) && c != ':' );

        যদি( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        যদি( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        কৰা c = *--endptr;
        যেতিয়ালৈ( c == ' ' );

        ++endptr;

        মান_placeholder = fs->add_গ্ৰন্থি(map_গ্ৰন্থি, std::string(ptr, endptr - ptr), File_গ্ৰন্থি::NONE);
        ptr = saveptr;

        ঘূৰাই ptr;
    }

    ৱর্ণ* parse_মান( ৱর্ণ* ptr, File_গ্ৰন্থি& গ্ৰন্থি, পূর্ণাঙ্ক min_indent, তার্কিক is_parent_flow, পূর্ণাঙ্ক depth = 0 )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ৱর্ণ* endptr = 0;
        ৱর্ণ c = ptr[0], d = ptr[1];
        পূর্ণাঙ্ক value_type = File_গ্ৰন্থি::NONE;
        পূর্ণাঙ্ক len;
        তার্কিক is_দ্বিমিক_string = মিছা;
        তার্কিক is_user = মিছা;

        যদি( c == '!' ) // handle explicit type specification
        {
            যদি( d == '!' || d == '^' )
            {
                ptr++;
                is_user = সঁচা;
                //value_type |= FileNode::USER;
            }
            যদি ( d == '<') //support of full type heading from YAML 1.2
            {
                ধ্রুৱ ৱর্ণ* yamlTypeHeading = "<tag:yaml.org,2002:";
                ধ্রুৱ size_t headingLength = strlen(yamlTypeHeading);

                ৱর্ণ* typeEndPtr = ++ptr;

                কৰা d = *++typeEndPtr;
                যেতিয়ালৈ( cv_isprint(d) && d != ' ' && d != '>' );

                যদি ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    যদি ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = সঁচা;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            কৰা d = *++endptr;
            যেতিয়ালৈ( cv_isprint(d) && d != ' ' );
            len = (পূর্ণাঙ্ক)(endptr - ptr);
            যদি( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            যদি( len == 3 && !is_user )
            {
                যদি( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_গ্ৰন্থি::STRING;
                অন্যথা যদি( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_গ্ৰন্থি::INT;
                অন্যথা যদি( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_গ্ৰন্থি::SEQ;
                অন্যথা যদি( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_গ্ৰন্থি::MAP;
            }
            অন্যথা যদি( len == 5 && !is_user )
            {
                যদি( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_গ্ৰন্থি::REAL;
            }
            অন্যথা যদি (len == 6 && is_user)
            {
                যদি( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_গ্ৰন্থি::SEQ;
                    is_দ্বিমিক_string = সঁচা;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    কৰা {
                        d = *++endptr;
                        যদি (d == '|')
                            ভঙ্গ;
                    } যেতিয়ালৈ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            যদি (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            যদি( !is_user )
            {
                যদি (value_type == File_গ্ৰন্থি::STRING && c != '\'' && c != '\"')
                    গচ্ছ force_string;
                যদি( value_type == File_গ্ৰন্থি::INT )
                    গচ্ছ force_int;
                যদি( value_type == File_গ্ৰন্থি::REAL )
                    গচ্ছ force_real;
            }
        }

        যদি (is_দ্বিমিক_string)
        {
            পূর্ণাঙ্ক indent = static_cast<পূর্ণাঙ্ক>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, গ্ৰন্থি);
        }
        অন্যথা যদি( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            যেতিয়ালৈ( cv_isdigit(*endptr) )
                endptr++;
            যদি( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                দ্ৱিগুণ fval = fs->strtod( ptr, &endptr );
                গ্ৰন্থি.set_মান(File_গ্ৰন্থি::REAL, &fval);
            }
            অন্যথা
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                গ্ৰন্থি.set_মান(File_গ্ৰন্থি::INT, &ival);
            }

            যদি( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        অন্যথা যদি( c == '\'' || c == '\"' ) // an explicit string
        {
            যদি( c == '\'' )
                বাবে( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    যদি( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    অন্যথা যদি( c == '\'' )
                    {
                        c = *++ptr;
                        যদি( c != '\'' )
                            ভঙ্গ;
                        buf[len++] = c;
                    }
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            অন্যথা
                বাবে( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    যদি( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    অন্যথা যদি( c == '\"' )
                    {
                        ++ptr;
                        ভঙ্গ;
                    }
                    অন্যথা যদি( c == '\\' )
                    {
                        d = *++ptr;
                        যদি( d == '\'' )
                            buf[len++] = d;
                        অন্যথা যদি( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        অন্যথা যদি( d == 'n' )
                            buf[len++] = '\n';
                        অন্যথা যদি( d == 'r' )
                            buf[len++] = '\r';
                        অন্যথা যদি( d == 't' )
                            buf[len++] = '\t';
                        অন্যথা যদি( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            পূর্ণাঙ্ক val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (পূর্ণাঙ্ক)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            যদি( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            অন্যথা
                            {
                                buf[len++] = (ৱর্ণ)val;
                                ptr = endptr;
                            }
                        }
                    }
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            যদি( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            গ্ৰন্থি.set_মান(File_গ্ৰন্থি::STRING, buf, len);
        }
        অন্যথা যদি( c == '[' || c == '{' ) // collection as a flow
        {
            পূর্ণাঙ্ক new_min_indent = min_indent + !is_parent_flow;
            পূর্ণাঙ্ক struct_type = c == '{' ? File_গ্ৰন্থি::MAP : File_গ্ৰন্থি::SEQ;
            পূর্ণাঙ্ক nelems = 0;

            fs->convertToCollection(struct_type, গ্ৰন্থি);
            d = c == '[' ? ']' : '}';

            বাবে( ++ptr ;; nelems++ )
            {
                File_গ্ৰন্থি elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                যদি (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                যদি( *ptr == '}' || *ptr == ']' )
                {
                    যদি( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ভঙ্গ;
                }

                যদি( nelems != 0 )
                {
                    যদি( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    যদি (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                যদি( struct_type == File_গ্ৰন্থি::MAP )
                {
                    ptr = parse_চাবি( ptr, গ্ৰন্থি, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                অন্যথা
                {
                    যদি( *ptr == ']' )
                        ভঙ্গ;
                    elem = fs->add_গ্ৰন্থি(গ্ৰন্থি, std::string(), File_গ্ৰন্থি::NONE);
                }
                ptr = parse_মান( ptr, elem, new_min_indent, সঁচা, depth + 1 );
            }
            fs->finalizeCollection(গ্ৰন্থি);
        }
        অন্যথা
        {
            পূর্ণাঙ্ক indent, struct_type;

            যদি( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                যদি( !is_parent_flow )
                {
                    যদি( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    যদি( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                কৰা c = *++endptr;
                যেতিয়ালৈ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_গ্ৰন্থি::STRING));

                যদি( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                যদি( is_parent_flow || c != ':' )
                {
                    ৱর্ণ* str_end = endptr;
                    কৰা c = *--str_end;
                    যেতিয়ালৈ( str_end > ptr && c == ' ' );
                    str_end++;
                    যদি ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        যদি (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            গ্ৰন্থি.set_মান(File_গ্ৰন্থি::INT, &ival);
                            ptr = endptr;
                            ঘূৰাই ptr;
                        }
                        অন্যথা যদি (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            গ্ৰন্থি.set_মান(File_গ্ৰন্থি::INT, &ival);
                            ptr = endptr;
                            ঘূৰাই ptr;
                        }
                        অন্যথা যদি (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ঘূৰাই ptr;
                        }
                    }

                    গ্ৰন্থি.set_মান(File_গ্ৰন্থি::STRING, ptr, (পূর্ণাঙ্ক)(str_end - ptr));
                    ptr = endptr;
                    ঘূৰাই ptr;
                }
                struct_type = File_গ্ৰন্থি::MAP;
            }
            অন্যথা
                struct_type = File_গ্ৰন্থি::SEQ;

            fs->convertToCollection( struct_type, গ্ৰন্থি );
            indent = (পূর্ণাঙ্ক)(ptr - fs->bufferStart());

            বাবে(;;)
            {
                File_গ্ৰন্থি elem;

                যদি( struct_type == File_গ্ৰন্থি::MAP )
                {
                    ptr = parse_চাবি( ptr, গ্ৰন্থি, elem );
                }
                অন্যথা
                {
                    c = *ptr++;
                    যদি( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_গ্ৰন্থি(গ্ৰন্থি, std::string(), File_গ্ৰন্থি::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_মান( ptr, elem, indent + 1, মিছা, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( ptr - fs->bufferStart() != indent )
                {
                    যদি( ptr - fs->bufferStart() < indent )
                        ভঙ্গ;
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                যদি( memcmp( ptr, "...", 3 ) == 0 )
                    ভঙ্গ;
            }
            fs->finalizeCollection(গ্ৰন্থি);
        }

        ঘূৰাই ptr;
    }

    তার্কিক parse( ৱর্ণ* ptr )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        তার্কিক first = সঁচা;
        তার্কিক ok = সঁচা;
        File_গ্ৰন্থি মূল_collection(fs->getFS(), 0, 0);

        বাবে(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            বাবে(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ভঙ্গ;
                }

                যদি( *ptr == '%' )
                {
                    যদি( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        যদি ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                অন্যথা যদি( *ptr == '-' )
                {
                    যদি( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ভঙ্গ;
                    }
                    অন্যথা যদি( first )
                        ভঙ্গ;
                }
                অন্যথা যদি( cv_isalnum(*ptr) || *ptr=='_')
                {
                    যদি( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ভঙ্গ;
                }
                অন্যথা যদি( fs->eof() )
                    ভঙ্গ;
                অন্যথা
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            যদি( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            যদি( !ptr || !ptr[0] )
                ভঙ্গ;
            যদি( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_গ্ৰন্থি মূল_গ্ৰন্থি = fs->add_গ্ৰন্থি(মূল_collection, std::string(), File_গ্ৰন্থি::NONE);

                ptr = parse_মান( ptr, মূল_গ্ৰন্থি, 0, মিছা );
                যদি( !মূল_গ্ৰন্থি.isMap() && !মূল_গ্ৰন্থি.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( !ptr )
                    ভঙ্গ;
            }

            যদি( fs->eof() )
                ভঙ্গ;
            ptr += 3;
            first = মিছা;
        }

        ঘূৰাই ok;
    }

    FileStorage_API* fs;
    ৱর্ণ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ঘূৰাই makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ঘূৰাই makePtr<YAMLParser>(fs);
}

}
