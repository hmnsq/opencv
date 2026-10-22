// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "persistence.hpp"

شمارش
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_TAG = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_TAG = 1,
    CV_XML_CLOSING_TAG = 2,
    CV_XML_EMPTY_TAG = 3,
    CV_XML_HEADER_TAG = 4,
    CV_XML_DIRECTIVE_TAG = 5
};

فضاینام cv
{

رده XMLEmitter : عمومی FileStorageEmitter
{
عمومی:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~XMLEmitter() {}

    تهی writeTag( ثابت نویسه* کلید, صحیح tag_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        نویسه* ptr = fs->bufferPtr();
        صحیح i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;

        اگر( کلید && کلید[0] == '\0' )
            کلید = 0;

        اگر( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            اگر( File_گره::isCollection(struct_flags) )
            {
                اگر( File_گره::isMap(struct_flags) ^ (کلید != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وگرنه
            {
                struct_flags = File_گره::EMPTY + (کلید ? File_گره::MAP : File_گره::SEQ);
                //fs->is_first = 0;
            }

            اگر( !File_گره::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        اگر( !کلید )
            کلید = "_";
        وگرنه اگر( کلید[0] == '_' && کلید[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحیح)strlen( کلید );
        *ptr++ = '<';
        اگر( tag_type == CV_XML_CLOSING_TAG )
        {
            اگر( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        برای( i = 0; i < len; i++ )
        {
            نویسه c = کلید[i];
            اگر( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        صحیح nattr = (صحیح)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        برای( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (صحیح)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            اگر( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        اگر( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_گره::EMPTY;
    }

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت نویسه* کلید,
                                 صحیح struct_flags, ثابت نویسه* type_name=0)
    {
        std::vector<std::string> attrlist;
        اگر( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( کلید, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = کلید ? std::string(کلید) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        بازگشت current_struct;
    }

    تهی endWriteStruct(ثابت FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    تهی اكتب(ثابت نویسه* کلید, صحیح value)
    {
        نویسه buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلید, ptr);
    }

    تهی اكتب(ثابت نویسه* کلید, int64_t value)
    {
        نویسه buf[128], *ptr = fs::itoa( value, buf, 10, درست );
        writeScalar( کلید, ptr);
    }

    تهی اكتب( ثابت نویسه* کلید, دوگانه value )
    {
        نویسه buf[128];
        writeScalar( کلید, fs::doubleToString( buf, اندازه(buf), value, نادرست ) );
    }

    تهی اكتب(ثابت نویسه* کلید, ثابت نویسه* str, منطقی quote)
    {
        نویسه buf[CV_FS_MAX_LEN*6+16];
        نویسه* data = (نویسه*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)strlen(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقی need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            برای( i = 0; i < len; i++ )
            {
                نویسه c = str[i];

                اگر( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = درست;
                }
                وگرنه اگر( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    اگر( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    وگرنه اگر( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    وگرنه اگر( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    وگرنه اگر( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    وگرنه اگر( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    وگرنه
                    {
                        snprintf( data, اندازه(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                وگرنه
                    *data++ = c;
            }
            اگر( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = درست;

            اگر( need_quote )
                *data++ = '\"';
            len = (صحیح)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (صحیح)!need_quote;
        }

        writeScalar( کلید, data );
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

        صحیح len = (صحیح)strlen(data);
        اگر( کلید && *کلید == '\0' )
            کلید = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;

        اگر( File_گره::isMap(struct_flags) ||
           (!File_گره::isCollection(struct_flags) && کلید) )
        {
            writeTag( کلید, CV_XML_OPENING_TAG );
            نویسه* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( کلید, CV_XML_CLOSING_TAG );
        }
        وگرنه
        {
            نویسه* ptr = fs->bufferPtr();
            صحیح new_offset = (صحیح)(ptr - fs->bufferStart()) + len;

            اگر( کلید )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_گره::SEQ;

            اگر( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وگرنه اگر( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    تهی writeComment(ثابت نویسه* comment, منطقی eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        صحیح len;
        صحیح multiline;
        ثابت نویسه* eol;
        نویسه* ptr;

        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        اگر( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحیح)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        اگر( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        وگرنه اگر( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        اگر( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحیح)strlen(ptr);
        }
        وگرنه
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        اگر( multiline )
        {
            وقتی( comment )
            {
                اگر( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (صحیح)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                وگرنه
                {
                    len = (صحیح)strlen(comment);
                    ptr = fs->resizeWriteBuffer( ptr, len );
                    memcpy( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->setBufferPtr(ptr + 3);
            fs->flush();
        }
    }

    تهی startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

محافظتشده:
    FileStorage_API* fs;
};

رده XMLParser : عمومی FileStorageParser
{
عمومی:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~XMLParser() {}

    نویسه* skipSpaces( نویسه* ptr, صحیح mode )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحیح level = 0;

        برای(;;)
        {
            نویسه c;
            ptr--;

            اگر( mode == CV_XML_INSIDE_COMMENT )
            {
                انجامبده c = *++ptr;
                وقتی( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                اگر( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            وگرنه اگر( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                انجامبده
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    اگر( level < 0 )
                        بازگشت ptr;
                } وقتی( cv_isprint_or_tab(c) );
            }
            وگرنه
            {
                انجامبده c = *++ptr;
                وقتی( c == ' ' || c == '\t' );

                اگر( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    اگر( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                وگرنه اگر( cv_isprint(c) )
                    بشکن;
            }

            اگر( !cv_isprint(*ptr) )
            {
                اگر( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                اگر( !ptr || *ptr == '\0' )
                    بشکن;
            }
        }
        بازگشت ptr;
    }

    منطقی getBase64Row(نویسه* ptr, صحیح /*indent*/, نویسه* &beg, نویسه* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        اگر( !ptr || !*ptr )
            بازگشت نادرست;

        // closing XML tag
        اگر ( *beg == '<' )
            بازگشت نادرست;

        // find end of the row
        وقتی( cv_isprint(*ptr) )
            ++ptr;
        اگر ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        بازگشت درست;
    }

    نویسه* parse_مقدار( نویسه* ptr, File_گره& گره, صحیح depth = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_گره new_elem;
        منطقی have_space = درست;
        صحیح value_type = گره.type();
        std::string کلید, key2, type_name;

        برای(;;)
        {
            نویسه c = *ptr, d;
            نویسه* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            اگر( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = درست;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            اگر( c =='<' || c == '\0' )
            {
                صحیح tag_type = 0;
                صحیح elem_type = File_گره::NONE;

                اگر( d == '/' || c == '\0' )
                    بشکن;

                ptr = parseTag( ptr, کلید, type_name, tag_type );

                اگر( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                اگر( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                منطقی دودویی_string = نادرست;

                اگر( !type_name.empty() )
                {
                    ثابت نویسه* tn = type_name.c_str();
                    اگر( strcmp(tn, "str") == 0 )
                        elem_type = File_گره::STRING;
                    وگرنه اگر( strcmp( tn, "map" ) == 0 )
                        elem_type = File_گره::MAP;
                    وگرنه اگر( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_گره::SEQ;
                    وگرنه اگر( strcmp( tn, "binary") == 0)
                        دودویی_string = درست;
                }

                new_elem = fs->add_گره(گره, کلید, elem_type, 0);
                اگر (!دودویی_string)
                    ptr = parse_مقدار(ptr, new_elem, depth + 1);
                وگرنه
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                اگر( tag_type != CV_XML_CLOSING_TAG || key2 != کلید )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = درست;
            }
            وگرنه
            {
                اگر( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_گره* elem = &گره;
                اگر( گره.type() != File_گره::NONE )
                {
                    fs->convertToCollection( File_گره::SEQ, گره );
                    new_elem = fs->add_گره(گره, std::string(), File_گره::NONE, 0);
                    elem = &new_elem;
                }

                اگر( value_type != File_گره::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    وقتی( cv_isdigit(*endptr) )
                        endptr++;
                    اگر( *endptr == '.' || *endptr == 'e' )
                    {
                        دوگانه fval = fs->strtod( ptr, &endptr );
                        elem->set_مقدار(File_گره::REAL, &fval);
                    }
                    وگرنه
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_مقدار(File_گره::INT, &ival);
                    }

                    اگر( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                وگرنه
                {
                    // string
                    صحیح i = 0, len, is_quoted = 0;
                    اگر( c == '\"' )
                        is_quoted = 1;
                    وگرنه
                        --ptr;
                    strbuf[0] = '\0';

                    برای( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        اگر( !cv_isalnum(c) )
                        {
                            اگر( c == '\"' )
                            {
                                اگر( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                بشکن;
                            }
                            وگرنه اگر( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                اگر( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                بشکن;
                            }
                            وگرنه اگر( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            وگرنه اگر( c == '&' )
                            {
                                اگر( *++ptr == '#' )
                                {
                                    صحیح val, base = 10;
                                    ptr++;
                                    اگر( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (صحیح)strtol( ptr, &endptr, base );
                                    اگر( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (نویسه)val;
                                }
                                وگرنه
                                {
                                    endptr = ptr;
                                    انجامبده c = *++endptr;
                                    وقتی( cv_isalnum(c) );
                                    اگر( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحیح)(endptr - ptr);
                                    اگر( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    وگرنه اگر( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    وگرنه اگر( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    وگرنه اگر( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    وگرنه اگر( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    وگرنه
                                    {
                                        اگر (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        اگر (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_مقدار(File_گره::STRING, strbuf, i);
                }

                اگر( value_type != File_گره::NONE && value_type != File_گره::SEQ && value_type != File_گره::MAP )
                    بشکن;
                have_space = نادرست;
            }
        }
        fs->finalizeCollection(گره);

        بازگشت ptr;
    }

    نویسه* parseTag( نویسه* ptr, std::string& tag_name,
                    std::string& type_name, صحیح& tag_type )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        اگر( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        اگر( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        اگر( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        وگرنه اگر( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        وگرنه اگر( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        وگرنه اگر( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        وگرنه
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        برای(;;)
        {
            نویسه c, *endptr;
            اگر( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            انجامبده c = *++endptr;
            وقتی( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            اگر( tag_name.empty() )
                tag_name = attrname;
            وگرنه
            {
                اگر( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                اگر( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    اگر (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    اگر( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                اگر( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    اگر(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    اگر( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                نویسه quote = *ptr++;
                endptr = ptr;
                برای(;;)
                {
                    c = *endptr++;
                    اگر( c == quote )
                        بشکن;
                    اگر( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                اگر( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقی have_space = cv_isspace(c) || c == '\0';

            اگر( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                اگر (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            اگر( c == '>' )
            {
                اگر( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                بشکن;
            }
            وگرنه اگر( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                اگر( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                بشکن;
            }
            وگرنه اگر( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                بشکن;
            }

            اگر( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        بازگشت ptr;
    }

    منطقی parse(نویسه* ptr)
    {
        CV_Assert( fs != 0 );

        std::string کلید, key2, type_name;
        صحیح tag_type = 0;
        منطقی ok = نادرست;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, کلید, type_name, tag_type );
        File_گره ریشه_collection(fs->getFS(), 0, 0);

        وقتی( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            اگر (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            اگر( *ptr != '\0' )
            {
                ptr = parseTag( ptr, کلید, type_name, tag_type );
                اگر( tag_type != CV_XML_OPENING_TAG || کلید != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_گره ریشه = fs->add_گره(ریشه_collection, std::string(), File_گره::MAP, 0);
                ptr = parse_مقدار( ptr, ریشه );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                اگر( tag_type != CV_XML_CLOSING_TAG || کلید != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = درست;
            }
        }
        CV_Assert( fs->eof() );
        بازگشت ok;
    }

    FileStorage_API* fs;
    نویسه strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    بازگشت makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    بازگشت makePtr<XMLParser>(fs);
}

}
