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
                                 정수 struct_flags, const char* type_name=0)
    {
        char buf[CV_FS_MAX_LEN + 1024];
        const char* data = 0;

        만약 ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        만약( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        만약 (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, sizeof(buf), "!!binary |");
            data = buf;
        }
        아니면 만약( FileNode::isFlow(struct_flags))
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            만약( type_name )
                snprintf( buf, sizeof(buf), "!!%s %c", type_name, c );
            아니면
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        아니면 만약( type_name )
        {
            snprintf( buf, sizeof(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        만약( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        반환 fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        char* ptr;

        정수 struct_flags = current_struct.flags;

        만약( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            만약( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        아니면 만약( FileNode::isEmptyCollection(struct_flags) )
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
        writeScalar( key, fs::doubleToString( buf, sizeof(buf), value, false ));
    }

    void write(const char* key, const char* str, bool quote)
    {
        char buf[CV_FS_MAX_LEN*4+16];
        char* data = (char*)str;
        정수 i, len;

        만약( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (정수)strlen(str);
        만약( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        만약( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            정수 need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            위해( i = 0; i < len; i++ )
            {
                char c = str[i];

                만약( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                만약( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    만약( cv_isprint(c) )
                        *data++ = c;
                    아니면 만약( c == '\n' )
                        *data++ = 'n';
                    아니면 만약( c == '\r' )
                        *data++ = 'r';
                    아니면 만약( c == '\t' )
                        *data++ = 't';
                    아니면
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                아니면
                    *data++ = c;
            }
            만약( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            만약( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        만약 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        아니면 만약 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        정수 i, keylen = 0;
        정수 datalen = 0;
        char* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        정수 struct_flags = current_struct.flags;

        만약( key && key[0] == '\0' )
            key = 0;

        만약( FileNode::isCollection(struct_flags) )
        {
            만약( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        아니면
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        만약( key )
        {
            keylen = (정수)strlen(key);
            만약( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            만약( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        만약( data )
            datalen = (정수)strlen(data);

        만약( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            만약( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            정수 new_offset = (정수)(ptr - fs->bufferStart()) + keylen + datalen;
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
            ptr = fs->flush();
            만약( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                만약( data )
                    *ptr++ = ' ';
            }
        }

        만약( key )
        {
            만약( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            위해( i = 0; i < keylen; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                만약( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            만약( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        만약( data )
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
        만약( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        정수 len = (정수)strlen(comment);
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        char* ptr = fs->bufferPtr();

        만약( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        아니면
            *ptr++ = ' ';

        동안( comment )
        {
            *ptr++ = '#';
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


class YAMLParser : public FileStorageParser
{
public:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~YAMLParser() {}

    char* skipSpaces( char* ptr, 정수 min_indent, 정수 max_comment_indent )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        위해(;;)
        {
            동안( *ptr == ' ' )
                ptr++;
            만약( *ptr == '#' )
            {
                만약( ptr - fs->bufferStart() > max_comment_indent )
                    반환 ptr;
                *ptr = '\0';
            }
            아니면 만약( cv_isprint(*ptr) )
            {
                만약( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                break;
            }
            아니면 만약( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                만약( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    break;
                }
                아니면
                {
                    정수 l = (정수)strlen(ptr);
                    만약( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            아니면
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        반환 ptr;
    }

    bool getBase64Row(char* ptr, 정수 indent, char* &beg, char* &end)
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        만약 (!ptr || !*ptr)
            반환 false; // end of file

        만약 (ptr - fs->bufferStart() != indent)
            반환 false; // end of base64 data

        /* find end */
        동안(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        만약 (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        반환 true;
    }


    char* parseKey( char* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        char c;
        char *endptr = ptr - 1, *saveptr;

        만약( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        동안( cv_isprint(c) && c != ':' );

        만약( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        만약( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        동안( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        반환 ptr;
    }

    char* parseValue( char* ptr, FileNode& node, 정수 min_indent, bool is_parent_flow, 정수 depth = 0 )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        만약( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        char* endptr = 0;
        char c = ptr[0], d = ptr[1];
        정수 value_type = FileNode::NONE;
        정수 len;
        bool is_binary_string = false;
        bool is_user = false;

        만약( c == '!' ) // handle explicit type specification
        {
            만약( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            만약 ( d == '<') //support of full type heading from YAML 1.2
            {
                const char* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                char* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                동안( cv_isprint(d) && d != ' ' && d != '>' );

                만약 ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    만약 ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            동안( cv_isprint(d) && d != ' ' );
            len = (정수)(endptr - ptr);
            만약( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            만약( len == 3 && !is_user )
            {
                만약( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                아니면 만약( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                아니면 만약( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                아니면 만약( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            아니면 만약( len == 5 && !is_user )
            {
                만약( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            아니면 만약 (len == 6 && is_user)
            {
                만약( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        만약 (d == '|')
                            break;
                    } 동안 (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            만약 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            만약( !is_user )
            {
                만약 (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                만약( value_type == FileNode::INT )
                    goto force_int;
                만약( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        만약 (is_binary_string)
        {
            정수 indent = static_cast<정수>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        아니면 만약( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            동안( cv_isdigit(*endptr) )
                endptr++;
            만약( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                double fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            아니면
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            만약( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        아니면 만약( c == '\'' || c == '\"' ) // an explicit string
        {
            만약( c == '\'' )
                위해( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    만약( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    아니면 만약( c == '\'' )
                    {
                        c = *++ptr;
                        만약( c != '\'' )
                            break;
                        buf[len++] = c;
                    }
                    아니면
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            아니면
                위해( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    만약( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    아니면 만약( c == '\"' )
                    {
                        ++ptr;
                        break;
                    }
                    아니면 만약( c == '\\' )
                    {
                        d = *++ptr;
                        만약( d == '\'' )
                            buf[len++] = d;
                        아니면 만약( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        아니면 만약( d == 'n' )
                            buf[len++] = '\n';
                        아니면 만약( d == 'r' )
                            buf[len++] = '\r';
                        아니면 만약( d == 't' )
                            buf[len++] = '\t';
                        아니면 만약( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            정수 val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (정수)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            만약( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            아니면
                            {
                                buf[len++] = (char)val;
                                ptr = endptr;
                            }
                        }
                    }
                    아니면
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            만약( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        아니면 만약( c == '[' || c == '{' ) // collection as a flow
        {
            정수 new_min_indent = min_indent + !is_parent_flow;
            정수 struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            정수 nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            위해( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                만약 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                만약( *ptr == '}' || *ptr == ']' )
                {
                    만약( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    break;
                }

                만약( nelems != 0 )
                {
                    만약( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    만약 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                만약( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                아니면
                {
                    만약( *ptr == ']' )
                        break;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        아니면
        {
            정수 indent, struct_type;

            만약( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                만약( !is_parent_flow )
                {
                    만약( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    만약( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                동안( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                만약( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                만약( is_parent_flow || c != ':' )
                {
                    char* str_end = endptr;
                    do c = *--str_end;
                    동안( str_end > ptr && c == ' ' );
                    str_end++;
                    만약 ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        만약 (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            반환 ptr;
                        }
                        아니면 만약 (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            반환 ptr;
                        }
                        아니면 만약 (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            반환 ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (정수)(str_end - ptr));
                    ptr = endptr;
                    반환 ptr;
                }
                struct_type = FileNode::MAP;
            }
            아니면
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (정수)(ptr - fs->bufferStart());

            위해(;;)
            {
                FileNode elem;

                만약( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                아니면
                {
                    c = *ptr++;
                    만약( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                만약( ptr - fs->bufferStart() != indent )
                {
                    만약( ptr - fs->bufferStart() < indent )
                        break;
                    아니면
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                만약( memcmp( ptr, "...", 3 ) == 0 )
                    break;
            }
            fs->finalizeCollection(node);
        }

        반환 ptr;
    }

    bool parse( char* ptr )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        위해(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            위해(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                만약( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    break;
                }

                만약( *ptr == '%' )
                {
                    만약( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        만약 ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                아니면 만약( *ptr == '-' )
                {
                    만약( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        break;
                    }
                    아니면 만약( first )
                        break;
                }
                아니면 만약( cv_isalnum(*ptr) || *ptr=='_')
                {
                    만약( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    break;
                }
                아니면 만약( fs->eof() )
                    break;
                아니면
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            만약( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            만약( !ptr || !ptr[0] )
                break;
            만약( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                만약( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                만약( !ptr )
                    break;
            }

            만약( fs->eof() )
                break;
            ptr += 3;
            first = false;
        }

        반환 ok;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    반환 makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    반환 makePtr<YAMLParser>(fs);
}

}
