// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence.hpp"

എണ്ണൽ
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

നാമാകാശ cv
{

വർഗ്ഗം XMLEmitter : പൊതു FileStorageEmitter
{
പൊതു:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    മായാ ~XMLEmitter() {}

    ശൂന്യം writeTag( സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ tag_type, സ്ഥിരം std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        അക്ഷരം* ptr = fs->bufferPtr();
        പൂർണ്ണസംഖ്യ i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;

        എങ്കിൽ( താക്കോൽ && താക്കോൽ[0] == '\0' )
            താക്കോൽ = 0;

        എങ്കിൽ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            എങ്കിൽ( File_കണ്ണി::isCollection(struct_flags) )
            {
                എങ്കിൽ( File_കണ്ണി::isMap(struct_flags) ^ (താക്കോൽ != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            അല്ലെങ്കിൽ
            {
                struct_flags = File_കണ്ണി::EMPTY + (താക്കോൽ ? File_കണ്ണി::MAP : File_കണ്ണി::SEQ);
                //fs->is_first = 0;
            }

            എങ്കിൽ( !File_കണ്ണി::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        എങ്കിൽ( !താക്കോൽ )
            താക്കോൽ = "_";
        അല്ലെങ്കിൽ എങ്കിൽ( താക്കോൽ[0] == '_' && താക്കോൽ[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (പൂർണ്ണസംഖ്യ)strlen( താക്കോൽ );
        *ptr++ = '<';
        എങ്കിൽ( tag_type == CV_XML_CLOSING_TAG )
        {
            എങ്കിൽ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        എങ്കിൽ( !cv_isalpha(താക്കോൽ[0]) && താക്കോൽ[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        വേണ്ടി( i = 0; i < len; i++ )
        {
            അക്ഷരം c = താക്കോൽ[i];
            എങ്കിൽ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        പൂർണ്ണസംഖ്യ nattr = (പൂർണ്ണസംഖ്യ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        വേണ്ടി( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (പൂർണ്ണസംഖ്യ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            എങ്കിൽ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        എങ്കിൽ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_കണ്ണി::EMPTY;
    }

    FStructData startWriteStruct(സ്ഥിരം FStructData& parent, സ്ഥിരം അക്ഷരം* താക്കോൽ,
                                 പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം* type_name=0)
    {
        std::vector<std::string> attrlist;
        എങ്കിൽ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( താക്കോൽ, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = താക്കോൽ ? std::string(താക്കോൽ) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        തിരികെ current_struct;
    }

    ശൂന്യം endWriteStruct(സ്ഥിരം FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ value)
    {
        അക്ഷരം buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( താക്കോൽ, ptr);
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, int64_t value)
    {
        അക്ഷരം buf[128], *ptr = fs::itoa( value, buf, 10, സത്യം );
        writeScalar( താക്കോൽ, ptr);
    }

    ശൂന്യം ലിഖ( സ്ഥിരം അക്ഷരം* താക്കോൽ, ഇരട്ട value )
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::doubleToString( buf, വലുപ്പം(buf), value, അസത്യം ) );
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* str, തർക്കം quote)
    {
        അക്ഷരം buf[CV_FS_MAX_LEN*6+16];
        അക്ഷരം* data = (അക്ഷരം*)str;
        പൂർണ്ണസംഖ്യ i, len;

        എങ്കിൽ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (പൂർണ്ണസംഖ്യ)strlen(str);
        എങ്കിൽ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        എങ്കിൽ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            തർക്കം need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            വേണ്ടി( i = 0; i < len; i++ )
            {
                അക്ഷരം c = str[i];

                എങ്കിൽ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = സത്യം;
                }
                അല്ലെങ്കിൽ എങ്കിൽ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    എങ്കിൽ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    അല്ലെങ്കിൽ
                    {
                        snprintf( data, വലുപ്പം(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                അല്ലെങ്കിൽ
                    *data++ = c;
            }
            എങ്കിൽ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = സത്യം;

            എങ്കിൽ( need_quote )
                *data++ = '\"';
            len = (പൂർണ്ണസംഖ്യ)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (പൂർണ്ണസംഖ്യ)!need_quote;
        }

        writeScalar( താക്കോൽ, data );
    }

    ശൂന്യം writeScalar(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* data)
    {
        fs->check_if_write_struct_is_delayed(അസത്യം);
        എങ്കിൽ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        അല്ലെങ്കിൽ എങ്കിൽ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)strlen(data);
        എങ്കിൽ( താക്കോൽ && *താക്കോൽ == '\0' )
            താക്കോൽ = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;

        എങ്കിൽ( File_കണ്ണി::isMap(struct_flags) ||
           (!File_കണ്ണി::isCollection(struct_flags) && താക്കോൽ) )
        {
            writeTag( താക്കോൽ, CV_XML_OPENING_TAG );
            അക്ഷരം* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( താക്കോൽ, CV_XML_CLOSING_TAG );
        }
        അല്ലെങ്കിൽ
        {
            അക്ഷരം* ptr = fs->bufferPtr();
            പൂർണ്ണസംഖ്യ new_offset = (പൂർണ്ണസംഖ്യ)(ptr - fs->bufferStart()) + len;

            എങ്കിൽ( താക്കോൽ )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_കണ്ണി::SEQ;

            എങ്കിൽ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            അല്ലെങ്കിൽ എങ്കിൽ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    ശൂന്യം writeComment(സ്ഥിരം അക്ഷരം* comment, തർക്കം eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        പൂർണ്ണസംഖ്യ len;
        പൂർണ്ണസംഖ്യ multiline;
        സ്ഥിരം അക്ഷരം* eol;
        അക്ഷരം* ptr;

        എങ്കിൽ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        എങ്കിൽ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (പൂർണ്ണസംഖ്യ)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        എങ്കിൽ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        അല്ലെങ്കിൽ എങ്കിൽ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        എങ്കിൽ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (പൂർണ്ണസംഖ്യ)strlen(ptr);
        }
        അല്ലെങ്കിൽ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        എങ്കിൽ( multiline )
        {
            ആകുമ്പോൾ( comment )
            {
                എങ്കിൽ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (പൂർണ്ണസംഖ്യ)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                അല്ലെങ്കിൽ
                {
                    len = (പൂർണ്ണസംഖ്യ)strlen(comment);
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

    ശൂന്യം startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

സംരക്ഷിത:
    FileStorage_API* fs;
};

വർഗ്ഗം XMLParser : പൊതു FileStorageParser
{
പൊതു:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    മായാ ~XMLParser() {}

    അക്ഷരം* skipSpaces( അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ mode )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        പൂർണ്ണസംഖ്യ level = 0;

        വേണ്ടി(;;)
        {
            അക്ഷരം c;
            ptr--;

            എങ്കിൽ( mode == CV_XML_INSIDE_COMMENT )
            {
                ചെയ്യുക c = *++ptr;
                ആകുമ്പോൾ( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                എങ്കിൽ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                ചെയ്യുക
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    എങ്കിൽ( level < 0 )
                        തിരികെ ptr;
                } ആകുമ്പോൾ( cv_isprint_or_tab(c) );
            }
            അല്ലെങ്കിൽ
            {
                ചെയ്യുക c = *++ptr;
                ആകുമ്പോൾ( c == ' ' || c == '\t' );

                എങ്കിൽ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    എങ്കിൽ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                അല്ലെങ്കിൽ എങ്കിൽ( cv_isprint(c) )
                    നിർത്തുക;
            }

            എങ്കിൽ( !cv_isprint(*ptr) )
            {
                എങ്കിൽ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                എങ്കിൽ( !ptr || *ptr == '\0' )
                    നിർത്തുക;
            }
        }
        തിരികെ ptr;
    }

    തർക്കം getBase64Row(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ /*indent*/, അക്ഷരം* &beg, അക്ഷരം* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        എങ്കിൽ( !ptr || !*ptr )
            തിരികെ അസത്യം;

        // closing XML tag
        എങ്കിൽ ( *beg == '<' )
            തിരികെ അസത്യം;

        // find end of the row
        ആകുമ്പോൾ( cv_isprint(*ptr) )
            ++ptr;
        എങ്കിൽ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        തിരികെ സത്യം;
    }

    അക്ഷരം* parse_മൂല്യം( അക്ഷരം* ptr, File_കണ്ണി& കണ്ണി, പൂർണ്ണസംഖ്യ depth = 0 )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        എങ്കിൽ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_കണ്ണി new_elem;
        തർക്കം have_space = സത്യം;
        പൂർണ്ണസംഖ്യ value_type = കണ്ണി.type();
        std::string താക്കോൽ, key2, type_name;

        വേണ്ടി(;;)
        {
            അക്ഷരം c = *ptr, d;
            അക്ഷരം* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            എങ്കിൽ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                എങ്കിൽ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = സത്യം;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            എങ്കിൽ( c =='<' || c == '\0' )
            {
                പൂർണ്ണസംഖ്യ tag_type = 0;
                പൂർണ്ണസംഖ്യ elem_type = File_കണ്ണി::NONE;

                എങ്കിൽ( d == '/' || c == '\0' )
                    നിർത്തുക;

                ptr = parseTag( ptr, താക്കോൽ, type_name, tag_type );

                എങ്കിൽ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                എങ്കിൽ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                തർക്കം ദ്വിമാന_string = അസത്യം;

                എങ്കിൽ( !type_name.empty() )
                {
                    സ്ഥിരം അക്ഷരം* tn = type_name.c_str();
                    എങ്കിൽ( strcmp(tn, "str") == 0 )
                        elem_type = File_കണ്ണി::STRING;
                    അല്ലെങ്കിൽ എങ്കിൽ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_കണ്ണി::MAP;
                    അല്ലെങ്കിൽ എങ്കിൽ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_കണ്ണി::SEQ;
                    അല്ലെങ്കിൽ എങ്കിൽ( strcmp( tn, "binary") == 0)
                        ദ്വിമാന_string = സത്യം;
                }

                new_elem = fs->add_കണ്ണി(കണ്ണി, താക്കോൽ, elem_type, 0);
                എങ്കിൽ (!ദ്വിമാന_string)
                    ptr = parse_മൂല്യം(ptr, new_elem, depth + 1);
                അല്ലെങ്കിൽ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    എങ്കിൽ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                എങ്കിൽ( tag_type != CV_XML_CLOSING_TAG || key2 != താക്കോൽ )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = സത്യം;
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_കണ്ണി* elem = &കണ്ണി;
                എങ്കിൽ( കണ്ണി.type() != File_കണ്ണി::NONE )
                {
                    fs->convertToCollection( File_കണ്ണി::SEQ, കണ്ണി );
                    new_elem = fs->add_കണ്ണി(കണ്ണി, std::string(), File_കണ്ണി::NONE, 0);
                    elem = &new_elem;
                }

                എങ്കിൽ( value_type != File_കണ്ണി::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ആകുമ്പോൾ( cv_isdigit(*endptr) )
                        endptr++;
                    എങ്കിൽ( *endptr == '.' || *endptr == 'e' )
                    {
                        ഇരട്ട fval = fs->strtod( ptr, &endptr );
                        elem->set_മൂല്യം(File_കണ്ണി::REAL, &fval);
                    }
                    അല്ലെങ്കിൽ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_മൂല്യം(File_കണ്ണി::INT, &ival);
                    }

                    എങ്കിൽ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                അല്ലെങ്കിൽ
                {
                    // string
                    പൂർണ്ണസംഖ്യ i = 0, len, is_quoted = 0;
                    എങ്കിൽ( c == '\"' )
                        is_quoted = 1;
                    അല്ലെങ്കിൽ
                        --ptr;
                    strbuf[0] = '\0';

                    വേണ്ടി( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        എങ്കിൽ( !cv_isalnum(c) )
                        {
                            എങ്കിൽ( c == '\"' )
                            {
                                എങ്കിൽ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                നിർത്തുക;
                            }
                            അല്ലെങ്കിൽ എങ്കിൽ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                എങ്കിൽ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                നിർത്തുക;
                            }
                            അല്ലെങ്കിൽ എങ്കിൽ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            അല്ലെങ്കിൽ എങ്കിൽ( c == '&' )
                            {
                                എങ്കിൽ( *++ptr == '#' )
                                {
                                    പൂർണ്ണസംഖ്യ val, base = 10;
                                    ptr++;
                                    എങ്കിൽ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (പൂർണ്ണസംഖ്യ)strtol( ptr, &endptr, base );
                                    എങ്കിൽ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (അക്ഷരം)val;
                                }
                                അല്ലെങ്കിൽ
                                {
                                    endptr = ptr;
                                    ചെയ്യുക c = *++endptr;
                                    ആകുമ്പോൾ( cv_isalnum(c) );
                                    എങ്കിൽ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (പൂർണ്ണസംഖ്യ)(endptr - ptr);
                                    എങ്കിൽ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    അല്ലെങ്കിൽ എങ്കിൽ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    അല്ലെങ്കിൽ എങ്കിൽ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    അല്ലെങ്കിൽ എങ്കിൽ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    അല്ലെങ്കിൽ എങ്കിൽ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    അല്ലെങ്കിൽ
                                    {
                                        എങ്കിൽ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        എങ്കിൽ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_മൂല്യം(File_കണ്ണി::STRING, strbuf, i);
                }

                എങ്കിൽ( value_type != File_കണ്ണി::NONE && value_type != File_കണ്ണി::SEQ && value_type != File_കണ്ണി::MAP )
                    നിർത്തുക;
                have_space = അസത്യം;
            }
        }
        fs->finalizeCollection(കണ്ണി);

        തിരികെ ptr;
    }

    അക്ഷരം* parseTag( അക്ഷരം* ptr, std::string& tag_name,
                    std::string& type_name, പൂർണ്ണസംഖ്യ& tag_type )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        എങ്കിൽ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        എങ്കിൽ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        എങ്കിൽ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        അല്ലെങ്കിൽ എങ്കിൽ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        അല്ലെങ്കിൽ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        വേണ്ടി(;;)
        {
            അക്ഷരം c, *endptr;
            എങ്കിൽ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            ചെയ്യുക c = *++endptr;
            ആകുമ്പോൾ( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            എങ്കിൽ( tag_name.empty() )
                tag_name = attrname;
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                എങ്കിൽ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    എങ്കിൽ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    എങ്കിൽ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                എങ്കിൽ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    എങ്കിൽ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    എങ്കിൽ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                അക്ഷരം quote = *ptr++;
                endptr = ptr;
                വേണ്ടി(;;)
                {
                    c = *endptr++;
                    എങ്കിൽ( c == quote )
                        നിർത്തുക;
                    എങ്കിൽ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                എങ്കിൽ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            തർക്കം have_space = cv_isspace(c) || c == '\0';

            എങ്കിൽ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                എങ്കിൽ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            എങ്കിൽ( c == '>' )
            {
                എങ്കിൽ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                നിർത്തുക;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                എങ്കിൽ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                നിർത്തുക;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                നിർത്തുക;
            }

            എങ്കിൽ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        തിരികെ ptr;
    }

    തർക്കം parse(അക്ഷരം* ptr)
    {
        CV_Assert( fs != 0 );

        std::string താക്കോൽ, key2, type_name;
        പൂർണ്ണസംഖ്യ tag_type = 0;
        തർക്കം ok = അസത്യം;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        എങ്കിൽ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, താക്കോൽ, type_name, tag_type );
        File_കണ്ണി വേര്_collection(fs->getFS(), 0, 0);

        ആകുമ്പോൾ( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            എങ്കിൽ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            എങ്കിൽ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, താക്കോൽ, type_name, tag_type );
                എങ്കിൽ( tag_type != CV_XML_OPENING_TAG || താക്കോൽ != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_കണ്ണി വേര് = fs->add_കണ്ണി(വേര്_collection, std::string(), File_കണ്ണി::MAP, 0);
                ptr = parse_മൂല്യം( ptr, വേര് );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                എങ്കിൽ( tag_type != CV_XML_CLOSING_TAG || താക്കോൽ != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = സത്യം;
            }
        }
        CV_Assert( fs->eof() );
        തിരികെ ok;
    }

    FileStorage_API* fs;
    അക്ഷരം strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    തിരികെ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    തിരികെ makePtr<XMLParser>(fs);
}

}
