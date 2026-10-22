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
                                  整数 struct_flags, const char* type_name=0 )
    {
        char data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        如果( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        如果( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        如果 (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = false;
        }

        如果 ( is_real_collection )
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        返回 current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        整数 struct_flags = current_struct.flags;

        如果 (FileNode::isCollection(struct_flags)) {
            如果 (!FileNode::isFlow(struct_flags)) {
                如果 (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
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
            如果 (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    void write(const char* key, 整数 value)
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
        整数 i, len;

        如果( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (整数)strlen(str);
        如果( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        如果( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            整数 need_quote = 1;
            data = buf;
            *data++ = '\"';
            为( i = 0; i < len; i++ )
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
                    如果 (byte < 0x20)
                    {
                        static const char hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    否则
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
        如果 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        否则 如果 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        如果( key && *key == '\0' )
            key = 0;
        如果 ( key )
        {
            key_len = strlen(key);
            如果 ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            否则 如果 ( static_cast<整数>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        如果 ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        整数 struct_flags = current_struct.flags;
        如果( FileNode::isCollection(struct_flags) )
        {
            如果 ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } 否则 {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        char* ptr = 0;

        如果( FileNode::isFlow(struct_flags) )
        {
            整数 new_offset;
            ptr = fs->bufferPtr();
            如果( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<整数>(ptr - fs->bufferStart() + key_len + data_len);
            如果( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            否则
                *ptr++ = ' ';
        }
        否则
        {
            如果 ( !FileNode::isEmptyCollection(struct_flags) )
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

        如果( key )
        {
            如果( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<整数>(key_len) );
            *ptr++ = '\"';

            为( size_t i = 0u; i < key_len; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                如果( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        如果( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<整数>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        如果( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        整数 len = static_cast<整数>(strlen(comment));
        char* ptr = fs->bufferPtr();
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;

        如果( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        否则
            *ptr++ = ' ';

        当( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            如果( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (整数)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            否则
            {
                len = (整数)strlen(comment);
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

        当 ( is_eof == false && is_completed == false )
        {
            如果 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            switch ( *ptr )
            {
                /* comment */
                case '/' : {
                    ptr++;
                    如果 ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        如果( !ptr || !*ptr ) { is_eof = true; break; }
                    }

                    如果 ( *ptr == '/' )
                    {
                        当 ( *ptr != '\n' && *ptr != '\r' )
                        {
                            如果 ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                如果( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            否则
                            {
                                ptr++;
                            }
                        }
                    }
                    否则 如果 ( *ptr == '*' )
                    {
                        ptr++;
                        为 (;;)
                        {
                            如果 ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                如果( !ptr || !*ptr ) { is_eof = true; break; }
                            }
                            否则 如果 ( *ptr == '*' )
                            {
                                ptr++;
                                如果 ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    如果( !ptr || !*ptr ) { is_eof = true; break; }
                                }
                                如果 ( *ptr == '/' )
                                {
                                    ptr++;
                                    break;
                                }
                            }
                            否则
                            {
                                ptr++;
                            }
                        }
                    }
                    否则
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
                    如果( !ptr || !*ptr ) { is_eof = true; break; }
                } break;
                    /* other character */
                default: {
                    如果( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } break;
            }
        }

        如果 ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            如果( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        返回 ptr;
    }

    uint32_t parseUnicodeEscape(char*& ptr)
    {
        uint32_t codepoint = 0;
        为 (整数 k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            char hex = *ptr;
            uint32_t digit = 0;
            如果      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            否则 如果 (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            否则 如果 (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            否则 CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        返回 codepoint;
    }

    static void appendUtf8(std::string& out, uint32_t codepoint)
    {
        如果 (codepoint < 0x80) {
            out += (char)codepoint;
        } 否则 如果 (codepoint < 0x800) {
            out += (char)(0xC0 | (codepoint >> 6));
            out += (char)(0x80 | (codepoint & 0x3F));
        } 否则 如果 (codepoint < 0x10000) {
            out += (char)(0xE0 | (codepoint >> 12));
            out += (char)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (char)(0x80 | (codepoint & 0x3F));
        } 否则 {
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
        如果 (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            如果 (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            如果 (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        否则 如果 (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    char* parseKey( char* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        如果( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        char * beg = ptr + 1;
        std::string key_name;
        do {
            如果 (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                如果 (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    continue;
                }
                key_name += *ptr;
            } 否则 如果 (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } 当( cv_isprint(*ptr) && *ptr != '"' );

        如果( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        如果( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        如果( !ptr || !*ptr )
            返回 0;

        如果( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        返回 ++ptr;
    }

    bool getBase64Row(char* ptr, 整数 /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr;
        如果( !ptr || !*ptr )
            返回 false;

        // find end of the row
        当( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        如果 ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        返回 true;
    }

    char* parseValue( char* ptr, FileNode& node )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        如果( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        如果( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            char * beg = ptr;
            size_t len = 0u;
            为 ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            如果 ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                如果 ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                否则
                    ptr++;
            }
            否则
            {   /**************** normal string ****************/
                整数 i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                当 ( !is_matching )
                {
                    switch ( *ptr )
                    {
                        case '\\':
                        {
                            sz = (整数)(ptr - beg);
                            如果( sz > 0 )
                            {
                                如果 (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            如果 (i + 1 >= CV_FS_MAX_LEN)
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
                                如果 (i + (整数)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (整数)utf8.size();
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
                            sz = (整数)(ptr - beg);
                            如果( sz > 0 )
                            {
                                如果 (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            如果 ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            break;
                        }
                        case '\"':
                        {
                            sz = (整数)(ptr - beg);
                            如果( sz > 0 )
                            {
                                如果 (i + sz >= CV_FS_MAX_LEN)
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

                如果 ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                否则
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        否则 如果 ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            char * beg = ptr;
            如果 ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            当( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            如果 (*ptr == '.' || *ptr == 'e')
            {
                double fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            否则
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            如果 ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        否则
        {    /**************** other data ****************/
            const char* beg = ptr;
            整数 len = 0;
            为 ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            如果( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            否则 如果( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            否则
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        返回 ptr;
    }

    char* parseSeq( char* ptr, FileNode& node, 整数 depth = 0 )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        如果( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        如果 ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        否则
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        为 (;;)
        {
            ptr = skipSpaces( ptr );
            如果( !ptr || !*ptr )
                break;

            如果 ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                如果 ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                否则 如果 ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                否则
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            如果( !ptr || !*ptr )
                break;

            如果 ( *ptr == ',' )
                ptr++;
            否则 如果 ( *ptr == ']' )
                break;
            否则
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        如果 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        如果 ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        否则
            ptr++;

        fs->finalizeCollection(node);
        返回 ptr;
    }

    char* parseMap( char* ptr, FileNode& node, 整数 depth = 0 )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        如果( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        如果 ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        否则
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        为( ;; )
        {
            ptr = skipSpaces( ptr );
            如果( !ptr || !*ptr )
                break;

            如果 ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                如果( !ptr || !*ptr )
                    break;
                ptr = skipSpaces( ptr );
                如果( !ptr || !*ptr )
                    break;

                如果 ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                否则 如果 ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                否则
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            如果( !ptr || !*ptr )
                break;

            如果 ( *ptr == ',' )
                ptr++;
            否则 如果 ( *ptr == '}' )
                break;
            否则
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        如果 (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        如果 ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        否则
            ptr++;

        fs->finalizeCollection(node);
        返回 ptr;
    }

    bool parse( char* ptr )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        如果 ( !ptr || !*ptr )
            返回 false;

        FileNode root_collection(fs->getFS(), 0, 0);

        如果( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        否则 如果 ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        否则
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        返回 true;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    返回 makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    返回 makePtr<JSONParser>(fs);
}

}
