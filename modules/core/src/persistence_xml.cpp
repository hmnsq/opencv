// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "persistence.hpp"

enum
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

    void writeTag( const char* key, số_nguyên tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        char* ptr = fs->bufferPtr();
        số_nguyên i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        số_nguyên struct_flags = current_struct.flags;

        nếu( key && key[0] == '\0' )
            key = 0;

        nếu( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            nếu( FileNode::isCollection(struct_flags) )
            {
                nếu( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            không_thì
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            nếu( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        nếu( !key )
            key = "_";
        không_thì nếu( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (số_nguyên)strlen( key );
        *ptr++ = '<';
        nếu( tag_type == CV_XML_CLOSING_TAG )
        {
            nếu( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        nếu( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        cho( i = 0; i < len; i++ )
        {
            char c = key[i];
            nếu( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        số_nguyên nattr = (số_nguyên)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        cho( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (số_nguyên)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            nếu( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        nếu( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const char* key,
                                 số_nguyên struct_flags, const char* type_name=0)
    {
        std::vector<std::string> attrlist;
        nếu( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        trả_về current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const char* key, số_nguyên value)
    {
        char buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( key, ptr);
    }

    void write(const char* key, int64_t value)
    {
        char buf[128], *ptr = fs::itoa( value, buf, 10, true );
        writeScalar( key, ptr);
    }

    void write( const char* key, double value )
    {
        char buf[128];
        writeScalar( key, fs::doubleToString( buf, sizeof(buf), value, false ) );
    }

    void write(const char* key, const char* str, bool quote)
    {
        char buf[CV_FS_MAX_LEN*6+16];
        char* data = (char*)str;
        số_nguyên i, len;

        nếu( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (số_nguyên)strlen(str);
        nếu( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        nếu( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            cho( i = 0; i < len; i++ )
            {
                char c = str[i];

                nếu( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                không_thì nếu( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    nếu( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    không_thì nếu( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    không_thì nếu( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    không_thì nếu( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    không_thì nếu( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    không_thì
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                không_thì
                    *data++ = c;
            }
            nếu( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            nếu( need_quote )
                *data++ = '\"';
            len = (số_nguyên)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (số_nguyên)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        nếu ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        không_thì nếu ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        số_nguyên len = (số_nguyên)strlen(data);
        nếu( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        số_nguyên struct_flags = current_struct.flags;

        nếu( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            char* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        không_thì
        {
            char* ptr = fs->bufferPtr();
            số_nguyên new_offset = (số_nguyên)(ptr - fs->bufferStart()) + len;

            nếu( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            nếu( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            không_thì nếu( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        số_nguyên len;
        số_nguyên multiline;
        const char* eol;
        char* ptr;

        nếu( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        nếu( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (số_nguyên)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        nếu( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        không_thì nếu( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        nếu( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (số_nguyên)strlen(ptr);
        }
        không_thì
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        nếu( multiline )
        {
            trong_khi( comment )
            {
                nếu( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (số_nguyên)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                không_thì
                {
                    len = (số_nguyên)strlen(comment);
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

protected:
    FileStorage_API* fs;
};

class XMLParser : public FileStorageParser
{
public:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~XMLParser() {}

    char* skipSpaces( char* ptr, số_nguyên mode )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        số_nguyên level = 0;

        cho(;;)
        {
            char c;
            ptr--;

            nếu( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                trong_khi( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                nếu( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            không_thì nếu( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    nếu( level < 0 )
                        trả_về ptr;
                } trong_khi( cv_isprint_or_tab(c) );
            }
            không_thì
            {
                do c = *++ptr;
                trong_khi( c == ' ' || c == '\t' );

                nếu( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    nếu( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                không_thì nếu( cv_isprint(c) )
                    break;
            }

            nếu( !cv_isprint(*ptr) )
            {
                nếu( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                nếu( !ptr || *ptr == '\0' )
                    break;
            }
        }
        trả_về ptr;
    }

    bool getBase64Row(char* ptr, số_nguyên /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        nếu( !ptr || !*ptr )
            trả_về false;

        // closing XML tag
        nếu ( *beg == '<' )
            trả_về false;

        // find end of the row
        trong_khi( cv_isprint(*ptr) )
            ++ptr;
        nếu ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        trả_về true;
    }

    char* parseValue( char* ptr, FileNode& node, số_nguyên depth = 0 )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        nếu( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        số_nguyên value_type = node.type();
        std::string key, key2, type_name;

        cho(;;)
        {
            char c = *ptr, d;
            char* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            nếu( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                nếu (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            nếu( c =='<' || c == '\0' )
            {
                số_nguyên tag_type = 0;
                số_nguyên elem_type = FileNode::NONE;

                nếu( d == '/' || c == '\0' )
                    break;

                ptr = parseTag( ptr, key, type_name, tag_type );

                nếu( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                nếu( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                nếu( !type_name.empty() )
                {
                    const char* tn = type_name.c_str();
                    nếu( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    không_thì nếu( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    không_thì nếu( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    không_thì nếu( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                nếu (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                không_thì
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    nếu (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                nếu( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            không_thì
            {
                nếu( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                nếu( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                nếu( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    trong_khi( cv_isdigit(*endptr) )
                        endptr++;
                    nếu( *endptr == '.' || *endptr == 'e' )
                    {
                        double fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    không_thì
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    nếu( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                không_thì
                {
                    // string
                    số_nguyên i = 0, len, is_quoted = 0;
                    nếu( c == '\"' )
                        is_quoted = 1;
                    không_thì
                        --ptr;
                    strbuf[0] = '\0';

                    cho( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        nếu( !cv_isalnum(c) )
                        {
                            nếu( c == '\"' )
                            {
                                nếu( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                break;
                            }
                            không_thì nếu( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                nếu( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                break;
                            }
                            không_thì nếu( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            không_thì nếu( c == '&' )
                            {
                                nếu( *++ptr == '#' )
                                {
                                    số_nguyên val, base = 10;
                                    ptr++;
                                    nếu( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (số_nguyên)strtol( ptr, &endptr, base );
                                    nếu( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (char)val;
                                }
                                không_thì
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    trong_khi( cv_isalnum(c) );
                                    nếu( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (số_nguyên)(endptr - ptr);
                                    nếu( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    không_thì nếu( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    không_thì nếu( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    không_thì nếu( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    không_thì nếu( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    không_thì
                                    {
                                        nếu (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        nếu (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                nếu( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    break;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        trả_về ptr;
    }

    char* parseTag( char* ptr, std::string& tag_name,
                    std::string& type_name, số_nguyên& tag_type )
    {
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        nếu( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        nếu( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        nếu( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        không_thì nếu( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        không_thì nếu( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        không_thì nếu( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        không_thì
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        cho(;;)
        {
            char c, *endptr;
            nếu( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            trong_khi( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            nếu( tag_name.empty() )
                tag_name = attrname;
            không_thì
            {
                nếu( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                nếu( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    nếu (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    nếu( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                nếu( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    nếu(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    nếu( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                char quote = *ptr++;
                endptr = ptr;
                cho(;;)
                {
                    c = *endptr++;
                    nếu( c == quote )
                        break;
                    nếu( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                nếu( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            nếu( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                nếu (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            nếu( c == '>' )
            {
                nếu( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                break;
            }
            không_thì nếu( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                nếu( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                break;
            }
            không_thì nếu( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                break;
            }

            nếu( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        trả_về ptr;
    }

    bool parse(char* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        số_nguyên tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        nếu (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        nếu( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        trong_khi( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            nếu (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            nếu( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                nếu( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                nếu( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        trả_về ok;
    }

    FileStorage_API* fs;
    char strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    trả_về makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    trả_về makePtr<XMLParser>(fs);
}

}
