// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

enum
{
    CV_YML_INDENT = 3,
    CV_YML_INDENT_FLOW = 1
};

namespace cv
{

class YAMLEmitter : public FileStorageEmitter
{
public:
    YAMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~YAMLEmitter() {}

    FStructData startWriteStruct(const FStructData& parent, const char* key,
                                 ціле struct_flags, const char* type_name=0)
    {
        char buf[CV_FS_MAX_LEN + 1024];
        const char* data = 0;

        якщо ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        якщо( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        якщо (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, sizeof(buf), "!!binary |");
            data = buf;
        }
        інакше якщо( FileNode::isFlow(struct_flags))
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            якщо( type_name )
                snprintf( buf, sizeof(buf), "!!%s %c", type_name, c );
            інакше
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        інакше якщо( type_name )
        {
            snprintf( buf, sizeof(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        якщо( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        повернути fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        char* ptr;

        ціле struct_flags = current_struct.flags;

        якщо( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            якщо( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        інакше якщо( FileNode::isEmptyCollection(struct_flags) )
        {
            ptr = fs->flush();
            memcpy( ptr, FileNode::isMap(struct_flags) ? "{}" : "[]", 2 );
            fs->setBufferPtr(ptr + 2);
        }
        /*
        if( !FileNode::isFlow(parent_flags) )
            fs->struct_indent -= CV_YML_INDENT + FileNode::isFlow(struct_flags);
        CV_Assert( fs->struct_indent >= 0 );*/
    }

    void write(const char* key, ціле value)
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
        writeScalar( key, fs::doubleToString( buf, sizeof(buf), value, false ));
    }

    void write(const char* key, const char* str, bool quote)
    {
        char buf[CV_FS_MAX_LEN*4+16];
        char* data = (char*)str;
        ціле i, len;

        якщо( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ціле)strlen(str);
        якщо( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        якщо( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ціле need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            для( i = 0; i < len; i++ )
            {
                char c = str[i];

                якщо( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                якщо( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    якщо( cv_isprint(c) )
                        *data++ = c;
                    інакше якщо( c == '\n' )
                        *data++ = 'n';
                    інакше якщо( c == '\r' )
                        *data++ = 'r';
                    інакше якщо( c == '\t' )
                        *data++ = 't';
                    інакше
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                інакше
                    *data++ = c;
            }
            якщо( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            якщо( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        якщо ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        інакше якщо ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ціле i, keylen = 0;
        ціле datalen = 0;
        char* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ціле struct_flags = current_struct.flags;

        якщо( key && key[0] == '\0' )
            key = 0;

        якщо( FileNode::isCollection(struct_flags) )
        {
            якщо( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        інакше
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        якщо( key )
        {
            keylen = (ціле)strlen(key);
            якщо( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            якщо( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        якщо( data )
            datalen = (ціле)strlen(data);

        якщо( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            якщо( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ціле new_offset = (ціле)(ptr - fs->bufferStart()) + keylen + datalen;
            якщо( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            інакше
                *ptr++ = ' ';
        }
        інакше
        {
            ptr = fs->flush();
            якщо( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                якщо( data )
                    *ptr++ = ' ';
            }
        }

        якщо( key )
        {
            якщо( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            для( i = 0; i < keylen; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                якщо( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            якщо( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        якщо( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        якщо( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ціле len = (ціле)strlen(comment);
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        char* ptr = fs->bufferPtr();

        якщо( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        інакше
            *ptr++ = ' ';

        поки( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            якщо( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ціле)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            інакше
            {
                len = (ціле)strlen(comment);
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


class YAMLParser : public FileStorageParser
{
public:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~YAMLParser() {}

    char* skipSpaces( char* ptr, ціле min_indent, ціле max_comment_indent )
    {
        якщо (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        для(;;)
        {
            поки( *ptr == ' ' )
                ptr++;
            якщо( *ptr == '#' )
            {
                якщо( ptr - fs->bufferStart() > max_comment_indent )
                    повернути ptr;
                *ptr = '\0';
            }
            інакше якщо( cv_isprint(*ptr) )
            {
                якщо( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                break;
            }
            інакше якщо( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                якщо( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    break;
                }
                інакше
                {
                    ціле l = (ціле)strlen(ptr);
                    якщо( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            інакше
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        повернути ptr;
    }

    bool getBase64Row(char* ptr, ціле indent, char* &beg, char* &end)
    {
        якщо (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        якщо (!ptr || !*ptr)
            повернути false; // end of file

        якщо (ptr - fs->bufferStart() != indent)
            повернути false; // end of base64 data

        /* find end */
        поки(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        якщо (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        повернути true;
    }


    char* parseKey( char* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        якщо (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        char c;
        char *endptr = ptr - 1, *saveptr;

        якщо( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        поки( cv_isprint(c) && c != ':' );

        якщо( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        якщо( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        поки( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        повернути ptr;
    }

    char* parseValue( char* ptr, FileNode& node, ціле min_indent, bool is_parent_flow, ціле depth = 0 )
    {
        якщо (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        якщо( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        char* endptr = 0;
        char c = ptr[0], d = ptr[1];
        ціле value_type = FileNode::NONE;
        ціле len;
        bool is_binary_string = false;
        bool is_user = false;

        якщо( c == '!' ) // handle explicit type specification
        {
            якщо( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            якщо ( d == '<') //support of full type heading from YAML 1.2
            {
                const char* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                char* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                поки( cv_isprint(d) && d != ' ' && d != '>' );

                якщо ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    якщо ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
                    {
                        *typeEndPtr = ' ';
                        ptr += headingLength - 1;
                        is_user = true;
                        //value_type |= FileNode::USER;
                    }
                }
            }

            endptr = ptr++;
            do d = *++endptr;
            поки( cv_isprint(d) && d != ' ' );
            len = (ціле)(endptr - ptr);
            якщо( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            якщо( len == 3 && !is_user )
            {
                якщо( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                інакше якщо( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                інакше якщо( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                інакше якщо( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            інакше якщо( len == 5 && !is_user )
            {
                якщо( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            інакше якщо (len == 6 && is_user)
            {
                якщо( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        якщо (d == '|')
                            break;
                    } поки (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            якщо (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            якщо( !is_user )
            {
                якщо (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                якщо( value_type == FileNode::INT )
                    goto force_int;
                якщо( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        якщо (is_binary_string)
        {
            ціле indent = static_cast<ціле>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        інакше якщо( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            поки( cv_isdigit(*endptr) )
                endptr++;
            якщо( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                double fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            інакше
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            якщо( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        інакше якщо( c == '\'' || c == '\"' ) // an explicit string
        {
            якщо( c == '\'' )
                для( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    якщо( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    інакше якщо( c == '\'' )
                    {
                        c = *++ptr;
                        якщо( c != '\'' )
                            break;
                        buf[len++] = c;
                    }
                    інакше
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            інакше
                для( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    якщо( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    інакше якщо( c == '\"' )
                    {
                        ++ptr;
                        break;
                    }
                    інакше якщо( c == '\\' )
                    {
                        d = *++ptr;
                        якщо( d == '\'' )
                            buf[len++] = d;
                        інакше якщо( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        інакше якщо( d == 'n' )
                            buf[len++] = '\n';
                        інакше якщо( d == 'r' )
                            buf[len++] = '\r';
                        інакше якщо( d == 't' )
                            buf[len++] = '\t';
                        інакше якщо( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ціле val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ціле)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            якщо( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            інакше
                            {
                                buf[len++] = (char)val;
                                ptr = endptr;
                            }
                        }
                    }
                    інакше
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            якщо( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        інакше якщо( c == '[' || c == '{' ) // collection as a flow
        {
            ціле new_min_indent = min_indent + !is_parent_flow;
            ціле struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            ціле nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            для( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                якщо (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                якщо( *ptr == '}' || *ptr == ']' )
                {
                    якщо( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    break;
                }

                якщо( nelems != 0 )
                {
                    якщо( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    якщо (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                якщо( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                інакше
                {
                    якщо( *ptr == ']' )
                        break;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        інакше
        {
            ціле indent, struct_type;

            якщо( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                якщо( !is_parent_flow )
                {
                    якщо( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    якщо( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                поки( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                якщо( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                якщо( is_parent_flow || c != ':' )
                {
                    char* str_end = endptr;
                    do c = *--str_end;
                    поки( str_end > ptr && c == ' ' );
                    str_end++;
                    якщо ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        якщо (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            повернути ptr;
                        }
                        інакше якщо (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            повернути ptr;
                        }
                        інакше якщо (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            повернути ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (ціле)(str_end - ptr));
                    ptr = endptr;
                    повернути ptr;
                }
                struct_type = FileNode::MAP;
            }
            інакше
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (ціле)(ptr - fs->bufferStart());

            для(;;)
            {
                FileNode elem;

                якщо( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                інакше
                {
                    c = *ptr++;
                    якщо( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                якщо( ptr - fs->bufferStart() != indent )
                {
                    якщо( ptr - fs->bufferStart() < indent )
                        break;
                    інакше
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                якщо( memcmp( ptr, "...", 3 ) == 0 )
                    break;
            }
            fs->finalizeCollection(node);
        }

        повернути ptr;
    }

    bool parse( char* ptr )
    {
        якщо (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        для(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            для(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                якщо( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    break;
                }

                якщо( *ptr == '%' )
                {
                    якщо( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        якщо ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                інакше якщо( *ptr == '-' )
                {
                    якщо( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        break;
                    }
                    інакше якщо( first )
                        break;
                }
                інакше якщо( cv_isalnum(*ptr) || *ptr=='_')
                {
                    якщо( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    break;
                }
                інакше якщо( fs->eof() )
                    break;
                інакше
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            якщо( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            якщо( !ptr || !ptr[0] )
                break;
            якщо( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                якщо( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                якщо( !ptr )
                    break;
            }

            якщо( fs->eof() )
                break;
            ptr += 3;
            first = false;
        }

        повернути ok;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    повернути makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    повернути makePtr<YAMLParser>(fs);
}

}
