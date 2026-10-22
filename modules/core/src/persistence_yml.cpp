// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence.hpp"

എണ്ണൽ
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

നാമാകാശ cv
{

വർഗ്ഗം YAMLEmitter : പൊതു FileStorageEmitter
{
പൊതു:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    മായാ ~YAMLEmitter() {}

    FStructData startWriteStruct(സ്ഥിരം FStructData& parent, സ്ഥിരം അക്ഷരം* താക്കോൽ,
                                 പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം* type_name=0)
    {
        അക്ഷരം buf[CV_FS_MAX_LEN + 1024];
        സ്ഥിരം അക്ഷരം* data = 0;

        എങ്കിൽ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_കണ്ണി::TYPE_MASK|File_കണ്ണി::FLOW)) | File_കണ്ണി::EMPTY;
        എങ്കിൽ( !File_കണ്ണി::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        എങ്കിൽ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_കണ്ണി::SEQ;
            snprintf(buf, വലുപ്പം(buf), "!!binary |");
            data = buf;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( File_കണ്ണി::isFlow(struct_flags))
        {
            അക്ഷരം c = File_കണ്ണി::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_കണ്ണി::FLOW;

            എങ്കിൽ( type_name )
                snprintf( buf, വലുപ്പം(buf), "!!%s %c", type_name, c );
            അല്ലെങ്കിൽ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( type_name )
        {
            snprintf( buf, വലുപ്പം(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( താക്കോൽ, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        എങ്കിൽ( !File_കണ്ണി::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_കണ്ണി::isFlow(struct_flags);

        തിരികെ fsd;
    }

    ശൂന്യം endWriteStruct(സ്ഥിരം FStructData& current_struct)
    {
        അക്ഷരം* ptr;

        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;

        എങ്കിൽ( File_കണ്ണി::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            എങ്കിൽ( ptr > fs->bufferStart() + current_struct.indent && !File_കണ്ണി::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_കണ്ണി::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( File_കണ്ണി::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_കണ്ണി::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ value)
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::itoa( value, buf, 10 ));
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, int64_t value)
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::itoa( value, buf, 10, സത്യം ));
    }

    ശൂന്യം ലിഖ( സ്ഥിരം അക്ഷരം* താക്കോൽ, ഇരട്ട value )
    {
        അക്ഷരം buf[128];
        writeScalar( താക്കോൽ, fs::doubleToString( buf, വലുപ്പം(buf), value, അസത്യം ));
    }

    ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* str, തർക്കം quote)
    {
        അക്ഷരം buf[CV_FS_MAX_LEN*4+16];
        അക്ഷരം* data = (അക്ഷരം*)str;
        പൂർണ്ണസംഖ്യ i, len;

        എങ്കിൽ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (പൂർണ്ണസംഖ്യ)strlen(str);
        എങ്കിൽ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        എങ്കിൽ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            പൂർണ്ണസംഖ്യ need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            വേണ്ടി( i = 0; i < len; i++ )
            {
                അക്ഷരം c = str[i];

                എങ്കിൽ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                എങ്കിൽ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    എങ്കിൽ( cv_isprint(c) )
                        *data++ = c;
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\n' )
                        *data++ = 'n';
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\r' )
                        *data++ = 'r';
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\t' )
                        *data++ = 't';
                    അല്ലെങ്കിൽ
                    {
                        snprintf( data, വലുപ്പം(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                അല്ലെങ്കിൽ
                    *data++ = c;
            }
            എങ്കിൽ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            എങ്കിൽ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( താക്കോൽ, data);
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

        പൂർണ്ണസംഖ്യ i, keylen = 0;
        പൂർണ്ണസംഖ്യ datalen = 0;
        അക്ഷരം* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        പൂർണ്ണസംഖ്യ struct_flags = current_struct.flags;

        എങ്കിൽ( താക്കോൽ && താക്കോൽ[0] == '\0' )
            താക്കോൽ = 0;

        എങ്കിൽ( File_കണ്ണി::isCollection(struct_flags) )
        {
            എങ്കിൽ( (File_കണ്ണി::isMap(struct_flags) ^ (താക്കോൽ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        അല്ലെങ്കിൽ
        {
            fs->setNonEmpty();
            struct_flags = File_കണ്ണി::EMPTY | (താക്കോൽ ? File_കണ്ണി::MAP : File_കണ്ണി::SEQ);
        }

        എങ്കിൽ( താക്കോൽ )
        {
            keylen = (പൂർണ്ണസംഖ്യ)strlen(താക്കോൽ);
            എങ്കിൽ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            എങ്കിൽ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        എങ്കിൽ( data )
            datalen = (പൂർണ്ണസംഖ്യ)strlen(data);

        എങ്കിൽ( File_കണ്ണി::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            എങ്കിൽ( !File_കണ്ണി::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            പൂർണ്ണസംഖ്യ new_offset = (പൂർണ്ണസംഖ്യ)(ptr - fs->bufferStart()) + keylen + datalen;
            എങ്കിൽ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            അല്ലെങ്കിൽ
                *ptr++ = ' ';
        }
        അല്ലെങ്കിൽ
        {
            ptr = fs->flush();
            എങ്കിൽ( !File_കണ്ണി::isMap(struct_flags) )
            {
                *ptr++ = '-';
                എങ്കിൽ( data )
                    *ptr++ = ' ';
            }
        }

        എങ്കിൽ( താക്കോൽ )
        {
            എങ്കിൽ( !cv_isalpha(താക്കോൽ[0]) && താക്കോൽ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            വേണ്ടി( i = 0; i < keylen; i++ )
            {
                അക്ഷരം c = താക്കോൽ[i];

                ptr[i] = c;
                എങ്കിൽ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            എങ്കിൽ( !File_കണ്ണി::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        എങ്കിൽ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_കണ്ണി::EMPTY;
    }

    ശൂന്യം writeComment(സ്ഥിരം അക്ഷരം* comment, തർക്കം eol_comment)
    {
        എങ്കിൽ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)strlen(comment);
        സ്ഥിരം അക്ഷരം* eol = strchr(comment, '\n');
        തർക്കം multiline = eol != 0;
        അക്ഷരം* ptr = fs->bufferPtr();

        എങ്കിൽ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        അല്ലെങ്കിൽ
            *ptr++ = ' ';

        ആകുമ്പോൾ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            എങ്കിൽ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (പൂർണ്ണസംഖ്യ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            അല്ലെങ്കിൽ
            {
                len = (പൂർണ്ണസംഖ്യ)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ശൂന്യം startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

സംരക്ഷിത:
    FileStorage_API* fs;
};


വർഗ്ഗം YAMLParser : പൊതു FileStorageParser
{
പൊതു:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    മായാ ~YAMLParser() {}

    അക്ഷരം* skipSpaces( അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ min_indent, പൂർണ്ണസംഖ്യ max_comment_indent )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        വേണ്ടി(;;)
        {
            ആകുമ്പോൾ( *ptr == ' ' )
                ptr++;
            എങ്കിൽ( *ptr == '#' )
            {
                എങ്കിൽ( ptr - fs->bufferStart() > max_comment_indent )
                    തിരികെ ptr;
                *ptr = '\0';
            }
            അല്ലെങ്കിൽ എങ്കിൽ( cv_isprint(*ptr) )
            {
                എങ്കിൽ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                നിർത്തുക;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                എങ്കിൽ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    നിർത്തുക;
                }
                അല്ലെങ്കിൽ
                {
                    പൂർണ്ണസംഖ്യ l = (പൂർണ്ണസംഖ്യ)strlen(ptr);
                    എങ്കിൽ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            അല്ലെങ്കിൽ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        തിരികെ ptr;
    }

    തർക്കം getBase64Row(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ indent, അക്ഷരം* &beg, അക്ഷരം* &end)
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        എങ്കിൽ (!ptr || !*ptr)
            തിരികെ അസത്യം; // end of file

        എങ്കിൽ (ptr - fs->bufferStart() != indent)
            തിരികെ അസത്യം; // end of base64 data

        /* find end */
        ആകുമ്പോൾ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        എങ്കിൽ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        തിരികെ സത്യം;
    }


    അക്ഷരം* parse_താക്കോൽ( അക്ഷരം* ptr, File_കണ്ണി& map_കണ്ണി, File_കണ്ണി& മൂല്യം_placeholder )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        അക്ഷരം c;
        അക്ഷരം *endptr = ptr - 1, *saveptr;

        എങ്കിൽ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        ചെയ്യുക c = *++endptr;
        ആകുമ്പോൾ( cv_isprint(c) && c != ':' );

        എങ്കിൽ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        എങ്കിൽ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        ചെയ്യുക c = *--endptr;
        ആകുമ്പോൾ( c == ' ' );

        ++endptr;

        മൂല്യം_placeholder = fs->add_കണ്ണി(map_കണ്ണി, std::string(ptr, endptr - ptr), File_കണ്ണി::NONE);
        ptr = saveptr;

        തിരികെ ptr;
    }

    അക്ഷരം* parse_മൂല്യം( അക്ഷരം* ptr, File_കണ്ണി& കണ്ണി, പൂർണ്ണസംഖ്യ min_indent, തർക്കം is_parent_flow, പൂർണ്ണസംഖ്യ depth = 0 )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        എങ്കിൽ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        അക്ഷരം* endptr = 0;
        അക്ഷരം c = ptr[0], d = ptr[1];
        പൂർണ്ണസംഖ്യ value_type = File_കണ്ണി::NONE;
        പൂർണ്ണസംഖ്യ len;
        തർക്കം is_ദ്വിമാന_string = അസത്യം;
        തർക്കം is_user = അസത്യം;

        എങ്കിൽ( c == '!' ) // handle explicit type specification
        {
            എങ്കിൽ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = സത്യം;
                //value_type |= FileNode::USER;
            }
            എങ്കിൽ ( d == '<') //support of full type heading from YAML 1.2
            {
                സ്ഥിരം അക്ഷരം* yamlTypeHeading = "<tag:yaml.org,2002:";
                സ്ഥിരം size_t headingLength = strlen(yamlTypeHeading);

                അക്ഷരം* typeEndPtr = ++ptr;

                ചെയ്യുക d = *++typeEndPtr;
                ആകുമ്പോൾ( cv_isprint(d) && d != ' ' && d != '>' );

                എങ്കിൽ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    എങ്കിൽ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = സത്യം;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            ചെയ്യുക d = *++endptr;
            ആകുമ്പോൾ( cv_isprint(d) && d != ' ' );
            len = (പൂർണ്ണസംഖ്യ)(endptr - ptr);
            എങ്കിൽ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            എങ്കിൽ( len == 3 && !is_user )
            {
                എങ്കിൽ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_കണ്ണി::STRING;
                അല്ലെങ്കിൽ എങ്കിൽ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_കണ്ണി::INT;
                അല്ലെങ്കിൽ എങ്കിൽ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_കണ്ണി::SEQ;
                അല്ലെങ്കിൽ എങ്കിൽ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_കണ്ണി::MAP;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( len == 5 && !is_user )
            {
                എങ്കിൽ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_കണ്ണി::REAL;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (len == 6 && is_user)
            {
                എങ്കിൽ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_കണ്ണി::SEQ;
                    is_ദ്വിമാന_string = സത്യം;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    ചെയ്യുക {
                        d = *++endptr;
                        എങ്കിൽ (d == '|')
                            നിർത്തുക;
                    } ആകുമ്പോൾ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            എങ്കിൽ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            എങ്കിൽ( !is_user )
            {
                എങ്കിൽ (value_type == File_കണ്ണി::STRING && c != '\'' && c != '\"')
                    പോകുക force_string;
                എങ്കിൽ( value_type == File_കണ്ണി::INT )
                    പോകുക force_int;
                എങ്കിൽ( value_type == File_കണ്ണി::REAL )
                    പോകുക force_real;
            }
        }

        എങ്കിൽ (is_ദ്വിമാന_string)
        {
            പൂർണ്ണസംഖ്യ indent = static_cast<പൂർണ്ണസംഖ്യ>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, കണ്ണി);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ആകുമ്പോൾ( cv_isdigit(*endptr) )
                endptr++;
            എങ്കിൽ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ഇരട്ട fval = fs->strtod( ptr, &endptr );
                കണ്ണി.set_മൂല്യം(File_കണ്ണി::REAL, &fval);
            }
            അല്ലെങ്കിൽ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                കണ്ണി.set_മൂല്യം(File_കണ്ണി::INT, &ival);
            }

            എങ്കിൽ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        അല്ലെങ്കിൽ എങ്കിൽ( c == '\'' || c == '\"' ) // an explicit string
        {
            എങ്കിൽ( c == '\'' )
                വേണ്ടി( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    എങ്കിൽ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\'' )
                    {
                        c = *++ptr;
                        എങ്കിൽ( c != '\'' )
                            നിർത്തുക;
                        buf[len++] = c;
                    }
                    അല്ലെങ്കിൽ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            അല്ലെങ്കിൽ
                വേണ്ടി( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    എങ്കിൽ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\"' )
                    {
                        ++ptr;
                        നിർത്തുക;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( c == '\\' )
                    {
                        d = *++ptr;
                        എങ്കിൽ( d == '\'' )
                            buf[len++] = d;
                        അല്ലെങ്കിൽ എങ്കിൽ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        അല്ലെങ്കിൽ എങ്കിൽ( d == 'n' )
                            buf[len++] = '\n';
                        അല്ലെങ്കിൽ എങ്കിൽ( d == 'r' )
                            buf[len++] = '\r';
                        അല്ലെങ്കിൽ എങ്കിൽ( d == 't' )
                            buf[len++] = '\t';
                        അല്ലെങ്കിൽ എങ്കിൽ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            പൂർണ്ണസംഖ്യ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (പൂർണ്ണസംഖ്യ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            എങ്കിൽ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            അല്ലെങ്കിൽ
                            {
                                buf[len++] = (അക്ഷരം)val;
                                ptr = endptr;
                            }
                        }
                    }
                    അല്ലെങ്കിൽ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            എങ്കിൽ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            കണ്ണി.set_മൂല്യം(File_കണ്ണി::STRING, buf, len);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( c == '[' || c == '{' ) // collection as a flow
        {
            പൂർണ്ണസംഖ്യ new_min_indent = min_indent + !is_parent_flow;
            പൂർണ്ണസംഖ്യ struct_type = c == '{' ? File_കണ്ണി::MAP : File_കണ്ണി::SEQ;
            പൂർണ്ണസംഖ്യ nelems = 0;

            fs->convertToCollection(struct_type, കണ്ണി);
            d = c == '[' ? ']' : '}';

            വേണ്ടി( ++ptr ;; nelems++ )
            {
                File_കണ്ണി elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                എങ്കിൽ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                എങ്കിൽ( *ptr == '}' || *ptr == ']' )
                {
                    എങ്കിൽ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    നിർത്തുക;
                }

                എങ്കിൽ( nelems != 0 )
                {
                    എങ്കിൽ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    എങ്കിൽ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                എങ്കിൽ( struct_type == File_കണ്ണി::MAP )
                {
                    ptr = parse_താക്കോൽ( ptr, കണ്ണി, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                അല്ലെങ്കിൽ
                {
                    എങ്കിൽ( *ptr == ']' )
                        നിർത്തുക;
                    elem = fs->add_കണ്ണി(കണ്ണി, std::string(), File_കണ്ണി::NONE);
                }
                ptr = parse_മൂല്യം( ptr, elem, new_min_indent, സത്യം, depth + 1 );
            }
            fs->finalizeCollection(കണ്ണി);
        }
        അല്ലെങ്കിൽ
        {
            പൂർണ്ണസംഖ്യ indent, struct_type;

            എങ്കിൽ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                എങ്കിൽ( !is_parent_flow )
                {
                    എങ്കിൽ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    എങ്കിൽ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                ചെയ്യുക c = *++endptr;
                ആകുമ്പോൾ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_കണ്ണി::STRING));

                എങ്കിൽ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                എങ്കിൽ( is_parent_flow || c != ':' )
                {
                    അക്ഷരം* str_end = endptr;
                    ചെയ്യുക c = *--str_end;
                    ആകുമ്പോൾ( str_end > ptr && c == ' ' );
                    str_end++;
                    എങ്കിൽ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        എങ്കിൽ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            കണ്ണി.set_മൂല്യം(File_കണ്ണി::INT, &ival);
                            ptr = endptr;
                            തിരികെ ptr;
                        }
                        അല്ലെങ്കിൽ എങ്കിൽ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            കണ്ണി.set_മൂല്യം(File_കണ്ണി::INT, &ival);
                            ptr = endptr;
                            തിരികെ ptr;
                        }
                        അല്ലെങ്കിൽ എങ്കിൽ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            തിരികെ ptr;
                        }
                    }

                    കണ്ണി.set_മൂല്യം(File_കണ്ണി::STRING, ptr, (പൂർണ്ണസംഖ്യ)(str_end - ptr));
                    ptr = endptr;
                    തിരികെ ptr;
                }
                struct_type = File_കണ്ണി::MAP;
            }
            അല്ലെങ്കിൽ
                struct_type = File_കണ്ണി::SEQ;

            fs->convertToCollection( struct_type, കണ്ണി );
            indent = (പൂർണ്ണസംഖ്യ)(ptr - fs->bufferStart());

            വേണ്ടി(;;)
            {
                File_കണ്ണി elem;

                എങ്കിൽ( struct_type == File_കണ്ണി::MAP )
                {
                    ptr = parse_താക്കോൽ( ptr, കണ്ണി, elem );
                }
                അല്ലെങ്കിൽ
                {
                    c = *ptr++;
                    എങ്കിൽ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_കണ്ണി(കണ്ണി, std::string(), File_കണ്ണി::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_മൂല്യം( ptr, elem, indent + 1, അസത്യം, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                എങ്കിൽ( ptr - fs->bufferStart() != indent )
                {
                    എങ്കിൽ( ptr - fs->bufferStart() < indent )
                        നിർത്തുക;
                    അല്ലെങ്കിൽ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                എങ്കിൽ( memcmp( ptr, "...", 3 ) == 0 )
                    നിർത്തുക;
            }
            fs->finalizeCollection(കണ്ണി);
        }

        തിരികെ ptr;
    }

    തർക്കം parse( അക്ഷരം* ptr )
    {
        എങ്കിൽ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        തർക്കം first = സത്യം;
        തർക്കം ok = സത്യം;
        File_കണ്ണി വേര്_collection(fs->getFS(), 0, 0);

        വേണ്ടി(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            വേണ്ടി(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                എങ്കിൽ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    നിർത്തുക;
                }

                എങ്കിൽ( *ptr == '%' )
                {
                    എങ്കിൽ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        എങ്കിൽ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                അല്ലെങ്കിൽ എങ്കിൽ( *ptr == '-' )
                {
                    എങ്കിൽ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        നിർത്തുക;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( first )
                        നിർത്തുക;
                }
                അല്ലെങ്കിൽ എങ്കിൽ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    എങ്കിൽ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    നിർത്തുക;
                }
                അല്ലെങ്കിൽ എങ്കിൽ( fs->eof() )
                    നിർത്തുക;
                അല്ലെങ്കിൽ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            എങ്കിൽ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            എങ്കിൽ( !ptr || !ptr[0] )
                നിർത്തുക;
            എങ്കിൽ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_കണ്ണി വേര്_കണ്ണി = fs->add_കണ്ണി(വേര്_collection, std::string(), File_കണ്ണി::NONE);

                ptr = parse_മൂല്യം( ptr, വേര്_കണ്ണി, 0, അസത്യം );
                എങ്കിൽ( !വേര്_കണ്ണി.isMap() && !വേര്_കണ്ണി.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                എങ്കിൽ( !ptr )
                    നിർത്തുക;
            }

            എങ്കിൽ( fs->eof() )
                നിർത്തുക;
            ptr += 3;
            first = അസത്യം;
        }

        തിരികെ ok;
    }

    FileStorage_API* fs;
    അക്ഷരം buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    തിരികെ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    തിരികെ makePtr<YAMLParser>(fs);
}

}
