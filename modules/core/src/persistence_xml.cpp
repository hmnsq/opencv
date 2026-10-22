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

    void writeTag( const char* key, целое tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        char* ptr = fs->bufferPtr();
        целое i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        целое struct_flags = current_struct.flags;

        если( key && key[0] == '\0' )
            key = 0;

        если( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            если( FileNode::isCollection(struct_flags) )
            {
                если( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            иначе
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            если( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        если( !key )
            key = "_";
        иначе если( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (целое)strlen( key );
        *ptr++ = '<';
        если( tag_type == CV_XML_CLOSING_TAG )
        {
            если( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        если( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        для( i = 0; i < len; i++ )
        {
            char c = key[i];
            если( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        целое nattr = (целое)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        для( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (целое)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            если( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        если( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const char* key,
                                 целое struct_flags, const char* type_name=0)
    {
        std::vector<std::string> attrlist;
        если( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        вернуть current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const char* key, целое value)
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
        целое i, len;

        если( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (целое)strlen(str);
        если( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        если( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            для( i = 0; i < len; i++ )
            {
                char c = str[i];

                если( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                иначе если( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    если( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    иначе если( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    иначе если( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    иначе если( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    иначе если( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    иначе
                    {
                        snprintf( data, sizeof(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                иначе
                    *data++ = c;
            }
            если( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            если( need_quote )
                *data++ = '\"';
            len = (целое)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (целое)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const char* key, const char* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        если ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        иначе если ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        целое len = (целое)strlen(data);
        если( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        целое struct_flags = current_struct.flags;

        если( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            char* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        иначе
        {
            char* ptr = fs->bufferPtr();
            целое new_offset = (целое)(ptr - fs->bufferStart()) + len;

            если( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            если( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            иначе если( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const char* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        целое len;
        целое multiline;
        const char* eol;
        char* ptr;

        если( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        если( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (целое)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        если( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        иначе если( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        если( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (целое)strlen(ptr);
        }
        иначе
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        если( multiline )
        {
            пока( comment )
            {
                если( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (целое)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                иначе
                {
                    len = (целое)strlen(comment);
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

    char* skipSpaces( char* ptr, целое mode )
    {
        если (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        целое level = 0;

        для(;;)
        {
            char c;
            ptr--;

            если( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                пока( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                если( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            иначе если( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    если( level < 0 )
                        вернуть ptr;
                } пока( cv_isprint_or_tab(c) );
            }
            иначе
            {
                do c = *++ptr;
                пока( c == ' ' || c == '\t' );

                если( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    если( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                иначе если( cv_isprint(c) )
                    break;
            }

            если( !cv_isprint(*ptr) )
            {
                если( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                если( !ptr || *ptr == '\0' )
                    break;
            }
        }
        вернуть ptr;
    }

    bool getBase64Row(char* ptr, целое /*indent*/, char* &beg, char* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        если( !ptr || !*ptr )
            вернуть false;

        // closing XML tag
        если ( *beg == '<' )
            вернуть false;

        // find end of the row
        пока( cv_isprint(*ptr) )
            ++ptr;
        если ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        вернуть true;
    }

    char* parseValue( char* ptr, FileNode& node, целое depth = 0 )
    {
        если (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        если( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        целое value_type = node.type();
        std::string key, key2, type_name;

        для(;;)
        {
            char c = *ptr, d;
            char* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            если( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                если (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            если( c =='<' || c == '\0' )
            {
                целое tag_type = 0;
                целое elem_type = FileNode::NONE;

                если( d == '/' || c == '\0' )
                    break;

                ptr = parseTag( ptr, key, type_name, tag_type );

                если( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                если( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                если( !type_name.empty() )
                {
                    const char* tn = type_name.c_str();
                    если( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    иначе если( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    иначе если( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    иначе если( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                если (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                иначе
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    если (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                если( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            иначе
            {
                если( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                если( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                если( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    пока( cv_isdigit(*endptr) )
                        endptr++;
                    если( *endptr == '.' || *endptr == 'e' )
                    {
                        double fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    иначе
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    если( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                иначе
                {
                    // string
                    целое i = 0, len, is_quoted = 0;
                    если( c == '\"' )
                        is_quoted = 1;
                    иначе
                        --ptr;
                    strbuf[0] = '\0';

                    для( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        если( !cv_isalnum(c) )
                        {
                            если( c == '\"' )
                            {
                                если( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                break;
                            }
                            иначе если( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                если( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                break;
                            }
                            иначе если( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            иначе если( c == '&' )
                            {
                                если( *++ptr == '#' )
                                {
                                    целое val, base = 10;
                                    ptr++;
                                    если( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (целое)strtol( ptr, &endptr, base );
                                    если( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (char)val;
                                }
                                иначе
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    пока( cv_isalnum(c) );
                                    если( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (целое)(endptr - ptr);
                                    если( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    иначе если( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    иначе если( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    иначе если( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    иначе если( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    иначе
                                    {
                                        если (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        если (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                если( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    break;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        вернуть ptr;
    }

    char* parseTag( char* ptr, std::string& tag_name,
                    std::string& type_name, целое& tag_type )
    {
        если (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        если( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        если( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        если( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        иначе если( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        иначе если( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        иначе если( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        иначе
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        для(;;)
        {
            char c, *endptr;
            если( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            пока( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            если( tag_name.empty() )
                tag_name = attrname;
            иначе
            {
                если( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                если( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    если (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    если( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                если( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    если(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    если( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                char quote = *ptr++;
                endptr = ptr;
                для(;;)
                {
                    c = *endptr++;
                    если( c == quote )
                        break;
                    если( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                если( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            если( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                если (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            если( c == '>' )
            {
                если( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                break;
            }
            иначе если( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                если( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                break;
            }
            иначе если( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                break;
            }

            если( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        вернуть ptr;
    }

    bool parse(char* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        целое tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        если (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        если( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        пока( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            если (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            если( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                если( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                если( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        вернуть ok;
    }

    FileStorage_API* fs;
    char strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    вернуть makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    вернуть makePtr<XMLParser>(fs);
}

}
