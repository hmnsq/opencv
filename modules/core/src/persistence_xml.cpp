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

    فراغ writeTag( ثابت حرف* کلیل, صحيح tag_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->bufferPtr();
        صحيح i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( کلیل && کلیل[0] == '\0' )
            کلیل = 0;

        إذا( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            إذا( File_گرێ::isCollection(struct_flags) )
            {
                إذا( File_گرێ::isMap(struct_flags) ^ (کلیل != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وإلا
            {
                struct_flags = File_گرێ::EMPTY + (کلیل ? File_گرێ::MAP : File_گرێ::SEQ);
                //fs->is_first = 0;
            }

            إذا( !File_گرێ::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        إذا( !کلیل )
            کلیل = "_";
        وإلا إذا( کلیل[0] == '_' && کلیل[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)strlen( کلیل );
        *ptr++ = '<';
        إذا( tag_type == CV_XML_CLOSING_TAG )
        {
            إذا( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        إذا( !cv_isalpha(کلیل[0]) && کلیل[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = کلیل[i];
            إذا( !cv_isalnum(c) && c != '_' && c != '-' )
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
            إذا( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        إذا( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_گرێ::EMPTY;
    }

    FStructData startWriteStruct(ثابت FStructData& parent, ثابت حرف* کلیل,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        إذا( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( کلیل, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = کلیل ? std::string(کلیل) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        أرجع current_struct;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    فراغ اكتب(ثابت حرف* کلیل, صحيح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلیل, ptr);
    }

    فراغ اكتب(ثابت حرف* کلیل, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, true );
        writeScalar( کلیل, ptr);
    }

    فراغ اكتب( ثابت حرف* کلیل, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلیل, fs::doubleToString( buf, حجم(buf), value, خطأ ) );
    }

    فراغ اكتب(ثابت حرف* کلیل, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                وإلا إذا( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    إذا( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    وإلا إذا( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    وإلا إذا( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    وإلا إذا( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    وإلا إذا( c == '\"' )
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
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            إذا( need_quote )
                *data++ = '\"';
            len = (صحيح)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (صحيح)!need_quote;
        }

        writeScalar( کلیل, data );
    }

    فراغ writeScalar(ثابت حرف* کلیل, ثابت حرف* data)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح len = (صحيح)strlen(data);
        إذا( کلیل && *کلیل == '\0' )
            کلیل = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( File_گرێ::isMap(struct_flags) ||
           (!File_گرێ::isCollection(struct_flags) && کلیل) )
        {
            writeTag( کلیل, CV_XML_OPENING_TAG );
            حرف* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( کلیل, CV_XML_CLOSING_TAG );
        }
        وإلا
        {
            حرف* ptr = fs->bufferPtr();
            صحيح new_offset = (صحيح)(ptr - fs->bufferStart()) + len;

            إذا( کلیل )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_گرێ::SEQ;

            إذا( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وإلا إذا( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
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

        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        إذا( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        إذا( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        وإلا إذا( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        إذا( !multiline )
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

        إذا( multiline )
        {
            طالما( comment )
            {
                إذا( eol )
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
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            إذا( mode == CV_XML_INSIDE_COMMENT )
            {
                افعل c = *++ptr;
                طالما( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                إذا( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            وإلا إذا( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                افعل
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    إذا( level < 0 )
                        أرجع ptr;
                } طالما( cv_isprint_or_tab(c) );
            }
            وإلا
            {
                افعل c = *++ptr;
                طالما( c == ' ' || c == '\t' );

                إذا( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    إذا( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                وإلا إذا( cv_isprint(c) )
                    اكسر;
            }

            إذا( !cv_isprint(*ptr) )
            {
                إذا( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                إذا( !ptr || *ptr == '\0' )
                    اكسر;
            }
        }
        أرجع ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        إذا( !ptr || !*ptr )
            أرجع خطأ;

        // closing XML tag
        إذا ( *beg == '<' )
            أرجع خطأ;

        // find end of the row
        طالما( cv_isprint(*ptr) )
            ++ptr;
        إذا ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        أرجع true;
    }

    حرف* parse_بەها( حرف* ptr, File_گرێ& گرێ, صحيح depth = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_گرێ new_elem;
        منطقي have_space = true;
        صحيح value_type = گرێ.type();
        std::string کلیل, key2, type_name;

        لكل(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            إذا( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            إذا( c =='<' || c == '\0' )
            {
                صحيح tag_type = 0;
                صحيح elem_type = File_گرێ::NONE;

                إذا( d == '/' || c == '\0' )
                    اكسر;

                ptr = parseTag( ptr, کلیل, type_name, tag_type );

                إذا( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                إذا( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                منطقي دووانەیی_string = خطأ;

                إذا( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    إذا( strcmp(tn, "str") == 0 )
                        elem_type = File_گرێ::STRING;
                    وإلا إذا( strcmp( tn, "map" ) == 0 )
                        elem_type = File_گرێ::MAP;
                    وإلا إذا( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_گرێ::SEQ;
                    وإلا إذا( strcmp( tn, "binary") == 0)
                        دووانەیی_string = true;
                }

                new_elem = fs->add_گرێ(گرێ, کلیل, elem_type, 0);
                إذا (!دووانەیی_string)
                    ptr = parse_بەها(ptr, new_elem, depth + 1);
                وإلا
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                إذا( tag_type != CV_XML_CLOSING_TAG || key2 != کلیل )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            وإلا
            {
                إذا( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_گرێ* elem = &گرێ;
                إذا( گرێ.type() != File_گرێ::NONE )
                {
                    fs->convertToCollection( File_گرێ::SEQ, گرێ );
                    new_elem = fs->add_گرێ(گرێ, std::string(), File_گرێ::NONE, 0);
                    elem = &new_elem;
                }

                إذا( value_type != File_گرێ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    طالما( cv_isdigit(*endptr) )
                        endptr++;
                    إذا( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->set_بەها(File_گرێ::REAL, &fval);
                    }
                    وإلا
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_بەها(File_گرێ::INT, &ival);
                    }

                    إذا( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                وإلا
                {
                    // string
                    صحيح i = 0, len, is_quoted = 0;
                    إذا( c == '\"' )
                        is_quoted = 1;
                    وإلا
                        --ptr;
                    strbuf[0] = '\0';

                    لكل( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        إذا( !cv_isalnum(c) )
                        {
                            إذا( c == '\"' )
                            {
                                إذا( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                اكسر;
                            }
                            وإلا إذا( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                إذا( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                اكسر;
                            }
                            وإلا إذا( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            وإلا إذا( c == '&' )
                            {
                                إذا( *++ptr == '#' )
                                {
                                    صحيح val, base = 10;
                                    ptr++;
                                    إذا( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, base );
                                    إذا( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                وإلا
                                {
                                    endptr = ptr;
                                    افعل c = *++endptr;
                                    طالما( cv_isalnum(c) );
                                    إذا( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحيح)(endptr - ptr);
                                    إذا( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    وإلا إذا( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    وإلا إذا( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    وإلا إذا( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    وإلا إذا( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    وإلا
                                    {
                                        إذا (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        إذا (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_بەها(File_گرێ::STRING, strbuf, i);
                }

                إذا( value_type != File_گرێ::NONE && value_type != File_گرێ::SEQ && value_type != File_گرێ::MAP )
                    اكسر;
                have_space = خطأ;
            }
        }
        fs->finalizeCollection(گرێ);

        أرجع ptr;
    }

    حرف* parseTag( حرف* ptr, std::string& tag_name,
                    std::string& type_name, صحيح& tag_type )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        إذا( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        إذا( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        إذا( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        وإلا إذا( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        وإلا إذا( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        وإلا إذا( *ptr == '!' )
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
            إذا( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            افعل c = *++endptr;
            طالما( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            إذا( tag_name.empty() )
                tag_name = attrname;
            وإلا
            {
                إذا( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                إذا( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    إذا( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                إذا( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    إذا(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    إذا( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                لكل(;;)
                {
                    c = *endptr++;
                    إذا( c == quote )
                        اكسر;
                    إذا( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                إذا( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقي have_space = cv_isspace(c) || c == '\0';

            إذا( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            إذا( c == '>' )
            {
                إذا( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            وإلا إذا( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                إذا( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            وإلا إذا( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                اكسر;
            }

            إذا( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        أرجع ptr;
    }

    منطقي parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string کلیل, key2, type_name;
        صحيح tag_type = 0;
        منطقي ok = خطأ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, کلیل, type_name, tag_type );
        File_گرێ ڕەگ_collection(fs->getFS(), 0, 0);

        طالما( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            إذا( *ptr != '\0' )
            {
                ptr = parseTag( ptr, کلیل, type_name, tag_type );
                إذا( tag_type != CV_XML_OPENING_TAG || کلیل != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_گرێ ڕەگ = fs->add_گرێ(ڕەگ_collection, std::string(), File_گرێ::MAP, 0);
                ptr = parse_بەها( ptr, ڕەگ );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                إذا( tag_type != CV_XML_CLOSING_TAG || کلیل != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
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
