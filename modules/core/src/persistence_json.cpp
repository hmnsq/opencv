// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

نطاق cv
{

صنف JSONEmitter : عام FileStorageEmitter
{
عام:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~JSONEmitter() {}

    FStructData startWriteStruct( ثابت FStructData& parent, ثابت حرف* key,
                                  صحيح struct_flags, ثابت حرف* type_name=0 )
    {
        حرف data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ܐܢ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ܐܢ( type_name && *type_name == '\0' )
            type_name = 0;

        منطقي is_real_collection = ܫܪܪܐ;
        ܐܢ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = ܕܓܠܐ;
        }

        ܐܢ ( is_real_collection )
        {
            حرف c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        ܦܢܐ current_struct;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        صحيح struct_flags = current_struct.flags;

        ܐܢ (FileNode::isCollection(struct_flags)) {
            ܐܢ (!FileNode::isFlow(struct_flags)) {
                ܐܢ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    حرف *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            حرف *ptr = fs->bufferPtr();
            ܐܢ (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    فراغ اكتب(ثابت حرف* key, صحيح value)
    {
        حرف buf[128];
        writeScalar( key, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* key, int64_t value)
    {
        حرف buf[128];
        writeScalar( key, fs::itoa( value, buf, 10, ܫܪܪܐ ));
    }

    فراغ اكتب( ثابت حرف* key, مزدوج value )
    {
        حرف buf[128];
        writeScalar( key, fs::doubleToString( buf, حجم(buf), value, ܫܪܪܐ ));
    }

    فراغ اكتب(ثابت حرف* key, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        ܐܢ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        ܐܢ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ܐܢ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = 1;
            data = buf;
            *data++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];
                ثابت unsigned حرف byte = static_cast<unsigned حرف>(c);

                بدل ( c )
                {
                حالة '\\':
                حالة '\"': { *data++ = '\\'; *data++ = c;   اكسر; }
                حالة '\n': { *data++ = '\\'; *data++ = 'n'; اكسر; }
                حالة '\r': { *data++ = '\\'; *data++ = 'r'; اكسر; }
                حالة '\t': { *data++ = '\\'; *data++ = 't'; اكسر; }
                حالة '\b': { *data++ = '\\'; *data++ = 'b'; اكسر; }
                حالة '\f': { *data++ = '\\'; *data++ = 'f'; اكسر; }
                افتراضي  :
                    ܐܢ (byte < 0x20)
                    {
                        ساكن ثابت حرف hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    ܐܚܪܢܐ
                        *data++ = c;
                    اكسر;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    فراغ writeScalar(ثابت حرف* key, ثابت حرف* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ܕܓܠܐ);
        ܐܢ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ܐܚܪܢܐ ܐܢ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        ܐܢ( key && *key == '\0' )
            key = 0;
        ܐܢ ( key )
        {
            key_len = strlen(key);
            ܐܢ ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ܐܚܪܢܐ ܐܢ ( static_cast<صحيح>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ܐܢ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;
        ܐܢ( FileNode::isCollection(struct_flags) )
        {
            ܐܢ ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ܐܚܪܢܐ {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        حرف* ptr = 0;

        ܐܢ( FileNode::isFlow(struct_flags) )
        {
            صحيح new_offset;
            ptr = fs->bufferPtr();
            ܐܢ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<صحيح>(ptr - fs->bufferStart() + key_len + data_len);
            ܐܢ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ܐܚܪܢܐ
                *ptr++ = ' ';
        }
        ܐܚܪܢܐ
        {
            ܐܢ ( !FileNode::isEmptyCollection(struct_flags) )
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

        ܐܢ( key )
        {
            ܐܢ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحيح>(key_len) );
            *ptr++ = '\"';

            لكل( size_t i = 0u; i < key_len; i++ )
            {
                حرف c = key[i];

                ptr[i] = c;
                ܐܢ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ܐܢ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحيح>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        ܐܢ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = static_cast<صحيح>(strlen(comment));
        حرف* ptr = fs->bufferPtr();
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;

        ܐܢ( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ܐܚܪܢܐ
            *ptr++ = ' ';

        ܟܡܐ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ܐܢ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ܐܚܪܢܐ
            {
                len = (صحيح)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محمي:
    FileStorage_API* fs;
};

صنف JSONParser : عام FileStorageParser
{
عام:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~JSONParser() {}

    حرف* skipSpaces( حرف* ptr )
    {
        منطقي is_eof = ܕܓܠܐ;
        منطقي is_completed = ܕܓܠܐ;

        ܟܡܐ ( is_eof == ܕܓܠܐ && is_completed == ܕܓܠܐ )
        {
            ܐܢ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            بدل ( *ptr )
            {
                /* comment */
                حالة '/' : {
                    ptr++;
                    ܐܢ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ܐܢ( !ptr || !*ptr ) { is_eof = ܫܪܪܐ; اكسر; }
                    }

                    ܐܢ ( *ptr == '/' )
                    {
                        ܟܡܐ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ܐܢ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ܐܢ( !ptr || !*ptr ) { is_eof = ܫܪܪܐ; اكسر; }
                            }
                            ܐܚܪܢܐ
                            {
                                ptr++;
                            }
                        }
                    }
                    ܐܚܪܢܐ ܐܢ ( *ptr == '*' )
                    {
                        ptr++;
                        لكل (;;)
                        {
                            ܐܢ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ܐܢ( !ptr || !*ptr ) { is_eof = ܫܪܪܐ; اكسر; }
                            }
                            ܐܚܪܢܐ ܐܢ ( *ptr == '*' )
                            {
                                ptr++;
                                ܐܢ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ܐܢ( !ptr || !*ptr ) { is_eof = ܫܪܪܐ; اكسر; }
                                }
                                ܐܢ ( *ptr == '/' )
                                {
                                    ptr++;
                                    اكسر;
                                }
                            }
                            ܐܚܪܢܐ
                            {
                                ptr++;
                            }
                        }
                    }
                    ܐܚܪܢܐ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } اكسر;
                    /* whitespace */
                حالة '\t':
                حالة ' ' : {
                    ptr++;
                } اكسر;
                    /* newline || end mark */
                حالة '\0':
                حالة '\n':
                حالة '\r': {
                    ptr = fs->gets();
                    ܐܢ( !ptr || !*ptr ) { is_eof = ܫܪܪܐ; اكسر; }
                } اكسر;
                    /* other character */
                افتراضي: {
                    ܐܢ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = ܫܪܪܐ;
                } اكسر;
            }
        }

        ܐܢ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ܐܢ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ܦܢܐ ptr;
    }

    uint32_t parseUnicodeEscape(حرف*& ptr)
    {
        uint32_t codepoint = 0;
        لكل (صحيح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            حرف hex = *ptr;
            uint32_t digit = 0;
            ܐܢ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ܐܚܪܢܐ ܐܢ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ܐܚܪܢܐ ܐܢ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ܐܚܪܢܐ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ܦܢܐ codepoint;
    }

    ساكن فراغ appendUtf8(std::string& out, uint32_t codepoint)
    {
        ܐܢ (codepoint < 0x80) {
            out += (حرف)codepoint;
        } ܐܚܪܢܐ ܐܢ (codepoint < 0x800) {
            out += (حرف)(0xC0 | (codepoint >> 6));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } ܐܚܪܢܐ ܐܢ (codepoint < 0x10000) {
            out += (حرف)(0xE0 | (codepoint >> 12));
            out += (حرف)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } ܐܚܪܢܐ {
            out += (حرف)(0xF0 | (codepoint >> 18));
            out += (حرف)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (حرف)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    فراغ parseUnicodeEscapeToUtf8(حرف*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ܐܢ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ܐܢ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            ܐܢ (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        ܐܚܪܢܐ ܐܢ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    حرف* parseKey( حرف* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ܐܢ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        حرف * beg = ptr + 1;
        std::string key_name;
        ܥܒܕ {
            ܐܢ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ܐܢ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    استمر;
                }
                key_name += *ptr;
            } ܐܚܪܢܐ ܐܢ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ܟܡܐ( cv_isprint(*ptr) && *ptr != '"' );

        ܐܢ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ܐܢ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ܐܢ( !ptr || !*ptr )
            ܦܢܐ 0;

        ܐܢ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ܦܢܐ ++ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr;
        ܐܢ( !ptr || !*ptr )
            ܦܢܐ ܕܓܠܐ;

        // find end of the row
        ܟܡܐ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ܐܢ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ܦܢܐ ܫܪܪܐ;
    }

    حرف* parseValue( حرف* ptr, FileNode& node )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ܐܢ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ܐܢ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            حرف * beg = ptr;
            size_t len = 0u;
            لكل ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ܐܢ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                ܐܢ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ܐܚܪܢܐ
                    ptr++;
            }
            ܐܚܪܢܐ
            {   /**************** normal string ****************/
                صحيح i = 0, sz;

                ptr = beg;
                منطقي is_matching = ܕܓܠܐ;
                ܟܡܐ ( !is_matching )
                {
                    بدل ( *ptr )
                    {
                        حالة '\\':
                        {
                            sz = (صحيح)(ptr - beg);
                            ܐܢ( sz > 0 )
                            {
                                ܐܢ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ܐܢ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            بدل ( *ptr )
                            {
                            حالة '\\':
                            حالة '\"':
                            حالة '\'': { buf[i++] = *ptr; اكسر; }
                            حالة 'n' : { buf[i++] = '\n'; اكسر; }
                            حالة 'r' : { buf[i++] = '\r'; اكسر; }
                            حالة 't' : { buf[i++] = '\t'; اكسر; }
                            حالة 'b' : { buf[i++] = '\b'; اكسر; }
                            حالة 'f' : { buf[i++] = '\f'; اكسر; }
                            حالة 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                ܐܢ (i + (صحيح)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (صحيح)utf8.size();
                                beg = ptr;
                                استمر;
                            }
                            افتراضي  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            اكسر;
                            }
                            ptr++;
                            beg = ptr;
                            اكسر;
                        }
                        حالة '\0':
                        {
                            sz = (صحيح)(ptr - beg);
                            ܐܢ( sz > 0 )
                            {
                                ܐܢ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ܐܢ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            اكسر;
                        }
                        حالة '\"':
                        {
                            sz = (صحيح)(ptr - beg);
                            ܐܢ( sz > 0 )
                            {
                                ܐܢ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = ܫܪܪܐ;
                            اكسر;
                        }
                        حالة '\n':
                        حالة '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            اكسر;
                        }
                        افتراضي:
                        {
                            ptr++;
                            اكسر;
                        }
                    }
                }

                ܐܢ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ܐܚܪܢܐ
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        ܐܚܪܢܐ ܐܢ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            حرف * beg = ptr;
            ܐܢ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ܟܡܐ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ܐܢ (*ptr == '.' || *ptr == 'e')
            {
                مزدوج fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            ܐܚܪܢܐ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            ܐܢ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ܐܚܪܢܐ
        {    /**************** other data ****************/
            ثابت حرف* beg = ptr;
            صحيح len = 0;
            لكل ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ܐܢ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ܐܚܪܢܐ ܐܢ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            ܐܚܪܢܐ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ܦܢܐ ptr;
    }

    حرف* parseSeq( حرف* ptr, FileNode& node, صحيح depth = 0 )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ܐܢ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ܐܢ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ܐܚܪܢܐ
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        لكل (;;)
        {
            ptr = skipSpaces( ptr );
            ܐܢ( !ptr || !*ptr )
                اكسر;

            ܐܢ ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                ܐܢ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ܐܚܪܢܐ ܐܢ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ܐܚܪܢܐ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ܐܢ( !ptr || !*ptr )
                اكسر;

            ܐܢ ( *ptr == ',' )
                ptr++;
            ܐܚܪܢܐ ܐܢ ( *ptr == ']' )
                اكسر;
            ܐܚܪܢܐ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ܐܢ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ܐܚܪܢܐ
            ptr++;

        fs->finalizeCollection(node);
        ܦܢܐ ptr;
    }

    حرف* parseMap( حرف* ptr, FileNode& node, صحيح depth = 0 )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ܐܢ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ܐܢ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ܐܚܪܢܐ
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        لكل( ;; )
        {
            ptr = skipSpaces( ptr );
            ܐܢ( !ptr || !*ptr )
                اكسر;

            ܐܢ ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                ܐܢ( !ptr || !*ptr )
                    اكسر;
                ptr = skipSpaces( ptr );
                ܐܢ( !ptr || !*ptr )
                    اكسر;

                ܐܢ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ܐܚܪܢܐ ܐܢ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ܐܚܪܢܐ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ܐܢ( !ptr || !*ptr )
                اكسر;

            ܐܢ ( *ptr == ',' )
                ptr++;
            ܐܚܪܢܐ ܐܢ ( *ptr == '}' )
                اكسر;
            ܐܚܪܢܐ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ܐܢ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ܐܚܪܢܐ
            ptr++;

        fs->finalizeCollection(node);
        ܦܢܐ ptr;
    }

    منطقي parse( حرف* ptr )
    {
        ܐܢ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ܐܢ ( !ptr || !*ptr )
            ܦܢܐ ܕܓܠܐ;

        FileNode root_collection(fs->getFS(), 0, 0);

        ܐܢ( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        ܐܚܪܢܐ ܐܢ ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        ܐܚܪܢܐ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ܦܢܐ ܫܪܪܐ;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ܦܢܐ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ܦܢܐ makePtr<JSONParser>(fs);
}

}
