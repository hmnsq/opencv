// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence.hpp"

નામાકાશ cv
{

વર્ગ JSONEmitter : જાહેર FileStorageEmitter
{
જાહેર:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    આભાસી ~JSONEmitter() {}

    FStructData startWriteStruct( અચલ FStructData& parent, અચલ અક્ષર* ચાવી,
                                  પૂર્ણાંક struct_flags, અચલ અક્ષર* type_name=0 )
    {
        અક્ષર data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ગાંઠ::TYPE_MASK|File_ગાંઠ::FLOW)) | File_ગાંઠ::EMPTY;
        જો( !File_ગાંઠ::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        જો( type_name && *type_name == '\0' )
            type_name = 0;

        તાર્કિક is_real_collection = સાચું;
        જો (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ગાંઠ::STR;
            data[0] = '\0';
            is_real_collection = ખોટું;
        }

        જો ( is_real_collection )
        {
            અક્ષર c = File_ગાંઠ::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( ચાવી, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        પાછા current_struct;
    }

    શૂન્ય endWriteStruct(અચલ FStructData& current_struct)
    {
        પૂર્ણાંક struct_flags = current_struct.flags;

        જો (File_ગાંઠ::isCollection(struct_flags)) {
            જો (!File_ગાંઠ::isFlow(struct_flags)) {
                જો (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    અક્ષર *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            અક્ષર *ptr = fs->bufferPtr();
            જો (ptr > fs->bufferStart() + current_struct.indent && !File_ગાંઠ::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ગાંઠ::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, પૂર્ણાંક value)
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::itoa( value, buf, 10 ));
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, int64_t value)
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::itoa( value, buf, 10, સાચું ));
    }

    શૂન્ય લિખ( અચલ અક્ષર* ચાવી, દ્વિગુણ value )
    {
        અક્ષર buf[128];
        writeScalar( ચાવી, fs::doubleToString( buf, કદ(buf), value, સાચું ));
    }

    શૂન્ય લિખ(અચલ અક્ષર* ચાવી, અચલ અક્ષર* str, તાર્કિક quote)
    {
        અક્ષર buf[CV_FS_MAX_LEN*6+16];
        અક્ષર* data = (અક્ષર*)str;
        પૂર્ણાંક i, len;

        જો( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (પૂર્ણાંક)strlen(str);
        જો( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        જો( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            પૂર્ણાંક need_quote = 1;
            data = buf;
            *data++ = '\"';
            માટે( i = 0; i < len; i++ )
            {
                અક્ષર c = str[i];
                અચલ unsigned અક્ષર byte = static_cast<unsigned અક્ષર>(c);

                પસંદગી ( c )
                {
                સ્થિતિ '\\':
                સ્થિતિ '\"': { *data++ = '\\'; *data++ = c;   તોડો; }
                સ્થિતિ '\n': { *data++ = '\\'; *data++ = 'n'; તોડો; }
                સ્થિતિ '\r': { *data++ = '\\'; *data++ = 'r'; તોડો; }
                સ્થિતિ '\t': { *data++ = '\\'; *data++ = 't'; તોડો; }
                સ્થિતિ '\b': { *data++ = '\\'; *data++ = 'b'; તોડો; }
                સ્થિતિ '\f': { *data++ = '\\'; *data++ = 'f'; તોડો; }
                મૂળભૂત  :
                    જો (byte < 0x20)
                    {
                        સ્થિર અચલ અક્ષર hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    નહીંતર
                        *data++ = c;
                    તોડો;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( ચાવી, data);
    }

    શૂન્ય writeScalar(અચલ અક્ષર* ચાવી, અચલ અક્ષર* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ખોટું);
        જો ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        નહીંતર જો ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ચાવી_len = 0u;
        જો( ચાવી && *ચાવી == '\0' )
            ચાવી = 0;
        જો ( ચાવી )
        {
            ચાવી_len = strlen(ચાવી);
            જો ( ચાવી_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            નહીંતર જો ( static_cast<પૂર્ણાંક>(ચાવી_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        જો ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        પૂર્ણાંક struct_flags = current_struct.flags;
        જો( File_ગાંઠ::isCollection(struct_flags) )
        {
            જો ( (File_ગાંઠ::isMap(struct_flags) ^ (ચાવી != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } નહીંતર {
            fs->setNonEmpty();
            struct_flags = File_ગાંઠ::EMPTY | (ચાવી ? File_ગાંઠ::MAP : File_ગાંઠ::SEQ);
        }

        // start to write
        અક્ષર* ptr = 0;

        જો( File_ગાંઠ::isFlow(struct_flags) )
        {
            પૂર્ણાંક new_offset;
            ptr = fs->bufferPtr();
            જો( !File_ગાંઠ::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<પૂર્ણાંક>(ptr - fs->bufferStart() + ચાવી_len + data_len);
            જો( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            નહીંતર
                *ptr++ = ' ';
        }
        નહીંતર
        {
            જો ( !File_ગાંઠ::isEmptyCollection(struct_flags) )
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

        જો( ચાવી )
        {
            જો( !cv_isalpha(ચાવી[0]) && ચાવી[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<પૂર્ણાંક>(ચાવી_len) );
            *ptr++ = '\"';

            માટે( size_t i = 0u; i < ચાવી_len; i++ )
            {
                અક્ષર c = ચાવી[i];

                ptr[i] = c;
                જો( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ચાવી_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        જો( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<પૂર્ણાંક>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~File_ગાંઠ::EMPTY;
    }

    શૂન્ય writeComment(અચલ અક્ષર* comment, તાર્કિક eol_comment)
    {
        જો( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        પૂર્ણાંક len = static_cast<પૂર્ણાંક>(strlen(comment));
        અક્ષર* ptr = fs->bufferPtr();
        અચલ અક્ષર* eol = strchr(comment, '\n');
        તાર્કિક multiline = eol != 0;

        જો( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        નહીંતર
            *ptr++ = ' ';

        જ્યારે( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            જો( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (પૂર્ણાંક)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            નહીંતર
            {
                len = (પૂર્ણાંક)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    શૂન્ય startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

સુરક્ષિત:
    FileStorage_API* fs;
};

વર્ગ JSONParser : જાહેર FileStorageParser
{
જાહેર:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    આભાસી ~JSONParser() {}

    અક્ષર* skipSpaces( અક્ષર* ptr )
    {
        તાર્કિક is_eof = ખોટું;
        તાર્કિક is_completed = ખોટું;

        જ્યારે ( is_eof == ખોટું && is_completed == ખોટું )
        {
            જો (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            પસંદગી ( *ptr )
            {
                /* comment */
                સ્થિતિ '/' : {
                    ptr++;
                    જો ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        જો( !ptr || !*ptr ) { is_eof = સાચું; તોડો; }
                    }

                    જો ( *ptr == '/' )
                    {
                        જ્યારે ( *ptr != '\n' && *ptr != '\r' )
                        {
                            જો ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                જો( !ptr || !*ptr ) { is_eof = સાચું; તોડો; }
                            }
                            નહીંતર
                            {
                                ptr++;
                            }
                        }
                    }
                    નહીંતર જો ( *ptr == '*' )
                    {
                        ptr++;
                        માટે (;;)
                        {
                            જો ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                જો( !ptr || !*ptr ) { is_eof = સાચું; તોડો; }
                            }
                            નહીંતર જો ( *ptr == '*' )
                            {
                                ptr++;
                                જો ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    જો( !ptr || !*ptr ) { is_eof = સાચું; તોડો; }
                                }
                                જો ( *ptr == '/' )
                                {
                                    ptr++;
                                    તોડો;
                                }
                            }
                            નહીંતર
                            {
                                ptr++;
                            }
                        }
                    }
                    નહીંતર
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } તોડો;
                    /* whitespace */
                સ્થિતિ '\t':
                સ્થિતિ ' ' : {
                    ptr++;
                } તોડો;
                    /* newline || end mark */
                સ્થિતિ '\0':
                સ્થિતિ '\n':
                સ્થિતિ '\r': {
                    ptr = fs->gets();
                    જો( !ptr || !*ptr ) { is_eof = સાચું; તોડો; }
                } તોડો;
                    /* other character */
                મૂળભૂત: {
                    જો( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = સાચું;
                } તોડો;
            }
        }

        જો ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            જો( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        પાછા ptr;
    }

    uint32_t parseUnicodeEscape(અક્ષર*& ptr)
    {
        uint32_t codepoint = 0;
        માટે (પૂર્ણાંક k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            અક્ષર hex = *ptr;
            uint32_t digit = 0;
            જો      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            નહીંતર જો (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            નહીંતર જો (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            નહીંતર CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        પાછા codepoint;
    }

    સ્થિર શૂન્ય appendUtf8(std::string& out, uint32_t codepoint)
    {
        જો (codepoint < 0x80) {
            out += (અક્ષર)codepoint;
        } નહીંતર જો (codepoint < 0x800) {
            out += (અક્ષર)(0xC0 | (codepoint >> 6));
            out += (અક્ષર)(0x80 | (codepoint & 0x3F));
        } નહીંતર જો (codepoint < 0x10000) {
            out += (અક્ષર)(0xE0 | (codepoint >> 12));
            out += (અક્ષર)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (અક્ષર)(0x80 | (codepoint & 0x3F));
        } નહીંતર {
            out += (અક્ષર)(0xF0 | (codepoint >> 18));
            out += (અક્ષર)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (અક્ષર)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (અક્ષર)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    શૂન્ય parseUnicodeEscapeToUtf8(અક્ષર*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        જો (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            જો (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t નીચલું = parseUnicodeEscape(ptr);
            જો (નીચલું < 0xDC00 || નીચલું > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (નીચલું - 0xDC00u);
        }
        નહીંતર જો (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    અક્ષર* parse_ચાવી( અક્ષર* ptr, File_ગાંઠ& collection, File_ગાંઠ& કિંમત_placeholder )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        જો( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        અક્ષર * beg = ptr + 1;
        std::string ચાવી_name;
        કરો {
            જો (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                જો (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, ચાવી_name);
                    ચાલુ;
                }
                ચાવી_name += *ptr;
            } નહીંતર જો (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ચાવી_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } જ્યારે( cv_isprint(*ptr) && *ptr != '"' );

        જો( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        જો( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        કિંમત_placeholder = fs->add_ગાંઠ(collection, ચાવી_name, File_ગાંઠ::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        જો( !ptr || !*ptr )
            પાછા 0;

        જો( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        પાછા ++ptr;
    }

    તાર્કિક getBase64Row(અક્ષર* ptr, પૂર્ણાંક /*indent*/, અક્ષર* &beg, અક્ષર* &end)
    {
        beg = end = ptr;
        જો( !ptr || !*ptr )
            પાછા ખોટું;

        // find end of the row
        જ્યારે( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        જો ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        પાછા સાચું;
    }

    અક્ષર* parse_કિંમત( અક્ષર* ptr, File_ગાંઠ& ગાંઠ )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        જો( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        જો( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            અક્ષર * beg = ptr;
            size_t len = 0u;
            માટે ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            જો ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ગાંઠ);

                જો ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                નહીંતર
                    ptr++;
            }
            નહીંતર
            {   /**************** normal string ****************/
                પૂર્ણાંક i = 0, sz;

                ptr = beg;
                તાર્કિક is_matching = ખોટું;
                જ્યારે ( !is_matching )
                {
                    પસંદગી ( *ptr )
                    {
                        સ્થિતિ '\\':
                        {
                            sz = (પૂર્ણાંક)(ptr - beg);
                            જો( sz > 0 )
                            {
                                જો (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            જો (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            પસંદગી ( *ptr )
                            {
                            સ્થિતિ '\\':
                            સ્થિતિ '\"':
                            સ્થિતિ '\'': { buf[i++] = *ptr; તોડો; }
                            સ્થિતિ 'n' : { buf[i++] = '\n'; તોડો; }
                            સ્થિતિ 'r' : { buf[i++] = '\r'; તોડો; }
                            સ્થિતિ 't' : { buf[i++] = '\t'; તોડો; }
                            સ્થિતિ 'b' : { buf[i++] = '\b'; તોડો; }
                            સ્થિતિ 'f' : { buf[i++] = '\f'; તોડો; }
                            સ્થિતિ 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                જો (i + (પૂર્ણાંક)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (પૂર્ણાંક)utf8.size();
                                beg = ptr;
                                ચાલુ;
                            }
                            મૂળભૂત  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            તોડો;
                            }
                            ptr++;
                            beg = ptr;
                            તોડો;
                        }
                        સ્થિતિ '\0':
                        {
                            sz = (પૂર્ણાંક)(ptr - beg);
                            જો( sz > 0 )
                            {
                                જો (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            જો ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            તોડો;
                        }
                        સ્થિતિ '\"':
                        {
                            sz = (પૂર્ણાંક)(ptr - beg);
                            જો( sz > 0 )
                            {
                                જો (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = સાચું;
                            તોડો;
                        }
                        સ્થિતિ '\n':
                        સ્થિતિ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            તોડો;
                        }
                        મૂળભૂત:
                        {
                            ptr++;
                            તોડો;
                        }
                    }
                }

                જો ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                નહીંતર
                    ptr++;

                ગાંઠ.set_કિંમત(File_ગાંઠ::STRING, buf, i);
            }
        }
        નહીંતર જો ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            અક્ષર * beg = ptr;
            જો ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            જ્યારે( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            જો (*ptr == '.' || *ptr == 'e')
            {
                દ્વિગુણ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ગાંઠ.set_કિંમત(File_ગાંઠ::REAL, &fval);
            }
            નહીંતર
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                ગાંઠ.set_કિંમત(File_ગાંઠ::INT, &ival);
            }

            જો ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        નહીંતર
        {    /**************** other data ****************/
            અચલ અક્ષર* beg = ptr;
            પૂર્ણાંક len = 0;
            માટે ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            જો( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            નહીંતર જો( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ગાંઠ.set_કિંમત(File_ગાંઠ::INT, &ival);
            }
            નહીંતર
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        પાછા ptr;
    }

    અક્ષર* parseSeq( અક્ષર* ptr, File_ગાંઠ& ગાંઠ, પૂર્ણાંક depth = 0 )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        જો( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        જો ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        નહીંતર
            ptr++;

        fs->convertToCollection(File_ગાંઠ::SEQ, ગાંઠ);

        માટે (;;)
        {
            ptr = skipSpaces( ptr );
            જો( !ptr || !*ptr )
                તોડો;

            જો ( *ptr != ']' )
            {
                File_ગાંઠ child = fs->add_ગાંઠ(ગાંઠ, std::string(), File_ગાંઠ::NONE );

                જો ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                નહીંતર જો ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                નહીંતર
                    ptr = parse_કિંમત( ptr, child );
            }

            ptr = skipSpaces( ptr );
            જો( !ptr || !*ptr )
                તોડો;

            જો ( *ptr == ',' )
                ptr++;
            નહીંતર જો ( *ptr == ']' )
                તોડો;
            નહીંતર
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        જો (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        જો ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        નહીંતર
            ptr++;

        fs->finalizeCollection(ગાંઠ);
        પાછા ptr;
    }

    અક્ષર* parseMap( અક્ષર* ptr, File_ગાંઠ& ગાંઠ, પૂર્ણાંક depth = 0 )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        જો( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        જો ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        નહીંતર
            ptr++;

        fs->convertToCollection(File_ગાંઠ::MAP, ગાંઠ);

        માટે( ;; )
        {
            ptr = skipSpaces( ptr );
            જો( !ptr || !*ptr )
                તોડો;

            જો ( *ptr == '"' )
            {
                File_ગાંઠ child;
                ptr = parse_ચાવી( ptr, ગાંઠ, child );
                જો( !ptr || !*ptr )
                    તોડો;
                ptr = skipSpaces( ptr );
                જો( !ptr || !*ptr )
                    તોડો;

                જો ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                નહીંતર જો ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                નહીંતર
                    ptr = parse_કિંમત( ptr, child );
            }

            ptr = skipSpaces( ptr );
            જો( !ptr || !*ptr )
                તોડો;

            જો ( *ptr == ',' )
                ptr++;
            નહીંતર જો ( *ptr == '}' )
                તોડો;
            નહીંતર
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        જો (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        જો ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        નહીંતર
            ptr++;

        fs->finalizeCollection(ગાંઠ);
        પાછા ptr;
    }

    તાર્કિક parse( અક્ષર* ptr )
    {
        જો (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        જો ( !ptr || !*ptr )
            પાછા ખોટું;

        File_ગાંઠ મૂળ_collection(fs->getFS(), 0, 0);

        જો( *ptr == '{' )
        {
            File_ગાંઠ મૂળ_ગાંઠ = fs->add_ગાંઠ(મૂળ_collection, std::string(), File_ગાંઠ::MAP);
            parseMap( ptr, મૂળ_ગાંઠ );
        }
        નહીંતર જો ( *ptr == '[' )
        {
            File_ગાંઠ મૂળ_ગાંઠ = fs->add_ગાંઠ(મૂળ_collection, std::string(), File_ગાંઠ::SEQ);
            parseSeq( ptr, મૂળ_ગાંઠ );
        }
        નહીંતર
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        પાછા સાચું;
    }

    FileStorage_API* fs;
    અક્ષર buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    પાછા makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    પાછા makePtr<JSONParser>(fs);
}

}
