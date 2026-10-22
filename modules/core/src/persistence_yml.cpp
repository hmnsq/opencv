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
                                 ຈຳນວນເຕັມ struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        ຖ້າ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ຖ້າ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ຖ້າ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ຖ້າ( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ບໍ່ດັ່ງນັ້ນ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ຖ້າ( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ສົ່ງຄືນ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        ຈຳນວນເຕັມ struct_flags = current_struct.flags;

        ຖ້າ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ຖ້າ( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, ຈຳນວນເຕັມ value)
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
        ຈຳນວນເຕັມ i, len;

        ຖ້າ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ຈຳນວນເຕັມ)strlen(str);
        ຖ້າ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ຖ້າ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ຈຳນວນເຕັມ need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            ສຳລັບ( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ຖ້າ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ຖ້າ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ຖ້າ( cv_isprint(c) )
                        *data++ = c;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\n' )
                        *data++ = 'n';
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\r' )
                        *data++ = 'r';
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\t' )
                        *data++ = 't';
                    ບໍ່ດັ່ງນັ້ນ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ບໍ່ດັ່ງນັ້ນ
                    *data++ = c;
            }
            ຖ້າ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ຖ້າ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ຖ້າ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ຈຳນວນເຕັມ i, keylen = 0;
        ຈຳນວນເຕັມ datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ຈຳນວນເຕັມ struct_flags = current_struct.flags;

        ຖ້າ( key && key[0] == '\0' )
            key = 0;

        ຖ້າ( FileNode::isCollection(struct_flags) )
        {
            ຖ້າ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ຖ້າ( key )
        {
            keylen = (ຈຳນວນເຕັມ)strlen(key);
            ຖ້າ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ຖ້າ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ຖ້າ( data )
            datalen = (ຈຳນວນເຕັມ)strlen(data);

        ຖ້າ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ຖ້າ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ຈຳນວນເຕັມ new_offset = (ຈຳນວນເຕັມ)(ptr - fs->bufferStart()) + keylen + datalen;
            ຖ້າ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ບໍ່ດັ່ງນັ້ນ
                *ptr++ = ' ';
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ptr = fs->flush();
            ຖ້າ( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ຖ້າ( data )
                    *ptr++ = ' ';
            }
        }

        ຖ້າ( key )
        {
            ຖ້າ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            ສຳລັບ( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ຖ້າ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ຖ້າ( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ຖ້າ( data )
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
        ຖ້າ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ຈຳນວນເຕັມ len = (ຈຳນວນເຕັມ)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ຖ້າ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ບໍ່ດັ່ງນັ້ນ
            *ptr++ = ' ';

        ໃນຂະນະທີ່( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ຖ້າ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ຈຳນວນເຕັມ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                len = (ຈຳນວນເຕັມ)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ຈຳນວນເຕັມ min_indent, ຈຳນວນເຕັມ max_comment_indent )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ສຳລັບ(;;)
        {
            ໃນຂະນະທີ່( *ptr == ' ' )
                ptr++;
            ຖ້າ( *ptr == '#' )
            {
                ຖ້າ( ptr - fs->bufferStart() > max_comment_indent )
                    ສົ່ງຄືນ ptr;
                *ptr = '\0';
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( cv_isprint(*ptr) )
            {
                ຖ້າ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ຖ້າ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    ຈຳນວນເຕັມ l = (ຈຳນວນເຕັມ)strlen(ptr);
                    ຖ້າ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ບໍ່ດັ່ງນັ້ນ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ສົ່ງຄືນ ptr;
    }

    bool getBase64Row(var_na* ptr, ຈຳນວນເຕັມ indent, var_na* &beg, var_na* &end)
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ຖ້າ (!ptr || !*ptr)
            ສົ່ງຄືນ false; // end of file

        ຖ້າ (ptr - fs->bufferStart() != indent)
            ສົ່ງຄືນ false; // end of base64 data

        /* find end */
        ໃນຂະນະທີ່(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ຖ້າ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ສົ່ງຄືນ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ຖ້າ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ໃນຂະນະທີ່( cv_isprint(c) && c != ':' );

        ຖ້າ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ຖ້າ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ໃນຂະນະທີ່( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ສົ່ງຄືນ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ຈຳນວນເຕັມ min_indent, bool is_parent_flow, ຈຳນວນເຕັມ depth = 0 )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ຖ້າ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        ຈຳນວນເຕັມ value_type = FileNode::NONE;
        ຈຳນວນເຕັມ len;
        bool is_binary_string = false;
        bool is_user = false;

        ຖ້າ( c == '!' ) // handle explicit type specification
        {
            ຖ້າ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ຖ້າ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ໃນຂະນະທີ່( cv_isprint(d) && d != ' ' && d != '>' );

                ຖ້າ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ຖ້າ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ໃນຂະນະທີ່( cv_isprint(d) && d != ' ' );
            len = (ຈຳນວນເຕັມ)(endptr - ptr);
            ຖ້າ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ຖ້າ( len == 3 && !is_user )
            {
                ຖ້າ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( len == 5 && !is_user )
            {
                ຖ້າ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ (len == 6 && is_user)
            {
                ຖ້າ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ຖ້າ (d == '|')
                            bha_nga;
                    } ໃນຂະນະທີ່ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ຖ້າ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ຖ້າ( !is_user )
            {
                ຖ້າ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ຖ້າ( value_type == FileNode::INT )
                    goto force_int;
                ຖ້າ( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ຖ້າ (is_binary_string)
        {
            ຈຳນວນເຕັມ indent = static_cast<ຈຳນວນເຕັມ>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ໃນຂະນະທີ່( cv_isdigit(*endptr) )
                endptr++;
            ຖ້າ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ຖ້າ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\'' || c == '\"' ) // an explicit string
        {
            ຖ້າ( c == '\'' )
                ສຳລັບ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ຖ້າ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\'' )
                    {
                        c = *++ptr;
                        ຖ້າ( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ບໍ່ດັ່ງນັ້ນ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ບໍ່ດັ່ງນັ້ນ
                ສຳລັບ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ຖ້າ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\\' )
                    {
                        d = *++ptr;
                        ຖ້າ( d == '\'' )
                            buf[len++] = d;
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 'n' )
                            buf[len++] = '\n';
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 'r' )
                            buf[len++] = '\r';
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 't' )
                            buf[len++] = '\t';
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ຈຳນວນເຕັມ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ຈຳນວນເຕັມ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ຖ້າ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ບໍ່ດັ່ງນັ້ນ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ບໍ່ດັ່ງນັ້ນ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ຖ້າ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '[' || c == '{' ) // collection as a flow
        {
            ຈຳນວນເຕັມ new_min_indent = min_indent + !is_parent_flow;
            ຈຳນວນເຕັມ struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            ຈຳນວນເຕັມ nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            ສຳລັບ( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ຖ້າ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ຖ້າ( *ptr == '}' || *ptr == ']' )
                {
                    ຖ້າ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ຖ້າ( nelems != 0 )
                {
                    ຖ້າ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ຖ້າ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ຖ້າ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    ຖ້າ( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ຈຳນວນເຕັມ indent, struct_type;

            ຖ້າ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ຖ້າ( !is_parent_flow )
                {
                    ຖ້າ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ຖ້າ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ໃນຂະນະທີ່( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ຖ້າ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ຖ້າ( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ໃນຂະນະທີ່( str_end > ptr && c == ' ' );
                    str_end++;
                    ຖ້າ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ຖ້າ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ສົ່ງຄືນ ptr;
                        }
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ສົ່ງຄືນ ptr;
                        }
                        ບໍ່ດັ່ງນັ້ນ ຖ້າ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ສົ່ງຄືນ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (ຈຳນວນເຕັມ)(str_end - ptr));
                    ptr = endptr;
                    ສົ່ງຄືນ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ບໍ່ດັ່ງນັ້ນ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (ຈຳນວນເຕັມ)(ptr - fs->bufferStart());

            ສຳລັບ(;;)
            {
                FileNode elem;

                ຖ້າ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    c = *ptr++;
                    ຖ້າ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ຖ້າ( ptr - fs->bufferStart() != indent )
                {
                    ຖ້າ( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ບໍ່ດັ່ງນັ້ນ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ຖ້າ( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ສົ່ງຄືນ ptr;
    }

    bool parse( var_na* ptr )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        ສຳລັບ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            ສຳລັບ(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ຖ້າ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ຖ້າ( *ptr == '%' )
                {
                    ຖ້າ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ຖ້າ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( *ptr == '-' )
                {
                    ຖ້າ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( first )
                        bha_nga;
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ຖ້າ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( fs->eof() )
                    bha_nga;
                ບໍ່ດັ່ງນັ້ນ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ຖ້າ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ຖ້າ( !ptr || !ptr[0] )
                bha_nga;
            ຖ້າ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ຖ້າ( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ຖ້າ( !ptr )
                    bha_nga;
            }

            ຖ້າ( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ສົ່ງຄືນ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<YAMLParser>(fs);
}

}
