// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ JSONEmitter : ಸಾರ್ವಜನಿಕ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    ವಾಸ್ತವಿಕ ~JSONEmitter() {}

    FStruct_ದತ್ತಾಂಶ ಆರಂಭ_Write_Struct( ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ,
                                  ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name=0 )
    {
        ಅಕ್ಷರ ದತ್ತಾಂಶ[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_ಮುಖವಾಡ|File_ಗಂಟು::FLOW)) | File_ಗಂಟು::ಖಾಲಿಗಳು_2;
        ಆದರೆ( !File_ಗಂಟು::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ಆದರೆ( type_name && *type_name == '\0' )
            type_name = 0;

        ತಾರ್ಕಿಕ is_real_collection = ನಿಜ;
        ಆದರೆ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = File_ಗಂಟು::STR;
            ದತ್ತಾಂಶ[0] = '\0';
            is_real_collection = ಸುಳ್ಳು;
        }

        ಆದರೆ ( is_real_collection )
        {
            ಅಕ್ಷರ c = File_ಗಂಟು::isMap(struct_flags) ? '{' : '[';
            ದತ್ತಾಂಶ[0] = c;
            ದತ್ತಾಂಶ[1] = '\0';
        }

        writeScalar( ಕೀಲಿ, ದತ್ತಾಂಶ );
        FStruct_ದತ್ತಾಂಶ current_struct("", struct_flags, ಪೋಷಕ.indent + 4);

        ಹಿಂತಿರುಗಿ current_struct;
    }

    ಶೂನ್ಯ ಕೊನೆ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& current_struct)
    {
        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;

        ಆದರೆ (File_ಗಂಟು::isCollection(struct_flags)) {
            ಆದರೆ (!File_ಗಂಟು::isFlow(struct_flags)) {
                ಆದರೆ (fs->ಬಫರ್_Ptr() <= fs->ಬಫರ್_ಆರಂಭ() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    ಅಕ್ಷರ *ptr = fs->ಬಫರ್_Ptr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->ಬಫರ್_ಆರಂಭ());
                    fs->ಗಣ_ಬಫರ್_Ptr(fs->ಬಫರ್_ಆರಂಭ());
                }
                fs->flush();
            }

            ಅಕ್ಷರ *ptr = fs->ಬಫರ್_Ptr();
            ಆದರೆ (ptr > fs->ಬಫರ್_ಆರಂಭ() + current_struct.indent && !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
            fs->ಗಣ_ಬಫರ್_Ptr(ptr);
        }
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value)
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10 ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, int64_t value)
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::itoa( value, buf, 10, ನಿಜ ));
    }

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ದ್ವಿಗುಣ value )
    {
        ಅಕ್ಷರ buf[128];
        writeScalar( ಕೀಲಿ, fs::double_ಗಮ್ಯ_String( buf, ಗಾತ್ರ(buf), value, ನಿಜ ));
    }

    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str, ತಾರ್ಕಿಕ quote)
    {
        ಅಕ್ಷರ buf[CV_FS_MAX_LEN*6+16];
        ಅಕ್ಷರ* ದತ್ತಾಂಶ = (ಅಕ್ಷರ*)str;
        ಪೂರ್ಣಾಂಕ i, len;

        ಆದರೆ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str);
        ಆದರೆ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ಆದರೆ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ಪೂರ್ಣಾಂಕ need_quote = 1;
            ದತ್ತಾಂಶ = buf;
            *ದತ್ತಾಂಶ++ = '\"';
            ಗಾಗಿ( i = 0; i < len; i++ )
            {
                ಅಕ್ಷರ c = str[i];
                ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ byte = static_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(c);

                ಆಯ್ಕೆ ( c )
                {
                ಸ್ಥಿತಿ '\\':
                ಸ್ಥಿತಿ '\"': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = c;   ನಿಲ್ಲಿಸು; }
                ಸ್ಥಿತಿ '\n': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 'n'; ನಿಲ್ಲಿಸು; }
                ಸ್ಥಿತಿ '\r': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 'r'; ನಿಲ್ಲಿಸು; }
                ಸ್ಥಿತಿ '\t': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 't'; ನಿಲ್ಲಿಸು; }
                ಸ್ಥಿತಿ '\b': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 'b'; ನಿಲ್ಲಿಸು; }
                ಸ್ಥಿತಿ '\f': { *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 'f'; ನಿಲ್ಲಿಸು; }
                ಪೂರ್ವನಿಯೋಜಿತ  :
                    ಆದರೆ (byte < 0x20)
                    {
                        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ hex[] = "0123456789abcdef";
                        *ದತ್ತಾಂಶ++ = '\\'; *ದತ್ತಾಂಶ++ = 'u'; *ದತ್ತಾಂಶ++ = '0'; *ದತ್ತಾಂಶ++ = '0';
                        *ದತ್ತಾಂಶ++ = hex[byte >> 4]; *ದತ್ತಾಂಶ++ = hex[byte & 0xf];
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        *ದತ್ತಾಂಶ++ = c;
                    ನಿಲ್ಲಿಸು;
                }
            }

            *ದತ್ತಾಂಶ++ = '\"';
            *ದತ್ತಾಂಶ++ = '\0';
            ದತ್ತಾಂಶ = buf + !need_quote;
        }

        writeScalar( ಕೀಲಿ, ದತ್ತಾಂಶ);
    }

    ಶೂನ್ಯ writeScalar(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ದತ್ತಾಂಶ)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(ಸುಳ್ಳು);
        ಆದರೆ ( fs->get_state_ದ_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_ಗಮ್ಯ_Base64_state( FileStorage_API::NotUse );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( fs->get_state_ದ_writing_base64() == FileStorage_API::ಒಳ_Use )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t ಕೀಲಿ_len = 0u;
        ಆದರೆ( ಕೀಲಿ && *ಕೀಲಿ == '\0' )
            ಕೀಲಿ = 0;
        ಆದರೆ ( ಕೀಲಿ )
        {
            ಕೀಲಿ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಕೀಲಿ);
            ಆದರೆ ( ಕೀಲಿ_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( static_cast<ಪೂರ್ಣಾಂಕ>(ಕೀಲಿ_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t ದತ್ತಾಂಶ_len = 0u;
        ಆದರೆ ( ದತ್ತಾಂಶ )
            ದತ್ತಾಂಶ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ದತ್ತಾಂಶ);

        FStruct_ದತ್ತಾಂಶ& current_struct = fs->getCurrentStruct();
        ಪೂರ್ಣಾಂಕ struct_flags = current_struct.flags;
        ಆದರೆ( File_ಗಂಟು::isCollection(struct_flags) )
        {
            ಆದರೆ ( (File_ಗಂಟು::isMap(struct_flags) ^ (ಕೀಲಿ != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ಇಲ್ಲದಿದ್ದರೆ {
            fs->ಗಣ_Non_ಖಾಲಿಗಳು();
            struct_flags = File_ಗಂಟು::ಖಾಲಿಗಳು_2 | (ಕೀಲಿ ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ);
        }

        // start to write
        ಅಕ್ಷರ* ptr = 0;

        ಆದರೆ( File_ಗಂಟು::isFlow(struct_flags) )
        {
            ಪೂರ್ಣಾಂಕ new_ಸ್ಥಾನಾಂತರ;
            ptr = fs->ಬಫರ್_Ptr();
            ಆದರೆ( !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
                *ptr++ = ',';
            new_ಸ್ಥಾನಾಂತರ = static_cast<ಪೂರ್ಣಾಂಕ>(ptr - fs->ಬಫರ್_ಆರಂಭ() + ಕೀಲಿ_len + ದತ್ತಾಂಶ_len);
            ಆದರೆ( new_ಸ್ಥಾನಾಂತರ > fs->wrapMargin() && new_ಸ್ಥಾನಾಂತರ - current_struct.indent > 10 )
            {
                fs->ಗಣ_ಬಫರ್_Ptr(ptr);
                ptr = fs->flush();
            }
            ಇಲ್ಲದಿದ್ದರೆ
                *ptr++ = ' ';
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ ( !File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(struct_flags) )
            {
                ptr = fs->ಬಫರ್_Ptr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->ಬಫರ್_ಆರಂಭ() );
                fs->ಗಣ_ಬಫರ್_Ptr(fs->ಬಫರ್_ಆರಂಭ());
            }
            ptr = fs->flush();
        }

        ಆದರೆ( ಕೀಲಿ )
        {
            ಆದರೆ( !cv_isalpha(ಕೀಲಿ[0]) && ಕೀಲಿ[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resize_Write_ಬಫರ್( ptr, static_cast<ಪೂರ್ಣಾಂಕ>(ಕೀಲಿ_len) );
            *ptr++ = '\"';

            ಗಾಗಿ( size_t i = 0u; i < ಕೀಲಿ_len; i++ )
            {
                ಅಕ್ಷರ c = ಕೀಲಿ[i];

                ptr[i] = c;
                ಆದರೆ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += ಕೀಲಿ_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ಆದರೆ( ದತ್ತಾಂಶ )
        {
            ptr = fs->resize_Write_ಬಫರ್( ptr, static_cast<ಪೂರ್ಣಾಂಕ>(ದತ್ತಾಂಶ_len) );
            ಸ್ಮರಣೆ_ನಕಲು( ptr, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_len );
            ptr += ದತ್ತಾಂಶ_len;
        }

        fs->ಗಣ_ಬಫರ್_Ptr(ptr);
        current_struct.flags &= ~File_ಗಂಟು::ಖಾಲಿಗಳು_2;
    }

    ಶೂನ್ಯ writeComment(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* comment, ತಾರ್ಕಿಕ eol_comment)
    {
        ಆದರೆ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ಪೂರ್ಣಾಂಕ len = static_cast<ಪೂರ್ಣಾಂಕ>(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment));
        ಅಕ್ಷರ* ptr = fs->ಬಫರ್_Ptr();
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* eol = strchr(comment, '\n');
        ತಾರ್ಕಿಕ multiline = eol != 0;

        ಆದರೆ( !eol_comment || multiline || fs->ಬಫರ್_ಕೊನೆ_2() - ptr < len || ptr == fs->ಬಫರ್_ಆರಂಭ() )
            ptr = fs->flush();
        ಇಲ್ಲದಿದ್ದರೆ
            *ptr++ = ' ';

        ಆಗ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ಆದರೆ( eol )
            {
                ptr = fs->resize_Write_ಬಫರ್( ptr, (ಪೂರ್ಣಾಂಕ)(eol - comment) + 1 );
                ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, eol - comment + 1 );
                fs->ಗಣ_ಬಫರ್_Ptr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(comment);
                ptr = fs->resize_Write_ಬಫರ್( ptr, len );
                ಸ್ಮರಣೆ_ನಕಲು( ptr, comment, len );
                fs->ಗಣ_ಬಫರ್_Ptr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    ಶೂನ್ಯ ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

ಸಂರಕ್ಷಿತ:
    FileStorage_API* fs;
};

ವರ್ಗ JSONParser : ಸಾರ್ವಜನಿಕ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    ವಾಸ್ತವಿಕ ~JSONParser() {}

    ಅಕ್ಷರ* skipSpaces( ಅಕ್ಷರ* ptr )
    {
        ತಾರ್ಕಿಕ is_eof = ಸುಳ್ಳು;
        ತಾರ್ಕಿಕ is_completed = ಸುಳ್ಳು;

        ಆಗ ( is_eof == ಸುಳ್ಳು && is_completed == ಸುಳ್ಳು )
        {
            ಆದರೆ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            ಆಯ್ಕೆ ( *ptr )
            {
                /* comment */
                ಸ್ಥಿತಿ '/' : {
                    ptr++;
                    ಆದರೆ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ಆದರೆ( !ptr || !*ptr ) { is_eof = ನಿಜ; ನಿಲ್ಲಿಸು; }
                    }

                    ಆದರೆ ( *ptr == '/' )
                    {
                        ಆಗ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ಆದರೆ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ಆದರೆ( !ptr || !*ptr ) { is_eof = ನಿಜ; ನಿಲ್ಲಿಸು; }
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                ptr++;
                            }
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '*' )
                    {
                        ptr++;
                        ಗಾಗಿ (;;)
                        {
                            ಆದರೆ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ಆದರೆ( !ptr || !*ptr ) { is_eof = ನಿಜ; ನಿಲ್ಲಿಸು; }
                            }
                            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '*' )
                            {
                                ptr++;
                                ಆದರೆ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ಆದರೆ( !ptr || !*ptr ) { is_eof = ನಿಜ; ನಿಲ್ಲಿಸು; }
                                }
                                ಆದರೆ ( *ptr == '/' )
                                {
                                    ptr++;
                                    ನಿಲ್ಲಿಸು;
                                }
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                ptr++;
                            }
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } ನಿಲ್ಲಿಸು;
                    /* whitespace */
                ಸ್ಥಿತಿ '\t':
                ಸ್ಥಿತಿ ' ' : {
                    ptr++;
                } ನಿಲ್ಲಿಸು;
                    /* newline || end mark */
                ಸ್ಥಿತಿ '\0':
                ಸ್ಥಿತಿ '\n':
                ಸ್ಥಿತಿ '\r': {
                    ptr = fs->gets();
                    ಆದರೆ( !ptr || !*ptr ) { is_eof = ನಿಜ; ನಿಲ್ಲಿಸು; }
                } ನಿಲ್ಲಿಸು;
                    /* other character */
                ಪೂರ್ವನಿಯೋಜಿತ: {
                    ಆದರೆ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = ನಿಜ;
                } ನಿಲ್ಲಿಸು;
            }
        }

        ಆದರೆ ( is_eof || !is_completed )
        {
            ptr = fs->ಬಫರ್_ಆರಂಭ();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->ಗಣ_Eof();
            ಆದರೆ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ಹಿಂತಿರುಗಿ ptr;
    }

    uint32_t parseUnicodeEscape(ಅಕ್ಷರ*& ptr)
    {
        uint32_t codepoint = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
            ಅಕ್ಷರ hex = *ptr;
            uint32_t digit = 0;
            ಆದರೆ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ಇಲ್ಲದಿದ್ದರೆ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ಹಿಂತಿರುಗಿ codepoint;
    }

    ಸ್ಥಿರ ಶೂನ್ಯ appendUtf8(std::string& out, uint32_t codepoint)
    {
        ಆದರೆ (codepoint < 0x80) {
            out += (ಅಕ್ಷರ)codepoint;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (codepoint < 0x800) {
            out += (ಅಕ್ಷರ)(0xC0 | (codepoint >> 6));
            out += (ಅಕ್ಷರ)(0x80 | (codepoint & 0x3F));
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (codepoint < 0x10000) {
            out += (ಅಕ್ಷರ)(0xE0 | (codepoint >> 12));
            out += (ಅಕ್ಷರ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ಅಕ್ಷರ)(0x80 | (codepoint & 0x3F));
        } ಇಲ್ಲದಿದ್ದರೆ {
            out += (ಅಕ್ಷರ)(0xF0 | (codepoint >> 18));
            out += (ಅಕ್ಷರ)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (ಅಕ್ಷರ)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (ಅಕ್ಷರ)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    ಶೂನ್ಯ parse_Unicode_Escape_ಗಮ್ಯ_Utf8(ಅಕ್ಷರ*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ಆದರೆ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ಆದರೆ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t ಕೆಳಗಿನ = parseUnicodeEscape(ptr);
            ಆದರೆ (ಕೆಳಗಿನ < 0xDC00 || ಕೆಳಗಿನ > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (ಕೆಳಗಿನ - 0xDC00u);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    ಅಕ್ಷರ* parse_ಕೀಲಿ( ಅಕ್ಷರ* ptr, File_ಗಂಟು& collection, File_ಗಂಟು& ಮೌಲ್ಯ_placeholder )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ಆದರೆ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        ಅಕ್ಷರ * beg = ptr + 1;
        std::string ಕೀಲಿ_name;
        ಮಾಡು {
            ಆದರೆ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
                ಆದರೆ (*ptr == 'u') {
                    ++ptr;
                    parse_Unicode_Escape_ಗಮ್ಯ_Utf8(ptr, ಕೀಲಿ_name);
                    ಮುಂದುವರಿ;
                }
                ಕೀಲಿ_name += *ptr;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                ಕೀಲಿ_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
        } ಆಗ( cv_isprint(*ptr) && *ptr != '"' );

        ಆದರೆ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ಆದರೆ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        ಮೌಲ್ಯ_placeholder = fs->ಕೂಡಿಸು_ಗಂಟು(collection, ಕೀಲಿ_name, File_ಗಂಟು::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ಆದರೆ( !ptr || !*ptr )
            ಹಿಂತಿರುಗಿ 0;

        ಆದರೆ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ಹಿಂತಿರುಗಿ ++ptr;
    }

    ತಾರ್ಕಿಕ get_Base64_ಅಡ್ಡಸಾಲು(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ /*indent*/, ಅಕ್ಷರ* &beg, ಅಕ್ಷರ* &end)
    {
        beg = end = ptr;
        ಆದರೆ( !ptr || !*ptr )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        // find end of the row
        ಆಗ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ಆದರೆ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಅಕ್ಷರ* parse_ಮೌಲ್ಯ( ಅಕ್ಷರ* ptr, File_ಗಂಟು& ಗಂಟು )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ಆದರೆ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ಆದರೆ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            ಅಕ್ಷರ * beg = ptr;
            size_t len = 0u;
            ಗಾಗಿ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ಆದರೆ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, ಗಂಟು);

                ಆದರೆ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ಇಲ್ಲದಿದ್ದರೆ
                    ptr++;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {   /**************** normal string ****************/
                ಪೂರ್ಣಾಂಕ i = 0, sz;

                ptr = beg;
                ತಾರ್ಕಿಕ is_matching = ಸುಳ್ಳು;
                ಆಗ ( !is_matching )
                {
                    ಆಯ್ಕೆ ( *ptr )
                    {
                        ಸ್ಥಿತಿ '\\':
                        {
                            sz = (ಪೂರ್ಣಾಂಕ)(ptr - beg);
                            ಆದರೆ( sz > 0 )
                            {
                                ಆದರೆ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ಸ್ಮರಣೆ_ನಕಲು(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ಆದರೆ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            ಆಯ್ಕೆ ( *ptr )
                            {
                            ಸ್ಥಿತಿ '\\':
                            ಸ್ಥಿತಿ '\"':
                            ಸ್ಥಿತಿ '\'': { buf[i++] = *ptr; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 'n' : { buf[i++] = '\n'; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 'r' : { buf[i++] = '\r'; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 't' : { buf[i++] = '\t'; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 'b' : { buf[i++] = '\b'; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 'f' : { buf[i++] = '\f'; ನಿಲ್ಲಿಸು; }
                            ಸ್ಥಿತಿ 'u' : {
                                ptr++;
                                std::string utf8;
                                parse_Unicode_Escape_ಗಮ್ಯ_Utf8(ptr, utf8);
                                ಆದರೆ (i + (ಪೂರ್ಣಾಂಕ)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ಸ್ಮರಣೆ_ನಕಲು(buf + i, utf8.ದತ್ತಾಂಶ(), utf8.size());
                                i += (ಪೂರ್ಣಾಂಕ)utf8.size();
                                beg = ptr;
                                ಮುಂದುವರಿ;
                            }
                            ಪೂರ್ವನಿಯೋಜಿತ  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            ನಿಲ್ಲಿಸು;
                            }
                            ptr++;
                            beg = ptr;
                            ನಿಲ್ಲಿಸು;
                        }
                        ಸ್ಥಿತಿ '\0':
                        {
                            sz = (ಪೂರ್ಣಾಂಕ)(ptr - beg);
                            ಆದರೆ( sz > 0 )
                            {
                                ಆದರೆ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ಸ್ಮರಣೆ_ನಕಲು(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ಆದರೆ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            ನಿಲ್ಲಿಸು;
                        }
                        ಸ್ಥಿತಿ '\"':
                        {
                            sz = (ಪೂರ್ಣಾಂಕ)(ptr - beg);
                            ಆದರೆ( sz > 0 )
                            {
                                ಆದರೆ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                ಸ್ಮರಣೆ_ನಕಲು(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = ನಿಜ;
                            ನಿಲ್ಲಿಸು;
                        }
                        ಸ್ಥಿತಿ '\n':
                        ಸ್ಥಿತಿ '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            ನಿಲ್ಲಿಸು;
                        }
                        ಪೂರ್ವನಿಯೋಜಿತ:
                        {
                            ptr++;
                            ನಿಲ್ಲಿಸು;
                        }
                    }
                }

                ಆದರೆ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ಇಲ್ಲದಿದ್ದರೆ
                    ptr++;

                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::STRING, buf, i);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            ಅಕ್ಷರ * beg = ptr;
            ಆದರೆ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
            }
            ಆಗ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
            }
            ಆದರೆ (*ptr == '.' || *ptr == 'e')
            {
                ದ್ವಿಗುಣ fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();

                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::REAL, &fval);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();

                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }

            ಆದರೆ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {    /**************** other data ****************/
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* beg = ptr;
            ಪೂರ್ಣಾಂಕ len = 0;
            ಗಾಗಿ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP();
            }

            ಆದರೆ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(File_ಗಂಟು::INT, &ival);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ಹಿಂತಿರುಗಿ ptr;
    }

    ಅಕ್ಷರ* parseSeq( ಅಕ್ಷರ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಂಕ ಆಳ = 0 )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ಆದರೆ( ಆಳ > CV_PERSISTENCE_MAX_ಆಳ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ಆದರೆ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ಇಲ್ಲದಿದ್ದರೆ
            ptr++;

        fs->convert_ಗಮ್ಯ_Collection(File_ಗಂಟು::SEQ, ಗಂಟು);

        ಗಾಗಿ (;;)
        {
            ptr = skipSpaces( ptr );
            ಆದರೆ( !ptr || !*ptr )
                ನಿಲ್ಲಿಸು;

            ಆದರೆ ( *ptr != ']' )
            {
                File_ಗಂಟು ಮಗು = fs->ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, std::string(), File_ಗಂಟು::NONE );

                ಆದರೆ ( *ptr == '[' )
                    ptr = parseSeq( ptr, ಮಗು, ಆಳ + 1 );
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '{' )
                    ptr = parseMap( ptr, ಮಗು, ಆಳ + 1 );
                ಇಲ್ಲದಿದ್ದರೆ
                    ptr = parse_ಮೌಲ್ಯ( ptr, ಮಗು );
            }

            ptr = skipSpaces( ptr );
            ಆದರೆ( !ptr || !*ptr )
                ನಿಲ್ಲಿಸು;

            ಆದರೆ ( *ptr == ',' )
                ptr++;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == ']' )
                ನಿಲ್ಲಿಸು;
            ಇಲ್ಲದಿದ್ದರೆ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಆದರೆ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ಇಲ್ಲದಿದ್ದರೆ
            ptr++;

        fs->finalizeCollection(ಗಂಟು);
        ಹಿಂತಿರುಗಿ ptr;
    }

    ಅಕ್ಷರ* parseMap( ಅಕ್ಷರ* ptr, File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಂಕ ಆಳ = 0 )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಆದರೆ( ಆಳ > CV_PERSISTENCE_MAX_ಆಳ )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ಆದರೆ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ಇಲ್ಲದಿದ್ದರೆ
            ptr++;

        fs->convert_ಗಮ್ಯ_Collection(File_ಗಂಟು::MAP, ಗಂಟು);

        ಗಾಗಿ( ;; )
        {
            ptr = skipSpaces( ptr );
            ಆದರೆ( !ptr || !*ptr )
                ನಿಲ್ಲಿಸು;

            ಆದರೆ ( *ptr == '"' )
            {
                File_ಗಂಟು ಮಗು;
                ptr = parse_ಕೀಲಿ( ptr, ಗಂಟು, ಮಗು );
                ಆದರೆ( !ptr || !*ptr )
                    ನಿಲ್ಲಿಸು;
                ptr = skipSpaces( ptr );
                ಆದರೆ( !ptr || !*ptr )
                    ನಿಲ್ಲಿಸು;

                ಆದರೆ ( *ptr == '[' )
                    ptr = parseSeq( ptr, ಮಗು, ಆಳ + 1 );
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '{' )
                    ptr = parseMap( ptr, ಮಗು, ಆಳ + 1 );
                ಇಲ್ಲದಿದ್ದರೆ
                    ptr = parse_ಮೌಲ್ಯ( ptr, ಮಗು );
            }

            ptr = skipSpaces( ptr );
            ಆದರೆ( !ptr || !*ptr )
                ನಿಲ್ಲಿಸು;

            ಆದರೆ ( *ptr == ',' )
                ptr++;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '}' )
                ನಿಲ್ಲಿಸು;
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ಆದರೆ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ಇಲ್ಲದಿದ್ದರೆ
            ptr++;

        fs->finalizeCollection(ಗಂಟು);
        ಹಿಂತಿರುಗಿ ptr;
    }

    ತಾರ್ಕಿಕ parse( ಅಕ್ಷರ* ptr )
    {
        ಆದರೆ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ಆದರೆ ( !ptr || !*ptr )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        File_ಗಂಟು ಬೇರು_collection(fs->getFS(), 0, 0);

        ಆದರೆ( *ptr == '{' )
        {
            File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->ಕೂಡಿಸು_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::MAP);
            parseMap( ptr, ಬೇರು_ಗಂಟು );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ptr == '[' )
        {
            File_ಗಂಟು ಬೇರು_ಗಂಟು = fs->ಕೂಡಿಸು_ಗಂಟು(ಬೇರು_collection, std::string(), File_ಗಂಟು::SEQ);
            parseSeq( ptr, ಬೇರು_ಗಂಟು );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    FileStorage_API* fs;
    ಅಕ್ಷರ buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ಹಿಂತಿರುಗಿ makePtr<JSONParser>(fs);
}

}
