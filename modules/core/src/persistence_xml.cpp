// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence.hpp"

எண்ணிக்கை
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

நாமாகாஶ cv
{

வகுப்பு XMLEmitter : பொது FileStorageEmitter
{
பொது:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    மெய்நிகர் ~XMLEmitter() {}

    வெறுமை writeTag( மாறிலி எழுத்து* சாவி, முழு tag_type, மாறிலி std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        எழுத்து* ptr = fs->bufferPtr();
        முழு i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        முழு struct_flags = current_struct.flags;

        எனில்( சாவி && சாவி[0] == '\0' )
            சாவி = 0;

        எனில்( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            எனில்( File_முனை::isCollection(struct_flags) )
            {
                எனில்( File_முனை::isMap(struct_flags) ^ (சாவி != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            இல்லையெனில்
            {
                struct_flags = File_முனை::EMPTY + (சாவி ? File_முனை::MAP : File_முனை::SEQ);
                //fs->is_first = 0;
            }

            எனில்( !File_முனை::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        எனில்( !சாவி )
            சாவி = "_";
        இல்லையெனில் எனில்( சாவி[0] == '_' && சாவி[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (முழு)strlen( சாவி );
        *ptr++ = '<';
        எனில்( tag_type == CV_XML_CLOSING_TAG )
        {
            எனில்( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        எனில்( !cv_isalpha(சாவி[0]) && சாவி[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        க்கு( i = 0; i < len; i++ )
        {
            எழுத்து c = சாவி[i];
            எனில்( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        முழு nattr = (முழு)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        க்கு( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (முழு)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            எனில்( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        எனில்( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_முனை::EMPTY;
    }

    FStructData startWriteStruct(மாறிலி FStructData& parent, மாறிலி எழுத்து* சாவி,
                                 முழு struct_flags, மாறிலி எழுத்து* type_name=0)
    {
        std::vector<std::string> attrlist;
        எனில்( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( சாவி, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = சாவி ? std::string(சாவி) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        திரும்பு current_struct;
    }

    வெறுமை endWriteStruct(மாறிலி FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, முழு value)
    {
        எழுத்து buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( சாவி, ptr);
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, int64_t value)
    {
        எழுத்து buf[128], *ptr = fs::itoa( value, buf, 10, உண்மை );
        writeScalar( சாவி, ptr);
    }

    வெறுமை லிக( மாறிலி எழுத்து* சாவி, இரட்டை value )
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::doubleToString( buf, அளவு(buf), value, பொய் ) );
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* str, தருக்க quote)
    {
        எழுத்து buf[CV_FS_MAX_LEN*6+16];
        எழுத்து* data = (எழுத்து*)str;
        முழு i, len;

        எனில்( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (முழு)strlen(str);
        எனில்( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        எனில்( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            தருக்க need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            க்கு( i = 0; i < len; i++ )
            {
                எழுத்து c = str[i];

                எனில்( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = உண்மை;
                }
                இல்லையெனில் எனில்( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    எனில்( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    இல்லையெனில் எனில்( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    இல்லையெனில் எனில்( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    இல்லையெனில் எனில்( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    இல்லையெனில் எனில்( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    இல்லையெனில்
                    {
                        snprintf( data, அளவு(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                இல்லையெனில்
                    *data++ = c;
            }
            எனில்( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = உண்மை;

            எனில்( need_quote )
                *data++ = '\"';
            len = (முழு)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (முழு)!need_quote;
        }

        writeScalar( சாவி, data );
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

        முழு len = (முழு)strlen(data);
        எனில்( சாவி && *சாவி == '\0' )
            சாவி = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        முழு struct_flags = current_struct.flags;

        எனில்( File_முனை::isMap(struct_flags) ||
           (!File_முனை::isCollection(struct_flags) && சாவி) )
        {
            writeTag( சாவி, CV_XML_OPENING_TAG );
            எழுத்து* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( சாவி, CV_XML_CLOSING_TAG );
        }
        இல்லையெனில்
        {
            எழுத்து* ptr = fs->bufferPtr();
            முழு new_offset = (முழு)(ptr - fs->bufferStart()) + len;

            எனில்( சாவி )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_முனை::SEQ;

            எனில்( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            இல்லையெனில் எனில்( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    வெறுமை writeComment(மாறிலி எழுத்து* comment, தருக்க eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        முழு len;
        முழு multiline;
        மாறிலி எழுத்து* eol;
        எழுத்து* ptr;

        எனில்( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        எனில்( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (முழு)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        எனில்( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        இல்லையெனில் எனில்( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        எனில்( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (முழு)strlen(ptr);
        }
        இல்லையெனில்
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        எனில்( multiline )
        {
            போது( comment )
            {
                எனில்( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (முழு)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                இல்லையெனில்
                {
                    len = (முழு)strlen(comment);
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

    வெறுமை startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

பாதுகாக்கப்பட்ட:
    FileStorage_API* fs;
};

வகுப்பு XMLParser : பொது FileStorageParser
{
பொது:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    மெய்நிகர் ~XMLParser() {}

    எழுத்து* skipSpaces( எழுத்து* ptr, முழு mode )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        முழு level = 0;

        க்கு(;;)
        {
            எழுத்து c;
            ptr--;

            எனில்( mode == CV_XML_INSIDE_COMMENT )
            {
                செய் c = *++ptr;
                போது( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                எனில்( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            இல்லையெனில் எனில்( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                செய்
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    எனில்( level < 0 )
                        திரும்பு ptr;
                } போது( cv_isprint_or_tab(c) );
            }
            இல்லையெனில்
            {
                செய் c = *++ptr;
                போது( c == ' ' || c == '\t' );

                எனில்( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    எனில்( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                இல்லையெனில் எனில்( cv_isprint(c) )
                    நிறுத்து;
            }

            எனில்( !cv_isprint(*ptr) )
            {
                எனில்( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                எனில்( !ptr || *ptr == '\0' )
                    நிறுத்து;
            }
        }
        திரும்பு ptr;
    }

    தருக்க getBase64Row(எழுத்து* ptr, முழு /*indent*/, எழுத்து* &beg, எழுத்து* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        எனில்( !ptr || !*ptr )
            திரும்பு பொய்;

        // closing XML tag
        எனில் ( *beg == '<' )
            திரும்பு பொய்;

        // find end of the row
        போது( cv_isprint(*ptr) )
            ++ptr;
        எனில் ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        திரும்பு உண்மை;
    }

    எழுத்து* parse_மதிப்பு( எழுத்து* ptr, File_முனை& முனை, முழு depth = 0 )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        எனில்( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_முனை new_elem;
        தருக்க have_space = உண்மை;
        முழு value_type = முனை.type();
        std::string சாவி, key2, type_name;

        க்கு(;;)
        {
            எழுத்து c = *ptr, d;
            எழுத்து* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            எனில்( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                எனில் (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = உண்மை;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            எனில்( c =='<' || c == '\0' )
            {
                முழு tag_type = 0;
                முழு elem_type = File_முனை::NONE;

                எனில்( d == '/' || c == '\0' )
                    நிறுத்து;

                ptr = parseTag( ptr, சாவி, type_name, tag_type );

                எனில்( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                எனில்( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                தருக்க இருமம்_string = பொய்;

                எனில்( !type_name.empty() )
                {
                    மாறிலி எழுத்து* tn = type_name.c_str();
                    எனில்( strcmp(tn, "str") == 0 )
                        elem_type = File_முனை::STRING;
                    இல்லையெனில் எனில்( strcmp( tn, "map" ) == 0 )
                        elem_type = File_முனை::MAP;
                    இல்லையெனில் எனில்( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_முனை::SEQ;
                    இல்லையெனில் எனில்( strcmp( tn, "binary") == 0)
                        இருமம்_string = உண்மை;
                }

                new_elem = fs->add_முனை(முனை, சாவி, elem_type, 0);
                எனில் (!இருமம்_string)
                    ptr = parse_மதிப்பு(ptr, new_elem, depth + 1);
                இல்லையெனில்
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    எனில் (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                எனில்( tag_type != CV_XML_CLOSING_TAG || key2 != சாவி )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = உண்மை;
            }
            இல்லையெனில்
            {
                எனில்( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_முனை* elem = &முனை;
                எனில்( முனை.type() != File_முனை::NONE )
                {
                    fs->convertToCollection( File_முனை::SEQ, முனை );
                    new_elem = fs->add_முனை(முனை, std::string(), File_முனை::NONE, 0);
                    elem = &new_elem;
                }

                எனில்( value_type != File_முனை::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    போது( cv_isdigit(*endptr) )
                        endptr++;
                    எனில்( *endptr == '.' || *endptr == 'e' )
                    {
                        இரட்டை fval = fs->strtod( ptr, &endptr );
                        elem->set_மதிப்பு(File_முனை::REAL, &fval);
                    }
                    இல்லையெனில்
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_மதிப்பு(File_முனை::INT, &ival);
                    }

                    எனில்( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                இல்லையெனில்
                {
                    // string
                    முழு i = 0, len, is_quoted = 0;
                    எனில்( c == '\"' )
                        is_quoted = 1;
                    இல்லையெனில்
                        --ptr;
                    strbuf[0] = '\0';

                    க்கு( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        எனில்( !cv_isalnum(c) )
                        {
                            எனில்( c == '\"' )
                            {
                                எனில்( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                நிறுத்து;
                            }
                            இல்லையெனில் எனில்( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                எனில்( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                நிறுத்து;
                            }
                            இல்லையெனில் எனில்( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            இல்லையெனில் எனில்( c == '&' )
                            {
                                எனில்( *++ptr == '#' )
                                {
                                    முழு val, base = 10;
                                    ptr++;
                                    எனில்( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (முழு)strtol( ptr, &endptr, base );
                                    எனில்( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (எழுத்து)val;
                                }
                                இல்லையெனில்
                                {
                                    endptr = ptr;
                                    செய் c = *++endptr;
                                    போது( cv_isalnum(c) );
                                    எனில்( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (முழு)(endptr - ptr);
                                    எனில்( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    இல்லையெனில் எனில்( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    இல்லையெனில் எனில்( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    இல்லையெனில் எனில்( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    இல்லையெனில் எனில்( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    இல்லையெனில்
                                    {
                                        எனில் (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        எனில் (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_மதிப்பு(File_முனை::STRING, strbuf, i);
                }

                எனில்( value_type != File_முனை::NONE && value_type != File_முனை::SEQ && value_type != File_முனை::MAP )
                    நிறுத்து;
                have_space = பொய்;
            }
        }
        fs->finalizeCollection(முனை);

        திரும்பு ptr;
    }

    எழுத்து* parseTag( எழுத்து* ptr, std::string& tag_name,
                    std::string& type_name, முழு& tag_type )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        எனில்( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        எனில்( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        எனில்( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        இல்லையெனில் எனில்( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        இல்லையெனில் எனில்( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        இல்லையெனில் எனில்( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        இல்லையெனில்
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        க்கு(;;)
        {
            எழுத்து c, *endptr;
            எனில்( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            செய் c = *++endptr;
            போது( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            எனில்( tag_name.empty() )
                tag_name = attrname;
            இல்லையெனில்
            {
                எனில்( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                எனில்( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    எனில் (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    எனில்( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                எனில்( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    எனில்(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    எனில்( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                எழுத்து quote = *ptr++;
                endptr = ptr;
                க்கு(;;)
                {
                    c = *endptr++;
                    எனில்( c == quote )
                        நிறுத்து;
                    எனில்( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                எனில்( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            தருக்க have_space = cv_isspace(c) || c == '\0';

            எனில்( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                எனில் (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            எனில்( c == '>' )
            {
                எனில்( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                நிறுத்து;
            }
            இல்லையெனில் எனில்( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                எனில்( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                நிறுத்து;
            }
            இல்லையெனில் எனில்( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                நிறுத்து;
            }

            எனில்( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        திரும்பு ptr;
    }

    தருக்க parse(எழுத்து* ptr)
    {
        CV_Assert( fs != 0 );

        std::string சாவி, key2, type_name;
        முழு tag_type = 0;
        தருக்க ok = பொய்;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        எனில்( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, சாவி, type_name, tag_type );
        File_முனை வேர்_collection(fs->getFS(), 0, 0);

        போது( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            எனில் (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            எனில்( *ptr != '\0' )
            {
                ptr = parseTag( ptr, சாவி, type_name, tag_type );
                எனில்( tag_type != CV_XML_OPENING_TAG || சாவி != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_முனை வேர் = fs->add_முனை(வேர்_collection, std::string(), File_முனை::MAP, 0);
                ptr = parse_மதிப்பு( ptr, வேர் );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                எனில்( tag_type != CV_XML_CLOSING_TAG || சாவி != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = உண்மை;
            }
        }
        CV_Assert( fs->eof() );
        திரும்பு ok;
    }

    FileStorage_API* fs;
    எழுத்து strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    திரும்பு makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    திரும்பு makePtr<XMLParser>(fs);
}

}
