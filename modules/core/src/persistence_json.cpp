// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence.hpp"

ନାମାକାଶ cv
{

ଶ୍ରେଣୀ JSONEmitter : ସର୍ବସାଧାରଣ FileStorageEmitter
{
ସର୍ବସାଧାରଣ:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ଆଭାସୀ ~JSONEmitter() {}

    FStructData startWriteStruct( ଅଚଳ FStructData& parent, ଅଚଳ ଅକ୍ଷର* ଚାବି,
                                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର* type_name=0 )
    {
        ଅକ୍ଷର data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ଗଣ୍ଠି::TYPE_MASK|File_ଗଣ୍ଠି::FLOW)) | File_ଗଣ୍ଠି::EMPTY;
        ଯଦି( !File_ଗଣ୍ଠି::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ଯଦି( type_name && *type_name == '\0' )
            type_name = 0;

        ତାର୍କିକ is_real_collection = ସତ୍ୟ;
        ଯଦି (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ଗଣ୍ଠି::STR;
            data[0] = '\0';
            is_real_collection = ମିଥ୍ୟା;
        }

        ଯଦି ( is_real_collection )
        {
            ଅକ୍ଷର c = File_ଗଣ୍ଠି::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( ଚାବି, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        ଫେରନ୍ତୁ current_struct;
    }

    ଶୂନ୍ୟ endWriteStruct(ଅଚଳ FStructData& current_struct)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;

        ଯଦି (File_ଗଣ୍ଠି::isCollection(struct_flags)) {
            ଯଦି (!File_ଗଣ୍ଠି::isFlow(struct_flags)) {
                ଯଦି (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    ଅକ୍ଷର *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            ଅକ୍ଷର *ptr = fs->bufferPtr();
            ଯଦି (ptr > fs->bufferStart() + current_struct.indent && !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ଗଣ୍ଠି::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
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
        writeScalar( ଚାବି, fs::doubleToString( buf, ଆକାର(buf), value, ସତ୍ୟ ));
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

        ଯଦି( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_quote = 1;
            data = buf;
            *data++ = '\"';
            ପାଇଁ( i = 0; i < len; i++ )
            {
                ଅକ୍ଷର c = str[i];
                ଅଚଳ unsigned ଅକ୍ଷର byte = static_cast<unsigned ଅକ୍ଷର>(c);

                ବାଛ ( c )
                {
                ସ୍ଥିତି '\\':
                ସ୍ଥିତି '\"': { *data++ = '\\'; *data++ = c;   ଭାଙ୍ଗ; }
                ସ୍ଥିତି '\n': { *data++ = '\\'; *data++ = 'n'; ଭାଙ୍ଗ; }
                ସ୍ଥିତି '\r': { *data++ = '\\'; *data++ = 'r'; ଭାଙ୍ଗ; }
                ସ୍ଥିତି '\t': { *data++ = '\\'; *data++ = 't'; ଭାଙ୍ଗ; }
                ସ୍ଥିତି '\b': { *data++ = '\\'; *data++ = 'b'; ଭାଙ୍ଗ; }
                ସ୍ଥିତି '\f': { *data++ = '\\'; *data++ = 'f'; ଭାଙ୍ଗ; }
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ  :
                    ଯଦି (byte < 0x20)
                    {
                        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    ଅନ୍ୟଥା
                        *data++ = c;
                    ଭାଙ୍ଗ;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ଚାବି, data);
    }

    ଶୂନ୍ୟ writeScalar(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ମିଥ୍ୟା);
        ଯଦି ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ଅନ୍ୟଥା ଯଦି ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ଚାବି_len = 0u;
        ଯଦି( ଚାବି && *ଚାବି == '\0' )
            ଚାବି = 0;
        ଯଦି ( ଚାବି )
        {
            ଚାବି_len = strlen(ଚାବି);
            ଯଦି ( ଚାବି_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ଅନ୍ୟଥା ଯଦି ( static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ଚାବି_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ଯଦି ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = current_struct.flags;
        ଯଦି( File_ଗଣ୍ଠି::isCollection(struct_flags) )
        {
            ଯଦି ( (File_ଗଣ୍ଠି::isMap(struct_flags) ^ (ଚାବି != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ଅନ୍ୟଥା {
            fs->setNonEmpty();
            struct_flags = File_ଗଣ୍ଠି::EMPTY | (ଚାବି ? File_ଗଣ୍ଠି::MAP : File_ଗଣ୍ଠି::SEQ);
        }

        // start to write
        ଅକ୍ଷର* ptr = 0;

        ଯଦି( File_ଗଣ୍ଠି::isFlow(struct_flags) )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_offset;
            ptr = fs->bufferPtr();
            ଯଦି( !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ptr - fs->bufferStart() + ଚାବି_len + data_len);
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
            ଯଦି ( !File_ଗଣ୍ଠି::isEmptyCollection(struct_flags) )
            {
                ptr = fs->bufferPtr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->bufferStart() );
                fs->setBufferPtr(fs->bufferStart());
            }
            ptr = fs->flush();
        }

        ଯଦି( ଚାବି )
        {
            ଯଦି( !cv_isalpha(ଚାବି[0]) && ଚାବି[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ଚାବି_len) );
            *ptr++ = '\"';

            ପାଇଁ( size_t i = 0u; i < ଚାବି_len; i++ )
            {
                ଅକ୍ଷର c = ଚାବି[i];

                ptr[i] = c;
                ଯଦି( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ଚାବି_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ଯଦି( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ଗଣ୍ଠି::EMPTY;
    }

    ଶୂନ୍ୟ writeComment(ଅଚଳ ଅକ୍ଷର* comment, ତାର୍କିକ eol_comment)
    {
        ଯଦି( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(strlen(comment));
        ଅକ୍ଷର* ptr = fs->bufferPtr();
        ଅଚଳ ଅକ୍ଷର* eol = strchr(comment, '\n');
        ତାର୍କିକ multiline = eol != 0;

        ଯଦି( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ଅନ୍ୟଥା
            *ptr++ = ' ';

        ଯେତେବେଳେ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
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

ଶ୍ରେଣୀ JSONParser : ସର୍ବସାଧାରଣ FileStorageParser
{
ସର୍ବସାଧାରଣ:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ଆଭାସୀ ~JSONParser() {}

    ଅକ୍ଷର* skipSpaces( ଅକ୍ଷର* ptr )
    {
        ତାର୍କିକ is_eof = ମିଥ୍ୟା;
        ତାର୍କିକ is_completed = ମିଥ୍ୟା;

        ଯେତେବେଳେ ( is_eof == ମିଥ୍ୟା && is_completed == ମିଥ୍ୟା )
        {
            ଯଦି (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            ବାଛ ( *ptr )
            {
                /* comment */
                ସ୍ଥିତି '/' : {
                    ptr++;
                    ଯଦି ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ଯଦି( !ptr || !*ptr ) { is_eof = ସତ୍ୟ; ଭାଙ୍ଗ; }
                    }

                    ଯଦି ( *ptr == '/' )
                    {
                        ଯେତେବେଳେ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ଯଦି ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ଯଦି( !ptr || !*ptr ) { is_eof = ସତ୍ୟ; ଭାଙ୍ଗ; }
                            }
                            ଅନ୍ୟଥା
                            {
                                ptr++;
                            }
                        }
                    }
                    ଅନ୍ୟଥା ଯଦି ( *ptr == '*' )
                    {
                        ptr++;
                        ପାଇଁ (;;)
                        {
                            ଯଦି ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ଯଦି( !ptr || !*ptr ) { is_eof = ସତ୍ୟ; ଭାଙ୍ଗ; }
                            }
                            ଅନ୍ୟଥା ଯଦି ( *ptr == '*' )
                            {
                                ptr++;
                                ଯଦି ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ଯଦି( !ptr || !*ptr ) { is_eof = ସତ୍ୟ; ଭାଙ୍ଗ; }
                                }
                                ଯଦି ( *ptr == '/' )
                                {
                                    ptr++;
                                    ଭାଙ୍ଗ;
                                }
                            }
                            ଅନ୍ୟଥା
                            {
                                ptr++;
                            }
                        }
                    }
                    ଅନ୍ୟଥା
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ଭାଙ୍ଗ;
                    /* whitespace */
                ସ୍ଥିତି '\t':
                ସ୍ଥିତି ' ' : {
                    ptr++;
                } ଭାଙ୍ଗ;
                    /* newline || end mark */
                ସ୍ଥିତି '\0':
                ସ୍ଥିତି '\n':
                ସ୍ଥିତି '\r': {
                    ptr = fs->gets();
                    ଯଦି( !ptr || !*ptr ) { is_eof = ସତ୍ୟ; ଭାଙ୍ଗ; }
                } ଭାଙ୍ଗ;
                    /* other character */
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
                    ଯଦି( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = ସତ୍ୟ;
                } ଭାଙ୍ଗ;
            }
        }

        ଯଦି ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ଯଦି( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ଫେରନ୍ତୁ ptr;
    }

    uint32_t parseUnicodeEscape(ଅକ୍ଷର*& ptr)
    {
        uint32_t codepoint = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            ଅକ୍ଷର hex = *ptr;
            uint32_t digit = 0;
            ଯଦି      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ଅନ୍ୟଥା ଯଦି (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ଅନ୍ୟଥା ଯଦି (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ଅନ୍ୟଥା CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ଫେରନ୍ତୁ codepoint;
    }

    ସ୍ଥିର ଶୂନ୍ୟ appendUtf8(std::string& out, uint32_t codepoint)
    {
        ଯଦି (codepoint < 0x80) {
            out += (ଅକ୍ଷର)codepoint;
        } ଅନ୍ୟଥା ଯଦି (codepoint < 0x800) {
            out += (ଅକ୍ଷର)(0xC0 | (codepoint >> 6));
            out += (ଅକ୍ଷର)(0x80 | (codepoint & 0x3F));
        } ଅନ୍ୟଥା ଯଦି (codepoint < 0x10000) {
            out += (ଅକ୍ଷର)(0xE0 | (codepoint >> 12));
            out += (ଅକ୍ଷର)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ଅକ୍ଷର)(0x80 | (codepoint & 0x3F));
        } ଅନ୍ୟଥା {
            out += (ଅକ୍ଷର)(0xF0 | (codepoint >> 18));
            out += (ଅକ୍ଷର)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (ଅକ୍ଷର)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ଅକ୍ଷର)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    ଶୂନ୍ୟ parseUnicodeEscapeToUtf8(ଅକ୍ଷର*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ଯଦି (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ଯଦି (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t ତଳ = parseUnicodeEscape(ptr);
            ଯଦି (ତଳ < 0xDC00 || ତଳ > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (ତଳ - 0xDC00u);
        }
        ଅନ୍ୟଥା ଯଦି (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    ଅକ୍ଷର* parse_ଚାବି( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& collection, File_ଗଣ୍ଠି& ମୂଲ୍ୟ_placeholder )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ଯଦି( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        ଅକ୍ଷର * beg = ptr + 1;
        std::string ଚାବି_name;
        କର {
            ଯଦି (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ଯଦି (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, ଚାବି_name);
                    ଜାରି;
                }
                ଚାବି_name += *ptr;
            } ଅନ୍ୟଥା ଯଦି (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ଚାବି_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ଯେତେବେଳେ( cv_isprint(*ptr) && *ptr != '"' );

        ଯଦି( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ଯଦି( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        ମୂଲ୍ୟ_placeholder = fs->add_ଗଣ୍ଠି(collection, ଚାବି_name, File_ଗଣ୍ଠି::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ଯଦି( !ptr || !*ptr )
            ଫେରନ୍ତୁ 0;

        ଯଦି( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ଫେରନ୍ତୁ ++ptr;
    }

    ତାର୍କିକ getBase64Row(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*indent*/, ଅକ୍ଷର* &beg, ଅକ୍ଷର* &end)
    {
        beg = end = ptr;
        ଯଦି( !ptr || !*ptr )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        // find end of the row
        ଯେତେବେଳେ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ଯଦି ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଅକ୍ଷର* parse_ମୂଲ୍ୟ( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& ଗଣ୍ଠି )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ଯଦି( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ଯଦି( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            ଅକ୍ଷର * beg = ptr;
            size_t len = 0u;
            ପାଇଁ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ଯଦି ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ଗଣ୍ଠି);

                ଯଦି ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ଅନ୍ୟଥା
                    ptr++;
            }
            ଅନ୍ୟଥା
            {   /**************** normal string ****************/
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, sz;

                ptr = beg;
                ତାର୍କିକ is_matching = ମିଥ୍ୟା;
                ଯେତେବେଳେ ( !is_matching )
                {
                    ବାଛ ( *ptr )
                    {
                        ସ୍ଥିତି '\\':
                        {
                            sz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - beg);
                            ଯଦି( sz > 0 )
                            {
                                ଯଦି (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ଯଦି (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            ବାଛ ( *ptr )
                            {
                            ସ୍ଥିତି '\\':
                            ସ୍ଥିତି '\"':
                            ସ୍ଥିତି '\'': { buf[i++] = *ptr; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 'n' : { buf[i++] = '\n'; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 'r' : { buf[i++] = '\r'; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 't' : { buf[i++] = '\t'; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 'b' : { buf[i++] = '\b'; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 'f' : { buf[i++] = '\f'; ଭାଙ୍ଗ; }
                            ସ୍ଥିତି 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                ଯଦି (i + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utf8.size();
                                beg = ptr;
                                ଜାରି;
                            }
                            ପୂର୍ବନିର୍ଦ୍ଧାରିତ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ଭାଙ୍ଗ;
                            }
                            ptr++;
                            beg = ptr;
                            ଭାଙ୍ଗ;
                        }
                        ସ୍ଥିତି '\0':
                        {
                            sz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - beg);
                            ଯଦି( sz > 0 )
                            {
                                ଯଦି (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ଯଦି ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ଭାଙ୍ଗ;
                        }
                        ସ୍ଥିତି '\"':
                        {
                            sz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ptr - beg);
                            ଯଦି( sz > 0 )
                            {
                                ଯଦି (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = ସତ୍ୟ;
                            ଭାଙ୍ଗ;
                        }
                        ସ୍ଥିତି '\n':
                        ସ୍ଥିତି '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ଭାଙ୍ଗ;
                        }
                        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                        {
                            ptr++;
                            ଭାଙ୍ଗ;
                        }
                    }
                }

                ଯଦି ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ଅନ୍ୟଥା
                    ptr++;

                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::STRING, buf, i);
            }
        }
        ଅନ୍ୟଥା ଯଦି ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            ଅକ୍ଷର * beg = ptr;
            ଯଦି ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ଯେତେବେଳେ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ଯଦି (*ptr == '.' || *ptr == 'e')
            {
                ଦ୍ୱିଗୁଣ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::REAL, &fval);
            }
            ଅନ୍ୟଥା
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
            }

            ଯଦି ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ଅନ୍ୟଥା
        {    /**************** other data ****************/
            ଅଚଳ ଅକ୍ଷର* beg = ptr;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = 0;
            ପାଇଁ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ଯଦି( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ଅନ୍ୟଥା ଯଦି( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ଗଣ୍ଠି.set_ମୂଲ୍ୟ(File_ଗଣ୍ଠି::INT, &ival);
            }
            ଅନ୍ୟଥା
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ଫେରନ୍ତୁ ptr;
    }

    ଅକ୍ଷର* parseSeq( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0 )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ଯଦି( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ଯଦି ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ଅନ୍ୟଥା
            ptr++;

        fs->convertToCollection(File_ଗଣ୍ଠି::SEQ, ଗଣ୍ଠି);

        ପାଇଁ (;;)
        {
            ptr = skipSpaces( ptr );
            ଯଦି( !ptr || !*ptr )
                ଭାଙ୍ଗ;

            ଯଦି ( *ptr != ']' )
            {
                File_ଗଣ୍ଠି child = fs->add_ଗଣ୍ଠି(ଗଣ୍ଠି, std::string(), File_ଗଣ୍ଠି::NONE );

                ଯଦି ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ଅନ୍ୟଥା ଯଦି ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ଅନ୍ୟଥା
                    ptr = parse_ମୂଲ୍ୟ( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ଯଦି( !ptr || !*ptr )
                ଭାଙ୍ଗ;

            ଯଦି ( *ptr == ',' )
                ptr++;
            ଅନ୍ୟଥା ଯଦି ( *ptr == ']' )
                ଭାଙ୍ଗ;
            ଅନ୍ୟଥା
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ଯଦି ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ଅନ୍ୟଥା
            ptr++;

        fs->finalizeCollection(ଗଣ୍ଠି);
        ଫେରନ୍ତୁ ptr;
    }

    ଅକ୍ଷର* parseMap( ଅକ୍ଷର* ptr, File_ଗଣ୍ଠି& ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0 )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ଯଦି( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ଯଦି ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ଅନ୍ୟଥା
            ptr++;

        fs->convertToCollection(File_ଗଣ୍ଠି::MAP, ଗଣ୍ଠି);

        ପାଇଁ( ;; )
        {
            ptr = skipSpaces( ptr );
            ଯଦି( !ptr || !*ptr )
                ଭାଙ୍ଗ;

            ଯଦି ( *ptr == '"' )
            {
                File_ଗଣ୍ଠି child;
                ptr = parse_ଚାବି( ptr, ଗଣ୍ଠି, child );
                ଯଦି( !ptr || !*ptr )
                    ଭାଙ୍ଗ;
                ptr = skipSpaces( ptr );
                ଯଦି( !ptr || !*ptr )
                    ଭାଙ୍ଗ;

                ଯଦି ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ଅନ୍ୟଥା ଯଦି ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ଅନ୍ୟଥା
                    ptr = parse_ମୂଲ୍ୟ( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ଯଦି( !ptr || !*ptr )
                ଭାଙ୍ଗ;

            ଯଦି ( *ptr == ',' )
                ptr++;
            ଅନ୍ୟଥା ଯଦି ( *ptr == '}' )
                ଭାଙ୍ଗ;
            ଅନ୍ୟଥା
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ଯଦି ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ଅନ୍ୟଥା
            ptr++;

        fs->finalizeCollection(ଗଣ୍ଠି);
        ଫେରନ୍ତୁ ptr;
    }

    ତାର୍କିକ parse( ଅକ୍ଷର* ptr )
    {
        ଯଦି (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ଯଦି ( !ptr || !*ptr )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        File_ଗଣ୍ଠି ମୂଳ_collection(fs->getFS(), 0, 0);

        ଯଦି( *ptr == '{' )
        {
            File_ଗଣ୍ଠି ମୂଳ_ଗଣ୍ଠି = fs->add_ଗଣ୍ଠି(ମୂଳ_collection, std::string(), File_ଗଣ୍ଠି::MAP);
            parseMap( ptr, ମୂଳ_ଗଣ୍ଠି );
        }
        ଅନ୍ୟଥା ଯଦି ( *ptr == '[' )
        {
            File_ଗଣ୍ଠି ମୂଳ_ଗଣ୍ଠି = fs->add_ଗଣ୍ଠି(ମୂଳ_collection, std::string(), File_ଗଣ୍ଠି::SEQ);
            parseSeq( ptr, ମୂଳ_ଗଣ୍ଠି );
        }
        ଅନ୍ୟଥା
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    FileStorage_API* fs;
    ଅକ୍ଷର buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ଫେରନ୍ତୁ makePtr<JSONParser>(fs);
}

}
