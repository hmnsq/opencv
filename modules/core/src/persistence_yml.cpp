// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence.hpp"

ଗଣନା
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

ନାମାକାଶ cv
{

ଶ୍ରେଣୀ YAMLEmitter : ସର୍ବସାଧାରଣ FileStorageEmitter
{
ସର୍ବସାଧାରଣ:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ଆଭାସୀ ~YAMLEmitter() {}

    FStructData startWriteStruct(ଅଚଳ FStructData& parent, ଅଚଳ ଅକ୍ଷର* ଚାବି,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର* type_name=0)
    {
        ଅକ୍ଷର buf[CV_FS_MAX_LEN + 1024];
        ଅଚଳ ଅକ୍ଷର* data = 0;

        ଯଦି ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (File_ଗଣ୍ଠି::TYPE_MASK|File_ଗଣ୍ଠି::FLOW)) | File_ଗଣ୍ଠି::EMPTY;
        ଯଦି( !File_ଗଣ୍ଠି::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ଯଦି (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = File_ଗଣ୍ଠି::SEQ;
            snprintf(buf, ଆକାର(buf), "!!binary |");
            data = buf;
        }
        ଅନ୍ୟଥା ଯଦି( File_ଗଣ୍ଠି::isFlow(struct_flags))
        {
            ଅକ୍ଷର c = File_ଗଣ୍ଠି::isMap(struct_flags) ? '{' : '[';
            struct_flags |= File_ଗଣ୍ଠି::FLOW;

            ଯଦି( type_name )
                snprintf( buf, ଆକାର(buf), "!!%s %c", type_name, c );
            ଅନ୍ୟଥା
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ଅନ୍ୟଥା ଯଦି( type_name )
        {
            snprintf( buf, ଆକାର(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( ଚାବି, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ଯଦି( !File_ଗଣ୍ଠି::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + File_ଗଣ୍ଠି::isFlow(struct_flags);

        ଫେରନ୍ତୁ fsd;
    }

    ଶୂନ୍ୟ endWriteStruct(ଅଚଳ FStructData& current_struct)
    {
        ଅକ୍ଷର* ptr;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;

        ଯଦି( File_ଗଣ୍ଠି::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ଯଦି( ptr > fs->bufferStart() + current_struct.indent && !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = File_ଗଣ୍ଠି::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ଅନ୍ୟଥା ଯଦି( File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, File_ଗଣ୍ଠି::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
    {
        ଅକ୍ଷର buf[128];
        writeScalar( ଚାବି, fs::itoa( value, buf, 10 ));
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, int64_t value)
    {
        ଅକ୍ଷର buf[128];
        writeScalar( ଚାବି, fs::itoa( value, buf, 10, ସତ୍ୟ ));
    }

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ ଅକ୍ଷର* ଚାବି, ଦ୍ୱିଗୁଣ value )
    {
        ଅକ୍ଷର buf[128];
        writeScalar( ଚାବି, fs::doubleToString( buf, ଆକାର(buf), value, ମିଥ୍ୟା ));
    }

    ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* str, ତାର୍କିକ quote)
    {
        ଅକ୍ଷର buf[CV_FS_MAX_LEN*4+16];
        ଅକ୍ଷର* data = (ଅକ୍ଷର*)str;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len;

        ଯଦି( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(str);
        ଯଦି( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ଯଦି( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            ପାଇଁ( i = 0; i < len; i++ )
            {
                ଅକ୍ଷର c = str[i];

                ଯଦି( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ଯଦି( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ଯଦି( cv_isprint(c) )
                        *data++ = c;
                    ଅନ୍ୟଥା ଯଦି( c == '\n' )
                        *data++ = 'n';
                    ଅନ୍ୟଥା ଯଦି( c == '\r' )
                        *data++ = 'r';
                    ଅନ୍ୟଥା ଯଦି( c == '\t' )
                        *data++ = 't';
                    ଅନ୍ୟଥା
                    {
                        snprintf( data, ଆକାର(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ଅନ୍ୟଥା
                    *data++ = c;
            }
            ଯଦି( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ଯଦି( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ଚାବି, data);
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

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, keylen = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା datalen = 0;
        ଅକ୍ଷର* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;

        ଯଦି( ଚାବି && ଚାବି[0] == '\0' )
            ଚାବି = 0;

        ଯଦି( File_ଗଣ୍ଠି::isCollection(struct_flags) )
        {
            ଯଦି( (File_ଗଣ୍ଠି::isMap(struct_flags) ^ (ଚାବି != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ଅନ୍ୟଥା
        {
            fs->setNonEmpty();
            struct_flags = File_ଗଣ୍ଠି::EMPTY | (ଚାବି ? File_ଗଣ୍ଠି::MAP : File_ଗଣ୍ଠି::SEQ);
        }

        ଯଦି( ଚାବି )
        {
            keylen = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(ଚାବି);
            ଯଦି( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ଯଦି( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ଯଦି( data )
            datalen = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(data);

        ଯଦି( File_ଗଣ୍ଠି::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ଯଦି( !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - fs->bufferStart()) + keylen + datalen;
            ଯଦି( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ଅନ୍ୟଥା
                *ptr++ = ' ';
        }
        ଅନ୍ୟଥା
        {
            ptr = fs->flush();
            ଯଦି( !File_ଗଣ୍ଠି::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ଯଦି( data )
                    *ptr++ = ' ';
            }
        }

        ଯଦି( ଚାବି )
        {
            ଯଦି( !cv_isalpha(ଚାବି[0]) && ଚାବି[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            ପାଇଁ( i = 0; i < keylen; i++ )
            {
                ଅକ୍ଷର c = ଚାବି[i];

                ptr[i] = c;
                ଯଦି( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ଯଦି( !File_ଗଣ୍ଠି::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ଯଦି( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ଗଣ୍ଠି::EMPTY;
    }

    ଶୂନ୍ୟ writeComment(ଅଚଳ ଅକ୍ଷର* comment, ତାର୍କିକ eol_comment)
    {
        ଯଦି( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(comment);
        ଅଚଳ ଅକ୍ଷର* eol = strchr(comment, '\n');
        ତାର୍କିକ multiline = eol != 0;
        ଅକ୍ଷର* ptr = fs->bufferPtr();

        ଯଦି( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ଅନ୍ୟଥା
            *ptr++ = ' ';

        ଯେତେବେଳେ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ଯଦି( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ଅନ୍ୟଥା
            {
                len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ଶୂନ୍ୟ startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

ସୁରକ୍ଷିତ:
    FileStorage_API* fs;
};


ଶ୍ରେଣୀ YAMLParser : ସର୍ବସାଧାରଣ FileStorageParser
{
ସର୍ବସାଧାରଣ:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ଆଭାସୀ ~YAMLParser() {}

    ଅକ୍ଷର* skipSpaces( ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_indent, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_comment_indent )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ପାଇଁ(;;)
        {
            ଯେତେବେଳେ( *ptr == ' ' )
                ptr++;
            ଯଦି( *ptr == '#' )
            {
                ଯଦି( ptr - fs->bufferStart() > max_comment_indent )
                    ଫେରନ୍ତୁ ptr;
                *ptr = '\0';
            }
            ଅନ୍ୟଥା ଯଦି( cv_isprint(*ptr) )
            {
                ଯଦି( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                ଭାଙ୍ଗ;
            }
            ଅନ୍ୟଥା ଯଦି( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ଯଦି( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    ଭାଙ୍ଗ;
                }
                ଅନ୍ୟଥା
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(ptr);
                    ଯଦି( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ଅନ୍ୟଥା
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ଫେରନ୍ତୁ ptr;
    }

    ତାର୍କିକ getBase64Row(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, ଅକ୍ଷର* &beg, ଅକ୍ଷର* &end)
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ଯଦି (!ptr || !*ptr)
            ଫେରନ୍ତୁ ମିଥ୍ୟା; // end of file

        ଯଦି (ptr - fs->bufferStart() != indent)
            ଫେରନ୍ତୁ ମିଥ୍ୟା; // end of base64 data

        /* find end */
        ଯେତେବେଳେ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ଯଦି (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }


    ଅକ୍ଷର* parse_ଚାବି( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& map_ଗଣ୍ଠି, File_ଗଣ୍ଠି& ମୂଲ୍ୟ_placeholder )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ଅକ୍ଷର c;
        ଅକ୍ଷର *endptr = ptr - 1, *saveptr;

        ଯଦି( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        କର c = *++endptr;
        ଯେତେବେଳେ( cv_isprint(c) && c != ':' );

        ଯଦି( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ଯଦି( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        କର c = *--endptr;
        ଯେତେବେଳେ( c == ' ' );

        ++endptr;

        ମୂଲ୍ୟ_placeholder = fs->add_ଗଣ୍ଠି(map_ଗଣ୍ଠି, std::string(ptr, endptr - ptr), File_ଗଣ୍ଠି::NONE);
        ptr = saveptr;

        ଫେରନ୍ତୁ ptr;
    }

    ଅକ୍ଷର* parse_ମୂଲ୍ୟ( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_indent, ତାର୍କିକ is_parent_flow, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0 )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ଯଦି( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ଅକ୍ଷର* endptr = 0;
        ଅକ୍ଷର c = ptr[0], d = ptr[1];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା value_type = File_ଗଣ୍ଠି::NONE;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
        ତାର୍କିକ is_ଦ୍ୱିମିକ_string = ମିଥ୍ୟା;
        ତାର୍କିକ is_user = ମିଥ୍ୟା;

        ଯଦି( c == '!' ) // handle explicit type specification
        {
            ଯଦି( d == '!' || d == '^' )
            {
                ptr++;
                is_user = ସତ୍ୟ;
                //value_type |= FileNode::USER;
            }
            ଯଦି ( d == '<') //support of full type heading from YAML 1.2
            {
                ଅଚଳ ଅକ୍ଷର* yamlTypeHeading = "<tag:yaml.org,2002:";
                ଅଚଳ size_t headingLength = strlen(yamlTypeHeading);

                ଅକ୍ଷର* typeEndPtr = ++ptr;

                କର d = *++typeEndPtr;
                ଯେତେବେଳେ( cv_isprint(d) && d != ' ' && d != '>' );

                ଯଦି ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ଯଦି ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = ସତ୍ୟ;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            କର d = *++endptr;
            ଯେତେବେଳେ( cv_isprint(d) && d != ' ' );
            len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(endptr - ptr);
            ଯଦି( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ଯଦି( len == 3 && !is_user )
            {
                ଯଦି( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = File_ଗଣ୍ଠି::STRING;
                ଅନ୍ୟଥା ଯଦି( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = File_ଗଣ୍ଠି::INT;
                ଅନ୍ୟଥା ଯଦି( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = File_ଗଣ୍ଠି::SEQ;
                ଅନ୍ୟଥା ଯଦି( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = File_ଗଣ୍ଠି::MAP;
            }
            ଅନ୍ୟଥା ଯଦି( len == 5 && !is_user )
            {
                ଯଦି( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = File_ଗଣ୍ଠି::REAL;
            }
            ଅନ୍ୟଥା ଯଦି (len == 6 && is_user)
            {
                ଯଦି( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = File_ଗଣ୍ଠି::SEQ;
                    is_ଦ୍ୱିମିକ_string = ସତ୍ୟ;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    କର {
                        d = *++endptr;
                        ଯଦି (d == '|')
                            ଭାଙ୍ଗ;
                    } ଯେତେବେଳେ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ଯଦି (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ଯଦି( !is_user )
            {
                ଯଦି (value_type == File_ଗଣ୍ଠି::STRING && c != '\'' && c != '\"')
                    ଯାଅ force_string;
                ଯଦି( value_type == File_ଗଣ୍ଠି::INT )
                    ଯାଅ force_int;
                ଯଦି( value_type == File_ଗଣ୍ଠି::REAL )
                    ଯାଅ force_real;
            }
        }

        ଯଦି (is_ଦ୍ୱିମିକ_string)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, ଗଣ୍ଠି);
        }
        ଅନ୍ୟଥା ଯଦି( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ଯେତେବେଳେ( cv_isdigit(*endptr) )
                endptr++;
            ଯଦି( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                ଦ୍ୱିଗୁଣ fval = fs->strtod( ptr, &endptr );
                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::REAL, &fval);
            }
            ଅନ୍ୟଥା
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
            }

            ଯଦି( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ଅନ୍ୟଥା ଯଦି( c == '\'' || c == '\"' ) // an explicit string
        {
            ଯଦି( c == '\'' )
                ପାଇଁ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ଯଦି( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ଅନ୍ୟଥା ଯଦି( c == '\'' )
                    {
                        c = *++ptr;
                        ଯଦି( c != '\'' )
                            ଭାଙ୍ଗ;
                        buf[len++] = c;
                    }
                    ଅନ୍ୟଥା
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ଅନ୍ୟଥା
                ପାଇଁ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ଯଦି( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ଅନ୍ୟଥା ଯଦି( c == '\"' )
                    {
                        ++ptr;
                        ଭାଙ୍ଗ;
                    }
                    ଅନ୍ୟଥା ଯଦି( c == '\\' )
                    {
                        d = *++ptr;
                        ଯଦି( d == '\'' )
                            buf[len++] = d;
                        ଅନ୍ୟଥା ଯଦି( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ଅନ୍ୟଥା ଯଦି( d == 'n' )
                            buf[len++] = '\n';
                        ଅନ୍ୟଥା ଯଦି( d == 'r' )
                            buf[len++] = '\r';
                        ଅନ୍ୟଥା ଯଦି( d == 't' )
                            buf[len++] = '\t';
                        ଅନ୍ୟଥା ଯଦି( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ଯଦି( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ଅନ୍ୟଥା
                            {
                                buf[len++] = (ଅକ୍ଷର)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ଅନ୍ୟଥା
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ଯଦି( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::STRING, buf, len);
        }
        ଅନ୍ୟଥା ଯଦି( c == '[' || c == '{' ) // collection as a flow
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_min_indent = min_indent + !is_parent_flow;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_type = c == '{' ? File_ଗଣ୍ଠି::MAP : File_ଗଣ୍ଠି::SEQ;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା nelems = 0;

            fs->convertToCollection(struct_type, ଗଣ୍ଠି);
            d = c == '[' ? ']' : '}';

            ପାଇଁ( ++ptr ;; nelems++ )
            {
                File_ଗଣ୍ଠି elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ଯଦି (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ଯଦି( *ptr == '}' || *ptr == ']' )
                {
                    ଯଦି( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    ଭାଙ୍ଗ;
                }

                ଯଦି( nelems != 0 )
                {
                    ଯଦି( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ଯଦି (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ଯଦି( struct_type == File_ଗଣ୍ଠି::MAP )
                {
                    ptr = parse_ଚାବି( ptr, ଗଣ୍ଠି, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ଅନ୍ୟଥା
                {
                    ଯଦି( *ptr == ']' )
                        ଭାଙ୍ଗ;
                    elem = fs->add_ଗଣ୍ଠି(ଗଣ୍ଠି, std::string(), File_ଗଣ୍ଠି::NONE);
                }
                ptr = parse_ମୂଲ୍ୟ( ptr, elem, new_min_indent, ସତ୍ୟ, depth + 1 );
            }
            fs->finalizeCollection(ଗଣ୍ଠି);
        }
        ଅନ୍ୟଥା
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, struct_type;

            ଯଦି( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ଯଦି( !is_parent_flow )
                {
                    ଯଦି( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ଯଦି( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                କର c = *++endptr;
                ଯେତେବେଳେ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == File_ଗଣ୍ଠି::STRING));

                ଯଦି( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ଯଦି( is_parent_flow || c != ':' )
                {
                    ଅକ୍ଷର* str_end = endptr;
                    କର c = *--str_end;
                    ଯେତେବେଳେ( str_end > ptr && c == ' ' );
                    str_end++;
                    ଯଦି ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ଯଦି (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
                            ptr = endptr;
                            ଫେରନ୍ତୁ ptr;
                        }
                        ଅନ୍ୟଥା ଯଦି (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
                            ptr = endptr;
                            ଫେରନ୍ତୁ ptr;
                        }
                        ଅନ୍ୟଥା ଯଦି (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ଫେରନ୍ତୁ ptr;
                        }
                    }

                    ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::STRING, ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(str_end - ptr));
                    ptr = endptr;
                    ଫେରନ୍ତୁ ptr;
                }
                struct_type = File_ଗଣ୍ଠି::MAP;
            }
            ଅନ୍ୟଥା
                struct_type = File_ଗଣ୍ଠି::SEQ;

            fs->convertToCollection( struct_type, ଗଣ୍ଠି );
            indent = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - fs->bufferStart());

            ପାଇଁ(;;)
            {
                File_ଗଣ୍ଠି elem;

                ଯଦି( struct_type == File_ଗଣ୍ଠି::MAP )
                {
                    ptr = parse_ଚାବି( ptr, ଗଣ୍ଠି, elem );
                }
                ଅନ୍ୟଥା
                {
                    c = *ptr++;
                    ଯଦି( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->add_ଗଣ୍ଠି(ଗଣ୍ଠି, std::string(), File_ଗଣ୍ଠି::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parse_ମୂଲ୍ୟ( ptr, elem, indent + 1, ମିଥ୍ୟା, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ଯଦି( ptr - fs->bufferStart() != indent )
                {
                    ଯଦି( ptr - fs->bufferStart() < indent )
                        ଭାଙ୍ଗ;
                    ଅନ୍ୟଥା
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ଯଦି( memcmp( ptr, "...", 3 ) == 0 )
                    ଭାଙ୍ଗ;
            }
            fs->finalizeCollection(ଗଣ୍ଠି);
        }

        ଫେରନ୍ତୁ ptr;
    }

    ତାର୍କିକ parse( ଅକ୍ଷର* ptr )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ତାର୍କିକ first = ସତ୍ୟ;
        ତାର୍କିକ ok = ସତ୍ୟ;
        File_ଗଣ୍ଠି ମୂଳ_collection(fs->getFS(), 0, 0);

        ପାଇଁ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            ପାଇଁ(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ଯଦି( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    ଭାଙ୍ଗ;
                }

                ଯଦି( *ptr == '%' )
                {
                    ଯଦି( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ଯଦି ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ଅନ୍ୟଥା ଯଦି( *ptr == '-' )
                {
                    ଯଦି( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        ଭାଙ୍ଗ;
                    }
                    ଅନ୍ୟଥା ଯଦି( first )
                        ଭାଙ୍ଗ;
                }
                ଅନ୍ୟଥା ଯଦି( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ଯଦି( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    ଭାଙ୍ଗ;
                }
                ଅନ୍ୟଥା ଯଦି( fs->eof() )
                    ଭାଙ୍ଗ;
                ଅନ୍ୟଥା
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ଯଦି( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ଯଦି( !ptr || !ptr[0] )
                ଭାଙ୍ଗ;
            ଯଦି( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                File_ଗଣ୍ଠି ମୂଳ_ଗଣ୍ଠି = fs->add_ଗଣ୍ଠି(ମୂଳ_collection, std::string(), File_ଗଣ୍ଠି::NONE);

                ptr = parse_ମୂଲ୍ୟ( ptr, ମୂଳ_ଗଣ୍ଠି, 0, ମିଥ୍ୟା );
                ଯଦି( !ମୂଳ_ଗଣ୍ଠି.isMap() && !ମୂଳ_ଗଣ୍ଠି.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ଯଦି( !ptr )
                    ଭାଙ୍ଗ;
            }

            ଯଦି( fs->eof() )
                ଭାଙ୍ଗ;
            ptr += 3;
            first = ମିଥ୍ୟା;
        }

        ଫେରନ୍ତୁ ok;
    }

    FileStorage_API* fs;
    ଅକ୍ଷର buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<YAMLParser>(fs);
}

}
