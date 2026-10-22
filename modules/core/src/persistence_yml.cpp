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
                                 ကိန်းပြည့် struct_flags, const var_na* type_name=0)
    {
        var_na buf[CV_FS_MAX_LEN + 1024];
        const var_na* data = 0;

        အကယ်၍ ( type_name && *type_name == '\0' )
            type_name = 0;

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        အကယ်၍( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        အကယ်၍ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            /* reset struct flag. in order not to print ']' */
            struct_flags = FileNode::SEQ;
            snprintf(buf, parimaa_na(buf), "!!binary |");
            data = buf;
        }
        မဟုတ်ပါက အကယ်၍( FileNode::isFlow(struct_flags))
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            struct_flags |= FileNode::FLOW;

            အကယ်၍( type_name )
                snprintf( buf, parimaa_na(buf), "!!%s %c", type_name, c );
            မဟုတ်ပါက
            {
                buf[0] = c;
                buf[1] = '\0';
            }
            data = buf;
        }
        မဟုတ်ပါက အကယ်၍( type_name )
        {
            snprintf( buf, parimaa_na(buf), "!!%s", type_name );
            data = buf;
        }

        writeScalar( key, data );

        FStructData fsd;
        fsd.indent = parent.indent;
        fsd.flags = struct_flags;

        အကယ်၍( !FileNode::isFlow(parent.flags) )
            fsd.indent += CV_YML_INDENT + FileNode::isFlow(struct_flags);

        ပြန်ပေး fsd;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        var_na* ptr;

        ကိန်းပြည့် struct_flags = current_struct.flags;

        အကယ်၍( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            အကယ်၍( ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
        မဟုတ်ပါက အကယ်၍( FileNode::isEmptyCollection(struct_flags) )
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

    void write(const var_na* key, ကိန်းပြည့် value)
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
        ကိန်းပြည့် i, len;

        အကယ်၍( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ကိန်းပြည့်)strlen(str);
        အကယ်၍( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        အကယ်၍( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ကိန်းပြည့် need_quote = quote || len == 0 || str[0] == ' ';
            data = buf;
            *data++ = '\"';
            အတွက်( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                အကယ်၍( !need_quote && !cv_isalnum(c) && c != '_' && c != ' ' && c != '-' &&
                   c != '(' && c != ')' && c != '/' && c != '+' && c != ';' )
                    need_quote = 1;

                အကယ်၍( !cv_isalnum(c) && (!cv_isprint(c) || c == '\\' || c == '\'' || c == '\"') )
                {
                    *data++ = '\\';
                    အကယ်၍( cv_isprint(c) )
                        *data++ = c;
                    မဟုတ်ပါက အကယ်၍( c == '\n' )
                        *data++ = 'n';
                    မဟုတ်ပါက အကယ်၍( c == '\r' )
                        *data++ = 'r';
                    မဟုတ်ပါက အကယ်၍( c == '\t' )
                        *data++ = 't';
                    မဟုတ်ပါက
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "x%02x", c );
                        data += 3;
                    }
                }
                မဟုတ်ပါက
                    *data++ = c;
            }
            အကယ်၍( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = 1;

            အကယ်၍( need_quote )
                *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        အကယ်၍ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        မဟုတ်ပါက အကယ်၍ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ကိန်းပြည့် i, keylen = 0;
        ကိန်းပြည့် datalen = 0;
        var_na* ptr;

        FStructData& current_struct = fs->getCurrentStruct();

        ကိန်းပြည့် struct_flags = current_struct.flags;

        အကယ်၍( key && key[0] == '\0' )
            key = 0;

        အကယ်၍( FileNode::isCollection(struct_flags) )
        {
            အကယ်၍( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        }
        မဟုတ်ပါက
        {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        အကယ်၍( key )
        {
            keylen = (ကိန်းပြည့်)strlen(key);
            အကယ်၍( keylen == 0 )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );

            အကယ်၍( keylen > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        အကယ်၍( data )
            datalen = (ကိန်းပြည့်)strlen(data);

        အကယ်၍( FileNode::isFlow(struct_flags) )
        {
            ptr = fs->bufferPtr();
            အကယ်၍( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            ကိန်းပြည့် new_offset = (ကိန်းပြည့်)(ptr - fs->bufferStart()) + keylen + datalen;
            အကယ်၍( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            မဟုတ်ပါက
                *ptr++ = ' ';
        }
        မဟုတ်ပါက
        {
            ptr = fs->flush();
            အကယ်၍( !FileNode::isMap(struct_flags) )
            {
                *ptr++ = '-';
                အကယ်၍( data )
                    *ptr++ = ' ';
            }
        }

        အကယ်၍( key )
        {
            အကယ်၍( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, keylen );

            အတွက်( i = 0; i < keylen; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                အကယ်၍( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += keylen;
            *ptr++ = ':';
            အကယ်၍( !FileNode::isFlow(struct_flags) && data )
                *ptr++ = ' ';
        }

        အကယ်၍( data )
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
        အကယ်၍( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ကိန်းပြည့် len = (ကိန်းပြည့်)strlen(comment);
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;
        var_na* ptr = fs->bufferPtr();

        အကယ်၍( !eol_comment || multiline ||
            fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        မဟုတ်ပါက
            *ptr++ = ' ';

        နေစဉ်( comment )
        {
            *ptr++ = '#';
            *ptr++ = ' ';
            အကယ်၍( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ကိန်းပြည့်)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            မဟုတ်ပါက
            {
                len = (ကိန်းပြည့်)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ကိန်းပြည့် min_indent, ကိန်းပြည့် max_comment_indent )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        အတွက်(;;)
        {
            နေစဉ်( *ptr == ' ' )
                ptr++;
            အကယ်၍( *ptr == '#' )
            {
                အကယ်၍( ptr - fs->bufferStart() > max_comment_indent )
                    ပြန်ပေး ptr;
                *ptr = '\0';
            }
            မဟုတ်ပါက အကယ်၍( cv_isprint(*ptr) )
            {
                အကယ်၍( ptr - fs->bufferStart() < min_indent )
                    CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                bha_nga;
            }
            မဟုတ်ပါက အကယ်၍( *ptr == '\0' || *ptr == '\n' || *ptr == '\r' )
            {
                ptr = fs->gets();
                အကယ်၍( !ptr )
                {
                    // emulate end of stream
                    ptr = fs->bufferStart();
                    ptr[0] = ptr[1] = ptr[2] = '.';
                    ptr[3] = '\0';
                    fs->setEof();
                    bha_nga;
                }
                မဟုတ်ပါက
                {
                    ကိန်းပြည့် l = (ကိန်းပြည့်)strlen(ptr);
                    အကယ်၍( ptr[l-1] != '\n' && ptr[l-1] != '\r' && !fs->eof() )
                        CV_PARSE_ERROR_CPP( "Too long string or a last string w/o newline" );
                }
            }
            မဟုတ်ပါက
                CV_PARSE_ERROR_CPP( *ptr == '\t' ? "Tabs are prohibited in YAML!" : "Invalid character" );
        }

        ပြန်ပေး ptr;
    }

    bool getBase64Row(var_na* ptr, ကိန်းပြည့် indent, var_na* &beg, var_na* &end)
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        beg = end = ptr = skipSpaces(ptr, 0, INT_MAX);
        အကယ်၍ (!ptr || !*ptr)
            ပြန်ပေး false; // end of file

        အကယ်၍ (ptr - fs->bufferStart() != indent)
            ပြန်ပေး false; // end of base64 data

        /* find end */
        နေစဉ်(cv_isprint(*ptr)) /* no check for base64 string */
            ++ptr;
        အကယ်၍ (*ptr == '\0')
            CV_PARSE_ERROR_CPP("Unexpected end of line");

        end = ptr;
        ပြန်ပေး true;
    }


    var_na* parseKey( var_na* ptr, FileNode& map_node, FileNode& value_placeholder )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        var_na c;
        var_na *endptr = ptr - 1, *saveptr;

        အကယ်၍( *ptr == '-' )
            CV_PARSE_ERROR_CPP( "Key may not start with \'-\'" );

        do c = *++endptr;
        နေစဉ်( cv_isprint(c) && c != ':' );

        အကယ်၍( c != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\'" );

        saveptr = endptr + 1;
        အကယ်၍( endptr == ptr )
            CV_PARSE_ERROR_CPP( "An empty key" );

        do c = *--endptr;
        နေစဉ်( c == ' ' );

        ++endptr;

        value_placeholder = fs->addNode(map_node, std::string(ptr, endptr - ptr), FileNode::NONE);
        ptr = saveptr;

        ပြန်ပေး ptr;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ကိန်းပြည့် min_indent, bool is_parent_flow, ကိန်းပြည့် depth = 0 )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        အကယ်၍( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        var_na* endptr = 0;
        var_na c = ptr[0], d = ptr[1];
        ကိန်းပြည့် value_type = FileNode::NONE;
        ကိန်းပြည့် len;
        bool is_binary_string = false;
        bool is_user = false;

        အကယ်၍( c == '!' ) // handle explicit type specification
        {
            အကယ်၍( d == '!' || d == '^' )
            {
                ptr++;
                is_user = true;
                //value_type |= FileNode::USER;
            }
            အကယ်၍ ( d == '<') //support of full type heading from YAML 1.2
            {
                const var_na* yamlTypeHeading = "<tag:yaml.org,2002:";
                const size_t headingLength = strlen(yamlTypeHeading);

                var_na* typeEndPtr = ++ptr;

                do d = *++typeEndPtr;
                နေစဉ်( cv_isprint(d) && d != ' ' && d != '>' );

                အကယ်၍ ( d == '>' && (size_t)(typeEndPtr - ptr) > headingLength )
                {
                    အကယ်၍ ( memcmp(ptr, yamlTypeHeading, headingLength) == 0 )
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
            နေစဉ်( cv_isprint(d) && d != ' ' );
            len = (ကိန်းပြည့်)(endptr - ptr);
            အကယ်၍( len == 0 )
                CV_PARSE_ERROR_CPP( "Empty type name" );
            d = *endptr;
            *endptr = '\0';

            အကယ်၍( len == 3 && !is_user )
            {
                အကယ်၍( memcmp( ptr, "str", 3 ) == 0 )
                    value_type = FileNode::STRING;
                မဟုတ်ပါက အကယ်၍( memcmp( ptr, "int", 3 ) == 0 )
                    value_type = FileNode::INT;
                မဟုတ်ပါက အကယ်၍( memcmp( ptr, "seq", 3 ) == 0 )
                    value_type = FileNode::SEQ;
                မဟုတ်ပါက အကယ်၍( memcmp( ptr, "map", 3 ) == 0 )
                    value_type = FileNode::MAP;
            }
            မဟုတ်ပါက အကယ်၍( len == 5 && !is_user )
            {
                အကယ်၍( memcmp( ptr, "float", 5 ) == 0 )
                    value_type = FileNode::REAL;
            }
            မဟုတ်ပါက အကယ်၍ (len == 6 && is_user)
            {
                အကယ်၍( memcmp( ptr, "binary", 6 ) == 0 ) {
                    value_type = FileNode::SEQ;
                    is_binary_string = true;

                    /* for ignore '|' */

                    /**** operation with endptr ****/
                    *endptr = d;

                    do {
                        d = *++endptr;
                        အကယ်၍ (d == '|')
                            bha_nga;
                    } နေစဉ် (d == ' ');

                    d = *++endptr;
                    *endptr = '\0';
                }
            }

            *endptr = d;
            ptr = skipSpaces( endptr, min_indent, INT_MAX );
            အကယ်၍ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            c = *ptr;

            အကယ်၍( !is_user )
            {
                အကယ်၍ (value_type == FileNode::STRING && c != '\'' && c != '\"')
                    goto force_string;
                အကယ်၍( value_type == FileNode::INT )
                    goto force_int;
                အကယ်၍( value_type == FileNode::REAL )
                    goto force_real;
            }
        }

        အကယ်၍ (is_binary_string)
        {
            ကိန်းပြည့် indent = static_cast<ကိန်းပြည့်>(ptr - fs->bufferStart());
            ptr = fs->parseBase64(ptr, indent, node);
        }
        မဟုတ်ပါက အကယ်၍( cv_isdigit(c) ||
                ((c == '-' || c == '+') && (cv_isdigit(d) || d == '.')) ||
                (c == '.' && cv_isalnum(d))) // a number
        {
            endptr = ptr + (c == '-' || c == '+');
            နေစဉ်( cv_isdigit(*endptr) )
                endptr++;
            အကယ်၍( *endptr == '.' || *endptr == 'e' )
            {
            force_real:
                dviguu_na fval = fs->strtod( ptr, &endptr );
                node.setValue(FileNode::REAL, &fval);
            }
            မဟုတ်ပါက
            {
            force_int:
                int64_t ival = strtoll( ptr, &endptr, 0 );
                node.setValue(FileNode::INT, &ival);
            }

            အကယ်၍( !endptr || endptr == ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        }
        မဟုတ်ပါက အကယ်၍( c == '\'' || c == '\"' ) // an explicit string
        {
            အကယ်၍( c == '\'' )
                အတွက်( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    အကယ်၍( cv_isalnum(c) || (c != '\'' && cv_isprint(c)))
                        buf[len++] = c;
                    မဟုတ်ပါက အကယ်၍( c == '\'' )
                    {
                        c = *++ptr;
                        အကယ်၍( c != '\'' )
                            bha_nga;
                        buf[len++] = c;
                    }
                    မဟုတ်ပါက
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }
            မဟုတ်ပါက
                အတွက်( len = 0; len < CV_FS_MAX_LEN; )
                {
                    c = *++ptr;
                    အကယ်၍( cv_isalnum(c) || (c != '\\' && c != '\"' && cv_isprint(c)))
                        buf[len++] = c;
                    မဟုတ်ပါက အကယ်၍( c == '\"' )
                    {
                        ++ptr;
                        bha_nga;
                    }
                    မဟုတ်ပါက အကယ်၍( c == '\\' )
                    {
                        d = *++ptr;
                        အကယ်၍( d == '\'' )
                            buf[len++] = d;
                        မဟုတ်ပါက အကယ်၍( d == '\"' || d == '\\' || d == '\'' )
                            buf[len++] = d;
                        မဟုတ်ပါက အကယ်၍( d == 'n' )
                            buf[len++] = '\n';
                        မဟုတ်ပါက အကယ်၍( d == 'r' )
                            buf[len++] = '\r';
                        မဟုတ်ပါက အကယ်၍( d == 't' )
                            buf[len++] = '\t';
                        မဟုတ်ပါက အကယ်၍( d == 'x' || (cv_isdigit(d) && d < '8') )
                        {
                            ကိန်းပြည့် val, is_hex = d == 'x';
                            c = ptr[3];
                            ptr[3] = '\0';
                            val = (ကိန်းပြည့်)strtol( ptr + is_hex, &endptr, is_hex ? 8 : 16 );
                            ptr[3] = c;
                            အကယ်၍( endptr == ptr + is_hex )
                                buf[len++] = 'x';
                            မဟုတ်ပါက
                            {
                                buf[len++] = (var_na)val;
                                ptr = endptr;
                            }
                        }
                    }
                    မဟုတ်ပါက
                        CV_PARSE_ERROR_CPP( "Invalid character" );
                }

            အကယ်၍( len >= CV_FS_MAX_LEN )
                CV_PARSE_ERROR_CPP( "Too long string literal" );

            node.setValue(FileNode::STRING, buf, len);
        }
        မဟုတ်ပါက အကယ်၍( c == '[' || c == '{' ) // collection as a flow
        {
            ကိန်းပြည့် new_min_indent = min_indent + !is_parent_flow;
            ကိန်းပြည့် struct_type = c == '{' ? FileNode::MAP : FileNode::SEQ;
            ကိန်းပြည့် nelems = 0;

            fs->convertToCollection(struct_type, node);
            d = c == '[' ? ']' : '}';

            အတွက်( ++ptr ;; nelems++ )
            {
                FileNode elem;

                ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                အကယ်၍ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                အကယ်၍( *ptr == '}' || *ptr == ']' )
                {
                    အကယ်၍( *ptr != d )
                        CV_PARSE_ERROR_CPP( "The wrong closing bracket" );
                    ptr++;
                    bha_nga;
                }

                အကယ်၍( nelems != 0 )
                {
                    အကယ်၍( *ptr != ',' )
                        CV_PARSE_ERROR_CPP( "Missing , between the elements" );
                    ptr = skipSpaces( ptr + 1, new_min_indent, INT_MAX );
                    အကယ်၍ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                အကယ်၍( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                    ptr = skipSpaces( ptr, new_min_indent, INT_MAX );
                }
                မဟုတ်ပါက
                {
                    အကယ်၍( *ptr == ']' )
                        bha_nga;
                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = parseValue( ptr, elem, new_min_indent, true, depth + 1 );
            }
            fs->finalizeCollection(node);
        }
        မဟုတ်ပါက
        {
            ကိန်းပြည့် indent, struct_type;

            အကယ်၍( is_parent_flow || c != '-' )
            {
                // implicit (one-line) string or nested block-style collection
                အကယ်၍( !is_parent_flow )
                {
                    အကယ်၍( c == '?' )
                        CV_PARSE_ERROR_CPP( "Complex keys are not supported" );
                    အကယ်၍( c == '|' || c == '>' )
                        CV_PARSE_ERROR_CPP( "Multi-line text literals are not supported" );
                }

            force_string:
                endptr = ptr - 1;

                do c = *++endptr;
                နေစဉ်( cv_isprint(c) &&
                      (!is_parent_flow || (c != ',' && c != '}' && c != ']')) &&
                      (is_parent_flow || c != ':' || value_type == FileNode::STRING));

                အကယ်၍( endptr == ptr )
                    CV_PARSE_ERROR_CPP( "Invalid character" );

                အကယ်၍( is_parent_flow || c != ':' )
                {
                    var_na* str_end = endptr;
                    do c = *--str_end;
                    နေစဉ်( str_end > ptr && c == ' ' );
                    str_end++;
                    အကယ်၍ ((fs->getFormat() & FileStorage::FORMAT_MASK) != FileStorage::FORMAT_YAML_1_0)
                    {
                        size_t llen = str_end - ptr;
                        အကယ်၍ (llen == 4 && memcmp(ptr, "true", 4) == 0) {
                            int64_t ival = 1;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ပြန်ပေး ptr;
                        }
                        မဟုတ်ပါက အကယ်၍ (llen == 5 && memcmp(ptr, "false", 5) == 0) {
                            int64_t ival = 0;
                            node.setValue(FileNode::INT, &ival);
                            ptr = endptr;
                            ပြန်ပေး ptr;
                        }
                        မဟုတ်ပါက အကယ်၍ (llen == 4 && memcmp(ptr, "null", 4) == 0) {
                            ptr = endptr;
                            ပြန်ပေး ptr;
                        }
                    }

                    node.setValue(FileNode::STRING, ptr, (ကိန်းပြည့်)(str_end - ptr));
                    ptr = endptr;
                    ပြန်ပေး ptr;
                }
                struct_type = FileNode::MAP;
            }
            မဟုတ်ပါက
                struct_type = FileNode::SEQ;

            fs->convertToCollection( struct_type, node );
            indent = (ကိန်းပြည့်)(ptr - fs->bufferStart());

            အတွက်(;;)
            {
                FileNode elem;

                အကယ်၍( struct_type == FileNode::MAP )
                {
                    ptr = parseKey( ptr, node, elem );
                }
                မဟုတ်ပါက
                {
                    c = *ptr++;
                    အကယ်၍( c != '-' )
                        CV_PARSE_ERROR_CPP( "Block sequence elements must be preceded with \'-\'" );

                    elem = fs->addNode(node, std::string(), FileNode::NONE);
                }
                ptr = skipSpaces( ptr, indent + 1, INT_MAX );
                ptr = parseValue( ptr, elem, indent + 1, false, depth + 1 );
                ptr = skipSpaces( ptr, 0, INT_MAX );
                အကယ်၍( ptr - fs->bufferStart() != indent )
                {
                    အကယ်၍( ptr - fs->bufferStart() < indent )
                        bha_nga;
                    မဟုတ်ပါက
                        CV_PARSE_ERROR_CPP( "Incorrect indentation" );
                }
                အကယ်၍( memcmp( ptr, "...", 3 ) == 0 )
                    bha_nga;
            }
            fs->finalizeCollection(node);
        }

        ပြန်ပေး ptr;
    }

    bool parse( var_na* ptr )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        bool first = true;
        bool ok = true;
        FileNode root_collection(fs->getFS(), 0, 0);

        အတွက်(;;)
        {
            // 0. skip leading comments and directives  and ...
            // 1. reach the first item
            အတွက်(;;)
            {
                ptr = skipSpaces( ptr, 0, INT_MAX );
                အကယ်၍( !ptr || *ptr == '\0' )
                {
                    ok = !first;
                    bha_nga;
                }

                အကယ်၍( *ptr == '%' )
                {
                    အကယ်၍( memcmp( ptr, "%YAML", 5 ) == 0 &&
                        memcmp( ptr, "%YAML:1.", 8 ) != 0 &&
                        memcmp( ptr, "%YAML 1.", 8 ) != 0)
                    {
                        အကယ်၍ ((fs->getFormat() & FileStorage::FORMAT_MASK) == FileStorage::FORMAT_YAML_1_0)
                            CV_PARSE_ERROR_CPP( "Unsupported YAML version (it must be 1.x)" );
                    }
                    *ptr = '\0';
                }
                မဟုတ်ပါက အကယ်၍( *ptr == '-' )
                {
                    အကယ်၍( memcmp(ptr, "---", 3) == 0 )
                    {
                        ptr += 3;
                        bha_nga;
                    }
                    မဟုတ်ပါက အကယ်၍( first )
                        bha_nga;
                }
                မဟုတ်ပါက အကယ်၍( cv_isalnum(*ptr) || *ptr=='_')
                {
                    အကယ်၍( !first )
                        CV_PARSE_ERROR_CPP( "The YAML streams must start with '---', except the first one" );
                    bha_nga;
                }
                မဟုတ်ပါက အကယ်၍( fs->eof() )
                    bha_nga;
                မဟုတ်ပါက
                    CV_PARSE_ERROR_CPP( "Invalid or unsupported syntax" );
            }

            အကယ်၍( ptr )
                ptr = skipSpaces( ptr, 0, INT_MAX );
            အကယ်၍( !ptr || !ptr[0] )
                bha_nga;
            အကယ်၍( memcmp( ptr, "...", 3 ) != 0 )
            {
                // 2. parse the collection
                FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::NONE);

                ptr = parseValue( ptr, root_node, 0, false );
                အကယ်၍( !root_node.isMap() && !root_node.isSeq() )
                    CV_PARSE_ERROR_CPP( "Only collections as YAML streams are supported by this parser" );

                // 3. parse until the end of file or next collection
                ptr = skipSpaces( ptr, 0, INT_MAX );
                အကယ်၍( !ptr )
                    bha_nga;
            }

            အကယ်၍( fs->eof() )
                bha_nga;
            ptr += 3;
            first = false;
        }

        ပြန်ပေး ok;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs)
{
    ပြန်ပေး makePtr<YAMLEmitter>(fs);
}

Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs)
{
    ပြန်ပေး makePtr<YAMLParser>(fs);
}

}
