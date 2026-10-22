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

    void writeTag( const var_na* key, පූර්ණ_සංඛ්‍යා tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        පූර්ණ_සංඛ්‍යා i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        පූර්ණ_සංඛ්‍යා struct_flags = current_struct.flags;

        නම්( key && key[0] == '\0' )
            key = 0;

        නම්( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            නම්( FileNode::isCollection(struct_flags) )
            {
                නම්( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            නැතහොත්
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            නම්( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        නම්( !key )
            key = "_";
        නැතහොත් නම්( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (පූර්ණ_සංඛ්‍යා)strlen( key );
        *ptr++ = '<';
        නම්( tag_type == CV_XML_CLOSING_TAG )
        {
            නම්( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        නම්( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        සඳහා( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            නම්( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        පූර්ණ_සංඛ්‍යා nattr = (පූර්ණ_සංඛ්‍යා)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        සඳහා( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (පූර්ණ_සංඛ්‍යා)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            නම්( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        නම්( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 පූර්ණ_සංඛ්‍යා struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        නම්( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ආපසු current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, පූර්ණ_සංඛ්‍යා value)
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
        පූර්ණ_සංඛ්‍යා i, len;

        නම්( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (පූර්ණ_සංඛ්‍යා)strlen(str);
        නම්( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        නම්( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            සඳහා( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                නම්( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                නැතහොත් නම්( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    නම්( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    නැතහොත් නම්( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    නැතහොත් නම්( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    නැතහොත් නම්( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    නැතහොත් නම්( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    නැතහොත්
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                නැතහොත්
                    *data++ = c;
            }
            නම්( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            නම්( need_quote )
                *data++ = '\"';
            len = (පූර්ණ_සංඛ්‍යා)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (පූර්ණ_සංඛ්‍යා)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        නම් ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        නැතහොත් නම් ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        පූර්ණ_සංඛ්‍යා len = (පූර්ණ_සංඛ්‍යා)strlen(data);
        නම්( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        පූර්ණ_සංඛ්‍යා struct_flags = current_struct.flags;

        නම්( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        නැතහොත්
        {
            var_na* ptr = fs->bufferPtr();
            පූර්ණ_සංඛ්‍යා new_offset = (පූර්ණ_සංඛ්‍යා)(ptr - fs->bufferStart()) + len;

            නම්( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            නම්( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            නැතහොත් නම්( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        පූර්ණ_සංඛ්‍යා len;
        පූර්ණ_සංඛ්‍යා multiline;
        const var_na* eol;
        var_na* ptr;

        නම්( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        නම්( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (පූර්ණ_සංඛ්‍යා)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        නම්( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        නැතහොත් නම්( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        නම්( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (පූර්ණ_සංඛ්‍යා)strlen(ptr);
        }
        නැතහොත්
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        නම්( multiline )
        {
            අතරතුර( comment )
            {
                නම්( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (පූර්ණ_සංඛ්‍යා)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                නැතහොත්
                {
                    len = (පූර්ණ_සංඛ්‍යා)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, පූර්ණ_සංඛ්‍යා mode )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        පූර්ණ_සංඛ්‍යා level = 0;

        සඳහා(;;)
        {
            var_na c;
            ptr--;

            නම්( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                අතරතුර( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                නම්( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            නැතහොත් නම්( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    නම්( level < 0 )
                        ආපසු ptr;
                } අතරතුර( cv_isprint_or_tab(c) );
            }
            නැතහොත්
            {
                do c = *++ptr;
                අතරතුර( c == ' ' || c == '\t' );

                නම්( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    නම්( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                නැතහොත් නම්( cv_isprint(c) )
                    bha_nga;
            }

            නම්( !cv_isprint(*ptr) )
            {
                නම්( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                නම්( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ආපසු ptr;
    }

    bool getBase64Row(var_na* ptr, පූර්ණ_සංඛ්‍යා /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        නම්( !ptr || !*ptr )
            ආපසු false;

        // closing XML tag
        නම් ( *beg == '<' )
            ආපසු false;

        // find end of the row
        අතරතුර( cv_isprint(*ptr) )
            ++ptr;
        නම් ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ආපසු true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, පූර්ණ_සංඛ්‍යා depth = 0 )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        නම්( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        පූර්ණ_සංඛ්‍යා value_type = node.type();
        std::string key, key2, type_name;

        සඳහා(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            නම්( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                නම් (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            නම්( c =='<' || c == '\0' )
            {
                පූර්ණ_සංඛ්‍යා tag_type = 0;
                පූර්ණ_සංඛ්‍යා elem_type = FileNode::NONE;

                නම්( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                නම්( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                නම්( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                නම්( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    නම්( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    නැතහොත් නම්( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    නැතහොත් නම්( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    නැතහොත් නම්( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                නම් (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                නැතහොත්
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    නම් (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                නම්( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            නැතහොත්
            {
                නම්( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                නම්( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                නම්( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    අතරතුර( cv_isdigit(*endptr) )
                        endptr++;
                    නම්( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    නැතහොත්
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    නම්( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                නැතහොත්
                {
                    // string
                    පූර්ණ_සංඛ්‍යා i = 0, len, is_quoted = 0;
                    නම්( c == '\"' )
                        is_quoted = 1;
                    නැතහොත්
                        --ptr;
                    strbuf[0] = '\0';

                    සඳහා( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        නම්( !cv_isalnum(c) )
                        {
                            නම්( c == '\"' )
                            {
                                නම්( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            නැතහොත් නම්( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                නම්( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            නැතහොත් නම්( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            නැතහොත් නම්( c == '&' )
                            {
                                නම්( *++ptr == '#' )
                                {
                                    පූර්ණ_සංඛ්‍යා val, base = 10;
                                    ptr++;
                                    නම්( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (පූර්ණ_සංඛ්‍යා)strtol( ptr, &endptr, base );
                                    නම්( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                නැතහොත්
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    අතරතුර( cv_isalnum(c) );
                                    නම්( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (පූර්ණ_සංඛ්‍යා)(endptr - ptr);
                                    නම්( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    නැතහොත් නම්( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    නැතහොත් නම්( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    නැතහොත් නම්( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    නැතහොත් නම්( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    නැතහොත්
                                    {
                                        නම් (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        නම් (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                නම්( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ආපසු ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, පූර්ණ_සංඛ්‍යා& tag_type )
    {
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        නම්( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        නම්( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        නම්( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        නැතහොත් නම්( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        නැතහොත් නම්( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        නැතහොත් නම්( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        නැතහොත්
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        සඳහා(;;)
        {
            var_na c, *endptr;
            නම්( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            අතරතුර( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            නම්( tag_name.empty() )
                tag_name = attrname;
            නැතහොත්
            {
                නම්( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                නම්( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    නම් (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    නම්( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                නම්( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    නම්(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    නම්( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                සඳහා(;;)
                {
                    c = *endptr++;
                    නම්( c == quote )
                        bha_nga;
                    නම්( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                නම්( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            නම්( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                නම් (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            නම්( c == '>' )
            {
                නම්( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            නැතහොත් නම්( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                නම්( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            නැතහොත් නම්( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            නම්( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ආපසු ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        පූර්ණ_සංඛ්‍යා tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        නම් (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        නම්( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        අතරතුර( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            නම් (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            නම්( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                නම්( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                නම්( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ආපසු ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ආපසු makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ආපසු makePtr<XMLParser>(fs);
}

}
