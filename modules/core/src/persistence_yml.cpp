// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

ga_nanaa
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

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 පූර්ණ_සංඛ්‍යා struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        නම් ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        නම්( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        නම් (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        නැතහොත් නම්( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            නම්( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            නැතහොත්
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        නැතහොත් නම්( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        නම්( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ආපසු fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        පූර්ණ_සංඛ්‍යා struct_flags = current_struct.flags;

        නම්( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            නම්( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        නැතහොත් නම්( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, පූර්ණ_සංඛ්‍යා value)
    {
        var_na buf[128];
        writeScalar( key, fs::itoa( value, buf, 10 ));
    }

    void write(const var_na* key, int64_t value)
    {
        var_na buf[128];
        writeScalar( key, fs::itoa( value, buf, 10, true ));
    }

    void write( const var_na* key, dviguu_na value )
    {
        var_na buf[128];
        writeScalar( key, fs::doubleToString( buf, parimaa_na(buf), value, false ));
    }

    void write(const var_na* key, const var_na* str, bool quote)
    {
        var_na buf[CV_FS_MAX_LEN*4+16];
        var_na* data = (var_na*)str;
        පූර්ණ_සංඛ්‍යා i, len;

        නම්( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (පූර්ණ_සංඛ්‍යා)strlen(str);
        නම්( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        නම්( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            පූර්ණ_සංඛ්‍යා need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            සඳහා( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                නම්( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                නම්( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    නම්( cv_isprint(c) )
                        *data++ = c;
                    නැතහොත් නම්( c == '\n' )
                        *data++ = 'n';
                    නැතහොත් නම්( c == '\r' )
                        *data++ = 'r';
                    නැතහොත් නම්( c == '\t' )
                        *data++ = 't';
                    නැතහොත්
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                නැතහොත්
                    *data++ = c;
            }
            නම්( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            නම්( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        නම් ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        නැතහොත් නම් ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        පූර්ණ_සංඛ්‍යා i, keylen = 0;
        පූර්ණ_සංඛ්‍යා datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        පූර්ණ_සංඛ්‍යා struct_flags = current_struct.flags;

        නම්( key && key[0] == '\0' )
            key = 0;

        නම්( FileNode::isCollection(struct_flags) )
        {
            නම්( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        නැතහොත්
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        නම්( key )
        {
            keylen = (පූර්ණ_සංඛ්‍යා)strlen(key);
            නම්( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            නම්( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        නම්( data )
            datalen = (පූර්ණ_සංඛ්‍යා)strlen(data);

        නම්( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            නම්( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            පූර්ණ_සංඛ්‍යා new_offset = (පූර්ණ_සංඛ්‍යා)(ptr - fs->bufferStart()) + keylen + datalen;
            නම්( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            නැතහොත්
                *ptr++ = ' ';
        }
        නැතහොත්
        {
            ptr = fs->flush();
            නම්( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                නම්( data )
                    *ptr++ = ' ';
            }
        }

        නම්( key )
        {
            නම්( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            සඳහා( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                නම්( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            නම්( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        නම්( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, datalen );
            memcpy( ptr, data, datalen );
            ptr += datalen;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        නම්( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        පූර්ණ_සංඛ්‍යා len = (පූර්ණ_සංඛ්‍යා)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        නම්( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        නැතහොත්
            *ptr++ = ' ';

        අතරතුර( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            නම්( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (පූර්ණ_සංඛ්‍යා)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            නැතහොත්
            {
                len = (පූර්ණ_සංඛ්‍යා)strlen(comment);
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

rak_sita:
    FileStorage_API* fs;
};


class YAMLParser : public FileStorageParser
{
public:
    YAMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~YAMLParser() {}

    var_na* skipSpaces( var_na* ptr, පූර්ණ_සංඛ්‍යා min_indent, පූර්ණ_සංඛ්‍යා max_comment_indent )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        සඳහා(;;)
        {
            අතරතුර( *ptr == ' ' )
                ptr++;
            නම්( *ptr == '#' )
            {
                නම්( ptr - fs->bufferStart() > max_comment_indent )
                    ආපසු ptr;
                *ptr = '\0';
            }
            නැතහොත් නම්( cv_isprint(*ptr) )
            {
                නම්( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            නැතහොත් නම්( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                නම්( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                නැතහොත්
                {
                    පූර්ණ_සංඛ්‍යා l = (පූර්ණ_සංඛ්‍යා)strlen(ptr);
                    නම්( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            නැතහොත්
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ආපසු ptr;
    }

    bool getBase64Row(var_na* ptr, පූර්ණ_සංඛ්‍යා indent, var_na* &beg, var_na* &end)
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        නම් (!ptr || !*ptr)
            ආපසු false; // end of file

        නම් (ptr - fs->bufferStart() != indent)
            ආපසු false; // end of base64 data

        /* find end */
        අතරතුර(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        නම් (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ආපසු true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        නම්( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        අතරතුර( cv_isprint(c) && c != ':' );

        නම්( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        නම්( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        අතරතුර( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ආපසු ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, පූර්ණ_සංඛ්‍යා min_indent, bool is_parent_flow, පූර්ණ_සංඛ්‍යා depth = 0 )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        නම්( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        පූර්ණ_සංඛ්‍යා value_type = FileNode::NONE;
        පූර්ණ_සංඛ්‍යා len;
        bool is_binary_string = false;
        bool is_user = false;

        නම්( c == '!' ) // handle explicit type specification
        {
            නම්( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            නම් ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                අතරතුර( cv_isprint(d) && d != ' ' && d != '>' );

                නම් ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    නම් ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            අතරතුර( cv_isprint(d) && d != ' ' );
            len = (පූර්ණ_සංඛ්‍යා)(endptr - ptr);
            නම්( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            නම්( len == 3 && !is_user )
            {
                නම්( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                නැතහොත් නම්( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                නැතහොත් නම්( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                නැතහොත් නම්( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            නැතහොත් නම්( len == 5 && !is_user )
            {
                නම්( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            නැතහොත් නම් (len == 6 && is_user)
            {
                නම්( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        නම් (d == '|')
                            bha_nga;
                    } අතරතුර (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            නම් (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            නම්( !is_user )
            {
                නම් (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                නම්( value_type == FileNode::INT )
                    goto force_int;
                නම්( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        නම් (is_binary_string)
        {
            පූර්ණ_සංඛ්‍යා indent = static_cast<පූර්ණ_සංඛ්‍යා>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        නැතහොත් නම්( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            අතරතුර( cv_isdigit(*endptr) )
                endptr++;
            නම්( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            නැතහොත්
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            නම්( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        නැතහොත් නම්( c == '\'' || c == '\"' ) // an explicit string
        {
            නම්( c == '\'' )
                සඳහා( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    නම්( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    නැතහොත් නම්( c == '\'' )
                    {
                        c = *++ptr;
                        නම්( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    නැතහොත්
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            නැතහොත්
                සඳහා( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    නම්( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    නැතහොත් නම්( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    නැතහොත් නම්( c == '\\' )
                    {
                        d = *++ptr;
                        නම්( d == '\'' )
                            buf[len++] = d;
                        නැතහොත් නම්( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        නැතහොත් නම්( d == 'n' )
                            buf[len++] = '\n';
                        නැතහොත් නම්( d == 'r' )
                            buf[len++] = '\r';
                        නැතහොත් නම්( d == 't' )
                            buf[len++] = '\t';
                        නැතහොත් නම්( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            පූර්ණ_සංඛ්‍යා val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (පූර්ණ_සංඛ්‍යා)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            නම්( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            නැතහොත්
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    නැතහොත්
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            නම්( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        නැතහොත් නම්( c == '[' || c == '{' ) // collection as a flow
        {
            පූර්ණ_සංඛ්‍යා new_min_indent = min_indent + !is_parent_flow;
            පූර්ණ_සංඛ්‍යා struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            පූර්ණ_සංඛ්‍යා nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            සඳහා( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                නම් (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                නම්( *ptr == '}' || *ptr == ']' )
                {
                    නම්( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                නම්( nelems != 0 )
                {
                    නම්( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    නම් (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                නම්( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                නැතහොත්
                {
                    නම්( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        නැතහොත්
        {
            පූර්ණ_සංඛ්‍යා indent, struct_type;

            නම්( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                නම්( !is_parent_flow )
                {
                    නම්( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    නම්( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                අතරතුර( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                නම්( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                නම්( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    අතරතුර( str_end > ptr && c == ' ' );
                    str_end++;
                    නම් ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        නම් (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ආපසු ptr;
                        }
                        නැතහොත් නම් (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ආපසු ptr;
                        }
                        නැතහොත් නම් (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ආපසු ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (පූර්ණ_සංඛ්‍යා)(str_end - ptr));
                    ptr = endptr;
                    ආපසු ptr;
                }
                struct_type = FileNode::MAP;
            }
            නැතහොත්
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (පූර්ණ_සංඛ්‍යා)(ptr - fs->bufferStart());

            සඳහා(;;)
            {
                FileNode elem;

                නම්( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                නැතහොත්
                {
                    c = *ptr++;
                    නම්( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                නම්( ptr - fs->bufferStart() != indent )
                {
                    නම්( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    නැතහොත්
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                නම්( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ආපසු ptr;
    }

    bool parse( var_na* ptr )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        සඳහා(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            සඳහා(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                නම්( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                නම්( *ptr == '%' )
                {
                    නම්( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        නම් ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                නැතහොත් නම්( *ptr == '-' )
                {
                    නම්( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    නැතහොත් නම්( first )
                        bha_nga;
                }
                නැතහොත් නම්( cv_isalnum(*ptr) || *ptr=='_')
                {
                    නම්( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                නැතහොත් නම්( fs->eof() )
                    bha_nga;
                නැතහොත්
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            නම්( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            නම්( !ptr || !ptr[0] )
                bha_nga;
            නම්( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                නම්( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                නම්( !ptr )
                    bha_nga;
            }

            නම්( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ආපසු ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ආපසු makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ආපසු makePtr<YAMLParser>(fs);
}

}
