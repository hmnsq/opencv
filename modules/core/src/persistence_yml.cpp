// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence.hpp"

గణన
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

నామాకాశ cv
{

తరగతి YAMLEmitter : బహిరంగ FileStorageEmitter
{
బహిరంగ:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    మాయా ~YAMLEmitter() {}

    FStruct_దత్తాంశం ప్రారంభం_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& మాతృ, స్థిరరాశి అక్షరం* కీలకం,
                                 పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం* type_name=0)
    {
        అక్షరం buf[CV_FS_MAX_LEN + 1024];
        స్థిరరాశి అక్షరం* దత్తాంశం = 0;

        ఒకవేళ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_కణుపు::TYPE_ముసుగు|File_కణుపు::FLOW)) | File_కణుపు::ఖాళీలు_2;
        ఒకవేళ( !File_కణుపు::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ఒకవేళ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_కణుపు::SEQ;
            snprintf(buf, పరిమాణం(buf), "!!binary |");
            దత్తాంశం = buf;
        }
        లేకపోతే ఒకవేళ( File_కణుపు::isFlow(struct_flags))
        {
            అక్షరం c = File_కణుపు::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_కణుపు::FLOW;

            ఒకవేళ( type_name )
                snprintf( buf, పరిమాణం(buf), "!!%s %c", type_name, c );
            లేకపోతే
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            దత్తాంశం = buf;
        }
        లేకపోతే ఒకవేళ( type_name )
        {
            snprintf( buf, పరిమాణం(buf), "!!%s", type_name );
            దత్తాంశం = buf;
        }

        writeScalar( కీలకం, దత్తాంశం );

        FStruct_దత్తాంశం fsd;
        fsd.indent = మాతృ.indent;
        fsd.flags = struct_flags;

        ఒకవేళ( !File_కణుపు::isFlow(మాతృ.flags) )
            fsd.indent += CV_YML_INDENT + File_కణుపు::isFlow(struct_flags);

        తిరిగి fsd;
    }

    శూన్య చివర_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& current_struct)
    {
        అక్షరం* ptr;

        పూర్ణాంకం struct_flags = current_struct.flags;

        ఒకవేళ( File_కణుపు::isFlow(struct_flags) )
        {
            ptr = fs->బఫరు_Ptr();
            ఒకవేళ( ptr > fs->బఫరు_ప్రారంభం() + current_struct.indent && !File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_కణుపు::isMap(struct_flags) ? '}' : ']';
            fs->సమితి_బఫరు_Ptr(ptr);
        }
        లేకపోతే ఒకవేళ( File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
        {
            ptr = fs->flush();
            జ్ఞాపకం_నకలు( ptr, File_కణుపు::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->సమితి_బఫరు_Ptr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం value)
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::itoa( value, buf, 10 ));
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, int64_t value)
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::itoa( value, buf, 10, నిజం ));
    }

    శూన్య లిఖ( స్థిరరాశి అక్షరం* కీలకం, ద్విగుణ value )
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::double_గమ్యం_String( buf, పరిమాణం(buf), value, అబద్ధం ));
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* str, తార్కిక quote)
    {
        అక్షరం buf[CV_FS_MAX_LEN*4+16];
        అక్షరం* దత్తాంశం = (అక్షరం*)str;
        పూర్ణాంకం i, len;

        ఒకవేళ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(str);
        ఒకవేళ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ఒకవేళ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            పూర్ణాంకం need_quote = quote || len == 0 || str[0] == ' ';
            దత్తాంశం = buf;
            *దత్తాంశం++ = '\"';
            కొరకు( i = 0; i < len; i++ )
            {
                అక్షరం c = str[i];

                ఒకవేళ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ఒకవేళ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *దత్తాంశం++ = '\\';
                    ఒకవేళ( cv_isprint(c) )
                        *దత్తాంశం++ = c;
                    లేకపోతే ఒకవేళ( c == '\n' )
                        *దత్తాంశం++ = 'n';
                    లేకపోతే ఒకవేళ( c == '\r' )
                        *దత్తాంశం++ = 'r';
                    లేకపోతే ఒకవేళ( c == '\t' )
                        *దత్తాంశం++ = 't';
                    లేకపోతే
                    {
                        snprintf( దత్తాంశం, పరిమాణం(buf) - (దత్తాంశం - buf), "x%02x", c );
                        దత్తాంశం += 3;
                    }
                }
                లేకపోతే
                    *దత్తాంశం++ = c;
            }
            ఒకవేళ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ఒకవేళ( need_quote )
                *దత్తాంశం++ = '\"';
            *దత్తాంశం++ = '\0';
            దత్తాంశం = buf + !need_quote;
        }

        writeScalar( కీలకం, దత్తాంశం);
    }

    శూన్య writeScalar(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* దత్తాంశం)
    {
        fs->check_if_write_struct_is_delayed(అబద్ధం);
        ఒకవేళ ( fs->get_state_యొక్క_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_గమ్యం_Base64_state( FileStorage_API::NotUse );
        }
        లేకపోతే ఒకవేళ ( fs->get_state_యొక్క_writing_base64() == FileStorage_API::లోపల_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        పూర్ణాంకం i, keylen = 0;
        పూర్ణాంకం datalen = 0;
        అక్షరం* ptr;

        FStruct_దత్తాంశం& current_struct = fs->getCurrentStruct();

        పూర్ణాంకం struct_flags = current_struct.flags;

        ఒకవేళ( కీలకం && కీలకం[0] == '\0' )
            కీలకం = 0;

        ఒకవేళ( File_కణుపు::isCollection(struct_flags) )
        {
            ఒకవేళ( (File_కణుపు::isMap(struct_flags) ^ (కీలకం != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        లేకపోతే
        {
            fs->సమితి_Non_ఖాళీలు();
            struct_flags = File_కణుపు::ఖాళీలు_2 | (కీలకం ? File_కణుపు::MAP : File_కణుపు::SEQ);
        }

        ఒకవేళ( కీలకం )
        {
            keylen = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(కీలకం);
            ఒకవేళ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ఒకవేళ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ఒకవేళ( దత్తాంశం )
            datalen = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(దత్తాంశం);

        ఒకవేళ( File_కణుపు::isFlow(struct_flags) )
        {
            ptr = fs->బఫరు_Ptr();
            ఒకవేళ( !File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
                *ptr++ = ',';
            పూర్ణాంకం new_స్థానభ్రంశం = (పూర్ణాంకం)(ptr - fs->బఫరు_ప్రారంభం()) + keylen + datalen;
            ఒకవేళ( new_స్థానభ్రంశం > fs->wrapMargin() && new_స్థానభ్రంశం - current_struct.indent > 10 )
            {
                fs->సమితి_బఫరు_Ptr(ptr);
                ptr = fs->flush();
            }
            లేకపోతే
                *ptr++ = ' ';
        }
        లేకపోతే
        {
            ptr = fs->flush();
            ఒకవేళ( !File_కణుపు::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ఒకవేళ( దత్తాంశం )
                    *ptr++ = ' ';
            }
        }

        ఒకవేళ( కీలకం )
        {
            ఒకవేళ( !cv_isalpha(కీలకం[0]) && కీలకం[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_బఫరు( ptr, keylen );

            కొరకు( i = 0; i < keylen; i++ )
            {
                అక్షరం c = కీలకం[i];

                ptr[i] = c;
                ఒకవేళ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ఒకవేళ( !File_కణుపు::isFlow(struct_flags) && దత్తాంశం )
                *ptr++ = ' ';
        }

        ఒకవేళ( దత్తాంశం )
        {
            ptr = fs->resize_Write_బఫరు( ptr, datalen );
            జ్ఞాపకం_నకలు( ptr, దత్తాంశం, datalen );
            ptr += datalen;
        }

        fs->సమితి_బఫరు_Ptr(ptr);
        current_struct.flags &= ~File_కణుపు::ఖాళీలు_2;
    }

    శూన్య writeComment(స్థిరరాశి అక్షరం* comment, తార్కిక eol_comment)
    {
        ఒకవేళ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        పూర్ణాంకం len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(comment);
        స్థిరరాశి అక్షరం* eol = strchr(comment, '\n');
        తార్కిక multiline = eol != 0;
        అక్షరం* ptr = fs->బఫరు_Ptr();

        ఒకవేళ( !eol_comment || multiline ||
            fs->బఫరు_చివర_2() - ptr < len || ptr == fs->బఫరు_ప్రారంభం() )
            ptr = fs->flush();
        లేకపోతే
            *ptr++ = ' ';

        అయితే( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ఒకవేళ( eol )
            {
                ptr = fs->resize_Write_బఫరు( ptr, (పూర్ణాంకం)(eol - comment) + 1 );
                జ్ఞాపకం_నకలు( ptr, comment, eol - comment + 1 );
                fs->సమితి_బఫరు_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            లేకపోతే
            {
                len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(comment);
                ptr = fs->resize_Write_బఫరు( ptr, len );
                జ్ఞాపకం_నకలు( ptr, comment, len );
                fs->సమితి_బఫరు_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    శూన్య ప్రారంభం_తదుపరి_ప్రవాహం()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

రక్షిత:
    FileStorage_API* fs;
};


తరగతి YAMLParser : బహిరంగ FileStorageParser
{
బహిరంగ:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    మాయా ~YAMLParser() {}

    అక్షరం* skipSpaces( అక్షరం* ptr, పూర్ణాంకం min_indent, పూర్ణాంకం max_comment_indent )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        కొరకు(;;)
        {
            అయితే( *ptr == ' ' )
                ptr++;
            ఒకవేళ( *ptr == '#' )
            {
                ఒకవేళ( ptr - fs->బఫరు_ప్రారంభం() > max_comment_indent )
                    తిరిగి ptr;
                *ptr = '\0';
            }
            లేకపోతే ఒకవేళ( cv_isprint(*ptr) )
            {
                ఒకవేళ( ptr - fs->బఫరు_ప్రారంభం() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ఆపు;
            }
            లేకపోతే ఒకవేళ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ఒకవేళ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->బఫరు_ప్రారంభం();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->సమితి_Eof();
                    ఆపు;
                }
                లేకపోతే
                {
                    పూర్ణాంకం l = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(ptr);
                    ఒకవేళ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            లేకపోతే
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        తిరిగి ptr;
    }

    తార్కిక get_Base64_అడ్డువరుస(అక్షరం* ptr, పూర్ణాంకం indent, అక్షరం* &beg, అక్షరం* &end)
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, గరిష్ఠ_పూర్ణాంకం);
        ఒకవేళ (!ptr || !*ptr)
            తిరిగి అబద్ధం; // end of file

        ఒకవేళ (ptr - fs->బఫరు_ప్రారంభం() != indent)
            తిరిగి అబద్ధం; // end of base64 data

        /* find end */
        అయితే(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ఒకవేళ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        తిరిగి నిజం;
    }


    అక్షరం* parse_కీలకం( అక్షరం* ptr, File_కణుపు& map_కణుపు, File_కణుపు& విలువ_placeholder )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        అక్షరం c;
        అక్షరం *endptr = ptr - 1, *saveptr;

        ఒకవేళ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        చేయి c = *++endptr;
        అయితే( cv_isprint(c) && c != ':' );

        ఒకవేళ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ఒకవేళ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        చేయి c = *--endptr;
        అయితే( c == ' ' );

        ++endptr;

        విలువ_placeholder = fs->కూడు_కణుపు(map_కణుపు, std::string(ptr, endptr - ptr), File_కణుపు::NONE);
        ptr = saveptr;

        తిరిగి ptr;
    }

    అక్షరం* parse_విలువ( అక్షరం* ptr, File_కణుపు& కణుపు, పూర్ణాంకం min_indent, తార్కిక is_మాతృ_flow, పూర్ణాంకం లోతు = 0 )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ఒకవేళ( లోతు > CV_PERSISTENCE_MAX_లోతు )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        అక్షరం* endptr = 0;
        అక్షరం c = ptr[0], d = ptr[1];
        పూర్ణాంకం value_type = File_కణుపు::NONE;
        పూర్ణాంకం len;
        తార్కిక is_ద్విమాన_string = అబద్ధం;
        తార్కిక is_user = అబద్ధం;

        ఒకవేళ( c == '!' ) // handle explicit type specification
        {
            ఒకవేళ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = నిజం;
                //value_type |= FileNode::USER;
            }
            ఒకవేళ ( d == '<') //support of full type heading from YAML 1.2
            {
                స్థిరరాశి అక్షరం* yamlTypeHeading = "<tag:yaml.org,2002:";
                స్థిరరాశి size_t heading_నిడివి = అక్షరశ్రేణి_పొడవు(yamlTypeHeading);

                అక్షరం* type_చివర_Ptr = ++ptr;

                చేయి d = *++type_చివర_Ptr;
                అయితే( cv_isprint(d) && d != ' ' && d != '>' );

                ఒకవేళ ( d == '>' && (size_t)(type_చివర_Ptr - ptr) > heading_నిడివి )
                {
                    ఒకవేళ ( memcmp(ptr, yamlTypeHeading, heading_నిడివి) == 0 )
                    {
                        *type_చివర_Ptr = ' ';
                        ptr += heading_నిడివి - 1;
                        is_user = నిజం;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            చేయి d = *++endptr;
            అయితే( cv_isprint(d) && d != ' ' );
            len = (పూర్ణాంకం)(endptr - ptr);
            ఒకవేళ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ఒకవేళ( len == 3 && !is_user )
            {
                ఒకవేళ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_కణుపు::STRING;
                లేకపోతే ఒకవేళ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_కణుపు::INT;
                లేకపోతే ఒకవేళ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_కణుపు::SEQ;
                లేకపోతే ఒకవేళ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_కణుపు::MAP;
            }
            లేకపోతే ఒకవేళ( len == 5 && !is_user )
            {
                ఒకవేళ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_కణుపు::REAL;
            }
            లేకపోతే ఒకవేళ (len == 6 && is_user)
            {
                ఒకవేళ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_కణుపు::SEQ;
                    is_ద్విమాన_string = నిజం;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    చేయి {
                        d = *++endptr;
                        ఒకవేళ (d == '|')
                            ఆపు;
                    } అయితే (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, గరిష్ఠ_పూర్ణాంకం );
            ఒకవేళ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ఒకవేళ( !is_user )
            {
                ఒకవేళ (value_type == File_కణుపు::STRING && c != '\'' && c != '\"')
                    వెళ్ళు force_string;
                ఒకవేళ( value_type == File_కణుపు::INT )
                    వెళ్ళు force_int;
                ఒకవేళ( value_type == File_కణుపు::REAL )
                    వెళ్ళు force_real;
            }
        }

        ఒకవేళ (is_ద్విమాన_string)
        {
            పూర్ణాంకం indent = static_cast<పూర్ణాంకం>(ptr - fs->బఫరు_ప్రారంభం());
            ptr = fs->parseBase64(ptr, indent, కణుపు);
        }
        లేకపోతే ఒకవేళ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            అయితే( cv_isdigit(*endptr) )
                endptr++;
            ఒకవేళ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ద్విగుణ fval = fs->strtod( ptr, &endptr );
                కణుపు.సమితి_విలువ(File_కణుపు::REAL, &fval);
            }
            లేకపోతే
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                కణుపు.సమితి_విలువ(File_కణుపు::INT, &ival);
            }

            ఒకవేళ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
        }
        లేకపోతే ఒకవేళ( c == '\'' || c == '\"' ) // an explicit string
        {
            ఒకవేళ( c == '\'' )
                కొరకు( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ఒకవేళ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    లేకపోతే ఒకవేళ( c == '\'' )
                    {
                        c = *++ptr;
                        ఒకవేళ( c != '\'' )
                            ఆపు;
                        buf[len++] = c;
                    }
                    లేకపోతే
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            లేకపోతే
                కొరకు( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ఒకవేళ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    లేకపోతే ఒకవేళ( c == '\"' )
                    {
                        ++ptr;
                        ఆపు;
                    }
                    లేకపోతే ఒకవేళ( c == '\\' )
                    {
                        d = *++ptr;
                        ఒకవేళ( d == '\'' )
                            buf[len++] = d;
                        లేకపోతే ఒకవేళ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        లేకపోతే ఒకవేళ( d == 'n' )
                            buf[len++] = '\n';
                        లేకపోతే ఒకవేళ( d == 'r' )
                            buf[len++] = '\r';
                        లేకపోతే ఒకవేళ( d == 't' )
                            buf[len++] = '\t';
                        లేకపోతే ఒకవేళ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            పూర్ణాంకం val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (పూర్ణాంకం)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ఒకవేళ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            లేకపోతే
                            {
                                buf[len++] = (అక్షరం)val;
                                ptr = endptr;
                            }
                        }
                    }
                    లేకపోతే
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ఒకవేళ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            కణుపు.సమితి_విలువ(File_కణుపు::STRING, buf, len);
        }
        లేకపోతే ఒకవేళ( c == '[' || c == '{' ) // collection as a flow
        {
            పూర్ణాంకం new_min_indent = min_indent + !is_మాతృ_flow;
            పూర్ణాంకం struct_type = c == '{' ? File_కణుపు::MAP : File_కణుపు::SEQ;
            పూర్ణాంకం nelems = 0;

            fs->convert_గమ్యం_Collection(struct_type, కణుపు);
            d = c == '[' ? ']' : '}';

            కొరకు( ++ptr ;; nelems++ )
            {
                File_కణుపు elem;

                ptr = skipSpaces( ptr, new_min_indent, గరిష్ఠ_పూర్ణాంకం );
                ఒకవేళ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ఒకవేళ( *ptr == '}' || *ptr == ']' )
                {
                    ఒకవేళ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ఆపు;
                }

                ఒకవేళ( nelems != 0 )
                {
                    ఒకవేళ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, గరిష్ఠ_పూర్ణాంకం );
                    ఒకవేళ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ఒకవేళ( struct_type == File_కణుపు::MAP )
                {
                    ptr = parse_కీలకం( ptr, కణుపు, elem );
                    ptr = skipSpaces( ptr, new_min_indent, గరిష్ఠ_పూర్ణాంకం );
                }
                లేకపోతే
                {
                    ఒకవేళ( *ptr == ']' )
                        ఆపు;
                    elem = fs->కూడు_కణుపు(కణుపు, std::string(), File_కణుపు::NONE);
                }
                ptr = parse_విలువ( ptr, elem, new_min_indent, నిజం, లోతు + 1 );
            }
            fs->finalizeCollection(కణుపు);
        }
        లేకపోతే
        {
            పూర్ణాంకం indent, struct_type;

            ఒకవేళ( is_మాతృ_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ఒకవేళ( !is_మాతృ_flow )
                {
                    ఒకవేళ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ఒకవేళ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                చేయి c = *++endptr;
                అయితే( cv_isprint(c) &&
                      (!is_మాతృ_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_మాతృ_flow || c != ':' || value_type == File_కణుపు::STRING));

                ఒకవేళ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ఒకవేళ( is_మాతృ_flow || c != ':' )
                {
                    అక్షరం* str_చివర = endptr;
                    చేయి c = *--str_చివర;
                    అయితే( str_చివర > ptr && c == ' ' );
                    str_చివర++;
                    ఒకవేళ ((fs->getFormat() & FileStorage::FORMAT_ముసుగు) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_చివర - ptr;
                        ఒకవేళ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            కణుపు.సమితి_విలువ(File_కణుపు::INT, &ival);
                            ptr = endptr;
                            తిరిగి ptr;
                        }
                        లేకపోతే ఒకవేళ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            కణుపు.సమితి_విలువ(File_కణుపు::INT, &ival);
                            ptr = endptr;
                            తిరిగి ptr;
                        }
                        లేకపోతే ఒకవేళ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            తిరిగి ptr;
                        }
                    }

                    కణుపు.సమితి_విలువ(File_కణుపు::STRING, ptr, (పూర్ణాంకం)(str_చివర - ptr));
                    ptr = endptr;
                    తిరిగి ptr;
                }
                struct_type = File_కణుపు::MAP;
            }
            లేకపోతే
                struct_type = File_కణుపు::SEQ;

            fs->convert_గమ్యం_Collection( struct_type, కణుపు );
            indent = (పూర్ణాంకం)(ptr - fs->బఫరు_ప్రారంభం());

            కొరకు(;;)
            {
                File_కణుపు elem;

                ఒకవేళ( struct_type == File_కణుపు::MAP )
                {
                    ptr = parse_కీలకం( ptr, కణుపు, elem );
                }
                లేకపోతే
                {
                    c = *ptr++;
                    ఒకవేళ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->కూడు_కణుపు(కణుపు, std::string(), File_కణుపు::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, గరిష్ఠ_పూర్ణాంకం );
                ptr = parse_విలువ( ptr, elem, indent + 1, అబద్ధం, లోతు + 1 );
                ptr = skipSpaces( ptr, 0, గరిష్ఠ_పూర్ణాంకం );
                ఒకవేళ( ptr - fs->బఫరు_ప్రారంభం() != indent )
                {
                    ఒకవేళ( ptr - fs->బఫరు_ప్రారంభం() < indent )
                        ఆపు;
                    లేకపోతే
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ఒకవేళ( memcmp( ptr, "...", 3 ) == 0 )
                    ఆపు;
            }
            fs->finalizeCollection(కణుపు);
        }

        తిరిగి ptr;
    }

    తార్కిక parse( అక్షరం* ptr )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        తార్కిక first = నిజం;
        తార్కిక ok = నిజం;
        File_కణుపు మూలం_collection(fs->getFS(), 0, 0);

        కొరకు(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            కొరకు(;;)
            {
                ptr = skipSpaces( ptr, 0, గరిష్ఠ_పూర్ణాంకం );
                ఒకవేళ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ఆపు;
                }

                ఒకవేళ( *ptr == '%' )
                {
                    ఒకవేళ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ఒకవేళ ((fs->getFormat() & FileStorage::FORMAT_ముసుగు) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                లేకపోతే ఒకవేళ( *ptr == '-' )
                {
                    ఒకవేళ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ఆపు;
                    }
                    లేకపోతే ఒకవేళ( first )
                        ఆపు;
                }
                లేకపోతే ఒకవేళ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ఒకవేళ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ఆపు;
                }
                లేకపోతే ఒకవేళ( fs->eof() )
                    ఆపు;
                లేకపోతే
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ఒకవేళ( ptr )
                ptr = skipSpaces( ptr, 0, గరిష్ఠ_పూర్ణాంకం );
            ఒకవేళ( !ptr || !ptr[0] )
                ఆపు;
            ఒకవేళ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_కణుపు మూలం_కణుపు = fs->కూడు_కణుపు(మూలం_collection, std::string(), File_కణుపు::NONE);

                ptr = parse_విలువ( ptr, మూలం_కణుపు, 0, అబద్ధం );
                ఒకవేళ( !మూలం_కణుపు.isMap() && !మూలం_కణుపు.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, గరిష్ఠ_పూర్ణాంకం );
                ఒకవేళ( !ptr )
                    ఆపు;
            }

            ఒకవేళ( fs->eof() )
                ఆపు;
            ptr += 3;
            first = అబద్ధం;
        }

        తిరిగి ok;
    }

    FileStorage_API* fs;
    అక్షరం buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    తిరిగి makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    తిరిగి makePtr<YAMLParser>(fs);
}

}
