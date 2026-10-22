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

        ᮚᮦᮔ᮪ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ᮚᮦᮔ᮪( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ᮚᮦᮔ᮪ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            ᮚᮦᮔ᮪( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            ᮜᮤᮚ
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        ᮚᮦᮔ᮪( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ᮝᮜᮤ fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        puur_naa_nka struct_flags = current_struct.flags;

        ᮚᮦᮔ᮪( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᮚᮦᮔ᮪( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( FileNode::isEmptyCollection(struct_flags) )
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

        ᮚᮦᮔ᮪( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ᮚᮦᮔ᮪( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ᮚᮦᮔ᮪( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ᮚᮦᮔ᮪( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                ᮚᮦᮔ᮪( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    ᮚᮦᮔ᮪( cv_isprint(c) )
                        *data++ = c;
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\n' )
                        *data++ = 'n';
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\r' )
                        *data++ = 'r';
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\t' )
                        *data++ = 't';
                    ᮜᮤᮚ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                ᮜᮤᮚ
                    *data++ = c;
            }
            ᮚᮦᮔ᮪( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            ᮚᮦᮔ᮪( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ᮚᮦᮔ᮪ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka i, keylen = 0;
        puur_naa_nka datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        puur_naa_nka struct_flags = current_struct.flags;

        ᮚᮦᮔ᮪( key && key[0] == '\0' )
            key = 0;

        ᮚᮦᮔ᮪( FileNode::isCollection(struct_flags) )
        {
            ᮚᮦᮔ᮪( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        ᮜᮤᮚ
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        ᮚᮦᮔ᮪( key )
        {
            keylen = (puur_naa_nka)strlen(key);
            ᮚᮦᮔ᮪( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            ᮚᮦᮔ᮪( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        ᮚᮦᮔ᮪( data )
            datalen = (puur_naa_nka)strlen(data);

        ᮚᮦᮔ᮪( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            ᮚᮦᮔ᮪( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + keylen + datalen;
            ᮚᮦᮔ᮪( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ᮜᮤᮚ
                *ptr++ = ' ';
        }
        ᮜᮤᮚ
        {
            ptr = fs->flush();
            ᮚᮦᮔ᮪( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                ᮚᮦᮔ᮪( data )
                    *ptr++ = ' ';
            }
        }

        ᮚᮦᮔ᮪( key )
        {
            ᮚᮦᮔ᮪( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            for( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ᮚᮦᮔ᮪( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            ᮚᮦᮔ᮪( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        ᮚᮦᮔ᮪( data )
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
        ᮚᮦᮔ᮪( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = (puur_naa_nka)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        ᮚᮦᮔ᮪( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ᮜᮤᮚ
            *ptr++ = ' ';

        ᮞᮨᮓᮨᮜ( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            ᮚᮦᮔ᮪( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ᮜᮤᮚ
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
        ᮚᮦᮔ᮪ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        for(;;)
        {
            ᮞᮨᮓᮨᮜ( *ptr == ' ' )
                ptr++;
            ᮚᮦᮔ᮪( *ptr == '#' )
            {
                ᮚᮦᮔ᮪( ptr - fs->bufferStart() > max_comment_indent )
                    ᮝᮜᮤ ptr;
                *ptr = '\0';
            }
            ᮜᮤᮚ ᮚᮦᮔ᮪( cv_isprint(*ptr) )
            {
                ᮚᮦᮔ᮪( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            ᮜᮤᮚ ᮚᮦᮔ᮪( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                ᮚᮦᮔ᮪( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                ᮜᮤᮚ
                {
                    puur_naa_nka l = (puur_naa_nka)strlen(ptr);
                    ᮚᮦᮔ᮪( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            ᮜᮤᮚ
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ᮝᮜᮤ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end)
    {
        ᮚᮦᮔ᮪ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        ᮚᮦᮔ᮪ (!ptr || !*ptr)
            ᮝᮜᮤ false; // end of file

        ᮚᮦᮔ᮪ (ptr - fs->bufferStart() != indent)
            ᮝᮜᮤ false; // end of base64 data

        /* find end */
        ᮞᮨᮓᮨᮜ(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        ᮚᮦᮔ᮪ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ᮝᮜᮤ true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        ᮚᮦᮔ᮪ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        ᮚᮦᮔ᮪( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        ᮞᮨᮓᮨᮜ( cv_isprint(c) && c != ':' );

        ᮚᮦᮔ᮪( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        ᮚᮦᮔ᮪( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        ᮞᮨᮓᮨᮜ( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ᮝᮜᮤ ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka min_indent, bool is_parent_flow, puur_naa_nka depth = 0 )
    {
        ᮚᮦᮔ᮪ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᮚᮦᮔ᮪( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        puur_naa_nka value_type = FileNode::NONE;
        puur_naa_nka len;
        bool is_binary_string = false;
        bool is_user = false;

        ᮚᮦᮔ᮪( c == '!' ) // handle explicit type specification
        {
            ᮚᮦᮔ᮪( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            ᮚᮦᮔ᮪ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                ᮞᮨᮓᮨᮜ( cv_isprint(d) && d != ' ' && d != '>' );

                ᮚᮦᮔ᮪ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    ᮚᮦᮔ᮪ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            ᮞᮨᮓᮨᮜ( cv_isprint(d) && d != ' ' );
            len = (puur_naa_nka)(endptr - ptr);
            ᮚᮦᮔ᮪( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            ᮚᮦᮔ᮪( len == 3 && !is_user )
            {
                ᮚᮦᮔ᮪( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                ᮜᮤᮚ ᮚᮦᮔ᮪( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                ᮜᮤᮚ ᮚᮦᮔ᮪( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                ᮜᮤᮚ ᮚᮦᮔ᮪( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            ᮜᮤᮚ ᮚᮦᮔ᮪( len == 5 && !is_user )
            {
                ᮚᮦᮔ᮪( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            ᮜᮤᮚ ᮚᮦᮔ᮪ (len == 6 && is_user)
            {
                ᮚᮦᮔ᮪( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        ᮚᮦᮔ᮪ (d == '|')
                            bha_nga;
                    } ᮞᮨᮓᮨᮜ (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            ᮚᮦᮔ᮪ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            ᮚᮦᮔ᮪( !is_user )
            {
                ᮚᮦᮔ᮪ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                ᮚᮦᮔ᮪( value_type == FileNode::INT )
                    goto force_int;
                ᮚᮦᮔ᮪( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        ᮚᮦᮔ᮪ (is_binary_string)
        {
            puur_naa_nka indent = static_cast<puur_naa_nka>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            ᮞᮨᮓᮨᮜ( cv_isdigit(*endptr) )
                endptr++;
            ᮚᮦᮔ᮪( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            ᮜᮤᮚ
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            ᮚᮦᮔ᮪( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\'' || c == '\"' ) // an explicit string
        {
            ᮚᮦᮔ᮪( c == '\'' )
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᮚᮦᮔ᮪( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\'' )
                    {
                        c = *++ptr;
                        ᮚᮦᮔ᮪( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    ᮜᮤᮚ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            ᮜᮤᮚ
                for( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    ᮚᮦᮔ᮪( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    ᮜᮤᮚ ᮚᮦᮔ᮪( c == '\\' )
                    {
                        d = *++ptr;
                        ᮚᮦᮔ᮪( d == '\'' )
                            buf[len++] = d;
                        ᮜᮤᮚ ᮚᮦᮔ᮪( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        ᮜᮤᮚ ᮚᮦᮔ᮪( d == 'n' )
                            buf[len++] = '\n';
                        ᮜᮤᮚ ᮚᮦᮔ᮪( d == 'r' )
                            buf[len++] = '\r';
                        ᮜᮤᮚ ᮚᮦᮔ᮪( d == 't' )
                            buf[len++] = '\t';
                        ᮜᮤᮚ ᮚᮦᮔ᮪( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            puur_naa_nka val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (puur_naa_nka)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            ᮚᮦᮔ᮪( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            ᮜᮤᮚ
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    ᮜᮤᮚ
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            ᮚᮦᮔ᮪( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪( c == '[' || c == '{' ) // collection as a flow
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
                ᮚᮦᮔ᮪ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                ᮚᮦᮔ᮪( *ptr == '}' || *ptr == ']' )
                {
                    ᮚᮦᮔ᮪( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                ᮚᮦᮔ᮪( nelems != 0 )
                {
                    ᮚᮦᮔ᮪( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    ᮚᮦᮔ᮪ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ᮚᮦᮔ᮪( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                ᮜᮤᮚ
                {
                    ᮚᮦᮔ᮪( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        ᮜᮤᮚ
        {
            puur_naa_nka indent, struct_type;

            ᮚᮦᮔ᮪( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                ᮚᮦᮔ᮪( !is_parent_flow )
                {
                    ᮚᮦᮔ᮪( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    ᮚᮦᮔ᮪( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                ᮞᮨᮓᮨᮜ( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                ᮚᮦᮔ᮪( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                ᮚᮦᮔ᮪( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    ᮞᮨᮓᮨᮜ( str_end > ptr && c == ' ' );
                    str_end++;
                    ᮚᮦᮔ᮪ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        ᮚᮦᮔ᮪ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᮝᮜᮤ ptr;
                        }
                        ᮜᮤᮚ ᮚᮦᮔ᮪ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ᮝᮜᮤ ptr;
                        }
                        ᮜᮤᮚ ᮚᮦᮔ᮪ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ᮝᮜᮤ ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (puur_naa_nka)(str_end - ptr));
                    ptr = endptr;
                    ᮝᮜᮤ ptr;
                }
                struct_type = FileNode::MAP;
            }
            ᮜᮤᮚ
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (puur_naa_nka)(ptr - fs->bufferStart());

            for(;;)
            {
                FileNode elem;

                ᮚᮦᮔ᮪( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                ᮜᮤᮚ
                {
                    c = *ptr++;
                    ᮚᮦᮔ᮪( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᮚᮦᮔ᮪( ptr - fs->bufferStart() != indent )
                {
                    ᮚᮦᮔ᮪( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    ᮜᮤᮚ
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                ᮚᮦᮔ᮪( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ᮝᮜᮤ ptr;
    }

    bool parse( var_na* ptr )
    {
        ᮚᮦᮔ᮪ (!ptr)
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
                ᮚᮦᮔ᮪( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                ᮚᮦᮔ᮪( *ptr == '%' )
                {
                    ᮚᮦᮔ᮪( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        ᮚᮦᮔ᮪ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                ᮜᮤᮚ ᮚᮦᮔ᮪( *ptr == '-' )
                {
                    ᮚᮦᮔ᮪( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    ᮜᮤᮚ ᮚᮦᮔ᮪( first )
                        bha_nga;
                }
                ᮜᮤᮚ ᮚᮦᮔ᮪( cv_isalnum(*ptr) || *ptr=='_')
                {
                    ᮚᮦᮔ᮪( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                ᮜᮤᮚ ᮚᮦᮔ᮪( fs->eof() )
                    bha_nga;
                ᮜᮤᮚ
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            ᮚᮦᮔ᮪( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            ᮚᮦᮔ᮪( !ptr || !ptr[0] )
                bha_nga;
            ᮚᮦᮔ᮪( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                ᮚᮦᮔ᮪( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                ᮚᮦᮔ᮪( !ptr )
                    bha_nga;
            }

            ᮚᮦᮔ᮪( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ᮝᮜᮤ ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ᮝᮜᮤ makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ᮝᮜᮤ makePtr<YAMLParser>(fs);
}

}
