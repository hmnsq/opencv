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
    مجازی ~JSONEmitter() {}

    FStruct_انکڑے شروع_Write_Struct( ثابت FStruct_انکڑے& ماپا, ثابت حرف* کلید,
                                  صحيح struct_flags, ثابت حرف* type_name=0 )
    {
        حرف انکڑے[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_گرہ::TYPE_نقاب|File_گرہ::FLOW)) | File_گرہ::خالی_تھاواں_2;
        إذا( !File_گرہ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        إذا( type_name && *type_name == '\0' )
            type_name = 0;

        منطقي is_real_collection = سچ;
        إذا (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_گرہ::STR;
            انکڑے[0] = '\0';
            is_real_collection = خطأ;
        }

        إذا ( is_real_collection )
        {
            حرف c = File_گرہ::isMap(struct_flags) ? '{' : '[';
            انکڑے[0] = c;
            انکڑے[1] = '\0';
        }

        writeScalar( کلید, انکڑے );
        FStruct_انکڑے current_struct("", struct_flags, ماپا.indent + 4);

        أرجع current_struct;
    }

    فراغ سرا_Write_Struct(ثابت FStruct_انکڑے& current_struct)
    {
        صحيح struct_flags = current_struct.flags;

        إذا (File_گرہ::isCollection(struct_flags)) {
            إذا (!File_گرہ::isFlow(struct_flags)) {
                إذا (fs->بفر_Ptr() <= fs->بفر_شروع() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    حرف *ptr = fs->بفر_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->بفر_شروع());
                    fs->سیٹ_بفر_Ptr(fs->بفر_شروع());
                }
                fs->flush();
            }

            حرف *ptr = fs->بفر_Ptr();
            إذا (ptr > fs->بفر_شروع() + current_struct.indent && !File_گرہ::is_خالی_تھاواں_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_گرہ::isMap(struct_flags) ? '}' : ']';
            fs->سیٹ_بفر_Ptr(ptr);
        }
    }

    فراغ اكتب(ثابت حرف* کلید, صحيح value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10 ));
    }

    فراغ اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128];
        writeScalar( کلید, fs::itoa( value, buf, 10, سچ ));
    }

    فراغ اكتب( ثابت حرف* کلید, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_منزل_String( buf, حجم(buf), value, سچ ));
    }

    فراغ اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* انکڑے = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)لڑی_لمبائی(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            صحيح need_quote = 1;
            انکڑے = buf;
            *انکڑے++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];
                ثابت بنانشانی حرف byte = static_cast<بنانشانی حرف>(c);

                بدل ( c )
                {
                حالة '\\':
                حالة '\"': { *انکڑے++ = '\\'; *انکڑے++ = c;   اكسر; }
                حالة '\n': { *انکڑے++ = '\\'; *انکڑے++ = 'n'; اكسر; }
                حالة '\r': { *انکڑے++ = '\\'; *انکڑے++ = 'r'; اكسر; }
                حالة '\t': { *انکڑے++ = '\\'; *انکڑے++ = 't'; اكسر; }
                حالة '\b': { *انکڑے++ = '\\'; *انکڑے++ = 'b'; اكسر; }
                حالة '\f': { *انکڑے++ = '\\'; *انکڑے++ = 'f'; اكسر; }
                افتراضي  :
                    إذا (byte < 0x20)
                    {
                        ساكن ثابت حرف hex[] = "0123456789abcdef";
                        *انکڑے++ = '\\'; *انکڑے++ = 'u'; *انکڑے++ = '0'; *انکڑے++ = '0';
                        *انکڑے++ = hex[byte >> 4]; *انکڑے++ = hex[byte & 0xf];
                    }
                    وإلا
                        *انکڑے++ = c;
                    اكسر;
                }
            }

            *انکڑے++ = '\"';
            *انکڑے++ = '\0';
            انکڑے = buf + !need_quote;
        }

        writeScalar( کلید, انکڑے);
    }

    فراغ writeScalar(ثابت حرف* کلید, ثابت حرف* انکڑے)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_دا_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_منزل_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_دا_writing_base64() == FileStorage_API::وچ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t کلید_len = 0u;
        إذا( کلید && *کلید == '\0' )
            کلید = 0;
        إذا ( کلید )
        {
            کلید_len = لڑی_لمبائی(کلید);
            إذا ( کلید_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            وإلا إذا ( static_cast<صحيح>(کلید_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t انکڑے_len = 0u;
        إذا ( انکڑے )
            انکڑے_len = لڑی_لمبائی(انکڑے);

        FStruct_انکڑے& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;
        إذا( File_گرہ::isCollection(struct_flags) )
        {
            إذا ( (File_گرہ::isMap(struct_flags) ^ (کلید != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } وإلا {
            fs->سیٹ_Non_خالی_تھاواں();
            struct_flags = File_گرہ::خالی_تھاواں_2 | (کلید ? File_گرہ::MAP : File_گرہ::SEQ);
        }

        // start to write
        حرف* ptr = 0;

        إذا( File_گرہ::isFlow(struct_flags) )
        {
            صحيح new_وتھ;
            ptr = fs->بفر_Ptr();
            إذا( !File_گرہ::is_خالی_تھاواں_Collection(struct_flags) )
                *ptr++ = ',';
            new_وتھ = static_cast<صحيح>(ptr - fs->بفر_شروع() + کلید_len + انکڑے_len);
            إذا( new_وتھ > fs->wrapMargin() && new_وتھ - current_struct.indent > 10 )
            {
                fs->سیٹ_بفر_Ptr(ptr);
                ptr = fs->flush();
            }
            وإلا
                *ptr++ = ' ';
        }
        وإلا
        {
            إذا ( !File_گرہ::is_خالی_تھاواں_Collection(struct_flags) )
            {
                ptr = fs->بفر_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->بفر_شروع() );
                fs->سیٹ_بفر_Ptr(fs->بفر_شروع());
            }
            ptr = fs->flush();
        }

        إذا( کلید )
        {
            إذا( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_بفر( ptr, static_cast<صحيح>(کلید_len) );
            *ptr++ = '\"';

            لكل( size_t i = 0u; i < کلید_len; i++ )
            {
                حرف c = کلید[i];

                ptr[i] = c;
                إذا( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += کلید_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        إذا( انکڑے )
        {
            ptr = fs->resize_Write_بفر( ptr, static_cast<صحيح>(انکڑے_len) );
            حافظہ_نقل( ptr, انکڑے, انکڑے_len );
            ptr += انکڑے_len;
        }

        fs->سیٹ_بفر_Ptr(ptr);
        current_struct.flags &= ~File_گرہ::خالی_تھاواں_2;
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        صحيح len = static_cast<صحيح>(لڑی_لمبائی(comment));
        حرف* ptr = fs->بفر_Ptr();
        ثابت حرف* eol = strchr(comment, '\n');
        منطقي multiline = eol != 0;

        إذا( !eol_comment || multiline || fs->بفر_سرا_2() - ptr < len || ptr == fs->بفر_شروع() )
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
                حافظہ_نقل( ptr, comment, eol - comment + 1 );
                fs->سیٹ_بفر_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            وإلا
            {
                len = (صحيح)لڑی_لمبائی(comment);
                ptr = fs->resize_Write_بفر( ptr, len );
                حافظہ_نقل( ptr, comment, len );
                fs->سیٹ_بفر_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    فراغ شروع_اگلا_دھارا()
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

    مجازی ~JSONParser() {}

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
            ptr = fs->بفر_شروع();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->سیٹ_Eof();
            إذا( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        أرجع ptr;
    }

    uint32_t parseUnicodeEscape(حرف*& ptr)
    {
        uint32_t codepoint = 0;
        لكل (صحيح k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
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
            uint32_t کم = parseUnicodeEscape(ptr);
            إذا (کم < 0xDC00 || کم > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (کم - 0xDC00u);
        }
        وإلا إذا (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    حرف* parse_کلید( حرف* ptr, File_گرہ& collection, File_گرہ& قدر_placeholder )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        حرف * beg = ptr + 1;
        std::string کلید_name;
        افعل {
            إذا (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
                إذا (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_منزل_Utf8(ptr, کلید_name);
                    استمر;
                }
                کلید_name += *ptr;
            } وإلا إذا (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                کلید_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
        } طالما( cv_isprint(*ptr) && *ptr != '"' );

        إذا( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        إذا( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        قدر_placeholder = fs->جوڑو_گرہ(collection, کلید_name, File_گرہ::NONE);

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

    حرف* parse_قدر( حرف* ptr, File_گرہ& گرہ )
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
                ptr = fs->parseBase64(ptr, 0, گرہ);

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
                                حافظہ_نقل(buf + i, beg, sz);
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
                                حافظہ_نقل(buf + i, utf8.انکڑے(), utf8.size());
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
                                حافظہ_نقل(buf + i, beg, sz);
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
                                حافظہ_نقل(buf + i, beg, sz);
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

                گرہ.سیٹ_قدر(File_گرہ::STRING, buf, i);
            }
        }
        وإلا إذا ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            حرف * beg = ptr;
            إذا ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
            }
            طالما( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
            }
            إذا (*ptr == '.' || *ptr == 'e')
            {
                مزدوج fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();

                گرہ.سیٹ_قدر(File_گرہ::REAL, &fval);
            }
            وإلا
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();

                گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
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
                CV_PERSISTENCE_CHECK_سرا_دا_بفر_BUG_CPP();
            }

            إذا( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            وإلا إذا( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                گرہ.سیٹ_قدر(File_گرہ::INT, &ival);
            }
            وإلا
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        أرجع ptr;
    }

    حرف* parseSeq( حرف* ptr, File_گرہ& گرہ, صحيح ڈونگھائی = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        إذا( ڈونگھائی > CV_PERSISTENCE_MAX_ڈونگھائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        وإلا
            ptr++;

        fs->convert_منزل_Collection(File_گرہ::SEQ, گرہ);

        لكل (;;)
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr != ']' )
            {
                File_گرہ بچہ = fs->جوڑو_گرہ(گرہ, std::string(), File_گرہ::NONE );

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, بچہ, ڈونگھائی + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, بچہ, ڈونگھائی + 1 );
                وإلا
                    ptr = parse_قدر( ptr, بچہ );
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

        fs->finalizeCollection(گرہ);
        أرجع ptr;
    }

    حرف* parseMap( حرف* ptr, File_گرہ& گرہ, صحيح ڈونگھائی = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        إذا( ڈونگھائی > CV_PERSISTENCE_MAX_ڈونگھائی )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        إذا ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        وإلا
            ptr++;

        fs->convert_منزل_Collection(File_گرہ::MAP, گرہ);

        لكل( ;; )
        {
            ptr = skipSpaces( ptr );
            إذا( !ptr || !*ptr )
                اكسر;

            إذا ( *ptr == '"' )
            {
                File_گرہ بچہ;
                ptr = parse_کلید( ptr, گرہ, بچہ );
                إذا( !ptr || !*ptr )
                    اكسر;
                ptr = skipSpaces( ptr );
                إذا( !ptr || !*ptr )
                    اكسر;

                إذا ( *ptr == '[' )
                    ptr = parseSeq( ptr, بچہ, ڈونگھائی + 1 );
                وإلا إذا ( *ptr == '{' )
                    ptr = parseMap( ptr, بچہ, ڈونگھائی + 1 );
                وإلا
                    ptr = parse_قدر( ptr, بچہ );
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

        fs->finalizeCollection(گرہ);
        أرجع ptr;
    }

    منطقي parse( حرف* ptr )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        إذا ( !ptr || !*ptr )
            أرجع خطأ;

        File_گرہ جڑ_collection(fs->getFS(), 0, 0);

        إذا( *ptr == '{' )
        {
            File_گرہ جڑ_گرہ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::MAP);
            parseMap( ptr, جڑ_گرہ );
        }
        وإلا إذا ( *ptr == '[' )
        {
            File_گرہ جڑ_گرہ = fs->جوڑو_گرہ(جڑ_collection, std::string(), File_گرہ::SEQ);
            parseSeq( ptr, جڑ_گرہ );
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
