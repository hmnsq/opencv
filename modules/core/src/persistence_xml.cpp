// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence.hpp"

ગણના
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

નામાકાશ cv
{

વર્ગ XMLEmitter : જાહેર FileStorageEmitter
{
જાહેર:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    આભાસી ~XMLEmitter() {}

    શૂન્ય writeTag( અચલ અક્ષર* ચાવી, પૂર્ણાંક tag_type, અચલ std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        અક્ષર* ptr = fs->bufferPtr();
        પૂર્ણાંક i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        પૂર્ણાંક struct_flags = current_struct.flags;

        જો( ચાવી && ચાવી[0] == '\0' )
            ચાવી = 0;

        જો( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            જો( File_ગાંઠ::isCollection(struct_flags) )
            {
                જો( File_ગાંઠ::isMap(struct_flags) ^ (ચાવી != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            નહીંતર
            {
                struct_flags = File_ગાંઠ::EMPTY + (ચાવી ? File_ગાંઠ::MAP : File_ગાંઠ::SEQ);
                //fs->is_first = 0;
            }

            જો( !File_ગાંઠ::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        જો( !ચાવી )
            ચાવી = "_";
        નહીંતર જો( ચાવી[0] == '_' && ચાવી[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (પૂર્ણાંક)strlen( ચાવી );
        *ptr++ = '<';
        જો( tag_type == CV_XML_CLOSING_TAG )
        {
            જો( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        જો( !cv_isalpha(ચાવી[0]) && ચાવી[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        માટે( i = 0; i < len; i++ )
        {
            અક્ષર c = ચાવી[i];
            જો( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        પૂર્ણાંક nattr = (પૂર્ણાંક)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        માટે( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (પૂર્ણાંક)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            જો( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        જો( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_ગાંઠ::EMPTY;
    }

    FStructData startWriteStruct(અચલ FStructData& parent, અચલ અક્ષર* ચાવી,
                                 પૂર્ણાંક struct_flags, અચલ અક્ષર* type_name=0)
    {
        std::vector<std::string> attrlist;
        જો( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( ચાવી, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = ચાવી ? std::string(ચાવી) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        પાછા current_struct;
    }

    શૂન્ય endWriteStruct(અચલ FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, પૂર્ણાંક value)
    {
        અક્ષર buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( ચાવી, ptr);
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, int64_t value)
    {
        અક્ષર buf[128], *ptr = fs::itoa( value, buf, 10, સાચું );
        writeScalar( ચાવી, ptr);
    }

    શૂન્ય લિખ( અચલ અક્ષર* ચાવી, દ્વિગુણ value )
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::doubleToString( buf, કદ(buf), value, ખોટું ) );
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, અચલ અક્ષર* str, તાર્કિક quote)
    {
        અક્ષર buf[CV_FS_MAX_LEN*6+16];
        અક્ષર* data = (અક્ષર*)str;
        પૂર્ણાંક i, len;

        જો( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (પૂર્ણાંક)strlen(str);
        જો( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        જો( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            તાર્કિક need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            માટે( i = 0; i < len; i++ )
            {
                અક્ષર c = str[i];

                જો( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = સાચું;
                }
                નહીંતર જો( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    જો( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    નહીંતર જો( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    નહીંતર જો( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    નહીંતર જો( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    નહીંતર જો( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    નહીંતર
                    {
                        snprintf( data, કદ(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                નહીંતર
                    *data++ = c;
            }
            જો( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = સાચું;

            જો( need_quote )
                *data++ = '\"';
            len = (પૂર્ણાંક)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (પૂર્ણાંક)!need_quote;
        }

        writeScalar( ચાવી, data );
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

        પૂર્ણાંક len = (પૂર્ણાંક)strlen(data);
        જો( ચાવી && *ચાવી == '\0' )
            ચાવી = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        પૂર્ણાંક struct_flags = current_struct.flags;

        જો( File_ગાંઠ::isMap(struct_flags) ||
           (!File_ગાંઠ::isCollection(struct_flags) && ચાવી) )
        {
            writeTag( ચાવી, CV_XML_OPENING_TAG );
            અક્ષર* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( ચાવી, CV_XML_CLOSING_TAG );
        }
        નહીંતર
        {
            અક્ષર* ptr = fs->bufferPtr();
            પૂર્ણાંક new_offset = (પૂર્ણાંક)(ptr - fs->bufferStart()) + len;

            જો( ચાવી )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ગાંઠ::SEQ;

            જો( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            નહીંતર જો( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    શૂન્ય writeComment(અચલ અક્ષર* comment, તાર્કિક eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        પૂર્ણાંક len;
        પૂર્ણાંક multiline;
        અચલ અક્ષર* eol;
        અક્ષર* ptr;

        જો( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        જો( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (પૂર્ણાંક)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        જો( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        નહીંતર જો( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        જો( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (પૂર્ણાંક)strlen(ptr);
        }
        નહીંતર
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        જો( multiline )
        {
            જ્યારે( comment )
            {
                જો( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (પૂર્ણાંક)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                નહીંતર
                {
                    len = (પૂર્ણાંક)strlen(comment);
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

    શૂન્ય startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

સુરક્ષિત:
    FileStorage_API* fs;
};

વર્ગ XMLParser : જાહેર FileStorageParser
{
જાહેર:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    આભાસી ~XMLParser() {}

    અક્ષર* skipSpaces( અક્ષર* ptr, પૂર્ણાંક mode )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        પૂર્ણાંક level = 0;

        માટે(;;)
        {
            અક્ષર c;
            ptr--;

            જો( mode == CV_XML_INSIDE_COMMENT )
            {
                કરો c = *++ptr;
                જ્યારે( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                જો( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            નહીંતર જો( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                કરો
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    જો( level < 0 )
                        પાછા ptr;
                } જ્યારે( cv_isprint_or_tab(c) );
            }
            નહીંતર
            {
                કરો c = *++ptr;
                જ્યારે( c == ' ' || c == '\t' );

                જો( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    જો( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                નહીંતર જો( cv_isprint(c) )
                    તોડો;
            }

            જો( !cv_isprint(*ptr) )
            {
                જો( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                જો( !ptr || *ptr == '\0' )
                    તોડો;
            }
        }
        પાછા ptr;
    }

    તાર્કિક getBase64Row(અક્ષર* ptr, પૂર્ણાંક /*indent*/, અક્ષર* &beg, અક્ષર* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        જો( !ptr || !*ptr )
            પાછા ખોટું;

        // closing XML tag
        જો ( *beg == '<' )
            પાછા ખોટું;

        // find end of the row
        જ્યારે( cv_isprint(*ptr) )
            ++ptr;
        જો ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        પાછા સાચું;
    }

    અક્ષર* parse_કિંમત( અક્ષર* ptr, File_ગાંઠ& ગાંઠ, પૂર્ણાંક depth = 0 )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        જો( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ગાંઠ new_elem;
        તાર્કિક have_space = સાચું;
        પૂર્ણાંક value_type = ગાંઠ.type();
        std::string ચાવી, key2, type_name;

        માટે(;;)
        {
            અક્ષર c = *ptr, d;
            અક્ષર* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            જો( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                જો (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = સાચું;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            જો( c =='<' || c == '\0' )
            {
                પૂર્ણાંક tag_type = 0;
                પૂર્ણાંક elem_type = File_ગાંઠ::NONE;

                જો( d == '/' || c == '\0' )
                    તોડો;

                ptr = parseTag( ptr, ચાવી, type_name, tag_type );

                જો( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                જો( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                તાર્કિક દ્વિઅંકી_string = ખોટું;

                જો( !type_name.empty() )
                {
                    અચલ અક્ષર* tn = type_name.c_str();
                    જો( strcmp(tn, "str") == 0 )
                        elem_type = File_ગાંઠ::STRING;
                    નહીંતર જો( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ગાંઠ::MAP;
                    નહીંતર જો( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ગાંઠ::SEQ;
                    નહીંતર જો( strcmp( tn, "binary") == 0)
                        દ્વિઅંકી_string = સાચું;
                }

                new_elem = fs->add_ગાંઠ(ગાંઠ, ચાવી, elem_type, 0);
                જો (!દ્વિઅંકી_string)
                    ptr = parse_કિંમત(ptr, new_elem, depth + 1);
                નહીંતર
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    જો (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                જો( tag_type != CV_XML_CLOSING_TAG || key2 != ચાવી )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = સાચું;
            }
            નહીંતર
            {
                જો( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ગાંઠ* elem = &ગાંઠ;
                જો( ગાંઠ.type() != File_ગાંઠ::NONE )
                {
                    fs->convertToCollection( File_ગાંઠ::SEQ, ગાંઠ );
                    new_elem = fs->add_ગાંઠ(ગાંઠ, std::string(), File_ગાંઠ::NONE, 0);
                    elem = &new_elem;
                }

                જો( value_type != File_ગાંઠ::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    જ્યારે( cv_isdigit(*endptr) )
                        endptr++;
                    જો( *endptr == '.' || *endptr == 'e' )
                    {
                        દ્વિગુણ fval = fs->strtod( ptr, &endptr );
                        elem->set_કિંમત(File_ગાંઠ::REAL, &fval);
                    }
                    નહીંતર
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_કિંમત(File_ગાંઠ::INT, &ival);
                    }

                    જો( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                નહીંતર
                {
                    // string
                    પૂર્ણાંક i = 0, len, is_quoted = 0;
                    જો( c == '\"' )
                        is_quoted = 1;
                    નહીંતર
                        --ptr;
                    strbuf[0] = '\0';

                    માટે( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        જો( !cv_isalnum(c) )
                        {
                            જો( c == '\"' )
                            {
                                જો( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                તોડો;
                            }
                            નહીંતર જો( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                જો( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                તોડો;
                            }
                            નહીંતર જો( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            નહીંતર જો( c == '&' )
                            {
                                જો( *++ptr == '#' )
                                {
                                    પૂર્ણાંક val, base = 10;
                                    ptr++;
                                    જો( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (પૂર્ણાંક)strtol( ptr, &endptr, base );
                                    જો( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (અક્ષર)val;
                                }
                                નહીંતર
                                {
                                    endptr = ptr;
                                    કરો c = *++endptr;
                                    જ્યારે( cv_isalnum(c) );
                                    જો( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (પૂર્ણાંક)(endptr - ptr);
                                    જો( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    નહીંતર જો( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    નહીંતર જો( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    નહીંતર જો( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    નહીંતર જો( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    નહીંતર
                                    {
                                        જો (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        જો (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_કિંમત(File_ગાંઠ::STRING, strbuf, i);
                }

                જો( value_type != File_ગાંઠ::NONE && value_type != File_ગાંઠ::SEQ && value_type != File_ગાંઠ::MAP )
                    તોડો;
                have_space = ખોટું;
            }
        }
        fs->finalizeCollection(ગાંઠ);

        પાછા ptr;
    }

    અક્ષર* parseTag( અક્ષર* ptr, std::string& tag_name,
                    std::string& type_name, પૂર્ણાંક& tag_type )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        જો( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        જો( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        જો( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        નહીંતર જો( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        નહીંતર જો( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        નહીંતર જો( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        નહીંતર
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        માટે(;;)
        {
            અક્ષર c, *endptr;
            જો( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            કરો c = *++endptr;
            જ્યારે( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            જો( tag_name.empty() )
                tag_name = attrname;
            નહીંતર
            {
                જો( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                જો( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    જો (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    જો( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                જો( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    જો(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    જો( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                અક્ષર quote = *ptr++;
                endptr = ptr;
                માટે(;;)
                {
                    c = *endptr++;
                    જો( c == quote )
                        તોડો;
                    જો( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                જો( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            તાર્કિક have_space = cv_isspace(c) || c == '\0';

            જો( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                જો (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            જો( c == '>' )
            {
                જો( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                તોડો;
            }
            નહીંતર જો( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                જો( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                તોડો;
            }
            નહીંતર જો( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                તોડો;
            }

            જો( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        પાછા ptr;
    }

    તાર્કિક parse(અક્ષર* ptr)
    {
        CV_Assert( fs != 0 );

        std::string ચાવી, key2, type_name;
        પૂર્ણાંક tag_type = 0;
        તાર્કિક ok = ખોટું;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        જો( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, ચાવી, type_name, tag_type );
        File_ગાંઠ મૂળ_collection(fs->getFS(), 0, 0);

        જ્યારે( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            જો (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            જો( *ptr != '\0' )
            {
                ptr = parseTag( ptr, ચાવી, type_name, tag_type );
                જો( tag_type != CV_XML_OPENING_TAG || ચાવી != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ગાંઠ મૂળ = fs->add_ગાંઠ(મૂળ_collection, std::string(), File_ગાંઠ::MAP, 0);
                ptr = parse_કિંમત( ptr, મૂળ );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                જો( tag_type != CV_XML_CLOSING_TAG || ચાવી != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = સાચું;
            }
        }
        CV_Assert( fs->eof() );
        પાછા ok;
    }

    FileStorage_API* fs;
    અક્ષર strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    પાછા makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    પાછા makePtr<XMLParser>(fs);
}

}
