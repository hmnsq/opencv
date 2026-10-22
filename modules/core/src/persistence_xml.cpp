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

    void writeTag( const var_na* key, ཧྲིལ tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        ཧྲིལ i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ཧྲིལ struct_flags = current_struct.flags;

        གལ་སྲིད( key && key[0] == '\0' )
            key = 0;

        གལ་སྲིད( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            གལ་སྲིད( FileNode::isCollection(struct_flags) )
            {
                གལ་སྲིད( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            དེ་མིན
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            གལ་སྲིད( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        གལ་སྲིད( !key )
            key = "_";
        དེ་མིན གལ་སྲིད( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ཧྲིལ)strlen( key );
        *ptr++ = '<';
        གལ་སྲིད( tag_type == CV_XML_CLOSING_TAG )
        {
            གལ་སྲིད( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        གལ་སྲིད( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        དོན་ལུ( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            གལ་སྲིད( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ཧྲིལ nattr = (ཧྲིལ)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        དོན་ལུ( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ཧྲིལ)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            གལ་སྲིད( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        གལ་སྲིད( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 ཧྲིལ struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        གལ་སྲིད( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ལོག current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, ཧྲིལ value)
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
        ཧྲིལ i, len;

        གལ་སྲིད( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ཧྲིལ)strlen(str);
        གལ་སྲིད( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        གལ་སྲིད( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            དོན་ལུ( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                གལ་སྲིད( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                དེ་མིན གལ་སྲིད( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    གལ་སྲིད( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    དེ་མིན གལ་སྲིད( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    དེ་མིན གལ་སྲིད( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    དེ་མིན གལ་སྲིད( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    དེ་མིན གལ་སྲིད( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    དེ་མིན
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                དེ་མིན
                    *data++ = c;
            }
            གལ་སྲིད( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            གལ་སྲིད( need_quote )
                *data++ = '\"';
            len = (ཧྲིལ)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ཧྲིལ)!need_quote;
        }

        writeScalar( key, data );
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

        ཧྲིལ len = (ཧྲིལ)strlen(data);
        གལ་སྲིད( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ཧྲིལ struct_flags = current_struct.flags;

        གལ་སྲིད( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        དེ་མིན
        {
            var_na* ptr = fs->bufferPtr();
            ཧྲིལ new_offset = (ཧྲིལ)(ptr - fs->bufferStart()) + len;

            གལ་སྲིད( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            གལ་སྲིད( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            དེ་མིན གལ་སྲིད( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ཧྲིལ len;
        ཧྲིལ multiline;
        const var_na* eol;
        var_na* ptr;

        གལ་སྲིད( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        གལ་སྲིད( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ཧྲིལ)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        གལ་སྲིད( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        དེ་མིན གལ་སྲིད( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        གལ་སྲིད( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ཧྲིལ)strlen(ptr);
        }
        དེ་མིན
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        གལ་སྲིད( multiline )
        {
            སྐབས( comment )
            {
                གལ་སྲིད( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ཧྲིལ)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                དེ་མིན
                {
                    len = (ཧྲིལ)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ཧྲིལ mode )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ཧྲིལ level = 0;

        དོན་ལུ(;;)
        {
            var_na c;
            ptr--;

            གལ་སྲིད( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                སྐབས( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                གལ་སྲིད( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            དེ་མིན གལ་སྲིད( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    གལ་སྲིད( level < 0 )
                        ལོག ptr;
                } སྐབས( cv_isprint_or_tab(c) );
            }
            དེ་མིན
            {
                do c = *++ptr;
                སྐབས( c == ' ' || c == '\t' );

                གལ་སྲིད( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    གལ་སྲིད( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                དེ་མིན གལ་སྲིད( cv_isprint(c) )
                    bha_nga;
            }

            གལ་སྲིད( !cv_isprint(*ptr) )
            {
                གལ་སྲིད( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                གལ་སྲིད( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ལོག ptr;
    }

    bool getBase64Row(var_na* ptr, ཧྲིལ /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        གལ་སྲིད( !ptr || !*ptr )
            ལོག false;

        // closing XML tag
        གལ་སྲིད ( *beg == '<' )
            ལོག false;

        // find end of the row
        སྐབས( cv_isprint(*ptr) )
            ++ptr;
        གལ་སྲིད ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ལོག true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ཧྲིལ depth = 0 )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        གལ་སྲིད( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        ཧྲིལ value_type = node.type();
        std::string key, key2, type_name;

        དོན་ལུ(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            གལ་སྲིད( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                གལ་སྲིད (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            གལ་སྲིད( c =='<' || c == '\0' )
            {
                ཧྲིལ tag_type = 0;
                ཧྲིལ elem_type = FileNode::NONE;

                གལ་སྲིད( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                གལ་སྲིད( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                གལ་སྲིད( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                གལ་སྲིད( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    གལ་སྲིད( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    དེ་མིན གལ་སྲིད( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    དེ་མིན གལ་སྲིད( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    དེ་མིན གལ་སྲིད( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                གལ་སྲིད (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                དེ་མིན
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    གལ་སྲིད (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                གལ་སྲིད( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            དེ་མིན
            {
                གལ་སྲིད( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                གལ་སྲིད( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                གལ་སྲིད( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    སྐབས( cv_isdigit(*endptr) )
                        endptr++;
                    གལ་སྲིད( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    དེ་མིན
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    གལ་སྲིད( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                དེ་མིན
                {
                    // string
                    ཧྲིལ i = 0, len, is_quoted = 0;
                    གལ་སྲིད( c == '\"' )
                        is_quoted = 1;
                    དེ་མིན
                        --ptr;
                    strbuf[0] = '\0';

                    དོན་ལུ( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        གལ་སྲིད( !cv_isalnum(c) )
                        {
                            གལ་སྲིད( c == '\"' )
                            {
                                གལ་སྲིད( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            དེ་མིན གལ་སྲིད( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                གལ་སྲིད( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            དེ་མིན གལ་སྲིད( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            དེ་མིན གལ་སྲིད( c == '&' )
                            {
                                གལ་སྲིད( *++ptr == '#' )
                                {
                                    ཧྲིལ val, base = 10;
                                    ptr++;
                                    གལ་སྲིད( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ཧྲིལ)strtol( ptr, &endptr, base );
                                    གལ་སྲིད( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                དེ་མིན
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    སྐབས( cv_isalnum(c) );
                                    གལ་སྲིད( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ཧྲིལ)(endptr - ptr);
                                    གལ་སྲིད( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    དེ་མིན གལ་སྲིད( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    དེ་མིན གལ་སྲིད( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    དེ་མིན གལ་སྲིད( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    དེ་མིན གལ་སྲིད( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    དེ་མིན
                                    {
                                        གལ་སྲིད (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        གལ་སྲིད (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                གལ་སྲིད( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ལོག ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, ཧྲིལ& tag_type )
    {
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        གལ་སྲིད( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        གལ་སྲིད( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        གལ་སྲིད( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        དེ་མིན གལ་སྲིད( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        དེ་མིན གལ་སྲིད( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        དེ་མིན གལ་སྲིད( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        དེ་མིན
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        དོན་ལུ(;;)
        {
            var_na c, *endptr;
            གལ་སྲིད( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            སྐབས( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            གལ་སྲིད( tag_name.empty() )
                tag_name = attrname;
            དེ་མིན
            {
                གལ་སྲིད( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                གལ་སྲིད( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    གལ་སྲིད (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    གལ་སྲིད( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                གལ་སྲིད( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    གལ་སྲིད(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    གལ་སྲིད( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                དོན་ལུ(;;)
                {
                    c = *endptr++;
                    གལ་སྲིད( c == quote )
                        bha_nga;
                    གལ་སྲིད( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                གལ་སྲིད( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            གལ་སྲིད( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                གལ་སྲིད (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            གལ་སྲིད( c == '>' )
            {
                གལ་སྲིད( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            དེ་མིན གལ་སྲིད( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                གལ་སྲིད( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            དེ་མིན གལ་སྲིད( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            གལ་སྲིད( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ལོག ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        ཧྲིལ tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        གལ་སྲིད (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        གལ་སྲིད( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        སྐབས( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            གལ་སྲིད (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            གལ་སྲིད( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                གལ་སྲིད( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                གལ་སྲིད( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ལོག ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ལོག makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ལོག makePtr<XMLParser>(fs);
}

}
