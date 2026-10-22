// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

تعداد
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

نطاق cv
{

صنف XMLEmitter : عام FileStorageEmitter
{
عام:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~XMLEmitter() {}

    فراغ writeTag( ثابت حرف* key, صحيح tag_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->bufferPtr();
        صحيح i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        ܐܢ( key && key[0] == '\0' )
            key = 0;

        ܐܢ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ܐܢ( FileNode::isCollection(struct_flags) )
            {
                ܐܢ( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ܐܚܪܢܐ
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            ܐܢ( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ܐܢ( !key )
            key = "_";
        ܐܚܪܢܐ ܐܢ( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)strlen( key );
        *ptr++ = '<';
        ܐܢ( tag_type == CV_XML_CLOSING_TAG )
        {
            ܐܢ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ܐܢ( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = key[i];
            ܐܢ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        صحيح nattr = (صحيح)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        لكل( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (صحيح)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ܐܢ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ܐܢ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* key,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        ܐܢ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ܦܢܐ current_struct;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    فراغ اكتب(ثابت حرف* key, صحيح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( key, ptr);
    }

    فراغ اكتب(ثابت حرف* key, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, ܫܪܪܐ );
        writeScalar( key, ptr);
    }

    فراغ اكتب( ثابت حرف* key, مزدوج value )
    {
        حرف buf[128];
        writeScalar( key, fs::doubleToString( buf, حجم(buf), value, ܕܓܠܐ ) );
    }

    فراغ اكتب(ثابت حرف* key, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        ܐܢ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        ܐܢ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ܐܢ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                ܐܢ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = ܫܪܪܐ;
                }
                ܐܚܪܢܐ ܐܢ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ܐܢ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    ܐܚܪܢܐ ܐܢ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    ܐܚܪܢܐ ܐܢ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    ܐܚܪܢܐ ܐܢ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    ܐܚܪܢܐ ܐܢ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    ܐܚܪܢܐ
                    {
                        snprintf( data, حجم(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                ܐܚܪܢܐ
                    *data++ = c;
            }
            ܐܢ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ܫܪܪܐ;

            ܐܢ( need_quote )
                *data++ = '\"';
            len = (صحيح)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (صحيح)!need_quote;
        }

        writeScalar( key, data );
    }

    فراغ writeScalar(ثابت حرف* key, ثابت حرف* data)
    {
        fs->check_if_write_struct_is_delayed(ܕܓܠܐ);
        ܐܢ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ܐܚܪܢܐ ܐܢ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح len = (صحيح)strlen(data);
        ܐܢ( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        ܐܢ( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            حرف* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        ܐܚܪܢܐ
        {
            حرف* ptr = fs->bufferPtr();
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + len;

            ܐܢ( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            ܐܢ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ܐܚܪܢܐ ܐܢ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        صحيح len;
        صحيح multiline;
        ثابت حرف* eol;
        حرف* ptr;

        ܐܢ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ܐܢ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ܐܢ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        ܐܚܪܢܐ ܐܢ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ܐܢ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحيح)strlen(ptr);
        }
        ܐܚܪܢܐ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ܐܢ( multiline )
        {
            ܟܡܐ( comment )
            {
                ܐܢ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ܐܚܪܢܐ
                {
                    len = (صحيح)strlen(comment);
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

    فراغ startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

محمي:
    FileStorage_API* fs;
};

صنف XMLParser : عام FileStorageParser
{
عام:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~XMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح mode )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            ܐܢ( mode == CV_XML_INSIDE_COMMENT )
            {
                ܥܒܕ c = *++ptr;
                ܟܡܐ( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ܐܢ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            ܐܚܪܢܐ ܐܢ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                ܥܒܕ
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ܐܢ( level < 0 )
                        ܦܢܐ ptr;
                } ܟܡܐ( cv_isprint_or_tab(c) );
            }
            ܐܚܪܢܐ
            {
                ܥܒܕ c = *++ptr;
                ܟܡܐ( c == ' ' || c == '\t' );

                ܐܢ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ܐܢ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ܐܚܪܢܐ ܐܢ( cv_isprint(c) )
                    اكسر;
            }

            ܐܢ( !cv_isprint(*ptr) )
            {
                ܐܢ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ܐܢ( !ptr || *ptr == '\0' )
                    اكسر;
            }
        }
        ܦܢܐ ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ܐܢ( !ptr || !*ptr )
            ܦܢܐ ܕܓܠܐ;

        // closing XML tag
        ܐܢ ( *beg == '<' )
            ܦܢܐ ܕܓܠܐ;

        // find end of the row
        ܟܡܐ( cv_isprint(*ptr) )
            ++ptr;
        ܐܢ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ܦܢܐ ܫܪܪܐ;
    }

    حرف* parseValue( حرف* ptr, FileNode& node, صحيح depth = 0 )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ܐܢ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        منطقي have_space = ܫܪܪܐ;
        صحيح value_type = node.type();
        std::string key, key2, type_name;

        لكل(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ܐܢ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ܐܢ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = ܫܪܪܐ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ܐܢ( c =='<' || c == '\0' )
            {
                صحيح tag_type = 0;
                صحيح elem_type = FileNode::NONE;

                ܐܢ( d == '/' || c == '\0' )
                    اكسر;

                ptr = parseTag( ptr, key, type_name, tag_type );

                ܐܢ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ܐܢ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                منطقي binary_string = ܕܓܠܐ;

                ܐܢ( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    ܐܢ( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    ܐܚܪܢܐ ܐܢ( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    ܐܚܪܢܐ ܐܢ( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    ܐܚܪܢܐ ܐܢ( strcmp( tn, "binary") == 0)
                        binary_string = ܫܪܪܐ;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                ܐܢ (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                ܐܚܪܢܐ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ܐܢ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ܐܢ( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ܫܪܪܐ;
            }
            ܐܚܪܢܐ
            {
                ܐܢ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                ܐܢ( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                ܐܢ( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ܟܡܐ( cv_isdigit(*endptr) )
                        endptr++;
                    ܐܢ( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    ܐܚܪܢܐ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    ܐܢ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                ܐܚܪܢܐ
                {
                    // string
                    صحيح i = 0, len, is_quoted = 0;
                    ܐܢ( c == '\"' )
                        is_quoted = 1;
                    ܐܚܪܢܐ
                        --ptr;
                    strbuf[0] = '\0';

                    لكل( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ܐܢ( !cv_isalnum(c) )
                        {
                            ܐܢ( c == '\"' )
                            {
                                ܐܢ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                اكسر;
                            }
                            ܐܚܪܢܐ ܐܢ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ܐܢ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                اكسر;
                            }
                            ܐܚܪܢܐ ܐܢ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ܐܚܪܢܐ ܐܢ( c == '&' )
                            {
                                ܐܢ( *++ptr == '#' )
                                {
                                    صحيح val, base = 10;
                                    ptr++;
                                    ܐܢ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, base );
                                    ܐܢ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                ܐܚܪܢܐ
                                {
                                    endptr = ptr;
                                    ܥܒܕ c = *++endptr;
                                    ܟܡܐ( cv_isalnum(c) );
                                    ܐܢ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحيح)(endptr - ptr);
                                    ܐܢ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ܐܚܪܢܐ ܐܢ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ܐܚܪܢܐ ܐܢ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ܐܚܪܢܐ ܐܢ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ܐܚܪܢܐ ܐܢ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ܐܚܪܢܐ
                                    {
                                        ܐܢ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ܐܢ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                ܐܢ( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    اكسر;
                have_space = ܕܓܠܐ;
            }
        }
        fs->finalizeCollection(node);

        ܦܢܐ ptr;
    }

    حرف* parseTag( حرف* ptr, std::string& tag_name,
                    std::string& type_name, صحيح& tag_type )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ܐܢ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ܐܢ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ܐܢ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        ܐܚܪܢܐ ܐܢ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        ܐܚܪܢܐ ܐܢ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        ܐܚܪܢܐ ܐܢ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ܐܚܪܢܐ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        لكل(;;)
        {
            حرف c, *endptr;
            ܐܢ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            ܥܒܕ c = *++endptr;
            ܟܡܐ( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ܐܢ( tag_name.empty() )
                tag_name = attrname;
            ܐܚܪܢܐ
            {
                ܐܢ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ܐܢ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ܐܢ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ܐܢ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ܐܢ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ܐܢ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ܐܢ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                لكل(;;)
                {
                    c = *endptr++;
                    ܐܢ( c == quote )
                        اكسر;
                    ܐܢ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ܐܢ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقي have_space = cv_isspace(c) || c == '\0';

            ܐܢ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ܐܢ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ܐܢ( c == '>' )
            {
                ܐܢ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            ܐܚܪܢܐ ܐܢ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ܐܢ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            ܐܚܪܢܐ ܐܢ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                اكسر;
            }

            ܐܢ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ܦܢܐ ptr;
    }

    منطقي parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        صحيح tag_type = 0;
        منطقي ok = ܕܓܠܐ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ܐܢ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        ܟܡܐ( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ܐܢ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ܐܢ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                ܐܢ( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ܐܢ( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ܫܪܪܐ;
            }
        }
        CV_Assert( fs->eof() );
        ܦܢܐ ok;
    }

    FileStorage_API* fs;
    حرف strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ܦܢܐ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ܦܢܐ makePtr<XMLParser>(fs);
}

}
