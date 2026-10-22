// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

فضاینام cv
{

رده JSONEmitter : عمومی FileStorageEmitter
{
عمومی:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~JSONEmitter() {}

    FStructData startWriteStruct( ثابت FStructData& parent, ثابت نویسه* کلید,
                                  صحیح struct_flags, ثابت نویسه* type_name=0 )
    {
        نویسه data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_گره::TYPE_MASK|File_گره::FLOW)) | File_گره::EMPTY;
        اگر( !File_گره::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        اگر( type_name && *type_name == '\0' )
            type_name = 0;

        منطقی is_real_collection = درست;
        اگر (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_گره::STR;
            data[0] = '\0';
            is_real_collection = نادرست;
        }

        اگر ( is_real_collection )
        {
            نویسه c = File_گره::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( کلید, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        بازگشت current_struct;
    }

    تهی endWriteStruct(ثابت FStructData& current_struct)
    {
        صحیح struct_flags = current_struct.flags;

        اگر (File_گره::isCollection(struct_flags)) {
            اگر (!File_گره::isFlow(struct_flags)) {
                اگر (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    نویسه *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            نویسه *ptr = fs->bufferPtr();
            اگر (ptr > fs->bufferStart() + current_struct.indent && !File_گره::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_گره::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    تهی اكتب(ثابت نویسه* کلید, صحیح value)
    {
        نویسه buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10 ));
    }

    تهی اكتب(ثابت نویسه* کلید, int64_t value)
    {
        نویسه buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10, درست ));
    }

    تهی اكتب( ثابت نویسه* کلید, دوگانه value )
    {
        نویسه buf[128];
        writeScalar( کلید, fs::doubleToString( buf, اندازه(buf), value, درست ));
    }

    تهی اكتب(ثابت نویسه* کلید, ثابت نویسه* str, منطقی quote)
    {
        نویسه buf[CV_FS_MAX_LEN*6+16];
        نویسه* data = (نویسه*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)strlen(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحیح need_quote = 1;
            data = buf;
            *data++ = '\"';
            برای( i = 0; i < len; i++ )
            {
                نویسه c = str[i];
                ثابت unsigned نویسه byte = static_cast<unsigned نویسه>(c);

                انتخاب ( c )
                {
                حالت '\\':
                حالت '\"': { *data++ = '\\'; *data++ = c;   بشکن; }
                حالت '\n': { *data++ = '\\'; *data++ = 'n'; بشکن; }
                حالت '\r': { *data++ = '\\'; *data++ = 'r'; بشکن; }
                حالت '\t': { *data++ = '\\'; *data++ = 't'; بشکن; }
                حالت '\b': { *data++ = '\\'; *data++ = 'b'; بشکن; }
                حالت '\f': { *data++ = '\\'; *data++ = 'f'; بشکن; }
                پیشفرض  :
                    اگر (byte < 0x20)
                    {
                        ایستا ثابت نویسه hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    وگرنه
                        *data++ = c;
                    بشکن;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( کلید, data);
    }

    تهی writeScalar(ثابت نویسه* کلید, ثابت نویسه* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(نادرست);
        اگر ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وگرنه اگر ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t کلید_len = 0u;
        اگر( کلید && *کلید == '\0' )
            کلید = 0;
        اگر ( کلید )
        {
            کلید_len = strlen(کلید);
            اگر ( کلید_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            وگرنه اگر ( static_cast<صحیح>(کلید_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        اگر ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;
        اگر( File_گره::isCollection(struct_flags) )
        {
            اگر ( (File_گره::isMap(struct_flags) ^ (کلید != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } وگرنه {
            fs->setNonEmpty();
            struct_flags = File_گره::EMPTY | (کلید ? File_گره::MAP : File_گره::SEQ);
        }

        // start to write
        نویسه* ptr = 0;

        اگر( File_گره::isFlow(struct_flags) )
        {
            صحیح new_offset;
            ptr = fs->bufferPtr();
            اگر( !File_گره::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<صحیح>(ptr - fs->bufferStart() + کلید_len + data_len);
            اگر( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            وگرنه
                *ptr++ = ' ';
        }
        وگرنه
        {
            اگر ( !File_گره::isEmptyCollection(struct_flags) )
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

        اگر( کلید )
        {
            اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحیح>(کلید_len) );
            *ptr++ = '\"';

            برای( size_t i = 0u; i < کلید_len; i++ )
            {
                نویسه c = کلید[i];

                ptr[i] = c;
                اگر( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += کلید_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        اگر( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحیح>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_گره::EMPTY;
    }

    تهی writeComment(ثابت نویسه* comment, منطقی eol_comment)
    {
        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحیح len = static_cast<صحیح>(strlen(comment));
        نویسه* ptr = fs->bufferPtr();
        ثابت نویسه* eol = strchr(comment, '\n');
        منطقی multiline = eol != 0;

        اگر( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        وگرنه
            *ptr++ = ' ';

        وقتی( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            اگر( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحیح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وگرنه
            {
                len = (صحیح)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    تهی startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محافظتشده:
    FileStorage_API* fs;
};

رده JSONParser : عمومی FileStorageParser
{
عمومی:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~JSONParser() {}

    نویسه* skipSpaces( نویسه* ptr )
    {
        منطقی is_eof = نادرست;
        منطقی is_completed = نادرست;

        وقتی ( is_eof == نادرست && is_completed == نادرست )
        {
            اگر (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            انتخاب ( *ptr )
            {
                /* comment */
                حالت '/' : {
                    ptr++;
                    اگر ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        اگر( !ptr || !*ptr ) { is_eof = درست; بشکن; }
                    }

                    اگر ( *ptr == '/' )
                    {
                        وقتی ( *ptr != '\n' && *ptr != '\r' )
                        {
                            اگر ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                اگر( !ptr || !*ptr ) { is_eof = درست; بشکن; }
                            }
                            وگرنه
                            {
                                ptr++;
                            }
                        }
                    }
                    وگرنه اگر ( *ptr == '*' )
                    {
                        ptr++;
                        برای (;;)
                        {
                            اگر ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                اگر( !ptr || !*ptr ) { is_eof = درست; بشکن; }
                            }
                            وگرنه اگر ( *ptr == '*' )
                            {
                                ptr++;
                                اگر ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    اگر( !ptr || !*ptr ) { is_eof = درست; بشکن; }
                                }
                                اگر ( *ptr == '/' )
                                {
                                    ptr++;
                                    بشکن;
                                }
                            }
                            وگرنه
                            {
                                ptr++;
                            }
                        }
                    }
                    وگرنه
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } بشکن;
                    /* whitespace */
                حالت '\t':
                حالت ' ' : {
                    ptr++;
                } بشکن;
                    /* newline || end mark */
                حالت '\0':
                حالت '\n':
                حالت '\r': {
                    ptr = fs->gets();
                    اگر( !ptr || !*ptr ) { is_eof = درست; بشکن; }
                } بشکن;
                    /* other character */
                پیشفرض: {
                    اگر( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = درست;
                } بشکن;
            }
        }

        اگر ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            اگر( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        بازگشت ptr;
    }

    uint32_t parseUnicodeEscape(نویسه*& ptr)
    {
        uint32_t codepoint = 0;
        برای (صحیح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            نویسه hex = *ptr;
            uint32_t digit = 0;
            اگر      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            وگرنه اگر (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            وگرنه اگر (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            وگرنه CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        بازگشت codepoint;
    }

    ایستا تهی appendUtf8(std::string& out, uint32_t codepoint)
    {
        اگر (codepoint < 0x80) {
            out += (نویسه)codepoint;
        } وگرنه اگر (codepoint < 0x800) {
            out += (نویسه)(0xC0 | (codepoint >> 6));
            out += (نویسه)(0x80 | (codepoint & 0x3F));
        } وگرنه اگر (codepoint < 0x10000) {
            out += (نویسه)(0xE0 | (codepoint >> 12));
            out += (نویسه)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (نویسه)(0x80 | (codepoint & 0x3F));
        } وگرنه {
            out += (نویسه)(0xF0 | (codepoint >> 18));
            out += (نویسه)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (نویسه)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (نویسه)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    تهی parseUnicodeEscapeToUtf8(نویسه*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        اگر (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            اگر (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t کم = parseUnicodeEscape(ptr);
            اگر (کم < 0xDC00 || کم > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (کم - 0xDC00u);
        }
        وگرنه اگر (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    نویسه* parse_کلید( نویسه* ptr, File_گره& collection, File_گره& مقدار_placeholder )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        نویسه * beg = ptr + 1;
        std::string کلید_name;
        انجامبده {
            اگر (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                اگر (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, کلید_name);
                    ادامه;
                }
                کلید_name += *ptr;
            } وگرنه اگر (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                کلید_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } وقتی( cv_isprint(*ptr) && *ptr != '"' );

        اگر( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        اگر( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        مقدار_placeholder = fs->add_گره(collection, کلید_name, File_گره::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        اگر( !ptr || !*ptr )
            بازگشت 0;

        اگر( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        بازگشت ++ptr;
    }

    منطقی getBase64Row(نویسه* ptr, صحیح /*indent*/, نویسه* &beg, نویسه* &end)
    {
        beg = end = ptr;
        اگر( !ptr || !*ptr )
            بازگشت نادرست;

        // find end of the row
        وقتی( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        اگر ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        بازگشت درست;
    }

    نویسه* parse_مقدار( نویسه* ptr, File_گره& گره )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        اگر( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        اگر( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            نویسه * beg = ptr;
            size_t len = 0u;
            برای ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            اگر ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, گره);

                اگر ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                وگرنه
                    ptr++;
            }
            وگرنه
            {   /**************** normal string ****************/
                صحیح i = 0, sz;

                ptr = beg;
                منطقی is_matching = نادرست;
                وقتی ( !is_matching )
                {
                    انتخاب ( *ptr )
                    {
                        حالت '\\':
                        {
                            sz = (صحیح)(ptr - beg);
                            اگر( sz > 0 )
                            {
                                اگر (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            اگر (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            انتخاب ( *ptr )
                            {
                            حالت '\\':
                            حالت '\"':
                            حالت '\'': { buf[i++] = *ptr; بشکن; }
                            حالت 'n' : { buf[i++] = '\n'; بشکن; }
                            حالت 'r' : { buf[i++] = '\r'; بشکن; }
                            حالت 't' : { buf[i++] = '\t'; بشکن; }
                            حالت 'b' : { buf[i++] = '\b'; بشکن; }
                            حالت 'f' : { buf[i++] = '\f'; بشکن; }
                            حالت 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                اگر (i + (صحیح)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (صحیح)utf8.size();
                                beg = ptr;
                                ادامه;
                            }
                            پیشفرض  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            بشکن;
                            }
                            ptr++;
                            beg = ptr;
                            بشکن;
                        }
                        حالت '\0':
                        {
                            sz = (صحیح)(ptr - beg);
                            اگر( sz > 0 )
                            {
                                اگر (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            اگر ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            بشکن;
                        }
                        حالت '\"':
                        {
                            sz = (صحیح)(ptr - beg);
                            اگر( sz > 0 )
                            {
                                اگر (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = درست;
                            بشکن;
                        }
                        حالت '\n':
                        حالت '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            بشکن;
                        }
                        پیشفرض:
                        {
                            ptr++;
                            بشکن;
                        }
                    }
                }

                اگر ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                وگرنه
                    ptr++;

                گره.set_مقدار(File_گره::STRING, buf, i);
            }
        }
        وگرنه اگر ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            نویسه * beg = ptr;
            اگر ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            وقتی( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            اگر (*ptr == '.' || *ptr == 'e')
            {
                دوگانه fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                گره.set_مقدار(File_گره::REAL, &fval);
            }
            وگرنه
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                گره.set_مقدار(File_گره::INT, &ival);
            }

            اگر ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        وگرنه
        {    /**************** other data ****************/
            ثابت نویسه* beg = ptr;
            صحیح len = 0;
            برای ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            اگر( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            وگرنه اگر( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                گره.set_مقدار(File_گره::INT, &ival);
            }
            وگرنه
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        بازگشت ptr;
    }

    نویسه* parseSeq( نویسه* ptr, File_گره& گره, صحیح depth = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        اگر( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        اگر ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        وگرنه
            ptr++;

        fs->convertToCollection(File_گره::SEQ, گره);

        برای (;;)
        {
            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                بشکن;

            اگر ( *ptr != ']' )
            {
                File_گره child = fs->add_گره(گره, std::string(), File_گره::NONE );

                اگر ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                وگرنه اگر ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                وگرنه
                    ptr = parse_مقدار( ptr, child );
            }

            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                بشکن;

            اگر ( *ptr == ',' )
                ptr++;
            وگرنه اگر ( *ptr == ']' )
                بشکن;
            وگرنه
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        وگرنه
            ptr++;

        fs->finalizeCollection(گره);
        بازگشت ptr;
    }

    نویسه* parseMap( نویسه* ptr, File_گره& گره, صحیح depth = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        اگر ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        وگرنه
            ptr++;

        fs->convertToCollection(File_گره::MAP, گره);

        برای( ;; )
        {
            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                بشکن;

            اگر ( *ptr == '"' )
            {
                File_گره child;
                ptr = parse_کلید( ptr, گره, child );
                اگر( !ptr || !*ptr )
                    بشکن;
                ptr = skipSpaces( ptr );
                اگر( !ptr || !*ptr )
                    بشکن;

                اگر ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                وگرنه اگر ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                وگرنه
                    ptr = parse_مقدار( ptr, child );
            }

            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                بشکن;

            اگر ( *ptr == ',' )
                ptr++;
            وگرنه اگر ( *ptr == '}' )
                بشکن;
            وگرنه
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        وگرنه
            ptr++;

        fs->finalizeCollection(گره);
        بازگشت ptr;
    }

    منطقی parse( نویسه* ptr )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        اگر ( !ptr || !*ptr )
            بازگشت نادرست;

        File_گره ریشه_collection(fs->getFS(), 0, 0);

        اگر( *ptr == '{' )
        {
            File_گره ریشه_گره = fs->add_گره(ریشه_collection, std::string(), File_گره::MAP);
            parseMap( ptr, ریشه_گره );
        }
        وگرنه اگر ( *ptr == '[' )
        {
            File_گره ریشه_گره = fs->add_گره(ریشه_collection, std::string(), File_گره::SEQ);
            parseSeq( ptr, ریشه_گره );
        }
        وگرنه
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        بازگشت درست;
    }

    FileStorage_API* fs;
    نویسه buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    بازگشت makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    بازگشت makePtr<JSONParser>(fs);
}

}
