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

    void writeTag( const char* key, 정수 tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        char* ptr = fs->bufferPtr();
        정수 i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        정수 struct_flags = current_struct.flags;

        만약( key && key[0] == '\0' )
            key = 0;

        만약( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            만약( FileNode::isCollection(struct_flags) )
            {
                만약( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            아니면
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            만약( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        만약( !key )
            key = "_";
        아니면 만약( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (정수)strlen( key );
        *ptr++ = '<';
        만약( tag_type == CV_XML_CLOSING_TAG )
        {
            만약( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        만약( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        위해( i = 0; i < len; i++ )
        {
            char c = key[i];
            만약( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        정수 nattr = (정수)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        위해( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (정수)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            만약( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        만약( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const char* key,
                                 정수 struct_flags, const char* type_name=0)
    {
        std::vector<std::string> attrlist;
        만약( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        반환 current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const char* key, 정수 value)
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
        정수 i, len;

        만약( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (정수)strlen(str);
        만약( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        만약( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            위해( i = 0; i < len; i++ )
            {
                char c = str[i];

                만약( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                아니면 만약( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    만약( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    아니면 만약( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    아니면 만약( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    아니면 만약( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    아니면 만약( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    아니면
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                아니면
                    *data++ = c;
            }
            만약( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            만약( need_quote )
                *data++ = '\"';
            len = (정수)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (정수)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        만약 ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        아니면 만약 ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        정수 len = (정수)strlen(data);
        만약( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        정수 struct_flags = current_struct.flags;

        만약( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            char* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        아니면
        {
            char* ptr = fs->bufferPtr();
            정수 new_offset = (정수)(ptr - fs->bufferStart()) + len;

            만약( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            만약( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            아니면 만약( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        정수 len;
        정수 multiline;
        const char* eol;
        char* ptr;

        만약( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        만약( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (정수)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        만약( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        아니면 만약( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        만약( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (정수)strlen(ptr);
        }
        아니면
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        만약( multiline )
        {
            동안( comment )
            {
                만약( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (정수)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                아니면
                {
                    len = (정수)strlen(comment);
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

    char* skipSpaces( char* ptr, 정수 mode )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        정수 level = 0;

        위해(;;)
        {
            char c;
            ptr--;

            만약( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                동안( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                만약( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            아니면 만약( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    만약( level < 0 )
                        반환 ptr;
                } 동안( cv_isprint_or_tab(c) );
            }
            아니면
            {
                do c = *++ptr;
                동안( c == ' ' || c == '\t' );

                만약( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    만약( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                아니면 만약( cv_isprint(c) )
                    break;
            }

            만약( !cv_isprint(*ptr) )
            {
                만약( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                만약( !ptr || *ptr == '\0' )
                    break;
            }
        }
        반환 ptr;
    }

    bool getBase64Row(char* ptr, 정수 /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        만약( !ptr || !*ptr )
            반환 false;

        // closing XML tag
        만약 ( *beg == '<' )
            반환 false;

        // find end of the row
        동안( cv_isprint(*ptr) )
            ++ptr;
        만약 ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        반환 true;
    }

    char* parseValue( char* ptr, FileNode& node, 정수 depth = 0 )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        만약( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        정수 value_type = node.type();
        std::string key, key2, type_name;

        위해(;;)
        {
            char c = *ptr, d;
            char* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            만약( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                만약 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            만약( c =='<' || c == '\0' )
            {
                정수 tag_type = 0;
                정수 elem_type = FileNode::NONE;

                만약( d == '/' || c == '\0' )
                    break;

                ptr = parseTag( ptr, key, type_name, tag_type );

                만약( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                만약( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                만약( !type_name.empty() )
                {
                    const char* tn = type_name.c_str();
                    만약( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    아니면 만약( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    아니면 만약( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    아니면 만약( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                만약 (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                아니면
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    만약 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                만약( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            아니면
            {
                만약( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                만약( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                만약( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    동안( cv_isdigit(*endptr) )
                        endptr++;
                    만약( *endptr == '.' || *endptr == 'e' )
                    {
                        double fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    아니면
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    만약( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                아니면
                {
                    // string
                    정수 i = 0, len, is_quoted = 0;
                    만약( c == '\"' )
                        is_quoted = 1;
                    아니면
                        --ptr;
                    strbuf[0] = '\0';

                    위해( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        만약( !cv_isalnum(c) )
                        {
                            만약( c == '\"' )
                            {
                                만약( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                break;
                            }
                            아니면 만약( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                만약( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                break;
                            }
                            아니면 만약( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            아니면 만약( c == '&' )
                            {
                                만약( *++ptr == '#' )
                                {
                                    정수 val, base = 10;
                                    ptr++;
                                    만약( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (정수)strtol( ptr, &endptr, base );
                                    만약( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (char)val;
                                }
                                아니면
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    동안( cv_isalnum(c) );
                                    만약( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (정수)(endptr - ptr);
                                    만약( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    아니면 만약( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    아니면 만약( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    아니면 만약( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    아니면 만약( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    아니면
                                    {
                                        만약 (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        만약 (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                만약( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    break;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        반환 ptr;
    }

    char* parseTag( char* ptr, std::string& tag_name,
                    std::string& type_name, 정수& tag_type )
    {
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        만약( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        만약( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        만약( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        아니면 만약( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        아니면 만약( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        아니면 만약( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        아니면
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        위해(;;)
        {
            char c, *endptr;
            만약( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            동안( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            만약( tag_name.empty() )
                tag_name = attrname;
            아니면
            {
                만약( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                만약( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    만약 (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    만약( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                만약( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    만약(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    만약( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                char quote = *ptr++;
                endptr = ptr;
                위해(;;)
                {
                    c = *endptr++;
                    만약( c == quote )
                        break;
                    만약( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                만약( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            만약( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                만약 (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            만약( c == '>' )
            {
                만약( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                break;
            }
            아니면 만약( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                만약( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                break;
            }
            아니면 만약( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                break;
            }

            만약( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        반환 ptr;
    }

    bool parse(char* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        정수 tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        만약 (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        만약( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        동안( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            만약 (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            만약( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                만약( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                만약( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        반환 ok;
    }

    FileStorage_API* fs;
    char strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    반환 makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    반환 makePtr<XMLParser>(fs);
}

}
