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

    void writeTag( const var_na* key, ចំនួនគត់ tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        ចំនួនគត់ i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ចំនួនគត់ struct_flags = current_struct.flags;

        ប្រសិនបើ( key && key[0] == '\0' )
            key = 0;

        ប្រសិនបើ( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            ប្រសិនបើ( FileNode::isCollection(struct_flags) )
            {
                ប្រសិនបើ( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            បើមិនដូច្នេះ
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            ប្រសិនបើ( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        ប្រសិនបើ( !key )
            key = "_";
        បើមិនដូច្នេះ ប្រសិនបើ( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ចំនួនគត់)strlen( key );
        *ptr++ = '<';
        ប្រសិនបើ( tag_type == CV_XML_CLOSING_TAG )
        {
            ប្រសិនបើ( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        ប្រសិនបើ( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        សម្រាប់( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            ប្រសិនបើ( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ចំនួនគត់ nattr = (ចំនួនគត់)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        សម្រាប់( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ចំនួនគត់)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            ប្រសិនបើ( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        ប្រសិនបើ( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 ចំនួនគត់ struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        ប្រសិនបើ( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ត្រឡប់ current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, ចំនួនគត់ value)
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
        ចំនួនគត់ i, len;

        ប្រសិនបើ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ចំនួនគត់)strlen(str);
        ប្រសិនបើ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ប្រសិនបើ( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            សម្រាប់( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                ប្រសិនបើ( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                បើមិនដូច្នេះ ប្រសិនបើ( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    ប្រសិនបើ( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    បើមិនដូច្នេះ
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                បើមិនដូច្នេះ
                    *data++ = c;
            }
            ប្រសិនបើ( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            ប្រសិនបើ( need_quote )
                *data++ = '\"';
            len = (ចំនួនគត់)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ចំនួនគត់)!need_quote;
        }

        writeScalar( key, data );
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        fs->check_if_write_struct_is_delayed(false);
        ប្រសិនបើ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        បើមិនដូច្នេះ ប្រសិនបើ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        ចំនួនគត់ len = (ចំនួនគត់)strlen(data);
        ប្រសិនបើ( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ចំនួនគត់ struct_flags = current_struct.flags;

        ប្រសិនបើ( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        បើមិនដូច្នេះ
        {
            var_na* ptr = fs->bufferPtr();
            ចំនួនគត់ new_offset = (ចំនួនគត់)(ptr - fs->bufferStart()) + len;

            ប្រសិនបើ( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            ប្រសិនបើ( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            បើមិនដូច្នេះ ប្រសិនបើ( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ចំនួនគត់ len;
        ចំនួនគត់ multiline;
        const var_na* eol;
        var_na* ptr;

        ប្រសិនបើ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ប្រសិនបើ( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ចំនួនគត់)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        ប្រសិនបើ( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        បើមិនដូច្នេះ ប្រសិនបើ( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        ប្រសិនបើ( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ចំនួនគត់)strlen(ptr);
        }
        បើមិនដូច្នេះ
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        ប្រសិនបើ( multiline )
        {
            ខណៈពេល( comment )
            {
                ប្រសិនបើ( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ចំនួនគត់)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                បើមិនដូច្នេះ
                {
                    len = (ចំនួនគត់)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ចំនួនគត់ mode )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ចំនួនគត់ level = 0;

        សម្រាប់(;;)
        {
            var_na c;
            ptr--;

            ប្រសិនបើ( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                ខណៈពេល( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                ប្រសិនបើ( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            បើមិនដូច្នេះ ប្រសិនបើ( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    ប្រសិនបើ( level < 0 )
                        ត្រឡប់ ptr;
                } ខណៈពេល( cv_isprint_or_tab(c) );
            }
            បើមិនដូច្នេះ
            {
                do c = *++ptr;
                ខណៈពេល( c == ' ' || c == '\t' );

                ប្រសិនបើ( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    ប្រសិនបើ( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                បើមិនដូច្នេះ ប្រសិនបើ( cv_isprint(c) )
                    bha_nga;
            }

            ប្រសិនបើ( !cv_isprint(*ptr) )
            {
                ប្រសិនបើ( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                ប្រសិនបើ( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ត្រឡប់ ptr;
    }

    bool getBase64Row(var_na* ptr, ចំនួនគត់ /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        ប្រសិនបើ( !ptr || !*ptr )
            ត្រឡប់ false;

        // closing XML tag
        ប្រសិនបើ ( *beg == '<' )
            ត្រឡប់ false;

        // find end of the row
        ខណៈពេល( cv_isprint(*ptr) )
            ++ptr;
        ប្រសិនបើ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ត្រឡប់ true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ចំនួនគត់ depth = 0 )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ប្រសិនបើ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        ចំនួនគត់ value_type = node.type();
        std::string key, key2, type_name;

        សម្រាប់(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            ប្រសិនបើ( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                ប្រសិនបើ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            ប្រសិនបើ( c =='<' || c == '\0' )
            {
                ចំនួនគត់ tag_type = 0;
                ចំនួនគត់ elem_type = FileNode::NONE;

                ប្រសិនបើ( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                ប្រសិនបើ( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                ប្រសិនបើ( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                ប្រសិនបើ( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    ប្រសិនបើ( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    បើមិនដូច្នេះ ប្រសិនបើ( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    បើមិនដូច្នេះ ប្រសិនបើ( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    បើមិនដូច្នេះ ប្រសិនបើ( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                ប្រសិនបើ (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                បើមិនដូច្នេះ
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    ប្រសិនបើ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                ប្រសិនបើ( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            បើមិនដូច្នេះ
            {
                ប្រសិនបើ( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                ប្រសិនបើ( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                ប្រសិនបើ( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    ខណៈពេល( cv_isdigit(*endptr) )
                        endptr++;
                    ប្រសិនបើ( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    បើមិនដូច្នេះ
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    ប្រសិនបើ( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                បើមិនដូច្នេះ
                {
                    // string
                    ចំនួនគត់ i = 0, len, is_quoted = 0;
                    ប្រសិនបើ( c == '\"' )
                        is_quoted = 1;
                    បើមិនដូច្នេះ
                        --ptr;
                    strbuf[0] = '\0';

                    សម្រាប់( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        ប្រសិនបើ( !cv_isalnum(c) )
                        {
                            ប្រសិនបើ( c == '\"' )
                            {
                                ប្រសិនបើ( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            បើមិនដូច្នេះ ប្រសិនបើ( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                ប្រសិនបើ( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            បើមិនដូច្នេះ ប្រសិនបើ( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            បើមិនដូច្នេះ ប្រសិនបើ( c == '&' )
                            {
                                ប្រសិនបើ( *++ptr == '#' )
                                {
                                    ចំនួនគត់ val, base = 10;
                                    ptr++;
                                    ប្រសិនបើ( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ចំនួនគត់)strtol( ptr, &endptr, base );
                                    ប្រសិនបើ( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                បើមិនដូច្នេះ
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    ខណៈពេល( cv_isalnum(c) );
                                    ប្រសិនបើ( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ចំនួនគត់)(endptr - ptr);
                                    ប្រសិនបើ( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    បើមិនដូច្នេះ ប្រសិនបើ( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    បើមិនដូច្នេះ ប្រសិនបើ( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    បើមិនដូច្នេះ ប្រសិនបើ( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    បើមិនដូច្នេះ ប្រសិនបើ( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    បើមិនដូច្នេះ
                                    {
                                        ប្រសិនបើ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        ប្រសិនបើ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                ប្រសិនបើ( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ត្រឡប់ ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, ចំនួនគត់& tag_type )
    {
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        ប្រសិនបើ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        ប្រសិនបើ( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        ប្រសិនបើ( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        បើមិនដូច្នេះ ប្រសិនបើ( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        បើមិនដូច្នេះ
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        សម្រាប់(;;)
        {
            var_na c, *endptr;
            ប្រសិនបើ( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            ខណៈពេល( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            ប្រសិនបើ( tag_name.empty() )
                tag_name = attrname;
            បើមិនដូច្នេះ
            {
                ប្រសិនបើ( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                ប្រសិនបើ( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ប្រសិនបើ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    ប្រសិនបើ( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                ប្រសិនបើ( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    ប្រសិនបើ(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    ប្រសិនបើ( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                សម្រាប់(;;)
                {
                    c = *endptr++;
                    ប្រសិនបើ( c == quote )
                        bha_nga;
                    ប្រសិនបើ( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                ប្រសិនបើ( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            ប្រសិនបើ( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                ប្រសិនបើ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            ប្រសិនបើ( c == '>' )
            {
                ប្រសិនបើ( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            បើមិនដូច្នេះ ប្រសិនបើ( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                ប្រសិនបើ( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            បើមិនដូច្នេះ ប្រសិនបើ( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            ប្រសិនបើ( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ត្រឡប់ ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        ចំនួនគត់ tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        ប្រសិនបើ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ប្រសិនបើ( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        ខណៈពេល( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            ប្រសិនបើ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            ប្រសិនបើ( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                ប្រសិនបើ( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                ប្រសិនបើ( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ត្រឡប់ ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ត្រឡប់ makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ត្រឡប់ makePtr<XMLParser>(fs);
}

}
