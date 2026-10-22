// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence.hpp"

ગણના
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

નામાકાશ cv
{

વર્ગ YAMLEmitter : જાહેર FileStorageEmitter
{
જાહેર:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    આભાસી ~YAMLEmitter() {}

    FStructData startWriteStruct(અચલ FStructData& parent, અચલ અક્ષર* ચાવી,
                                 પૂર્ણાંક struct_flags, અચલ અક્ષર* type_name=0)
    {
        અક્ષર buf[CV_FS_MAX_LEN + 1024];
        અચલ અક્ષર* data = 0;

        જો ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ગાંઠ::TYPE_MASK|File_ગાંઠ::FLOW)) | File_ગાંઠ::EMPTY;
        જો( !File_ગાંઠ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        જો (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ગાંઠ::SEQ;
            snprintf(buf, કદ(buf), "!!binary |");
            data = buf;
        }
        નહીંતર જો( File_ગાંઠ::isFlow(struct_flags))
        {
            અક્ષર c = File_ગાંઠ::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ગાંઠ::FLOW;

            જો( type_name )
                snprintf( buf, કદ(buf), "!!%s %c", type_name, c );
            નહીંતર
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        નહીંતર જો( type_name )
        {
            snprintf( buf, કદ(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( ચાવી, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        જો( !File_ગાંઠ::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_ગાંઠ::isFlow(struct_flags);

        પાછા fsd;
    }

    શૂન્ય endWriteStruct(અચલ FStructData& current_struct)
    {
        અક્ષર* ptr;

        પૂર્ણાંક struct_flags = current_struct.flags;

        જો( File_ગાંઠ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            જો( ptr > fs->bufferStart() + current_struct.indent && !File_ગાંઠ::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ગાંઠ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        નહીંતર જો( File_ગાંઠ::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_ગાંઠ::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, પૂર્ણાંક value)
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::itoa( value, buf, 10 ));
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, int64_t value)
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::itoa( value, buf, 10, સાચું ));
    }

    શૂન્ય લિખ( અચલ અક્ષર* ચાવી, દ્વિગુણ value )
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::doubleToString( buf, કદ(buf), value, ખોટું ));
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, અચલ અક્ષર* str, તાર્કિક quote)
    {
        અક્ષર buf[CV_FS_MAX_LEN*4+16];
        અક્ષર* data = (અક્ષર*)str;
        પૂર્ણાંક i, len;

        જો( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (પૂર્ણાંક)strlen(str);
        જો( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        જો( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            પૂર્ણાંક need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            માટે( i = 0; i < len; i++ )
            {
                અક્ષર c = str[i];

                જો( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                જો( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    જો( cv_isprint(c) )
                        *data++ = c;
                    નહીંતર જો( c == '\n' )
                        *data++ = 'n';
                    નહીંતર જો( c == '\r' )
                        *data++ = 'r';
                    નહીંતર જો( c == '\t' )
                        *data++ = 't';
                    નહીંતર
                    {
                        snprintf( data, કદ(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                નહીંતર
                    *data++ = c;
            }
            જો( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            જો( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ચાવી, data);
    }

    શૂન્ય writeScalar(અચલ અક્ષર* ચાવી, અચલ અક્ષર* data)
    {
        fs->check_if_write_struct_is_delayed(ખોટું);
        જો ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        નહીંતર જો ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        પૂર્ણાંક i, keylen = 0;
        પૂર્ણાંક datalen = 0;
        અક્ષર* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        પૂર્ણાંક struct_flags = current_struct.flags;

        જો( ચાવી && ચાવી[0] == '\0' )
            ચાવી = 0;

        જો( File_ગાંઠ::isCollection(struct_flags) )
        {
            જો( (File_ગાંઠ::isMap(struct_flags) ^ (ચાવી != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        નહીંતર
        {
            fs->setNonEmpty();
            struct_flags = File_ગાંઠ::EMPTY | (ચાવી ? File_ગાંઠ::MAP : File_ગાંઠ::SEQ);
        }

        જો( ચાવી )
        {
            keylen = (પૂર્ણાંક)strlen(ચાવી);
            જો( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            જો( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        જો( data )
            datalen = (પૂર્ણાંક)strlen(data);

        જો( File_ગાંઠ::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            જો( !File_ગાંઠ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            પૂર્ણાંક new_offset = (પૂર્ણાંક)(ptr - fs->bufferStart()) + keylen + datalen;
            જો( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            નહીંતર
                *ptr++ = ' ';
        }
        નહીંતર
        {
            ptr = fs->flush();
            જો( !File_ગાંઠ::isMap(struct_flags) )
            {
                *ptr++ = '-';
                જો( data )
                    *ptr++ = ' ';
            }
        }

        જો( ચાવી )
        {
            જો( !cv_isalpha(ચાવી[0]) && ચાવી[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            માટે( i = 0; i < keylen; i++ )
            {
                અક્ષર c = ચાવી[i];

                ptr[i] = c;
                જો( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            જો( !File_ગાંઠ::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        જો( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ગાંઠ::EMPTY;
    }

    શૂન્ય writeComment(અચલ અક્ષર* comment, તાર્કિક eol_comment)
    {
        જો( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        પૂર્ણાંક len = (પૂર્ણાંક)strlen(comment);
        અચલ અક્ષર* eol = strchr(comment, '\n');
        તાર્કિક multiline = eol != 0;
        અક્ષર* ptr = fs->bufferPtr();

        જો( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        નહીંતર
            *ptr++ = ' ';

        જ્યારે( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            જો( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (પૂર્ણાંક)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            નહીંતર
            {
                len = (પૂર્ણાંક)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    શૂન્ય startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

સુરક્ષિત:
    FileStorage_API* fs;
};


વર્ગ YAMLParser : જાહેર FileStorageParser
{
જાહેર:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    આભાસી ~YAMLParser() {}

    અક્ષર* skipSpaces( અક્ષર* ptr, પૂર્ણાંક min_indent, પૂર્ણાંક max_comment_indent )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        માટે(;;)
        {
            જ્યારે( *ptr == ' ' )
                ptr++;
            જો( *ptr == '#' )
            {
                જો( ptr - fs->bufferStart() > max_comment_indent )
                    પાછા ptr;
                *ptr = '\0';
            }
            નહીંતર જો( cv_isprint(*ptr) )
            {
                જો( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                તોડો;
            }
            નહીંતર જો( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                જો( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    તોડો;
                }
                નહીંતર
                {
                    પૂર્ણાંક l = (પૂર્ણાંક)strlen(ptr);
                    જો( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            નહીંતર
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        પાછા ptr;
    }

    તાર્કિક getBase64Row(અક્ષર* ptr, પૂર્ણાંક indent, અક્ષર* &beg, અક્ષર* &end)
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        જો (!ptr || !*ptr)
            પાછા ખોટું; // end of file

        જો (ptr - fs->bufferStart() != indent)
            પાછા ખોટું; // end of base64 data

        /* find end */
        જ્યારે(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        જો (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        પાછા સાચું;
    }


    અક્ષર* parse_ચાવી( અક્ષર* ptr, File_ગાંઠ& map_ગાંઠ, File_ગાંઠ& કિંમત_placeholder )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        અક્ષર c;
        અક્ષર *endptr = ptr - 1, *saveptr;

        જો( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        કરો c = *++endptr;
        જ્યારે( cv_isprint(c) && c != ':' );

        જો( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        જો( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        કરો c = *--endptr;
        જ્યારે( c == ' ' );

        ++endptr;

        કિંમત_placeholder = fs->add_ગાંઠ(map_ગાંઠ, std::string(ptr, endptr - ptr), File_ગાંઠ::NONE);
        ptr = saveptr;

        પાછા ptr;
    }

    અક્ષર* parse_કિંમત( અક્ષર* ptr, File_ગાંઠ& ગાંઠ, પૂર્ણાંક min_indent, તાર્કિક is_parent_flow, પૂર્ણાંક depth = 0 )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        જો( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        અક્ષર* endptr = 0;
        અક્ષર c = ptr[0], d = ptr[1];
        પૂર્ણાંક value_type = File_ગાંઠ::NONE;
        પૂર્ણાંક len;
        તાર્કિક is_દ્વિઅંકી_string = ખોટું;
        તાર્કિક is_user = ખોટું;

        જો( c == '!' ) // handle explicit type specification
        {
            જો( d == '!' || d == '^' )
            {
                ptr++;
                is_user = સાચું;
                //value_type |= FileNode::USER;
            }
            જો ( d == '<') //support of full type heading from YAML 1.2
            {
                અચલ અક્ષર* yamlTypeHeading = "<tag:yaml.org,2002:";
                અચલ size_t headingLength = strlen(yamlTypeHeading);

                અક્ષર* typeEndPtr = ++ptr;

                કરો d = *++typeEndPtr;
                જ્યારે( cv_isprint(d) && d != ' ' && d != '>' );

                જો ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    જો ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = સાચું;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            કરો d = *++endptr;
            જ્યારે( cv_isprint(d) && d != ' ' );
            len = (પૂર્ણાંક)(endptr - ptr);
            જો( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            જો( len == 3 && !is_user )
            {
                જો( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ગાંઠ::STRING;
                નહીંતર જો( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ગાંઠ::INT;
                નહીંતર જો( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ગાંઠ::SEQ;
                નહીંતર જો( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ગાંઠ::MAP;
            }
            નહીંતર જો( len == 5 && !is_user )
            {
                જો( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ગાંઠ::REAL;
            }
            નહીંતર જો (len == 6 && is_user)
            {
                જો( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ગાંઠ::SEQ;
                    is_દ્વિઅંકી_string = સાચું;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    કરો {
                        d = *++endptr;
                        જો (d == '|')
                            તોડો;
                    } જ્યારે (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            જો (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            જો( !is_user )
            {
                જો (value_type == File_ગાંઠ::STRING && c != '\'' && c != '\"')
                    જાઓ force_string;
                જો( value_type == File_ગાંઠ::INT )
                    જાઓ force_int;
                જો( value_type == File_ગાંઠ::REAL )
                    જાઓ force_real;
            }
        }

        જો (is_દ્વિઅંકી_string)
        {
            પૂર્ણાંક indent = static_cast<પૂર્ણાંક>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, ગાંઠ);
        }
        નહીંતર જો( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            જ્યારે( cv_isdigit(*endptr) )
                endptr++;
            જો( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                દ્વિગુણ fval = fs->strtod( ptr, &endptr );
                ગાંઠ.set_કિંમત(File_ગાંઠ::REAL, &fval);
            }
            નહીંતર
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ગાંઠ.set_કિંમત(File_ગાંઠ::INT, &ival);
            }

            જો( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        નહીંતર જો( c == '\'' || c == '\"' ) // an explicit string
        {
            જો( c == '\'' )
                માટે( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    જો( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    નહીંતર જો( c == '\'' )
                    {
                        c = *++ptr;
                        જો( c != '\'' )
                            તોડો;
                        buf[len++] = c;
                    }
                    નહીંતર
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            નહીંતર
                માટે( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    જો( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    નહીંતર જો( c == '\"' )
                    {
                        ++ptr;
                        તોડો;
                    }
                    નહીંતર જો( c == '\\' )
                    {
                        d = *++ptr;
                        જો( d == '\'' )
                            buf[len++] = d;
                        નહીંતર જો( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        નહીંતર જો( d == 'n' )
                            buf[len++] = '\n';
                        નહીંતર જો( d == 'r' )
                            buf[len++] = '\r';
                        નહીંતર જો( d == 't' )
                            buf[len++] = '\t';
                        નહીંતર જો( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            પૂર્ણાંક val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (પૂર્ણાંક)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            જો( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            નહીંતર
                            {
                                buf[len++] = (અક્ષર)val;
                                ptr = endptr;
                            }
                        }
                    }
                    નહીંતર
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            જો( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ગાંઠ.set_કિંમત(File_ગાંઠ::STRING, buf, len);
        }
        નહીંતર જો( c == '[' || c == '{' ) // collection as a flow
        {
            પૂર્ણાંક new_min_indent = min_indent + !is_parent_flow;
            પૂર્ણાંક struct_type = c == '{' ? File_ગાંઠ::MAP : File_ગાંઠ::SEQ;
            પૂર્ણાંક nelems = 0;

            fs->convertToCollection(struct_type, ગાંઠ);
            d = c == '[' ? ']' : '}';

            માટે( ++ptr ;; nelems++ )
            {
                File_ગાંઠ elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                જો (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                જો( *ptr == '}' || *ptr == ']' )
                {
                    જો( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    તોડો;
                }

                જો( nelems != 0 )
                {
                    જો( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    જો (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                જો( struct_type == File_ગાંઠ::MAP )
                {
                    ptr = parse_ચાવી( ptr, ગાંઠ, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                નહીંતર
                {
                    જો( *ptr == ']' )
                        તોડો;
                    elem = fs->add_ગાંઠ(ગાંઠ, std::string(), File_ગાંઠ::NONE);
                }
                ptr = parse_કિંમત( ptr, elem, new_min_indent, સાચું, depth + 1 );
            }
            fs->finalizeCollection(ગાંઠ);
        }
        નહીંતર
        {
            પૂર્ણાંક indent, struct_type;

            જો( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                જો( !is_parent_flow )
                {
                    જો( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    જો( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                કરો c = *++endptr;
                જ્યારે( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_ગાંઠ::STRING));

                જો( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                જો( is_parent_flow || c != ':' )
                {
                    અક્ષર* str_end = endptr;
                    કરો c = *--str_end;
                    જ્યારે( str_end > ptr && c == ' ' );
                    str_end++;
                    જો ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        જો (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ગાંઠ.set_કિંમત(File_ગાંઠ::INT, &ival);
                            ptr = endptr;
                            પાછા ptr;
                        }
                        નહીંતર જો (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ગાંઠ.set_કિંમત(File_ગાંઠ::INT, &ival);
                            ptr = endptr;
                            પાછા ptr;
                        }
                        નહીંતર જો (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            પાછા ptr;
                        }
                    }

                    ગાંઠ.set_કિંમત(File_ગાંઠ::STRING, ptr, (પૂર્ણાંક)(str_end - ptr));
                    ptr = endptr;
                    પાછા ptr;
                }
                struct_type = File_ગાંઠ::MAP;
            }
            નહીંતર
                struct_type = File_ગાંઠ::SEQ;

            fs->convertToCollection( struct_type, ગાંઠ );
            indent = (પૂર્ણાંક)(ptr - fs->bufferStart());

            માટે(;;)
            {
                File_ગાંઠ elem;

                જો( struct_type == File_ગાંઠ::MAP )
                {
                    ptr = parse_ચાવી( ptr, ગાંઠ, elem );
                }
                નહીંતર
                {
                    c = *ptr++;
                    જો( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_ગાંઠ(ગાંઠ, std::string(), File_ગાંઠ::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_કિંમત( ptr, elem, indent + 1, ખોટું, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                જો( ptr - fs->bufferStart() != indent )
                {
                    જો( ptr - fs->bufferStart() < indent )
                        તોડો;
                    નહીંતર
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                જો( memcmp( ptr, "...", 3 ) == 0 )
                    તોડો;
            }
            fs->finalizeCollection(ગાંઠ);
        }

        પાછા ptr;
    }

    તાર્કિક parse( અક્ષર* ptr )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        તાર્કિક first = સાચું;
        તાર્કિક ok = સાચું;
        File_ગાંઠ મૂળ_collection(fs->getFS(), 0, 0);

        માટે(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            માટે(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                જો( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    તોડો;
                }

                જો( *ptr == '%' )
                {
                    જો( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        જો ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                નહીંતર જો( *ptr == '-' )
                {
                    જો( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        તોડો;
                    }
                    નહીંતર જો( first )
                        તોડો;
                }
                નહીંતર જો( cv_isalnum(*ptr) || *ptr=='_')
                {
                    જો( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    તોડો;
                }
                નહીંતર જો( fs->eof() )
                    તોડો;
                નહીંતર
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            જો( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            જો( !ptr || !ptr[0] )
                તોડો;
            જો( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ગાંઠ મૂળ_ગાંઠ = fs->add_ગાંઠ(મૂળ_collection, std::string(), File_ગાંઠ::NONE);

                ptr = parse_કિંમત( ptr, મૂળ_ગાંઠ, 0, ખોટું );
                જો( !મૂળ_ગાંઠ.isMap() && !મૂળ_ગાંઠ.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                જો( !ptr )
                    તોડો;
            }

            જો( fs->eof() )
                તોડો;
            ptr += 3;
            first = ખોટું;
        }

        પાછા ok;
    }

    FileStorage_API* fs;
    અક્ષર buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    પાછા makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    પાછા makePtr<YAMLParser>(fs);
}

}
