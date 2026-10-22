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
                                 số_nguyên struct_flags, const char* type_name=0)
    {
        char buf[CV_FS_MAX_LEN + 1024];
        const char* data = 0;

        nếu ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        nếu( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        nếu (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, sizeof(buf), "!!binary |");
            data = buf;
        }
        không_thì nếu( FileNode::isFlow(struct_flags))
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            nếu( type_name )
                snprintf( buf, sizeof(buf), "!!%s %c", type_name, c );
            không_thì
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        không_thì nếu( type_name )
        {
            snprintf( buf, sizeof(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        nếu( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        trả_về fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        char* ptr;

        số_nguyên struct_flags = current_struct.flags;

        nếu( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            nếu( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        không_thì nếu( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const char* key, số_nguyên value)
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
        số_nguyên i, len;

        nếu( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (số_nguyên)strlen(str);
        nếu( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        nếu( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            số_nguyên need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            cho( i = 0; i < len; i++ )
            {
                char c = str[i];

                nếu( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                nếu( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    nếu( cv_isprint(c) )
                        *data++ = c;
                    không_thì nếu( c == '\n' )
                        *data++ = 'n';
                    không_thì nếu( c == '\r' )
                        *data++ = 'r';
                    không_thì nếu( c == '\t' )
                        *data++ = 't';
                    không_thì
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                không_thì
                    *data++ = c;
            }
            nếu( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            nếu( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        nếu ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        không_thì nếu ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        số_nguyên i, keylen = 0;
        số_nguyên datalen = 0;
        char* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        số_nguyên struct_flags = current_struct.flags;

        nếu( key && key[0] == '\0' )
            key = 0;

        nếu( FileNode::isCollection(struct_flags) )
        {
            nếu( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        không_thì
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        nếu( key )
        {
            keylen = (số_nguyên)strlen(key);
            nếu( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            nếu( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        nếu( data )
            datalen = (số_nguyên)strlen(data);

        nếu( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            nếu( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            số_nguyên new_offset = (số_nguyên)(ptr - fs->bufferStart()) + keylen + datalen;
            nếu( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            không_thì
                *ptr++ = ' ';
        }
        không_thì
        {
            ptr = fs->flush();
            nếu( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                nếu( data )
                    *ptr++ = ' ';
            }
        }

        nếu( key )
        {
            nếu( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            cho( i = 0; i < keylen; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                nếu( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            nếu( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        nếu( data )
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
        nếu( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        số_nguyên len = (số_nguyên)strlen(comment);
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        char* ptr = fs->bufferPtr();

        nếu( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        không_thì
            *ptr++ = ' ';

        trong_khi( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            nếu( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (số_nguyên)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            không_thì
            {
                len = (số_nguyên)strlen(comment);
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

    char* skipSpaces( char* ptr, số_nguyên min_indent, số_nguyên max_comment_indent )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        cho(;;)
        {
            trong_khi( *ptr == ' ' )
                ptr++;
            nếu( *ptr == '#' )
            {
                nếu( ptr - fs->bufferStart() > max_comment_indent )
                    trả_về ptr;
                *ptr = '\0';
            }
            không_thì nếu( cv_isprint(*ptr) )
            {
                nếu( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                break;
            }
            không_thì nếu( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                nếu( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    break;
                }
                không_thì
                {
                    số_nguyên l = (số_nguyên)strlen(ptr);
                    nếu( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            không_thì
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        trả_về ptr;
    }

    bool getBase64Row(char* ptr, số_nguyên indent, char* &beg, char* &end)
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        nếu (!ptr || !*ptr)
            trả_về false; // end of file

        nếu (ptr - fs->bufferStart() != indent)
            trả_về false; // end of base64 data

        /* find end */
        trong_khi(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        nếu (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        trả_về true;
    }


    char* parseKey( char* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        char c;
        char *endptr = ptr - 1, *saveptr;

        nếu( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        trong_khi( cv_isprint(c) && c != ':' );

        nếu( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        nếu( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        trong_khi( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        trả_về ptr;
    }

    char* parseValue( char* ptr, FileNode& node, số_nguyên min_indent, bool is_parent_flow, số_nguyên depth = 0 )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        nếu( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        char* endptr = 0;
        char c = ptr[0], d = ptr[1];
        số_nguyên value_type = FileNode::NONE;
        số_nguyên len;
        bool is_binary_string = false;
        bool is_user = false;

        nếu( c == '!' ) // handle explicit type specification
        {
            nếu( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            nếu ( d == '<') //support of full type heading from YAML 1.2
            {
                const char* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                char* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                trong_khi( cv_isprint(d) && d != ' ' && d != '>' );

                nếu ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    nếu ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            trong_khi( cv_isprint(d) && d != ' ' );
            len = (số_nguyên)(endptr - ptr);
            nếu( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            nếu( len == 3 && !is_user )
            {
                nếu( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                không_thì nếu( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                không_thì nếu( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                không_thì nếu( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            không_thì nếu( len == 5 && !is_user )
            {
                nếu( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            không_thì nếu (len == 6 && is_user)
            {
                nếu( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        nếu (d == '|')
                            break;
                    } trong_khi (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            nếu (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            nếu( !is_user )
            {
                nếu (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                nếu( value_type == FileNode::INT )
                    goto force_int;
                nếu( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        nếu (is_binary_string)
        {
            số_nguyên indent = static_cast<số_nguyên>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        không_thì nếu( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            trong_khi( cv_isdigit(*endptr) )
                endptr++;
            nếu( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                double fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            không_thì
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            nếu( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        không_thì nếu( c == '\'' || c == '\"' ) // an explicit string
        {
            nếu( c == '\'' )
                cho( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    nếu( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    không_thì nếu( c == '\'' )
                    {
                        c = *++ptr;
                        nếu( c != '\'' )
                            break;
                        buf[len++] = c;
                    }
                    không_thì
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            không_thì
                cho( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    nếu( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    không_thì nếu( c == '\"' )
                    {
                        ++ptr;
                        break;
                    }
                    không_thì nếu( c == '\\' )
                    {
                        d = *++ptr;
                        nếu( d == '\'' )
                            buf[len++] = d;
                        không_thì nếu( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        không_thì nếu( d == 'n' )
                            buf[len++] = '\n';
                        không_thì nếu( d == 'r' )
                            buf[len++] = '\r';
                        không_thì nếu( d == 't' )
                            buf[len++] = '\t';
                        không_thì nếu( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            số_nguyên val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (số_nguyên)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            nếu( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            không_thì
                            {
                                buf[len++] = (char)val;
                                ptr = endptr;
                            }
                        }
                    }
                    không_thì
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            nếu( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        không_thì nếu( c == '[' || c == '{' ) // collection as a flow
        {
            số_nguyên new_min_indent = min_indent + !is_parent_flow;
            số_nguyên struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            số_nguyên nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            cho( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                nếu (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                nếu( *ptr == '}' || *ptr == ']' )
                {
                    nếu( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    break;
                }

                nếu( nelems != 0 )
                {
                    nếu( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    nếu (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                nếu( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                không_thì
                {
                    nếu( *ptr == ']' )
                        break;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        không_thì
        {
            số_nguyên indent, struct_type;

            nếu( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                nếu( !is_parent_flow )
                {
                    nếu( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    nếu( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                trong_khi( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                nếu( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                nếu( is_parent_flow || c != ':' )
                {
                    char* str_end = endptr;
                    do c = *--str_end;
                    trong_khi( str_end > ptr && c == ' ' );
                    str_end++;
                    nếu ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        nếu (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            trả_về ptr;
                        }
                        không_thì nếu (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            trả_về ptr;
                        }
                        không_thì nếu (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            trả_về ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (số_nguyên)(str_end - ptr));
                    ptr = endptr;
                    trả_về ptr;
                }
                struct_type = FileNode::MAP;
            }
            không_thì
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (số_nguyên)(ptr - fs->bufferStart());

            cho(;;)
            {
                FileNode elem;

                nếu( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                không_thì
                {
                    c = *ptr++;
                    nếu( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                nếu( ptr - fs->bufferStart() != indent )
                {
                    nếu( ptr - fs->bufferStart() < indent )
                        break;
                    không_thì
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                nếu( memcmp( ptr, "...", 3 ) == 0 )
                    break;
            }
            fs->finalizeCollection(node);
        }

        trả_về ptr;
    }

    bool parse( char* ptr )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        cho(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            cho(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                nếu( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    break;
                }

                nếu( *ptr == '%' )
                {
                    nếu( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        nếu ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                không_thì nếu( *ptr == '-' )
                {
                    nếu( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        break;
                    }
                    không_thì nếu( first )
                        break;
                }
                không_thì nếu( cv_isalnum(*ptr) || *ptr=='_')
                {
                    nếu( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    break;
                }
                không_thì nếu( fs->eof() )
                    break;
                không_thì
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            nếu( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            nếu( !ptr || !ptr[0] )
                break;
            nếu( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                nếu( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                nếu( !ptr )
                    break;
            }

            nếu( fs->eof() )
                break;
            ptr += 3;
            first = false;
        }

        trả_về ok;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    trả_về makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    trả_về makePtr<YAMLParser>(fs);
}

}
