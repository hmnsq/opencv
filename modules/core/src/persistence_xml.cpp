// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "persistence.hpp"

تعداد
{
    CV_XML_INDENT = 2,
    CV_XML_INSIDE_COMMENT = 1,
    CV_XML_INSIDE_برچسب = 2,
    CV_XML_INSIDE_DIRECTIVE = 3,
    CV_XML_OPENING_برچسب = 1,
    CV_XML_CLOSING_برچسب = 2,
    CV_XML_خالی_برچسب = 3,
    CV_XML_سرآیند_برچسب = 4,
    CV_XML_DIRECTIVE_برچسب = 5
};

نطاق cv
{

صنف XMLEmitter : عام FileStorageEmitter
{
عام:
    XMLEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    مجازی ~XMLEmitter() {}

    فراغ write_برچسب( ثابت حرف* کلید, صحيح برچسب_type, ثابت std::vector<std::string>& attrlist=std::vector<std::string>() )
    {
        حرف* ptr = fs->میانگیر_Ptr();
        صحيح i, len = 0;
        FStruct_داده& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( کلید && کلید[0] == '\0' )
            کلید = 0;

        إذا( برچسب_type == CV_XML_OPENING_برچسب || برچسب_type == CV_XML_خالی_برچسب )
        {
            إذا( File_گره::isCollection(struct_flags) )
            {
                إذا( File_گره::isMap(struct_flags) ^ (کلید != 0) )
                    CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                             "or add element with key to sequence" );
            }
            وإلا
            {
                struct_flags = File_گره::خالی_2 + (کلید ? File_گره::MAP : File_گره::SEQ);
                //fs->is_first = 0;
            }

            إذا( !File_گره::is_خالی_Collection(struct_flags) )
                ptr = fs->flush();
        }

        إذا( !کلید )
            کلید = "_";
        وإلا إذا( کلید[0] == '_' && کلید[1] == '\0' )
            CV_Error( cv::Error::StsBadArg, "A single _ is a reserved tag name" );

        len = (صحيح)طول_رشته( کلید );
        *ptr++ = '<';
        إذا( برچسب_type == CV_XML_CLOSING_برچسب )
        {
            إذا( !attrlist.empty() )
                CV_Error( cv::Error::StsBadArg, "Closing tag should not include any attributes" );
            *ptr++ = '/';
        }

        إذا( !cv_isalpha(کلید[0]) && کلید[0] != '_' )
            CV_Error( cv::Error::StsBadArg, "Key should start with a letter or _" );

        ptr = fs->resize_Write_میانگیر( ptr, len );
        لكل( i = 0; i < len; i++ )
        {
            حرف c = کلید[i];
            إذا( !cv_isalnum(c) && c != '_' && c != '-' )
                CV_Error( cv::Error::StsBadArg, "Key name may only contain alphanumeric characters [a-zA-Z0-9], '-' and '_'" );
            ptr[i] = c;
        }
        ptr += len;

        صحيح nattr = (صحيح)attrlist.size();
        CV_Assert( nattr % 2 == 0 );

        لكل( i = 0; i < nattr; i += 2 )
        {
            size_t len0 = attrlist[i].size();
            size_t len1 = attrlist[i+1].size();
            CV_Assert( len0 > 0 );

            ptr = fs->resize_Write_میانگیر( ptr, (صحيح)(len0 + len1 + 4) );
            *ptr++ = ' ';

            کپی_حافظه( ptr, attrlist[i].c_str(), len0 );
            ptr += len0;
            *ptr++ = '=';
            *ptr++ = '\"';
            إذا( len1 > 0 )
                کپی_حافظه( ptr, attrlist[i+1].c_str(), len1 );
            ptr += len1;
            *ptr++ = '\"';
        }

        إذا( برچسب_type == CV_XML_خالی_برچسب )
            *ptr++ = '/';
        *ptr++ = '>';
        fs->مجموعه_میانگیر_Ptr(ptr);
        current_struct.flags = struct_flags & ~File_گره::خالی_2;
    }

    FStruct_داده شروع_Write_Struct(ثابت FStruct_داده& والد, ثابت حرف* کلید,
                                 صحيح struct_flags, ثابت حرف* type_name=0)
    {
        std::vector<std::string> attrlist;
        إذا( type_name && *type_name )
        {
            attrlist.push_back("type_id");
            attrlist.push_back(type_name);
        }

        write_برچسب( کلید, CV_XML_OPENING_برچسب, attrlist );

        FStruct_داده current_struct;
        current_struct.برچسب = کلید ? std::string(کلید) : std::string();
        current_struct.flags = struct_flags;
        current_struct.indent = والد.indent + CV_XML_INDENT;

        أرجع current_struct;
    }

    فراغ انتها_Write_Struct(ثابت FStruct_داده& current_struct)
    {
        write_برچسب( current_struct.برچسب.c_str(), CV_XML_CLOSING_برچسب );
    }

    فراغ اكتب(ثابت حرف* کلید, صحيح value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10 );
        writeScalar( کلید, ptr);
    }

    فراغ اكتب(ثابت حرف* کلید, int64_t value)
    {
        حرف buf[128], *ptr = fs::itoa( value, buf, 10, درست );
        writeScalar( کلید, ptr);
    }

    فراغ اكتب( ثابت حرف* کلید, مزدوج value )
    {
        حرف buf[128];
        writeScalar( کلید, fs::double_مقصد_String( buf, حجم(buf), value, خطأ ) );
    }

    فراغ اكتب(ثابت حرف* کلید, ثابت حرف* str, منطقي quote)
    {
        حرف buf[CV_FS_MAX_LEN*6+16];
        حرف* داده = (حرف*)str;
        صحيح i, len;

        إذا( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (صحيح)طول_رشته(str);
        إذا( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        إذا( quote || len == 0 || str[0] != '\"' || str[0] != str[len-1] )
        {
            منطقي need_quote = quote || len == 0;
            داده = buf;
            *داده++ = '\"';
            لكل( i = 0; i < len; i++ )
            {
                حرف c = str[i];

                إذا( (uchar)c >= 128 || c == ' ' )
                {
                    *داده++ = c;
                    need_quote = درست;
                }
                وإلا إذا( !cv_isprint(c) || c == '<' || c == '>' || c == '&' || c == '\'' || c == '\"' )
                {
                    *داده++ = '&';
                    إذا( c == '<' )
                    {
                        کپی_حافظه(داده, "lt", 2);
                        داده += 2;
                    }
                    وإلا إذا( c == '>' )
                    {
                        کپی_حافظه(داده, "gt", 2);
                        داده += 2;
                    }
                    وإلا إذا( c == '&' )
                    {
                        کپی_حافظه(داده, "amp", 3);
                        داده += 3;
                    }
                    وإلا إذا( c == '\'' )
                    {
                        کپی_حافظه(داده, "apos", 4);
                        داده += 4;
                    }
                    وإلا إذا( c == '\"' )
                    {
                        کپی_حافظه( داده, "quot", 4);
                        داده += 4;
                    }
                    وإلا
                    {
                        snprintf( داده, حجم(buf) - (داده - buf), "#x%02x", (uchar)c );
                        داده += 4;
                    }
                    *داده++ = ';';
                    need_quote = 1;
                }
                وإلا
                    *داده++ = c;
            }
            إذا( !need_quote && (cv_isdigit(str[0]) ||
                                str[0] == '+' || str[0] == '-' || str[0] == '.' ))
                need_quote = درست;

            إذا( need_quote )
                *داده++ = '\"';
            len = (صحيح)(داده - buf) - !need_quote;
            *داده++ = '\0';
            داده = buf + (صحيح)!need_quote;
        }

        writeScalar( کلید, داده );
    }

    فراغ writeScalar(ثابت حرف* کلید, ثابت حرف* داده)
    {
        fs->check_if_write_struct_is_delayed(خطأ);
        إذا ( fs->get_state_از_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_مقصد_Base64_state( FileStorage_API::NotUse );
        }
        وإلا إذا ( fs->get_state_از_writing_base64() == FileStorage_API::درون_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        صحيح len = (صحيح)طول_رشته(داده);
        إذا( کلید && *کلید == '\0' )
            کلید = 0;

        FStruct_داده& current_struct = fs->getCurrentStruct();
        صحيح struct_flags = current_struct.flags;

        إذا( File_گره::isMap(struct_flags) ||
           (!File_گره::isCollection(struct_flags) && کلید) )
        {
            write_برچسب( کلید, CV_XML_OPENING_برچسب );
            حرف* ptr = fs->resize_Write_میانگیر( fs->میانگیر_Ptr(), len );
            کپی_حافظه( ptr, داده, len );
            fs->مجموعه_میانگیر_Ptr( ptr + len );
            write_برچسب( کلید, CV_XML_CLOSING_برچسب );
        }
        وإلا
        {
            حرف* ptr = fs->میانگیر_Ptr();
            صحيح new_جابه_جایی = (صحيح)(ptr - fs->میانگیر_شروع()) + len;

            إذا( کلید )
                CV_Error( cv::Error::StsBadArg, "elements with keys can not be written to sequence" );

            current_struct.flags = File_گره::SEQ;

            إذا( (new_جابه_جایی > fs->wrapMargin() && new_جابه_جایی - current_struct.indent > 10) ||
               (ptr > fs->میانگیر_شروع() && ptr[-1] == '>') )
            {
                ptr = fs->flush();
            }
            وإلا إذا( ptr > fs->میانگیر_شروع() + current_struct.indent && ptr[-1] != '>' )
                *ptr++ = ' ';

            کپی_حافظه( ptr, داده, len );
            fs->مجموعه_میانگیر_Ptr(ptr + len);
        }
    }

    فراغ writeComment(ثابت حرف* comment, منطقي eol_comment)
    {
        FStruct_داده& current_struct = fs->getCurrentStruct();
        صحيح len;
        صحيح multiline;
        ثابت حرف* eol;
        حرف* ptr;

        إذا( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        إذا( جستجوی_رشته(comment, "--") != 0 )
            CV_Error( cv::Error::StsBadArg, "Double hyphen \'--\' is not allowed in the comments" );

        len = (صحيح)طول_رشته(comment);
        eol = strchr(comment, '\n');
        multiline = eol != 0;
        ptr = fs->میانگیر_Ptr();

        إذا( multiline || !eol_comment || fs->میانگیر_انتها_2() - ptr < len + 5 )
            ptr = fs->flush();
        وإلا إذا( ptr > fs->میانگیر_شروع() + current_struct.indent )
            *ptr++ = ' ';

        إذا( !multiline )
        {
            ptr = fs->resize_Write_میانگیر( ptr, len + 5+4+1 );
            snprintf( ptr, len + 5+4+1, "<!-- %s -->", comment );
            len = (صحيح)طول_رشته(ptr);
        }
        وإلا
        {
            strcpy( ptr, "<!--" );
            len = 4;
        }

        fs->مجموعه_میانگیر_Ptr(ptr + len);
        ptr = fs->flush();

        إذا( multiline )
        {
            طالما( comment )
            {
                إذا( eol )
                {
                    ptr = fs->resize_Write_میانگیر( ptr, (صحيح)(eol - comment) + 1 );
                    کپی_حافظه( ptr, comment, eol - comment + 1 );
                    ptr += eol - comment;
                    comment = eol + 1;
                    eol = strchr( comment, '\n' );
                }
                وإلا
                {
                    len = (صحيح)طول_رشته(comment);
                    ptr = fs->resize_Write_میانگیر( ptr, len );
                    کپی_حافظه( ptr, comment, len );
                    ptr += len;
                    comment = 0;
                }
                fs->مجموعه_میانگیر_Ptr(ptr);
                ptr = fs->flush();
            }
            strcpy( ptr, "-->" );
            fs->مجموعه_میانگیر_Ptr(ptr + 3);
            fs->flush();
        }
    }

    فراغ شروع_بعدی_جریان()
    {
        fs->puts( "\n<!-- next stream -->\n" );
    }

محمي:
    FileStorage_API* fs;
};

صنف XMLParser : عام FileStorageParser
{
عام:
    XMLParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    مجازی ~XMLParser() {}

    حرف* skipSpaces( حرف* ptr, صحيح نما )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        صحيح level = 0;

        لكل(;;)
        {
            حرف c;
            ptr--;

            إذا( نما == CV_XML_INSIDE_COMMENT )
            {
                افعل c = *++ptr;
                طالما( cv_isprint_or_tab(c) && (c != '-' || ptr[1] != '-' || ptr[2] != '>') );

                إذا( c == '-' )
                {
                    CV_Assert( ptr[1] == '-' && ptr[2] == '>' );
                    نما = 0;
                    ptr += 3;
                }
            }
            وإلا إذا( نما == CV_XML_INSIDE_DIRECTIVE )
            {
                // !!!NOTE!!! This is not quite correct, but should work in most cases
                افعل
                {
                    c = *++ptr;
                    level += c == '<';
                    level -= c == '>';
                    إذا( level < 0 )
                        أرجع ptr;
                } طالما( cv_isprint_or_tab(c) );
            }
            وإلا
            {
                افعل c = *++ptr;
                طالما( c == ' ' || c == '\t' );

                إذا( c == '<' && ptr[1] == '!' && ptr[2] == '-' && ptr[3] == '-' )
                {
                    إذا( نما != 0 )
                        CV_PARSE_ERROR_CPP( "Comments are not allowed here" );
                    نما = CV_XML_INSIDE_COMMENT;
                    ptr += 4;
                }
                وإلا إذا( cv_isprint(c) )
                    اكسر;
            }

            إذا( !cv_isprint(*ptr) )
            {
                إذا( *ptr != '\0' && *ptr != '\n' && *ptr != '\r' )
                    CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                ptr = fs->gets();
                إذا( !ptr || *ptr == '\0' )
                    اكسر;
            }
        }
        أرجع ptr;
    }

    منطقي get_Base64_ردیف(حرف* ptr, صحيح /*indent*/, حرف* &beg, حرف* &end)
    {
        beg = end = ptr = skipSpaces(ptr, CV_XML_INSIDE_برچسب);
        إذا( !ptr || !*ptr )
            أرجع خطأ;

        // closing XML tag
        إذا ( *beg == '<' )
            أرجع خطأ;

        // find end of the row
        طالما( cv_isprint(*ptr) )
            ++ptr;
        إذا ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        أرجع درست;
    }

    حرف* parse_مقدار( حرف* ptr, File_گره& گره, صحيح عمق = 0 )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( عمق > CV_PERSISTENCE_MAX_عمق )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        File_گره new_elem;
        منطقي have_space = درست;
        صحيح value_type = گره.type();
        std::string کلید, key2, type_name;

        لكل(;;)
        {
            حرف c = *ptr, d;
            حرف* endptr;

            // FIXIT ptr[1], ptr[2] - out of bounds read without check or data fetch (#11061)
            إذا( cv_isspace(c) || c == '\0' ||
                (c == '<' && ptr[1] == '!' && ptr[2] == '-') )
            {
                ptr = skipSpaces( ptr, 0 );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                have_space = درست;
                c = *ptr;
            }

            d = ptr[1];  // FIXIT ptr[1] - out of bounds read without check or data fetch (#11061)

            إذا( c =='<' || c == '\0' )
            {
                صحيح برچسب_type = 0;
                صحيح elem_type = File_گره::NONE;

                إذا( d == '/' || c == '\0' )
                    اكسر;

                ptr = parse_برچسب( ptr, کلید, type_name, برچسب_type );

                إذا( برچسب_type == CV_XML_DIRECTIVE_برچسب )
                    CV_PARSE_ERROR_CPP( "Directive tags are not allowed here" );
                إذا( برچسب_type == CV_XML_خالی_برچسب )
                    CV_PARSE_ERROR_CPP( "Empty tags are not supported" );

                CV_Assert(برچسب_type == CV_XML_OPENING_برچسب);

                /* for base64 string */
                منطقي دودویی_string = خطأ;

                إذا( !type_name.empty() )
                {
                    ثابت حرف* tn = type_name.c_str();
                    إذا( strcmp(tn, "str") == 0 )
                        elem_type = File_گره::STRING;
                    وإلا إذا( strcmp( tn, "map" ) == 0 )
                        elem_type = File_گره::MAP;
                    وإلا إذا( strcmp( tn, "seq" ) == 0 )
                        elem_type = File_گره::SEQ;
                    وإلا إذا( strcmp( tn, "binary") == 0)
                        دودویی_string = درست;
                }

                new_elem = fs->جمع_کن_گره(گره, کلید, elem_type, 0);
                إذا (!دودویی_string)
                    ptr = parse_مقدار(ptr, new_elem, عمق + 1);
                وإلا
                {
                    ptr = fs->parseBase64( ptr, 0, new_elem);
                    ptr = skipSpaces( ptr, 0 );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid input");
                }

                ptr = parse_برچسب( ptr, key2, type_name, برچسب_type );
                إذا( برچسب_type != CV_XML_CLOSING_برچسب || key2 != کلید )
                    CV_PARSE_ERROR_CPP( "Mismatched closing tag" );
                have_space = درست;
            }
            وإلا
            {
                إذا( !have_space )
                    CV_PARSE_ERROR_CPP( "There should be space between literals" );

                File_گره* elem = &گره;
                إذا( گره.type() != File_گره::NONE )
                {
                    fs->convert_مقصد_Collection( File_گره::SEQ, گره );
                    new_elem = fs->جمع_کن_گره(گره, std::string(), File_گره::NONE, 0);
                    elem = &new_elem;
                }

                إذا( value_type != File_گره::STRING &&
                   (cv_isdigit(c) || ((c == '-' || c == '+') &&
                   (cv_isdigit(d) || d == '.')) || (c == '.' && cv_isalnum(d))) ) // a number
                {
                    endptr = ptr + (c == '-' || c == '+');
                    طالما( cv_isdigit(*endptr) )
                        endptr++;
                    إذا( *endptr == '.' || *endptr == 'e' )
                    {
                        مزدوج fval = fs->strtod( ptr, &endptr );
                        elem->مجموعه_مقدار(File_گره::REAL, &fval);
                    }
                    وإلا
                    {
                        int64_t ival = strtoll( ptr, &endptr, 0 );
                        elem->مجموعه_مقدار(File_گره::INT, &ival);
                    }

                    إذا( endptr == ptr )
                        CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );

                    ptr = endptr;
                    CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP();
                }
                وإلا
                {
                    // string
                    صحيح i = 0, len, is_quoted = 0;
                    إذا( c == '\"' )
                        is_quoted = 1;
                    وإلا
                        --ptr;
                    strbuf[0] = '\0';

                    لكل( ;; )
                    {
                        c = *++ptr;
                        CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP();

                        إذا( !cv_isalnum(c) )
                        {
                            إذا( c == '\"' )
                            {
                                إذا( !is_quoted )
                                    CV_PARSE_ERROR_CPP( "Literal \" is not allowed within a string. Use &quot;" );
                                ++ptr;
                                اكسر;
                            }
                            وإلا إذا( !cv_isprint(c) || c == '<' || (!is_quoted && cv_isspace(c)))
                            {
                                إذا( is_quoted )
                                    CV_PARSE_ERROR_CPP( "Closing \" is expected" );
                                اكسر;
                            }
                            وإلا إذا( c == '\'' || c == '>' )
                            {
                                CV_PARSE_ERROR_CPP( "Literal \' or > are not allowed. Use &apos; or &gt;" );
                            }
                            وإلا إذا( c == '&' )
                            {
                                إذا( *++ptr == '#' )
                                {
                                    صحيح val, پایه = 10;
                                    ptr++;
                                    إذا( *ptr == 'x' )
                                    {
                                        پایه = 16;
                                        ptr++;
                                    }
                                    val = (صحيح)strtol( ptr, &endptr, پایه );
                                    إذا( (بیعلامت)val > (بیعلامت)255 ||
                                       !endptr || *endptr != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid numeric value in the string" );
                                    c = (حرف)val;
                                }
                                وإلا
                                {
                                    endptr = ptr;
                                    افعل c = *++endptr;
                                    طالما( cv_isalnum(c) );
                                    إذا( c != ';' )
                                        CV_PARSE_ERROR_CPP( "Invalid character in the symbol entity name" );
                                    len = (صحيح)(endptr - ptr);
                                    إذا( len == 2 && memcmp( ptr, "lt", len ) == 0 )
                                        c = '<';
                                    وإلا إذا( len == 2 && memcmp( ptr, "gt", len ) == 0 )
                                        c = '>';
                                    وإلا إذا( len == 3 && memcmp( ptr, "amp", len ) == 0 )
                                        c = '&';
                                    وإلا إذا( len == 4 && memcmp( ptr, "apos", len ) == 0 )
                                        c = '\'';
                                    وإلا إذا( len == 4 && memcmp( ptr, "quot", len ) == 0 )
                                        c = '\"';
                                    وإلا
                                    {
                                        إذا (len + 2 + i >= CV_FS_MAX_LEN)
                                            CV_PARSE_ERROR_CPP("string is too long");
                                        کپی_حافظه( strbuf + i, ptr-1, len + 2 );
                                        i += len + 2;
                                    }
                                }
                                ptr = endptr;
                                CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP();
                            }
                        }
                        إذا (i + 1 >= CV_FS_MAX_LEN)
                            CV_PARSE_ERROR_CPP("Too long string literal");
                        strbuf[i++] = c;
                    }
                    elem->مجموعه_مقدار(File_گره::STRING, strbuf, i);
                }

                إذا( value_type != File_گره::NONE && value_type != File_گره::SEQ && value_type != File_گره::MAP )
                    اكسر;
                have_space = خطأ;
            }
        }
        fs->finalizeCollection(گره);

        أرجع ptr;
    }

    حرف* parse_برچسب( حرف* ptr, std::string& برچسب_name,
                    std::string& type_name, صحيح& برچسب_type )
    {
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid tag input");

        إذا( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of the stream" );

        إذا( *ptr != '<' )
            CV_PARSE_ERROR_CPP( "Tag should start with \'<\'" );

        ptr++;
        CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP();

        إذا( cv_isalnum(*ptr) || *ptr == '_' )
            برچسب_type = CV_XML_OPENING_برچسب;
        وإلا إذا( *ptr == '/' )
        {
            برچسب_type = CV_XML_CLOSING_برچسب;
            ptr++;
        }
        وإلا إذا( *ptr == '?' )
        {
            برچسب_type = CV_XML_سرآیند_برچسب;
            ptr++;
        }
        وإلا إذا( *ptr == '!' )
        {
            برچسب_type = CV_XML_DIRECTIVE_برچسب;
            CV_Assert( ptr[1] != '-' || ptr[2] != '-' );
            ptr++;
        }
        وإلا
            CV_PARSE_ERROR_CPP( "Unknown tag type" );

        برچسب_name.clear();
        type_name.clear();

        لكل(;;)
        {
            حرف c, *endptr;
            إذا( !cv_isalpha(*ptr) && *ptr != '_' )
                CV_PARSE_ERROR_CPP( "Name should start with a letter or underscore" );

            endptr = ptr - 1;
            افعل c = *++endptr;
            طالما( cv_isalnum(c) || c == '_' || c == '-' );

            std::string attrname(ptr, (size_t)(endptr - ptr));
            ptr = endptr;
            CV_PERSISTENCE_CHECK_انتها_از_میانگیر_BUG_CPP();

            إذا( برچسب_name.empty() )
                برچسب_name = attrname;
            وإلا
            {
                إذا( برچسب_type == CV_XML_CLOSING_برچسب )
                    CV_PARSE_ERROR_CPP( "Closing tag should not contain any attributes" );

                إذا( *ptr != '=' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_برچسب );
                    إذا (!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute");
                    إذا( *ptr != '=' )
                        CV_PARSE_ERROR_CPP( "Attribute name should be followed by \'=\'" );
                }

                c = *++ptr;
                إذا( c != '\"' && c != '\'' )
                {
                    ptr = skipSpaces( ptr, CV_XML_INSIDE_برچسب );
                    إذا(!ptr)
                        CV_PARSE_ERROR_CPP("Invalid attribute value");
                    إذا( *ptr != '\"' && *ptr != '\'' )
                        CV_PARSE_ERROR_CPP( "Attribute value should be put into single or double quotes" );
                }

                حرف quote = *ptr++;
                endptr = ptr;
                لكل(;;)
                {
                    c = *endptr++;
                    إذا( c == quote )
                        اكسر;
                    إذا( c == '\0' )
                        CV_PARSE_ERROR_CPP( "Unexpected end of line" );
                }

                إذا( attrname == "type_id" )
                {
                    CV_Assert( type_name.empty() );
                    type_name = std::string(ptr, (size_t)(endptr - 1 - ptr));
                }

                ptr = endptr;
            }

            c = *ptr;
            منطقي have_space = cv_isspace(c) || c == '\0';

            إذا( c != '>' )
            {
                ptr = skipSpaces( ptr, CV_XML_INSIDE_برچسب );
                إذا (!ptr)
                    CV_PARSE_ERROR_CPP("Invalid input");
                c = *ptr;
            }

            إذا( c == '>' )
            {
                إذا( برچسب_type == CV_XML_سرآیند_برچسب )
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr++;
                اكسر;
            }
            وإلا إذا( c == '?' && برچسب_type == CV_XML_سرآیند_برچسب )
            {
                إذا( ptr[1] != '>'  )  // FIXIT ptr[1] - out of bounds read without check
                    CV_PARSE_ERROR_CPP( "Invalid closing tag for <?xml ..." );
                ptr += 2;
                اكسر;
            }
            وإلا إذا( c == '/' && ptr[1] == '>' && برچسب_type == CV_XML_OPENING_برچسب )  // FIXIT ptr[1] - out of bounds read without check
            {
                برچسب_type = CV_XML_خالی_برچسب;
                ptr += 2;
                اكسر;
            }

            إذا( !have_space )
                CV_PARSE_ERROR_CPP( "There should be space between attributes" );
        }

        أرجع ptr;
    }

    منطقي parse(حرف* ptr)
    {
        CV_Assert( fs != 0 );

        std::string کلید, key2, type_name;
        صحيح برچسب_type = 0;
        منطقي ok = خطأ;

        // CV_XML_INSIDE_TAG is used to prohibit leading comments
        ptr = skipSpaces( ptr, CV_XML_INSIDE_برچسب );
        إذا (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        إذا( memcmp( ptr, "<?xml", 5 ) != 0 )  // FIXIT ptr[1..] - out of bounds read without check
            CV_PARSE_ERROR_CPP( "Valid XML should start with \'<?xml ...?>\'" );

        ptr = parse_برچسب( ptr, کلید, type_name, برچسب_type );
        File_گره ریشه_collection(fs->getFS(), 0, 0);

        طالما( ptr && *ptr != '\0' )
        {
            ptr = skipSpaces( ptr, 0 );
            إذا (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");

            إذا( *ptr != '\0' )
            {
                ptr = parse_برچسب( ptr, کلید, type_name, برچسب_type );
                إذا( برچسب_type != CV_XML_OPENING_برچسب || کلید != "opencv_storage" )
                    CV_PARSE_ERROR_CPP( "<opencv_storage> tag is missing" );
                File_گره ریشه = fs->جمع_کن_گره(ریشه_collection, std::string(), File_گره::MAP, 0);
                ptr = parse_مقدار( ptr, ریشه );
                ptr = parse_برچسب( ptr, key2, type_name, برچسب_type );
                إذا( برچسب_type != CV_XML_CLOSING_برچسب || کلید != key2 )
                    CV_PARSE_ERROR_CPP( "</opencv_storage> tag is missing" );
                ptr = skipSpaces( ptr, 0 );
                ok = درست;
            }
        }
        CV_Assert( fs->eof() );
        أرجع ok;
    }

    FileStorage_API* fs;
    حرف strbuf[CV_FS_MAX_LEN+16];
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs)
{
    أرجع makePtr<XMLEmitter>(fs);
}

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs)
{
    أرجع makePtr<XMLParser>(fs);
}

}
