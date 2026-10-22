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

    void writeTag( const char* key, 整数 tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        char* ptr = fs->bufferPtr();
        整数 i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        整数 struct_flags = current_struct.flags;

        如果( key && key[0] == '\0' )
            key = 0;

        如果( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            如果( FileNode::isCollection(struct_flags) )
            {
                如果( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            否则
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            如果( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        如果( !key )
            key = "_";
        否则 如果( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (整数)strlen( key );
        *ptr++ = '<';
        如果( tag_type == CV_XML_CLOSING_TAG )
        {
            如果( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        如果( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        为( i = 0; i < len; i++ )
        {
            char c = key[i];
            如果( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        整数 nattr = (整数)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        为( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (整数)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            如果( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        如果( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const char* key,
                                 整数 struct_flags, const char* type_name=0)
    {
        std::vector<std::string> attrlist;
        如果( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        返回 current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const char* key, 整数 value)
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
        整数 i, len;

        如果( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (整数)strlen(str);
        如果( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        如果( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            为( i = 0; i < len; i++ )
            {
                char c = str[i];

                如果( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                否则 如果( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    如果( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    否则 如果( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    否则 如果( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    否则 如果( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    否则 如果( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    否则
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                否则
                    *data++ = c;
            }
            如果( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            如果( need_quote )
                *data++ = '\"';
            len = (整数)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (整数)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        如果 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        否则 如果 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        整数 len = (整数)strlen(data);
        如果( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        整数 struct_flags = current_struct.flags;

        如果( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            char* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        否则
        {
            char* ptr = fs->bufferPtr();
            整数 new_offset = (整数)(ptr - fs->bufferStart()) + len;

            如果( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            如果( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            否则 如果( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        整数 len;
        整数 multiline;
        const char* eol;
        char* ptr;

        如果( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        如果( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (整数)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        如果( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        否则 如果( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        如果( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (整数)strlen(ptr);
        }
        否则
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        如果( multiline )
        {
            当( comment )
            {
                如果( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (整数)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                否则
                {
                    len = (整数)strlen(comment);
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

    char* skipSpaces( char* ptr, 整数 mode )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        整数 level = 0;

        为(;;)
        {
            char c;
            ptr--;

            如果( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                当( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                如果( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            否则 如果( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    如果( level < 0 )
                        返回 ptr;
                } 当( cv_isprint_or_tab(c) );
            }
            否则
            {
                do c = *++ptr;
                当( c == ' ' || c == '\t' );

                如果( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    如果( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                否则 如果( cv_isprint(c) )
                    break;
            }

            如果( !cv_isprint(*ptr) )
            {
                如果( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                如果( !ptr || *ptr == '\0' )
                    break;
            }
        }
        返回 ptr;
    }

    bool getBase64Row(char* ptr, 整数 /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        如果( !ptr || !*ptr )
            返回 false;

        // closing XML tag
        如果 ( *beg == '<' )
            返回 false;

        // find end of the row
        当( cv_isprint(*ptr) )
            ++ptr;
        如果 ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        返回 true;
    }

    char* parseValue( char* ptr, FileNode& node, 整数 depth = 0 )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        如果( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        整数 value_type = node.type();
        std::string key, key2, type_name;

        为(;;)
        {
            char c = *ptr, d;
            char* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            如果( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                如果 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            如果( c =='<' || c == '\0' )
            {
                整数 tag_type = 0;
                整数 elem_type = FileNode::NONE;

                如果( d == '/' || c == '\0' )
                    break;

                ptr = parseTag( ptr, key, type_name, tag_type );

                如果( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                如果( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                如果( !type_name.empty() )
                {
                    const char* tn = type_name.c_str();
                    如果( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    否则 如果( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    否则 如果( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    否则 如果( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                如果 (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                否则
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    如果 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                如果( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            否则
            {
                如果( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                如果( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                如果( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    当( cv_isdigit(*endptr) )
                        endptr++;
                    如果( *endptr == '.' || *endptr == 'e' )
                    {
                        double fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    否则
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    如果( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                否则
                {
                    // string
                    整数 i = 0, len, is_quoted = 0;
                    如果( c == '\"' )
                        is_quoted = 1;
                    否则
                        --ptr;
                    strbuf[0] = '\0';

                    为( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        如果( !cv_isalnum(c) )
                        {
                            如果( c == '\"' )
                            {
                                如果( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                break;
                            }
                            否则 如果( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                如果( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                break;
                            }
                            否则 如果( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            否则 如果( c == '&' )
                            {
                                如果( *++ptr == '#' )
                                {
                                    整数 val, base = 10;
                                    ptr++;
                                    如果( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (整数)strtol( ptr, &endptr, base );
                                    如果( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (char)val;
                                }
                                否则
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    当( cv_isalnum(c) );
                                    如果( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (整数)(endptr - ptr);
                                    如果( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    否则 如果( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    否则 如果( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    否则 如果( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    否则 如果( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    否则
                                    {
                                        如果 (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        如果 (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                如果( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    break;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        返回 ptr;
    }

    char* parseTag( char* ptr, std::string& tag_name,
                    std::string& type_name, 整数& tag_type )
    {
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        如果( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        如果( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        如果( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        否则 如果( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        否则 如果( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        否则 如果( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        否则
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        为(;;)
        {
            char c, *endptr;
            如果( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            当( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            如果( tag_name.empty() )
                tag_name = attrname;
            否则
            {
                如果( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                如果( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    如果 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    如果( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                如果( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    如果(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    如果( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                char quote = *ptr++;
                endptr = ptr;
                为(;;)
                {
                    c = *endptr++;
                    如果( c == quote )
                        break;
                    如果( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                如果( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            如果( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                如果 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            如果( c == '>' )
            {
                如果( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                break;
            }
            否则 如果( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                如果( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                break;
            }
            否则 如果( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                break;
            }

            如果( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        返回 ptr;
    }

    bool parse(char* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        整数 tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        如果 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        如果( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        当( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            如果 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            如果( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                如果( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                如果( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        返回 ok;
    }

    FileStorage_API* fs;
    char strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    返回 makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    返回 makePtr<XMLParser>(fs);
}

}
