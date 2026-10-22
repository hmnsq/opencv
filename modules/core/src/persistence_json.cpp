// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence.hpp"

నామాకాశ cv
{

తరగతి JSONEmitter : బహిరంగ FileStorageEmitter
{
బహిరంగ:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    మాయా ~JSONEmitter() {}

    FStruct_దత్తాంశం ప్రారంభం_Write_Struct( స్థిరరాశి FStruct_దత్తాంశం& మాతృ, స్థిరరాశి అక్షరం* కీలకం,
                                  పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం* type_name=0 )
    {
        అక్షరం దత్తాంశం[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_కణుపు::TYPE_ముసుగు|File_కణుపు::FLOW)) | File_కణుపు::ఖాళీలు_2;
        ఒకవేళ( !File_కణుపు::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ఒకవేళ( type_name && *type_name == '\0' )
            type_name = 0;

        తార్కిక is_real_collection = నిజం;
        ఒకవేళ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_కణుపు::STR;
            దత్తాంశం[0] = '\0';
            is_real_collection = అబద్ధం;
        }

        ఒకవేళ ( is_real_collection )
        {
            అక్షరం c = File_కణుపు::isMap(struct_flags) ? '{' : '[';
            దత్తాంశం[0] = c;
            దత్తాంశం[1] = '\0';
        }

        writeScalar( కీలకం, దత్తాంశం );
        FStruct_దత్తాంశం current_struct("", struct_flags, మాతృ.indent + 4);

        తిరిగి current_struct;
    }

    శూన్య చివర_Write_Struct(స్థిరరాశి FStruct_దత్తాంశం& current_struct)
    {
        పూర్ణాంకం struct_flags = current_struct.flags;

        ఒకవేళ (File_కణుపు::isCollection(struct_flags)) {
            ఒకవేళ (!File_కణుపు::isFlow(struct_flags)) {
                ఒకవేళ (fs->బఫరు_Ptr() <= fs->బఫరు_ప్రారంభం() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    అక్షరం *ptr = fs->బఫరు_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->బఫరు_ప్రారంభం());
                    fs->సమితి_బఫరు_Ptr(fs->బఫరు_ప్రారంభం());
                }
                fs->flush();
            }

            అక్షరం *ptr = fs->బఫరు_Ptr();
            ఒకవేళ (ptr > fs->బఫరు_ప్రారంభం() + current_struct.indent && !File_కణుపు::is_ఖాళీలు_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_కణుపు::isMap(struct_flags) ? '}' : ']';
            fs->సమితి_బఫరు_Ptr(ptr);
        }
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం value)
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::itoa( value, buf, 10 ));
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, int64_t value)
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::itoa( value, buf, 10, నిజం ));
    }

    శూన్య లిఖ( స్థిరరాశి అక్షరం* కీలకం, ద్విగుణ value )
    {
        అక్షరం buf[128];
        writeScalar( కీలకం, fs::double_గమ్యం_String( buf, పరిమాణం(buf), value, నిజం ));
    }

    శూన్య లిఖ(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* str, తార్కిక quote)
    {
        అక్షరం buf[CV_FS_MAX_LEN*6+16];
        అక్షరం* దత్తాంశం = (అక్షరం*)str;
        పూర్ణాంకం i, len;

        ఒకవేళ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(str);
        ఒకవేళ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ఒకవేళ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            పూర్ణాంకం need_quote = 1;
            దత్తాంశం = buf;
            *దత్తాంశం++ = '\"';
            కొరకు( i = 0; i < len; i++ )
            {
                అక్షరం c = str[i];
                స్థిరరాశి గుర్తులేని అక్షరం byte = static_cast<గుర్తులేని అక్షరం>(c);

                ఎంపిక ( c )
                {
                స్థితి '\\':
                స్థితి '\"': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = c;   ఆపు; }
                స్థితి '\n': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 'n'; ఆపు; }
                స్థితి '\r': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 'r'; ఆపు; }
                స్థితి '\t': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 't'; ఆపు; }
                స్థితి '\b': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 'b'; ఆపు; }
                స్థితి '\f': { *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 'f'; ఆపు; }
                అప్రమేయ  :
                    ఒకవేళ (byte < 0x20)
                    {
                        స్థిర స్థిరరాశి అక్షరం hex[] = "0123456789abcdef";
                        *దత్తాంశం++ = '\\'; *దత్తాంశం++ = 'u'; *దత్తాంశం++ = '0'; *దత్తాంశం++ = '0';
                        *దత్తాంశం++ = hex[byte >> 4]; *దత్తాంశం++ = hex[byte & 0xf];
                    }
                    లేకపోతే
                        *దత్తాంశం++ = c;
                    ఆపు;
                }
            }

            *దత్తాంశం++ = '\"';
            *దత్తాంశం++ = '\0';
            దత్తాంశం = buf + !need_quote;
        }

        writeScalar( కీలకం, దత్తాంశం);
    }

    శూన్య writeScalar(స్థిరరాశి అక్షరం* కీలకం, స్థిరరాశి అక్షరం* దత్తాంశం)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(అబద్ధం);
        ఒకవేళ ( fs->get_state_యొక్క_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_గమ్యం_Base64_state( FileStorage_API::NotUse );
        }
        లేకపోతే ఒకవేళ ( fs->get_state_యొక్క_writing_base64() == FileStorage_API::లోపల_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t కీలకం_len = 0u;
        ఒకవేళ( కీలకం && *కీలకం == '\0' )
            కీలకం = 0;
        ఒకవేళ ( కీలకం )
        {
            కీలకం_len = అక్షరశ్రేణి_పొడవు(కీలకం);
            ఒకవేళ ( కీలకం_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            లేకపోతే ఒకవేళ ( static_cast<పూర్ణాంకం>(కీలకం_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t దత్తాంశం_len = 0u;
        ఒకవేళ ( దత్తాంశం )
            దత్తాంశం_len = అక్షరశ్రేణి_పొడవు(దత్తాంశం);

        FStruct_దత్తాంశం& current_struct = fs->getCurrentStruct();
        పూర్ణాంకం struct_flags = current_struct.flags;
        ఒకవేళ( File_కణుపు::isCollection(struct_flags) )
        {
            ఒకవేళ ( (File_కణుపు::isMap(struct_flags) ^ (కీలకం != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } లేకపోతే {
            fs->సమితి_Non_ఖాళీలు();
            struct_flags = File_కణుపు::ఖాళీలు_2 | (కీలకం ? File_కణుపు::MAP : File_కణుపు::SEQ);
        }

        // start to write
        అక్షరం* ptr = 0;

        ఒకవేళ( File_కణుపు::isFlow(struct_flags) )
        {
            పూర్ణాంకం new_స్థానభ్రంశం;
            ptr = fs->బఫరు_Ptr();
            ఒకవేళ( !File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
                *ptr++ = ',';
            new_స్థానభ్రంశం = static_cast<పూర్ణాంకం>(ptr - fs->బఫరు_ప్రారంభం() + కీలకం_len + దత్తాంశం_len);
            ఒకవేళ( new_స్థానభ్రంశం > fs->wrapMargin() && new_స్థానభ్రంశం - current_struct.indent > 10 )
            {
                fs->సమితి_బఫరు_Ptr(ptr);
                ptr = fs->flush();
            }
            లేకపోతే
                *ptr++ = ' ';
        }
        లేకపోతే
        {
            ఒకవేళ ( !File_కణుపు::is_ఖాళీలు_Collection(struct_flags) )
            {
                ptr = fs->బఫరు_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->బఫరు_ప్రారంభం() );
                fs->సమితి_బఫరు_Ptr(fs->బఫరు_ప్రారంభం());
            }
            ptr = fs->flush();
        }

        ఒకవేళ( కీలకం )
        {
            ఒకవేళ( !cv_isalpha(కీలకం[0]) && కీలకం[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_బఫరు( ptr, static_cast<పూర్ణాంకం>(కీలకం_len) );
            *ptr++ = '\"';

            కొరకు( size_t i = 0u; i < కీలకం_len; i++ )
            {
                అక్షరం c = కీలకం[i];

                ptr[i] = c;
                ఒకవేళ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += కీలకం_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ఒకవేళ( దత్తాంశం )
        {
            ptr = fs->resize_Write_బఫరు( ptr, static_cast<పూర్ణాంకం>(దత్తాంశం_len) );
            జ్ఞాపకం_నకలు( ptr, దత్తాంశం, దత్తాంశం_len );
            ptr += దత్తాంశం_len;
        }

        fs->సమితి_బఫరు_Ptr(ptr);
        current_struct.flags &= ~File_కణుపు::ఖాళీలు_2;
    }

    శూన్య writeComment(స్థిరరాశి అక్షరం* comment, తార్కిక eol_comment)
    {
        ఒకవేళ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        పూర్ణాంకం len = static_cast<పూర్ణాంకం>(అక్షరశ్రేణి_పొడవు(comment));
        అక్షరం* ptr = fs->బఫరు_Ptr();
        స్థిరరాశి అక్షరం* eol = strchr(comment, '\n');
        తార్కిక multiline = eol != 0;

        ఒకవేళ( !eol_comment || multiline || fs->బఫరు_చివర_2() - ptr < len || ptr == fs->బఫరు_ప్రారంభం() )
            ptr = fs->flush();
        లేకపోతే
            *ptr++ = ' ';

        అయితే( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ఒకవేళ( eol )
            {
                ptr = fs->resize_Write_బఫరు( ptr, (పూర్ణాంకం)(eol - comment) + 1 );
                జ్ఞాపకం_నకలు( ptr, comment, eol - comment + 1 );
                fs->సమితి_బఫరు_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            లేకపోతే
            {
                len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(comment);
                ptr = fs->resize_Write_బఫరు( ptr, len );
                జ్ఞాపకం_నకలు( ptr, comment, len );
                fs->సమితి_బఫరు_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    శూన్య ప్రారంభం_తదుపరి_ప్రవాహం()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

రక్షిత:
    FileStorage_API* fs;
};

తరగతి JSONParser : బహిరంగ FileStorageParser
{
బహిరంగ:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    మాయా ~JSONParser() {}

    అక్షరం* skipSpaces( అక్షరం* ptr )
    {
        తార్కిక is_eof = అబద్ధం;
        తార్కిక is_completed = అబద్ధం;

        అయితే ( is_eof == అబద్ధం && is_completed == అబద్ధం )
        {
            ఒకవేళ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            ఎంపిక ( *ptr )
            {
                /* comment */
                స్థితి '/' : {
                    ptr++;
                    ఒకవేళ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ఒకవేళ( !ptr || !*ptr ) { is_eof = నిజం; ఆపు; }
                    }

                    ఒకవేళ ( *ptr == '/' )
                    {
                        అయితే ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ఒకవేళ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ఒకవేళ( !ptr || !*ptr ) { is_eof = నిజం; ఆపు; }
                            }
                            లేకపోతే
                            {
                                ptr++;
                            }
                        }
                    }
                    లేకపోతే ఒకవేళ ( *ptr == '*' )
                    {
                        ptr++;
                        కొరకు (;;)
                        {
                            ఒకవేళ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ఒకవేళ( !ptr || !*ptr ) { is_eof = నిజం; ఆపు; }
                            }
                            లేకపోతే ఒకవేళ ( *ptr == '*' )
                            {
                                ptr++;
                                ఒకవేళ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ఒకవేళ( !ptr || !*ptr ) { is_eof = నిజం; ఆపు; }
                                }
                                ఒకవేళ ( *ptr == '/' )
                                {
                                    ptr++;
                                    ఆపు;
                                }
                            }
                            లేకపోతే
                            {
                                ptr++;
                            }
                        }
                    }
                    లేకపోతే
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ఆపు;
                    /* whitespace */
                స్థితి '\t':
                స్థితి ' ' : {
                    ptr++;
                } ఆపు;
                    /* newline || end mark */
                స్థితి '\0':
                స్థితి '\n':
                స్థితి '\r': {
                    ptr = fs->gets();
                    ఒకవేళ( !ptr || !*ptr ) { is_eof = నిజం; ఆపు; }
                } ఆపు;
                    /* other character */
                అప్రమేయ: {
                    ఒకవేళ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = నిజం;
                } ఆపు;
            }
        }

        ఒకవేళ ( is_eof || !is_completed )
        {
            ptr = fs->బఫరు_ప్రారంభం();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->సమితి_Eof();
            ఒకవేళ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        తిరిగి ptr;
    }

    uint32_t parseUnicodeEscape(అక్షరం*& ptr)
    {
        uint32_t codepoint = 0;
        కొరకు (పూర్ణాంకం k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
            అక్షరం hex = *ptr;
            uint32_t digit = 0;
            ఒకవేళ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            లేకపోతే ఒకవేళ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            లేకపోతే ఒకవేళ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            లేకపోతే CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        తిరిగి codepoint;
    }

    స్థిర శూన్య appendUtf8(std::string& out, uint32_t codepoint)
    {
        ఒకవేళ (codepoint < 0x80) {
            out += (అక్షరం)codepoint;
        } లేకపోతే ఒకవేళ (codepoint < 0x800) {
            out += (అక్షరం)(0xC0 | (codepoint >> 6));
            out += (అక్షరం)(0x80 | (codepoint & 0x3F));
        } లేకపోతే ఒకవేళ (codepoint < 0x10000) {
            out += (అక్షరం)(0xE0 | (codepoint >> 12));
            out += (అక్షరం)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (అక్షరం)(0x80 | (codepoint & 0x3F));
        } లేకపోతే {
            out += (అక్షరం)(0xF0 | (codepoint >> 18));
            out += (అక్షరం)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (అక్షరం)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (అక్షరం)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    శూన్య parse_Unicode_Escape_గమ్యం_Utf8(అక్షరం*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ఒకవేళ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ఒకవేళ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t దిగువ = parseUnicodeEscape(ptr);
            ఒకవేళ (దిగువ < 0xDC00 || దిగువ > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (దిగువ - 0xDC00u);
        }
        లేకపోతే ఒకవేళ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    అక్షరం* parse_కీలకం( అక్షరం* ptr, File_కణుపు& collection, File_కణుపు& విలువ_placeholder )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ఒకవేళ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        అక్షరం * beg = ptr + 1;
        std::string కీలకం_name;
        చేయి {
            ఒకవేళ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
                ఒకవేళ (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_గమ్యం_Utf8(ptr, కీలకం_name);
                    కొనసాగు;
                }
                కీలకం_name += *ptr;
            } లేకపోతే ఒకవేళ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                కీలకం_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
        } అయితే( cv_isprint(*ptr) && *ptr != '"' );

        ఒకవేళ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ఒకవేళ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        విలువ_placeholder = fs->కూడు_కణుపు(collection, కీలకం_name, File_కణుపు::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ఒకవేళ( !ptr || !*ptr )
            తిరిగి 0;

        ఒకవేళ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        తిరిగి ++ptr;
    }

    తార్కిక get_Base64_అడ్డువరుస(అక్షరం* ptr, పూర్ణాంకం /*indent*/, అక్షరం* &beg, అక్షరం* &end)
    {
        beg = end = ptr;
        ఒకవేళ( !ptr || !*ptr )
            తిరిగి అబద్ధం;

        // find end of the row
        అయితే( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ఒకవేళ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        తిరిగి నిజం;
    }

    అక్షరం* parse_విలువ( అక్షరం* ptr, File_కణుపు& కణుపు )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ఒకవేళ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ఒకవేళ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            అక్షరం * beg = ptr;
            size_t len = 0u;
            కొరకు ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ఒకవేళ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, కణుపు);

                ఒకవేళ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                లేకపోతే
                    ptr++;
            }
            లేకపోతే
            {   /**************** normal string ****************/
                పూర్ణాంకం i = 0, sz;

                ptr = beg;
                తార్కిక is_matching = అబద్ధం;
                అయితే ( !is_matching )
                {
                    ఎంపిక ( *ptr )
                    {
                        స్థితి '\\':
                        {
                            sz = (పూర్ణాంకం)(ptr - beg);
                            ఒకవేళ( sz > 0 )
                            {
                                ఒకవేళ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                జ్ఞాపకం_నకలు(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ఒకవేళ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            ఎంపిక ( *ptr )
                            {
                            స్థితి '\\':
                            స్థితి '\"':
                            స్థితి '\'': { buf[i++] = *ptr; ఆపు; }
                            స్థితి 'n' : { buf[i++] = '\n'; ఆపు; }
                            స్థితి 'r' : { buf[i++] = '\r'; ఆపు; }
                            స్థితి 't' : { buf[i++] = '\t'; ఆపు; }
                            స్థితి 'b' : { buf[i++] = '\b'; ఆపు; }
                            స్థితి 'f' : { buf[i++] = '\f'; ఆపు; }
                            స్థితి 'u' : {
                                ptr++;
                                std::string utf8;
                                parse_Unicode_Escape_గమ్యం_Utf8(ptr, utf8);
                                ఒకవేళ (i + (పూర్ణాంకం)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                జ్ఞాపకం_నకలు(buf + i, utf8.దత్తాంశం(), utf8.size());
                                i += (పూర్ణాంకం)utf8.size();
                                beg = ptr;
                                కొనసాగు;
                            }
                            అప్రమేయ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ఆపు;
                            }
                            ptr++;
                            beg = ptr;
                            ఆపు;
                        }
                        స్థితి '\0':
                        {
                            sz = (పూర్ణాంకం)(ptr - beg);
                            ఒకవేళ( sz > 0 )
                            {
                                ఒకవేళ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                జ్ఞాపకం_నకలు(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ఒకవేళ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ఆపు;
                        }
                        స్థితి '\"':
                        {
                            sz = (పూర్ణాంకం)(ptr - beg);
                            ఒకవేళ( sz > 0 )
                            {
                                ఒకవేళ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                జ్ఞాపకం_నకలు(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = నిజం;
                            ఆపు;
                        }
                        స్థితి '\n':
                        స్థితి '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ఆపు;
                        }
                        అప్రమేయ:
                        {
                            ptr++;
                            ఆపు;
                        }
                    }
                }

                ఒకవేళ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                లేకపోతే
                    ptr++;

                కణుపు.సమితి_విలువ(File_కణుపు::STRING, buf, i);
            }
        }
        లేకపోతే ఒకవేళ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            అక్షరం * beg = ptr;
            ఒకవేళ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
            }
            అయితే( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
            }
            ఒకవేళ (*ptr == '.' || *ptr == 'e')
            {
                ద్విగుణ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();

                కణుపు.సమితి_విలువ(File_కణుపు::REAL, &fval);
            }
            లేకపోతే
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();

                కణుపు.సమితి_విలువ(File_కణుపు::INT, &ival);
            }

            ఒకవేళ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        లేకపోతే
        {    /**************** other data ****************/
            స్థిరరాశి అక్షరం* beg = ptr;
            పూర్ణాంకం len = 0;
            కొరకు ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_చివర_యొక్క_బఫరు_BUG_CPP();
            }

            ఒకవేళ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            లేకపోతే ఒకవేళ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                కణుపు.సమితి_విలువ(File_కణుపు::INT, &ival);
            }
            లేకపోతే
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        తిరిగి ptr;
    }

    అక్షరం* parseSeq( అక్షరం* ptr, File_కణుపు& కణుపు, పూర్ణాంకం లోతు = 0 )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ఒకవేళ( లోతు > CV_PERSISTENCE_MAX_లోతు )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ఒకవేళ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        లేకపోతే
            ptr++;

        fs->convert_గమ్యం_Collection(File_కణుపు::SEQ, కణుపు);

        కొరకు (;;)
        {
            ptr = skipSpaces( ptr );
            ఒకవేళ( !ptr || !*ptr )
                ఆపు;

            ఒకవేళ ( *ptr != ']' )
            {
                File_కణుపు బిడ్డ = fs->కూడు_కణుపు(కణుపు, std::string(), File_కణుపు::NONE );

                ఒకవేళ ( *ptr == '[' )
                    ptr = parseSeq( ptr, బిడ్డ, లోతు + 1 );
                లేకపోతే ఒకవేళ ( *ptr == '{' )
                    ptr = parseMap( ptr, బిడ్డ, లోతు + 1 );
                లేకపోతే
                    ptr = parse_విలువ( ptr, బిడ్డ );
            }

            ptr = skipSpaces( ptr );
            ఒకవేళ( !ptr || !*ptr )
                ఆపు;

            ఒకవేళ ( *ptr == ',' )
                ptr++;
            లేకపోతే ఒకవేళ ( *ptr == ']' )
                ఆపు;
            లేకపోతే
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ఒకవేళ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        లేకపోతే
            ptr++;

        fs->finalizeCollection(కణుపు);
        తిరిగి ptr;
    }

    అక్షరం* parseMap( అక్షరం* ptr, File_కణుపు& కణుపు, పూర్ణాంకం లోతు = 0 )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ఒకవేళ( లోతు > CV_PERSISTENCE_MAX_లోతు )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ఒకవేళ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        లేకపోతే
            ptr++;

        fs->convert_గమ్యం_Collection(File_కణుపు::MAP, కణుపు);

        కొరకు( ;; )
        {
            ptr = skipSpaces( ptr );
            ఒకవేళ( !ptr || !*ptr )
                ఆపు;

            ఒకవేళ ( *ptr == '"' )
            {
                File_కణుపు బిడ్డ;
                ptr = parse_కీలకం( ptr, కణుపు, బిడ్డ );
                ఒకవేళ( !ptr || !*ptr )
                    ఆపు;
                ptr = skipSpaces( ptr );
                ఒకవేళ( !ptr || !*ptr )
                    ఆపు;

                ఒకవేళ ( *ptr == '[' )
                    ptr = parseSeq( ptr, బిడ్డ, లోతు + 1 );
                లేకపోతే ఒకవేళ ( *ptr == '{' )
                    ptr = parseMap( ptr, బిడ్డ, లోతు + 1 );
                లేకపోతే
                    ptr = parse_విలువ( ptr, బిడ్డ );
            }

            ptr = skipSpaces( ptr );
            ఒకవేళ( !ptr || !*ptr )
                ఆపు;

            ఒకవేళ ( *ptr == ',' )
                ptr++;
            లేకపోతే ఒకవేళ ( *ptr == '}' )
                ఆపు;
            లేకపోతే
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ఒకవేళ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        లేకపోతే
            ptr++;

        fs->finalizeCollection(కణుపు);
        తిరిగి ptr;
    }

    తార్కిక parse( అక్షరం* ptr )
    {
        ఒకవేళ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ఒకవేళ ( !ptr || !*ptr )
            తిరిగి అబద్ధం;

        File_కణుపు మూలం_collection(fs->getFS(), 0, 0);

        ఒకవేళ( *ptr == '{' )
        {
            File_కణుపు మూలం_కణుపు = fs->కూడు_కణుపు(మూలం_collection, std::string(), File_కణుపు::MAP);
            parseMap( ptr, మూలం_కణుపు );
        }
        లేకపోతే ఒకవేళ ( *ptr == '[' )
        {
            File_కణుపు మూలం_కణుపు = fs->కూడు_కణుపు(మూలం_collection, std::string(), File_కణుపు::SEQ);
            parseSeq( ptr, మూలం_కణుపు );
        }
        లేకపోతే
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        తిరిగి నిజం;
    }

    FileStorage_API* fs;
    అక్షరం buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    తిరిగి makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    తిరిగి makePtr<JSONParser>(fs);
}

}
