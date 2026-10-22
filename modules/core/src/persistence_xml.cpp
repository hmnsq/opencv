// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence.hpp"

গণনা
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

নামাকাশ cv
{

শ্রেণী XMLEmitter : সর্বজনীন FileStorageEmitter
{
সর্বজনীন:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    আভাসী ~XMLEmitter() {}

    শূন্য writeTag( ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা tag_type, ধ্রুবক std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        অক্ষর* ptr = fs->বাফার_Ptr();
        পূর্ণসংখ্যা i, len = 0;
        FStruct_তথ্য& current_struct = fs->getCurrentStruct();
        পূর্ণসংখ্যা struct_flags = current_struct.flags;

        যদি( চাবি && চাবি[0] == '\0' )
            চাবি = 0;

        যদি( tag_type == CV_XML_OPENING_TAG || tag_type == CV_XML_EMPTY_TAG )
        {
            যদি( File_গ্রন্থি::isCollection(struct_flags) )
            {
                যদি( File_গ্রন্থি::isMap(struct_flags) ^ (চাবি != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            অন্যথা
            {
                struct_flags = File_গ্রন্থি::EMPTY + (চাবি ? File_গ্রন্থি::MAP : File_গ্রন্থি::SEQ);
                //fs->is_first = 0;
            }

            যদি( !File_গ্রন্থি::isEmptyCollection(struct_flags) )
                ptr = fs->flush();
        }

        যদি( !চাবি )
            চাবি = "_";
        অন্যথা যদি( চাবি[0] == '_' && চাবি[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (পূর্ণসংখ্যা)strlen( চাবি );
        *ptr++ = '<';
        যদি( tag_type == CV_XML_CLOSING_TAG )
        {
            যদি( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        যদি( !cv_isalpha(চাবি[0]) && চাবি[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_বাফার( ptr, len );
        জন্য( i = 0; i < len; i++ )
        {
            অক্ষর c = চাবি[i];
            যদি( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        পূর্ণসংখ্যা nattr = (পূর্ণসংখ্যা)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        জন্য( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_বাফার( ptr, (পূর্ণসংখ্যা)(len0 + len1 + 4) );
            *ptr++ = ' ';

            memcpy( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            যদি( len1 > 0 )
                memcpy( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        যদি( tag_type == CV_XML_EMPTY_TAG )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->set_বাফার_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_গ্রন্থি::EMPTY;
    }

    FStruct_তথ্য startWriteStruct(ধ্রুবক FStruct_তথ্য& parent, ধ্রুবক অক্ষর* চাবি,
                                 পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর* type_name=0)
    {
        std::vector<std::string> attrlist;
        যদি( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        writeTag( চাবি, CV_XML_OPENING_TAG, attrlist );

        FStruct_তথ্য current_struct;
        current_struct.tag = চাবি ? std::string(চাবি) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = parent.indent + CV_XML_INDENT;

        ফেরত current_struct;
    }

    শূন্য endWriteStruct(ধ্রুবক FStruct_তথ্য& current_struct)
    {
        writeTag( current_struct.tag.c_str(), CV_XML_CLOSING_TAG );
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা value)
    {
        অক্ষর buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( চাবি, ptr);
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, int64_t value)
    {
        অক্ষর buf[128], *ptr = fs::itoa( value, buf, 10, সত্য );
        writeScalar( চাবি, ptr);
    }

    শূন্য লিখ( ধ্রুবক অক্ষর* চাবি, দ্বিগুণ value )
    {
        অক্ষর buf[128];
        writeScalar( চাবি, fs::doubleToString( buf, আকার(buf), value, মিথ্যা ) );
    }

    শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* str, যৌক্তিক quote)
    {
        অক্ষর buf[CV_FS_MAX_LEN*6+16];
        অক্ষর* তথ্য = (অক্ষর*)str;
        পূর্ণসংখ্যা i, len;

        যদি( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (পূর্ণসংখ্যা)strlen(str);
        যদি( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        যদি( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            যৌক্তিক need_quote = quote || len == 0;
            তথ্য = buf;
            *তথ্য++ = '\"';
            জন্য( i = 0; i < len; i++ )
            {
                অক্ষর c = str[i];

                যদি( (uchar)c >= 128 || c == ' ' )
                {
                    *তথ্য++ = c;
                    need_quote = সত্য;
                }
                অন্যথা যদি( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *তথ্য++ = '&';
                    যদি( c == '<' )
                    {
                        memcpy(তথ্য, "lt", 2);
                        তথ্য += 2;
                    }
                    অন্যথা যদি( c == '>' )
                    {
                        memcpy(তথ্য, "gt", 2);
                        তথ্য += 2;
                    }
                    অন্যথা যদি( c == '&' )
                    {
                        memcpy(তথ্য, "amp", 3);
                        তথ্য += 3;
                    }
                    অন্যথা যদি( c == '\'' )
                    {
                        memcpy(তথ্য, "apos", 4);
                        তথ্য += 4;
                    }
                    অন্যথা যদি( c == '\"' )
                    {
                        memcpy( তথ্য, "quot", 4);
                        তথ্য += 4;
                    }
                    অন্যথা
                    {
                        snprintf( তথ্য, আকার(buf) - (তথ্য - buf), "#x%02x", (uchar)c );
                        তথ্য += 4;
                    }
                    *তথ্য++ = ';';
                    need_quote = 1;
                }
                অন্যথা
                    *তথ্য++ = c;
            }
            যদি( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = সত্য;

            যদি( need_quote )
                *তথ্য++ = '\"';
            len = (পূর্ণসংখ্যা)(তথ্য - buf) - !need_quote;
            *তথ্য++ = '\0';
            তথ্য = buf + (পূর্ণসংখ্যা)!need_quote;
        }

        writeScalar( চাবি, তথ্য );
    }

    শূন্য writeScalar(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* তথ্য)
    {
        fs->check_if_write_struct_is_delayed(মিথ্যা);
        যদি ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        অন্যথা যদি ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)strlen(তথ্য);
        যদি( চাবি && *চাবি == '\0' )
            চাবি = 0;

        FStruct_তথ্য& current_struct = fs->getCurrentStruct();
        পূর্ণসংখ্যা struct_flags = current_struct.flags;

        যদি( File_গ্রন্থি::isMap(struct_flags) ||
           (!File_গ্রন্থি::isCollection(struct_flags) && চাবি) )
        {
            writeTag( চাবি, CV_XML_OPENING_TAG );
            অক্ষর* ptr = fs->resize_Write_বাফার( fs->বাফার_Ptr(), len );
            memcpy( ptr, তথ্য, len );
            fs->set_বাফার_Ptr( ptr + len );
            writeTag( চাবি, CV_XML_CLOSING_TAG );
        }
        অন্যথা
        {
            অক্ষর* ptr = fs->বাফার_Ptr();
            পূর্ণসংখ্যা new_offset = (পূর্ণসংখ্যা)(ptr - fs->বাফার_Start()) + len;

            যদি( চাবি )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_গ্রন্থি::SEQ;

            যদি( (new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10) ||
               (ptr > fs->বাফার_Start() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            অন্যথা যদি( ptr > fs->বাফার_Start() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            memcpy( ptr, তথ্য, len );
            fs->set_বাফার_Ptr(ptr + len);
        }
    }

    শূন্য writeComment(ধ্রুবক অক্ষর* comment, যৌক্তিক eol_comment)
    {
        FStruct_তথ্য& current_struct = fs->getCurrentStruct();
        পূর্ণসংখ্যা len;
        পূর্ণসংখ্যা multiline;
        ধ্রুবক অক্ষর* eol;
        অক্ষর* ptr;

        যদি( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        যদি( strstr(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (পূর্ণসংখ্যা)strlen(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->বাফার_Ptr();

        যদি( multiline || !eol_comment || fs->বাফার_End() - ptr < len + 5 )
            ptr = fs->flush();
        অন্যথা যদি( ptr > fs->বাফার_Start() + current_struct.indent )
            *ptr++ = ' ';

        যদি( !multiline )
        {
            ptr = fs->resize_Write_বাফার( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (পূর্ণসংখ্যা)strlen(ptr);
        }
        অন্যথা
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->set_বাফার_Ptr(ptr + len);
        ptr = fs->flush();

        যদি( multiline )
        {
            যখন( comment )
            {
                যদি( eol )
                {
                    ptr = fs->resize_Write_বাফার( ptr, (পূর্ণসংখ্যা)(eol - comment) + 1 );
                    memcpy( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                অন্যথা
                {
                    len = (পূর্ণসংখ্যা)strlen(comment);
                    ptr = fs->resize_Write_বাফার( ptr, len );
                    memcpy( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->set_বাফার_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->set_বাফার_Ptr(ptr + 3);
            fs->flush();
        }
    }

    শূন্য start_পরবর্তী_Stream()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

সুরক্ষিত:
    FileStorage_API* fs;
};

শ্রেণী XMLParser : সর্বজনীন FileStorageParser
{
সর্বজনীন:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    আভাসী ~XMLParser() {}

    অক্ষর* skipSpaces( অক্ষর* ptr, পূর্ণসংখ্যা mode )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        পূর্ণসংখ্যা level = 0;

        জন্য(;;)
        {
            অক্ষর c;
            ptr--;

            যদি( mode == CV_XML_INSIDE_COMMENT )
            {
                করো c = *++ptr;
                যখন( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                যদি( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    mode = 0;
                    ptr += 3;
                }
            }
            অন্যথা যদি( mode == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                করো
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    যদি( level < 0 )
                        ফেরত ptr;
                } যখন( cv_isprint_or_tab(c) );
            }
            অন্যথা
            {
                করো c = *++ptr;
                যখন( c == ' ' || c == '\t' );

                যদি( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    যদি( mode != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    mode = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                অন্যথা যদি( cv_isprint(c) )
                    বিরতি;
            }

            যদি( !cv_isprint(*ptr) )
            {
                যদি( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                যদি( !ptr || *ptr == '\0' )
                    বিরতি;
            }
        }
        ফেরত ptr;
    }

    যৌক্তিক getBase64Row(অক্ষর* ptr, পূর্ণসংখ্যা /*indent*/, অক্ষর* &beg, অক্ষর* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_TAG);
        যদি( !ptr || !*ptr )
            ফেরত মিথ্যা;

        // closing XML tag
        যদি ( *beg == '<' )
            ফেরত মিথ্যা;

        // find end of the row
        যখন( cv_isprint(*ptr) )
            ++ptr;
        যদি ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ফেরত সত্য;
    }

    অক্ষর* parse_মান( অক্ষর* ptr, File_গ্রন্থি& গ্রন্থি, পূর্ণসংখ্যা depth = 0 )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_গ্রন্থি new_elem;
        যৌক্তিক have_space = সত্য;
        পূর্ণসংখ্যা value_type = গ্রন্থি.type();
        std::string চাবি, key2, type_name;

        জন্য(;;)
        {
            অক্ষর c = *ptr, d;
            অক্ষর* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            যদি( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                যদি (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = সত্য;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            যদি( c =='<' || c == '\0' )
            {
                পূর্ণসংখ্যা tag_type = 0;
                পূর্ণসংখ্যা elem_type = File_গ্রন্থি::NONE;

                যদি( d == '/' || c == '\0' )
                    বিরতি;

                ptr = parseTag( ptr, চাবি, type_name, tag_type );

                যদি( tag_type == CV_XML_DIRECTIVE_TAG )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                যদি( tag_type == CV_XML_EMPTY_TAG )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(tag_type == CV_XML_OPENING_TAG);

                /* for base64 string */
                যৌক্তিক দ্বিমিক_string = মিথ্যা;

                যদি( !type_name.empty() )
                {
                    ধ্রুবক অক্ষর* tn = type_name.c_str();
                    যদি( strcmp(tn, "str") == 0 )
                        elem_type = File_গ্রন্থি::STRING;
                    অন্যথা যদি( strcmp( tn, "map" ) == 0 )
                        elem_type = File_গ্রন্থি::MAP;
                    অন্যথা যদি( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_গ্রন্থি::SEQ;
                    অন্যথা যদি( strcmp( tn, "binary") == 0)
                        দ্বিমিক_string = সত্য;
                }

                new_elem = fs->add_গ্রন্থি(গ্রন্থি, চাবি, elem_type, 0);
                যদি (!দ্বিমিক_string)
                    ptr = parse_মান(ptr, new_elem, depth + 1);
                অন্যথা
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    যদি (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parseTag( ptr, key2, type_name, tag_type );
                যদি( tag_type != CV_XML_CLOSING_TAG || key2 != চাবি )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = সত্য;
            }
            অন্যথা
            {
                যদি( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_গ্রন্থি* elem = &গ্রন্থি;
                যদি( গ্রন্থি.type() != File_গ্রন্থি::NONE )
                {
                    fs->convertToCollection( File_গ্রন্থি::SEQ, গ্রন্থি );
                    new_elem = fs->add_গ্রন্থি(গ্রন্থি, std::string(), File_গ্রন্থি::NONE, 0);
                    elem = &new_elem;
                }

                যদি( value_type != File_গ্রন্থি::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    যখন( cv_isdigit(*endptr) )
                        endptr++;
                    যদি( *endptr == '.' || *endptr == 'e' )
                    {
                        দ্বিগুণ fval = fs->strtod( ptr, &endptr );
                        elem->set_মান(File_গ্রন্থি::REAL, &fval);
                    }
                    অন্যথা
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->set_মান(File_গ্রন্থি::INT, &ival);
                    }

                    যদি( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
                }
                অন্যথা
                {
                    // string
                    পূর্ণসংখ্যা i = 0, len, is_quoted = 0;
                    যদি( c == '\"' )
                        is_quoted = 1;
                    অন্যথা
                        --ptr;
                    strbuf[0] = '\0';

                    জন্য( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();

                        যদি( !cv_isalnum(c) )
                        {
                            যদি( c == '\"' )
                            {
                                যদি( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                বিরতি;
                            }
                            অন্যথা যদি( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                যদি( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                বিরতি;
                            }
                            অন্যথা যদি( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            অন্যথা যদি( c == '&' )
                            {
                                যদি( *++ptr == '#' )
                                {
                                    পূর্ণসংখ্যা val, base = 10;
                                    ptr++;
                                    যদি( *ptr == 'x' )
                                    {
                                        base = 16;
                                        ptr++;
                                    }
                                    val = (পূর্ণসংখ্যা)strtol( ptr, &endptr, base );
                                    যদি( (unsigned)val > (unsigned)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (অক্ষর)val;
                                }
                                অন্যথা
                                {
                                    endptr = ptr;
                                    করো c = *++endptr;
                                    যখন( cv_isalnum(c) );
                                    যদি( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (পূর্ণসংখ্যা)(endptr - ptr);
                                    যদি( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    অন্যথা যদি( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    অন্যথা যদি( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    অন্যথা যদি( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    অন্যথা যদি( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    অন্যথা
                                    {
                                        যদি (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        memcpy( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();
                            }
                        }
                        যদি (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->set_মান(File_গ্রন্থি::STRING, strbuf, i);
                }

                যদি( value_type != File_গ্রন্থি::NONE && value_type != File_গ্রন্থি::SEQ && value_type != File_গ্রন্থি::MAP )
                    বিরতি;
                have_space = মিথ্যা;
            }
        }
        fs->finalizeCollection(গ্রন্থি);

        ফেরত ptr;
    }

    অক্ষর* parseTag( অক্ষর* ptr, std::string& tag_name,
                    std::string& type_name, পূর্ণসংখ্যা& tag_type )
    {
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        যদি( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        যদি( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();

        যদি( cv_isalnum(*ptr) || *ptr == '_' )
            tag_type = CV_XML_OPENING_TAG;
        অন্যথা যদি( *ptr == '/' )
        {
            tag_type = CV_XML_CLOSING_TAG;
            ptr++;
        }
        অন্যথা যদি( *ptr == '?' )
        {
            tag_type = CV_XML_HEADER_TAG;
            ptr++;
        }
        অন্যথা যদি( *ptr == '!' )
        {
            tag_type = CV_XML_DIRECTIVE_TAG;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        অন্যথা
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        tag_name.clear();
        type_name.clear();

        জন্য(;;)
        {
            অক্ষর c, *endptr;
            যদি( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            করো c = *++endptr;
            যখন( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP();

            যদি( tag_name.empty() )
                tag_name = attrname;
            অন্যথা
            {
                যদি( tag_type == CV_XML_CLOSING_TAG )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                যদি( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    যদি (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    যদি( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                যদি( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                    যদি(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    যদি( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                অক্ষর quote = *ptr++;
                endptr = ptr;
                জন্য(;;)
                {
                    c = *endptr++;
                    যদি( c == quote )
                        বিরতি;
                    যদি( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                যদি( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            যৌক্তিক have_space = cv_isspace(c) || c == '\0';

            যদি( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
                যদি (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            যদি( c == '>' )
            {
                যদি( tag_type == CV_XML_HEADER_TAG )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                বিরতি;
            }
            অন্যথা যদি( c == '?' && tag_type == CV_XML_HEADER_TAG )
            {
                যদি( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                বিরতি;
            }
            অন্যথা যদি( c == '/' && ptr[1] == '>' && tag_type == CV_XML_OPENING_TAG )  // FIXIT ptr[1] - out of bounds read without check
            {
                tag_type = CV_XML_EMPTY_TAG;
                ptr += 2;
                বিরতি;
            }

            যদি( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        ফেরত ptr;
    }

    যৌক্তিক parse(অক্ষর* ptr)
    {
        CV_Assert( fs != 0 );

        std::string চাবি, key2, type_name;
        পূর্ণসংখ্যা tag_type = 0;
        যৌক্তিক ok = মিথ্যা;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_TAG );
        যদি (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        যদি( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parseTag( ptr, চাবি, type_name, tag_type );
        File_গ্রন্থি মূল_collection(fs->getFS(), 0, 0);

        যখন( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            যদি (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            যদি( *ptr != '\0' )
            {
                ptr = parseTag( ptr, চাবি, type_name, tag_type );
                যদি( tag_type != CV_XML_OPENING_TAG || চাবি != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_গ্রন্থি মূল = fs->add_গ্রন্থি(মূল_collection, std::string(), File_গ্রন্থি::MAP, 0);
                ptr = parse_মান( ptr, মূল );
                ptr = parseTag( ptr, key2, type_name, tag_type );
                যদি( tag_type != CV_XML_CLOSING_TAG || চাবি != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = সত্য;
            }
        }
        CV_Assert( fs->eof() );
        ফেরত ok;
    }

    FileStorage_API* fs;
    অক্ষর strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    ফেরত makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    ফেরত makePtr<XMLParser>(fs);
}

}
