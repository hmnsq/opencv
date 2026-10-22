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

    void writeTag( const var_na* key, ကိန်းပြည့် tag_type, const std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        var_na* ptr = fs->bufferPtr();
        ကိန်းပြည့် i, len = 0;
        FStructData& current_struct = fs->getCurrentStruct();
        ကိန်းပြည့် struct_flags = current_struct.flags;

        အကယ်၍( key && key[0] == '\0' )
            key = 0;

        အကယ်၍( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            အကယ်၍( FileNode::isCollection(struct_flags) )
            {
                အကယ်၍( FileNode::isMap(struct_flags) ^ (key != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            မဟုတ်ပါက
            {
                struct_flags = FileNode::EMPTY + (key ? FileNode::MAP : FileNode::SEQ);
                //fs->is_first = 0;
            }

            အကယ်၍( !FileNode::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        အကယ်၍( !key )
            key = "_";
        မဟုတ်ပါက အကယ်၍( key[0] == '_' && key[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (ကိန်းပြည့်)strlen( key );
        *ptr++ = '<';
        အကယ်၍( tag_type == CV_XML_CLOSING_TAG )
        {
            အကယ်၍( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        အကယ်၍( !cv_isalpha(key[0]) && key[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resizeWriteBuffer( ptr, len );
        အတွက်( i = 0; i < len; i++ )
        {
            var_na c = key[i];
            အကယ်၍( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        ကိန်းပြည့် nattr = (ကိန်းပြည့်)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        အတွက်( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resizeWriteBuffer( ptr, (ကိန်းပြည့်)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            အကယ်၍( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        အကယ်၍( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->setBufferPtr(ptr);
        current_struct.flags = struct_flags & ~FileNode::EMPTY;
    }

    FStructData startWriteStruct(const FStructData& parent, const var_na* key,
                                 ကိန်းပြည့် struct_flags, const var_na* type_name=0)
    {
        std::vector<std::string> attrlist;
        အကယ်၍( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( key, CV_XML_OPENING_TAG, attrlist );

        FStructData current_struct;
        current_struct.tag = key ? std::string(key) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ပြန်ပေး current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    void write(const var_na* key, ကိန်းပြည့် value)
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
        ကိန်းပြည့် i, len;

        အကယ်၍( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ကိန်းပြည့်)strlen(str);
        အကယ်၍( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        အကယ်၍( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            bool need_quote = quote || len == 0;
            data = buf;
            *data++ = '\"';
            အတွက်( i = 0; i < len; i++ )
            {
                var_na c = str[i];

                အကယ်၍( (uchar)c >= 128 || c == ' ' )
                {
                    *data++ = c;
                    need_quote = true;
                }
                မဟုတ်ပါက အကယ်၍( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *data++ = '&';
                    အကယ်၍( c == '<' )
                    {
                        memcpy(data, "lt", 2);
                        data += 2;
                    }
                    မဟုတ်ပါက အကယ်၍( c == '>' )
                    {
                        memcpy(data, "gt", 2);
                        data += 2;
                    }
                    မဟုတ်ပါက အကယ်၍( c == '&' )
                    {
                        memcpy(data, "amp", 3);
                        data += 3;
                    }
                    မဟုတ်ပါက အကယ်၍( c == '\'' )
                    {
                        memcpy(data, "apos", 4);
                        data += 4;
                    }
                    မဟုတ်ပါက အကယ်၍( c == '\"' )
                    {
                        memcpy( data, "quot", 4);
                        data += 4;
                    }
                    မဟုတ်ပါက
                    {
                        snprintf( data, parimaa_na(buf) - (data - buf), "#x%02x", (uchar)c );
                        data += 4;
                    }
                    *data++ = ';';
                    need_quote = 1;
                }
                မဟုတ်ပါက
                    *data++ = c;
            }
            အကယ်၍( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = true;

            အကယ်၍( need_quote )
                *data++ = '\"';
            len = (ကိန်းပြည့်)(data - buf) - !need_quote;
            *data++ = '\0';
            data = buf + (ကိန်းပြည့်)!need_quote;
        }

        writeScalar( key, data );
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

        ကိန်းပြည့် len = (ကိန်းပြည့်)strlen(data);
        အကယ်၍( key && *key == '\0' )
            key = 0;

        FStructData& current_struct = fs->getCurrentStruct();
        ကိန်းပြည့် struct_flags = current_struct.flags;

        အကယ်၍( FileNode::isMap(struct_flags) ||
           (!FileNode::isCollection(struct_flags) && key) )
        {
            writeTag( key, CV_XML_OPENING_TAG );
            var_na* ptr = fs->resizeWriteBuffer( fs->bufferPtr(), len );
            memcpy( ptr, data, len );
            fs->setBufferPtr( ptr + len );
            writeTag( key, CV_XML_CLOSING_TAG );
        }
        မဟုတ်ပါက
        {
            var_na* ptr = fs->bufferPtr();
            ကိန်းပြည့် new_offset = (ကိန်းပြည့်)(ptr - fs->bufferStart()) + len;

            အကယ်၍( key )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = FileNode::SEQ;

            အကယ်၍( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->bufferStart() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            မဟုတ်ပါက အကယ်၍( ptr > fs->bufferStart() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, data, len );
            fs->setBufferPtr(ptr + len);
        }
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        FStructData& current_struct = fs->getCurrentStruct();
        ကိန်းပြည့် len;
        ကိန်းပြည့် multiline;
        const var_na* eol;
        var_na* ptr;

        အကယ်၍( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        အကယ်၍( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (ကိန်းပြည့်)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->bufferPtr();

        အကယ်၍( multiline || !eol_comment || fs->bufferEnd() - ptr < len + 5 )
            ptr = fs->flush();
        မဟုတ်ပါက အကယ်၍( ptr > fs->bufferStart() + current_struct.indent )
            *ptr++ = ' ';

        အကယ်၍( !multiline )
        {
            ptr = fs->resizeWriteBuffer( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (ကိန်းပြည့်)strlen(ptr);
        }
        မဟုတ်ပါက
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->setBufferPtr(ptr + len);
        ptr = fs->flush();

        အကယ်၍( multiline )
        {
            နေစဉ်( comment )
            {
                အကယ်၍( eol )
                {
                    ptr = fs->resizeWriteBuffer( ptr, (ကိန်းပြည့်)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                မဟုတ်ပါက
                {
                    len = (ကိန်းပြည့်)strlen(comment);
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

    var_na* skipSpaces( var_na* ptr, ကိန်းပြည့် mode )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ကိန်းပြည့် level = 0;

        အတွက်(;;)
        {
            var_na c;
            ptr--;

            အကယ်၍( mode == CV_XML_INSIDE_COMMENT )
            {
                do c = *++ptr;
                နေစဉ်( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                အကယ်၍( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            မဟုတ်ပါက အကယ်၍( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                do
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    အကယ်၍( level < 0 )
                        ပြန်ပေး ptr;
                } နေစဉ်( cv_isprint_or_tab(c) );
            }
            မဟုတ်ပါက
            {
                do c = *++ptr;
                နေစဉ်( c == ' ' || c == '\t' );

                အကယ်၍( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    အကယ်၍( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                မဟုတ်ပါက အကယ်၍( cv_isprint(c) )
                    bha_nga;
            }

            အကယ်၍( !cv_isprint(*ptr) )
            {
                အကယ်၍( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                အကယ်၍( !ptr || *ptr == '\0' )
                    bha_nga;
            }
        }
        ပြန်ပေး ptr;
    }

    bool getBase64Row(var_na* ptr, ကိန်းပြည့် /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        အကယ်၍( !ptr || !*ptr )
            ပြန်ပေး false;

        // closing XML tag
        အကယ်၍ ( *beg == '<' )
            ပြန်ပေး false;

        // find end of the row
        နေစဉ်( cv_isprint(*ptr) )
            ++ptr;
        အကယ်၍ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ပြန်ပေး true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node, ကိန်းပြည့် depth = 0 )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        အကယ်၍( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        FileNode new_elem;
        bool have_space = true;
        ကိန်းပြည့် value_type = node.type();
        std::string key, key2, type_name;

        အတွက်(;;)
        {
            var_na c = *ptr, d;
            var_na* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            အကယ်၍( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                အကယ်၍ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = true;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            အကယ်၍( c =='<' || c == '\0' )
            {
                ကိန်းပြည့် tag_type = 0;
                ကိန်းပြည့် elem_type = FileNode::NONE;

                အကယ်၍( d == '/' || c == '\0' )
                    bha_nga;

                ptr = parseTag( ptr, key, type_name, tag_type );

                အကယ်၍( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                အကယ်၍( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                bool binary_string = false;

                အကယ်၍( !type_name.empty() )
                {
                    const var_na* tn = type_name.c_str();
                    အကယ်၍( strcmp(tn, "str") == 0 )
                        elem_type = FileNode::STRING;
                    မဟုတ်ပါက အကယ်၍( strcmp( tn, "map" ) == 0 )
                        elem_type = FileNode::MAP;
                    မဟုတ်ပါက အကယ်၍( strcmp( tn, "seq" ) == 0 )
                        elem_type = FileNode::SEQ;
                    မဟုတ်ပါက အကယ်၍( strcmp( tn, "binary") == 0)
                        binary_string = true;
                }

                new_elem = fs->addNode(node, key, elem_type, 0);
                အကယ်၍ (!binary_string)
                    ptr = parseValue(ptr, new_elem, depth + 1);
                မဟုတ်ပါက
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    အကယ်၍ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                အကယ်၍( tag_type != CV_XML_CLOSING_TAG || key2 != key )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = true;
            }
            မဟုတ်ပါက
            {
                အကယ်၍( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                FileNode* elem = &node;
                အကယ်၍( node.type() != FileNode::NONE )
                {
                    fs->convertToCollection( FileNode::SEQ, node );
                    new_elem = fs->addNode(node, std::string(), FileNode::NONE, 0);
                    elem = &new_elem;
                }

                အကယ်၍( value_type != FileNode::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    နေစဉ်( cv_isdigit(*endptr) )
                        endptr++;
                    အကယ်၍( *endptr == '.' || *endptr == 'e' )
                    {
                        dviguu_na fval = fs->strtod( ptr, &endptr );
                        elem->setValue(FileNode::REAL, &fval);
                    }
                    မဟုတ်ပါက
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->setValue(FileNode::INT, &ival);
                    }

                    အကယ်၍( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                }
                မဟုတ်ပါက
                {
                    // string
                    ကိန်းပြည့် i = 0, len, is_quoted = 0;
                    အကယ်၍( c == '\"' )
                        is_quoted = 1;
                    မဟုတ်ပါက
                        --ptr;
                    strbuf[0] = '\0';

                    အတွက်( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                        အကယ်၍( !cv_isalnum(c) )
                        {
                            အကယ်၍( c == '\"' )
                            {
                                အကယ်၍( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                bha_nga;
                            }
                            မဟုတ်ပါက အကယ်၍( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                အကယ်၍( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                bha_nga;
                            }
                            မဟုတ်ပါက အကယ်၍( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            မဟုတ်ပါက အကယ်၍( c == '&' )
                            {
                                အကယ်၍( *++ptr == '#' )
                                {
                                    ကိန်းပြည့် val, base = 10;
                                    ptr++;
                                    အကယ်၍( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (ကိန်းပြည့်)strtol( ptr, &endptr, base );
                                    အကယ်၍( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (var_na)val;
                                }
                                မဟုတ်ပါက
                                {
                                    endptr = ptr;
                                    do c = *++endptr;
                                    နေစဉ်( cv_isalnum(c) );
                                    အကယ်၍( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (ကိန်းပြည့်)(endptr - ptr);
                                    အကယ်၍( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    မဟုတ်ပါက အကယ်၍( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    မဟုတ်ပါက အကယ်၍( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    မဟုတ်ပါက အကယ်၍( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    မဟုတ်ပါက အကယ်၍( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    မဟုတ်ပါက
                                    {
                                        အကယ်၍ (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                            }
                        }
                        အကယ်၍ (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->setValue(FileNode::STRING, strbuf, i);
                }

                အကယ်၍( value_type != FileNode::NONE && value_type != FileNode::SEQ && value_type != FileNode::MAP )
                    bha_nga;
                have_space = false;
            }
        }
        fs->finalizeCollection(node);

        ပြန်ပေး ptr;
    }

    var_na* parseTag( var_na* ptr, std::string& tag_name,
                    std::string& type_name, ကိန်းပြည့်& tag_type )
    {
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        အကယ်၍( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        အကယ်၍( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

        အကယ်၍( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        မဟုတ်ပါက အကယ်၍( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        မဟုတ်ပါက အကယ်၍( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        မဟုတ်ပါက အကယ်၍( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        မဟုတ်ပါက
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        အတွက်(;;)
        {
            var_na c, *endptr;
            အကယ်၍( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            do c = *++endptr;
            နေစဉ်( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

            အကယ်၍( tag_name.empty() )
                tag_name = attrname;
            မဟုတ်ပါက
            {
                အကယ်၍( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                အကယ်၍( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    အကယ်၍ (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    အကယ်၍( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                အကယ်၍( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    အကယ်၍(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    အကယ်၍( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                var_na quote = *ptr++;
                endptr = ptr;
                အတွက်(;;)
                {
                    c = *endptr++;
                    အကယ်၍( c == quote )
                        bha_nga;
                    အကယ်၍( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                အကယ်၍( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            bool have_space = cv_isspace(c) || c == '\0';

            အကယ်၍( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                အကယ်၍ (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            အကယ်၍( c == '>' )
            {
                အကယ်၍( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                bha_nga;
            }
            မဟုတ်ပါက အကယ်၍( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                အကယ်၍( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                bha_nga;
            }
            မဟုတ်ပါက အကယ်၍( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                bha_nga;
            }

            အကယ်၍( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ပြန်ပေး ptr;
    }

    bool parse(var_na* ptr)
    {
        CV_Assert( fs != 0 );

        std::string key, key2, type_name;
        ကိန်းပြည့် tag_type = 0;
        bool ok = false;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        အကယ်၍ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        အကယ်၍( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, key, type_name, tag_type );
        FileNode root_collection(fs->getFS(), 0, 0);

        နေစဉ်( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            အကယ်၍ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            အကယ်၍( *ptr != '\0' )
            {
                ptr = parseTag( ptr, key, type_name, tag_type );
                အကယ်၍( tag_type != CV_XML_OPENING_TAG || key != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                FileNode root = fs->addNode(root_collection, std::string(), FileNode::MAP, 0);
                ptr = parseValue( ptr, root );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                အကယ်၍( tag_type != CV_XML_CLOSING_TAG || key != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = true;
            }
        }
        CV_Assert( fs->eof() );
        ပြန်ပေး ok;
    }

    FileStorage_API* fs;
    var_na strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ပြန်ပေး makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ပြန်ပေး makePtr<XMLParser>(fs);
}

}
