// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

namespace cv
{

class JSONEmitter : public FileStorageEmitter
{
public:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~JSONEmitter() {}

    FStructData startWriteStruct( const FStructData& parent, const char* key,
                                  цяло struct_flags, const char* type_name=0 )
    {
        char data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ако( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ако( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        ако (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = false;
        }

        ако ( is_real_collection )
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        върни current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        цяло struct_flags = current_struct.flags;

        ако (FileNode::isCollection(struct_flags)) {
            ако (!FileNode::isFlow(struct_flags)) {
                ако (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    char *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            char *ptr = fs->bufferPtr();
            ако (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    void write(const char* key, цяло value)
    {
        char buf[128];
        writeScalar( key, fs::itoa( value, buf, 10 ));
    }

    void write(const char* key, int64_t value)
    {
        char buf[128];
        writeScalar( key, fs::itoa( value, buf, 10, true ));
    }

    void write( const char* key, double value )
    {
        char buf[128];
        writeScalar( key, fs::doubleToString( buf, sizeof(buf), value, true ));
    }

    void write(const char* key, const char* str, bool quote)
    {
        char buf[CV_FS_MAX_LEN*6+16];
        char* data = (char*)str;
        цяло i, len;

        ако( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (цяло)strlen(str);
        ако( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ако( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            цяло need_quote = 1;
            data = buf;
            *data++ = '\"';
            за( i = 0; i < len; i++ )
            {
                char c = str[i];
                const unsigned char byte = static_cast<unsigned char>(c);

                switch ( c )
                {
                case '\\':
                case '\"': { *data++ = '\\'; *data++ = c;   break; }
                case '\n': { *data++ = '\\'; *data++ = 'n'; break; }
                case '\r': { *data++ = '\\'; *data++ = 'r'; break; }
                case '\t': { *data++ = '\\'; *data++ = 't'; break; }
                case '\b': { *data++ = '\\'; *data++ = 'b'; break; }
                case '\f': { *data++ = '\\'; *data++ = 'f'; break; }
                default  :
                    ако (byte < 0x20)
                    {
                        static const char hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    иначе
                        *data++ = c;
                    break;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const char* key, const char* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(false);
        ако ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        иначе ако ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        ако( key && *key == '\0' )
            key = 0;
        ако ( key )
        {
            key_len = strlen(key);
            ако ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            иначе ако ( static_cast<цяло>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ако ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        цяло struct_flags = current_struct.flags;
        ако( FileNode::isCollection(struct_flags) )
        {
            ако ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } иначе {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        char* ptr = 0;

        ако( FileNode::isFlow(struct_flags) )
        {
            цяло new_offset;
            ptr = fs->bufferPtr();
            ако( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<цяло>(ptr - fs->bufferStart() + key_len + data_len);
            ако( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            иначе
                *ptr++ = ' ';
        }
        иначе
        {
            ако ( !FileNode::isEmptyCollection(struct_flags) )
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

        ако( key )
        {
            ако( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<цяло>(key_len) );
            *ptr++ = '\"';

            за( size_t i = 0u; i < key_len; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                ако( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ако( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<цяло>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        ако( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        цяло len = static_cast<цяло>(strlen(comment));
        char* ptr = fs->bufferPtr();
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;

        ако( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        иначе
            *ptr++ = ' ';

        докато( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ако( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (цяло)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            иначе
            {
                len = (цяло)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    void startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

protected:
    FileStorage_API* fs;
};

class JSONParser : public FileStorageParser
{
public:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~JSONParser() {}

    char* skipSpaces( char* ptr )
    {
        bool is_eof = false;
        bool is_completed = false;

        докато ( is_eof == false && is_completed == false )
        {
            ако (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            switch ( *ptr )
            {
                /* comment */
                case '/' : {
                    ptr++;
                    ако ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ако( !ptr || !*ptr ) { is_eof = true; break; }
                    }

                    ако ( *ptr == '/' )
                    {
                        докато ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ако ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ако( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            иначе
                            {
                                ptr++;
                            }
                        }
                    }
                    иначе ако ( *ptr == '*' )
                    {
                        ptr++;
                        за (;;)
                        {
                            ако ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ако( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            иначе ако ( *ptr == '*' )
                            {
                                ptr++;
                                ако ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ако( !ptr || !*ptr ) { is_eof = true; break; }
                                }
                                ако ( *ptr == '/' )
                                {
                                    ptr++;
                                    break;
                                }
                            }
                            иначе
                            {
                                ptr++;
                            }
                        }
                    }
                    иначе
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } break;
                    /* whitespace */
                case '\t':
                case ' ' : {
                    ptr++;
                } break;
                    /* newline || end mark */
                case '\0':
                case '\n':
                case '\r': {
                    ptr = fs->gets();
                    ако( !ptr || !*ptr ) { is_eof = true; break; }
                } break;
                    /* other character */
                default: {
                    ако( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } break;
            }
        }

        ако ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ако( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        върни ptr;
    }

    uint32_t parseUnicodeEscape(char*& ptr)
    {
        uint32_t codepoint = 0;
        за (цяло k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            char hex = *ptr;
            uint32_t digit = 0;
            ако      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            иначе ако (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            иначе ако (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            иначе CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        върни codepoint;
    }

    static void appendUtf8(std::string& out, uint32_t codepoint)
    {
        ако (codepoint < 0x80) {
            out += (char)codepoint;
        } иначе ако (codepoint < 0x800) {
            out += (char)(0xC0 | (codepoint >> 6));
            out += (char)(0x80 | (codepoint & 0x3F));
        } иначе ако (codepoint < 0x10000) {
            out += (char)(0xE0 | (codepoint >> 12));
            out += (char)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (char)(0x80 | (codepoint & 0x3F));
        } иначе {
            out += (char)(0xF0 | (codepoint >> 18));
            out += (char)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (char)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (char)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    void parseUnicodeEscapeToUtf8(char*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ако (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ако (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            ако (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        иначе ако (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    char* parseKey( char* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ако( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        char * beg = ptr + 1;
        std::string key_name;
        do {
            ако (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ако (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    continue;
                }
                key_name += *ptr;
            } иначе ако (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } докато( cv_isprint(*ptr) && *ptr != '"' );

        ако( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ако( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ако( !ptr || !*ptr )
            върни 0;

        ако( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        върни ++ptr;
    }

    bool getBase64Row(char* ptr, цяло /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr;
        ако( !ptr || !*ptr )
            върни false;

        // find end of the row
        докато( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ако ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        върни true;
    }

    char* parseValue( char* ptr, FileNode& node )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ако( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ако( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            char * beg = ptr;
            size_t len = 0u;
            за ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ако ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                ако ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                иначе
                    ptr++;
            }
            иначе
            {   /**************** normal string ****************/
                цяло i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                докато ( !is_matching )
                {
                    switch ( *ptr )
                    {
                        case '\\':
                        {
                            sz = (цяло)(ptr - beg);
                            ако( sz > 0 )
                            {
                                ако (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ако (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            switch ( *ptr )
                            {
                            case '\\':
                            case '\"':
                            case '\'': { buf[i++] = *ptr; break; }
                            case 'n' : { buf[i++] = '\n'; break; }
                            case 'r' : { buf[i++] = '\r'; break; }
                            case 't' : { buf[i++] = '\t'; break; }
                            case 'b' : { buf[i++] = '\b'; break; }
                            case 'f' : { buf[i++] = '\f'; break; }
                            case 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                ако (i + (цяло)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (цяло)utf8.size();
                                beg = ptr;
                                continue;
                            }
                            default  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            break;
                            }
                            ptr++;
                            beg = ptr;
                            break;
                        }
                        case '\0':
                        {
                            sz = (цяло)(ptr - beg);
                            ако( sz > 0 )
                            {
                                ако (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ако ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            break;
                        }
                        case '\"':
                        {
                            sz = (цяло)(ptr - beg);
                            ако( sz > 0 )
                            {
                                ако (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = true;
                            break;
                        }
                        case '\n':
                        case '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            break;
                        }
                        default:
                        {
                            ptr++;
                            break;
                        }
                    }
                }

                ако ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                иначе
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        иначе ако ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            char * beg = ptr;
            ако ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            докато( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ако (*ptr == '.' || *ptr == 'e')
            {
                double fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            иначе
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            ако ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        иначе
        {    /**************** other data ****************/
            const char* beg = ptr;
            цяло len = 0;
            за ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ако( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            иначе ако( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            иначе
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        върни ptr;
    }

    char* parseSeq( char* ptr, FileNode& node, цяло depth = 0 )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ако( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ако ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        иначе
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        за (;;)
        {
            ptr = skipSpaces( ptr );
            ако( !ptr || !*ptr )
                break;

            ако ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                ако ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                иначе ако ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                иначе
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ако( !ptr || !*ptr )
                break;

            ако ( *ptr == ',' )
                ptr++;
            иначе ако ( *ptr == ']' )
                break;
            иначе
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ако (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ако ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        иначе
            ptr++;

        fs->finalizeCollection(node);
        върни ptr;
    }

    char* parseMap( char* ptr, FileNode& node, цяло depth = 0 )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ако( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ако ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        иначе
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        за( ;; )
        {
            ptr = skipSpaces( ptr );
            ако( !ptr || !*ptr )
                break;

            ако ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                ако( !ptr || !*ptr )
                    break;
                ptr = skipSpaces( ptr );
                ако( !ptr || !*ptr )
                    break;

                ако ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                иначе ако ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                иначе
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ако( !ptr || !*ptr )
                break;

            ако ( *ptr == ',' )
                ptr++;
            иначе ако ( *ptr == '}' )
                break;
            иначе
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ако (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ако ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        иначе
            ptr++;

        fs->finalizeCollection(node);
        върни ptr;
    }

    bool parse( char* ptr )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ако ( !ptr || !*ptr )
            върни false;

        FileNode root_collection(fs->getFS(), 0, 0);

        ако( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        иначе ако ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        иначе
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        върни true;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    върни makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    върни makePtr<JSONParser>(fs);
}

}
