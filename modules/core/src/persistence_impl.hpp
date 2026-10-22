// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_PERSISTENCE_IMPL_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence_base64_encoding.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <unordered_map>
#ਅਨ੍ਤਰ੍ਭਾਵ <iterator>


ਨਾਮਾਕਾਸ਼ cv
{

ਗਿਣਤੀ Base64State{
    Uncertain,
    NotUse,
    ਵਿੱਚ_Use,
};

ਵਰਗ cv::FileStorage::Impl : ਜਨਤਕ FileStorage_API
{
ਜਨਤਕ:
    ਖਾਲੀ init();

    Impl(FileStorage* _fs);

    ਆਭਾਸੀ ~Impl();

    ਖਾਲੀ release(String* out=0);

    ਖਾਲੀ analyze_file_name( ਅਚਲ std::string& file_name, std::vector<std::string>& params );

    ਤਾਰਕਿਕ open( ਅਚਲ ਅੱਖਰ* filename_or_buf, ਪੂਰਨ _flags, ਅਚਲ ਅੱਖਰ* encoding );

    ਖਾਲੀ puts( ਅਚਲ ਅੱਖਰ* str );

    ਅੱਖਰ* getsFromFile( ਅੱਖਰ* buf, ਪੂਰਨ ਗਿਣਤੀ_3 );

    ਅੱਖਰ* gets( size_t max_ਗਿਣਤੀ );

    ਅੱਖਰ* gets();

    ਤਾਰਕਿਕ eof();

    ਖਾਲੀ ਸੈੱਟ_Eof();

    ਖਾਲੀ closeFile();

    ਖਾਲੀ rewind();

    ਅੱਖਰ* resize_Write_ਬਫਰ( ਅੱਖਰ* ptr, ਪੂਰਨ len );

    ਅੱਖਰ* flush();

    ਖਾਲੀ ਸਿਰਾ_Write_Struct();

    ਖਾਲੀ ਸ਼ੁਰੂ_Write_Struct_helper( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ struct_flags,
                                  ਅਚਲ ਅੱਖਰ* type_name );

    ਖਾਲੀ ਸ਼ੁਰੂ_Write_Struct( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ struct_flags,
                           ਅਚਲ ਅੱਖਰ* type_name );

    ਖਾਲੀ writeComment( ਅਚਲ ਅੱਖਰ* comment, ਤਾਰਕਿਕ eol_comment );

    ਖਾਲੀ ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ();

    ਖਾਲੀ ਲਿਖ( ਅਚਲ String& ਕੁੰਜੀ, ਪੂਰਨ value );

    ਖਾਲੀ ਲਿਖ( ਅਚਲ String& ਕੁੰਜੀ, ਤਾਰਕਿਕ value );

    ਖਾਲੀ ਲਿਖ( ਅਚਲ String& ਕੁੰਜੀ, int64_t value );

    ਖਾਲੀ ਲਿਖ( ਅਚਲ String& ਕੁੰਜੀ, ਦੁੱਗਣਾ value );

    ਖਾਲੀ ਲਿਖ( ਅਚਲ String& ਕੁੰਜੀ, ਅਚਲ String& value );

    ਖਾਲੀ write_Raw_ਅੰਕੜੇ( ਅਚਲ std::string& dt, ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ_3, size_t len );

    ਖਾਲੀ workaround();

    ਖਾਲੀ switch_ਮੰਜ਼ਿਲ_Base64_state( FileStorage_API::Base64State new_state);

    ਖਾਲੀ make_write_struct_delayed( ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ* type_name );

    ਖਾਲੀ check_if_write_struct_is_delayed( ਤਾਰਕਿਕ change_type_ਮੰਜ਼ਿਲ_base64 );

    ਖਾਲੀ write_Raw_ਅੰਕੜੇ_Base64(ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ_3, size_t len, ਅਚਲ ਅੱਖਰ* dt );

    String releaseAndGetString();

    File_ਗੰਢ get_ਪਹਿਲਾ_ਸਿਖਰ_Level_ਗੰਢ() ਅਚਲ;

    File_ਗੰਢ ਜੜ੍ਹ(ਪੂਰਨ ਧਾਰਾ_Idx=0) ਅਚਲ;

    File_ਗੰਢ ਸੰਚਾਲਕ[](ਅਚਲ String& nodename) ਅਚਲ;

    File_ਗੰਢ ਸੰਚਾਲਕ[](ਅਚਲ ਅੱਖਰ* /*nodename*/) ਅਚਲ;

    ਆਭਾਸੀ ਪੂਰਨ getFormat() ਅਚਲ CV_OVERRIDE;

    ਅੱਖਰ* ਬਫਰ_Ptr() ਅਚਲ;
    ਅੱਖਰ* ਬਫਰ_ਸ਼ੁਰੂ() ਅਚਲ;
    ਅੱਖਰ* ਬਫਰ_ਸਿਰਾ_2() ਅਚਲ;
    ਖਾਲੀ ਸੈੱਟ_ਬਫਰ_Ptr(ਅੱਖਰ* ptr);
    ਪੂਰਨ wrapMargin() ਅਚਲ;

    FStruct_ਅੰਕੜੇ& getCurrentStruct();

    ਖਾਲੀ ਸੈੱਟ_Non_ਖਾਲੀ_ਥਾਵਾਂ();

    ਖਾਲੀ processSpecialDouble( ਅੱਖਰ* buf, ਦੁੱਗਣਾ* value, ਅੱਖਰ** endptr );

    ਦੁੱਗਣਾ strtod( ਅੱਖਰ* ptr, ਅੱਖਰ** endptr );

    ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_Collection(ਪੂਰਨ type, File_ਗੰਢ& ਗੰਢ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ਗੰਢ_Space(File_ਗੰਢ& ਗੰਢ, size_t sz);

    ਬਿਨਾਨਿਸ਼ਾਨੀ getStringOfs( ਅਚਲ std::string& ਕੁੰਜੀ ) ਅਚਲ;

    File_ਗੰਢ ਜੋੜੋ_ਗੰਢ( File_ਗੰਢ& collection, ਅਚਲ std::string& ਕੁੰਜੀ,
                      ਪੂਰਨ elem_type, ਅਚਲ ਖਾਲੀ* value, ਪੂਰਨ len );

    ਖਾਲੀ finalizeCollection( File_ਗੰਢ& collection );

    ਖਾਲੀ normalize_ਗੰਢ_Ofs(size_t& ਖੰਡ_Idx, size_t& ofs) ਅਚਲ;

    Base64State get_state_ਦਾ_writing_base64();

    ਪੂਰਨ get_space();

    ਵਰਗ Base64Decoder
    {
    ਜਨਤਕ:
        Base64Decoder();
        ਖਾਲੀ init(ਅਚਲ Ptr<FileStorageParser>& _parser, ਅੱਖਰ* _ptr, ਪੂਰਨ _indent);

        ਤਾਰਕਿਕ read_ਹੋਰ(ਪੂਰਨ needed);

        uchar getUInt8();

        ushort getUInt16();

        ਪੂਰਨ getInt32();

        int64_t getInt64();

        ਦੁੱਗਣਾ getFloat64();

        ਤਾਰਕਿਕ ਸਿਰਾ_ਦਾ_ਧਾਰਾ() ਅਚਲ;
        ਅੱਖਰ* getPtr() ਅਚਲ;
    ਸੁਰੱਖਿਅਤ:

        Ptr<FileStorageParser> parser_do_not_use_ਸਿੱਧਾ_dereference;
        FileStorageParser& getParser() ਅਚਲ
        {
            ਜੇ (!parser_do_not_use_ਸਿੱਧਾ_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ਵਾਪਸ *parser_do_not_use_ਸਿੱਧਾ_dereference;
        }
        ਅੱਖਰ* ptr;
        ਪੂਰਨ indent;
        std::vector<ਅੱਖਰ> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        ਤਾਰਕਿਕ eos;
    };

    ਅੱਖਰ* parseBase64(ਅੱਖਰ* ptr, ਪੂਰਨ indent, File_ਗੰਢ& collection);

    ਖਾਲੀ parseError( ਅਚਲ ਅੱਖਰ* func_name, ਅਚਲ std::string& err_ਸੁਨੇਹਾ, ਅਚਲ ਅੱਖਰ* ਸਰੋਤ_file, ਪੂਰਨ ਸਰੋਤ_ਸਤਰ );

    ਅਚਲ uchar* get_ਗੰਢ_Ptr(size_t ਖੰਡ_Idx, size_t ofs) ਅਚਲ;

    std::string getName( size_t nameofs ) ਅਚਲ;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    ਪੂਰਨ flags;
    ਤਾਰਕਿਕ ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ;

    FILE* file;
    gzFile gzfile;

    ਤਾਰਕਿਕ is_opened;
    ਤਾਰਕਿਕ dummy_eof;
    ਤਾਰਕਿਕ write_ਬਹੁਲਕ;
    ਤਾਰਕਿਕ mem_ਬਹੁਲਕ;
    ਪੂਰਨ fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    ਤਾਰਕਿਕ is_using_base64;
    ਤਾਰਕਿਕ is_write_struct_delayed;
    ਅੱਖਰ* delayed_struct_ਕੁੰਜੀ;
    ਪੂਰਨ   delayed_struct_flags;
    ਅੱਖਰ* delayed_type_name;
    FileStorage_API::Base64State state_ਦਾ_writing_base64;

    ਪੂਰਨ space, wrap_margin;
    std::deque<FStruct_ਅੰਕੜੇ> write_ਢੇਰ;
    std::vector<ਅੱਖਰ> ਬਫਰ;
    size_t bufofs;

    std::deque<ਅੱਖਰ> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_ਸਿੱਧਾ_dereference;
    FileStorageEmitter& getEmitter()
    {
        ਜੇ (!emitter_do_not_use_ਸਿੱਧਾ_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ਵਾਪਸ *emitter_do_not_use_ਸਿੱਧਾ_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_ਸਿੱਧਾ_dereference;
    FileStorageParser& getParser() ਅਚਲ
    {
        ਜੇ (!parser_do_not_use_ਸਿੱਧਾ_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ਵਾਪਸ *parser_do_not_use_ਸਿੱਧਾ_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ਗੰਢ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_ਅੰਕੜੇ;
    std::vector<uchar*> fs_ਅੰਕੜੇ_ptrs;
    std::vector<size_t> fs_ਅੰਕੜੇ_blksz;
    size_t freeSpaceOfs;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::unordered_map<std::string, ਬਿਨਾਨਿਸ਼ਾਨੀ> str_hash_t;
    str_hash_t str_hash;
    std::vector<ਅੱਖਰ> str_hash_ਅੰਕੜੇ;

    std::vector<ਅੱਖਰ> strbufv;
    ਅੱਖਰ* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    ਪੂਰਨ lineno;
};

}

#ਜੇ_ਅੰਤ
