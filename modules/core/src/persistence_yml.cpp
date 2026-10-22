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

        ᰀᰧ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ᰀᰧ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ᰀᰧ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ᰀᰧᰀ ᰀᰧ( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ᰀᰧ( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ᰀᰧᰀ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ᰀᰧᰀ ᰀᰧ( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ᰀᰧ( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ᰀᰧᰌ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        puur_naa_nka struct_flags = current_struct.flags;

        ᰀᰧ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᰀᰧ( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ᰀᰧᰀ ᰀᰧ( FileNode::isEmptyCollection(struct_flags) )
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

        ᰀᰧ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ᰀᰧ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ᰀᰧ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ᰀᰧ( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ᰀᰧ( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ᰀᰧ( cv_isprint(c) )
                        *data++ = c;
                    ᰀᰧᰀ ᰀᰧ( c == '\n' )
                        *data++ = 'n';
                    ᰀᰧᰀ ᰀᰧ( c == '\r' )
                        *data++ = 'r';
                    ᰀᰧᰀ ᰀᰧ( c == '\t' )
                        *data++ = 't';
                    ᰀᰧᰀ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ᰀᰧᰀ
                    *data++ = c;
            }
            ᰀᰧ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ᰀᰧ( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ᰀᰧ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ᰀᰧᰀ ᰀᰧ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka i, keylen = 0;
        puur_naa_nka datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        puur_naa_nka struct_flags = current_struct.flags;

        ᰀᰧ( key && key[0] == '\0' )
            key = 0;

        ᰀᰧ( FileNode::isCollection(struct_flags) )
        {
            ᰀᰧ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ᰀᰧᰀ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ᰀᰧ( key )
        {
            keylen = (puur_naa_nka)strlen(key);
            ᰀᰧ( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ᰀᰧ( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ᰀᰧ( data )
            datalen = (puur_naa_nka)strlen(data);

        ᰀᰧ( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᰀᰧ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + keylen + datalen;
            ᰀᰧ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ᰀᰧᰀ
                *ptr++ = ' ';
        }
        ᰀᰧᰀ
        {
            ptr = fs->flush();
            ᰀᰧ( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ᰀᰧ( data )
                    *ptr++ = ' ';
            }
        }

        ᰀᰧ( key )
        {
            ᰀᰧ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            for( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ᰀᰧ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ᰀᰧ( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ᰀᰧ( data )
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
        ᰀᰧ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = (puur_naa_nka)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ᰀᰧ( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ᰀᰧᰀ
            *ptr++ = ' ';

        ᰀᰬ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ᰀᰧ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ᰀᰧᰀ
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
        ᰀᰧ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        for(;;)
        {
            ᰀᰬ( *ptr == ' ' )
                ptr++;
            ᰀᰧ( *ptr == '#' )
            {
                ᰀᰧ( ptr - fs->bufferStart() > max_comment_indent )
                    ᰀᰧᰌ ptr;
                *ptr = '\0';
            }
            ᰀᰧᰀ ᰀᰧ( cv_isprint(*ptr) )
            {
                ᰀᰧ( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ᰀᰧᰀ ᰀᰧ( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ᰀᰧ( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ᰀᰧᰀ
                {
                    puur_naa_nka l = (puur_naa_nka)strlen(ptr);
                    ᰀᰧ( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ᰀᰧᰀ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ᰀᰧᰌ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end)
    {
        ᰀᰧ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ᰀᰧ (!ptr || !*ptr)
            ᰀᰧᰌ false; // end of file

        ᰀᰧ (ptr - fs->bufferStart() != indent)
            ᰀᰧᰌ false; // end of base64 data

        /* find end */
        ᰀᰬ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ᰀᰧ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ᰀᰧᰌ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ᰀᰧ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ᰀᰧ( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ᰀᰬ( cv_isprint(c) && c != ':' );

        ᰀᰧ( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ᰀᰧ( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ᰀᰬ( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ᰀᰧᰌ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka min_indent, bool is_parent_flow, puur_naa_nka depth = 0 )
    {
        ᰀᰧ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᰀᰧ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        puur_naa_nka value_type = FileNode::NONE;
        puur_naa_nka len;
        bool is_binary_string = false;
        bool is_user = false;

        ᰀᰧ( c == '!' ) // handle explicit type specification
        {
            ᰀᰧ( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ᰀᰧ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ᰀᰬ( cv_isprint(d) && d != ' ' && d != '>' );

                ᰀᰧ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ᰀᰧ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ᰀᰬ( cv_isprint(d) && d != ' ' );
            len = (puur_naa_nka)(endptr - ptr);
            ᰀᰧ( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ᰀᰧ( len == 3 && !is_user )
            {
                ᰀᰧ( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ᰀᰧᰀ ᰀᰧ( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ᰀᰧᰀ ᰀᰧ( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ᰀᰧᰀ ᰀᰧ( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ᰀᰧᰀ ᰀᰧ( len == 5 && !is_user )
            {
                ᰀᰧ( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ᰀᰧᰀ ᰀᰧ (len == 6 && is_user)
            {
                ᰀᰧ( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ᰀᰧ (d == '|')
                            bha_nga;
                    } ᰀᰬ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ᰀᰧ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ᰀᰧ( !is_user )
            {
                ᰀᰧ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ᰀᰧ( value_type == FileNode::INT )
                    goto force_int;
                ᰀᰧ( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ᰀᰧ (is_binary_string)
        {
            puur_naa_nka indent = static_cast<puur_naa_nka>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ᰀᰧᰀ ᰀᰧ( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ᰀᰬ( cv_isdigit(*endptr) )
                endptr++;
            ᰀᰧ( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ᰀᰧᰀ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ᰀᰧ( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ᰀᰧᰀ ᰀᰧ( c == '\'' || c == '\"' ) // an explicit string
        {
            ᰀᰧ( c == '\'' )
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᰀᰧ( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ᰀᰧᰀ ᰀᰧ( c == '\'' )
                    {
                        c = *++ptr;
                        ᰀᰧ( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ᰀᰧᰀ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ᰀᰧᰀ
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᰀᰧ( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ᰀᰧᰀ ᰀᰧ( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ᰀᰧᰀ ᰀᰧ( c == '\\' )
                    {
                        d = *++ptr;
                        ᰀᰧ( d == '\'' )
                            buf[len++] = d;
                        ᰀᰧᰀ ᰀᰧ( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ᰀᰧᰀ ᰀᰧ( d == 'n' )
                            buf[len++] = '\n';
                        ᰀᰧᰀ ᰀᰧ( d == 'r' )
                            buf[len++] = '\r';
                        ᰀᰧᰀ ᰀᰧ( d == 't' )
                            buf[len++] = '\t';
                        ᰀᰧᰀ ᰀᰧ( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            puur_naa_nka val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (puur_naa_nka)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ᰀᰧ( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ᰀᰧᰀ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ᰀᰧᰀ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ᰀᰧ( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ᰀᰧᰀ ᰀᰧ( c == '[' || c == '{' ) // collection as a flow
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
                ᰀᰧ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ᰀᰧ( *ptr == '}' || *ptr == ']' )
                {
                    ᰀᰧ( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ᰀᰧ( nelems != 0 )
                {
                    ᰀᰧ( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ᰀᰧ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ᰀᰧ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ᰀᰧᰀ
                {
                    ᰀᰧ( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ᰀᰧᰀ
        {
            puur_naa_nka indent, struct_type;

            ᰀᰧ( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ᰀᰧ( !is_parent_flow )
                {
                    ᰀᰧ( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ᰀᰧ( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ᰀᰬ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ᰀᰧ( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ᰀᰧ( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ᰀᰬ( str_end > ptr && c == ' ' );
                    str_end++;
                    ᰀᰧ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ᰀᰧ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᰀᰧᰌ ptr;
                        }
                        ᰀᰧᰀ ᰀᰧ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᰀᰧᰌ ptr;
                        }
                        ᰀᰧᰀ ᰀᰧ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ᰀᰧᰌ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (puur_naa_nka)(str_end - ptr));
                    ptr = endptr;
                    ᰀᰧᰌ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ᰀᰧᰀ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (puur_naa_nka)(ptr - fs->bufferStart());

            for(;;)
            {
                FileNode elem;

                ᰀᰧ( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ᰀᰧᰀ
                {
                    c = *ptr++;
                    ᰀᰧ( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᰀᰧ( ptr - fs->bufferStart() != indent )
                {
                    ᰀᰧ( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ᰀᰧᰀ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ᰀᰧ( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ᰀᰧᰌ ptr;
    }

    bool parse( var_na* ptr )
    {
        ᰀᰧ (!ptr)
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
                ᰀᰧ( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ᰀᰧ( *ptr == '%' )
                {
                    ᰀᰧ( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ᰀᰧ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ᰀᰧᰀ ᰀᰧ( *ptr == '-' )
                {
                    ᰀᰧ( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ᰀᰧᰀ ᰀᰧ( first )
                        bha_nga;
                }
                ᰀᰧᰀ ᰀᰧ( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ᰀᰧ( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ᰀᰧᰀ ᰀᰧ( fs->eof() )
                    bha_nga;
                ᰀᰧᰀ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ᰀᰧ( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ᰀᰧ( !ptr || !ptr[0] )
                bha_nga;
            ᰀᰧ( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ᰀᰧ( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᰀᰧ( !ptr )
                    bha_nga;
            }

            ᰀᰧ( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ᰀᰧᰌ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ᰀᰧᰌ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ᰀᰧᰌ makePtr<YAMLParser>(fs);
}

}
