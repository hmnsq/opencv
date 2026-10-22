// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence.hpp"

ଗଣନା
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

ନାମାକାଶ cv
{

ଶ୍ରେଣୀ XMLEmitter : ସର୍ବସାଧାରଣ FileStorageEmitter
{
ସର୍ବସାଧାରଣ:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ଆଭାସୀ ~XMLEmitter() {}

    ଶୂନ୍ୟ writeTag( ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag_type, ଅଚଳ std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        ଅକ୍ଷର* ptr = fs->bufferPtr();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;

        ଯଦି( ଚାବି && ଚାବି[0] == '\0' )
            ଚାବି = 0;

        ଯଦି( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ଯଦି( File_ଗଣ୍ଠି::isCollection(struct_flags) )
            {
                ଯଦି( File_ଗଣ୍ଠି::isMap(struct_flags) ^ (ଚାବି != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ଅନ୍ୟଥା
            {
                struct_flags = File_ଗଣ୍ଠି::EMPTY + (ଚାବି ? File_ଗଣ୍ଠି::MAP : File_ଗଣ୍ଠି::SEQ);
                //fs->is_first = 0;
            }

            ଯଦି( !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ଯଦି( !ଚାବି )
            ଚାବି = "_";
        ଅନ୍ୟଥା ଯଦି( ଚାବି[0] == '_' && ଚାବି[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen( ଚାବି );
        *ptr++ = '<';
        ଯଦି( tag_type == CV_XML_CLOSING_TAG )
        {
            ଯଦି( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ଯଦି( !cv_isalpha(ଚାବି[0]) && ଚାବି[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        ପାଇଁ( i = 0; i < len; i++ )
        {
            ଅକ୍ଷର c = ଚାବି[i];
            ଯଦି( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା nattr = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        ପାଇଁ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ଯଦି( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ଯଦି( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~File_ଗଣ୍ଠି::EMPTY;
    }

    FStructData startWriteStruct(ଅଚଳ FStructData& parent, ଅଚଳ ଅକ୍ଷର* ଚାବି,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର* type_name=0)
    {
        std::vector<std::string> attrlist;
        ଯଦି( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( ଚାବି, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = ଚାବି ? std::string(ଚାବି) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ଫେରନ୍ତୁ current_struct;
    }

    ଶୂନ୍ୟ endWriteStruct(ଅଚଳ FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
    {
        ଅକ୍ଷର buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( ଚାବି, ptr);
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, int64_t value)
    {
        ଅକ୍ଷର buf[128], *ptr = fs::itoa( value, buf, 10, ସତ୍ୟ );
        writeScalar( ଚାବି, ptr);
    }

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ ଅକ୍ଷର* ଚାବି, ଦ୍ୱିଗୁଣ value )
    {
        ଅକ୍ଷର buf[128];
        writeScalar( ଚାବି, fs::doubleToString( buf, ଆକାର(buf), value, ମିଥ୍ୟା ) );
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* str, ତାର୍କିକ quote)
    {
        ଅକ୍ଷର buf[CV_FS_MAX_LEN*6+16];
        ଅକ୍ଷର* data = (ଅକ୍ଷର*)str;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len;

        ଯଦି( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(str);
        ଯଦି( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ଯଦି( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            ତାର୍କିକ need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            ପାଇଁ( i = 0; i < len; i++ )
            {
                ଅକ୍ଷର c = str[i];

                ଯଦି( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = ସତ୍ୟ;
                }
                ଅନ୍ୟଥା ଯଦି( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ଯଦି( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    ଅନ୍ୟଥା ଯଦି( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    ଅନ୍ୟଥା ଯଦି( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    ଅନ୍ୟଥା ଯଦି( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    ଅନ୍ୟଥା ଯଦି( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    ଅନ୍ୟଥା
                    {
                        snprintf( data, ଆକାର(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                ଅନ୍ୟଥା
                    *data++ = c;
            }
            ଯଦି( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = ସତ୍ୟ;

            ଯଦି( need_quote )
                *data++ = '\"';
            len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)!need_quote;
        }

        writeScalar( ଚାବି, data );
    }

    ଶୂନ୍ୟ writeScalar(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* data)
    {
        fs->check_if_write_struct_is_delayed(ମିଥ୍ୟା);
        ଯଦି ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ଅନ୍ୟଥା ଯଦି ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(data);
        ଯଦି( ଚାବି && *ଚାବି == '\0' )
            ଚାବି = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;

        ଯଦି( File_ଗଣ୍ଠି::isMap(struct_flags) ||
           (!File_ଗଣ୍ଠି::isCollection(struct_flags) && ଚାବି) )
        {
            writeTag( ଚାବି, CV_XML_OPENING_TAG );
            ଅକ୍ଷର* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( ଚାବି, CV_XML_CLOSING_TAG );
        }
        ଅନ୍ୟଥା
        {
            ଅକ୍ଷର* ptr = fs->bufferPtr();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - fs->bufferStart()) + len;

            ଯଦି( ଚାବି )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_ଗଣ୍ଠି::SEQ;

            ଯଦି( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ଅନ୍ୟଥା ଯଦି( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    ଶୂନ୍ୟ writeComment(ଅଚଳ ଅକ୍ଷର* comment, ତାର୍କିକ eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା multiline;
        ଅଚଳ ଅକ୍ଷର* eol;
        ଅକ୍ଷର* ptr;

        ଯଦି( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ଯଦି( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ଯଦି( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        ଅନ୍ୟଥା ଯଦି( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ଯଦି( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(ptr);
        }
        ଅନ୍ୟଥା
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ଯଦି( multiline )
        {
            ଯେତେବେଳେ( comment )
            {
                ଯଦି( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ଅନ୍ୟଥା
                {
                    len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(comment);
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

    ଶୂନ୍ୟ startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

ସୁରକ୍ଷିତ:
    FileStorage_API* fs;
};

ଶ୍ରେଣୀ XMLParser : ସର୍ବସାଧାରଣ FileStorageParser
{
ସର୍ବସାଧାରଣ:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ଆଭାସୀ ~XMLParser() {}

    ଅକ୍ଷର* skipSpaces( ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା level = 0;

        ପାଇଁ(;;)
        {
            ଅକ୍ଷର c;
            ptr--;

            ଯଦି( mode == CV_XML_INSIDE_COMMENT )
            {
                କର c = *++ptr;
                ଯେତେବେଳେ( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ଯଦି( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            ଅନ୍ୟଥା ଯଦି( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                କର
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ଯଦି( level < 0 )
                        ଫେରନ୍ତୁ ptr;
                } ଯେତେବେଳେ( cv_isprint_or_tab(c) );
            }
            ଅନ୍ୟଥା
            {
                କର c = *++ptr;
                ଯେତେବେଳେ( c == ' ' || c == '\t' );

                ଯଦି( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ଯଦି( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ଅନ୍ୟଥା ଯଦି( cv_isprint(c) )
                    ଭାଙ୍ଗ;
            }

            ଯଦି( !cv_isprint(*ptr) )
            {
                ଯଦି( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ଯଦି( !ptr || *ptr == '\0' )
                    ଭାଙ୍ଗ;
            }
        }
        ଫେରନ୍ତୁ ptr;
    }

    ତାର୍କିକ getBase64Row(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*indent*/, ଅକ୍ଷର* &beg, ଅକ୍ଷର* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ଯଦି( !ptr || !*ptr )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        // closing XML tag
        ଯଦି ( *beg == '<' )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        // find end of the row
        ଯେତେବେଳେ( cv_isprint(*ptr) )
            ++ptr;
        ଯଦି ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଅକ୍ଷର* parse_ମୂଲ୍ୟ( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0 )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ଯଦି( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_ଗଣ୍ଠି new_elem;
        ତାର୍କିକ have_space = ସତ୍ୟ;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା value_type = ଗଣ୍ଠି.type();
        std::string ଚାବି, key2, type_name;

        ପାଇଁ(;;)
        {
            ଅକ୍ଷର c = *ptr, d;
            ଅକ୍ଷର* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ଯଦି( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ଯଦି (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = ସତ୍ୟ;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ଯଦି( c =='<' || c == '\0' )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag_type = 0;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = File_ଗଣ୍ଠି::NONE;

                ଯଦି( d == '/' || c == '\0' )
                    ଭାଙ୍ଗ;

                ptr = parseTag( ptr, ଚାବି, type_name, tag_type );

                ଯଦି( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ଯଦି( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                ତାର୍କିକ ଦ୍ୱିମିକ_string = ମିଥ୍ୟା;

                ଯଦି( !type_name.empty() )
                {
                    ଅଚଳ ଅକ୍ଷର* tn = type_name.c_str();
                    ଯଦି( strcmp(tn, "str") == 0 )
                        elem_type = File_ଗଣ୍ଠି::STRING;
                    ଅନ୍ୟଥା ଯଦି( strcmp( tn, "map" ) == 0 )
                        elem_type = File_ଗଣ୍ଠି::MAP;
                    ଅନ୍ୟଥା ଯଦି( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_ଗଣ୍ଠି::SEQ;
                    ଅନ୍ୟଥା ଯଦି( strcmp( tn, "binary") == 0)
                        ଦ୍ୱିମିକ_string = ସତ୍ୟ;
                }

                new_elem = fs->add_ଗଣ୍ଠି(ଗଣ୍ଠି, ଚାବି, elem_type, 0);
                ଯଦି (!ଦ୍ୱିମିକ_string)
                    ptr = parse_ମୂଲ୍ୟ(ptr, new_elem, depth + 1);
                ଅନ୍ୟଥା
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ଯଦି (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ଯଦି( tag_type != CV_XML_CLOSING_TAG || key2 != ଚାବି )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = ସତ୍ୟ;
            }
            ଅନ୍ୟଥା
            {
                ଯଦି( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_ଗଣ୍ଠି* elem = &ଗଣ୍ଠି;
                ଯଦି( ଗଣ୍ଠି.type() != File_ଗଣ୍ଠି::NONE )
                {
                    fs->convertToCollection( File_ଗଣ୍ଠି::SEQ, ଗଣ୍ଠି );
                    new_elem = fs->add_ଗଣ୍ଠି(ଗଣ୍ଠି, std::string(), File_ଗଣ୍ଠି::NONE, 0);
                    elem = &new_elem;
                }

                ଯଦି( value_type != File_ଗଣ୍ଠି::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ଯେତେବେଳେ( cv_isdigit(*endptr) )
                        endptr++;
                    ଯଦି( *endptr == '.' || *endptr == 'e' )
                    {
                        ଦ୍ୱିଗୁଣ fval = fs->strtod( ptr, &endptr );
                        elem->set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::REAL, &fval);
                    }
                    ଅନ୍ୟଥା
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
                    }

                    ଯଦି( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                ଅନ୍ୟଥା
                {
                    // string
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, len, is_quoted = 0;
                    ଯଦି( c == '\"' )
                        is_quoted = 1;
                    ଅନ୍ୟଥା
                        --ptr;
                    strbuf[0] = '\0';

                    ପାଇଁ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ଯଦି( !cv_isalnum(c) )
                        {
                            ଯଦି( c == '\"' )
                            {
                                ଯଦି( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                ଭାଙ୍ଗ;
                            }
                            ଅନ୍ୟଥା ଯଦି( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ଯଦି( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                ଭାଙ୍ଗ;
                            }
                            ଅନ୍ୟଥା ଯଦି( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ଅନ୍ୟଥା ଯଦି( c == '&' )
                            {
                                ଯଦି( *++ptr == '#' )
                                {
                                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା val, base = 10;
                                    ptr++;
                                    ଯଦି( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strtol( ptr, &endptr, base );
                                    ଯଦି( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (ଅକ୍ଷର)val;
                                }
                                ଅନ୍ୟଥା
                                {
                                    endptr = ptr;
                                    କର c = *++endptr;
                                    ଯେତେବେଳେ( cv_isalnum(c) );
                                    ଯଦି( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(endptr - ptr);
                                    ଯଦି( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ଅନ୍ୟଥା ଯଦି( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ଅନ୍ୟଥା ଯଦି( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ଅନ୍ୟଥା ଯଦି( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ଅନ୍ୟଥା ଯଦି( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ଅନ୍ୟଥା
                                    {
                                        ଯଦି (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ଯଦି (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::STRING, strbuf, i);
                }

                ଯଦି( value_type != File_ଗଣ୍ଠି::NONE && value_type != File_ଗଣ୍ଠି::SEQ && value_type != File_ଗଣ୍ଠି::MAP )
                    ଭାଙ୍ଗ;
                have_space = ମିଥ୍ୟା;
            }
        }
        fs->finalizeCollection(ଗଣ୍ଠି);

        ଫେରନ୍ତୁ ptr;
    }

    ଅକ୍ଷର* parseTag( ଅକ୍ଷର* ptr, std::string& tag_name,
                    std::string& type_name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& tag_type )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ଯଦି( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ଯଦି( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ଯଦି( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        ଅନ୍ୟଥା ଯଦି( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        ଅନ୍ୟଥା ଯଦି( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        ଅନ୍ୟଥା ଯଦି( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ଅନ୍ୟଥା
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        ପାଇଁ(;;)
        {
            ଅକ୍ଷର c, *endptr;
            ଯଦି( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            କର c = *++endptr;
            ଯେତେବେଳେ( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ଯଦି( tag_name.empty() )
                tag_name = attrname;
            ଅନ୍ୟଥା
            {
                ଯଦି( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ଯଦି( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ଯଦି (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ଯଦି( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ଯଦି( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ଯଦି(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ଯଦି( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                ଅକ୍ଷର quote = *ptr++;
                endptr = ptr;
                ପାଇଁ(;;)
                {
                    c = *endptr++;
                    ଯଦି( c == quote )
                        ଭାଙ୍ଗ;
                    ଯଦି( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ଯଦି( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            ତାର୍କିକ have_space = cv_isspace(c) || c == '\0';

            ଯଦି( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ଯଦି (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ଯଦି( c == '>' )
            {
                ଯଦି( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                ଭାଙ୍ଗ;
            }
            ଅନ୍ୟଥା ଯଦି( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ଯଦି( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                ଭାଙ୍ଗ;
            }
            ଅନ୍ୟଥା ଯଦି( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                ଭାଙ୍ଗ;
            }

            ଯଦି( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ଫେରନ୍ତୁ ptr;
    }

    ତାର୍କିକ parse(ଅକ୍ଷର* ptr)
    {
        CV_Assert( fs != 0 );

        std::string ଚାବି, key2, type_name;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag_type = 0;
        ତାର୍କିକ ok = ମିଥ୍ୟା;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ଯଦି( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, ଚାବି, type_name, tag_type );
        File_ଗଣ୍ଠି ମୂଳ_collection(fs->getFS(), 0, 0);

        ଯେତେବେଳେ( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ଯଦି (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ଯଦି( *ptr != '\0' )
            {
                ptr = parseTag( ptr, ଚାବି, type_name, tag_type );
                ଯଦି( tag_type != CV_XML_OPENING_TAG || ଚାବି != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_ଗଣ୍ଠି ମୂଳ = fs->add_ଗଣ୍ଠି(ମୂଳ_collection, std::string(), File_ଗଣ୍ଠି::MAP, 0);
                ptr = parse_ମୂଲ୍ୟ( ptr, ମୂଳ );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ଯଦି( tag_type != CV_XML_CLOSING_TAG || ଚାବି != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = ସତ୍ୟ;
            }
        }
        CV_Assert( fs->eof() );
        ଫେରନ୍ତୁ ok;
    }

    FileStorage_API* fs;
    ଅକ୍ଷର strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<XMLParser>(fs);
}

}
