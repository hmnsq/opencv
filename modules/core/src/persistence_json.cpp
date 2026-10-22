// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence.hpp"

நாமாகாஶ cv
{

வகுப்பு JSONEmitter : பொது FileStorageEmitter
{
பொது:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    மெய்நிகர் ~JSONEmitter() {}

    FStructData startWriteStruct( மாறிலி FStructData& parent, மாறிலி எழுத்து* சாவி,
                                  முழு struct_flags, மாறிலி எழுத்து* type_name=0 )
    {
        எழுத்து data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_முனை::TYPE_MASK|File_முனை::FLOW)) | File_முனை::EMPTY;
        எனில்( !File_முனை::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        எனில்( type_name && *type_name == '\0' )
            type_name = 0;

        தருக்க is_real_collection = உண்மை;
        எனில் (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_முனை::STR;
            data[0] = '\0';
            is_real_collection = பொய்;
        }

        எனில் ( is_real_collection )
        {
            எழுத்து c = File_முனை::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( சாவி, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        திரும்பு current_struct;
    }

    வெறுமை endWriteStruct(மாறிலி FStructData& current_struct)
    {
        முழு struct_flags = current_struct.flags;

        எனில் (File_முனை::isCollection(struct_flags)) {
            எனில் (!File_முனை::isFlow(struct_flags)) {
                எனில் (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    எழுத்து *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            எழுத்து *ptr = fs->bufferPtr();
            எனில் (ptr > fs->bufferStart() + current_struct.indent && !File_முனை::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_முனை::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, முழு value)
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::itoa( value, buf, 10 ));
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, int64_t value)
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::itoa( value, buf, 10, உண்மை ));
    }

    வெறுமை லிக( மாறிலி எழுத்து* சாவி, இரட்டை value )
    {
        எழுத்து buf[128];
        writeScalar( சாவி, fs::doubleToString( buf, அளவு(buf), value, உண்மை ));
    }

    வெறுமை லிக(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* str, தருக்க quote)
    {
        எழுத்து buf[CV_FS_MAX_LEN*6+16];
        எழுத்து* data = (எழுத்து*)str;
        முழு i, len;

        எனில்( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (முழு)strlen(str);
        எனில்( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        எனில்( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            முழு need_quote = 1;
            data = buf;
            *data++ = '\"';
            க்கு( i = 0; i < len; i++ )
            {
                எழுத்து c = str[i];
                மாறிலி unsigned எழுத்து byte = static_cast<unsigned எழுத்து>(c);

                தேர்வு ( c )
                {
                நிலை '\\':
                நிலை '\"': { *data++ = '\\'; *data++ = c;   நிறுத்து; }
                நிலை '\n': { *data++ = '\\'; *data++ = 'n'; நிறுத்து; }
                நிலை '\r': { *data++ = '\\'; *data++ = 'r'; நிறுத்து; }
                நிலை '\t': { *data++ = '\\'; *data++ = 't'; நிறுத்து; }
                நிலை '\b': { *data++ = '\\'; *data++ = 'b'; நிறுத்து; }
                நிலை '\f': { *data++ = '\\'; *data++ = 'f'; நிறுத்து; }
                இயல்பு  :
                    எனில் (byte < 0x20)
                    {
                        நிலையான மாறிலி எழுத்து hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    இல்லையெனில்
                        *data++ = c;
                    நிறுத்து;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( சாவி, data);
    }

    வெறுமை writeScalar(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(பொய்);
        எனில் ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        இல்லையெனில் எனில் ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t சாவி_len = 0u;
        எனில்( சாவி && *சாவி == '\0' )
            சாவி = 0;
        எனில் ( சாவி )
        {
            சாவி_len = strlen(சாவி);
            எனில் ( சாவி_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            இல்லையெனில் எனில் ( static_cast<முழு>(சாவி_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        எனில் ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        முழு struct_flags = current_struct.flags;
        எனில்( File_முனை::isCollection(struct_flags) )
        {
            எனில் ( (File_முனை::isMap(struct_flags) ^ (சாவி != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } இல்லையெனில் {
            fs->setNonEmpty();
            struct_flags = File_முனை::EMPTY | (சாவி ? File_முனை::MAP : File_முனை::SEQ);
        }

        // start to write
        எழுத்து* ptr = 0;

        எனில்( File_முனை::isFlow(struct_flags) )
        {
            முழு new_offset;
            ptr = fs->bufferPtr();
            எனில்( !File_முனை::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<முழு>(ptr - fs->bufferStart() + சாவி_len + data_len);
            எனில்( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            இல்லையெனில்
                *ptr++ = ' ';
        }
        இல்லையெனில்
        {
            எனில் ( !File_முனை::isEmptyCollection(struct_flags) )
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

        எனில்( சாவி )
        {
            எனில்( !cv_isalpha(சாவி[0]) && சாவி[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<முழு>(சாவி_len) );
            *ptr++ = '\"';

            க்கு( size_t i = 0u; i < சாவி_len; i++ )
            {
                எழுத்து c = சாவி[i];

                ptr[i] = c;
                எனில்( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += சாவி_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        எனில்( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<முழு>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_முனை::EMPTY;
    }

    வெறுமை writeComment(மாறிலி எழுத்து* comment, தருக்க eol_comment)
    {
        எனில்( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        முழு len = static_cast<முழு>(strlen(comment));
        எழுத்து* ptr = fs->bufferPtr();
        மாறிலி எழுத்து* eol = strchr(comment, '\n');
        தருக்க multiline = eol != 0;

        எனில்( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        இல்லையெனில்
            *ptr++ = ' ';

        போது( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            எனில்( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (முழு)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            இல்லையெனில்
            {
                len = (முழு)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    வெறுமை startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

பாதுகாக்கப்பட்ட:
    FileStorage_API* fs;
};

வகுப்பு JSONParser : பொது FileStorageParser
{
பொது:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    மெய்நிகர் ~JSONParser() {}

    எழுத்து* skipSpaces( எழுத்து* ptr )
    {
        தருக்க is_eof = பொய்;
        தருக்க is_completed = பொய்;

        போது ( is_eof == பொய் && is_completed == பொய் )
        {
            எனில் (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            தேர்வு ( *ptr )
            {
                /* comment */
                நிலை '/' : {
                    ptr++;
                    எனில் ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        எனில்( !ptr || !*ptr ) { is_eof = உண்மை; நிறுத்து; }
                    }

                    எனில் ( *ptr == '/' )
                    {
                        போது ( *ptr != '\n' && *ptr != '\r' )
                        {
                            எனில் ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                எனில்( !ptr || !*ptr ) { is_eof = உண்மை; நிறுத்து; }
                            }
                            இல்லையெனில்
                            {
                                ptr++;
                            }
                        }
                    }
                    இல்லையெனில் எனில் ( *ptr == '*' )
                    {
                        ptr++;
                        க்கு (;;)
                        {
                            எனில் ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                எனில்( !ptr || !*ptr ) { is_eof = உண்மை; நிறுத்து; }
                            }
                            இல்லையெனில் எனில் ( *ptr == '*' )
                            {
                                ptr++;
                                எனில் ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    எனில்( !ptr || !*ptr ) { is_eof = உண்மை; நிறுத்து; }
                                }
                                எனில் ( *ptr == '/' )
                                {
                                    ptr++;
                                    நிறுத்து;
                                }
                            }
                            இல்லையெனில்
                            {
                                ptr++;
                            }
                        }
                    }
                    இல்லையெனில்
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } நிறுத்து;
                    /* whitespace */
                நிலை '\t':
                நிலை ' ' : {
                    ptr++;
                } நிறுத்து;
                    /* newline || end mark */
                நிலை '\0':
                நிலை '\n':
                நிலை '\r': {
                    ptr = fs->gets();
                    எனில்( !ptr || !*ptr ) { is_eof = உண்மை; நிறுத்து; }
                } நிறுத்து;
                    /* other character */
                இயல்பு: {
                    எனில்( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = உண்மை;
                } நிறுத்து;
            }
        }

        எனில் ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            எனில்( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        திரும்பு ptr;
    }

    uint32_t parseUnicodeEscape(எழுத்து*& ptr)
    {
        uint32_t codepoint = 0;
        க்கு (முழு k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            எழுத்து hex = *ptr;
            uint32_t digit = 0;
            எனில்      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            இல்லையெனில் எனில் (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            இல்லையெனில் எனில் (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            இல்லையெனில் CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        திரும்பு codepoint;
    }

    நிலையான வெறுமை appendUtf8(std::string& out, uint32_t codepoint)
    {
        எனில் (codepoint < 0x80) {
            out += (எழுத்து)codepoint;
        } இல்லையெனில் எனில் (codepoint < 0x800) {
            out += (எழுத்து)(0xC0 | (codepoint >> 6));
            out += (எழுத்து)(0x80 | (codepoint & 0x3F));
        } இல்லையெனில் எனில் (codepoint < 0x10000) {
            out += (எழுத்து)(0xE0 | (codepoint >> 12));
            out += (எழுத்து)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (எழுத்து)(0x80 | (codepoint & 0x3F));
        } இல்லையெனில் {
            out += (எழுத்து)(0xF0 | (codepoint >> 18));
            out += (எழுத்து)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (எழுத்து)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (எழுத்து)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    வெறுமை parseUnicodeEscapeToUtf8(எழுத்து*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        எனில் (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            எனில் (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t கீழ் = parseUnicodeEscape(ptr);
            எனில் (கீழ் < 0xDC00 || கீழ் > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (கீழ் - 0xDC00u);
        }
        இல்லையெனில் எனில் (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    எழுத்து* parse_சாவி( எழுத்து* ptr, File_முனை& collection, File_முனை& மதிப்பு_placeholder )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        எனில்( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        எழுத்து * beg = ptr + 1;
        std::string சாவி_name;
        செய் {
            எனில் (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                எனில் (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, சாவி_name);
                    தொடர்;
                }
                சாவி_name += *ptr;
            } இல்லையெனில் எனில் (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                சாவி_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } போது( cv_isprint(*ptr) && *ptr != '"' );

        எனில்( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        எனில்( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        மதிப்பு_placeholder = fs->add_முனை(collection, சாவி_name, File_முனை::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        எனில்( !ptr || !*ptr )
            திரும்பு 0;

        எனில்( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        திரும்பு ++ptr;
    }

    தருக்க getBase64Row(எழுத்து* ptr, முழு /*indent*/, எழுத்து* &beg, எழுத்து* &end)
    {
        beg = end = ptr;
        எனில்( !ptr || !*ptr )
            திரும்பு பொய்;

        // find end of the row
        போது( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        எனில் ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        திரும்பு உண்மை;
    }

    எழுத்து* parse_மதிப்பு( எழுத்து* ptr, File_முனை& முனை )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        எனில்( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        எனில்( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            எழுத்து * beg = ptr;
            size_t len = 0u;
            க்கு ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            எனில் ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, முனை);

                எனில் ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                இல்லையெனில்
                    ptr++;
            }
            இல்லையெனில்
            {   /**************** normal string ****************/
                முழு i = 0, sz;

                ptr = beg;
                தருக்க is_matching = பொய்;
                போது ( !is_matching )
                {
                    தேர்வு ( *ptr )
                    {
                        நிலை '\\':
                        {
                            sz = (முழு)(ptr - beg);
                            எனில்( sz > 0 )
                            {
                                எனில் (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            எனில் (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            தேர்வு ( *ptr )
                            {
                            நிலை '\\':
                            நிலை '\"':
                            நிலை '\'': { buf[i++] = *ptr; நிறுத்து; }
                            நிலை 'n' : { buf[i++] = '\n'; நிறுத்து; }
                            நிலை 'r' : { buf[i++] = '\r'; நிறுத்து; }
                            நிலை 't' : { buf[i++] = '\t'; நிறுத்து; }
                            நிலை 'b' : { buf[i++] = '\b'; நிறுத்து; }
                            நிலை 'f' : { buf[i++] = '\f'; நிறுத்து; }
                            நிலை 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                எனில் (i + (முழு)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (முழு)utf8.size();
                                beg = ptr;
                                தொடர்;
                            }
                            இயல்பு  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            நிறுத்து;
                            }
                            ptr++;
                            beg = ptr;
                            நிறுத்து;
                        }
                        நிலை '\0':
                        {
                            sz = (முழு)(ptr - beg);
                            எனில்( sz > 0 )
                            {
                                எனில் (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            எனில் ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            நிறுத்து;
                        }
                        நிலை '\"':
                        {
                            sz = (முழு)(ptr - beg);
                            எனில்( sz > 0 )
                            {
                                எனில் (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = உண்மை;
                            நிறுத்து;
                        }
                        நிலை '\n':
                        நிலை '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            நிறுத்து;
                        }
                        இயல்பு:
                        {
                            ptr++;
                            நிறுத்து;
                        }
                    }
                }

                எனில் ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                இல்லையெனில்
                    ptr++;

                முனை.set_மதிப்பு(File_முனை::STRING, buf, i);
            }
        }
        இல்லையெனில் எனில் ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            எழுத்து * beg = ptr;
            எனில் ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            போது( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            எனில் (*ptr == '.' || *ptr == 'e')
            {
                இரட்டை fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                முனை.set_மதிப்பு(File_முனை::REAL, &fval);
            }
            இல்லையெனில்
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                முனை.set_மதிப்பு(File_முனை::INT, &ival);
            }

            எனில் ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        இல்லையெனில்
        {    /**************** other data ****************/
            மாறிலி எழுத்து* beg = ptr;
            முழு len = 0;
            க்கு ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            எனில்( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            இல்லையெனில் எனில்( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                முனை.set_மதிப்பு(File_முனை::INT, &ival);
            }
            இல்லையெனில்
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        திரும்பு ptr;
    }

    எழுத்து* parseSeq( எழுத்து* ptr, File_முனை& முனை, முழு depth = 0 )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        எனில்( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        எனில் ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        இல்லையெனில்
            ptr++;

        fs->convertToCollection(File_முனை::SEQ, முனை);

        க்கு (;;)
        {
            ptr = skipSpaces( ptr );
            எனில்( !ptr || !*ptr )
                நிறுத்து;

            எனில் ( *ptr != ']' )
            {
                File_முனை child = fs->add_முனை(முனை, std::string(), File_முனை::NONE );

                எனில் ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                இல்லையெனில் எனில் ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                இல்லையெனில்
                    ptr = parse_மதிப்பு( ptr, child );
            }

            ptr = skipSpaces( ptr );
            எனில்( !ptr || !*ptr )
                நிறுத்து;

            எனில் ( *ptr == ',' )
                ptr++;
            இல்லையெனில் எனில் ( *ptr == ']' )
                நிறுத்து;
            இல்லையெனில்
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        எனில் ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        இல்லையெனில்
            ptr++;

        fs->finalizeCollection(முனை);
        திரும்பு ptr;
    }

    எழுத்து* parseMap( எழுத்து* ptr, File_முனை& முனை, முழு depth = 0 )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        எனில்( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        எனில் ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        இல்லையெனில்
            ptr++;

        fs->convertToCollection(File_முனை::MAP, முனை);

        க்கு( ;; )
        {
            ptr = skipSpaces( ptr );
            எனில்( !ptr || !*ptr )
                நிறுத்து;

            எனில் ( *ptr == '"' )
            {
                File_முனை child;
                ptr = parse_சாவி( ptr, முனை, child );
                எனில்( !ptr || !*ptr )
                    நிறுத்து;
                ptr = skipSpaces( ptr );
                எனில்( !ptr || !*ptr )
                    நிறுத்து;

                எனில் ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                இல்லையெனில் எனில் ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                இல்லையெனில்
                    ptr = parse_மதிப்பு( ptr, child );
            }

            ptr = skipSpaces( ptr );
            எனில்( !ptr || !*ptr )
                நிறுத்து;

            எனில் ( *ptr == ',' )
                ptr++;
            இல்லையெனில் எனில் ( *ptr == '}' )
                நிறுத்து;
            இல்லையெனில்
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        எனில் ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        இல்லையெனில்
            ptr++;

        fs->finalizeCollection(முனை);
        திரும்பு ptr;
    }

    தருக்க parse( எழுத்து* ptr )
    {
        எனில் (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        எனில் ( !ptr || !*ptr )
            திரும்பு பொய்;

        File_முனை வேர்_collection(fs->getFS(), 0, 0);

        எனில்( *ptr == '{' )
        {
            File_முனை வேர்_முனை = fs->add_முனை(வேர்_collection, std::string(), File_முனை::MAP);
            parseMap( ptr, வேர்_முனை );
        }
        இல்லையெனில் எனில் ( *ptr == '[' )
        {
            File_முனை வேர்_முனை = fs->add_முனை(வேர்_collection, std::string(), File_முனை::SEQ);
            parseSeq( ptr, வேர்_முனை );
        }
        இல்லையெனில்
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        திரும்பு உண்மை;
    }

    FileStorage_API* fs;
    எழுத்து buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    திரும்பு makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    திரும்பு makePtr<JSONParser>(fs);
}

}
