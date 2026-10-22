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
                                 จำนวนเต็ม struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        ถ้า ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ถ้า( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ถ้า (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        มิฉะนั้น ถ้า( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ถ้า( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            มิฉะนั้น
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        มิฉะนั้น ถ้า( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ถ้า( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        คืน fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        จำนวนเต็ม struct_flags = current_struct.flags;

        ถ้า( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ถ้า( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        มิฉะนั้น ถ้า( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, จำนวนเต็ม value)
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
        จำนวนเต็ม i, len;

        ถ้า( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (จำนวนเต็ม)strlen(str);
        ถ้า( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ถ้า( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            จำนวนเต็ม need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            สำหรับ( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ถ้า( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ถ้า( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ถ้า( cv_isprint(c) )
                        *data++ = c;
                    มิฉะนั้น ถ้า( c == '\n' )
                        *data++ = 'n';
                    มิฉะนั้น ถ้า( c == '\r' )
                        *data++ = 'r';
                    มิฉะนั้น ถ้า( c == '\t' )
                        *data++ = 't';
                    มิฉะนั้น
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                มิฉะนั้น
                    *data++ = c;
            }
            ถ้า( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ถ้า( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ถ้า ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        มิฉะนั้น ถ้า ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        จำนวนเต็ม i, keylen = 0;
        จำนวนเต็ม datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        จำนวนเต็ม struct_flags = current_struct.flags;

        ถ้า( key && key[0] == '\0' )
            key = 0;

        ถ้า( FileNode::isCollection(struct_flags) )
        {
            ถ้า( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        มิฉะนั้น
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ถ้า( key )
        {
            keylen = (จำนวนเต็ม)strlen(key);
            ถ้า( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ถ้า( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ถ้า( data )
            datalen = (จำนวนเต็ม)strlen(data);

        ถ้า( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ถ้า( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            จำนวนเต็ม new_offset = (จำนวนเต็ม)(ptr - fs->bufferStart()) + keylen + datalen;
            ถ้า( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            มิฉะนั้น
                *ptr++ = ' ';
        }
        มิฉะนั้น
        {
            ptr = fs->flush();
            ถ้า( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ถ้า( data )
                    *ptr++ = ' ';
            }
        }

        ถ้า( key )
        {
            ถ้า( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            สำหรับ( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ถ้า( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ถ้า( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ถ้า( data )
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
        ถ้า( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        จำนวนเต็ม len = (จำนวนเต็ม)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ถ้า( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        มิฉะนั้น
            *ptr++ = ' ';

        ขณะ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ถ้า( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (จำนวนเต็ม)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            มิฉะนั้น
            {
                len = (จำนวนเต็ม)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, จำนวนเต็ม min_indent, จำนวนเต็ม max_comment_indent )
    {
        ถ้า (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        สำหรับ(;;)
        {
            ขณะ( *ptr == ' ' )
                ptr++;
            ถ้า( *ptr == '#' )
            {
                ถ้า( ptr - fs->bufferStart() > max_comment_indent )
                    คืน ptr;
                *ptr = '\0';
            }
            มิฉะนั้น ถ้า( cv_isprint(*ptr) )
            {
                ถ้า( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            มิฉะนั้น ถ้า( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ถ้า( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                มิฉะนั้น
                {
                    จำนวนเต็ม l = (จำนวนเต็ม)strlen(ptr);
                    ถ้า( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            มิฉะนั้น
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        คืน ptr;
    }

    bool getBase64Row(var_na* ptr, จำนวนเต็ม indent, var_na* &beg, var_na* &end)
    {
        ถ้า (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ถ้า (!ptr || !*ptr)
            คืน false; // end of file

        ถ้า (ptr - fs->bufferStart() != indent)
            คืน false; // end of base64 data

        /* find end */
        ขณะ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ถ้า (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        คืน true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ถ้า (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ถ้า( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ขณะ( cv_isprint(c) && c != ':' );

        ถ้า( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ถ้า( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ขณะ( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        คืน ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, จำนวนเต็ม min_indent, bool is_parent_flow, จำนวนเต็ม depth = 0 )
    {
        ถ้า (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ถ้า( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        จำนวนเต็ม value_type = FileNode::NONE;
        จำนวนเต็ม len;
        bool is_binary_string = false;
        bool is_user = false;

        ถ้า( c == '!' ) // handle explicit type specification
        {
            ถ้า( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ถ้า ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ขณะ( cv_isprint(d) && d != ' ' && d != '>' );

                ถ้า ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ถ้า ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ขณะ( cv_isprint(d) && d != ' ' );
            len = (จำนวนเต็ม)(endptr - ptr);
            ถ้า( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ถ้า( len == 3 && !is_user )
            {
                ถ้า( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                มิฉะนั้น ถ้า( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                มิฉะนั้น ถ้า( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                มิฉะนั้น ถ้า( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            มิฉะนั้น ถ้า( len == 5 && !is_user )
            {
                ถ้า( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            มิฉะนั้น ถ้า (len == 6 && is_user)
            {
                ถ้า( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ถ้า (d == '|')
                            bha_nga;
                    } ขณะ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ถ้า (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ถ้า( !is_user )
            {
                ถ้า (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ถ้า( value_type == FileNode::INT )
                    goto force_int;
                ถ้า( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ถ้า (is_binary_string)
        {
            จำนวนเต็ม indent = static_cast<จำนวนเต็ม>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        มิฉะนั้น ถ้า( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ขณะ( cv_isdigit(*endptr) )
                endptr++;
            ถ้า( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            มิฉะนั้น
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ถ้า( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        มิฉะนั้น ถ้า( c == '\'' || c == '\"' ) // an explicit string
        {
            ถ้า( c == '\'' )
                สำหรับ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ถ้า( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    มิฉะนั้น ถ้า( c == '\'' )
                    {
                        c = *++ptr;
                        ถ้า( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    มิฉะนั้น
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            มิฉะนั้น
                สำหรับ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ถ้า( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    มิฉะนั้น ถ้า( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    มิฉะนั้น ถ้า( c == '\\' )
                    {
                        d = *++ptr;
                        ถ้า( d == '\'' )
                            buf[len++] = d;
                        มิฉะนั้น ถ้า( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        มิฉะนั้น ถ้า( d == 'n' )
                            buf[len++] = '\n';
                        มิฉะนั้น ถ้า( d == 'r' )
                            buf[len++] = '\r';
                        มิฉะนั้น ถ้า( d == 't' )
                            buf[len++] = '\t';
                        มิฉะนั้น ถ้า( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            จำนวนเต็ม val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (จำนวนเต็ม)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ถ้า( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            มิฉะนั้น
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    มิฉะนั้น
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ถ้า( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        มิฉะนั้น ถ้า( c == '[' || c == '{' ) // collection as a flow
        {
            จำนวนเต็ม new_min_indent = min_indent + !is_parent_flow;
            จำนวนเต็ม struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            จำนวนเต็ม nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            สำหรับ( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ถ้า (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ถ้า( *ptr == '}' || *ptr == ']' )
                {
                    ถ้า( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ถ้า( nelems != 0 )
                {
                    ถ้า( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ถ้า (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ถ้า( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                มิฉะนั้น
                {
                    ถ้า( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        มิฉะนั้น
        {
            จำนวนเต็ม indent, struct_type;

            ถ้า( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ถ้า( !is_parent_flow )
                {
                    ถ้า( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ถ้า( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ขณะ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ถ้า( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ถ้า( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ขณะ( str_end > ptr && c == ' ' );
                    str_end++;
                    ถ้า ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ถ้า (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            คืน ptr;
                        }
                        มิฉะนั้น ถ้า (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            คืน ptr;
                        }
                        มิฉะนั้น ถ้า (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            คืน ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (จำนวนเต็ม)(str_end - ptr));
                    ptr = endptr;
                    คืน ptr;
                }
                struct_type = FileNode::MAP;
            }
            มิฉะนั้น
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (จำนวนเต็ม)(ptr - fs->bufferStart());

            สำหรับ(;;)
            {
                FileNode elem;

                ถ้า( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                มิฉะนั้น
                {
                    c = *ptr++;
                    ถ้า( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ถ้า( ptr - fs->bufferStart() != indent )
                {
                    ถ้า( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    มิฉะนั้น
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ถ้า( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        คืน ptr;
    }

    bool parse( var_na* ptr )
    {
        ถ้า (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        สำหรับ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            สำหรับ(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ถ้า( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ถ้า( *ptr == '%' )
                {
                    ถ้า( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ถ้า ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                มิฉะนั้น ถ้า( *ptr == '-' )
                {
                    ถ้า( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    มิฉะนั้น ถ้า( first )
                        bha_nga;
                }
                มิฉะนั้น ถ้า( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ถ้า( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                มิฉะนั้น ถ้า( fs->eof() )
                    bha_nga;
                มิฉะนั้น
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ถ้า( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ถ้า( !ptr || !ptr[0] )
                bha_nga;
            ถ้า( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ถ้า( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ถ้า( !ptr )
                    bha_nga;
            }

            ถ้า( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        คืน ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    คืน makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    คืน makePtr<YAMLParser>(fs);
}

}
