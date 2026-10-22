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

        ᯀᯮᯝ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ᯀᯮᯝ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ᯀᯮᯝ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ᯀᯮᯝ( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ᯀᯮᯝᯂᯪ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ᯀᯮᯝ( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ᯀᯮᯞ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        puur_naa_nka struct_flags = current_struct.flags;

        ᯀᯮᯝ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᯀᯮᯝ( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( FileNode::isEmptyCollection(struct_flags) )
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

        ᯀᯮᯝ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ᯀᯮᯝ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ᯀᯮᯝ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ᯀᯮᯝ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ᯀᯮᯝ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ᯀᯮᯝ( cv_isprint(c) )
                        *data++ = c;
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\n' )
                        *data++ = 'n';
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\r' )
                        *data++ = 'r';
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\t' )
                        *data++ = 't';
                    ᯀᯮᯝᯂᯪ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ᯀᯮᯝᯂᯪ
                    *data++ = c;
            }
            ᯀᯮᯝ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ᯀᯮᯝ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ᯀᯮᯝ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka i, keylen = 0;
        puur_naa_nka datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        puur_naa_nka struct_flags = current_struct.flags;

        ᯀᯮᯝ( key && key[0] == '\0' )
            key = 0;

        ᯀᯮᯝ( FileNode::isCollection(struct_flags) )
        {
            ᯀᯮᯝ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ᯀᯮᯝᯂᯪ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ᯀᯮᯝ( key )
        {
            keylen = (puur_naa_nka)strlen(key);
            ᯀᯮᯝ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ᯀᯮᯝ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ᯀᯮᯝ( data )
            datalen = (puur_naa_nka)strlen(data);

        ᯀᯮᯝ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᯀᯮᯝ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + keylen + datalen;
            ᯀᯮᯝ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ᯀᯮᯝᯂᯪ
                *ptr++ = ' ';
        }
        ᯀᯮᯝᯂᯪ
        {
            ptr = fs->flush();
            ᯀᯮᯝ( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ᯀᯮᯝ( data )
                    *ptr++ = ' ';
            }
        }

        ᯀᯮᯝ( key )
        {
            ᯀᯮᯝ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            for( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ᯀᯮᯝ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ᯀᯮᯝ( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ᯀᯮᯝ( data )
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
        ᯀᯮᯝ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = (puur_naa_nka)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ᯀᯮᯝ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ᯀᯮᯝᯂᯪ
            *ptr++ = ' ';

        ᯀᯬᯝ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ᯀᯮᯝ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ᯀᯮᯝᯂᯪ
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
        ᯀᯮᯝ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        for(;;)
        {
            ᯀᯬᯝ( *ptr == ' ' )
                ptr++;
            ᯀᯮᯝ( *ptr == '#' )
            {
                ᯀᯮᯝ( ptr - fs->bufferStart() > max_comment_indent )
                    ᯀᯮᯞ ptr;
                *ptr = '\0';
            }
            ᯀᯮᯝᯂᯪ ᯀᯮᯝ( cv_isprint(*ptr) )
            {
                ᯀᯮᯝ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ᯀᯮᯝᯂᯪ ᯀᯮᯝ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ᯀᯮᯝ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ᯀᯮᯝᯂᯪ
                {
                    puur_naa_nka l = (puur_naa_nka)strlen(ptr);
                    ᯀᯮᯝ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ᯀᯮᯝᯂᯪ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ᯀᯮᯞ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end)
    {
        ᯀᯮᯝ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ᯀᯮᯝ (!ptr || !*ptr)
            ᯀᯮᯞ false; // end of file

        ᯀᯮᯝ (ptr - fs->bufferStart() != indent)
            ᯀᯮᯞ false; // end of base64 data

        /* find end */
        ᯀᯬᯝ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ᯀᯮᯝ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ᯀᯮᯞ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ᯀᯮᯝ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ᯀᯮᯝ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ᯀᯬᯝ( cv_isprint(c) && c != ':' );

        ᯀᯮᯝ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ᯀᯮᯝ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ᯀᯬᯝ( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ᯀᯮᯞ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka min_indent, bool is_parent_flow, puur_naa_nka depth = 0 )
    {
        ᯀᯮᯝ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᯀᯮᯝ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        puur_naa_nka value_type = FileNode::NONE;
        puur_naa_nka len;
        bool is_binary_string = false;
        bool is_user = false;

        ᯀᯮᯝ( c == '!' ) // handle explicit type specification
        {
            ᯀᯮᯝ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ᯀᯮᯝ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ᯀᯬᯝ( cv_isprint(d) && d != ' ' && d != '>' );

                ᯀᯮᯝ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ᯀᯮᯝ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ᯀᯬᯝ( cv_isprint(d) && d != ' ' );
            len = (puur_naa_nka)(endptr - ptr);
            ᯀᯮᯝ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ᯀᯮᯝ( len == 3 && !is_user )
            {
                ᯀᯮᯝ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ᯀᯮᯝᯂᯪ ᯀᯮᯝ( len == 5 && !is_user )
            {
                ᯀᯮᯝ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ᯀᯮᯝᯂᯪ ᯀᯮᯝ (len == 6 && is_user)
            {
                ᯀᯮᯝ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ᯀᯮᯝ (d == '|')
                            bha_nga;
                    } ᯀᯬᯝ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ᯀᯮᯝ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ᯀᯮᯝ( !is_user )
            {
                ᯀᯮᯝ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ᯀᯮᯝ( value_type == FileNode::INT )
                    goto force_int;
                ᯀᯮᯝ( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ᯀᯮᯝ (is_binary_string)
        {
            puur_naa_nka indent = static_cast<puur_naa_nka>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ᯀᯬᯝ( cv_isdigit(*endptr) )
                endptr++;
            ᯀᯮᯝ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ᯀᯮᯝᯂᯪ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ᯀᯮᯝ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\'' || c == '\"' ) // an explicit string
        {
            ᯀᯮᯝ( c == '\'' )
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᯀᯮᯝ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\'' )
                    {
                        c = *++ptr;
                        ᯀᯮᯝ( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ᯀᯮᯝᯂᯪ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ᯀᯮᯝᯂᯪ
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᯀᯮᯝ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '\\' )
                    {
                        d = *++ptr;
                        ᯀᯮᯝ( d == '\'' )
                            buf[len++] = d;
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( d == 'n' )
                            buf[len++] = '\n';
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( d == 'r' )
                            buf[len++] = '\r';
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( d == 't' )
                            buf[len++] = '\t';
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            puur_naa_nka val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (puur_naa_nka)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ᯀᯮᯝ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ᯀᯮᯝᯂᯪ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ᯀᯮᯝᯂᯪ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ᯀᯮᯝ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ( c == '[' || c == '{' ) // collection as a flow
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
                ᯀᯮᯝ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ᯀᯮᯝ( *ptr == '}' || *ptr == ']' )
                {
                    ᯀᯮᯝ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ᯀᯮᯝ( nelems != 0 )
                {
                    ᯀᯮᯝ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ᯀᯮᯝ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ᯀᯮᯝ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ᯀᯮᯝᯂᯪ
                {
                    ᯀᯮᯝ( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ᯀᯮᯝᯂᯪ
        {
            puur_naa_nka indent, struct_type;

            ᯀᯮᯝ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ᯀᯮᯝ( !is_parent_flow )
                {
                    ᯀᯮᯝ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ᯀᯮᯝ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ᯀᯬᯝ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ᯀᯮᯝ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ᯀᯮᯝ( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ᯀᯬᯝ( str_end > ptr && c == ' ' );
                    str_end++;
                    ᯀᯮᯝ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ᯀᯮᯝ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᯀᯮᯞ ptr;
                        }
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᯀᯮᯞ ptr;
                        }
                        ᯀᯮᯝᯂᯪ ᯀᯮᯝ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ᯀᯮᯞ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (puur_naa_nka)(str_end - ptr));
                    ptr = endptr;
                    ᯀᯮᯞ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ᯀᯮᯝᯂᯪ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (puur_naa_nka)(ptr - fs->bufferStart());

            for(;;)
            {
                FileNode elem;

                ᯀᯮᯝ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ᯀᯮᯝᯂᯪ
                {
                    c = *ptr++;
                    ᯀᯮᯝ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᯀᯮᯝ( ptr - fs->bufferStart() != indent )
                {
                    ᯀᯮᯝ( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ᯀᯮᯝᯂᯪ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ᯀᯮᯝ( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ᯀᯮᯞ ptr;
    }

    bool parse( var_na* ptr )
    {
        ᯀᯮᯝ (!ptr)
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
                ᯀᯮᯝ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ᯀᯮᯝ( *ptr == '%' )
                {
                    ᯀᯮᯝ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ᯀᯮᯝ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( *ptr == '-' )
                {
                    ᯀᯮᯝ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( first )
                        bha_nga;
                }
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ᯀᯮᯝ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ᯀᯮᯝᯂᯪ ᯀᯮᯝ( fs->eof() )
                    bha_nga;
                ᯀᯮᯝᯂᯪ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ᯀᯮᯝ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ᯀᯮᯝ( !ptr || !ptr[0] )
                bha_nga;
            ᯀᯮᯝ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ᯀᯮᯝ( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᯀᯮᯝ( !ptr )
                    bha_nga;
            }

            ᯀᯮᯝ( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ᯀᯮᯞ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ᯀᯮᯞ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ᯀᯮᯞ makePtr<YAMLParser>(fs);
}

}
