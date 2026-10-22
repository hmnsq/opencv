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
                                 ཧྲིལ struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        གལ་སྲིད ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        གལ་སྲིད( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        གལ་སྲིད (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        དེ་མིན གལ་སྲིད( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            གལ་སྲིད( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            དེ་མིན
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        དེ་མིན གལ་སྲིད( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        གལ་སྲིད( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ལོག fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        ཧྲིལ struct_flags = current_struct.flags;

        གལ་སྲིད( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            གལ་སྲིད( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        དེ་མིན གལ་སྲིད( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, ཧྲིལ value)
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
        ཧྲིལ i, len;

        གལ་སྲིད( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ཧྲིལ)strlen(str);
        གལ་སྲིད( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        གལ་སྲིད( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ཧྲིལ need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            དོན་ལུ( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                གལ་སྲིད( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                གལ་སྲིད( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    གལ་སྲིད( cv_isprint(c) )
                        *data++ = c;
                    དེ་མིན གལ་སྲིད( c == '\n' )
                        *data++ = 'n';
                    དེ་མིན གལ་སྲིད( c == '\r' )
                        *data++ = 'r';
                    དེ་མིན གལ་སྲིད( c == '\t' )
                        *data++ = 't';
                    དེ་མིན
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                དེ་མིན
                    *data++ = c;
            }
            གལ་སྲིད( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            གལ་སྲིད( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        གལ་སྲིད ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        དེ་མིན གལ་སྲིད ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ཧྲིལ i, keylen = 0;
        ཧྲིལ datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ཧྲིལ struct_flags = current_struct.flags;

        གལ་སྲིད( key && key[0] == '\0' )
            key = 0;

        གལ་སྲིད( FileNode::isCollection(struct_flags) )
        {
            གལ་སྲིད( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        དེ་མིན
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        གལ་སྲིད( key )
        {
            keylen = (ཧྲིལ)strlen(key);
            གལ་སྲིད( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            གལ་སྲིད( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        གལ་སྲིད( data )
            datalen = (ཧྲིལ)strlen(data);

        གལ་སྲིད( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            གལ་སྲིད( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ཧྲིལ new_offset = (ཧྲིལ)(ptr - fs->bufferStart()) + keylen + datalen;
            གལ་སྲིད( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            དེ་མིན
                *ptr++ = ' ';
        }
        དེ་མིན
        {
            ptr = fs->flush();
            གལ་སྲིད( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                གལ་སྲིད( data )
                    *ptr++ = ' ';
            }
        }

        གལ་སྲིད( key )
        {
            གལ་སྲིད( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            དོན་ལུ( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                གལ་སྲིད( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            གལ་སྲིད( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        གལ་སྲིད( data )
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
        གལ་སྲིད( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ཧྲིལ len = (ཧྲིལ)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        གལ་སྲིད( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        དེ་མིན
            *ptr++ = ' ';

        སྐབས( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            གལ་སྲིད( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ཧྲིལ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            དེ་མིན
            {
                len = (ཧྲིལ)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ཧྲིལ min_indent, ཧྲིལ max_comment_indent )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        དོན་ལུ(;;)
        {
            སྐབས( *ptr == ' ' )
                ptr++;
            གལ་སྲིད( *ptr == '#' )
            {
                གལ་སྲིད( ptr - fs->bufferStart() > max_comment_indent )
                    ལོག ptr;
                *ptr = '\0';
            }
            དེ་མིན གལ་སྲིད( cv_isprint(*ptr) )
            {
                གལ་སྲིད( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            དེ་མིན གལ་སྲིད( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                གལ་སྲིད( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                དེ་མིན
                {
                    ཧྲིལ l = (ཧྲིལ)strlen(ptr);
                    གལ་སྲིད( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            དེ་མིན
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ལོག ptr;
    }

    bool getBase64Row(var_na* ptr, ཧྲིལ indent, var_na* &beg, var_na* &end)
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        གལ་སྲིད (!ptr || !*ptr)
            ལོག false; // end of file

        གལ་སྲིད (ptr - fs->bufferStart() != indent)
            ལོག false; // end of base64 data

        /* find end */
        སྐབས(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        གལ་སྲིད (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ལོག true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        གལ་སྲིད( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        སྐབས( cv_isprint(c) && c != ':' );

        གལ་སྲིད( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        གལ་སྲིད( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        སྐབས( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ལོག ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ཧྲིལ min_indent, bool is_parent_flow, ཧྲིལ depth = 0 )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        གལ་སྲིད( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        ཧྲིལ value_type = FileNode::NONE;
        ཧྲིལ len;
        bool is_binary_string = false;
        bool is_user = false;

        གལ་སྲིད( c == '!' ) // handle explicit type specification
        {
            གལ་སྲིད( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            གལ་སྲིད ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                སྐབས( cv_isprint(d) && d != ' ' && d != '>' );

                གལ་སྲིད ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    གལ་སྲིད ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            སྐབས( cv_isprint(d) && d != ' ' );
            len = (ཧྲིལ)(endptr - ptr);
            གལ་སྲིད( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            གལ་སྲིད( len == 3 && !is_user )
            {
                གལ་སྲིད( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                དེ་མིན གལ་སྲིད( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                དེ་མིན གལ་སྲིད( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                དེ་མིན གལ་སྲིད( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            དེ་མིན གལ་སྲིད( len == 5 && !is_user )
            {
                གལ་སྲིད( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            དེ་མིན གལ་སྲིད (len == 6 && is_user)
            {
                གལ་སྲིད( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        གལ་སྲིད (d == '|')
                            bha_nga;
                    } སྐབས (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            གལ་སྲིད (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            གལ་སྲིད( !is_user )
            {
                གལ་སྲིད (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                གལ་སྲིད( value_type == FileNode::INT )
                    goto force_int;
                གལ་སྲིད( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        གལ་སྲིད (is_binary_string)
        {
            ཧྲིལ indent = static_cast<ཧྲིལ>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        དེ་མིན གལ་སྲིད( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            སྐབས( cv_isdigit(*endptr) )
                endptr++;
            གལ་སྲིད( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            དེ་མིན
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            གལ་སྲིད( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        དེ་མིན གལ་སྲིད( c == '\'' || c == '\"' ) // an explicit string
        {
            གལ་སྲིད( c == '\'' )
                དོན་ལུ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    གལ་སྲིད( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    དེ་མིན གལ་སྲིད( c == '\'' )
                    {
                        c = *++ptr;
                        གལ་སྲིད( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    དེ་མིན
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            དེ་མིན
                དོན་ལུ( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    གལ་སྲིད( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    དེ་མིན གལ་སྲིད( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    དེ་མིན གལ་སྲིད( c == '\\' )
                    {
                        d = *++ptr;
                        གལ་སྲིད( d == '\'' )
                            buf[len++] = d;
                        དེ་མིན གལ་སྲིད( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        དེ་མིན གལ་སྲིད( d == 'n' )
                            buf[len++] = '\n';
                        དེ་མིན གལ་སྲིད( d == 'r' )
                            buf[len++] = '\r';
                        དེ་མིན གལ་སྲིད( d == 't' )
                            buf[len++] = '\t';
                        དེ་མིན གལ་སྲིད( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ཧྲིལ val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ཧྲིལ)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            གལ་སྲིད( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            དེ་མིན
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    དེ་མིན
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            གལ་སྲིད( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        དེ་མིན གལ་སྲིད( c == '[' || c == '{' ) // collection as a flow
        {
            ཧྲིལ new_min_indent = min_indent + !is_parent_flow;
            ཧྲིལ struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            ཧྲིལ nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            དོན་ལུ( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                གལ་སྲིད (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                གལ་སྲིད( *ptr == '}' || *ptr == ']' )
                {
                    གལ་སྲིད( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                གལ་སྲིད( nelems != 0 )
                {
                    གལ་སྲིད( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    གལ་སྲིད (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                གལ་སྲིད( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                དེ་མིན
                {
                    གལ་སྲིད( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        དེ་མིན
        {
            ཧྲིལ indent, struct_type;

            གལ་སྲིད( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                གལ་སྲིད( !is_parent_flow )
                {
                    གལ་སྲིད( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    གལ་སྲིད( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                སྐབས( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                གལ་སྲིད( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                གལ་སྲིད( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    སྐབས( str_end > ptr && c == ' ' );
                    str_end++;
                    གལ་སྲིད ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        གལ་སྲིད (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ལོག ptr;
                        }
                        དེ་མིན གལ་སྲིད (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ལོག ptr;
                        }
                        དེ་མིན གལ་སྲིད (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ལོག ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (ཧྲིལ)(str_end - ptr));
                    ptr = endptr;
                    ལོག ptr;
                }
                struct_type = FileNode::MAP;
            }
            དེ་མིན
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (ཧྲིལ)(ptr - fs->bufferStart());

            དོན་ལུ(;;)
            {
                FileNode elem;

                གལ་སྲིད( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                དེ་མིན
                {
                    c = *ptr++;
                    གལ་སྲིད( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                གལ་སྲིད( ptr - fs->bufferStart() != indent )
                {
                    གལ་སྲིད( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    དེ་མིན
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                གལ་སྲིད( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ལོག ptr;
    }

    bool parse( var_na* ptr )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        དོན་ལུ(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            དོན་ལུ(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                གལ་སྲིད( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                གལ་སྲིད( *ptr == '%' )
                {
                    གལ་སྲིད( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        གལ་སྲིད ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                དེ་མིན གལ་སྲིད( *ptr == '-' )
                {
                    གལ་སྲིད( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    དེ་མིན གལ་སྲིད( first )
                        bha_nga;
                }
                དེ་མིན གལ་སྲིད( cv_isalnum(*ptr) || *ptr=='_')
                {
                    གལ་སྲིད( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                དེ་མིན གལ་སྲིད( fs->eof() )
                    bha_nga;
                དེ་མིན
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            གལ་སྲིད( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            གལ་སྲིད( !ptr || !ptr[0] )
                bha_nga;
            གལ་སྲིད( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                གལ་སྲིད( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                གལ་སྲིད( !ptr )
                    bha_nga;
            }

            གལ་སྲིད( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ལོག ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ལོག makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ལོག makePtr<YAMLParser>(fs);
}

}
