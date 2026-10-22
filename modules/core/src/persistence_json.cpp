// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence.hpp"

নামাকাশ cv
{

বর্গ JSONEmitter : সার্বজনিক FileStorageEmitter
{
সার্বজনিক:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    আভাস ~JSONEmitter() {}

    FStructData startWriteStruct( ধ্রুব FStructData& parent, ধ্রুব বর্ণ* চাবি,
                                  পূর্ণাঙ্ক struct_flags, ধ্রুব বর্ণ* type_name=0 )
    {
        বর্ণ data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_গ্রন্থি::TYPE_MASK|File_গ্রন্থি::FLOW)) | File_গ্রন্থি::EMPTY;
        যদি( !File_গ্রন্থি::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        যদি( type_name && *type_name == '\0' )
            type_name = 0;

        তার্কিক is_real_collection = সত্য;
        যদি (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_গ্রন্থি::STR;
            data[0] = '\0';
            is_real_collection = অসত্য;
        }

        যদি ( is_real_collection )
        {
            বর্ণ c = File_গ্রন্থি::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( চাবি, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        প্রত্যাগম current_struct;
    }

    শূন্য endWriteStruct(ধ্রুব FStructData& current_struct)
    {
        পূর্ণাঙ্ক struct_flags = current_struct.flags;

        যদি (File_গ্রন্থি::isCollection(struct_flags)) {
            যদি (!File_গ্রন্থি::isFlow(struct_flags)) {
                যদি (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    বর্ণ *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            বর্ণ *ptr = fs->bufferPtr();
            যদি (ptr > fs->bufferStart() + current_struct.indent && !File_গ্রন্থি::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_গ্রন্থি::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    শূন্য লিখ(ধ্রুব বর্ণ* চাবি, পূর্ণাঙ্ক value)
    {
        বর্ণ buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10 ));
    }

    শূন্য লিখ(ধ্রুব বর্ণ* চাবি, int64_t value)
    {
        বর্ণ buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10, সত্য ));
    }

    শূন্য লিখ( ধ্রুব বর্ণ* চাবি, দ্বিগুণ value )
    {
        বর্ণ buf[128];
        writeScalar( চাবি, fs::doubleToString( buf, পরিমাণ(buf), value, সত্য ));
    }

    শূন্য লিখ(ধ্রুব বর্ণ* চাবি, ধ্রুব বর্ণ* str, তার্কিক quote)
    {
        বর্ণ buf[CV_FS_MAX_LEN*6+16];
        বর্ণ* data = (বর্ণ*)str;
        পূর্ণাঙ্ক i, len;

        যদি( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (পূর্ণাঙ্ক)strlen(str);
        যদি( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        যদি( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            পূর্ণাঙ্ক need_quote = 1;
            data = buf;
            *data++ = '\"';
            প্রতি( i = 0; i < len; i++ )
            {
                বর্ণ c = str[i];
                ধ্রুব unsigned বর্ণ byte = static_cast<unsigned বর্ণ>(c);

                বিকল্প ( c )
                {
                পক্ষ '\\':
                পক্ষ '\"': { *data++ = '\\'; *data++ = c;   ভঙ্গ; }
                পক্ষ '\n': { *data++ = '\\'; *data++ = 'n'; ভঙ্গ; }
                পক্ষ '\r': { *data++ = '\\'; *data++ = 'r'; ভঙ্গ; }
                পক্ষ '\t': { *data++ = '\\'; *data++ = 't'; ভঙ্গ; }
                পক্ষ '\b': { *data++ = '\\'; *data++ = 'b'; ভঙ্গ; }
                পক্ষ '\f': { *data++ = '\\'; *data++ = 'f'; ভঙ্গ; }
                স্বভাব  :
                    যদি (byte < 0x20)
                    {
                        স্থির ধ্রুব বর্ণ hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    অন্যথা
                        *data++ = c;
                    ভঙ্গ;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( চাবি, data);
    }

    শূন্য writeScalar(ধ্রুব বর্ণ* চাবি, ধ্রুব বর্ণ* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(অসত্য);
        যদি ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        অন্যথা যদি ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t চাবি_len = 0u;
        যদি( চাবি && *চাবি == '\0' )
            চাবি = 0;
        যদি ( চাবি )
        {
            চাবি_len = strlen(চাবি);
            যদি ( চাবি_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            অন্যথা যদি ( static_cast<পূর্ণাঙ্ক>(চাবি_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        যদি ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        পূর্ণাঙ্ক struct_flags = current_struct.flags;
        যদি( File_গ্রন্থি::isCollection(struct_flags) )
        {
            যদি ( (File_গ্রন্থি::isMap(struct_flags) ^ (চাবি != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } অন্যথা {
            fs->setNonEmpty();
            struct_flags = File_গ্রন্থি::EMPTY | (চাবি ? File_গ্রন্থি::MAP : File_গ্রন্থি::SEQ);
        }

        // start to write
        বর্ণ* ptr = 0;

        যদি( File_গ্রন্থি::isFlow(struct_flags) )
        {
            পূর্ণাঙ্ক new_offset;
            ptr = fs->bufferPtr();
            যদি( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<পূর্ণাঙ্ক>(ptr - fs->bufferStart() + চাবি_len + data_len);
            যদি( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            অন্যথা
                *ptr++ = ' ';
        }
        অন্যথা
        {
            যদি ( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
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

        যদি( চাবি )
        {
            যদি( !cv_isalpha(চাবি[0]) && চাবি[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<পূর্ণাঙ্ক>(চাবি_len) );
            *ptr++ = '\"';

            প্রতি( size_t i = 0u; i < চাবি_len; i++ )
            {
                বর্ণ c = চাবি[i];

                ptr[i] = c;
                যদি( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += চাবি_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        যদি( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<পূর্ণাঙ্ক>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_গ্রন্থি::EMPTY;
    }

    শূন্য writeComment(ধ্রুব বর্ণ* comment, তার্কিক eol_comment)
    {
        যদি( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        পূর্ণাঙ্ক len = static_cast<পূর্ণাঙ্ক>(strlen(comment));
        বর্ণ* ptr = fs->bufferPtr();
        ধ্রুব বর্ণ* eol = strchr(comment, '\n');
        তার্কিক multiline = eol != 0;

        যদি( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        অন্যথা
            *ptr++ = ' ';

        যাবত্( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            যদি( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (পূর্ণাঙ্ক)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            অন্যথা
            {
                len = (পূর্ণাঙ্ক)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    শূন্য startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

রক্ষিত:
    FileStorage_API* fs;
};

বর্গ JSONParser : সার্বজনিক FileStorageParser
{
সার্বজনিক:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    আভাস ~JSONParser() {}

    বর্ণ* skipSpaces( বর্ণ* ptr )
    {
        তার্কিক is_eof = অসত্য;
        তার্কিক is_completed = অসত্য;

        যাবত্ ( is_eof == অসত্য && is_completed == অসত্য )
        {
            যদি (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            বিকল্প ( *ptr )
            {
                /* comment */
                পক্ষ '/' : {
                    ptr++;
                    যদি ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        যদি( !ptr || !*ptr ) { is_eof = সত্য; ভঙ্গ; }
                    }

                    যদি ( *ptr == '/' )
                    {
                        যাবত্ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            যদি ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                যদি( !ptr || !*ptr ) { is_eof = সত্য; ভঙ্গ; }
                            }
                            অন্যথা
                            {
                                ptr++;
                            }
                        }
                    }
                    অন্যথা যদি ( *ptr == '*' )
                    {
                        ptr++;
                        প্রতি (;;)
                        {
                            যদি ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                যদি( !ptr || !*ptr ) { is_eof = সত্য; ভঙ্গ; }
                            }
                            অন্যথা যদি ( *ptr == '*' )
                            {
                                ptr++;
                                যদি ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    যদি( !ptr || !*ptr ) { is_eof = সত্য; ভঙ্গ; }
                                }
                                যদি ( *ptr == '/' )
                                {
                                    ptr++;
                                    ভঙ্গ;
                                }
                            }
                            অন্যথা
                            {
                                ptr++;
                            }
                        }
                    }
                    অন্যথা
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ভঙ্গ;
                    /* whitespace */
                পক্ষ '\t':
                পক্ষ ' ' : {
                    ptr++;
                } ভঙ্গ;
                    /* newline || end mark */
                পক্ষ '\0':
                পক্ষ '\n':
                পক্ষ '\r': {
                    ptr = fs->gets();
                    যদি( !ptr || !*ptr ) { is_eof = সত্য; ভঙ্গ; }
                } ভঙ্গ;
                    /* other character */
                স্বভাব: {
                    যদি( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = সত্য;
                } ভঙ্গ;
            }
        }

        যদি ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            যদি( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        প্রত্যাগম ptr;
    }

    uint32_t parseUnicodeEscape(বর্ণ*& ptr)
    {
        uint32_t codepoint = 0;
        প্রতি (পূর্ণাঙ্ক k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            বর্ণ hex = *ptr;
            uint32_t digit = 0;
            যদি      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            অন্যথা যদি (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            অন্যথা যদি (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            অন্যথা CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        প্রত্যাগম codepoint;
    }

    স্থির শূন্য appendUtf8(std::string& out, uint32_t codepoint)
    {
        যদি (codepoint < 0x80) {
            out += (বর্ণ)codepoint;
        } অন্যথা যদি (codepoint < 0x800) {
            out += (বর্ণ)(0xC0 | (codepoint >> 6));
            out += (বর্ণ)(0x80 | (codepoint & 0x3F));
        } অন্যথা যদি (codepoint < 0x10000) {
            out += (বর্ণ)(0xE0 | (codepoint >> 12));
            out += (বর্ণ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (বর্ণ)(0x80 | (codepoint & 0x3F));
        } অন্যথা {
            out += (বর্ণ)(0xF0 | (codepoint >> 18));
            out += (বর্ণ)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (বর্ণ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (বর্ণ)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    শূন্য parseUnicodeEscapeToUtf8(বর্ণ*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        যদি (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            যদি (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t নিচের = parseUnicodeEscape(ptr);
            যদি (নিচের < 0xDC00 || নিচের > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (নিচের - 0xDC00u);
        }
        অন্যথা যদি (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    বর্ণ* parse_চাবি( বর্ণ* ptr, File_গ্রন্থি& collection, File_গ্রন্থি& মান_placeholder )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        বর্ণ * beg = ptr + 1;
        std::string চাবি_name;
        কুরু {
            যদি (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                যদি (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, চাবি_name);
                    অনুবর্ত;
                }
                চাবি_name += *ptr;
            } অন্যথা যদি (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                চাবি_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } যাবত্( cv_isprint(*ptr) && *ptr != '"' );

        যদি( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        যদি( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        মান_placeholder = fs->add_গ্রন্থি(collection, চাবি_name, File_গ্রন্থি::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        যদি( !ptr || !*ptr )
            প্রত্যাগম 0;

        যদি( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        প্রত্যাগম ++ptr;
    }

    তার্কিক getBase64Row(বর্ণ* ptr, পূর্ণাঙ্ক /*indent*/, বর্ণ* &beg, বর্ণ* &end)
    {
        beg = end = ptr;
        যদি( !ptr || !*ptr )
            প্রত্যাগম অসত্য;

        // find end of the row
        যাবত্( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        যদি ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        প্রত্যাগম সত্য;
    }

    বর্ণ* parse_মান( বর্ণ* ptr, File_গ্রন্থি& গ্রন্থি )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        যদি( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        যদি( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            বর্ণ * beg = ptr;
            size_t len = 0u;
            প্রতি ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            যদি ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, গ্রন্থি);

                যদি ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                অন্যথা
                    ptr++;
            }
            অন্যথা
            {   /**************** normal string ****************/
                পূর্ণাঙ্ক i = 0, sz;

                ptr = beg;
                তার্কিক is_matching = অসত্য;
                যাবত্ ( !is_matching )
                {
                    বিকল্প ( *ptr )
                    {
                        পক্ষ '\\':
                        {
                            sz = (পূর্ণাঙ্ক)(ptr - beg);
                            যদি( sz > 0 )
                            {
                                যদি (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            যদি (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            বিকল্প ( *ptr )
                            {
                            পক্ষ '\\':
                            পক্ষ '\"':
                            পক্ষ '\'': { buf[i++] = *ptr; ভঙ্গ; }
                            পক্ষ 'n' : { buf[i++] = '\n'; ভঙ্গ; }
                            পক্ষ 'r' : { buf[i++] = '\r'; ভঙ্গ; }
                            পক্ষ 't' : { buf[i++] = '\t'; ভঙ্গ; }
                            পক্ষ 'b' : { buf[i++] = '\b'; ভঙ্গ; }
                            পক্ষ 'f' : { buf[i++] = '\f'; ভঙ্গ; }
                            পক্ষ 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                যদি (i + (পূর্ণাঙ্ক)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (পূর্ণাঙ্ক)utf8.size();
                                beg = ptr;
                                অনুবর্ত;
                            }
                            স্বভাব  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ভঙ্গ;
                            }
                            ptr++;
                            beg = ptr;
                            ভঙ্গ;
                        }
                        পক্ষ '\0':
                        {
                            sz = (পূর্ণাঙ্ক)(ptr - beg);
                            যদি( sz > 0 )
                            {
                                যদি (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            যদি ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ভঙ্গ;
                        }
                        পক্ষ '\"':
                        {
                            sz = (পূর্ণাঙ্ক)(ptr - beg);
                            যদি( sz > 0 )
                            {
                                যদি (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = সত্য;
                            ভঙ্গ;
                        }
                        পক্ষ '\n':
                        পক্ষ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ভঙ্গ;
                        }
                        স্বভাব:
                        {
                            ptr++;
                            ভঙ্গ;
                        }
                    }
                }

                যদি ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                অন্যথা
                    ptr++;

                গ্রন্থি.set_মান(File_গ্রন্থি::STRING, buf, i);
            }
        }
        অন্যথা যদি ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            বর্ণ * beg = ptr;
            যদি ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            যাবত্( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            যদি (*ptr == '.' || *ptr == 'e')
            {
                দ্বিগুণ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                গ্রন্থি.set_মান(File_গ্রন্থি::REAL, &fval);
            }
            অন্যথা
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
            }

            যদি ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        অন্যথা
        {    /**************** other data ****************/
            ধ্রুব বর্ণ* beg = ptr;
            পূর্ণাঙ্ক len = 0;
            প্রতি ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            যদি( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            অন্যথা যদি( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
            }
            অন্যথা
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        প্রত্যাগম ptr;
    }

    বর্ণ* parseSeq( বর্ণ* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণাঙ্ক depth = 0 )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        যদি( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        যদি ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        অন্যথা
            ptr++;

        fs->convertToCollection(File_গ্রন্থি::SEQ, গ্রন্থি);

        প্রতি (;;)
        {
            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                ভঙ্গ;

            যদি ( *ptr != ']' )
            {
                File_গ্রন্থি child = fs->add_গ্রন্থি(গ্রন্থি, std::string(), File_গ্রন্থি::NONE );

                যদি ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                অন্যথা যদি ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                অন্যথা
                    ptr = parse_মান( ptr, child );
            }

            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                ভঙ্গ;

            যদি ( *ptr == ',' )
                ptr++;
            অন্যথা যদি ( *ptr == ']' )
                ভঙ্গ;
            অন্যথা
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        যদি (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        যদি ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        অন্যথা
            ptr++;

        fs->finalizeCollection(গ্রন্থি);
        প্রত্যাগম ptr;
    }

    বর্ণ* parseMap( বর্ণ* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণাঙ্ক depth = 0 )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        যদি( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        যদি ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        অন্যথা
            ptr++;

        fs->convertToCollection(File_গ্রন্থি::MAP, গ্রন্থি);

        প্রতি( ;; )
        {
            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                ভঙ্গ;

            যদি ( *ptr == '"' )
            {
                File_গ্রন্থি child;
                ptr = parse_চাবি( ptr, গ্রন্থি, child );
                যদি( !ptr || !*ptr )
                    ভঙ্গ;
                ptr = skipSpaces( ptr );
                যদি( !ptr || !*ptr )
                    ভঙ্গ;

                যদি ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                অন্যথা যদি ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                অন্যথা
                    ptr = parse_মান( ptr, child );
            }

            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                ভঙ্গ;

            যদি ( *ptr == ',' )
                ptr++;
            অন্যথা যদি ( *ptr == '}' )
                ভঙ্গ;
            অন্যথা
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        যদি (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        যদি ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        অন্যথা
            ptr++;

        fs->finalizeCollection(গ্রন্থি);
        প্রত্যাগম ptr;
    }

    তার্কিক parse( বর্ণ* ptr )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        যদি ( !ptr || !*ptr )
            প্রত্যাগম অসত্য;

        File_গ্রন্থি মূল_collection(fs->getFS(), 0, 0);

        যদি( *ptr == '{' )
        {
            File_গ্রন্থি মূল_গ্রন্থি = fs->add_গ্রন্থি(মূল_collection, std::string(), File_গ্রন্থি::MAP);
            parseMap( ptr, মূল_গ্রন্থি );
        }
        অন্যথা যদি ( *ptr == '[' )
        {
            File_গ্রন্থি মূল_গ্রন্থি = fs->add_গ্রন্থি(মূল_collection, std::string(), File_গ্রন্থি::SEQ);
            parseSeq( ptr, মূল_গ্রন্থি );
        }
        অন্যথা
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        প্রত্যাগম সত্য;
    }

    FileStorage_API* fs;
    বর্ণ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    প্রত্যাগম makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    প্রত্যাগম makePtr<JSONParser>(fs);
}

}
