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

        ᤀᤡ᤺ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ᤀᤡ᤺( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ᤀᤡ᤺ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ᤀᤡ᤺( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ᤀᤡ᤺ᤆᤡ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ᤀᤡ᤺( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ᤀᤡᤴ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        puur_naa_nka struct_flags = current_struct.flags;

        ᤀᤡ᤺( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᤀᤡ᤺( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( FileNode::isEmptyCollection(struct_flags) )
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

        ᤀᤡ᤺( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ᤀᤡ᤺( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ᤀᤡ᤺( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ᤀᤡ᤺( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ᤀᤡ᤺( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ᤀᤡ᤺( cv_isprint(c) )
                        *data++ = c;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\n' )
                        *data++ = 'n';
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\r' )
                        *data++ = 'r';
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\t' )
                        *data++ = 't';
                    ᤀᤡ᤺ᤆᤡ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ᤀᤡ᤺ᤆᤡ
                    *data++ = c;
            }
            ᤀᤡ᤺( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ᤀᤡ᤺( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ᤀᤡ᤺ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka i, keylen = 0;
        puur_naa_nka datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        puur_naa_nka struct_flags = current_struct.flags;

        ᤀᤡ᤺( key && key[0] == '\0' )
            key = 0;

        ᤀᤡ᤺( FileNode::isCollection(struct_flags) )
        {
            ᤀᤡ᤺( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ᤀᤡ᤺ᤆᤡ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ᤀᤡ᤺( key )
        {
            keylen = (puur_naa_nka)strlen(key);
            ᤀᤡ᤺( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ᤀᤡ᤺( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ᤀᤡ᤺( data )
            datalen = (puur_naa_nka)strlen(data);

        ᤀᤡ᤺( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᤀᤡ᤺( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + keylen + datalen;
            ᤀᤡ᤺( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ᤀᤡ᤺ᤆᤡ
                *ptr++ = ' ';
        }
        ᤀᤡ᤺ᤆᤡ
        {
            ptr = fs->flush();
            ᤀᤡ᤺( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ᤀᤡ᤺( data )
                    *ptr++ = ' ';
            }
        }

        ᤀᤡ᤺( key )
        {
            ᤀᤡ᤺( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            for( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ᤀᤡ᤺( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ᤀᤡ᤺( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ᤀᤡ᤺( data )
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
        ᤀᤡ᤺( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = (puur_naa_nka)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ᤀᤡ᤺( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ᤀᤡ᤺ᤆᤡ
            *ptr++ = ' ';

        ᤀᤠ᤺( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ᤀᤡ᤺( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ᤀᤡ᤺ᤆᤡ
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
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        for(;;)
        {
            ᤀᤠ᤺( *ptr == ' ' )
                ptr++;
            ᤀᤡ᤺( *ptr == '#' )
            {
                ᤀᤡ᤺( ptr - fs->bufferStart() > max_comment_indent )
                    ᤀᤡᤴ ptr;
                *ptr = '\0';
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( cv_isprint(*ptr) )
            {
                ᤀᤡ᤺( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ᤀᤡ᤺( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    puur_naa_nka l = (puur_naa_nka)strlen(ptr);
                    ᤀᤡ᤺( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ᤀᤡ᤺ᤆᤡ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ᤀᤡᤴ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end)
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ᤀᤡ᤺ (!ptr || !*ptr)
            ᤀᤡᤴ false; // end of file

        ᤀᤡ᤺ (ptr - fs->bufferStart() != indent)
            ᤀᤡᤴ false; // end of base64 data

        /* find end */
        ᤀᤠ᤺(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ᤀᤡ᤺ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ᤀᤡᤴ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ᤀᤡ᤺( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ᤀᤠ᤺( cv_isprint(c) && c != ':' );

        ᤀᤡ᤺( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ᤀᤡ᤺( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ᤀᤠ᤺( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ᤀᤡᤴ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka min_indent, bool is_parent_flow, puur_naa_nka depth = 0 )
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᤀᤡ᤺( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        puur_naa_nka value_type = FileNode::NONE;
        puur_naa_nka len;
        bool is_binary_string = false;
        bool is_user = false;

        ᤀᤡ᤺( c == '!' ) // handle explicit type specification
        {
            ᤀᤡ᤺( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ᤀᤡ᤺ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ᤀᤠ᤺( cv_isprint(d) && d != ' ' && d != '>' );

                ᤀᤡ᤺ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ᤀᤡ᤺ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ᤀᤠ᤺( cv_isprint(d) && d != ' ' );
            len = (puur_naa_nka)(endptr - ptr);
            ᤀᤡ᤺( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ᤀᤡ᤺( len == 3 && !is_user )
            {
                ᤀᤡ᤺( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( len == 5 && !is_user )
            {
                ᤀᤡ᤺( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (len == 6 && is_user)
            {
                ᤀᤡ᤺( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ᤀᤡ᤺ (d == '|')
                            bha_nga;
                    } ᤀᤠ᤺ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ᤀᤡ᤺ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ᤀᤡ᤺( !is_user )
            {
                ᤀᤡ᤺ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ᤀᤡ᤺( value_type == FileNode::INT )
                    goto force_int;
                ᤀᤡ᤺( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ᤀᤡ᤺ (is_binary_string)
        {
            puur_naa_nka indent = static_cast<puur_naa_nka>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ᤀᤠ᤺( cv_isdigit(*endptr) )
                endptr++;
            ᤀᤡ᤺( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ᤀᤡ᤺ᤆᤡ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ᤀᤡ᤺( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\'' || c == '\"' ) // an explicit string
        {
            ᤀᤡ᤺( c == '\'' )
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᤀᤡ᤺( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\'' )
                    {
                        c = *++ptr;
                        ᤀᤡ᤺( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ᤀᤡ᤺ᤆᤡ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ᤀᤡ᤺ᤆᤡ
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᤀᤡ᤺( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\\' )
                    {
                        d = *++ptr;
                        ᤀᤡ᤺( d == '\'' )
                            buf[len++] = d;
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( d == 'n' )
                            buf[len++] = '\n';
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( d == 'r' )
                            buf[len++] = '\r';
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( d == 't' )
                            buf[len++] = '\t';
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            puur_naa_nka val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (puur_naa_nka)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ᤀᤡ᤺( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ᤀᤡ᤺ᤆᤡ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ᤀᤡ᤺ᤆᤡ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ᤀᤡ᤺( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '[' || c == '{' ) // collection as a flow
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
                ᤀᤡ᤺ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ᤀᤡ᤺( *ptr == '}' || *ptr == ']' )
                {
                    ᤀᤡ᤺( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ᤀᤡ᤺( nelems != 0 )
                {
                    ᤀᤡ᤺( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ᤀᤡ᤺ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ᤀᤡ᤺( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    ᤀᤡ᤺( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ᤀᤡ᤺ᤆᤡ
        {
            puur_naa_nka indent, struct_type;

            ᤀᤡ᤺( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ᤀᤡ᤺( !is_parent_flow )
                {
                    ᤀᤡ᤺( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ᤀᤡ᤺( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ᤀᤠ᤺( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ᤀᤡ᤺( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ᤀᤡ᤺( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ᤀᤠ᤺( str_end > ptr && c == ' ' );
                    str_end++;
                    ᤀᤡ᤺ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ᤀᤡ᤺ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᤀᤡᤴ ptr;
                        }
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᤀᤡᤴ ptr;
                        }
                        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ᤀᤡᤴ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (puur_naa_nka)(str_end - ptr));
                    ptr = endptr;
                    ᤀᤡᤴ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ᤀᤡ᤺ᤆᤡ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (puur_naa_nka)(ptr - fs->bufferStart());

            for(;;)
            {
                FileNode elem;

                ᤀᤡ᤺( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    c = *ptr++;
                    ᤀᤡ᤺( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᤀᤡ᤺( ptr - fs->bufferStart() != indent )
                {
                    ᤀᤡ᤺( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ᤀᤡ᤺ᤆᤡ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ᤀᤡ᤺( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ᤀᤡᤴ ptr;
    }

    bool parse( var_na* ptr )
    {
        ᤀᤡ᤺ (!ptr)
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
                ᤀᤡ᤺( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ᤀᤡ᤺( *ptr == '%' )
                {
                    ᤀᤡ᤺( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ᤀᤡ᤺ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( *ptr == '-' )
                {
                    ᤀᤡ᤺( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( first )
                        bha_nga;
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ᤀᤡ᤺( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( fs->eof() )
                    bha_nga;
                ᤀᤡ᤺ᤆᤡ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ᤀᤡ᤺( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ᤀᤡ᤺( !ptr || !ptr[0] )
                bha_nga;
            ᤀᤡ᤺( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ᤀᤡ᤺( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᤀᤡ᤺( !ptr )
                    bha_nga;
            }

            ᤀᤡ᤺( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ᤀᤡᤴ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ᤀᤡᤴ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ᤀᤡᤴ makePtr<YAMLParser>(fs);
}

}
