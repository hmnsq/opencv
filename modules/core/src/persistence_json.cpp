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
    مجازي ~JSONEmitter() {}

    FStruct_معلومات شروعات_Write_Struct( ثابت FStruct_معلومات& والد, ثابت حرف* ڪنجي,
                                  صحيح struct_flags, ثابت حرف* type_name=0 )
    {
        حرف معلومات[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ڳنڍ::TYPE_نقاب|File_ڳنڍ::FLOW)) | File_ڳنڍ::خالي_جايون_2;
        إذا( !File_ڳنڍ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا( type_name && *type_name == '\0' )
            type_name = 0;

        منطقي is_real_collection = سچ;
        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ڳنڍ::STR;
            معلومات[0] = '\0';
            is_real_collection = خطأ;
        }

        إذا ( is_real_collection )
        {
            حرف c = File_ڳنڍ::isMap(struct_flags) ? '{' : '[';
            معلومات[0] = c;
            معلومات[1] = '\0';
        }

        writeScalar( ڪنجي, معلومات );
        FStruct_معلومات current_struct("", struct_flags, والد.indent + 4);

        أرجع current_struct;
    }

    فراغ پڇاڙي_Write_Struct(ثابت FStruct_معلومات& current_struct)
    {
        صحيح struct_flags = current_struct.flags;

        إذا (File_ڳنڍ::isCollection(struct_flags)) {
            إذا (!File_ڳنڍ::isFlow(struct_flags)) {
                إذا (fs->بفر_Ptr() <= fs->بفر_شروعات() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    حرف *ptr = fs->بفر_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->بفر_شروعات());
                    fs->سيٽ_بفر_Ptr(fs->بفر_شروعات());
                }
                fs->flush();
            }

            حرف *ptr = fs->بفر_Ptr();
            إذا (ptr > fs->بفر_شروعات() + current_struct.indent && !File_ڳنڍ::is_خالي_جايون_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ڳنڍ::isMap(struct_flags) ? '}' : ']';
            fs->سيٽ_بفر_Ptr(ptr);
        }
    }

    فراغ اكتب(ثابت حرف* ڪنجي, صحيح value)
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* ڪنجي, int64_t value)
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::itoa( value, buf, 10, سچ ));
    }

    فراغ اكتب( ثابت حرف* ڪنجي, مزدوج value )
    {
        حرف buf[128];
        writeScalar( ڪنجي, fs::double_منزل_String( buf, حجم(buf), value, سچ ));
    }

    فراغ اكتب(ثابت حرف* ڪنجي, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* معلومات = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)لڙي_ڊيگهه(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = 1;
            معلومات = buf;
            *معلومات++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];
                ثابت بنانشان حرف byte = static_cast<بنانشان حرف>(c);

                بدل ( c )
                {
                حالة '\\':
                حالة '\"': { *معلومات++ = '\\'; *معلومات++ = c;   اكسر; }
                حالة '\n': { *معلومات++ = '\\'; *معلومات++ = 'n'; اكسر; }
                حالة '\r': { *معلومات++ = '\\'; *معلومات++ = 'r'; اكسر; }
                حالة '\t': { *معلومات++ = '\\'; *معلومات++ = 't'; اكسر; }
                حالة '\b': { *معلومات++ = '\\'; *معلومات++ = 'b'; اكسر; }
                حالة '\f': { *معلومات++ = '\\'; *معلومات++ = 'f'; اكسر; }
                افتراضي  :
                    إذا (byte < 0x20)
                    {
                        ساكن ثابت حرف hex[] = "0123456789abcdef";
                        *معلومات++ = '\\'; *معلومات++ = 'u'; *معلومات++ = '0'; *معلومات++ = '0';
                        *معلومات++ = hex[byte >> 4]; *معلومات++ = hex[byte & 0xf];
                    }
                    وإلا
                        *معلومات++ = c;
                    اكسر;
                }
            }

            *معلومات++ = '\"';
            *معلومات++ = '\0';
            معلومات = buf + !need_quote;
        }

        writeScalar( ڪنجي, معلومات);
    }

    فراغ writeScalar(ثابت حرف* ڪنجي, ثابت حرف* معلومات)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_جو_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_منزل_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_جو_writing_base64() == FileStorage_API::اندريون_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ڪنجي_len = 0u;
        إذا( ڪنجي && *ڪنجي == '\0' )
            ڪنجي = 0;
        إذا ( ڪنجي )
        {
            ڪنجي_len = لڙي_ڊيگهه(ڪنجي);
            إذا ( ڪنجي_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            وإلا إذا ( static_cast<صحيح>(ڪنجي_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t معلومات_len = 0u;
        إذا ( معلومات )
            معلومات_len = لڙي_ڊيگهه(معلومات);

        FStruct_معلومات& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;
        إذا( File_ڳنڍ::isCollection(struct_flags) )
        {
            إذا ( (File_ڳنڍ::isMap(struct_flags) ^ (ڪنجي != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } وإلا {
            fs->سيٽ_Non_خالي_جايون();
            struct_flags = File_ڳنڍ::خالي_جايون_2 | (ڪنجي ? File_ڳنڍ::MAP : File_ڳنڍ::SEQ);
        }

        // start to write
        حرف* ptr = 0;

        إذا( File_ڳنڍ::isFlow(struct_flags) )
        {
            صحيح new_وٿي;
            ptr = fs->بفر_Ptr();
            إذا( !File_ڳنڍ::is_خالي_جايون_Collection(struct_flags) )
                *ptr++ = ',';
            new_وٿي = static_cast<صحيح>(ptr - fs->بفر_شروعات() + ڪنجي_len + معلومات_len);
            إذا( new_وٿي > fs->wrapMargin() && new_وٿي - current_struct.indent > 10 )
            {
                fs->سيٽ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            إذا ( !File_ڳنڍ::is_خالي_جايون_Collection(struct_flags) )
            {
                ptr = fs->بفر_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->بفر_شروعات() );
                fs->سيٽ_بفر_Ptr(fs->بفر_شروعات());
            }
            ptr = fs->flush();
        }

        إذا( ڪنجي )
        {
            إذا( !cv_isalpha(ڪنجي[0]) && ڪنجي[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, static_cast<صحيح>(ڪنجي_len) );
            *ptr++ = '\"';

            لكل( size_t i = 0u; i < ڪنجي_len; i++ )
            {
                حرف c = ڪنجي[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ڪنجي_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        إذا( معلومات )
        {
            ptr = fs->resize_Write_بفر( ptr, static_cast<صحيح>(معلومات_len) );
            يادگيري_نقل( ptr, معلومات, معلومات_len );
            ptr += معلومات_len;
        }

        fs->سيٽ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_ڳنڍ::خالي_جايون_2;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = static_cast<صحيح>(لڙي_ڊيگهه(comment));
        حرف* ptr = fs->بفر_Ptr();
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;

        إذا( !eol_comment || multiline || fs->بفر_پڇاڙي_2() - ptr < len || ptr == fs->بفر_شروعات() )
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
                ptr = fs->resize_Write_بفر( ptr, (صحيح)(eol - comment) + 1 );
                يادگيري_نقل( ptr, comment, eol - comment + 1 );
                fs->سيٽ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
            {
                len = (صحيح)لڙي_ڊيگهه(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                يادگيري_نقل( ptr, comment, len );
                fs->سيٽ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ شروعات_ايندڙ_وهڪرو()
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

    مجازي ~JSONParser() {}

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
                        إذا( !ptr || !*ptr ) { is_eof = سچ; اكسر; }
                    }

                    إذا ( *ptr == '/' )
                    {
                        طالما ( *ptr != '\n' && *ptr != '\r' )
                        {
                            إذا ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                إذا( !ptr || !*ptr ) { is_eof = سچ; اكسر; }
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
                                إذا( !ptr || !*ptr ) { is_eof = سچ; اكسر; }
                            }
                            وإلا إذا ( *ptr == '*' )
                            {
                                ptr++;
                                إذا ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    إذا( !ptr || !*ptr ) { is_eof = سچ; اكسر; }
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
                    إذا( !ptr || !*ptr ) { is_eof = سچ; اكسر; }
                } اكسر;
                    /* other character */
                افتراضي: {
                    إذا( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = سچ;
                } اكسر;
            }
        }

        إذا ( is_eof || !is_completed )
        {
            ptr = fs->بفر_شروعات();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->سيٽ_Eof();
            إذا( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        أرجع ptr;
    }

    uint32_t parseUnicodeEscape(حرف*& ptr)
    {
        uint32_t codepoint = 0;
        لكل (صحيح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
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
    فراغ parse_Unicode_Escape_منزل_Utf8(حرف*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        إذا (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            إذا (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t هيٺيون = parseUnicodeEscape(ptr);
            إذا (هيٺيون < 0xDC00 || هيٺيون > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (هيٺيون - 0xDC00u);
        }
        وإلا إذا (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    حرف* parse_ڪنجي( حرف* ptr, File_ڳنڍ& collection, File_ڳنڍ& قدر_placeholder )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        حرف * beg = ptr + 1;
        std::string ڪنجي_name;
        افعل {
            إذا (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
                إذا (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_منزل_Utf8(ptr, ڪنجي_name);
                    استمر;
                }
                ڪنجي_name += *ptr;
            } وإلا إذا (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ڪنجي_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
        } طالما( cv_isprint(*ptr) && *ptr != '"' );

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        إذا( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        قدر_placeholder = fs->جوڙيو_ڳنڍ(collection, ڪنجي_name, File_ڳنڍ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        إذا( !ptr || !*ptr )
            أرجع 0;

        إذا( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        أرجع ++ptr;
    }

    منطقي get_Base64_صف(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
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
        أرجع سچ;
    }

    حرف* parse_قدر( حرف* ptr, File_ڳنڍ& ڳنڍ )
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
                ptr = fs->parseBase64(ptr, 0, ڳنڍ);

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
                                يادگيري_نقل(buf + i, beg, sz);
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
                                parse_Unicode_Escape_منزل_Utf8(ptr, utf8);
                                إذا (i + (صحيح)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                يادگيري_نقل(buf + i, utf8.معلومات(), utf8.size());
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
                                يادگيري_نقل(buf + i, beg, sz);
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
                                يادگيري_نقل(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = سچ;
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

                ڳنڍ.سيٽ_قدر(File_ڳنڍ::STRING, buf, i);
            }
        }
        وإلا إذا ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            حرف * beg = ptr;
            إذا ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
            }
            طالما( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
            }
            إذا (*ptr == '.' || *ptr == 'e')
            {
                مزدوج fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();

                ڳنڍ.سيٽ_قدر(File_ڳنڍ::REAL, &fval);
            }
            وإلا
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();

                ڳنڍ.سيٽ_قدر(File_ڳنڍ::INT, &ival);
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
                CV_PERSISTENCE_CHECK_پڇاڙي_جو_بفر_BUG_CPP();
            }

            إذا( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            وإلا إذا( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ڳنڍ.سيٽ_قدر(File_ڳنڍ::INT, &ival);
            }
            وإلا
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        أرجع ptr;
    }

    حرف* parseSeq( حرف* ptr, File_ڳنڍ& ڳنڍ, صحيح اونهائي = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        إذا( اونهائي > CV_PERSISTENCE_MAX_اونهائي )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        وإلا
            ptr++;

        fs->convert_منزل_Collection(File_ڳنڍ::SEQ, ڳنڍ);

        لكل (;;)
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr != ']' )
            {
                File_ڳنڍ ٻار = fs->جوڙيو_ڳنڍ(ڳنڍ, std::string(), File_ڳنڍ::NONE );

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, ٻار, اونهائي + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, ٻار, اونهائي + 1 );
                وإلا
                    ptr = parse_قدر( ptr, ٻار );
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

        fs->finalizeCollection(ڳنڍ);
        أرجع ptr;
    }

    حرف* parseMap( حرف* ptr, File_ڳنڍ& ڳنڍ, صحيح اونهائي = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        إذا( اونهائي > CV_PERSISTENCE_MAX_اونهائي )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        وإلا
            ptr++;

        fs->convert_منزل_Collection(File_ڳنڍ::MAP, ڳنڍ);

        لكل( ;; )
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr == '"' )
            {
                File_ڳنڍ ٻار;
                ptr = parse_ڪنجي( ptr, ڳنڍ, ٻار );
                إذا( !ptr || !*ptr )
                    اكسر;
                ptr = skipSpaces( ptr );
                إذا( !ptr || !*ptr )
                    اكسر;

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, ٻار, اونهائي + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, ٻار, اونهائي + 1 );
                وإلا
                    ptr = parse_قدر( ptr, ٻار );
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

        fs->finalizeCollection(ڳنڍ);
        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        إذا ( !ptr || !*ptr )
            أرجع خطأ;

        File_ڳنڍ پاڙ_collection(fs->getFS(), 0, 0);

        إذا( *ptr == '{' )
        {
            File_ڳنڍ پاڙ_ڳنڍ = fs->جوڙيو_ڳنڍ(پاڙ_collection, std::string(), File_ڳنڍ::MAP);
            parseMap( ptr, پاڙ_ڳنڍ );
        }
        وإلا إذا ( *ptr == '[' )
        {
            File_ڳنڍ پاڙ_ڳنڍ = fs->جوڙيو_ڳنڍ(پاڙ_collection, std::string(), File_ڳنڍ::SEQ);
            parseSeq( ptr, پاڙ_ڳنڍ );
        }
        وإلا
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        أرجع سچ;
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
