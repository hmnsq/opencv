// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence.hpp"

গণনা
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

নামাকাশ cv
{

শ্রেণী YAMLEmitter : সর্বজনীন FileStorageEmitter
{
সর্বজনীন:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    আভাসী ~YAMLEmitter() {}

    FStruct_তথ্য startWriteStruct(ধ্রুবক FStruct_তথ্য& parent, ধ্রুবক অক্ষর* চাবি,
                                 পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর* type_name=0)
    {
        অক্ষর buf[CV_FS_MAX_LEN + 1024];
        ধ্রুবক অক্ষর* তথ্য = 0;

        যদি ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_গ্রন্থি::TYPE_MASK|File_গ্রন্থি::FLOW)) | File_গ্রন্থি::EMPTY;
        যদি( !File_গ্রন্থি::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        যদি (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_গ্রন্থি::SEQ;
            snprintf(buf, আকার(buf), "!!binary |");
            তথ্য = buf;
        }
        অন্যথা যদি( File_গ্রন্থি::isFlow(struct_flags))
        {
            অক্ষর c = File_গ্রন্থি::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_গ্রন্থি::FLOW;

            যদি( type_name )
                snprintf( buf, আকার(buf), "!!%s %c", type_name, c );
            অন্যথা
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            তথ্য = buf;
        }
        অন্যথা যদি( type_name )
        {
            snprintf( buf, আকার(buf), "!!%s", type_name );
            তথ্য = buf;
        }

        writeScalar( চাবি, তথ্য );

        FStruct_তথ্য fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        যদি( !File_গ্রন্থি::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_গ্রন্থি::isFlow(struct_flags);

        ফেরত fsd;
    }

    শূন্য endWriteStruct(ধ্রুবক FStruct_তথ্য& current_struct)
    {
        অক্ষর* ptr;

        পূর্ণসংখ্যা struct_flags = current_struct.flags;

        যদি( File_গ্রন্থি::isFlow(struct_flags) )
        {
            ptr = fs->বাফার_Ptr();
            যদি( ptr > fs->বাফার_Start() + current_struct.indent && !File_গ্রন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_গ্রন্থি::isMap(struct_flags) ? '}' : ']';
            fs->set_বাফার_Ptr(ptr);
        }
        অন্যথা যদি( File_গ্রন্থি::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_গ্রন্থি::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->set_বাফার_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা value)
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10 ));
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, int64_t value)
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10, সত্য ));
    }

    শূন্য লিখ( ধ্রুবক অক্ষর* চাবি, দ্বিগুণ value )
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::doubleToString( buf, আকার(buf), value, মিথ্যা ));
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* str, যৌক্তিক quote)
    {
        অক্ষর buf[CV_FS_MAX_LEN*4+16];
        অক্ষর* তথ্য = (অক্ষর*)str;
        পূর্ণসংখ্যা i, len;

        যদি( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (পূর্ণসংখ্যা)strlen(str);
        যদি( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        যদি( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            পূর্ণসংখ্যা need_quote = quote || len == 0 || str[0] == ' ';
            তথ্য = buf;
            *তথ্য++ = '\"';
            জন্য( i = 0; i < len; i++ )
            {
                অক্ষর c = str[i];

                যদি( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                যদি( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *তথ্য++ = '\\';
                    যদি( cv_isprint(c) )
                        *তথ্য++ = c;
                    অন্যথা যদি( c == '\n' )
                        *তথ্য++ = 'n';
                    অন্যথা যদি( c == '\r' )
                        *তথ্য++ = 'r';
                    অন্যথা যদি( c == '\t' )
                        *তথ্য++ = 't';
                    অন্যথা
                    {
                        snprintf( তথ্য, আকার(buf) - (তথ্য - buf), "x%02x", c );
                        তথ্য += 3;
                    }
                }
                অন্যথা
                    *তথ্য++ = c;
            }
            যদি( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            যদি( need_quote )
                *তথ্য++ = '\"';
            *তথ্য++ = '\0';
            তথ্য = buf + !need_quote;
        }

        writeScalar( চাবি, তথ্য);
    }

    শূন্য writeScalar(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* তথ্য)
    {
        fs->check_if_write_struct_is_delayed(মিথ্যা);
        যদি ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        অন্যথা যদি ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        পূর্ণসংখ্যা i, keylen = 0;
        পূর্ণসংখ্যা datalen = 0;
        অক্ষর* ptr;

        FStruct_তথ্য& current_struct = fs->getCurrentStruct();

        পূর্ণসংখ্যা struct_flags = current_struct.flags;

        যদি( চাবি && চাবি[0] == '\0' )
            চাবি = 0;

        যদি( File_গ্রন্থি::isCollection(struct_flags) )
        {
            যদি( (File_গ্রন্থি::isMap(struct_flags) ^ (চাবি != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        অন্যথা
        {
            fs->setNonEmpty();
            struct_flags = File_গ্রন্থি::EMPTY | (চাবি ? File_গ্রন্থি::MAP : File_গ্রন্থি::SEQ);
        }

        যদি( চাবি )
        {
            keylen = (পূর্ণসংখ্যা)strlen(চাবি);
            যদি( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            যদি( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        যদি( তথ্য )
            datalen = (পূর্ণসংখ্যা)strlen(তথ্য);

        যদি( File_গ্রন্থি::isFlow(struct_flags) )
        {
            ptr = fs->বাফার_Ptr();
            যদি( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            পূর্ণসংখ্যা new_offset = (পূর্ণসংখ্যা)(ptr - fs->বাফার_Start()) + keylen + datalen;
            যদি( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->set_বাফার_Ptr(ptr);
                ptr = fs->flush();
            }
            অন্যথা
                *ptr++ = ' ';
        }
        অন্যথা
        {
            ptr = fs->flush();
            যদি( !File_গ্রন্থি::isMap(struct_flags) )
            {
                *ptr++ = '-';
                যদি( তথ্য )
                    *ptr++ = ' ';
            }
        }

        যদি( চাবি )
        {
            যদি( !cv_isalpha(চাবি[0]) && চাবি[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_বাফার( ptr, keylen );

            জন্য( i = 0; i < keylen; i++ )
            {
                অক্ষর c = চাবি[i];

                ptr[i] = c;
                যদি( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            যদি( !File_গ্রন্থি::isFlow(struct_flags) && তথ্য )
                *ptr++ = ' ';
        }

        যদি( তথ্য )
        {
            ptr = fs->resize_Write_বাফার( ptr, datalen );
            memcpy( ptr, তথ্য, datalen );
            ptr += datalen;
        }

        fs->set_বাফার_Ptr(ptr);
        current_struct.flags &= ~File_গ্রন্থি::EMPTY;
    }

    শূন্য writeComment(ধ্রুবক অক্ষর* comment, যৌক্তিক eol_comment)
    {
        যদি( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)strlen(comment);
        ধ্রুবক অক্ষর* eol = strchr(comment, '\n');
        যৌক্তিক multiline = eol != 0;
        অক্ষর* ptr = fs->বাফার_Ptr();

        যদি( !eol_comment || multiline ||
            fs->বাফার_End() - ptr < len || ptr == fs->বাফার_Start() )
            ptr = fs->flush();
        অন্যথা
            *ptr++ = ' ';

        যখন( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            যদি( eol )
            {
                ptr = fs->resize_Write_বাফার( ptr, (পূর্ণসংখ্যা)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->set_বাফার_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            অন্যথা
            {
                len = (পূর্ণসংখ্যা)strlen(comment);
                ptr = fs->resize_Write_বাফার( ptr, len );
                memcpy( ptr, comment, len );
                fs->set_বাফার_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    শূন্য start_পরবর্তী_Stream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

সুরক্ষিত:
    FileStorage_API* fs;
};


শ্রেণী YAMLParser : সর্বজনীন FileStorageParser
{
সর্বজনীন:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    আভাসী ~YAMLParser() {}

    অক্ষর* skipSpaces( অক্ষর* ptr, পূর্ণসংখ্যা min_indent, পূর্ণসংখ্যা max_comment_indent )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        জন্য(;;)
        {
            যখন( *ptr == ' ' )
                ptr++;
            যদি( *ptr == '#' )
            {
                যদি( ptr - fs->বাফার_Start() > max_comment_indent )
                    ফেরত ptr;
                *ptr = '\0';
            }
            অন্যথা যদি( cv_isprint(*ptr) )
            {
                যদি( ptr - fs->বাফার_Start() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                বিরতি;
            }
            অন্যথা যদি( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                যদি( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->বাফার_Start();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    বিরতি;
                }
                অন্যথা
                {
                    পূর্ণসংখ্যা l = (পূর্ণসংখ্যা)strlen(ptr);
                    যদি( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            অন্যথা
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ফেরত ptr;
    }

    যৌক্তিক getBase64Row(অক্ষর* ptr, পূর্ণসংখ্যা indent, অক্ষর* &beg, অক্ষর* &end)
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        যদি (!ptr || !*ptr)
            ফেরত মিথ্যা; // end of file

        যদি (ptr - fs->বাফার_Start() != indent)
            ফেরত মিথ্যা; // end of base64 data

        /* find end */
        যখন(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        যদি (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ফেরত সত্য;
    }


    অক্ষর* parse_চাবি( অক্ষর* ptr, File_গ্রন্থি& map_গ্রন্থি, File_গ্রন্থি& মান_placeholder )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        অক্ষর c;
        অক্ষর *endptr = ptr - 1, *saveptr;

        যদি( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        করো c = *++endptr;
        যখন( cv_isprint(c) && c != ':' );

        যদি( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        যদি( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        করো c = *--endptr;
        যখন( c == ' ' );

        ++endptr;

        মান_placeholder = fs->add_গ্রন্থি(map_গ্রন্থি, std::string(ptr, endptr - ptr), File_গ্রন্থি::NONE);
        ptr = saveptr;

        ফেরত ptr;
    }

    অক্ষর* parse_মান( অক্ষর* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণসংখ্যা min_indent, যৌক্তিক is_parent_flow, পূর্ণসংখ্যা depth = 0 )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        অক্ষর* endptr = 0;
        অক্ষর c = ptr[0], d = ptr[1];
        পূর্ণসংখ্যা value_type = File_গ্রন্থি::NONE;
        পূর্ণসংখ্যা len;
        যৌক্তিক is_দ্বিমিক_string = মিথ্যা;
        যৌক্তিক is_user = মিথ্যা;

        যদি( c == '!' ) // handle explicit type specification
        {
            যদি( d == '!' || d == '^' )
            {
                ptr++;
                is_user = সত্য;
                //value_type |= FileNode::USER;
            }
            যদি ( d == '<') //support of full type heading from YAML 1.2
            {
                ধ্রুবক অক্ষর* yamlTypeHeading = "<tag:yaml.org,2002:";
                ধ্রুবক size_t heading_দৈর্ঘ্য = strlen(yamlTypeHeading);

                অক্ষর* typeEndPtr = ++ptr;

                করো d = *++typeEndPtr;
                যখন( cv_isprint(d) && d != ' ' && d != '>' );

                যদি ( d == '>' && (size_t)(typeEndPtr - ptr) > heading_দৈর্ঘ্য )
                {
                    যদি ( memcmp(ptr, yamlTypeHeading, heading_দৈর্ঘ্য) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += heading_দৈর্ঘ্য - 1;
                        is_user = সত্য;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            করো d = *++endptr;
            যখন( cv_isprint(d) && d != ' ' );
            len = (পূর্ণসংখ্যা)(endptr - ptr);
            যদি( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            যদি( len == 3 && !is_user )
            {
                যদি( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_গ্রন্থি::STRING;
                অন্যথা যদি( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_গ্রন্থি::INT;
                অন্যথা যদি( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_গ্রন্থি::SEQ;
                অন্যথা যদি( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_গ্রন্থি::MAP;
            }
            অন্যথা যদি( len == 5 && !is_user )
            {
                যদি( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_গ্রন্থি::REAL;
            }
            অন্যথা যদি (len == 6 && is_user)
            {
                যদি( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_গ্রন্থি::SEQ;
                    is_দ্বিমিক_string = সত্য;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    করো {
                        d = *++endptr;
                        যদি (d == '|')
                            বিরতি;
                    } যখন (d == ' ');

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
                যদি (value_type == File_গ্রন্থি::STRING && c != '\'' && c != '\"')
                    যাও force_string;
                যদি( value_type == File_গ্রন্থি::INT )
                    যাও force_int;
                যদি( value_type == File_গ্রন্থি::REAL )
                    যাও force_real;
            }
        }

        যদি (is_দ্বিমিক_string)
        {
            পূর্ণসংখ্যা indent = static_cast<পূর্ণসংখ্যা>(ptr - fs->বাফার_Start());
            ptr = fs->parseBase64(ptr, indent, গ্রন্থি);
        }
        অন্যথা যদি( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            যখন( cv_isdigit(*endptr) )
                endptr++;
            যদি( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                দ্বিগুণ fval = fs->strtod( ptr, &endptr );
                গ্রন্থি.set_মান(File_গ্রন্থি::REAL, &fval);
            }
            অন্যথা
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
            }

            যদি( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
        }
        অন্যথা যদি( c == '\'' || c == '\"' ) // an explicit string
        {
            যদি( c == '\'' )
                জন্য( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    যদি( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    অন্যথা যদি( c == '\'' )
                    {
                        c = *++ptr;
                        যদি( c != '\'' )
                            বিরতি;
                        buf[len++] = c;
                    }
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            অন্যথা
                জন্য( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    যদি( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    অন্যথা যদি( c == '\"' )
                    {
                        ++ptr;
                        বিরতি;
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
                            পূর্ণসংখ্যা val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (পূর্ণসংখ্যা)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            যদি( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            অন্যথা
                            {
                                buf[len++] = (অক্ষর)val;
                                ptr = endptr;
                            }
                        }
                    }
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            যদি( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            গ্রন্থি.set_মান(File_গ্রন্থি::STRING, buf, len);
        }
        অন্যথা যদি( c == '[' || c == '{' ) // collection as a flow
        {
            পূর্ণসংখ্যা new_min_indent = min_indent + !is_parent_flow;
            পূর্ণসংখ্যা struct_type = c == '{' ? File_গ্রন্থি::MAP : File_গ্রন্থি::SEQ;
            পূর্ণসংখ্যা nelems = 0;

            fs->convertToCollection(struct_type, গ্রন্থি);
            d = c == '[' ? ']' : '}';

            জন্য( ++ptr ;; nelems++ )
            {
                File_গ্রন্থি elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                যদি (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                যদি( *ptr == '}' || *ptr == ']' )
                {
                    যদি( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    বিরতি;
                }

                যদি( nelems != 0 )
                {
                    যদি( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    যদি (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                যদি( struct_type == File_গ্রন্থি::MAP )
                {
                    ptr = parse_চাবি( ptr, গ্রন্থি, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                অন্যথা
                {
                    যদি( *ptr == ']' )
                        বিরতি;
                    elem = fs->add_গ্রন্থি(গ্রন্থি, std::string(), File_গ্রন্থি::NONE);
                }
                ptr = parse_মান( ptr, elem, new_min_indent, সত্য, depth + 1 );
            }
            fs->finalizeCollection(গ্রন্থি);
        }
        অন্যথা
        {
            পূর্ণসংখ্যা indent, struct_type;

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

                করো c = *++endptr;
                যখন( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_গ্রন্থি::STRING));

                যদি( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                যদি( is_parent_flow || c != ':' )
                {
                    অক্ষর* str_end = endptr;
                    করো c = *--str_end;
                    যখন( str_end > ptr && c == ' ' );
                    str_end++;
                    যদি ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        যদি (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
                            ptr = endptr;
                            ফেরত ptr;
                        }
                        অন্যথা যদি (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
                            ptr = endptr;
                            ফেরত ptr;
                        }
                        অন্যথা যদি (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ফেরত ptr;
                        }
                    }

                    গ্রন্থি.set_মান(File_গ্রন্থি::STRING, ptr, (পূর্ণসংখ্যা)(str_end - ptr));
                    ptr = endptr;
                    ফেরত ptr;
                }
                struct_type = File_গ্রন্থি::MAP;
            }
            অন্যথা
                struct_type = File_গ্রন্থি::SEQ;

            fs->convertToCollection( struct_type, গ্রন্থি );
            indent = (পূর্ণসংখ্যা)(ptr - fs->বাফার_Start());

            জন্য(;;)
            {
                File_গ্রন্থি elem;

                যদি( struct_type == File_গ্রন্থি::MAP )
                {
                    ptr = parse_চাবি( ptr, গ্রন্থি, elem );
                }
                অন্যথা
                {
                    c = *ptr++;
                    যদি( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_গ্রন্থি(গ্রন্থি, std::string(), File_গ্রন্থি::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_মান( ptr, elem, indent + 1, মিথ্যা, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( ptr - fs->বাফার_Start() != indent )
                {
                    যদি( ptr - fs->বাফার_Start() < indent )
                        বিরতি;
                    অন্যথা
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                যদি( memcmp( ptr, "...", 3 ) == 0 )
                    বিরতি;
            }
            fs->finalizeCollection(গ্রন্থি);
        }

        ফেরত ptr;
    }

    যৌক্তিক parse( অক্ষর* ptr )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যৌক্তিক first = সত্য;
        যৌক্তিক ok = সত্য;
        File_গ্রন্থি মূল_collection(fs->getFS(), 0, 0);

        জন্য(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            জন্য(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    বিরতি;
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
                        বিরতি;
                    }
                    অন্যথা যদি( first )
                        বিরতি;
                }
                অন্যথা যদি( cv_isalnum(*ptr) || *ptr=='_')
                {
                    যদি( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    বিরতি;
                }
                অন্যথা যদি( fs->eof() )
                    বিরতি;
                অন্যথা
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            যদি( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            যদি( !ptr || !ptr[0] )
                বিরতি;
            যদি( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_গ্রন্থি মূল_গ্রন্থি = fs->add_গ্রন্থি(মূল_collection, std::string(), File_গ্রন্থি::NONE);

                ptr = parse_মান( ptr, মূল_গ্রন্থি, 0, মিথ্যা );
                যদি( !মূল_গ্রন্থি.isMap() && !মূল_গ্রন্থি.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                যদি( !ptr )
                    বিরতি;
            }

            যদি( fs->eof() )
                বিরতি;
            ptr += 3;
            first = মিথ্যা;
        }

        ফেরত ok;
    }

    FileStorage_API* fs;
    অক্ষর buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ফেরত makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ফেরত makePtr<YAMLParser>(fs);
}

}
