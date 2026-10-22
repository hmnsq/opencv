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

    void writeTag( const var_na* key, puur_naa_nka tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        puur_naa_nka i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        puur_naa_nka struct_flags = current_struct.flags;

        ᤀᤡ᤺( key && key[0] == '\0' )
            key = 0;

        ᤀᤡ᤺( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ᤀᤡ᤺( FileNode::isCollection(struct_flags) )
            {
                ᤀᤡ᤺( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            ᤀᤡ᤺ᤆᤡ
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            ᤀᤡ᤺( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ᤀᤡ᤺( !key )
            key = "_";
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (puur_naa_nka)strlen( key );
        *ptr++ = '<';
        ᤀᤡ᤺( tag_type == CV_XML_CLOSING_TAG )
        {
            ᤀᤡ᤺( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ᤀᤡ᤺( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        for( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            ᤀᤡ᤺( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        puur_naa_nka nattr = (puur_naa_nka)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        for( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ᤀᤡ᤺( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ᤀᤡ᤺( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 puur_naa_nka struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        ᤀᤡ᤺( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ᤀᤡᤴ current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, puur_naa_nka value)
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
        puur_naa_nka i, len;

        ᤀᤡ᤺( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ᤀᤡ᤺( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ᤀᤡ᤺( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            for( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ᤀᤡ᤺( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ᤀᤡ᤺( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    ᤀᤡ᤺ᤆᤡ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                ᤀᤡ᤺ᤆᤡ
                    *data++ = c;
            }
            ᤀᤡ᤺( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            ᤀᤡ᤺( need_quote )
                *data++ = '\"';
            len = (puur_naa_nka)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (puur_naa_nka)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ᤀᤡ᤺ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        puur_naa_nka len = (puur_naa_nka)strlen(data);
        ᤀᤡ᤺( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        puur_naa_nka struct_flags = current_struct.flags;

        ᤀᤡ᤺( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        ᤀᤡ᤺ᤆᤡ
        {
            var_na* ptr = fs->bufferPtr();
            puur_naa_nka new_offset = (puur_naa_nka)(ptr - fs->bufferStart()) + len;

            ᤀᤡ᤺( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            ᤀᤡ᤺( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        puur_naa_nka len;
        puur_naa_nka multiline;
        const var_na* eol;
        var_na* ptr;

        ᤀᤡ᤺( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ᤀᤡ᤺( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (puur_naa_nka)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ᤀᤡ᤺( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ᤀᤡ᤺( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (puur_naa_nka)strlen(ptr);
        }
        ᤀᤡ᤺ᤆᤡ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ᤀᤡ᤺( multiline )
        {
            ᤀᤠ᤺( comment )
            {
                ᤀᤡ᤺( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    len = (puur_naa_nka)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, puur_naa_nka mode )
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        puur_naa_nka level = 0;

        for(;;)
        {
            var_na c;
            ptr--;

            ᤀᤡ᤺( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                ᤀᤠ᤺( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ᤀᤡ᤺( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ᤀᤡ᤺( level < 0 )
                        ᤀᤡᤴ ptr;
                } ᤀᤠ᤺( cv_isprint_or_tab(c) );
            }
            ᤀᤡ᤺ᤆᤡ
            {
                do c = *++ptr;
                ᤀᤠ᤺( c == ' ' || c == '\t' );

                ᤀᤡ᤺( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ᤀᤡ᤺( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( cv_isprint(c) )
                    bha_nga;
            }

            ᤀᤡ᤺( !cv_isprint(*ptr) )
            {
                ᤀᤡ᤺( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ᤀᤡ᤺( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ᤀᤡᤴ ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ᤀᤡ᤺( !ptr || !*ptr )
            ᤀᤡᤴ false;

        // closing XML tag
        ᤀᤡ᤺ ( *beg == '<' )
            ᤀᤡᤴ false;

        // find end of the row
        ᤀᤠ᤺( cv_isprint(*ptr) )
            ++ptr;
        ᤀᤡ᤺ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ᤀᤡᤴ true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, puur_naa_nka depth = 0 )
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᤀᤡ᤺( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        puur_naa_nka value_type = node.type();
        std::string key, key2, type_name;

        for(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ᤀᤡ᤺( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ᤀᤡ᤺ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ᤀᤡ᤺( c =='<' || c == '\0' )
            {
                puur_naa_nka tag_type = 0;
                puur_naa_nka elem_type = FileNode::NONE;

                ᤀᤡ᤺( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                ᤀᤡ᤺( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ᤀᤡ᤺( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                ᤀᤡ᤺( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    ᤀᤡ᤺( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                ᤀᤡ᤺ (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                ᤀᤡ᤺ᤆᤡ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ᤀᤡ᤺ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ᤀᤡ᤺( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            ᤀᤡ᤺ᤆᤡ
            {
                ᤀᤡ᤺( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                ᤀᤡ᤺( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                ᤀᤡ᤺( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ᤀᤠ᤺( cv_isdigit(*endptr) )
                        endptr++;
                    ᤀᤡ᤺( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    ᤀᤡ᤺ᤆᤡ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    ᤀᤡ᤺( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    // string
                    puur_naa_nka i = 0, len, is_quoted = 0;
                    ᤀᤡ᤺( c == '\"' )
                        is_quoted = 1;
                    ᤀᤡ᤺ᤆᤡ
                        --ptr;
                    strbuf[0] = '\0';

                    for( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ᤀᤡ᤺( !cv_isalnum(c) )
                        {
                            ᤀᤡ᤺( c == '\"' )
                            {
                                ᤀᤡ᤺( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ᤀᤡ᤺( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '&' )
                            {
                                ᤀᤡ᤺( *++ptr == '#' )
                                {
                                    puur_naa_nka val, base = 10;
                                    ptr++;
                                    ᤀᤡ᤺( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (puur_naa_nka)strtol( ptr, &endptr, base );
                                    ᤀᤡ᤺( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                ᤀᤡ᤺ᤆᤡ
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    ᤀᤠ᤺( cv_isalnum(c) );
                                    ᤀᤡ᤺( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (puur_naa_nka)(endptr - ptr);
                                    ᤀᤡ᤺( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    ᤀᤡ᤺ᤆᤡ
                                    {
                                        ᤀᤡ᤺ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ᤀᤡ᤺ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                ᤀᤡ᤺( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ᤀᤡᤴ ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, puur_naa_nka& tag_type )
    {
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ᤀᤡ᤺( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ᤀᤡ᤺( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ᤀᤡ᤺( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        ᤀᤡ᤺ᤆᤡ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        for(;;)
        {
            var_na c, *endptr;
            ᤀᤡ᤺( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            ᤀᤠ᤺( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ᤀᤡ᤺( tag_name.empty() )
                tag_name = attrname;
            ᤀᤡ᤺ᤆᤡ
            {
                ᤀᤡ᤺( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ᤀᤡ᤺( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ᤀᤡ᤺ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ᤀᤡ᤺( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ᤀᤡ᤺( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ᤀᤡ᤺(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ᤀᤡ᤺( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                for(;;)
                {
                    c = *endptr++;
                    ᤀᤡ᤺( c == quote )
                        bha_nga;
                    ᤀᤡ᤺( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ᤀᤡ᤺( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            ᤀᤡ᤺( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ᤀᤡ᤺ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ᤀᤡ᤺( c == '>' )
            {
                ᤀᤡ᤺( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ᤀᤡ᤺( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            ᤀᤡ᤺( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ᤀᤡᤴ ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        puur_naa_nka tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ᤀᤡ᤺ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ᤀᤡ᤺( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        ᤀᤠ᤺( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ᤀᤡ᤺ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ᤀᤡ᤺( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                ᤀᤡ᤺( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ᤀᤡ᤺( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ᤀᤡᤴ ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ᤀᤡᤴ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ᤀᤡᤴ makePtr<XMLParser>(fs);
}

}
