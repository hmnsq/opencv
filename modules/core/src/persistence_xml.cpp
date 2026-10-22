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

        𐡄𐡍( key && key[0] == '\0' )
            key = 0;

        𐡄𐡍( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            𐡄𐡍( FileNode::isCollection(struct_flags) )
            {
                𐡄𐡍( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وإلا
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            𐡄𐡍( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        𐡄𐡍( !key )
            key = "_";
        وإلا 𐡄𐡍( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)strlen( key );
        *ptr++ = '<';
        𐡄𐡍( tag_type == CV_XML_CLOSING_TAG )
        {
            𐡄𐡍( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        𐡄𐡍( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = key[i];
            𐡄𐡍( !cv_isalnum(c) && c != '_' && c != '-' )
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
            𐡄𐡍( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        𐡄𐡍( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* key,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        𐡄𐡍( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        أرجع current_struct;
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
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, 𐡒𐡔𐡈 );
        writeScalar( key, ptr);
    }

    فراغ اكتب( ثابت حرف* key, مزدوج value )
    {
        حرف buf[128];
        writeScalar( key, fs::doubleToString( buf, حجم(buf), value, خطأ ) );
    }

    فراغ اكتب(ثابت حرف* key, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        𐡄𐡍( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        𐡄𐡍( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        𐡄𐡍( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                𐡄𐡍( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = 𐡒𐡔𐡈;
                }
                وإلا 𐡄𐡍( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    𐡄𐡍( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    وإلا 𐡄𐡍( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    وإلا 𐡄𐡍( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    وإلا 𐡄𐡍( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    وإلا 𐡄𐡍( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    وإلا
                    {
                        snprintf( data, حجم(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                وإلا
                    *data++ = c;
            }
            𐡄𐡍( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 𐡒𐡔𐡈;

            𐡄𐡍( need_quote )
                *data++ = '\"';
            len = (صحيح)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (صحيح)!need_quote;
        }

        writeScalar( key, data );
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

        صحيح len = (صحيح)strlen(data);
        𐡄𐡍( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        𐡄𐡍( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            حرف* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        وإلا
        {
            حرف* ptr = fs->bufferPtr();
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + len;

            𐡄𐡍( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            𐡄𐡍( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وإلا 𐡄𐡍( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
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

        𐡄𐡍( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        𐡄𐡍( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        𐡄𐡍( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        وإلا 𐡄𐡍( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        𐡄𐡍( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحيح)strlen(ptr);
        }
        وإلا
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        𐡄𐡍( multiline )
        {
            طالما( comment )
            {
                𐡄𐡍( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                وإلا
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
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            𐡄𐡍( mode == CV_XML_INSIDE_COMMENT )
            {
                𐡏𐡁𐡃 c = *++ptr;
                طالما( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                𐡄𐡍( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            وإلا 𐡄𐡍( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                𐡏𐡁𐡃
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    𐡄𐡍( level < 0 )
                        أرجع ptr;
                } طالما( cv_isprint_or_tab(c) );
            }
            وإلا
            {
                𐡏𐡁𐡃 c = *++ptr;
                طالما( c == ' ' || c == '\t' );

                𐡄𐡍( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    𐡄𐡍( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                وإلا 𐡄𐡍( cv_isprint(c) )
                    اكسر;
            }

            𐡄𐡍( !cv_isprint(*ptr) )
            {
                𐡄𐡍( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                𐡄𐡍( !ptr || *ptr == '\0' )
                    اكسر;
            }
        }
        أرجع ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        𐡄𐡍( !ptr || !*ptr )
            أرجع خطأ;

        // closing XML tag
        𐡄𐡍 ( *beg == '<' )
            أرجع خطأ;

        // find end of the row
        طالما( cv_isprint(*ptr) )
            ++ptr;
        𐡄𐡍 ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        أرجع 𐡒𐡔𐡈;
    }

    حرف* parseValue( حرف* ptr, FileNode& node, صحيح depth = 0 )
    {
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        𐡄𐡍( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        منطقي have_space = 𐡒𐡔𐡈;
        صحيح value_type = node.type();
        std::string key, key2, type_name;

        لكل(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            𐡄𐡍( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                𐡄𐡍 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = 𐡒𐡔𐡈;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            𐡄𐡍( c =='<' || c == '\0' )
            {
                صحيح tag_type = 0;
                صحيح elem_type = FileNode::NONE;

                𐡄𐡍( d == '/' || c == '\0' )
                    اكسر;

                ptr = parseTag( ptr, key, type_name, tag_type );

                𐡄𐡍( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                𐡄𐡍( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                منطقي binary_string = خطأ;

                𐡄𐡍( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    𐡄𐡍( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    وإلا 𐡄𐡍( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    وإلا 𐡄𐡍( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    وإلا 𐡄𐡍( strcmp( tn, "binary") == 0)
                        binary_string = 𐡒𐡔𐡈;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                𐡄𐡍 (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                وإلا
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    𐡄𐡍 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                𐡄𐡍( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = 𐡒𐡔𐡈;
            }
            وإلا
            {
                𐡄𐡍( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                𐡄𐡍( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                𐡄𐡍( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    طالما( cv_isdigit(*endptr) )
                        endptr++;
                    𐡄𐡍( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    وإلا
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    𐡄𐡍( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                وإلا
                {
                    // string
                    صحيح i = 0, len, is_quoted = 0;
                    𐡄𐡍( c == '\"' )
                        is_quoted = 1;
                    وإلا
                        --ptr;
                    strbuf[0] = '\0';

                    لكل( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        𐡄𐡍( !cv_isalnum(c) )
                        {
                            𐡄𐡍( c == '\"' )
                            {
                                𐡄𐡍( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                اكسر;
                            }
                            وإلا 𐡄𐡍( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                𐡄𐡍( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                اكسر;
                            }
                            وإلا 𐡄𐡍( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            وإلا 𐡄𐡍( c == '&' )
                            {
                                𐡄𐡍( *++ptr == '#' )
                                {
                                    صحيح val, base = 10;
                                    ptr++;
                                    𐡄𐡍( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, base );
                                    𐡄𐡍( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                وإلا
                                {
                                    endptr = ptr;
                                    𐡏𐡁𐡃 c = *++endptr;
                                    طالما( cv_isalnum(c) );
                                    𐡄𐡍( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحيح)(endptr - ptr);
                                    𐡄𐡍( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    وإلا 𐡄𐡍( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    وإلا 𐡄𐡍( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    وإلا 𐡄𐡍( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    وإلا 𐡄𐡍( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    وإلا
                                    {
                                        𐡄𐡍 (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        𐡄𐡍 (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                𐡄𐡍( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    اكسر;
                have_space = خطأ;
            }
        }
        fs->finalizeCollection(node);

        أرجع ptr;
    }

    حرف* parseTag( حرف* ptr, std::string& tag_name,
                    std::string& type_name, صحيح& tag_type )
    {
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        𐡄𐡍( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        𐡄𐡍( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        𐡄𐡍( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        وإلا 𐡄𐡍( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        وإلا 𐡄𐡍( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        وإلا 𐡄𐡍( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        وإلا
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        لكل(;;)
        {
            حرف c, *endptr;
            𐡄𐡍( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            𐡏𐡁𐡃 c = *++endptr;
            طالما( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            𐡄𐡍( tag_name.empty() )
                tag_name = attrname;
            وإلا
            {
                𐡄𐡍( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                𐡄𐡍( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    𐡄𐡍 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    𐡄𐡍( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                𐡄𐡍( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    𐡄𐡍(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    𐡄𐡍( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                لكل(;;)
                {
                    c = *endptr++;
                    𐡄𐡍( c == quote )
                        اكسر;
                    𐡄𐡍( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                𐡄𐡍( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقي have_space = cv_isspace(c) || c == '\0';

            𐡄𐡍( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                𐡄𐡍 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            𐡄𐡍( c == '>' )
            {
                𐡄𐡍( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            وإلا 𐡄𐡍( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                𐡄𐡍( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            وإلا 𐡄𐡍( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                اكسر;
            }

            𐡄𐡍( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        أرجع ptr;
    }

    منطقي parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        صحيح tag_type = 0;
        منطقي ok = خطأ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        𐡄𐡍 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        𐡄𐡍( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        طالما( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            𐡄𐡍 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            𐡄𐡍( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                𐡄𐡍( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                𐡄𐡍( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = 𐡒𐡔𐡈;
            }
        }
        CV_Assert( fs->eof() );
        أرجع ok;
    }

    FileStorage_API* fs;
    حرف strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    أرجع makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    أرجع makePtr<XMLParser>(fs);
}

}
