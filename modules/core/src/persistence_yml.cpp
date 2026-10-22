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
                                 puur_naa_nka struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        ပေႃး ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ပေႃး( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ပေႃး (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ပေႃးဢမ်ႇ ပေႃး( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ပေႃး( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ပေႃးဢမ်ႇ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ပေႃးဢမ်ႇ ပေႃး( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ပေႃး( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ပွၵ်ႈ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        puur_naa_nka struct_flags = current_struct.flags;

        ပေႃး( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ပေႃး( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ပေႃးဢမ်ႇ ပေႃး( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, puur_naa_nka value)
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
        puur_naa_nka i, len;

        ပေႃး( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ပေႃး( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ပေႃး( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ပေႃး( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ပေႃး( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ပေႃး( cv_isprint(c) )
                        *data++ = c;
                    ပေႃးဢမ်ႇ ပေႃး( c == '\n' )
                        *data++ = 'n';
                    ပေႃးဢမ်ႇ ပေႃး( c == '\r' )
                        *data++ = 'r';
                    ပေႃးဢမ်ႇ ပေႃး( c == '\t' )
                        *data++ = 't';
                    ပေႃးဢမ်ႇ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ပေႃးဢမ်ႇ
                    *data++ = c;
            }
            ပေႃး( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ပေႃး( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ပေႃး ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ပေႃးဢမ်ႇ ပေႃး ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka i, keylen = 0;
        puur_naa_nka datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        puur_naa_nka struct_flags = current_struct.flags;

        ပေႃး( key && key[0] == '\0' )
            key = 0;

        ပေႃး( FileNode::isCollection(struct_flags) )
        {
            ပေႃး( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ပေႃးဢမ်ႇ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ပေႃး( key )
        {
            keylen = (puur_naa_nka)strlen(key);
            ပေႃး( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ပေႃး( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ပေႃး( data )
            datalen = (puur_naa_nka)strlen(data);

        ပေႃး( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ပေႃး( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + keylen + datalen;
            ပေႃး( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ပေႃးဢမ်ႇ
                *ptr++ = ' ';
        }
        ပေႃးဢမ်ႇ
        {
            ptr = fs->flush();
            ပေႃး( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ပေႃး( data )
                    *ptr++ = ' ';
            }
        }

        ပေႃး( key )
        {
            ပေႃး( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            for( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ပေႃး( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ပေႃး( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ပေႃး( data )
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
        ပေႃး( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = (puur_naa_nka)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ပေႃး( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ပေႃးဢမ်ႇ
            *ptr++ = ' ';

        ၶၢဝ်း( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ပေႃး( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ပေႃးဢမ်ႇ
            {
                len = (puur_naa_nka)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, puur_naa_nka min_indent, puur_naa_nka max_comment_indent )
    {
        ပေႃး (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        for(;;)
        {
            ၶၢဝ်း( *ptr == ' ' )
                ptr++;
            ပေႃး( *ptr == '#' )
            {
                ပေႃး( ptr - fs->bufferStart() > max_comment_indent )
                    ပွၵ်ႈ ptr;
                *ptr = '\0';
            }
            ပေႃးဢမ်ႇ ပေႃး( cv_isprint(*ptr) )
            {
                ပေႃး( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ပေႃးဢမ်ႇ ပေႃး( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ပေႃး( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ပေႃးဢမ်ႇ
                {
                    puur_naa_nka l = (puur_naa_nka)strlen(ptr);
                    ပေႃး( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ပေႃးဢမ်ႇ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ပွၵ်ႈ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end)
    {
        ပေႃး (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ပေႃး (!ptr || !*ptr)
            ပွၵ်ႈ false; // end of file

        ပေႃး (ptr - fs->bufferStart() != indent)
            ပွၵ်ႈ false; // end of base64 data

        /* find end */
        ၶၢဝ်း(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ပေႃး (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ပွၵ်ႈ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ပေႃး (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ပေႃး( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ၶၢဝ်း( cv_isprint(c) && c != ':' );

        ပေႃး( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ပေႃး( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ၶၢဝ်း( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ပွၵ်ႈ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka min_indent, bool is_parent_flow, puur_naa_nka depth = 0 )
    {
        ပေႃး (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ပေႃး( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        puur_naa_nka value_type = FileNode::NONE;
        puur_naa_nka len;
        bool is_binary_string = false;
        bool is_user = false;

        ပေႃး( c == '!' ) // handle explicit type specification
        {
            ပေႃး( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ပေႃး ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ၶၢဝ်း( cv_isprint(d) && d != ' ' && d != '>' );

                ပေႃး ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ပေႃး ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ၶၢဝ်း( cv_isprint(d) && d != ' ' );
            len = (puur_naa_nka)(endptr - ptr);
            ပေႃး( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ပေႃး( len == 3 && !is_user )
            {
                ပေႃး( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ပေႃးဢမ်ႇ ပေႃး( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ပေႃးဢမ်ႇ ပေႃး( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ပေႃးဢမ်ႇ ပေႃး( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ပေႃးဢမ်ႇ ပေႃး( len == 5 && !is_user )
            {
                ပေႃး( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ပေႃးဢမ်ႇ ပေႃး (len == 6 && is_user)
            {
                ပေႃး( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ပေႃး (d == '|')
                            bha_nga;
                    } ၶၢဝ်း (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ပေႃး (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ပေႃး( !is_user )
            {
                ပေႃး (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ပေႃး( value_type == FileNode::INT )
                    goto force_int;
                ပေႃး( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ပေႃး (is_binary_string)
        {
            puur_naa_nka indent = static_cast<puur_naa_nka>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ပေႃးဢမ်ႇ ပေႃး( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ၶၢဝ်း( cv_isdigit(*endptr) )
                endptr++;
            ပေႃး( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ပေႃးဢမ်ႇ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ပေႃး( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ပေႃးဢမ်ႇ ပေႃး( c == '\'' || c == '\"' ) // an explicit string
        {
            ပေႃး( c == '\'' )
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ပေႃး( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ပေႃးဢမ်ႇ ပေႃး( c == '\'' )
                    {
                        c = *++ptr;
                        ပေႃး( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ပေႃးဢမ်ႇ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ပေႃးဢမ်ႇ
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ပေႃး( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ပေႃးဢမ်ႇ ပေႃး( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ပေႃးဢမ်ႇ ပေႃး( c == '\\' )
                    {
                        d = *++ptr;
                        ပေႃး( d == '\'' )
                            buf[len++] = d;
                        ပေႃးဢမ်ႇ ပေႃး( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ပေႃးဢမ်ႇ ပေႃး( d == 'n' )
                            buf[len++] = '\n';
                        ပေႃးဢမ်ႇ ပေႃး( d == 'r' )
                            buf[len++] = '\r';
                        ပေႃးဢမ်ႇ ပေႃး( d == 't' )
                            buf[len++] = '\t';
                        ပေႃးဢမ်ႇ ပေႃး( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            puur_naa_nka val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (puur_naa_nka)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ပေႃး( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ပေႃးဢမ်ႇ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ပေႃးဢမ်ႇ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ပေႃး( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ပေႃးဢမ်ႇ ပေႃး( c == '[' || c == '{' ) // collection as a flow
        {
            puur_naa_nka new_min_indent = min_indent + !is_parent_flow;
            puur_naa_nka struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            puur_naa_nka nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            for( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                ပေႃး (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ပေႃး( *ptr == '}' || *ptr == ']' )
                {
                    ပေႃး( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ပေႃး( nelems != 0 )
                {
                    ပေႃး( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ပေႃး (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ပေႃး( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ပေႃးဢမ်ႇ
                {
                    ပေႃး( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ပေႃးဢမ်ႇ
        {
            puur_naa_nka indent, struct_type;

            ပေႃး( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ပေႃး( !is_parent_flow )
                {
                    ပေႃး( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ပေႃး( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ၶၢဝ်း( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ပေႃး( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ပေႃး( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ၶၢဝ်း( str_end > ptr && c == ' ' );
                    str_end++;
                    ပေႃး ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ပေႃး (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ပွၵ်ႈ ptr;
                        }
                        ပေႃးဢမ်ႇ ပေႃး (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ပွၵ်ႈ ptr;
                        }
                        ပေႃးဢမ်ႇ ပေႃး (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ပွၵ်ႈ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (puur_naa_nka)(str_end - ptr));
                    ptr = endptr;
                    ပွၵ်ႈ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ပေႃးဢမ်ႇ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (puur_naa_nka)(ptr - fs->bufferStart());

            for(;;)
            {
                FileNode elem;

                ပေႃး( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ပေႃးဢမ်ႇ
                {
                    c = *ptr++;
                    ပေႃး( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ပေႃး( ptr - fs->bufferStart() != indent )
                {
                    ပေႃး( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ပေႃးဢမ်ႇ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ပေႃး( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ပွၵ်ႈ ptr;
    }

    bool parse( var_na* ptr )
    {
        ပေႃး (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        for(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            for(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ပေႃး( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ပေႃး( *ptr == '%' )
                {
                    ပေႃး( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ပေႃး ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ပေႃးဢမ်ႇ ပေႃး( *ptr == '-' )
                {
                    ပေႃး( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ပေႃးဢမ်ႇ ပေႃး( first )
                        bha_nga;
                }
                ပေႃးဢမ်ႇ ပေႃး( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ပေႃး( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ပေႃးဢမ်ႇ ပေႃး( fs->eof() )
                    bha_nga;
                ပေႃးဢမ်ႇ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ပေႃး( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ပေႃး( !ptr || !ptr[0] )
                bha_nga;
            ပေႃး( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ပေႃး( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ပေႃး( !ptr )
                    bha_nga;
            }

            ပေႃး( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ပွၵ်ႈ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ပွၵ်ႈ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ပွၵ်ႈ makePtr<YAMLParser>(fs);
}

}
