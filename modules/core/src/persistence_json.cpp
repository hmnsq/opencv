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
                                  정수 struct_flags, const char* type_name=0 )
    {
        char data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        만약( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        만약( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        만약 (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = false;
        }

        만약 ( is_real_collection )
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        반환 current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        정수 struct_flags = current_struct.flags;

        만약 (FileNode::isCollection(struct_flags)) {
            만약 (!FileNode::isFlow(struct_flags)) {
                만약 (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
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
            만약 (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    void write(const char* key, 정수 value)
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
        정수 i, len;

        만약( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (정수)strlen(str);
        만약( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        만약( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            정수 need_quote = 1;
            data = buf;
            *data++ = '\"';
            위해( i = 0; i < len; i++ )
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
                    만약 (byte < 0x20)
                    {
                        static const char hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    아니면
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
        만약 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        아니면 만약 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        만약( key && *key == '\0' )
            key = 0;
        만약 ( key )
        {
            key_len = strlen(key);
            만약 ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            아니면 만약 ( static_cast<정수>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        만약 ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        정수 struct_flags = current_struct.flags;
        만약( FileNode::isCollection(struct_flags) )
        {
            만약 ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } 아니면 {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        char* ptr = 0;

        만약( FileNode::isFlow(struct_flags) )
        {
            정수 new_offset;
            ptr = fs->bufferPtr();
            만약( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<정수>(ptr - fs->bufferStart() + key_len + data_len);
            만약( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            아니면
                *ptr++ = ' ';
        }
        아니면
        {
            만약 ( !FileNode::isEmptyCollection(struct_flags) )
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

        만약( key )
        {
            만약( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<정수>(key_len) );
            *ptr++ = '\"';

            위해( size_t i = 0u; i < key_len; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                만약( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        만약( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<정수>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        만약( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        정수 len = static_cast<정수>(strlen(comment));
        char* ptr = fs->bufferPtr();
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;

        만약( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        아니면
            *ptr++ = ' ';

        동안( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            만약( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (정수)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            아니면
            {
                len = (정수)strlen(comment);
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

        동안 ( is_eof == false && is_completed == false )
        {
            만약 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            switch ( *ptr )
            {
                /* comment */
                case '/' : {
                    ptr++;
                    만약 ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        만약( !ptr || !*ptr ) { is_eof = true; break; }
                    }

                    만약 ( *ptr == '/' )
                    {
                        동안 ( *ptr != '\n' && *ptr != '\r' )
                        {
                            만약 ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                만약( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            아니면
                            {
                                ptr++;
                            }
                        }
                    }
                    아니면 만약 ( *ptr == '*' )
                    {
                        ptr++;
                        위해 (;;)
                        {
                            만약 ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                만약( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            아니면 만약 ( *ptr == '*' )
                            {
                                ptr++;
                                만약 ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    만약( !ptr || !*ptr ) { is_eof = true; break; }
                                }
                                만약 ( *ptr == '/' )
                                {
                                    ptr++;
                                    break;
                                }
                            }
                            아니면
                            {
                                ptr++;
                            }
                        }
                    }
                    아니면
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
                    만약( !ptr || !*ptr ) { is_eof = true; break; }
                } break;
                    /* other character */
                default: {
                    만약( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } break;
            }
        }

        만약 ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            만약( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        반환 ptr;
    }

    uint32_t parseUnicodeEscape(char*& ptr)
    {
        uint32_t codepoint = 0;
        위해 (정수 k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            char hex = *ptr;
            uint32_t digit = 0;
            만약      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            아니면 만약 (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            아니면 만약 (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            아니면 CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        반환 codepoint;
    }

    static void appendUtf8(std::string& out, uint32_t codepoint)
    {
        만약 (codepoint < 0x80) {
            out += (char)codepoint;
        } 아니면 만약 (codepoint < 0x800) {
            out += (char)(0xC0 | (codepoint >> 6));
            out += (char)(0x80 | (codepoint & 0x3F));
        } 아니면 만약 (codepoint < 0x10000) {
            out += (char)(0xE0 | (codepoint >> 12));
            out += (char)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (char)(0x80 | (codepoint & 0x3F));
        } 아니면 {
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
        만약 (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            만약 (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            만약 (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        아니면 만약 (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    char* parseKey( char* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        만약( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        char * beg = ptr + 1;
        std::string key_name;
        do {
            만약 (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                만약 (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    continue;
                }
                key_name += *ptr;
            } 아니면 만약 (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } 동안( cv_isprint(*ptr) && *ptr != '"' );

        만약( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        만약( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        만약( !ptr || !*ptr )
            반환 0;

        만약( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        반환 ++ptr;
    }

    bool getBase64Row(char* ptr, 정수 /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr;
        만약( !ptr || !*ptr )
            반환 false;

        // find end of the row
        동안( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        만약 ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        반환 true;
    }

    char* parseValue( char* ptr, FileNode& node )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        만약( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        만약( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            char * beg = ptr;
            size_t len = 0u;
            위해 ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            만약 ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                만약 ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                아니면
                    ptr++;
            }
            아니면
            {   /**************** normal string ****************/
                정수 i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                동안 ( !is_matching )
                {
                    switch ( *ptr )
                    {
                        case '\\':
                        {
                            sz = (정수)(ptr - beg);
                            만약( sz > 0 )
                            {
                                만약 (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            만약 (i + 1 >= CV_FS_MAX_LEN)
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
                                만약 (i + (정수)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (정수)utf8.size();
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
                            sz = (정수)(ptr - beg);
                            만약( sz > 0 )
                            {
                                만약 (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            만약 ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            break;
                        }
                        case '\"':
                        {
                            sz = (정수)(ptr - beg);
                            만약( sz > 0 )
                            {
                                만약 (i + sz >= CV_FS_MAX_LEN)
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

                만약 ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                아니면
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        아니면 만약 ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            char * beg = ptr;
            만약 ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            동안( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            만약 (*ptr == '.' || *ptr == 'e')
            {
                double fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            아니면
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            만약 ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        아니면
        {    /**************** other data ****************/
            const char* beg = ptr;
            정수 len = 0;
            위해 ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            만약( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            아니면 만약( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            아니면
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        반환 ptr;
    }

    char* parseSeq( char* ptr, FileNode& node, 정수 depth = 0 )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        만약( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        만약 ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        아니면
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        위해 (;;)
        {
            ptr = skipSpaces( ptr );
            만약( !ptr || !*ptr )
                break;

            만약 ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                만약 ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                아니면 만약 ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                아니면
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            만약( !ptr || !*ptr )
                break;

            만약 ( *ptr == ',' )
                ptr++;
            아니면 만약 ( *ptr == ']' )
                break;
            아니면
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        만약 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        만약 ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        아니면
            ptr++;

        fs->finalizeCollection(node);
        반환 ptr;
    }

    char* parseMap( char* ptr, FileNode& node, 정수 depth = 0 )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        만약( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        만약 ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        아니면
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        위해( ;; )
        {
            ptr = skipSpaces( ptr );
            만약( !ptr || !*ptr )
                break;

            만약 ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                만약( !ptr || !*ptr )
                    break;
                ptr = skipSpaces( ptr );
                만약( !ptr || !*ptr )
                    break;

                만약 ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                아니면 만약 ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                아니면
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            만약( !ptr || !*ptr )
                break;

            만약 ( *ptr == ',' )
                ptr++;
            아니면 만약 ( *ptr == '}' )
                break;
            아니면
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        만약 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        만약 ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        아니면
            ptr++;

        fs->finalizeCollection(node);
        반환 ptr;
    }

    bool parse( char* ptr )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        만약 ( !ptr || !*ptr )
            반환 false;

        FileNode root_collection(fs->getFS(), 0, 0);

        만약( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        아니면 만약 ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        아니면
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        반환 true;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    반환 makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    반환 makePtr<JSONParser>(fs);
}

}
