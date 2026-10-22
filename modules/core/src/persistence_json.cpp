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
                                  puur_naa_nka struct_flags, const var_na* type_name=0 )
    {
        var_na data[CV_FS_MAX_LEN + 1024];

        struct_flags = (struct_flags & (FileNode::TYPE_MASK|FileNode::FLOW)) | FileNode::EMPTY;
        ꦪꦼꦤ꧀( !FileNode::isCollection(struct_flags))
            CV_Error( cv::Error::StsBadArg,
                     "Some collection type - FileNode::SEQ or FileNode::MAP, must be specified" );

        ꦪꦼꦤ꧀( type_name && *type_name == '\0' )
            type_name = 0;

        bool is_real_collection = true;
        ꦪꦼꦤ꧀ (type_name && memcmp(type_name, "binary", 6) == 0)
        {
            struct_flags = FileNode::STR;
            data[0] = '\0';
            is_real_collection = false;
        }

        ꦪꦼꦤ꧀ ( is_real_collection )
        {
            var_na c = FileNode::isMap(struct_flags) ? '{' : '[';
            data[0] = c;
            data[1] = '\0';
        }

        writeScalar( key, data );
        FStructData current_struct("", struct_flags, parent.indent + 4);

        ꦮꦭꦶ current_struct;
    }

    void endWriteStruct(const FStructData& current_struct)
    {
        puur_naa_nka struct_flags = current_struct.flags;

        ꦪꦼꦤ꧀ (FileNode::isCollection(struct_flags)) {
            ꦪꦼꦤ꧀ (!FileNode::isFlow(struct_flags)) {
                ꦪꦼꦤ꧀ (fs->bufferPtr() <= fs->bufferStart() + fs->get_space()) {
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
            ꦪꦼꦤ꧀ (ptr > fs->bufferStart() + current_struct.indent && !FileNode::isEmptyCollection(struct_flags))
                *ptr++ = ' ';
            *ptr++ = FileNode::isMap(struct_flags) ? '}' : ']';
            fs->setBufferPtr(ptr);
        }
    }

    void write(const var_na* key, puur_naa_nka value)
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
        puur_naa_nka i, len;

        ꦪꦼꦤ꧀( !str )
            CV_Error( cv::Error::StsNullPtr, "Null string pointer" );

        len = (puur_naa_nka)strlen(str);
        ꦪꦼꦤ꧀( len > CV_FS_MAX_LEN )
            CV_Error( cv::Error::StsBadArg, "The written string is too long" );

        ꦪꦼꦤ꧀( quote || len == 0 || str[0] != str[len-1] || (str[0] != '\"' && str[0] != '\'') )
        {
            puur_naa_nka need_quote = 1;
            data = buf;
            *data++ = '\"';
            ꦏꦁꦒꦺꦴ( i = 0; i < len; i++ )
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
                    ꦪꦼꦤ꧀ (byte < 0x20)
                    {
                        static const var_na hex[] = "0123456789abcdef";
                        *data++ = '\\'; *data++ = 'u'; *data++ = '0'; *data++ = '0';
                        *data++ = hex[byte >> 4]; *data++ = hex[byte & 0xf];
                    }
                    ꦭꦶꦪ
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
        ꦪꦼꦤ꧀ ( fs->get_state_of_writing_base64() == FileStorage_API::Uncertain )
        {
            fs->switch_to_Base64_state( FileStorage_API::NotUse );
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ ( fs->get_state_of_writing_base64() == FileStorage_API::InUse )
        {
            CV_Error( cv::Error::StsError, "At present, output Base64 data only." );
        }

        /* check parameters */

        size_t key_len = 0u;
        ꦪꦼꦤ꧀( key && *key == '\0' )
            key = 0;
        ꦪꦼꦤ꧀ ( key )
        {
            key_len = strlen(key);
            ꦪꦼꦤ꧀ ( key_len == 0u )
                CV_Error( cv::Error::StsBadArg, "The key is an empty" );
            ꦭꦶꦪ ꦪꦼꦤ꧀ ( static_cast<puur_naa_nka>(key_len) > CV_FS_MAX_LEN )
                CV_Error( cv::Error::StsBadArg, "The key is too long" );
        }

        size_t data_len = 0u;
        ꦪꦼꦤ꧀ ( data )
            data_len = strlen(data);

        FStructData& current_struct = fs->getCurrentStruct();
        puur_naa_nka struct_flags = current_struct.flags;
        ꦪꦼꦤ꧀( FileNode::isCollection(struct_flags) )
        {
            ꦪꦼꦤ꧀ ( (FileNode::isMap(struct_flags) ^ (key != 0)) )
                CV_Error( cv::Error::StsBadArg, "An attempt to add element without a key to a map, "
                         "or add element with key to sequence" );
        } ꦭꦶꦪ {
            fs->setNonEmpty();
            struct_flags = FileNode::EMPTY | (key ? FileNode::MAP : FileNode::SEQ);
        }

        // start to write
        var_na* ptr = 0;

        ꦪꦼꦤ꧀( FileNode::isFlow(struct_flags) )
        {
            puur_naa_nka new_offset;
            ptr = fs->bufferPtr();
            ꦪꦼꦤ꧀( !FileNode::isEmptyCollection(struct_flags) )
                *ptr++ = ',';
            new_offset = static_cast<puur_naa_nka>(ptr - fs->bufferStart() + key_len + data_len);
            ꦪꦼꦤ꧀( new_offset > fs->wrapMargin() && new_offset - current_struct.indent > 10 )
            {
                fs->setBufferPtr(ptr);
                ptr = fs->flush();
            }
            ꦭꦶꦪ
                *ptr++ = ' ';
        }
        ꦭꦶꦪ
        {
            ꦪꦼꦤ꧀ ( !FileNode::isEmptyCollection(struct_flags) )
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

        ꦪꦼꦤ꧀( key )
        {
            ꦪꦼꦤ꧀( !cv_isalpha(key[0]) && key[0] != '_' )
                CV_Error( cv::Error::StsBadArg, "Key must start with a letter or _" );

            ptr = fs->resizeWriteBuffer( ptr, static_cast<puur_naa_nka>(key_len) );
            *ptr++ = '\"';

            ꦏꦁꦒꦺꦴ( size_t i = 0u; i < key_len; i++ )
            {
                var_na c = key[i];

                ptr[i] = c;
                ꦪꦼꦤ꧀( !cv_isalnum(c) && c != '-' && c != '_' && c != ' ' )
                    CV_Error( cv::Error::StsBadArg, "Key names may only contain alphanumeric characters [a-zA-Z0-9], '-', '_' and ' '" );
            }

            ptr += key_len;
            *ptr++ = '\"';
            *ptr++ = ':';
            *ptr++ = ' ';
        }

        ꦪꦼꦤ꧀( data )
        {
            ptr = fs->resizeWriteBuffer( ptr, static_cast<puur_naa_nka>(data_len) );
            memcpy( ptr, data, data_len );
            ptr += data_len;
        }

        fs->setBufferPtr(ptr);
        current_struct.flags &= ~FileNode::EMPTY;
    }

    void writeComment(const var_na* comment, bool eol_comment)
    {
        ꦪꦼꦤ꧀( !comment )
            CV_Error( cv::Error::StsNullPtr, "Null comment" );

        puur_naa_nka len = static_cast<puur_naa_nka>(strlen(comment));
        var_na* ptr = fs->bufferPtr();
        const var_na* eol = strchr(comment, '\n');
        bool multiline = eol != 0;

        ꦪꦼꦤ꧀( !eol_comment || multiline || fs->bufferEnd() - ptr < len || ptr == fs->bufferStart() )
            ptr = fs->flush();
        ꦭꦶꦪ
            *ptr++ = ' ';

        ꦱꦼꦢꦼꦭ( comment )
        {
            *ptr++ = '/';
            *ptr++ = '/';
            *ptr++ = ' ';
            ꦪꦼꦤ꧀( eol )
            {
                ptr = fs->resizeWriteBuffer( ptr, (puur_naa_nka)(eol - comment) + 1 );
                memcpy( ptr, comment, eol - comment + 1 );
                fs->setBufferPtr(ptr + (eol - comment));
                comment = eol + 1;
                eol = strchr( comment, '\n' );
            }
            ꦭꦶꦪ
            {
                len = (puur_naa_nka)strlen(comment);
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

        ꦱꦼꦢꦼꦭ ( is_eof == false && is_completed == false )
        {
            ꦪꦼꦤ꧀ (!ptr)
                CV_PARSE_ERROR_CPP("Invalid input");
            switch ( *ptr )
            {
                /* comment */
                pak_sa '/' : {
                    ptr++;
                    ꦪꦼꦤ꧀ ( *ptr == '\0' )
                    {
                        ptr = fs->gets();
                        ꦪꦼꦤ꧀( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                    }

                    ꦪꦼꦤ꧀ ( *ptr == '/' )
                    {
                        ꦱꦼꦢꦼꦭ ( *ptr != '\n' && *ptr != '\r' )
                        {
                            ꦪꦼꦤ꧀ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ꦪꦼꦤ꧀( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                            }
                            ꦭꦶꦪ
                            {
                                ptr++;
                            }
                        }
                    }
                    ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '*' )
                    {
                        ptr++;
                        ꦏꦁꦒꦺꦴ (;;)
                        {
                            ꦪꦼꦤ꧀ ( *ptr == '\0' )
                            {
                                ptr = fs->gets();
                                ꦪꦼꦤ꧀( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                            }
                            ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '*' )
                            {
                                ptr++;
                                ꦪꦼꦤ꧀ ( *ptr == '\0' )
                                {
                                    ptr = fs->gets();
                                    ꦪꦼꦤ꧀( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                                }
                                ꦪꦼꦤ꧀ ( *ptr == '/' )
                                {
                                    ptr++;
                                    bha_nga;
                                }
                            }
                            ꦭꦶꦪ
                            {
                                ptr++;
                            }
                        }
                    }
                    ꦭꦶꦪ
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
                    ꦪꦼꦤ꧀( !ptr || !*ptr ) { is_eof = true; bha_nga; }
                } bha_nga;
                    /* other character */
                default: {
                    ꦪꦼꦤ꧀( !cv_isprint(*ptr) )
                        CV_PARSE_ERROR_CPP( "Invalid character in the stream" );
                    is_completed = true;
                } bha_nga;
            }
        }

        ꦪꦼꦤ꧀ ( is_eof || !is_completed )
        {
            ptr = fs->bufferStart();
            CV_Assert(ptr);
            *ptr = '\0';
            fs->setEof();
            ꦪꦼꦤ꧀( !is_completed )
                CV_PARSE_ERROR_CPP( "Abort at parse time" );
        }

        ꦮꦭꦶ ptr;
    }

    uint32_t parseUnicodeEscape(var_na*& ptr)
    {
        uint32_t codepoint = 0;
        ꦏꦁꦒꦺꦴ (puur_naa_nka k = 0; k < 4; k++, ptr++) {
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            var_na hex = *ptr;
            uint32_t digit = 0;
            ꦪꦼꦤ꧀      (hex >= '0' && hex <= '9') digit = (uint32_t)(hex - '0');
            ꦭꦶꦪ ꦪꦼꦤ꧀ (hex >= 'a' && hex <= 'f') digit = (uint32_t)(hex - 'a') + 10u;
            ꦭꦶꦪ ꦪꦼꦤ꧀ (hex >= 'A' && hex <= 'F') digit = (uint32_t)(hex - 'A') + 10u;
            ꦭꦶꦪ CV_PARSE_ERROR_CPP("invalid \\uXXXX escape sequence");
            codepoint = (codepoint << 4) | digit;
        }
        ꦮꦭꦶ codepoint;
    }

    static void appendUtf8(std::string& out, uint32_t codepoint)
    {
        ꦪꦼꦤ꧀ (codepoint < 0x80) {
            out += (var_na)codepoint;
        } ꦭꦶꦪ ꦪꦼꦤ꧀ (codepoint < 0x800) {
            out += (var_na)(0xC0 | (codepoint >> 6));
            out += (var_na)(0x80 | (codepoint & 0x3F));
        } ꦭꦶꦪ ꦪꦼꦤ꧀ (codepoint < 0x10000) {
            out += (var_na)(0xE0 | (codepoint >> 12));
            out += (var_na)(0x80 | ((codepoint >> 6) & 0x3F));
            out += (var_na)(0x80 | (codepoint & 0x3F));
        } ꦭꦶꦪ {
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
        ꦪꦼꦤ꧀ (codepoint >= 0xD800 && codepoint <= 0xDBFF)
        {
            // ptr is just past the four hex digits; the low half must be its own escape.
            // Short-circuits before ptr[1] when ptr[0] is the buffer's terminating NUL.
            ꦪꦼꦤ꧀ (ptr[0] != '\\' || ptr[1] != 'u')
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            ptr += 2;
            uint32_t low = parseUnicodeEscape(ptr);
            ꦪꦼꦤ꧀ (low < 0xDC00 || low > 0xDFFF)
                CV_PARSE_ERROR_CPP("high surrogate is not followed by a \\uXXXX low surrogate");
            codepoint = 0x10000u + ((codepoint - 0xD800u) << 10) + (low - 0xDC00u);
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            CV_PARSE_ERROR_CPP("unpaired \\uXXXX low surrogate");
        appendUtf8(out, codepoint);
    }

    var_na* parseKey( var_na* ptr, FileNode& collection, FileNode& value_placeholder )
    {
        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ꦪꦼꦤ꧀( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must start with \'\"\'" );

        var_na * beg = ptr + 1;
        std::string key_name;
        do {
            ꦪꦼꦤ꧀ (*ptr == '\\') { // skip the next character if current is back slash
                ++ptr;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
                ꦪꦼꦤ꧀ (*ptr == 'u') {
                    ++ptr;
                    parseUnicodeEscapeToUtf8(ptr, key_name);
                    continue;
                }
                key_name += *ptr;
            } ꦭꦶꦪ ꦪꦼꦤ꧀ (*ptr != '"') {
                // normal byte: append current, do NOT skip ahead first
                key_name += *ptr;
            }
            ++ptr;
            CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
        } ꦱꦼꦢꦼꦭ( cv_isprint(*ptr) && *ptr != '"' );

        ꦪꦼꦤ꧀( *ptr != '"' )
            CV_PARSE_ERROR_CPP( "Key must end with \'\"\'" );

        ꦪꦼꦤ꧀( ptr == beg )
            CV_PARSE_ERROR_CPP( "Key is empty" );
        value_placeholder = fs->addNode(collection, key_name, FileNode::NONE);

        ptr++;
        ptr = skipSpaces( ptr );
        ꦪꦼꦤ꧀( !ptr || !*ptr )
            ꦮꦭꦶ 0;

        ꦪꦼꦤ꧀( *ptr != ':' )
            CV_PARSE_ERROR_CPP( "Missing \':\' between key and value" );

        ꦮꦭꦶ ++ptr;
    }

    bool getBase64Row(var_na* ptr, puur_naa_nka /*indent*/, var_na* &beg, var_na* &end)
    {
        beg = end = ptr;
        ꦪꦼꦤ꧀( !ptr || !*ptr )
            ꦮꦭꦶ false;

        // find end of the row
        ꦱꦼꦢꦼꦭ( cv_isprint(*ptr) && (*ptr != ',') && (*ptr != '"'))
            ++ptr;
        ꦪꦼꦤ꧀ ( *ptr == '\0' )
            CV_PARSE_ERROR_CPP( "Unexpected end of line" );

        end = ptr;
        ꦮꦭꦶ true;
    }

    var_na* parseValue( var_na* ptr, FileNode& node )
    {
        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid value input");

        ptr = skipSpaces( ptr );
        ꦪꦼꦤ꧀( !ptr || !*ptr )
            CV_PARSE_ERROR_CPP( "Unexpected End-Of-File" );

        ꦪꦼꦤ꧀( *ptr == '"' )
        {   /* must be string or Base64 string */
            ptr++;
            var_na * beg = ptr;
            size_t len = 0u;
            ꦏꦁꦒꦺꦴ ( ; (cv_isalnum(*ptr) || *ptr == '$' ) && len <= 9u; ptr++ )
                len++;

            ꦪꦼꦤ꧀ ((len >= 8u) && (memcmp( beg, "$base64$", 8u ) == 0) )
            {   /**************** Base64 string ****************/
                ptr = beg + 8;
                ptr = fs->parseBase64(ptr, 0, node);

                ꦪꦼꦤ꧀ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ꦭꦶꦪ
                    ptr++;
            }
            ꦭꦶꦪ
            {   /**************** normal string ****************/
                puur_naa_nka i = 0, sz;

                ptr = beg;
                bool is_matching = false;
                ꦱꦼꦢꦼꦭ ( !is_matching )
                {
                    switch ( *ptr )
                    {
                        pak_sa '\\':
                        {
                            sz = (puur_naa_nka)(ptr - beg);
                            ꦪꦼꦤ꧀( sz > 0 )
                            {
                                ꦪꦼꦤ꧀ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr++;
                            ꦪꦼꦤ꧀ (i + 1 >= CV_FS_MAX_LEN)
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
                                ꦪꦼꦤ꧀ (i + (puur_naa_nka)utf8.size() >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, utf8.data(), utf8.size());
                                i += (puur_naa_nka)utf8.size();
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
                            sz = (puur_naa_nka)(ptr - beg);
                            ꦪꦼꦤ꧀( sz > 0 )
                            {
                                ꦪꦼꦤ꧀ (i + sz >= CV_FS_MAX_LEN)
                                    CV_PARSE_ERROR_CPP("string is too long");
                                memcpy(buf + i, beg, sz);
                                i += sz;
                            }
                            ptr = fs->gets();
                            ꦪꦼꦤ꧀ ( !ptr || !*ptr )
                                CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );

                            beg = ptr;
                            bha_nga;
                        }
                        pak_sa '\"':
                        {
                            sz = (puur_naa_nka)(ptr - beg);
                            ꦪꦼꦤ꧀( sz > 0 )
                            {
                                ꦪꦼꦤ꧀ (i + sz >= CV_FS_MAX_LEN)
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

                ꦪꦼꦤ꧀ ( *ptr != '\"' )
                    CV_PARSE_ERROR_CPP( "'\"' - right-quote of string is missing" );
                ꦭꦶꦪ
                    ptr++;

                node.setValue(FileNode::STRING, buf, i);
            }
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ ( cv_isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == '.' )
        {    /**************** number ****************/
            var_na * beg = ptr;
            ꦪꦼꦤ꧀ ( *ptr == '+' || *ptr == '-' )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ꦱꦼꦢꦼꦭ( cv_isdigit(*ptr) )
            {
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }
            ꦪꦼꦤ꧀ (*ptr == '.' || *ptr == 'e')
            {
                dviguu_na fval = fs->strtod( beg, &ptr );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::REAL, &fval);
            }
            ꦭꦶꦪ
            {
                int64_t ival = strtoll( beg, &ptr, 0 );
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();

                node.setValue(FileNode::INT, &ival);
            }

            ꦪꦼꦤ꧀ ( beg >= ptr )
                CV_PARSE_ERROR_CPP( "Invalid numeric value (inconsistent explicit type specification?)" );
        }
        ꦭꦶꦪ
        {    /**************** other data ****************/
            const var_na* beg = ptr;
            puur_naa_nka len = 0;
            ꦏꦁꦒꦺꦴ ( ; cv_isalpha(*ptr) && len <= 6; )
            {
                len++;
                ptr++;
                CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP();
            }

            ꦪꦼꦤ꧀( len == 4 && memcmp( beg, "null", 4 ) == 0 )
                ;
            ꦭꦶꦪ ꦪꦼꦤ꧀( (len == 4 && memcmp( beg, "true", 4 ) == 0) ||
                     (len == 5 && memcmp( beg, "false", 5 ) == 0) )
            {
                int64_t ival = *beg == 't' ? 1 : 0;
                node.setValue(FileNode::INT, &ival);
            }
            ꦭꦶꦪ
            {
                CV_PARSE_ERROR_CPP( "Unrecognized value" );
            }
        }

        ꦮꦭꦶ ptr;
    }

    var_na* parseSeq( var_na* ptr, FileNode& node, puur_naa_nka depth = 0 )
    {
        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP( "ptr is NULL" );

        ꦪꦼꦤ꧀( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ꦪꦼꦤ꧀ ( *ptr != '[' )
            CV_PARSE_ERROR_CPP( "'[' - left-brace of seq is missing" );
        ꦭꦶꦪ
            ptr++;

        fs->convertToCollection(FileNode::SEQ, node);

        ꦏꦁꦒꦺꦴ (;;)
        {
            ptr = skipSpaces( ptr );
            ꦪꦼꦤ꧀( !ptr || !*ptr )
                bha_nga;

            ꦪꦼꦤ꧀ ( *ptr != ']' )
            {
                FileNode child = fs->addNode(node, std::string(), FileNode::NONE );

                ꦪꦼꦤ꧀ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ꦭꦶꦪ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ꦪꦼꦤ꧀( !ptr || !*ptr )
                bha_nga;

            ꦪꦼꦤ꧀ ( *ptr == ',' )
                ptr++;
            ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == ']' )
                bha_nga;
            ꦭꦶꦪ
                CV_PARSE_ERROR_CPP( "Unexpected character" );
        }

        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ꦪꦼꦤ꧀ ( *ptr != ']' )
            CV_PARSE_ERROR_CPP( "']' - right-brace of seq is missing" );
        ꦭꦶꦪ
            ptr++;

        fs->finalizeCollection(node);
        ꦮꦭꦶ ptr;
    }

    var_na* parseMap( var_na* ptr, FileNode& node, puur_naa_nka depth = 0 )
    {
        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ꦪꦼꦤ꧀( depth > CV_PERSISTENCE_MAX_DEPTH )
            CV_PARSE_ERROR_CPP("Too many nested collections");

        ꦪꦼꦤ꧀ ( *ptr != '{' )
            CV_PARSE_ERROR_CPP( "'{' - left-brace of map is missing" );
        ꦭꦶꦪ
            ptr++;

        fs->convertToCollection(FileNode::MAP, node);

        ꦏꦁꦒꦺꦴ( ;; )
        {
            ptr = skipSpaces( ptr );
            ꦪꦼꦤ꧀( !ptr || !*ptr )
                bha_nga;

            ꦪꦼꦤ꧀ ( *ptr == '"' )
            {
                FileNode child;
                ptr = parseKey( ptr, node, child );
                ꦪꦼꦤ꧀( !ptr || !*ptr )
                    bha_nga;
                ptr = skipSpaces( ptr );
                ꦪꦼꦤ꧀( !ptr || !*ptr )
                    bha_nga;

                ꦪꦼꦤ꧀ ( *ptr == '[' )
                    ptr = parseSeq( ptr, child, depth + 1 );
                ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '{' )
                    ptr = parseMap( ptr, child, depth + 1 );
                ꦭꦶꦪ
                    ptr = parseValue( ptr, child );
            }

            ptr = skipSpaces( ptr );
            ꦪꦼꦤ꧀( !ptr || !*ptr )
                bha_nga;

            ꦪꦼꦤ꧀ ( *ptr == ',' )
                ptr++;
            ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '}' )
                bha_nga;
            ꦭꦶꦪ
            {
                CV_PARSE_ERROR_CPP( "Unexpected character" );
            }
        }

        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("ptr is NULL");

        ꦪꦼꦤ꧀ ( *ptr != '}' )
            CV_PARSE_ERROR_CPP( "'}' - right-brace of map is missing" );
        ꦭꦶꦪ
            ptr++;

        fs->finalizeCollection(node);
        ꦮꦭꦶ ptr;
    }

    bool parse( var_na* ptr )
    {
        ꦪꦼꦤ꧀ (!ptr)
            CV_PARSE_ERROR_CPP("Invalid input");

        ptr = skipSpaces( ptr );
        ꦪꦼꦤ꧀ ( !ptr || !*ptr )
            ꦮꦭꦶ false;

        FileNode root_collection(fs->getFS(), 0, 0);

        ꦪꦼꦤ꧀( *ptr == '{' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::MAP);
            parseMap( ptr, root_node );
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ ( *ptr == '[' )
        {
            FileNode root_node = fs->addNode(root_collection, std::string(), FileNode::SEQ);
            parseSeq( ptr, root_node );
        }
        ꦭꦶꦪ
        {
            CV_PARSE_ERROR_CPP( "left-brace of top level is missing" );
        }

        ꦮꦭꦶ true;
    }

    FileStorage_API* fs;
    var_na buf[CV_FS_MAX_LEN+1024];
};

Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs)
{
    ꦮꦭꦶ makePtr<JSONEmitter>(fs);
}

Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs)
{
    ꦮꦭꦶ makePtr<JSONParser>(fs);
}

}
