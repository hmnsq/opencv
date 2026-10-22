// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ಗಣನಾ
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

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ XMLEmitter : ಸಾರ್ವಜನಿಕ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ಆಭಾಸ ~XMLEmitter() {}

    ಶೂನ್ಯ writeTag( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ tag_type, ಧ್ರುವ std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        ವರ್ಣ* ptr = fs->bufferPtr();
        ಪೂರ್ಣಾಙ್ಕ i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;

        ಯದಿ( ಕೀಲಿ && ಕೀಲಿ[0] == '\0' )
            ಕೀಲಿ = 0;

        ಯದಿ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ಯದಿ( File_ಗಂಟು::isCollection(struct_flags) )
            {
                ಯದಿ( File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ಅನ್ಯಥಾ
            {
                struct_flags = File_ಗಂಟು::EMPTY + (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
                //fs->is_first = 0;
            }

            ಯದಿ( !File_ಗಂಟು::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ಯದಿ( !ಕೀಲಿ )
            ಕೀಲಿ = "_";
        ಅನ್ಯಥಾ ಯದಿ( ಕೀಲಿ[0] == '_' && ಕೀಲಿ[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ಪೂರ್ಣಾಙ್ಕ)strlen( ಕೀಲಿ );
        *ptr++ = '<';
        ಯದಿ( tag_type == CV_XML_CLOSING_TAG )
        {
            ಯದಿ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ಯದಿ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        ಪ್ರತಿ( i = 0; i < len; i++ )
        {
            ವರ್ಣ c = ಕೀಲಿ[i];
            ಯದಿ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ಪೂರ್ಣಾಙ್ಕ nattr = (ಪೂರ್ಣಾಙ್ಕ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        ಪ್ರತಿ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ಪೂರ್ಣಾಙ್ಕ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ಯದಿ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ಯದಿ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_ಗಂಟು::EMPTY;
    }

    FStructData startWriteStruct(ಧ್ರುವ FStructData& parent, ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ,
                                 ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ* type_name=0)
    {
        std::vector<std::string> attrlist;
        ಯದಿ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( ಕೀಲಿ, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = ಕೀಲಿ ? std::string(ಕೀಲಿ) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ಪ್ರತ್ಯಾಗಮ current_struct;
    }

    ಶೂನ್ಯ endWriteStruct(ಧ್ರುವ FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ value)
    {
        ವರ್ಣ buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( ಕೀಲಿ, ptr);
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, int64_t value)
    {
        ವರ್ಣ buf[128], *ptr = fs::itoa( value, buf, 10, ಸತ್ಯ );
        writeScalar( ಕೀಲಿ, ptr);
    }

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ದ್ವಿಗುಣ value )
    {
        ವರ್ಣ buf[128];
        writeScalar( ಕೀಲಿ, fs::doubleToString( buf, ಪರಿಮಾಣ(buf), value, ಅಸತ್ಯ ) );
    }

    ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* str, ತಾರ್ಕಿಕ quote)
    {
        ವರ್ಣ buf[CV_FS_MAX_LEN*6+16];
        ವರ್ಣ* data = (ವರ್ಣ*)str;
        ಪೂರ್ಣಾಙ್ಕ i, len;

        ಯದಿ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಙ್ಕ)strlen(str);
        ಯದಿ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಯದಿ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            ತಾರ್ಕಿಕ need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            ಪ್ರತಿ( i = 0; i < len; i++ )
            {
                ವರ್ಣ c = str[i];

                ಯದಿ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = ಸತ್ಯ;
                }
                ಅನ್ಯಥಾ ಯದಿ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ಯದಿ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    ಅನ್ಯಥಾ ಯದಿ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    ಅನ್ಯಥಾ
                    {
                        snprintf( data, ಪರಿಮಾಣ(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                ಅನ್ಯಥಾ
                    *data++ = c;
            }
            ಯದಿ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ಸತ್ಯ;

            ಯದಿ( need_quote )
                *data++ = '\"';
            len = (ಪೂರ್ಣಾಙ್ಕ)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ಪೂರ್ಣಾಙ್ಕ)!need_quote;
        }

        writeScalar( ಕೀಲಿ, data );
    }

    ಶೂನ್ಯ writeScalar(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* data)
    {
        fs->check_if_write_struct_is_delayed(ಅಸತ್ಯ);
        ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ಅನ್ಯಥಾ ಯದಿ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)strlen(data);
        ಯದಿ( ಕೀಲಿ && *ಕೀಲಿ == '\0' )
            ಕೀಲಿ = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಙ್ಕ struct_flags = current_struct.flags;

        ಯದಿ( File_ಗಂಟು::isMap(struct_flags) ||
           (!File_ಗಂಟು::isCollection(struct_flags) && ಕೀಲಿ) )
        {
            writeTag( ಕೀಲಿ, CV_XML_OPENING_TAG );
            ವರ್ಣ* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( ಕೀಲಿ, CV_XML_CLOSING_TAG );
        }
        ಅನ್ಯಥಾ
        {
            ವರ್ಣ* ptr = fs->bufferPtr();
            ಪೂರ್ಣಾಙ್ಕ new_offset = (ಪೂರ್ಣಾಙ್ಕ)(ptr - fs->bufferStart()) + len;

            ಯದಿ( ಕೀಲಿ )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ಗಂಟು::SEQ;

            ಯದಿ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ಅನ್ಯಥಾ ಯದಿ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    ಶೂನ್ಯ writeComment(ಧ್ರುವ ವರ್ಣ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಙ್ಕ len;
        ಪೂರ್ಣಾಙ್ಕ multiline;
        ಧ್ರುವ ವರ್ಣ* eol;
        ವರ್ಣ* ptr;

        ಯದಿ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಯದಿ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ಪೂರ್ಣಾಙ್ಕ)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ಯದಿ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        ಅನ್ಯಥಾ ಯದಿ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ಯದಿ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ಪೂರ್ಣಾಙ್ಕ)strlen(ptr);
        }
        ಅನ್ಯಥಾ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ಯದಿ( multiline )
        {
            ಯಾವತ್( comment )
            {
                ಯದಿ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ಪೂರ್ಣಾಙ್ಕ)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ಅನ್ಯಥಾ
                {
                    len = (ಪೂರ್ಣಾಙ್ಕ)strlen(comment);
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

    ಶೂನ್ಯ startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

ರಕ್ಷಿತ:
    FileStorage_API* fs;
};

ವರ್ಗ XMLParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ಆಭಾಸ ~XMLParser() {}

    ವರ್ಣ* skipSpaces( ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ mode )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಪೂರ್ಣಾಙ್ಕ level = 0;

        ಪ್ರತಿ(;;)
        {
            ವರ್ಣ c;
            ptr--;

            ಯದಿ( mode == CV_XML_INSIDE_COMMENT )
            {
                ಕುರು c = *++ptr;
                ಯಾವತ್( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ಯದಿ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            ಅನ್ಯಥಾ ಯದಿ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                ಕುರು
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ಯದಿ( level < 0 )
                        ಪ್ರತ್ಯಾಗಮ ptr;
                } ಯಾವತ್( cv_isprint_or_tab(c) );
            }
            ಅನ್ಯಥಾ
            {
                ಕುರು c = *++ptr;
                ಯಾವತ್( c == ' ' || c == '\t' );

                ಯದಿ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ಯದಿ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ಅನ್ಯಥಾ ಯದಿ( cv_isprint(c) )
                    ಭಙ್ಗ;
            }

            ಯದಿ( !cv_isprint(*ptr) )
            {
                ಯದಿ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ಯದಿ( !ptr || *ptr == '\0' )
                    ಭಙ್ಗ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ತಾರ್ಕಿಕ getBase64Row(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ /*indent*/, ವರ್ಣ* &beg, ವರ್ಣ* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ಯದಿ( !ptr || !*ptr )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        // closing XML tag
        ಯದಿ ( *beg == '<' )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        // find end of the row
        ಯಾವತ್( cv_isprint(*ptr) )
            ++ptr;
        ಯದಿ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ವರ್ಣ* parse_ಮೌಲ್ಯ( ವರ್ಣ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಙ್ಕ depth = 0 )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಯದಿ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ಗಂಟು new_elem;
        ತಾರ್ಕಿಕ have_space = ಸತ್ಯ;
        ಪೂರ್ಣಾಙ್ಕ value_type = ಗಂಟು.type();
        std::string ಕೀಲಿ, key2, type_name;

        ಪ್ರತಿ(;;)
        {
            ವರ್ಣ c = *ptr, d;
            ವರ್ಣ* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ಯದಿ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ಯದಿ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = ಸತ್ಯ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ಯದಿ( c =='<' || c == '\0' )
            {
                ಪೂರ್ಣಾಙ್ಕ tag_type = 0;
                ಪೂರ್ಣಾಙ್ಕ elem_type = File_ಗಂಟು::NONE;

                ಯದಿ( d == '/' || c == '\0' )
                    ಭಙ್ಗ;

                ptr = parseTag( ptr, ಕೀಲಿ, type_name, tag_type );

                ಯದಿ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ಯದಿ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                ತಾರ್ಕಿಕ ದ್ವಿಮಾನ_string = ಅಸತ್ಯ;

                ಯದಿ( !type_name.empty() )
                {
                    ಧ್ರುವ ವರ್ಣ* tn = type_name.c_str();
                    ಯದಿ( strcmp(tn, "str") == 0 )
                        elem_type = File_ಗಂಟು::STRING;
                    ಅನ್ಯಥಾ ಯದಿ( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ಗಂಟು::MAP;
                    ಅನ್ಯಥಾ ಯದಿ( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ಗಂಟು::SEQ;
                    ಅನ್ಯಥಾ ಯದಿ( strcmp( tn, "binary") == 0)
                        ದ್ವಿಮಾನ_string = ಸತ್ಯ;
                }

                new_elem = fs->add_ಗಂಟು(ಗಂಟು, ಕೀಲಿ, elem_type, 0);
                ಯದಿ (!ದ್ವಿಮಾನ_string)
                    ptr = parse_ಮೌಲ್ಯ(ptr, new_elem, depth + 1);
                ಅನ್ಯಥಾ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ಯದಿ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ಯದಿ( tag_type != CV_XML_CLOSING_TAG || key2 != ಕೀಲಿ )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ಸತ್ಯ;
            }
            ಅನ್ಯಥಾ
            {
                ಯದಿ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ಗಂಟು* elem = &ಗಂಟು;
                ಯದಿ( ಗಂಟು.type() != File_ಗಂಟು::NONE )
                {
                    fs->convertToCollection( File_ಗಂಟು::SEQ, ಗಂಟು );
                    new_elem = fs->add_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE, 0);
                    elem = &new_elem;
                }

                ಯದಿ( value_type != File_ಗಂಟು::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ಯಾವತ್( cv_isdigit(*endptr) )
                        endptr++;
                    ಯದಿ( *endptr == '.' || *endptr == 'e' )
                    {
                        ದ್ವಿಗುಣ fval = fs->strtod( ptr, &endptr );
                        elem->set_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
                    }
                    ಅನ್ಯಥಾ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
                    }

                    ಯದಿ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                ಅನ್ಯಥಾ
                {
                    // string
                    ಪೂರ್ಣಾಙ್ಕ i = 0, len, is_quoted = 0;
                    ಯದಿ( c == '\"' )
                        is_quoted = 1;
                    ಅನ್ಯಥಾ
                        --ptr;
                    strbuf[0] = '\0';

                    ಪ್ರತಿ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ಯದಿ( !cv_isalnum(c) )
                        {
                            ಯದಿ( c == '\"' )
                            {
                                ಯದಿ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                ಭಙ್ಗ;
                            }
                            ಅನ್ಯಥಾ ಯದಿ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ಯದಿ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                ಭಙ್ಗ;
                            }
                            ಅನ್ಯಥಾ ಯದಿ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ಅನ್ಯಥಾ ಯದಿ( c == '&' )
                            {
                                ಯದಿ( *++ptr == '#' )
                                {
                                    ಪೂರ್ಣಾಙ್ಕ val, base = 10;
                                    ptr++;
                                    ಯದಿ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ಪೂರ್ಣಾಙ್ಕ)strtol( ptr, &endptr, base );
                                    ಯದಿ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (ವರ್ಣ)val;
                                }
                                ಅನ್ಯಥಾ
                                {
                                    endptr = ptr;
                                    ಕುರು c = *++endptr;
                                    ಯಾವತ್( cv_isalnum(c) );
                                    ಯದಿ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ಪೂರ್ಣಾಙ್ಕ)(endptr - ptr);
                                    ಯದಿ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ಅನ್ಯಥಾ ಯದಿ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ಅನ್ಯಥಾ ಯದಿ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ಅನ್ಯಥಾ ಯದಿ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ಅನ್ಯಥಾ ಯದಿ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ಅನ್ಯಥಾ
                                    {
                                        ಯದಿ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ಯದಿ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, strbuf, i);
                }

                ಯದಿ( value_type != File_ಗಂಟು::NONE && value_type != File_ಗಂಟು::SEQ && value_type != File_ಗಂಟು::MAP )
                    ಭಙ್ಗ;
                have_space = ಅಸತ್ಯ;
            }
        }
        fs->finalizeCollection(ಗಂಟು);

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ವರ್ಣ* parseTag( ವರ್ಣ* ptr, std::string& tag_name,
                    std::string& type_name, ಪೂರ್ಣಾಙ್ಕ& tag_type )
    {
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ಯದಿ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ಯದಿ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ಯದಿ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        ಅನ್ಯಥಾ ಯದಿ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        ಅನ್ಯಥಾ ಯದಿ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        ಅನ್ಯಥಾ ಯದಿ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ಅನ್ಯಥಾ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        ಪ್ರತಿ(;;)
        {
            ವರ್ಣ c, *endptr;
            ಯದಿ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            ಕುರು c = *++endptr;
            ಯಾವತ್( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ಯದಿ( tag_name.empty() )
                tag_name = attrname;
            ಅನ್ಯಥಾ
            {
                ಯದಿ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ಯದಿ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ಯದಿ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ಯದಿ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ಯದಿ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ಯದಿ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ಯದಿ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                ವರ್ಣ quote = *ptr++;
                endptr = ptr;
                ಪ್ರತಿ(;;)
                {
                    c = *endptr++;
                    ಯದಿ( c == quote )
                        ಭಙ್ಗ;
                    ಯದಿ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ಯದಿ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            ತಾರ್ಕಿಕ have_space = cv_isspace(c) || c == '\0';

            ಯದಿ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ಯದಿ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ಯದಿ( c == '>' )
            {
                ಯದಿ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                ಭಙ್ಗ;
            }
            ಅನ್ಯಥಾ ಯದಿ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ಯದಿ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                ಭಙ್ಗ;
            }
            ಅನ್ಯಥಾ ಯದಿ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                ಭಙ್ಗ;
            }

            ಯದಿ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ತಾರ್ಕಿಕ parse(ವರ್ಣ* ptr)
    {
        CV_Assert( fs != 0 );

        std::string ಕೀಲಿ, key2, type_name;
        ಪೂರ್ಣಾಙ್ಕ tag_type = 0;
        ತಾರ್ಕಿಕ ok = ಅಸತ್ಯ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ಯದಿ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಯದಿ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, ಕೀಲಿ, type_name, tag_type );
        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಯಾವತ್( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ಯದಿ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ಯದಿ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, ಕೀಲಿ, type_name, tag_type );
                ಯದಿ( tag_type != CV_XML_OPENING_TAG || ಕೀಲಿ != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ಗಂಟು ಬೇರು = fs->add_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::MAP, 0);
                ptr = parse_ಮೌಲ್ಯ( ptr, ಬೇರು );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ಯದಿ( tag_type != CV_XML_CLOSING_TAG || ಕೀಲಿ != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ಸತ್ಯ;
            }
        }
        CV_Assert( fs->eof() );
        ಪ್ರತ್ಯಾಗಮ ok;
    }

    FileStorage_API* fs;
    ವರ್ಣ strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ಪ್ರತ್ಯಾಗಮ makePtr<XMLParser>(fs);
}

}
