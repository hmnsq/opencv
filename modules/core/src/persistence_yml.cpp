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
                                 ចំនួនគត់ struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        ប្រសិនបើ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ប្រសិនបើ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ប្រសិនបើ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ប្រសិនបើ( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            បើមិនដូច្នេះ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ប្រសិនបើ( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ត្រឡប់ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        ចំនួនគត់ struct_flags = current_struct.flags;

        ប្រសិនបើ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ប្រសិនបើ( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        បើមិនដូច្នេះ ប្រសិនបើ( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, ចំនួនគត់ value)
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
        ចំនួនគត់ i, len;

        ប្រសិនបើ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ចំនួនគត់)strlen(str);
        ប្រសិនបើ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ប្រសិនបើ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ចំនួនគត់ need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            សម្រាប់( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ប្រសិនបើ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ប្រសិនបើ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ប្រសិនបើ( cv_isprint(c) )
                        *data++ = c;
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\n' )
                        *data++ = 'n';
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\r' )
                        *data++ = 'r';
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\t' )
                        *data++ = 't';
                    បើមិនដូច្នេះ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                បើមិនដូច្នេះ
                    *data++ = c;
            }
            ប្រសិនបើ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ប្រសិនបើ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ប្រសិនបើ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        បើមិនដូច្នេះ ប្រសិនបើ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ចំនួនគត់ i, keylen = 0;
        ចំនួនគត់ datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ចំនួនគត់ struct_flags = current_struct.flags;

        ប្រសិនបើ( key && key[0] == '\0' )
            key = 0;

        ប្រសិនបើ( FileNode::isCollection(struct_flags) )
        {
            ប្រសិនបើ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        បើមិនដូច្នេះ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ប្រសិនបើ( key )
        {
            keylen = (ចំនួនគត់)strlen(key);
            ប្រសិនបើ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ប្រសិនបើ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ប្រសិនបើ( data )
            datalen = (ចំនួនគត់)strlen(data);

        ប្រសិនបើ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ប្រសិនបើ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ចំនួនគត់ new_offset = (ចំនួនគត់)(ptr - fs->bufferStart()) + keylen + datalen;
            ប្រសិនបើ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            បើមិនដូច្នេះ
                *ptr++ = ' ';
        }
        បើមិនដូច្នេះ
        {
            ptr = fs->flush();
            ប្រសិនបើ( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ប្រសិនបើ( data )
                    *ptr++ = ' ';
            }
        }

        ប្រសិនបើ( key )
        {
            ប្រសិនបើ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            សម្រាប់( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ប្រសិនបើ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ប្រសិនបើ( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ប្រសិនបើ( data )
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
        ប្រសិនបើ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ចំនួនគត់ len = (ចំនួនគត់)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ប្រសិនបើ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        បើមិនដូច្នេះ
            *ptr++ = ' ';

        ខណៈពេល( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ប្រសិនបើ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ចំនួនគត់)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            បើមិនដូច្នេះ
            {
                len = (ចំនួនគត់)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ចំនួនគត់ min_indent, ចំនួនគត់ max_comment_indent )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        សម្រាប់(;;)
        {
            ខណៈពេល( *ptr == ' ' )
                ptr++;
            ប្រសិនបើ( *ptr == '#' )
            {
                ប្រសិនបើ( ptr - fs->bufferStart() > max_comment_indent )
                    ត្រឡប់ ptr;
                *ptr = '\0';
            }
            បើមិនដូច្នេះ ប្រសិនបើ( cv_isprint(*ptr) )
            {
                ប្រសិនបើ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            បើមិនដូច្នេះ ប្រសិនបើ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ប្រសិនបើ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                បើមិនដូច្នេះ
                {
                    ចំនួនគត់ l = (ចំនួនគត់)strlen(ptr);
                    ប្រសិនបើ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            បើមិនដូច្នេះ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ត្រឡប់ ptr;
    }

    bool getBase64Row(var_na* ptr, ចំនួនគត់ indent, var_na* &beg, var_na* &end)
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ប្រសិនបើ (!ptr || !*ptr)
            ត្រឡប់ false; // end of file

        ប្រសិនបើ (ptr - fs->bufferStart() != indent)
            ត្រឡប់ false; // end of base64 data

        /* find end */
        ខណៈពេល(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ប្រសិនបើ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ត្រឡប់ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ប្រសិនបើ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ខណៈពេល( cv_isprint(c) && c != ':' );

        ប្រសិនបើ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ប្រសិនបើ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ខណៈពេល( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ត្រឡប់ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ចំនួនគត់ min_indent, bool is_parent_flow, ចំនួនគត់ depth = 0 )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ប្រសិនបើ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        ចំនួនគត់ value_type = FileNode::NONE;
        ចំនួនគត់ len;
        bool is_binary_string = false;
        bool is_user = false;

        ប្រសិនបើ( c == '!' ) // handle explicit type specification
        {
            ប្រសិនបើ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ប្រសិនបើ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ខណៈពេល( cv_isprint(d) && d != ' ' && d != '>' );

                ប្រសិនបើ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ប្រសិនបើ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ខណៈពេល( cv_isprint(d) && d != ' ' );
            len = (ចំនួនគត់)(endptr - ptr);
            ប្រសិនបើ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ប្រសិនបើ( len == 3 && !is_user )
            {
                ប្រសិនបើ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                បើមិនដូច្នេះ ប្រសិនបើ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                បើមិនដូច្នេះ ប្រសិនបើ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                បើមិនដូច្នេះ ប្រសិនបើ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            បើមិនដូច្នេះ ប្រសិនបើ( len == 5 && !is_user )
            {
                ប្រសិនបើ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            បើមិនដូច្នេះ ប្រសិនបើ (len == 6 && is_user)
            {
                ប្រសិនបើ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ប្រសិនបើ (d == '|')
                            bha_nga;
                    } ខណៈពេល (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ប្រសិនបើ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ប្រសិនបើ( !is_user )
            {
                ប្រសិនបើ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ប្រសិនបើ( value_type == FileNode::INT )
                    goto force_int;
                ប្រសិនបើ( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ប្រសិនបើ (is_binary_string)
        {
            ចំនួនគត់ indent = static_cast<ចំនួនគត់>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        បើមិនដូច្នេះ ប្រសិនបើ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ខណៈពេល( cv_isdigit(*endptr) )
                endptr++;
            ប្រសិនបើ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            បើមិនដូច្នេះ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ប្រសិនបើ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        បើមិនដូច្នេះ ប្រសិនបើ( c == '\'' || c == '\"' ) // an explicit string
        {
            ប្រសិនបើ( c == '\'' )
                សម្រាប់( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ប្រសិនបើ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\'' )
                    {
                        c = *++ptr;
                        ប្រសិនបើ( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    បើមិនដូច្នេះ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            បើមិនដូច្នេះ
                សម្រាប់( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ប្រសិនបើ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\\' )
                    {
                        d = *++ptr;
                        ប្រសិនបើ( d == '\'' )
                            buf[len++] = d;
                        បើមិនដូច្នេះ ប្រសិនបើ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        បើមិនដូច្នេះ ប្រសិនបើ( d == 'n' )
                            buf[len++] = '\n';
                        បើមិនដូច្នេះ ប្រសិនបើ( d == 'r' )
                            buf[len++] = '\r';
                        បើមិនដូច្នេះ ប្រសិនបើ( d == 't' )
                            buf[len++] = '\t';
                        បើមិនដូច្នេះ ប្រសិនបើ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ចំនួនគត់ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ចំនួនគត់)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ប្រសិនបើ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            បើមិនដូច្នេះ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    បើមិនដូច្នេះ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ប្រសិនបើ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        បើមិនដូច្នេះ ប្រសិនបើ( c == '[' || c == '{' ) // collection as a flow
        {
            ចំនួនគត់ new_min_indent = min_indent + !is_parent_flow;
            ចំនួនគត់ struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            ចំនួនគត់ nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            សម្រាប់( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ប្រសិនបើ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ប្រសិនបើ( *ptr == '}' || *ptr == ']' )
                {
                    ប្រសិនបើ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ប្រសិនបើ( nelems != 0 )
                {
                    ប្រសិនបើ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ប្រសិនបើ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ប្រសិនបើ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                បើមិនដូច្នេះ
                {
                    ប្រសិនបើ( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        បើមិនដូច្នេះ
        {
            ចំនួនគត់ indent, struct_type;

            ប្រសិនបើ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ប្រសិនបើ( !is_parent_flow )
                {
                    ប្រសិនបើ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ប្រសិនបើ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ខណៈពេល( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ប្រសិនបើ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ប្រសិនបើ( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ខណៈពេល( str_end > ptr && c == ' ' );
                    str_end++;
                    ប្រសិនបើ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ប្រសិនបើ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ត្រឡប់ ptr;
                        }
                        បើមិនដូច្នេះ ប្រសិនបើ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ត្រឡប់ ptr;
                        }
                        បើមិនដូច្នេះ ប្រសិនបើ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ត្រឡប់ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (ចំនួនគត់)(str_end - ptr));
                    ptr = endptr;
                    ត្រឡប់ ptr;
                }
                struct_type = FileNode::MAP;
            }
            បើមិនដូច្នេះ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (ចំនួនគត់)(ptr - fs->bufferStart());

            សម្រាប់(;;)
            {
                FileNode elem;

                ប្រសិនបើ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                បើមិនដូច្នេះ
                {
                    c = *ptr++;
                    ប្រសិនបើ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ប្រសិនបើ( ptr - fs->bufferStart() != indent )
                {
                    ប្រសិនបើ( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    បើមិនដូច្នេះ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ប្រសិនបើ( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ត្រឡប់ ptr;
    }

    bool parse( var_na* ptr )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        សម្រាប់(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            សម្រាប់(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ប្រសិនបើ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ប្រសិនបើ( *ptr == '%' )
                {
                    ប្រសិនបើ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ប្រសិនបើ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                បើមិនដូច្នេះ ប្រសិនបើ( *ptr == '-' )
                {
                    ប្រសិនបើ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( first )
                        bha_nga;
                }
                បើមិនដូច្នេះ ប្រសិនបើ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ប្រសិនបើ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                បើមិនដូច្នេះ ប្រសិនបើ( fs->eof() )
                    bha_nga;
                បើមិនដូច្នេះ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ប្រសិនបើ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ប្រសិនបើ( !ptr || !ptr[0] )
                bha_nga;
            ប្រសិនបើ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ប្រសិនបើ( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ប្រសិនបើ( !ptr )
                    bha_nga;
            }

            ប្រសិនបើ( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ត្រឡប់ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ត្រឡប់ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ត្រឡប់ makePtr<YAMLParser>(fs);
}

}
