// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence.hpp"

గణన
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_ట్యాగ్ = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_ట్యాగ్ = 1,
    CV_XML_CLOSING_ట్యాగ్ = 2,
    CV_XML_ఖాళీలు_ట్యాగ్ = 3,
    CV_XML_శీర్షిక_ట్యాగ్ = 4,
    CV_XML_DIRECTIVE_ట్యాగ్ = 5
};

నామాకాశ cv
{

తరగతి XMLEmitter : బహిరంగ FileStorageEmitter
{
బహిరంగ:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    మాయా ~XMLEmitter() {}

    శూన్య write_ట్యాగ్( స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం ట్యాగ్_type, స్థిరరాశి std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        అక్షరం* ptr = fs->బఫరు_Ptr();
        పూర్ణాంకం i, len = 0;
        FStruct_దత్తాంశం& current_struct = fs->getCurrentStruct();
        పూర్ణాంకం struct_flags = current_struct.flags;

        ఒకవేళ( కీలకం && కీలకం[0] == '\0' )
            కీలకం = 0;

        ఒకవేళ( ట్యాగ్_type == CV_XML_OPENING_ట్యాగ్ || ట్యాగ్_type == CV_XML_ఖాళీలు_ట్యాగ్ )
        {
            ఒకవేళ( File_కణుపు::isCollection(struct_flags) )
            {
                ఒకవేళ( File_కణుపు::isMap(struct_flags) ^ (కీలకం != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            లేకపోతే
            {
                struct_flags = File_కణుపు::ఖాళీలు_2 + (కీలకం ? File_కణుపు::MAP : File_కణుపు::SEQ);
                //fs->is_first = 0;
            }

            ఒకవేళ( !File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
                ptr = fs->flush();
        }

        ఒకవేళ( !కీలకం )
            కీలకం = "_";
        లేకపోతే ఒకవేళ( కీలకం[0] == '_' && కీలకం[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు( కీలకం );
        *ptr++ = '<';
        ఒకవేళ( ట్యాగ్_type == CV_XML_CLOSING_ట్యాగ్ )
        {
            ఒకవేళ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ఒకవేళ( !cv_isalpha(కీలకం[0]) && కీలకం[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_బఫరు( ptr, len );
        కొరకు( i = 0; i < len; i++ )
        {
            అక్షరం c = కీలకం[i];
            ఒకవేళ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        పూర్ణాంకం nattr = (పూర్ణాంకం)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        కొరకు( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_బఫరు( ptr, (పూర్ణాంకం)(len0 + len1 + 4) );
            *ptr++ = ' ';

            జ్ఞాపకం_నకలు( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ఒకవేళ( len1 > 0 )
                జ్ఞాపకం_నకలు( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ఒకవేళ( ట్యాగ్_type == CV_XML_ఖాళీలు_ట్యాగ్ )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->సమితి_బఫరు_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_కణుపు::ఖాళీలు_2;
    }

    FStruct_దత్తాంశం ప్రారంభం_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& మాతృ, స్థిరరాశి అక్షరం* కీలకం,
                                 పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం* type_name=0)
    {
        std::vector<std::string> attrlist;
        ఒకవేళ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_ట్యాగ్( కీలకం, CV_XML_OPENING_ట్యాగ్, attrlist );

        FStruct_దత్తాంశం current_struct;
        current_struct.ట్యాగ్ = కీలకం ? std::string(కీలకం) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = మాతృ.indent + CV_XML_INDENT;

        తిరిగి current_struct;
    }

    శూన్య చివర_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& current_struct)
    {
        write_ట్యాగ్( current_struct.ట్యాగ్.c_str(), CV_XML_CLOSING_ట్యాగ్ );
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం value)
    {
        అక్షరం buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( కీలకం, ptr);
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, int64_t value)
    {
        అక్షరం buf[128], *ptr = fs::itoa( value, buf, 10, నిజం );
        writeScalar( కీలకం, ptr);
    }

    శూన్య లిఖ( స్థిరరాశి అక్షరం* కీలకం, ద్విగుణ value )
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::double_గమ్యం_String( buf, పరిమాణం(buf), value, అబద్ధం ) );
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* str, తార్కిక quote)
    {
        అక్షరం buf[CV_FS_MAX_LEN*6+16];
        అక్షరం* దత్తాంశం = (అక్షరం*)str;
        పూర్ణాంకం i, len;

        ఒకవేళ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(str);
        ఒకవేళ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ఒకవేళ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            తార్కిక need_quote = quote || len == 0;
            దత్తాంశం = buf;
            *దత్తాంశం++ = '\"';
            కొరకు( i = 0; i < len; i++ )
            {
                అక్షరం c = str[i];

                ఒకవేళ( (uchar)c >= 128 || c == ' ' )
                {
                    *దత్తాంశం++ = c;
                    need_quote = నిజం;
                }
                లేకపోతే ఒకవేళ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *దత్తాంశం++ = '&';
                    ఒకవేళ( c == '<' )
                    {
                        జ్ఞాపకం_నకలు(దత్తాంశం, "lt", 2);
                        దత్తాంశం += 2;
                    }
                    లేకపోతే ఒకవేళ( c == '>' )
                    {
                        జ్ఞాపకం_నకలు(దత్తాంశం, "gt", 2);
                        దత్తాంశం += 2;
                    }
                    లేకపోతే ఒకవేళ( c == '&' )
                    {
                        జ్ఞాపకం_నకలు(దత్తాంశం, "amp", 3);
                        దత్తాంశం += 3;
                    }
                    లేకపోతే ఒకవేళ( c == '\'' )
                    {
                        జ్ఞాపకం_నకలు(దత్తాంశం, "apos", 4);
                        దత్తాంశం += 4;
                    }
                    లేకపోతే ఒకవేళ( c == '\"' )
                    {
                        జ్ఞాపకం_నకలు( దత్తాంశం, "quot", 4);
                        దత్తాంశం += 4;
                    }
                    లేకపోతే
                    {
                        snprintf( దత్తాంశం, పరిమాణం(buf) - (దత్తాంశం - buf), "#x%02x", (uchar)c );
                        దత్తాంశం += 4;
                    }
                    *దత్తాంశం++ = ';';
                    need_quote = 1;
                }
                లేకపోతే
                    *దత్తాంశం++ = c;
            }
            ఒకవేళ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = నిజం;

            ఒకవేళ( need_quote )
                *దత్తాంశం++ = '\"';
            len = (పూర్ణాంకం)(దత్తాంశం - buf) - !need_quote;
            *దత్తాంశం++ = '\0';
            దత్తాంశం = buf + (పూర్ణాంకం)!need_quote;
        }

        writeScalar( కీలకం, దత్తాంశం );
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

        పూర్ణాంకం len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(దత్తాంశం);
        ఒకవేళ( కీలకం && *కీలకం == '\0' )
            కీలకం = 0;

        FStruct_దత్తాంశం& current_struct = fs->getCurrentStruct();
        పూర్ణాంకం struct_flags = current_struct.flags;

        ఒకవేళ( File_కణుపు::isMap(struct_flags) ||
           (!File_కణుపు::isCollection(struct_flags) && కీలకం) )
        {
            write_ట్యాగ్( కీలకం, CV_XML_OPENING_ట్యాగ్ );
            అక్షరం* ptr = fs->resize_Write_బఫరు( fs->బఫరు_Ptr(), len );
            జ్ఞాపకం_నకలు( ptr, దత్తాంశం, len );
            fs->సమితి_బఫరు_Ptr( ptr + len );
            write_ట్యాగ్( కీలకం, CV_XML_CLOSING_ట్యాగ్ );
        }
        లేకపోతే
        {
            అక్షరం* ptr = fs->బఫరు_Ptr();
            పూర్ణాంకం new_స్థానభ్రంశం = (పూర్ణాంకం)(ptr - fs->బఫరు_ప్రారంభం()) + len;

            ఒకవేళ( కీలకం )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_కణుపు::SEQ;

            ఒకవేళ( (new_స్థానభ్రంశం > fs->wrapMargin() && new_స్థానభ్రంశం - current_struct.indent > 10) ||
               (ptr > fs->బఫరు_ప్రారంభం() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            లేకపోతే ఒకవేళ( ptr > fs->బఫరు_ప్రారంభం() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            జ్ఞాపకం_నకలు( ptr, దత్తాంశం, len );
            fs->సమితి_బఫరు_Ptr(ptr + len);
        }
    }

    శూన్య writeComment(స్థిరరాశి అక్షరం* comment, తార్కిక eol_comment)
    {
        FStruct_దత్తాంశం& current_struct = fs->getCurrentStruct();
        పూర్ణాంకం len;
        పూర్ణాంకం multiline;
        స్థిరరాశి అక్షరం* eol;
        అక్షరం* ptr;

        ఒకవేళ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ఒకవేళ( అక్షరశ్రేణి_శోధన(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->బఫరు_Ptr();

        ఒకవేళ( multiline || !eol_comment || fs->బఫరు_చివర_2() - ptr < len + 5 )
            ptr = fs->flush();
        లేకపోతే ఒకవేళ( ptr > fs->బఫరు_ప్రారంభం() + current_struct.indent )
            *ptr++ = ' ';

        ఒకవేళ( !multiline )
        {
            ptr = fs->resize_Write_బఫరు( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(ptr);
        }
        లేకపోతే
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->సమితి_బఫరు_Ptr(ptr + len);
        ptr = fs->flush();

        ఒకవేళ( multiline )
        {
            అయితే( comment )
            {
                ఒకవేళ( eol )
                {
                    ptr = fs->resize_Write_బఫరు( ptr, (పూర్ణాంకం)(eol - comment) + 1 );
                    జ్ఞాపకం_నకలు( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                లేకపోతే
                {
                    len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(comment);
                    ptr = fs->resize_Write_బఫరు( ptr, len );
                    జ్ఞాపకం_నకలు( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->సమితి_బఫరు_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->సమితి_బఫరు_Ptr(ptr + 3);
            fs->flush();
        }
    }

    శూన్య ప్రారంభం_తదుపరి_ప్రవాహం()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

రక్షిత:
    FileStorage_API* fs;
};

తరగతి XMLParser : బహిరంగ FileStorageParser
{
బహిరంగ:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    మాయా ~XMLParser() {}

    అక్షరం* skipSpaces( అక్షరం* ptr, పూర్ణాంకం బహుళకం )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        పూర్ణాంకం level = 0;

        కొరకు(;;)
        {
            అక్షరం c;
            ptr--;

            ఒకవేళ( బహుళకం == CV_XML_INSIDE_COMMENT )
            {
                చేయి c = *++ptr;
                అయితే( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ఒకవేళ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    బహుళకం = 0;
                    ptr += 3;
                }
            }
            లేకపోతే ఒకవేళ( బహుళకం == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                చేయి
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ఒకవేళ( level < 0 )
                        తిరిగి ptr;
                } అయితే( cv_isprint_or_tab(c) );
            }
            లేకపోతే
            {
                చేయి c = *++ptr;
                అయితే( c == ' ' || c == '\t' );

                ఒకవేళ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ఒకవేళ( బహుళకం != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    బహుళకం = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                లేకపోతే ఒకవేళ( cv_isprint(c) )
                    ఆపు;
            }

            ఒకవేళ( !cv_isprint(*ptr) )
            {
                ఒకవేళ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ఒకవేళ( !ptr || *ptr == '\0' )
                    ఆపు;
            }
        }
        తిరిగి ptr;
    }

    తార్కిక get_Base64_అడ్డువరుస(అక్షరం* ptr, పూర్ణాంకం /*indent*/, అక్షరం* &beg, అక్షరం* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_ట్యాగ్);
        ఒకవేళ( !ptr || !*ptr )
            తిరిగి అబద్ధం;

        // closing XML tag
        ఒకవేళ ( *beg == '<' )
            తిరిగి అబద్ధం;

        // find end of the row
        అయితే( cv_isprint(*ptr) )
            ++ptr;
        ఒకవేళ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        తిరిగి నిజం;
    }

    అక్షరం* parse_విలువ( అక్షరం* ptr, File_కణుపు& కణుపు, పూర్ణాంకం లోతు = 0 )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ఒకవేళ( లోతు > CV_PERSISTENCE_MAX_లోతు )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_కణుపు new_elem;
        తార్కిక have_space = నిజం;
        పూర్ణాంకం value_type = కణుపు.type();
        std::string కీలకం, key2, type_name;

        కొరకు(;;)
        {
            అక్షరం c = *ptr, d;
            అక్షరం* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ఒకవేళ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ఒకవేళ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = నిజం;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ఒకవేళ( c =='<' || c == '\0' )
            {
                పూర్ణాంకం ట్యాగ్_type = 0;
                పూర్ణాంకం elem_type = File_కణుపు::NONE;

                ఒకవేళ( d == '/' || c == '\0' )
                    ఆపు;

                ptr = parse_ట్యాగ్( ptr, కీలకం, type_name, ట్యాగ్_type );

                ఒకవేళ( ట్యాగ్_type == CV_XML_DIRECTIVE_ట్యాగ్ )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ఒకవేళ( ట్యాగ్_type == CV_XML_ఖాళీలు_ట్యాగ్ )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(ట్యాగ్_type == CV_XML_OPENING_ట్యాగ్);

                /* for base64 string */
                తార్కిక ద్విమాన_string = అబద్ధం;

                ఒకవేళ( !type_name.empty() )
                {
                    స్థిరరాశి అక్షరం* tn = type_name.c_str();
                    ఒకవేళ( strcmp(tn, "str") == 0 )
                        elem_type = File_కణుపు::STRING;
                    లేకపోతే ఒకవేళ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_కణుపు::MAP;
                    లేకపోతే ఒకవేళ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_కణుపు::SEQ;
                    లేకపోతే ఒకవేళ( strcmp( tn, "binary") == 0)
                        ద్విమాన_string = నిజం;
                }

                new_elem = fs->కూడు_కణుపు(కణుపు, కీలకం, elem_type, 0);
                ఒకవేళ (!ద్విమాన_string)
                    ptr = parse_విలువ(ptr, new_elem, లోతు + 1);
                లేకపోతే
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ఒకవేళ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_ట్యాగ్( ptr, key2, type_name, ట్యాగ్_type );
                ఒకవేళ( ట్యాగ్_type != CV_XML_CLOSING_ట్యాగ్ || key2 != కీలకం )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = నిజం;
            }
            లేకపోతే
            {
                ఒకవేళ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_కణుపు* elem = &కణుపు;
                ఒకవేళ( కణుపు.type() != File_కణుపు::NONE )
                {
                    fs->convert_గమ్యం_Collection( File_కణుపు::SEQ, కణుపు );
                    new_elem = fs->కూడు_కణుపు(కణుపు, std::string(), File_కణుపు::NONE, 0);
                    elem = &new_elem;
                }

                ఒకవేళ( value_type != File_కణుపు::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    అయితే( cv_isdigit(*endptr) )
                        endptr++;
                    ఒకవేళ( *endptr == '.' || *endptr == 'e' )
                    {
                        ద్విగుణ fval = fs->strtod( ptr, &endptr );
                        elem->సమితి_విలువ(File_కణుపు::REAL, &fval);
                    }
                    లేకపోతే
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->సమితి_విలువ(File_కణుపు::INT, &ival);
                    }

                    ఒకవేళ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
                }
                లేకపోతే
                {
                    // string
                    పూర్ణాంకం i = 0, len, is_quoted = 0;
                    ఒకవేళ( c == '\"' )
                        is_quoted = 1;
                    లేకపోతే
                        --ptr;
                    strbuf[0] = '\0';

                    కొరకు( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();

                        ఒకవేళ( !cv_isalnum(c) )
                        {
                            ఒకవేళ( c == '\"' )
                            {
                                ఒకవేళ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                ఆపు;
                            }
                            లేకపోతే ఒకవేళ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ఒకవేళ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                ఆపు;
                            }
                            లేకపోతే ఒకవేళ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            లేకపోతే ఒకవేళ( c == '&' )
                            {
                                ఒకవేళ( *++ptr == '#' )
                                {
                                    పూర్ణాంకం val, పునాది = 10;
                                    ptr++;
                                    ఒకవేళ( *ptr == 'x' )
                                    {
                                        పునాది = 16;
                                        ptr++;
                                    }
                                    val = (పూర్ణాంకం)strtol( ptr, &endptr, పునాది );
                                    ఒకవేళ( (గుర్తులేని)val > (గుర్తులేని)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (అక్షరం)val;
                                }
                                లేకపోతే
                                {
                                    endptr = ptr;
                                    చేయి c = *++endptr;
                                    అయితే( cv_isalnum(c) );
                                    ఒకవేళ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (పూర్ణాంకం)(endptr - ptr);
                                    ఒకవేళ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    లేకపోతే ఒకవేళ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    లేకపోతే ఒకవేళ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    లేకపోతే ఒకవేళ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    లేకపోతే ఒకవేళ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    లేకపోతే
                                    {
                                        ఒకవేళ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        జ్ఞాపకం_నకలు( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
                            }
                        }
                        ఒకవేళ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->సమితి_విలువ(File_కణుపు::STRING, strbuf, i);
                }

                ఒకవేళ( value_type != File_కణుపు::NONE && value_type != File_కణుపు::SEQ && value_type != File_కణుపు::MAP )
                    ఆపు;
                have_space = అబద్ధం;
            }
        }
        fs->finalizeCollection(కణుపు);

        తిరిగి ptr;
    }

    అక్షరం* parse_ట్యాగ్( అక్షరం* ptr, std::string& ట్యాగ్_name,
                    std::string& type_name, పూర్ణాంకం& ట్యాగ్_type )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ఒకవేళ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ఒకవేళ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();

        ఒకవేళ( cv_isalnum(*ptr) || *ptr == '_' )
            ట్యాగ్_type = CV_XML_OPENING_ట్యాగ్;
        లేకపోతే ఒకవేళ( *ptr == '/' )
        {
            ట్యాగ్_type = CV_XML_CLOSING_ట్యాగ్;
            ptr++;
        }
        లేకపోతే ఒకవేళ( *ptr == '?' )
        {
            ట్యాగ్_type = CV_XML_శీర్షిక_ట్యాగ్;
            ptr++;
        }
        లేకపోతే ఒకవేళ( *ptr == '!' )
        {
            ట్యాగ్_type = CV_XML_DIRECTIVE_ట్యాగ్;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        లేకపోతే
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        ట్యాగ్_name.clear();
        type_name.clear();

        కొరకు(;;)
        {
            అక్షరం c, *endptr;
            ఒకవేళ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            చేయి c = *++endptr;
            అయితే( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();

            ఒకవేళ( ట్యాగ్_name.empty() )
                ట్యాగ్_name = attrname;
            లేకపోతే
            {
                ఒకవేళ( ట్యాగ్_type == CV_XML_CLOSING_ట్యాగ్ )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ఒకవేళ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ట్యాగ్ );
                    ఒకవేళ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ఒకవేళ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ఒకవేళ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_ట్యాగ్ );
                    ఒకవేళ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ఒకవేళ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                అక్షరం quote = *ptr++;
                endptr = ptr;
                కొరకు(;;)
                {
                    c = *endptr++;
                    ఒకవేళ( c == quote )
                        ఆపు;
                    ఒకవేళ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ఒకవేళ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            తార్కిక have_space = cv_isspace(c) || c == '\0';

            ఒకవేళ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_ట్యాగ్ );
                ఒకవేళ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ఒకవేళ( c == '>' )
            {
                ఒకవేళ( ట్యాగ్_type == CV_XML_శీర్షిక_ట్యాగ్ )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                ఆపు;
            }
            లేకపోతే ఒకవేళ( c == '?' && ట్యాగ్_type == CV_XML_శీర్షిక_ట్యాగ్ )
            {
                ఒకవేళ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                ఆపు;
            }
            లేకపోతే ఒకవేళ( c == '/' && ptr[1] == '>' && ట్యాగ్_type == CV_XML_OPENING_ట్యాగ్ )  // FIXIT ptr[1] - out of bounds read without check
            {
                ట్యాగ్_type = CV_XML_ఖాళీలు_ట్యాగ్;
                ptr += 2;
                ఆపు;
            }

            ఒకవేళ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        తిరిగి ptr;
    }

    తార్కిక parse(అక్షరం* ptr)
    {
        CV_Assert( fs != 0 );

        std::string కీలకం, key2, type_name;
        పూర్ణాంకం ట్యాగ్_type = 0;
        తార్కిక ok = అబద్ధం;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_ట్యాగ్ );
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ఒకవేళ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_ట్యాగ్( ptr, కీలకం, type_name, ట్యాగ్_type );
        File_కణుపు మూలం_collection(fs->getFS(), 0, 0);

        అయితే( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ఒకవేళ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ఒకవేళ( *ptr != '\0' )
            {
                ptr = parse_ట్యాగ్( ptr, కీలకం, type_name, ట్యాగ్_type );
                ఒకవేళ( ట్యాగ్_type != CV_XML_OPENING_ట్యాగ్ || కీలకం != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_కణుపు మూలం = fs->కూడు_కణుపు(మూలం_collection, std::string(), File_కణుపు::MAP, 0);
                ptr = parse_విలువ( ptr, మూలం );
                ptr = parse_ట్యాగ్( ptr, key2, type_name, ట్యాగ్_type );
                ఒకవేళ( ట్యాగ్_type != CV_XML_CLOSING_ట్యాగ్ || కీలకం != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = నిజం;
            }
        }
        CV_Assert( fs->eof() );
        తిరిగి ok;
    }

    FileStorage_API* fs;
    అక్షరం strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    తిరిగి makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    తిరిగి makePtr<XMLParser>(fs);
}

}
