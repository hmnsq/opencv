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
                                 цяло struct_flags, const char* type_name=0)
    {
        char buf[CV_FS_MAX_LEN + 1024];
        const char* data = 0;

        ако ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ако( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ако (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, sizeof(buf), "!!binary |");
            data = buf;
        }
        иначе ако( FileNode::isFlow(struct_flags))
        {
            char c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ако( type_name )
                snprintf( buf, sizeof(buf), "!!%s %c", type_name, c );
            иначе
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        иначе ако( type_name )
        {
            snprintf( buf, sizeof(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ако( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        върни fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        char* ptr;

        цяло struct_flags = current_struct.flags;

        ако( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ако( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        иначе ако( FileNode::isEmptyCollection(struct_flags) )
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
        writeScalar( key, fs::doubleToString( buf, sizeof(buf), value, false ));
    }

    void write(const char* key, const char* str, bool quote)
    {
        char buf[CV_FS_MAX_LEN*4+16];
        char* data = (char*)str;
        цяло i, len;

        ако( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (цяло)strlen(str);
        ако( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ако( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            цяло need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            за( i = 0; i < len; i++ )
            {
                char c = str[i];

                ако( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ако( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ако( cv_isprint(c) )
                        *data++ = c;
                    иначе ако( c == '\n' )
                        *data++ = 'n';
                    иначе ако( c == '\r' )
                        *data++ = 'r';
                    иначе ако( c == '\t' )
                        *data++ = 't';
                    иначе
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                иначе
                    *data++ = c;
            }
            ако( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ако( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ако ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        иначе ако ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        цяло i, keylen = 0;
        цяло datalen = 0;
        char* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        цяло struct_flags = current_struct.flags;

        ако( key && key[0] == '\0' )
            key = 0;

        ако( FileNode::isCollection(struct_flags) )
        {
            ако( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        иначе
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ако( key )
        {
            keylen = (цяло)strlen(key);
            ако( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ако( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ако( data )
            datalen = (цяло)strlen(data);

        ако( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ако( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            цяло new_offset = (цяло)(ptr - fs->bufferStart()) + keylen + datalen;
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
            ptr = fs->flush();
            ако( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ако( data )
                    *ptr++ = ' ';
            }
        }

        ако( key )
        {
            ако( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            за( i = 0; i < keylen; i++ )
            {
                char c = key[i];

                ptr[i] = c;
                ако( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ако( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ако( data )
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
        ако( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        цяло len = (цяло)strlen(comment);
        const char* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        char* ptr = fs->bufferPtr();

        ако( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        иначе
            *ptr++ = ' ';

        докато( comment )
        {
            *ptr++ = '#';
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


class YAMLParser : public FileStorageParser
{
public:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~YAMLParser() {}

    char* skipSpaces( char* ptr, цяло min_indent, цяло max_comment_indent )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        за(;;)
        {
            докато( *ptr == ' ' )
                ptr++;
            ако( *ptr == '#' )
            {
                ако( ptr - fs->bufferStart() > max_comment_indent )
                    върни ptr;
                *ptr = '\0';
            }
            иначе ако( cv_isprint(*ptr) )
            {
                ако( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                break;
            }
            иначе ако( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ако( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    break;
                }
                иначе
                {
                    цяло l = (цяло)strlen(ptr);
                    ако( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            иначе
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        върни ptr;
    }

    bool getBase64Row(char* ptr, цяло indent, char* &beg, char* &end)
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ако (!ptr || !*ptr)
            върни false; // end of file

        ако (ptr - fs->bufferStart() != indent)
            върни false; // end of base64 data

        /* find end */
        докато(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ако (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        върни true;
    }


    char* parseKey( char* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        char c;
        char *endptr = ptr - 1, *saveptr;

        ако( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        докато( cv_isprint(c) && c != ':' );

        ако( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ако( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        докато( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        върни ptr;
    }

    char* parseValue( char* ptr, FileNode& node, цяло min_indent, bool is_parent_flow, цяло depth = 0 )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ако( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        char* endptr = 0;
        char c = ptr[0], d = ptr[1];
        цяло value_type = FileNode::NONE;
        цяло len;
        bool is_binary_string = false;
        bool is_user = false;

        ако( c == '!' ) // handle explicit type specification
        {
            ако( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ако ( d == '<') //support of full type heading from YAML 1.2
            {
                const char* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                char* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                докато( cv_isprint(d) && d != ' ' && d != '>' );

                ако ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ако ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            докато( cv_isprint(d) && d != ' ' );
            len = (цяло)(endptr - ptr);
            ако( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ако( len == 3 && !is_user )
            {
                ако( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                иначе ако( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                иначе ако( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                иначе ако( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            иначе ако( len == 5 && !is_user )
            {
                ако( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            иначе ако (len == 6 && is_user)
            {
                ако( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ако (d == '|')
                            break;
                    } докато (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ако (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ако( !is_user )
            {
                ако (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ако( value_type == FileNode::INT )
                    goto force_int;
                ако( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ако (is_binary_string)
        {
            цяло indent = static_cast<цяло>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        иначе ако( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            докато( cv_isdigit(*endptr) )
                endptr++;
            ако( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                double fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            иначе
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ако( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        иначе ако( c == '\'' || c == '\"' ) // an explicit string
        {
            ако( c == '\'' )
                за( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ако( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    иначе ако( c == '\'' )
                    {
                        c = *++ptr;
                        ако( c != '\'' )
                            break;
                        buf[len++] = c;
                    }
                    иначе
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            иначе
                за( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ако( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    иначе ако( c == '\"' )
                    {
                        ++ptr;
                        break;
                    }
                    иначе ако( c == '\\' )
                    {
                        d = *++ptr;
                        ако( d == '\'' )
                            buf[len++] = d;
                        иначе ако( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        иначе ако( d == 'n' )
                            buf[len++] = '\n';
                        иначе ако( d == 'r' )
                            buf[len++] = '\r';
                        иначе ако( d == 't' )
                            buf[len++] = '\t';
                        иначе ако( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            цяло val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (цяло)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ако( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            иначе
                            {
                                buf[len++] = (char)val;
                                ptr = endptr;
                            }
                        }
                    }
                    иначе
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ако( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        иначе ако( c == '[' || c == '{' ) // collection as a flow
        {
            цяло new_min_indent = min_indent + !is_parent_flow;
            цяло struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            цяло nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            за( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ако (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ако( *ptr == '}' || *ptr == ']' )
                {
                    ако( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    break;
                }

                ако( nelems != 0 )
                {
                    ако( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ако (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ако( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                иначе
                {
                    ако( *ptr == ']' )
                        break;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        иначе
        {
            цяло indent, struct_type;

            ако( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ако( !is_parent_flow )
                {
                    ако( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ако( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                докато( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ако( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ако( is_parent_flow || c != ':' )
                {
                    char* str_end = endptr;
                    do c = *--str_end;
                    докато( str_end > ptr && c == ' ' );
                    str_end++;
                    ако ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ако (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            върни ptr;
                        }
                        иначе ако (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            върни ptr;
                        }
                        иначе ако (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            върни ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (цяло)(str_end - ptr));
                    ptr = endptr;
                    върни ptr;
                }
                struct_type = FileNode::MAP;
            }
            иначе
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (цяло)(ptr - fs->bufferStart());

            за(;;)
            {
                FileNode elem;

                ако( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                иначе
                {
                    c = *ptr++;
                    ако( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ако( ptr - fs->bufferStart() != indent )
                {
                    ако( ptr - fs->bufferStart() < indent )
                        break;
                    иначе
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ако( memcmp( ptr, "...", 3 ) == 0 )
                    break;
            }
            fs->finalizeCollection(node);
        }

        върни ptr;
    }

    bool parse( char* ptr )
    {
        ако (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        за(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            за(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ако( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    break;
                }

                ако( *ptr == '%' )
                {
                    ако( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ако ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                иначе ако( *ptr == '-' )
                {
                    ако( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        break;
                    }
                    иначе ако( first )
                        break;
                }
                иначе ако( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ако( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    break;
                }
                иначе ако( fs->eof() )
                    break;
                иначе
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ако( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ако( !ptr || !ptr[0] )
                break;
            ако( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ако( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ако( !ptr )
                    break;
            }

            ако( fs->eof() )
                break;
            ptr += 3;
            first = false;
        }

        върни ok;
    }

    FileStorage_API* fs;
    char buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    върни makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    върни makePtr<YAMLParser>(fs);
}

}
