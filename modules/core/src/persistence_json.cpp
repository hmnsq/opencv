// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"

نامخلا cv
{

جماعت JSONEmitter : عوامی FileStorageEmitter
{
عوامی:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~JSONEmitter() {}

    FStruct_ڈیٹا آغاز_Write_Struct( ثابت FStruct_ڈیٹا& والد, ثابت حرف* کلید,
                                  صحیح struct_flags, ثابت حرف* type_name=0 )
    {
        حرف ڈیٹا[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_گرہ::TYPE_نقاب|File_گرہ::FLOW)) | File_گرہ::خالی_خانے_2;
        اگر( !File_گرہ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        اگر( type_name && *type_name == '\0' )
            type_name = 0;

        منطقی is_real_collection = سچ;
        اگر (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_گرہ::STR;
            ڈیٹا[0] = '\0';
            is_real_collection = جھوٹ;
        }

        اگر ( is_real_collection )
        {
            حرف c = File_گرہ::isMap(struct_flags) ? '{' : '[';
            ڈیٹا[0] = c;
            ڈیٹا[1] = '\0';
        }

        writeScalar( کلید, ڈیٹا );
        FStruct_ڈیٹا current_struct("", struct_flags, والد.indent + 4);

        واپس current_struct;
    }

    خالی انتہا_Write_Struct(ثابت FStruct_ڈیٹا& current_struct)
    {
        صحیح struct_flags = current_struct.flags;

        اگر (File_گرہ::isCollection(struct_flags)) {
            اگر (!File_گرہ::isFlow(struct_flags)) {
                اگر (fs->بفر_Ptr() <= fs->بفر_آغاز() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    حرف *ptr = fs->بفر_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->بفر_آغاز());
                    fs->سیٹ_بفر_Ptr(fs->بفر_آغاز());
                }
                fs->flush();
            }

            حرف *ptr = fs->بفر_Ptr();
            اگر (ptr > fs->بفر_آغاز() + current_struct.indent && !File_گرہ::is_خالی_خانے_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_گرہ::isMap(struct_flags) ? '}' : ']';
            fs->سیٹ_بفر_Ptr(ptr);
        }
    }

    خالی اكتب(ثابت حرف* کلید, صحیح value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10 ));
    }

    خالی اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10, سچ ));
    }

    خالی اكتب( ثابت حرف* کلید, دوہرا value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_مقصد_String( buf, حجم(buf), value, سچ ));
    }

    خالی اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقی quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* ڈیٹا = (حرف*)str;
        صحیح i, len;

        اگر( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحیح)لڑی_لمبائی(str);
        اگر( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        اگر( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحیح need_quote = 1;
            ڈیٹا = buf;
            *ڈیٹا++ = '\"';
            برائے( i = 0; i < len; i++ )
            {
                حرف c = str[i];
                ثابت غیرعلامتی حرف byte = static_cast<غیرعلامتی حرف>(c);

                انتخاب ( c )
                {
                حالت '\\':
                حالت '\"': { *ڈیٹا++ = '\\'; *ڈیٹا++ = c;   توڑو; }
                حالت '\n': { *ڈیٹا++ = '\\'; *ڈیٹا++ = 'n'; توڑو; }
                حالت '\r': { *ڈیٹا++ = '\\'; *ڈیٹا++ = 'r'; توڑو; }
                حالت '\t': { *ڈیٹا++ = '\\'; *ڈیٹا++ = 't'; توڑو; }
                حالت '\b': { *ڈیٹا++ = '\\'; *ڈیٹا++ = 'b'; توڑو; }
                حالت '\f': { *ڈیٹا++ = '\\'; *ڈیٹا++ = 'f'; توڑو; }
                طےشدہ  :
                    اگر (byte < 0x20)
                    {
                        ساکن ثابت حرف hex[] = "0123456789abcdef";
                        *ڈیٹا++ = '\\'; *ڈیٹا++ = 'u'; *ڈیٹا++ = '0'; *ڈیٹا++ = '0';
                        *ڈیٹا++ = hex[byte >> 4]; *ڈیٹا++ = hex[byte & 0xf];
                    }
                    ورنہ
                        *ڈیٹا++ = c;
                    توڑو;
                }
            }

            *ڈیٹا++ = '\"';
            *ڈیٹا++ = '\0';
            ڈیٹا = buf + !need_quote;
        }

        writeScalar( کلید, ڈیٹا);
    }

    خالی writeScalar(ثابت حرف* کلید, ثابت حرف* ڈیٹا)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(جھوٹ);
        اگر ( fs->get_state_کا_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_مقصد_Base64_state( FileStorage_API::NotUse );
        }
        ورنہ اگر ( fs->get_state_کا_writing_base64() == FileStorage_API::اندر_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t کلید_len = 0u;
        اگر( کلید && *کلید == '\0' )
            کلید = 0;
        اگر ( کلید )
        {
            کلید_len = لڑی_لمبائی(کلید);
            اگر ( کلید_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ورنہ اگر ( static_cast<صحیح>(کلید_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t ڈیٹا_len = 0u;
        اگر ( ڈیٹا )
            ڈیٹا_len = لڑی_لمبائی(ڈیٹا);

        FStruct_ڈیٹا& current_struct = fs->getCurrentStruct();
        صحیح struct_flags = current_struct.flags;
        اگر( File_گرہ::isCollection(struct_flags) )
        {
            اگر ( (File_گرہ::isMap(struct_flags) ^ (کلید != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ورنہ {
            fs->سیٹ_Non_خالی_خانے();
            struct_flags = File_گرہ::خالی_خانے_2 | (کلید ? File_گرہ::MAP : File_گرہ::SEQ);
        }

        // start to write
        حرف* ptr = 0;

        اگر( File_گرہ::isFlow(struct_flags) )
        {
            صحیح new_انحراف;
            ptr = fs->بفر_Ptr();
            اگر( !File_گرہ::is_خالی_خانے_Collection(struct_flags) )
                *ptr++ = ',';
            new_انحراف = static_cast<صحیح>(ptr - fs->بفر_آغاز() + کلید_len + ڈیٹا_len);
            اگر( new_انحراف > fs->wrapMargin() && new_انحراف - current_struct.indent > 10 )
            {
                fs->سیٹ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            ورنہ
                *ptr++ = ' ';
        }
        ورنہ
        {
            اگر ( !File_گرہ::is_خالی_خانے_Collection(struct_flags) )
            {
                ptr = fs->بفر_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->بفر_آغاز() );
                fs->سیٹ_بفر_Ptr(fs->بفر_آغاز());
            }
            ptr = fs->flush();
        }

        اگر( کلید )
        {
            اگر( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, static_cast<صحیح>(کلید_len) );
            *ptr++ = '\"';

            برائے( size_t i = 0u; i < کلید_len; i++ )
            {
                حرف c = کلید[i];

                ptr[i] = c;
                اگر( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += کلید_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        اگر( ڈیٹا )
        {
            ptr = fs->resize_Write_بفر( ptr, static_cast<صحیح>(ڈیٹا_len) );
            حافظہ_نقل( ptr, ڈیٹا, ڈیٹا_len );
            ptr += ڈیٹا_len;
        }

        fs->سیٹ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_گرہ::خالی_خانے_2;
    }

    خالی writeComment(ثابت حرف* comment, منطقی eol_comment)
    {
        اگر( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحیح len = static_cast<صحیح>(لڑی_لمبائی(comment));
        حرف* ptr = fs->بفر_Ptr();
        ثابت حرف* eol = strchr(comment, '\n');
        منطقی multiline = eol != 0;

        اگر( !eol_comment || multiline || fs->بفر_انتہا_2() - ptr < len || ptr == fs->بفر_آغاز() )
            ptr = fs->flush();
        ورنہ
            *ptr++ = ' ';

        جب( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            اگر( eol )
            {
                ptr = fs->resize_Write_بفر( ptr, (صحیح)(eol - comment) + 1 );
                حافظہ_نقل( ptr, comment, eol - comment + 1 );
                fs->سیٹ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ورنہ
            {
                len = (صحیح)لڑی_لمبائی(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                حافظہ_نقل( ptr, comment, len );
                fs->سیٹ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    خالی آغاز_اگلا_دھارا()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

محفوظ:
    FileStorage_API* fs;
};

جماعت JSONParser : عوامی FileStorageParser
{
عوامی:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~JSONParser() {}

    حرف* skipSpaces( حرف* ptr )
    {
        منطقی is_eof = جھوٹ;
        منطقی is_completed = جھوٹ;

        جب ( is_eof == جھوٹ && is_completed == جھوٹ )
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
                        اگر( !ptr || !*ptr ) { is_eof = سچ; توڑو; }
                    }

                    اگر ( *ptr == '/' )
                    {
                        جب ( *ptr != '\n' && *ptr != '\r' )
                        {
                            اگر ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                اگر( !ptr || !*ptr ) { is_eof = سچ; توڑو; }
                            }
                            ورنہ
                            {
                                ptr++;
                            }
                        }
                    }
                    ورنہ اگر ( *ptr == '*' )
                    {
                        ptr++;
                        برائے (;;)
                        {
                            اگر ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                اگر( !ptr || !*ptr ) { is_eof = سچ; توڑو; }
                            }
                            ورنہ اگر ( *ptr == '*' )
                            {
                                ptr++;
                                اگر ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    اگر( !ptr || !*ptr ) { is_eof = سچ; توڑو; }
                                }
                                اگر ( *ptr == '/' )
                                {
                                    ptr++;
                                    توڑو;
                                }
                            }
                            ورنہ
                            {
                                ptr++;
                            }
                        }
                    }
                    ورنہ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } توڑو;
                    /* whitespace */
                حالت '\t':
                حالت ' ' : {
                    ptr++;
                } توڑو;
                    /* newline || end mark */
                حالت '\0':
                حالت '\n':
                حالت '\r': {
                    ptr = fs->gets();
                    اگر( !ptr || !*ptr ) { is_eof = سچ; توڑو; }
                } توڑو;
                    /* other character */
                طےشدہ: {
                    اگر( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = سچ;
                } توڑو;
            }
        }

        اگر ( is_eof || !is_completed )
        {
            ptr = fs->بفر_آغاز();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->سیٹ_Eof();
            اگر( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        واپس ptr;
    }

    uint32_t parseUnicodeEscape(حرف*& ptr)
    {
        uint32_t codepoint = 0;
        برائے (صحیح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
            حرف hex = *ptr;
            uint32_t digit = 0;
            اگر      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ورنہ اگر (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ورنہ اگر (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ورنہ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        واپس codepoint;
    }

    ساکن خالی appendUtf8(std::string& out, uint32_t codepoint)
    {
        اگر (codepoint < 0x80) {
            out += (حرف)codepoint;
        } ورنہ اگر (codepoint < 0x800) {
            out += (حرف)(0xC0 | (codepoint >> 6));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } ورنہ اگر (codepoint < 0x10000) {
            out += (حرف)(0xE0 | (codepoint >> 12));
            out += (حرف)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (حرف)(0x80 | (codepoint & 0x3F));
        } ورنہ {
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
    خالی parse_Unicode_Escape_مقصد_Utf8(حرف*& ptr, std::string& out)
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
        ورنہ اگر (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    حرف* parse_کلید( حرف* ptr, File_گرہ& collection, File_گرہ& قدر_placeholder )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        اگر( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        حرف * beg = ptr + 1;
        std::string کلید_name;
        کرو {
            اگر (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
                اگر (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_مقصد_Utf8(ptr, کلید_name);
                    جاری;
                }
                کلید_name += *ptr;
            } ورنہ اگر (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                کلید_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
        } جب( cv_isprint(*ptr) && *ptr != '"' );

        اگر( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        اگر( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        قدر_placeholder = fs->جوڑو_گرہ(collection, کلید_name, File_گرہ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        اگر( !ptr || !*ptr )
            واپس 0;

        اگر( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        واپس ++ptr;
    }

    منطقی get_Base64_صف(حرف* ptr, صحیح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr;
        اگر( !ptr || !*ptr )
            واپس جھوٹ;

        // find end of the row
        جب( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        اگر ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        واپس سچ;
    }

    حرف* parse_قدر( حرف* ptr, File_گرہ& گرہ )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        اگر( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        اگر( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            حرف * beg = ptr;
            size_t len = 0u;
            برائے ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            اگر ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, گرہ);

                اگر ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ورنہ
                    ptr++;
            }
            ورنہ
            {   /**************** normal string ****************/
                صحیح i = 0, sz;

                ptr = beg;
                منطقی is_matching = جھوٹ;
                جب ( !is_matching )
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
                                حافظہ_نقل(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            اگر (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            انتخاب ( *ptr )
                            {
                            حالت '\\':
                            حالت '\"':
                            حالت '\'': { buf[i++] = *ptr; توڑو; }
                            حالت 'n' : { buf[i++] = '\n'; توڑو; }
                            حالت 'r' : { buf[i++] = '\r'; توڑو; }
                            حالت 't' : { buf[i++] = '\t'; توڑو; }
                            حالت 'b' : { buf[i++] = '\b'; توڑو; }
                            حالت 'f' : { buf[i++] = '\f'; توڑو; }
                            حالت 'u' : {
                                ptr++;
                                std::string utf8;
                                parse_Unicode_Escape_مقصد_Utf8(ptr, utf8);
                                اگر (i + (صحیح)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                حافظہ_نقل(buf + i, utf8.ڈیٹا(), utf8.size());
                                i += (صحیح)utf8.size();
                                beg = ptr;
                                جاری;
                            }
                            طےشدہ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            توڑو;
                            }
                            ptr++;
                            beg = ptr;
                            توڑو;
                        }
                        حالت '\0':
                        {
                            sz = (صحیح)(ptr - beg);
                            اگر( sz > 0 )
                            {
                                اگر (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                حافظہ_نقل(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            اگر ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            توڑو;
                        }
                        حالت '\"':
                        {
                            sz = (صحیح)(ptr - beg);
                            اگر( sz > 0 )
                            {
                                اگر (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                حافظہ_نقل(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = سچ;
                            توڑو;
                        }
                        حالت '\n':
                        حالت '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            توڑو;
                        }
                        طےشدہ:
                        {
                            ptr++;
                            توڑو;
                        }
                    }
                }

                اگر ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ورنہ
                    ptr++;

                گرہ.سیٹ_قدر(File_گرہ::STRING, buf, i);
            }
        }
        ورنہ اگر ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            حرف * beg = ptr;
            اگر ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
            }
            جب( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
            }
            اگر (*ptr == '.' || *ptr == 'e')
            {
                دوہرا fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();

                گرہ.سیٹ_قدر(File_گرہ::REAL, &fval);
            }
            ورنہ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();

                گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
            }

            اگر ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ورنہ
        {    /**************** other data ****************/
            ثابت حرف* beg = ptr;
            صحیح len = 0;
            برائے ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_انتہا_کا_بفر_BUG_CPP();
            }

            اگر( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ورنہ اگر( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
            }
            ورنہ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        واپس ptr;
    }

    حرف* parseSeq( حرف* ptr, File_گرہ& گرہ, صحیح گہرائی = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        اگر( گہرائی > CV_PERSISTENCE_MAX_گہرائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        اگر ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ورنہ
            ptr++;

        fs->convert_مقصد_Collection(File_گرہ::SEQ, گرہ);

        برائے (;;)
        {
            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                توڑو;

            اگر ( *ptr != ']' )
            {
                File_گرہ بچہ = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE );

                اگر ( *ptr == '[' )
                    ptr = parseSeq( ptr, بچہ, گہرائی + 1 );
                ورنہ اگر ( *ptr == '{' )
                    ptr = parseMap( ptr, بچہ, گہرائی + 1 );
                ورنہ
                    ptr = parse_قدر( ptr, بچہ );
            }

            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                توڑو;

            اگر ( *ptr == ',' )
                ptr++;
            ورنہ اگر ( *ptr == ']' )
                توڑو;
            ورنہ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ورنہ
            ptr++;

        fs->finalizeCollection(گرہ);
        واپس ptr;
    }

    حرف* parseMap( حرف* ptr, File_گرہ& گرہ, صحیح گہرائی = 0 )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر( گہرائی > CV_PERSISTENCE_MAX_گہرائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        اگر ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ورنہ
            ptr++;

        fs->convert_مقصد_Collection(File_گرہ::MAP, گرہ);

        برائے( ;; )
        {
            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                توڑو;

            اگر ( *ptr == '"' )
            {
                File_گرہ بچہ;
                ptr = parse_کلید( ptr, گرہ, بچہ );
                اگر( !ptr || !*ptr )
                    توڑو;
                ptr = skipSpaces( ptr );
                اگر( !ptr || !*ptr )
                    توڑو;

                اگر ( *ptr == '[' )
                    ptr = parseSeq( ptr, بچہ, گہرائی + 1 );
                ورنہ اگر ( *ptr == '{' )
                    ptr = parseMap( ptr, بچہ, گہرائی + 1 );
                ورنہ
                    ptr = parse_قدر( ptr, بچہ );
            }

            ptr = skipSpaces( ptr );
            اگر( !ptr || !*ptr )
                توڑو;

            اگر ( *ptr == ',' )
                ptr++;
            ورنہ اگر ( *ptr == '}' )
                توڑو;
            ورنہ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        اگر (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        اگر ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ورنہ
            ptr++;

        fs->finalizeCollection(گرہ);
        واپس ptr;
    }

    منطقی parse( حرف* ptr )
    {
        اگر (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        اگر ( !ptr || !*ptr )
            واپس جھوٹ;

        File_گرہ جڑ_collection(fs->getFS(), 0, 0);

        اگر( *ptr == '{' )
        {
            File_گرہ جڑ_گرہ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::MAP);
            parseMap( ptr, جڑ_گرہ );
        }
        ورنہ اگر ( *ptr == '[' )
        {
            File_گرہ جڑ_گرہ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::SEQ);
            parseSeq( ptr, جڑ_گرہ );
        }
        ورنہ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        واپس سچ;
    }

    FileStorage_API* fs;
    حرف buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    واپس makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    واپس makePtr<JSONParser>(fs);
}

}
