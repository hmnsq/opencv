// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "persistence.hpp"

ga_nanaa
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_TAG = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_TAG = 1,
    CV_XML_CLOSING_TAG = 2,
    CV_XML_EMPTY_TAG = 3,
    CV_XML_HEADER_TAG = 4,
    CV_XML_DIRECTIVE_TAG = 5
};

namespace cv
{

class XMLEmitter : public FileStorageEmitter
{
public:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~XMLEmitter() {}

    void writeTag( const var_na* key, ຈຳນວນເຕັມ tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        ຈຳນວນເຕັມ i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ຈຳນວນເຕັມ struct_flags = current_struct.flags;

        ຖ້າ( key && key[0] == '\0' )
            key = 0;

        ຖ້າ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ຖ້າ( FileNode::isCollection(struct_flags) )
            {
                ຖ້າ( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            ຖ້າ( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ຖ້າ( !key )
            key = "_";
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ຈຳນວນເຕັມ)strlen( key );
        *ptr++ = '<';
        ຖ້າ( tag_type == CV_XML_CLOSING_TAG )
        {
            ຖ້າ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ຖ້າ( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        ສຳລັບ( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            ຖ້າ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ຈຳນວນເຕັມ nattr = (ຈຳນວນເຕັມ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        ສຳລັບ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ຈຳນວນເຕັມ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ຖ້າ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ຖ້າ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 ຈຳນວນເຕັມ struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        ຖ້າ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ສົ່ງຄືນ current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, ຈຳນວນເຕັມ value)
    {
        var_na buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( key, ptr);
    }

    void write(const var_na* key, int64_t value)
    {
        var_na buf[128], *ptr = fs::itoa( value, buf, 10, true );
        writeScalar( key, ptr);
    }

    void write( const var_na* key, dviguu_na value )
    {
        var_na buf[128];
        writeScalar( key, fs::doubleToString( buf, parimaa_na(buf), value, false ) );
    }

    void write(const var_na* key, const var_na* str, bool quote)
    {
        var_na buf[CV_FS_MAX_LEN*6+16];
        var_na* data = (var_na*)str;
        ຈຳນວນເຕັມ i, len;

        ຖ້າ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ຈຳນວນເຕັມ)strlen(str);
        ຖ້າ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ຖ້າ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            ສຳລັບ( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ຖ້າ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ຖ້າ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    ບໍ່ດັ່ງນັ້ນ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                ບໍ່ດັ່ງນັ້ນ
                    *data++ = c;
            }
            ຖ້າ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            ຖ້າ( need_quote )
                *data++ = '\"';
            len = (ຈຳນວນເຕັມ)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ຈຳນວນເຕັມ)!need_quote;
        }

        writeScalar( key, data );
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

        ຈຳນວນເຕັມ len = (ຈຳນວນເຕັມ)strlen(data);
        ຖ້າ( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ຈຳນວນເຕັມ struct_flags = current_struct.flags;

        ຖ້າ( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            var_na* ptr = fs->bufferPtr();
            ຈຳນວນເຕັມ new_offset = (ຈຳນວນເຕັມ)(ptr - fs->bufferStart()) + len;

            ຖ້າ( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            ຖ້າ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ຈຳນວນເຕັມ len;
        ຈຳນວນເຕັມ multiline;
        const var_na* eol;
        var_na* ptr;

        ຖ້າ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ຖ້າ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ຈຳນວນເຕັມ)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ຖ້າ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ຖ້າ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ຈຳນວນເຕັມ)strlen(ptr);
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ຖ້າ( multiline )
        {
            ໃນຂະນະທີ່( comment )
            {
                ຖ້າ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ຈຳນວນເຕັມ)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    len = (ຈຳນວນເຕັມ)strlen(comment);
                    ptr = fs->resizeWriteBuffer( ptr, len );
                    memcpy( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->setBufferPtr(ptr + 3);
            fs->flush();
        }
    }

    void startNextStream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

rak_sita:
    FileStorage_API* fs;
};

class XMLParser : public FileStorageParser
{
public:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~XMLParser() {}

    var_na* skipSpaces( var_na* ptr, ຈຳນວນເຕັມ mode )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ຈຳນວນເຕັມ level = 0;

        ສຳລັບ(;;)
        {
            var_na c;
            ptr--;

            ຖ້າ( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                ໃນຂະນະທີ່( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ຖ້າ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ຖ້າ( level < 0 )
                        ສົ່ງຄືນ ptr;
                } ໃນຂະນະທີ່( cv_isprint_or_tab(c) );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                do c = *++ptr;
                ໃນຂະນະທີ່( c == ' ' || c == '\t' );

                ຖ້າ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ຖ້າ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ( cv_isprint(c) )
                    bha_nga;
            }

            ຖ້າ( !cv_isprint(*ptr) )
            {
                ຖ້າ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ຖ້າ( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ສົ່ງຄືນ ptr;
    }

    bool getBase64Row(var_na* ptr, ຈຳນວນເຕັມ /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ຖ້າ( !ptr || !*ptr )
            ສົ່ງຄືນ false;

        // closing XML tag
        ຖ້າ ( *beg == '<' )
            ສົ່ງຄືນ false;

        // find end of the row
        ໃນຂະນະທີ່( cv_isprint(*ptr) )
            ++ptr;
        ຖ້າ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ສົ່ງຄືນ true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ຈຳນວນເຕັມ depth = 0 )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ຖ້າ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        ຈຳນວນເຕັມ value_type = node.type();
        std::string key, key2, type_name;

        ສຳລັບ(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ຖ້າ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ຖ້າ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ຖ້າ( c =='<' || c == '\0' )
            {
                ຈຳນວນເຕັມ tag_type = 0;
                ຈຳນວນເຕັມ elem_type = FileNode::NONE;

                ຖ້າ( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                ຖ້າ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ຖ້າ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                ຖ້າ( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    ຖ້າ( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                ຖ້າ (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                ບໍ່ດັ່ງນັ້ນ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ຖ້າ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ຖ້າ( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                ຖ້າ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                ຖ້າ( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                ຖ້າ( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ໃນຂະນະທີ່( cv_isdigit(*endptr) )
                        endptr++;
                    ຖ້າ( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    ບໍ່ດັ່ງນັ້ນ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    ຖ້າ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    // string
                    ຈຳນວນເຕັມ i = 0, len, is_quoted = 0;
                    ຖ້າ( c == '\"' )
                        is_quoted = 1;
                    ບໍ່ດັ່ງນັ້ນ
                        --ptr;
                    strbuf[0] = '\0';

                    ສຳລັບ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ຖ້າ( !cv_isalnum(c) )
                        {
                            ຖ້າ( c == '\"' )
                            {
                                ຖ້າ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            ບໍ່ດັ່ງນັ້ນ ຖ້າ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ຖ້າ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '&' )
                            {
                                ຖ້າ( *++ptr == '#' )
                                {
                                    ຈຳນວນເຕັມ val, base = 10;
                                    ptr++;
                                    ຖ້າ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ຈຳນວນເຕັມ)strtol( ptr, &endptr, base );
                                    ຖ້າ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                ບໍ່ດັ່ງນັ້ນ
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    ໃນຂະນະທີ່( cv_isalnum(c) );
                                    ຖ້າ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ຈຳນວນເຕັມ)(endptr - ptr);
                                    ຖ້າ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ບໍ່ດັ່ງນັ້ນ ຖ້າ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ບໍ່ດັ່ງນັ້ນ
                                    {
                                        ຖ້າ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ຖ້າ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                ຖ້າ( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ສົ່ງຄືນ ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, ຈຳນວນເຕັມ& tag_type )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ຖ້າ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ຖ້າ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ຖ້າ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ບໍ່ດັ່ງນັ້ນ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        ສຳລັບ(;;)
        {
            var_na c, *endptr;
            ຖ້າ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            ໃນຂະນະທີ່( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ຖ້າ( tag_name.empty() )
                tag_name = attrname;
            ບໍ່ດັ່ງນັ້ນ
            {
                ຖ້າ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ຖ້າ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ຖ້າ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ຖ້າ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ຖ້າ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ຖ້າ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ຖ້າ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                ສຳລັບ(;;)
                {
                    c = *endptr++;
                    ຖ້າ( c == quote )
                        bha_nga;
                    ຖ້າ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ຖ້າ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            ຖ້າ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ຖ້າ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ຖ້າ( c == '>' )
            {
                ຖ້າ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ຖ້າ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            ຖ້າ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ສົ່ງຄືນ ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        ຈຳນວນເຕັມ tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ຖ້າ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        ໃນຂະນະທີ່( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ຖ້າ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ຖ້າ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                ຖ້າ( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ຖ້າ( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ສົ່ງຄືນ ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<XMLParser>(fs);
}

}
