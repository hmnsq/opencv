// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence.hpp"

নামাকাশ cv
{

শ্রেণী JSONEmitter : সর্বজনীন FileStorageEmitter
{
সর্বজনীন:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    আভাসী ~JSONEmitter() {}

    FStruct_তথ্য startWriteStruct( ধ্রুবক FStruct_তথ্য& parent, ধ্রুবক অক্ষর* চাবি,
                                  পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর* type_name=0 )
    {
        অক্ষর তথ্য[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_গ্রন্থি::TYPE_MASK|File_গ্রন্থি::FLOW)) | File_গ্রন্থি::EMPTY;
        যদি( !File_গ্রন্থি::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        যদি( type_name && *type_name == '\0' )
            type_name = 0;

        যৌক্তিক is_real_collection = সত্য;
        যদি (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_গ্রন্থি::STR;
            তথ্য[0] = '\0';
            is_real_collection = মিথ্যা;
        }

        যদি ( is_real_collection )
        {
            অক্ষর c = File_গ্রন্থি::isMap(struct_flags) ? '{' : '[';
            তথ্য[0] = c;
            তথ্য[1] = '\0';
        }

        writeScalar( চাবি, তথ্য );
        FStruct_তথ্য current_struct("", struct_flags, parent.indent + 4);

        ফেরত current_struct;
    }

    শূন্য endWriteStruct(ধ্রুবক FStruct_তথ্য& current_struct)
    {
        পূর্ণসংখ্যা struct_flags = current_struct.flags;

        যদি (File_গ্রন্থি::isCollection(struct_flags)) {
            যদি (!File_গ্রন্থি::isFlow(struct_flags)) {
                যদি (fs->বাফার_Ptr() <= fs->বাফার_Start() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    অক্ষর *ptr = fs->বাফার_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->বাফার_Start());
                    fs->set_বাফার_Ptr(fs->বাফার_Start());
                }
                fs->flush();
            }

            অক্ষর *ptr = fs->বাফার_Ptr();
            যদি (ptr > fs->বাফার_Start() + current_struct.indent && !File_গ্রন্থি::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_গ্রন্থি::isMap(struct_flags) ? '}' : ']';
            fs->set_বাফার_Ptr(ptr);
        }
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা value)
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10 ));
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, int64_t value)
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::itoa( value, buf, 10, সত্য ));
    }

    শূন্য লিখ( ধ্রুবক অক্ষর* চাবি, দ্বিগুণ value )
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::doubleToString( buf, আকার(buf), value, সত্য ));
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* str, যৌক্তিক quote)
    {
        অক্ষর buf[CV_FS_MAX_LEN*6+16];
        অক্ষর* তথ্য = (অক্ষর*)str;
        পূর্ণসংখ্যা i, len;

        যদি( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (পূর্ণসংখ্যা)strlen(str);
        যদি( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        যদি( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            পূর্ণসংখ্যা need_quote = 1;
            তথ্য = buf;
            *তথ্য++ = '\"';
            জন্য( i = 0; i < len; i++ )
            {
                অক্ষর c = str[i];
                ধ্রুবক unsigned অক্ষর byte = static_cast<unsigned অক্ষর>(c);

                নির্বাচন ( c )
                {
                ক্ষেত্র '\\':
                ক্ষেত্র '\"': { *তথ্য++ = '\\'; *তথ্য++ = c;   বিরতি; }
                ক্ষেত্র '\n': { *তথ্য++ = '\\'; *তথ্য++ = 'n'; বিরতি; }
                ক্ষেত্র '\r': { *তথ্য++ = '\\'; *তথ্য++ = 'r'; বিরতি; }
                ক্ষেত্র '\t': { *তথ্য++ = '\\'; *তথ্য++ = 't'; বিরতি; }
                ক্ষেত্র '\b': { *তথ্য++ = '\\'; *তথ্য++ = 'b'; বিরতি; }
                ক্ষেত্র '\f': { *তথ্য++ = '\\'; *তথ্য++ = 'f'; বিরতি; }
                পূর্বনির্ধারিত  :
                    যদি (byte < 0x20)
                    {
                        স্থির ধ্রুবক অক্ষর hex[] = "0123456789abcdef";
                        *তথ্য++ = '\\'; *তথ্য++ = 'u'; *তথ্য++ = '0'; *তথ্য++ = '0';
                        *তথ্য++ = hex[byte >> 4]; *তথ্য++ = hex[byte & 0xf];
                    }
                    অন্যথা
                        *তথ্য++ = c;
                    বিরতি;
                }
            }

            *তথ্য++ = '\"';
            *তথ্য++ = '\0';
            তথ্য = buf + !need_quote;
        }

        writeScalar( চাবি, তথ্য);
    }

    শূন্য writeScalar(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* তথ্য)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(মিথ্যা);
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
            অন্যথা যদি ( static_cast<পূর্ণসংখ্যা>(চাবি_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t তথ্য_len = 0u;
        যদি ( তথ্য )
            তথ্য_len = strlen(তথ্য);

        FStruct_তথ্য& current_struct = fs->getCurrentStruct();
        পূর্ণসংখ্যা struct_flags = current_struct.flags;
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
        অক্ষর* ptr = 0;

        যদি( File_গ্রন্থি::isFlow(struct_flags) )
        {
            পূর্ণসংখ্যা new_offset;
            ptr = fs->বাফার_Ptr();
            যদি( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<পূর্ণসংখ্যা>(ptr - fs->বাফার_Start() + চাবি_len + তথ্য_len);
            যদি( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->set_বাফার_Ptr(ptr);
                ptr = fs->flush();
            }
            অন্যথা
                *ptr++ = ' ';
        }
        অন্যথা
        {
            যদি ( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
            {
                ptr = fs->বাফার_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->বাফার_Start() );
                fs->set_বাফার_Ptr(fs->বাফার_Start());
            }
            ptr = fs->flush();
        }

        যদি( চাবি )
        {
            যদি( !cv_isalpha(চাবি[0]) && চাবি[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_বাফার( ptr, static_cast<পূর্ণসংখ্যা>(চাবি_len) );
            *ptr++ = '\"';

            জন্য( size_t i = 0u; i < চাবি_len; i++ )
            {
                অক্ষর c = চাবি[i];

                ptr[i] = c;
                যদি( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += চাবি_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        যদি( তথ্য )
        {
            ptr = fs->resize_Write_বাফার( ptr, static_cast<পূর্ণসংখ্যা>(তথ্য_len) );
            memcpy( ptr, তথ্য, তথ্য_len );
            ptr += তথ্য_len;
        }

        fs->set_বাফার_Ptr(ptr);
        current_struct.flags &= ~File_গ্রন্থি::EMPTY;
    }

    শূন্য writeComment(ধ্রুবক অক্ষর* comment, যৌক্তিক eol_comment)
    {
        যদি( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        পূর্ণসংখ্যা len = static_cast<পূর্ণসংখ্যা>(strlen(comment));
        অক্ষর* ptr = fs->বাফার_Ptr();
        ধ্রুবক অক্ষর* eol = strchr(comment, '\n');
        যৌক্তিক multiline = eol != 0;

        যদি( !eol_comment || multiline || fs->বাফার_End() - ptr < len || ptr == fs->বাফার_Start() )
            ptr = fs->flush();
        অন্যথা
            *ptr++ = ' ';

        যখন( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            যদি( eol )
            {
                ptr = fs->resize_Write_বাফার( ptr, (পূর্ণসংখ্যা)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->set_বাফার_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            অন্যথা
            {
                len = (পূর্ণসংখ্যা)strlen(comment);
                ptr = fs->resize_Write_বাফার( ptr, len );
                memcpy( ptr, comment, len );
                fs->set_বাফার_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    শূন্য start_পরবর্তী_Stream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

সুরক্ষিত:
    FileStorage_API* fs;
};

শ্রেণী JSONParser : সর্বজনীন FileStorageParser
{
সর্বজনীন:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    আভাসী ~JSONParser() {}

    অক্ষর* skipSpaces( অক্ষর* ptr )
    {
        যৌক্তিক is_eof = মিথ্যা;
        যৌক্তিক is_completed = মিথ্যা;

        যখন ( is_eof == মিথ্যা && is_completed == মিথ্যা )
        {
            যদি (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            নির্বাচন ( *ptr )
            {
                /* comment */
                ক্ষেত্র '/' : {
                    ptr++;
                    যদি ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        যদি( !ptr || !*ptr ) { is_eof = সত্য; বিরতি; }
                    }

                    যদি ( *ptr == '/' )
                    {
                        যখন ( *ptr != '\n' && *ptr != '\r' )
                        {
                            যদি ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                যদি( !ptr || !*ptr ) { is_eof = সত্য; বিরতি; }
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
                        জন্য (;;)
                        {
                            যদি ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                যদি( !ptr || !*ptr ) { is_eof = সত্য; বিরতি; }
                            }
                            অন্যথা যদি ( *ptr == '*' )
                            {
                                ptr++;
                                যদি ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    যদি( !ptr || !*ptr ) { is_eof = সত্য; বিরতি; }
                                }
                                যদি ( *ptr == '/' )
                                {
                                    ptr++;
                                    বিরতি;
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
                } বিরতি;
                    /* whitespace */
                ক্ষেত্র '\t':
                ক্ষেত্র ' ' : {
                    ptr++;
                } বিরতি;
                    /* newline || end mark */
                ক্ষেত্র '\0':
                ক্ষেত্র '\n':
                ক্ষেত্র '\r': {
                    ptr = fs->gets();
                    যদি( !ptr || !*ptr ) { is_eof = সত্য; বিরতি; }
                } বিরতি;
                    /* other character */
                পূর্বনির্ধারিত: {
                    যদি( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = সত্য;
                } বিরতি;
            }
        }

        যদি ( is_eof || !is_completed )
        {
            ptr = fs->বাফার_Start();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            যদি( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ফেরত ptr;
    }

    uint32_t parseUnicodeEscape(অক্ষর*& ptr)
    {
        uint32_t codepoint = 0;
        জন্য (পূর্ণসংখ্যা k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
            অক্ষর hex = *ptr;
            uint32_t digit = 0;
            যদি      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            অন্যথা যদি (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            অন্যথা যদি (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            অন্যথা CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ফেরত codepoint;
    }

    স্থির শূন্য appendUtf8(std::string& out, uint32_t codepoint)
    {
        যদি (codepoint < 0x80) {
            out += (অক্ষর)codepoint;
        } অন্যথা যদি (codepoint < 0x800) {
            out += (অক্ষর)(0xC0 | (codepoint >> 6));
            out += (অক্ষর)(0x80 | (codepoint & 0x3F));
        } অন্যথা যদি (codepoint < 0x10000) {
            out += (অক্ষর)(0xE0 | (codepoint >> 12));
            out += (অক্ষর)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (অক্ষর)(0x80 | (codepoint & 0x3F));
        } অন্যথা {
            out += (অক্ষর)(0xF0 | (codepoint >> 18));
            out += (অক্ষর)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (অক্ষর)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (অক্ষর)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    শূন্য parseUnicodeEscapeToUtf8(অক্ষর*& ptr, std::string& out)
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

    অক্ষর* parse_চাবি( অক্ষর* ptr, File_গ্রন্থি& collection, File_গ্রন্থি& মান_placeholder )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        অক্ষর * beg = ptr + 1;
        std::string চাবি_name;
        করো {
            যদি (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
                যদি (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, চাবি_name);
                    চালু;
                }
                চাবি_name += *ptr;
            } অন্যথা যদি (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                চাবি_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
        } যখন( cv_isprint(*ptr) && *ptr != '"' );

        যদি( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        যদি( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        মান_placeholder = fs->add_গ্রন্থি(collection, চাবি_name, File_গ্রন্থি::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        যদি( !ptr || !*ptr )
            ফেরত 0;

        যদি( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ফেরত ++ptr;
    }

    যৌক্তিক getBase64Row(অক্ষর* ptr, পূর্ণসংখ্যা /*indent*/, অক্ষর* &beg, অক্ষর* &end)
    {
        beg = end = ptr;
        যদি( !ptr || !*ptr )
            ফেরত মিথ্যা;

        // find end of the row
        যখন( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        যদি ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ফেরত সত্য;
    }

    অক্ষর* parse_মান( অক্ষর* ptr, File_গ্রন্থি& গ্রন্থি )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        যদি( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        যদি( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            অক্ষর * beg = ptr;
            size_t len = 0u;
            জন্য ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
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
                পূর্ণসংখ্যা i = 0, sz;

                ptr = beg;
                যৌক্তিক is_matching = মিথ্যা;
                যখন ( !is_matching )
                {
                    নির্বাচন ( *ptr )
                    {
                        ক্ষেত্র '\\':
                        {
                            sz = (পূর্ণসংখ্যা)(ptr - beg);
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
                            নির্বাচন ( *ptr )
                            {
                            ক্ষেত্র '\\':
                            ক্ষেত্র '\"':
                            ক্ষেত্র '\'': { buf[i++] = *ptr; বিরতি; }
                            ক্ষেত্র 'n' : { buf[i++] = '\n'; বিরতি; }
                            ক্ষেত্র 'r' : { buf[i++] = '\r'; বিরতি; }
                            ক্ষেত্র 't' : { buf[i++] = '\t'; বিরতি; }
                            ক্ষেত্র 'b' : { buf[i++] = '\b'; বিরতি; }
                            ক্ষেত্র 'f' : { buf[i++] = '\f'; বিরতি; }
                            ক্ষেত্র 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                যদি (i + (পূর্ণসংখ্যা)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.তথ্য(), utf8.size());
                                i += (পূর্ণসংখ্যা)utf8.size();
                                beg = ptr;
                                চালু;
                            }
                            পূর্বনির্ধারিত  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            বিরতি;
                            }
                            ptr++;
                            beg = ptr;
                            বিরতি;
                        }
                        ক্ষেত্র '\0':
                        {
                            sz = (পূর্ণসংখ্যা)(ptr - beg);
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
                            বিরতি;
                        }
                        ক্ষেত্র '\"':
                        {
                            sz = (পূর্ণসংখ্যা)(ptr - beg);
                            যদি( sz > 0 )
                            {
                                যদি (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = সত্য;
                            বিরতি;
                        }
                        ক্ষেত্র '\n':
                        ক্ষেত্র '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            বিরতি;
                        }
                        পূর্বনির্ধারিত:
                        {
                            ptr++;
                            বিরতি;
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
            অক্ষর * beg = ptr;
            যদি ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
            }
            যখন( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
            }
            যদি (*ptr == '.' || *ptr == 'e')
            {
                দ্বিগুণ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();

                গ্রন্থি.set_মান(File_গ্রন্থি::REAL, &fval);
            }
            অন্যথা
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();

                গ্রন্থি.set_মান(File_গ্রন্থি::INT, &ival);
            }

            যদি ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        অন্যথা
        {    /**************** other data ****************/
            ধ্রুবক অক্ষর* beg = ptr;
            পূর্ণসংখ্যা len = 0;
            জন্য ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
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

        ফেরত ptr;
    }

    অক্ষর* parseSeq( অক্ষর* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণসংখ্যা depth = 0 )
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

        জন্য (;;)
        {
            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                বিরতি;

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
                বিরতি;

            যদি ( *ptr == ',' )
                ptr++;
            অন্যথা যদি ( *ptr == ']' )
                বিরতি;
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
        ফেরত ptr;
    }

    অক্ষর* parseMap( অক্ষর* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণসংখ্যা depth = 0 )
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

        জন্য( ;; )
        {
            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                বিরতি;

            যদি ( *ptr == '"' )
            {
                File_গ্রন্থি child;
                ptr = parse_চাবি( ptr, গ্রন্থি, child );
                যদি( !ptr || !*ptr )
                    বিরতি;
                ptr = skipSpaces( ptr );
                যদি( !ptr || !*ptr )
                    বিরতি;

                যদি ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                অন্যথা যদি ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                অন্যথা
                    ptr = parse_মান( ptr, child );
            }

            ptr = skipSpaces( ptr );
            যদি( !ptr || !*ptr )
                বিরতি;

            যদি ( *ptr == ',' )
                ptr++;
            অন্যথা যদি ( *ptr == '}' )
                বিরতি;
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
        ফেরত ptr;
    }

    যৌক্তিক parse( অক্ষর* ptr )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        যদি ( !ptr || !*ptr )
            ফেরত মিথ্যা;

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

        ফেরত সত্য;
    }

    FileStorage_API* fs;
    অক্ষর buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ফেরত makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ফেরত makePtr<JSONParser>(fs);
}

}
