// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"

namespace cv
{

class JSONEmitter : public FileStorageEmitter
{
public:
    JSONEmitter(FileStorage_API* _fs) : fs(_fs)
    {
    }
    virtual ~JSONEmitter() {}

    FStructData startWriteStruct( const FStructData& parent, const var_na* key,
                                  ຈຳນວນເຕັມ struct_flags, const var_na* type_name=0 )
    {
        var_na data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ຖ້າ( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ຖ້າ( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        ຖ້າ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = false;
        }

        ຖ້າ ( is_real_collection )
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        ສົ່ງຄືນ current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        ຈຳນວນເຕັມ struct_flags = current_struct.flags;

        ຖ້າ (FileNode::isCollection(struct_flags)) {
            ຖ້າ (!FileNode::isFlow(struct_flags)) {
                ຖ້າ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
                    /* some bad code for base64_writer... */
                    var_na *ptr = fs->bufferPtr();
                    *ptr++ = '\n';
                    *ptr++ = '\0';
                    fs->puts(fs->bufferStart());
                    fs->setBufferPtr(fs->bufferStart());
                }
                fs->flush();
            }

            var_na *ptr = fs->bufferPtr();
            ຖ້າ (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    void write(const var_na* key, ຈຳນວນເຕັມ value)
    {
        var_na buf[128];
        writeScalar( key, fs::itoa( value, buf, 10 ));
    }

    void write(const var_na* key, int64_t value)
    {
        var_na buf[128];
        writeScalar( key, fs::itoa( value, buf, 10, true ));
    }

    void write( const var_na* key, dviguu_na value )
    {
        var_na buf[128];
        writeScalar( key, fs::doubleToString( buf, parimaa_na(buf), value, true ));
    }

    void write(const var_na* key, const var_na* str, bool quote)
    {
        var_na buf[CV_FS_MAX_LEN*6+16];
        var_na* data = (var_na*)str;
        ຈຳນວນເຕັມ i, len;

        ຖ້າ( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (ຈຳນວນເຕັມ)strlen(str);
        ຖ້າ( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ຖ້າ( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            ຈຳນວນເຕັມ need_quote = 1;
            data = buf;
            *data++ = '\"';
            ສຳລັບ( i = 0; i < len; i++ )
            {
                var_na c = str[i];
                const unsigned var_na byte = static_cast<unsigned var_na>(c);

                switch ( c )
                {
                pak_sa '\\':
                pak_sa '\"': { *data++ = '\\'; *data++ = c;   bha_nga; }
                pak_sa '\n': { *data++ = '\\'; *data++ = 'n'; bha_nga; }
                pak_sa '\r': { *data++ = '\\'; *data++ = 'r'; bha_nga; }
                pak_sa '\t': { *data++ = '\\'; *data++ = 't'; bha_nga; }
                pak_sa '\b': { *data++ = '\\'; *data++ = 'b'; bha_nga; }
                pak_sa '\f': { *data++ = '\\'; *data++ = 'f'; bha_nga; }
                default  :
                    ຖ້າ (byte < 0x20)
                    {
                        static const var_na hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    ບໍ່ດັ່ງນັ້ນ
                        *data++ = c;
                    bha_nga;
                }
            }

            *data++ = '\"';
            *data++ = '\0';
            data = buf + !need_quote;
        }

        writeScalar( key, data);
    }

    void writeScalar(const var_na* key, const var_na* data)
    {
        /* check write_struct */

        fs->check_if_write_struct_is_delayed(false);
        ຖ້າ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        ຖ້າ( key && *key == '\0' )
            key = 0;
        ຖ້າ ( key )
        {
            key_len = strlen(key);
            ຖ້າ ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ບໍ່ດັ່ງນັ້ນ ຖ້າ ( static_cast<ຈຳນວນເຕັມ>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ຖ້າ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        ຈຳນວນເຕັມ struct_flags = current_struct.flags;
        ຖ້າ( FileNode::isCollection(struct_flags) )
        {
            ຖ້າ ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ບໍ່ດັ່ງນັ້ນ {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        var_na* ptr = 0;

        ຖ້າ( FileNode::isFlow(struct_flags) )
        {
            ຈຳນວນເຕັມ new_offset;
            ptr = fs->bufferPtr();
            ຖ້າ( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<ຈຳນວນເຕັມ>(ptr - fs->bufferStart() + key_len + data_len);
            ຖ້າ( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ບໍ່ດັ່ງນັ້ນ
                *ptr++ = ' ';
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ຖ້າ ( !FileNode::isEmptyCollection(struct_flags) )
            {
                ptr = fs->bufferPtr();
                *ptr++ = ',';
                *ptr++ = '\n';
                *ptr++ = '\0';
                fs->puts( fs->bufferStart() );
                fs->setBufferPtr(fs->bufferStart());
            }
            ptr = fs->flush();
        }

        ຖ້າ( key )
        {
            ຖ້າ( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<ຈຳນວນເຕັມ>(key_len) );
            *ptr++ = '\"';

            ສຳລັບ( size_t i = 0u; i < key_len; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ຖ້າ( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ຖ້າ( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<ຈຳນວນເຕັມ>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        ຖ້າ( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        ຈຳນວນເຕັມ len = static_cast<ຈຳນວນເຕັມ>(strlen(comment));
        var_na* ptr = fs->bufferPtr();
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;

        ຖ້າ( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ບໍ່ດັ່ງນັ້ນ
            *ptr++ = ' ';

        ໃນຂະນະທີ່( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ຖ້າ( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (ຈຳນວນເຕັມ)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                len = (ຈຳນວນເຕັມ)strlen(comment);
                ptr = fs->resizeWriteBuffer( ptr, len );
                memcpy( ptr, comment, len );
                fs->setBufferPtr(ptr + len);
                comment = 0;
            }
            ptr = fs->flush();
        }
    }

    void startNextStream()
    {
        fs->puts( "...\n" );
        fs->puts( "---\n" );
    }

rak_sita:
    FileStorage_API* fs;
};

class JSONParser : public FileStorageParser
{
public:
    JSONParser(FileStorage_API* _fs) : fs(_fs)
    {
    }

    virtual ~JSONParser() {}

    var_na* skipSpaces( var_na* ptr )
    {
        bool is_eof = false;
        bool is_completed = false;

        ໃນຂະນະທີ່ ( is_eof == false && is_completed == false )
        {
            ຖ້າ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            switch ( *ptr )
            {
                /* comment */
                pak_sa '/' : {
                    ptr++;
                    ຖ້າ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ຖ້າ( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                    }

                    ຖ້າ ( *ptr == '/' )
                    {
                        ໃນຂະນະທີ່ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ຖ້າ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ຖ້າ( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                            }
                            ບໍ່ດັ່ງນັ້ນ
                            {
                                ptr++;
                            }
                        }
                    }
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '*' )
                    {
                        ptr++;
                        ສຳລັບ (;;)
                        {
                            ຖ້າ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ຖ້າ( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                            }
                            ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '*' )
                            {
                                ptr++;
                                ຖ້າ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ຖ້າ( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                                }
                                ຖ້າ ( *ptr == '/' )
                                {
                                    ptr++;
                                    bha_nga;
                                }
                            }
                            ບໍ່ດັ່ງນັ້ນ
                            {
                                ptr++;
                            }
                        }
                    }
                    ບໍ່ດັ່ງນັ້ນ
                    {
                        CV_PARSE_ERROR_CPP( "Not supported escape character" );
                    }
                } bha_nga;
                    /* whitespace */
                pak_sa '\t':
                pak_sa ' ' : {
                    ptr++;
                } bha_nga;
                    /* newline || end mark */
                pak_sa '\0':
                pak_sa '\n':
                pak_sa '\r': {
                    ptr = fs->gets();
                    ຖ້າ( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                } bha_nga;
                    /* other character */
                default: {
                    ຖ້າ( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } bha_nga;
            }
        }

        ຖ້າ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ຖ້າ( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ສົ່ງຄືນ ptr;
    }

    uint32_t parseUnicodeEscape(var_na*& ptr)
    {
        uint32_t codepoint = 0;
        ສຳລັບ (ຈຳນວນເຕັມ k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            var_na hex = *ptr;
            uint32_t digit = 0;
            ຖ້າ      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ບໍ່ດັ່ງນັ້ນ ຖ້າ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ບໍ່ດັ່ງນັ້ນ ຖ້າ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ບໍ່ດັ່ງນັ້ນ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ສົ່ງຄືນ codepoint;
    }

    static void appendUtf8(std::string& out, uint32_t codepoint)
    {
        ຖ້າ (codepoint < 0x80) {
            out += (var_na)codepoint;
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (codepoint < 0x800) {
            out += (var_na)(0xC0 | (codepoint >> 6));
            out += (var_na)(0x80 | (codepoint & 0x3F));
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (codepoint < 0x10000) {
            out += (var_na)(0xE0 | (codepoint >> 12));
            out += (var_na)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (var_na)(0x80 | (codepoint & 0x3F));
        } ບໍ່ດັ່ງນັ້ນ {
            out += (var_na)(0xF0 | (codepoint >> 18));
            out += (var_na)(0x80 | ((codepoint >> 12) & 0x3F));
            out += (var_na)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (var_na)(0x80 | (codepoint & 0x3F));
        }
    }

    // A \uXXXX escape carries one UTF-16 code unit, so anything above the BMP arrives as a
    // surrogate pair and has to be recombined here. Encoding the halves separately would
    // emit three bytes each for code points that are not Unicode scalar values at all --
    // CESU-8, which no UTF-8 reader accepts.
    void parseUnicodeEscapeToUtf8(var_na*& ptr, std::string& out)
    {
        uint32_t codepoint = parseUnicodeEscape(ptr);
        ຖ້າ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ຖ້າ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            ຖ້າ (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    var_na* parseKey( var_na* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ຖ້າ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        var_na * beg = ptr + 1;
        std::string key_name;
        do {
            ຖ້າ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ຖ້າ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    continue;
                }
                key_name += *ptr;
            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ໃນຂະນະທີ່( cv_isprint(*ptr) && *ptr != '"' );

        ຖ້າ( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ຖ້າ( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ຖ້າ( !ptr || !*ptr )
            ສົ່ງຄືນ 0;

        ຖ້າ( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ສົ່ງຄືນ ++ptr;
    }

    bool getBase64Row(var_na* ptr, ຈຳນວນເຕັມ /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr;
        ຖ້າ( !ptr || !*ptr )
            ສົ່ງຄືນ false;

        // find end of the row
        ໃນຂະນະທີ່( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ຖ້າ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ສົ່ງຄືນ true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ຖ້າ( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ຖ້າ( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            var_na * beg = ptr;
            size_t len = 0u;
            ສຳລັບ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ຖ້າ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                ຖ້າ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ບໍ່ດັ່ງນັ້ນ
                    ptr++;
            }
            ບໍ່ດັ່ງນັ້ນ
            {   /**************** normal string ****************/
                ຈຳນວນເຕັມ i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                ໃນຂະນະທີ່ ( !is_matching )
                {
                    switch ( *ptr )
                    {
                        pak_sa '\\':
                        {
                            sz = (ຈຳນວນເຕັມ)(ptr - beg);
                            ຖ້າ( sz > 0 )
                            {
                                ຖ້າ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ຖ້າ (i + 1 >= CV_FS_MAX_LEN)
                                CV_PARSE_ERROR_CPP("string is too long");
                            switch ( *ptr )
                            {
                            pak_sa '\\':
                            pak_sa '\"':
                            pak_sa '\'': { buf[i++] = *ptr; bha_nga; }
                            pak_sa 'n' : { buf[i++] = '\n'; bha_nga; }
                            pak_sa 'r' : { buf[i++] = '\r'; bha_nga; }
                            pak_sa 't' : { buf[i++] = '\t'; bha_nga; }
                            pak_sa 'b' : { buf[i++] = '\b'; bha_nga; }
                            pak_sa 'f' : { buf[i++] = '\f'; bha_nga; }
                            pak_sa 'u' : {
                                ptr++;
                                std::string utf8;
                                parseUnicodeEscapeToUtf8(ptr, utf8);
                                ຖ້າ (i + (ຈຳນວນເຕັມ)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (ຈຳນວນເຕັມ)utf8.size();
                                beg = ptr;
                                continue;
                            }
                            default  : { CV_PARSE_ERROR_CPP( "Invalid escape character" ); }
                            bha_nga;
                            }
                            ptr++;
                            beg = ptr;
                            bha_nga;
                        }
                        pak_sa '\0':
                        {
                            sz = (ຈຳນວນເຕັມ)(ptr - beg);
                            ຖ້າ( sz > 0 )
                            {
                                ຖ້າ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ຖ້າ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            bha_nga;
                        }
                        pak_sa '\"':
                        {
                            sz = (ຈຳນວນເຕັມ)(ptr - beg);
                            ຖ້າ( sz > 0 )
                            {
                                ຖ້າ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            beg = ptr;
                            is_matching = true;
                            bha_nga;
                        }
                        pak_sa '\n':
                        pak_sa '\r':
                        {
                            CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                            bha_nga;
                        }
                        default:
                        {
                            ptr++;
                            bha_nga;
                        }
                    }
                }

                ຖ້າ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ບໍ່ດັ່ງນັ້ນ
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            var_na * beg = ptr;
            ຖ້າ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ໃນຂະນະທີ່( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ຖ້າ (*ptr == '.' || *ptr == 'e')
            {
                dviguu_na fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            ຖ້າ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ບໍ່ດັ່ງນັ້ນ
        {    /**************** other data ****************/
            const var_na* beg = ptr;
            ຈຳນວນເຕັມ len = 0;
            ສຳລັບ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ຖ້າ( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ບໍ່ດັ່ງນັ້ນ ຖ້າ( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ສົ່ງຄືນ ptr;
    }

    var_na* parseSeq( var_na* ptr, FileNode& node, ຈຳນວນເຕັມ depth = 0 )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ຖ້າ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ຖ້າ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ບໍ່ດັ່ງນັ້ນ
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        ສຳລັບ (;;)
        {
            ptr = skipSpaces( ptr );
            ຖ້າ( !ptr || !*ptr )
                bha_nga;

            ຖ້າ ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                ຖ້າ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ບໍ່ດັ່ງນັ້ນ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ຖ້າ( !ptr || !*ptr )
                bha_nga;

            ຖ້າ ( *ptr == ',' )
                ptr++;
            ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == ']' )
                bha_nga;
            ບໍ່ດັ່ງນັ້ນ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ຖ້າ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ບໍ່ດັ່ງນັ້ນ
            ptr++;

        fs->finalizeCollection(node);
        ສົ່ງຄືນ ptr;
    }

    var_na* parseMap( var_na* ptr, FileNode& node, ຈຳນວນເຕັມ depth = 0 )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ຖ້າ( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ຖ້າ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ບໍ່ດັ່ງນັ້ນ
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        ສຳລັບ( ;; )
        {
            ptr = skipSpaces( ptr );
            ຖ້າ( !ptr || !*ptr )
                bha_nga;

            ຖ້າ ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                ຖ້າ( !ptr || !*ptr )
                    bha_nga;
                ptr = skipSpaces( ptr );
                ຖ້າ( !ptr || !*ptr )
                    bha_nga;

                ຖ້າ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ບໍ່ດັ່ງນັ້ນ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ຖ້າ( !ptr || !*ptr )
                bha_nga;

            ຖ້າ ( *ptr == ',' )
                ptr++;
            ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '}' )
                bha_nga;
            ບໍ່ດັ່ງນັ້ນ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ຖ້າ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ບໍ່ດັ່ງນັ້ນ
            ptr++;

        fs->finalizeCollection(node);
        ສົ່ງຄືນ ptr;
    }

    bool parse( var_na* ptr )
    {
        ຖ້າ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ຖ້າ ( !ptr || !*ptr )
            ສົ່ງຄືນ false;

        FileNode root_collection(fs->getFS(), 0, 0);

        ຖ້າ( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ສົ່ງຄືນ true;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ສົ່ງຄືນ makePtr<JSONParser>(fs);
}

}
