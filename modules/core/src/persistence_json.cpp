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

    FStructData startWriteStruct( ثابت FStructData& parent, ثابت حرف* کلیل,
                                  صحيح struct_flags, ثابت حرف* type_name=0 )
    {
        حرف data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_گرێ::TYPE_MASK|File_گرێ::FLOW)) | File_گرێ::EMPTY;
        إذا( !File_گرێ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا( type_name && *type_name == '\0' )
            type_name = 0;

        منطقي is_real_collection = true;
        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_گرێ::STR;
            data[0] = '\0';
            is_real_collection = خطأ;
        }

        إذا ( is_real_collection )
        {
            حرف c = File_گرێ::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( کلیل, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        أرجع current_struct;
    }

    فراغ endWriteStruct(ثابت FStructData& current_struct)
    {
        صحيح struct_flags = current_struct.flags;

        إذا (File_گرێ::isCollection(struct_flags)) {
            إذا (!File_گرێ::isFlow(struct_flags)) {
                إذا (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
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
            إذا (ptr > fs->bufferStart() + current_struct.indent && !File_گرێ::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_گرێ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    فراغ اكتب(ثابت حرف* کلیل, صحيح value)
    {
        حرف buf[128];
        writeScalar( کلیل, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* کلیل, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلیل, fs::itoa( value, buf, 10, true ));
    }

    فراغ اكتب( ثابت حرف* کلیل, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلیل, fs::doubleToString( buf, حجم(buf), value, true ));
    }

    فراغ اكتب(ثابت حرف* کلیل, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* data = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)strlen(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
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
                    إذا (byte < 0x20)
                    {
                        ساكن ثابت حرف hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    وإلا
                        *data++ = c;
                    اكسر;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( کلیل, data);
    }

    فراغ writeScalar(ثابت حرف* کلیل, ثابت حرف* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t کلیل_len = 0u;
        إذا( کلیل && *کلیل == '\0' )
            کلیل = 0;
        إذا ( کلیل )
        {
            کلیل_len = strlen(کلیل);
            إذا ( کلیل_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            وإلا إذا ( static_cast<صحيح>(کلیل_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        إذا ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;
        إذا( File_گرێ::isCollection(struct_flags) )
        {
            إذا ( (File_گرێ::isMap(struct_flags) ^ (کلیل != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } وإلا {
            fs->setNonEmpty();
            struct_flags = File_گرێ::EMPTY | (کلیل ? File_گرێ::MAP : File_گرێ::SEQ);
        }

        // start to write
        حرف* ptr = 0;

        إذا( File_گرێ::isFlow(struct_flags) )
        {
            صحيح new_offset;
            ptr = fs->bufferPtr();
            إذا( !File_گرێ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<صحيح>(ptr - fs->bufferStart() + کلیل_len + data_len);
            إذا( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            إذا ( !File_گرێ::isEmptyCollection(struct_flags) )
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

        إذا( کلیل )
        {
            إذا( !cv_isalpha(کلیل[0]) && کلیل[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحيح>(کلیل_len) );
            *ptr++ = '\"';

            لكل( size_t i = 0u; i < کلیل_len; i++ )
            {
                حرف c = کلیل[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += کلیل_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        إذا( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<صحيح>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_گرێ::EMPTY;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = static_cast<صحيح>(strlen(comment));
        حرف* ptr = fs->bufferPtr();
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;

        إذا( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        وإلا
            *ptr++ = ' ';

        طالما( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            إذا( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (صحيح)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
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
        منطقي is_eof = خطأ;
        منطقي is_completed = خطأ;

        طالما ( is_eof == خطأ && is_completed == خطأ )
        {
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            بدل ( *ptr )
            {
                /* comment */
                حالة '/' : {
                    ptr++;
                    إذا ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        إذا( !ptr || !*ptr ) { is_eof = true; اكسر; }
                    }

                    إذا ( *ptr == '/' )
                    {
                        طالما ( *ptr != '\n' && *ptr != '\r' )
                        {
                            إذا ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                إذا( !ptr || !*ptr ) { is_eof = true; اكسر; }
                            }
                            وإلا
                            {
                                ptr++;
                            }
                        }
                    }
                    وإلا إذا ( *ptr == '*' )
                    {
                        ptr++;
                        لكل (;;)
                        {
                            إذا ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                إذا( !ptr || !*ptr ) { is_eof = true; اكسر; }
                            }
                            وإلا إذا ( *ptr == '*' )
                            {
                                ptr++;
                                إذا ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    إذا( !ptr || !*ptr ) { is_eof = true; اكسر; }
                                }
                                إذا ( *ptr == '/' )
                                {
                                    ptr++;
                                    اكسر;
                                }
                            }
                            وإلا
                            {
                                ptr++;
                            }
                        }
                    }
                    وإلا
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
                    إذا( !ptr || !*ptr ) { is_eof = true; اكسر; }
                } اكسر;
                    /* other character */
                افتراضي: {
                    إذا( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } اكسر;
            }
        }

        إذا ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            إذا( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        أرجع ptr;
    }

    uint32_t parseUnicodeEscape(حرف*& ptr)
    {
        uint32_t codepoint = 0;
        لكل (صحيح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            حرف hex = *ptr;
            uint32_t digit = 0;
            إذا      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            وإلا إذا (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            وإلا إذا (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            وإلا CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        أرجع codepoint;
    }

    ساكن فراغ appendUtf8(std::string& out, uint32_t codepoint)
    {
        إذا (codepoint < 0x80) {
            out += (حرف)codepoint;
        } وإلا إذا (codepoint < 0x800) {
            out += (حرف)(0xC0 | (codepoint >> 6));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } وإلا إذا (codepoint < 0x10000) {
            out += (حرف)(0xE0 | (codepoint >> 12));
            out += (حرف)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } وإلا {
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
        إذا (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            إذا (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t نزم = parseUnicodeEscape(ptr);
            إذا (نزم < 0xDC00 || نزم > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (نزم - 0xDC00u);
        }
        وإلا إذا (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    حرف* parse_کلیل( حرف* ptr, File_گرێ& collection, File_گرێ& بەها_placeholder )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        حرف * beg = ptr + 1;
        std::string کلیل_name;
        افعل {
            إذا (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                إذا (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, کلیل_name);
                    استمر;
                }
                کلیل_name += *ptr;
            } وإلا إذا (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                کلیل_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } طالما( cv_isprint(*ptr) && *ptr != '"' );

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        إذا( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        بەها_placeholder = fs->add_گرێ(collection, کلیل_name, File_گرێ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        إذا( !ptr || !*ptr )
            أرجع 0;

        إذا( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        أرجع ++ptr;
    }

    منطقي getBase64Row(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr;
        إذا( !ptr || !*ptr )
            أرجع خطأ;

        // find end of the row
        طالما( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        إذا ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        أرجع true;
    }

    حرف* parse_بەها( حرف* ptr, File_گرێ& گرێ )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        إذا( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        إذا( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            حرف * beg = ptr;
            size_t len = 0u;
            لكل ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            إذا ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, گرێ);

                إذا ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                وإلا
                    ptr++;
            }
            وإلا
            {   /**************** normal string ****************/
                صحيح i = 0, sz;

                ptr = beg;
                منطقي is_matching = خطأ;
                طالما ( !is_matching )
                {
                    بدل ( *ptr )
                    {
                        حالة '\\':
                        {
                            sz = (صحيح)(ptr - beg);
                            إذا( sz > 0 )
                            {
                                إذا (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            إذا (i + 1 >= CV_FS_MAX_LEN)
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
                                إذا (i + (صحيح)utf8.size() >= CV_FS_MAX_LEN)
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
                            إذا( sz > 0 )
                            {
                                إذا (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            إذا ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            اكسر;
                        }
                        حالة '\"':
                        {
                            sz = (صحيح)(ptr - beg);
                            إذا( sz > 0 )
                            {
                                إذا (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = true;
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

                إذا ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                وإلا
                    ptr++;

                گرێ.set_بەها(File_گرێ::STRING, buf, i);
            }
        }
        وإلا إذا ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            حرف * beg = ptr;
            إذا ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            طالما( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            إذا (*ptr == '.' || *ptr == 'e')
            {
                مزدوج fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                گرێ.set_بەها(File_گرێ::REAL, &fval);
            }
            وإلا
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                گرێ.set_بەها(File_گرێ::INT, &ival);
            }

            إذا ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        وإلا
        {    /**************** other data ****************/
            ثابت حرف* beg = ptr;
            صحيح len = 0;
            لكل ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            إذا( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            وإلا إذا( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                گرێ.set_بەها(File_گرێ::INT, &ival);
            }
            وإلا
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        أرجع ptr;
    }

    حرف* parseSeq( حرف* ptr, File_گرێ& گرێ, صحيح depth = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        إذا( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        وإلا
            ptr++;

        fs->convertToCollection(File_گرێ::SEQ, گرێ);

        لكل (;;)
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr != ']' )
            {
                File_گرێ child = fs->add_گرێ(گرێ, std::string(), File_گرێ::NONE );

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                وإلا
                    ptr = parse_بەها( ptr, child );
            }

            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr == ',' )
                ptr++;
            وإلا إذا ( *ptr == ']' )
                اكسر;
            وإلا
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        إذا (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        إذا ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        وإلا
            ptr++;

        fs->finalizeCollection(گرێ);
        أرجع ptr;
    }

    حرف* parseMap( حرف* ptr, File_گرێ& گرێ, صحيح depth = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        إذا( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        وإلا
            ptr++;

        fs->convertToCollection(File_گرێ::MAP, گرێ);

        لكل( ;; )
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr == '"' )
            {
                File_گرێ child;
                ptr = parse_کلیل( ptr, گرێ, child );
                إذا( !ptr || !*ptr )
                    اكسر;
                ptr = skipSpaces( ptr );
                إذا( !ptr || !*ptr )
                    اكسر;

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                وإلا
                    ptr = parse_بەها( ptr, child );
            }

            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr == ',' )
                ptr++;
            وإلا إذا ( *ptr == '}' )
                اكسر;
            وإلا
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        إذا (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        إذا ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        وإلا
            ptr++;

        fs->finalizeCollection(گرێ);
        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        إذا ( !ptr || !*ptr )
            أرجع خطأ;

        File_گرێ ڕەگ_collection(fs->getFS(), 0, 0);

        إذا( *ptr == '{' )
        {
            File_گرێ ڕەگ_گرێ = fs->add_گرێ(ڕەگ_collection, std::string(), File_گرێ::MAP);
            parseMap( ptr, ڕەگ_گرێ );
        }
        وإلا إذا ( *ptr == '[' )
        {
            File_گرێ ڕەگ_گرێ = fs->add_گرێ(ڕەگ_collection, std::string(), File_گرێ::SEQ);
            parseSeq( ptr, ڕەگ_گرێ );
        }
        وإلا
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        أرجع true;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    أرجع makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    أرجع makePtr<JSONParser>(fs);
}

}
