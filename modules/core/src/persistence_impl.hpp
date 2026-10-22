// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_PERSISTENCE_IMPL_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ಅನ್ತರ್ಭಾವ "persistence.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_base64_encoding.hpp"
#ಅನ್ತರ್ಭಾವ <unordered_map>
#ಅನ್ತರ್ಭಾವ <iterator>


ನಾಮಾಕಾಶ cv
{

ಎಣಿಕೆ Base64State{
    Uncertain,
    NotUse,
    ಒಳ_Use,
};

ವರ್ಗ cv::FileStorage::Impl : ಸಾರ್ವಜನಿಕ FileStorage_API
{
ಸಾರ್ವಜನಿಕ:
    ಶೂನ್ಯ init();

    Impl(FileStorage* _fs);

    ವಾಸ್ತವಿಕ ~Impl();

    ಶೂನ್ಯ release(String* out=0);

    ಶೂನ್ಯ analyze_file_name( ಸ್ಥಿರಾಂಕ std::string& file_name, std::vector<std::string>& params );

    ತಾರ್ಕಿಕ open( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* filename_or_buf, ಪೂರ್ಣಾಂಕ _flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* encoding );

    ಶೂನ್ಯ puts( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str );

    ಅಕ್ಷರ* getsFromFile( ಅಕ್ಷರ* buf, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 );

    ಅಕ್ಷರ* gets( size_t max_ಎಣಿಕೆ );

    ಅಕ್ಷರ* gets();

    ತಾರ್ಕಿಕ eof();

    ಶೂನ್ಯ ಗಣ_Eof();

    ಶೂನ್ಯ closeFile();

    ಶೂನ್ಯ rewind();

    ಅಕ್ಷರ* resize_Write_ಬಫರ್( ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ len );

    ಅಕ್ಷರ* flush();

    ಶೂನ್ಯ ಕೊನೆ_Write_Struct();

    ಶೂನ್ಯ ಆರಂಭ_Write_Struct_helper( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name );

    ಶೂನ್ಯ ಆರಂಭ_Write_Struct( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags,
                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name );

    ಶೂನ್ಯ writeComment( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* comment, ತಾರ್ಕಿಕ eol_comment );

    ಶೂನ್ಯ ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು();

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value );

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ತಾರ್ಕಿಕ value );

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, int64_t value );

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ದ್ವಿಗುಣ value );

    ಶೂನ್ಯ ಲಿಖ( ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ಸ್ಥಿರಾಂಕ String& value );

    ಶೂನ್ಯ write_Raw_ದತ್ತಾಂಶ( ಸ್ಥಿರಾಂಕ std::string& dt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t len );

    ಶೂನ್ಯ workaround();

    ಶೂನ್ಯ switch_ಗಮ್ಯ_Base64_state( FileStorage_API::Base64State new_state);

    ಶೂನ್ಯ make_write_struct_delayed( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name );

    ಶೂನ್ಯ check_if_write_struct_is_delayed( ತಾರ್ಕಿಕ change_type_ಗಮ್ಯ_base64 );

    ಶೂನ್ಯ write_Raw_ದತ್ತಾಂಶ_Base64(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt );

    String releaseAndGetString();

    File_ಗಂಟು get_ಮೊದಲ_ತುದಿ_Level_ಗಂಟು() ಸ್ಥಿರಾಂಕ;

    File_ಗಂಟು ಬೇರು(ಪೂರ್ಣಾಂಕ ಹರಿವು_Idx=0) ಸ್ಥಿರಾಂಕ;

    File_ಗಂಟು ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ String& nodename) ಸ್ಥಿರಾಂಕ;

    File_ಗಂಟು ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* /*nodename*/) ಸ್ಥಿರಾಂಕ;

    ವಾಸ್ತವಿಕ ಪೂರ್ಣಾಂಕ getFormat() ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಅಕ್ಷರ* ಬಫರ್_Ptr() ಸ್ಥಿರಾಂಕ;
    ಅಕ್ಷರ* ಬಫರ್_ಆರಂಭ() ಸ್ಥಿರಾಂಕ;
    ಅಕ್ಷರ* ಬಫರ್_ಕೊನೆ_2() ಸ್ಥಿರಾಂಕ;
    ಶೂನ್ಯ ಗಣ_ಬಫರ್_Ptr(ಅಕ್ಷರ* ptr);
    ಪೂರ್ಣಾಂಕ wrapMargin() ಸ್ಥಿರಾಂಕ;

    FStruct_ದತ್ತಾಂಶ& getCurrentStruct();

    ಶೂನ್ಯ ಗಣ_Non_ಖಾಲಿಗಳು();

    ಶೂನ್ಯ processSpecialDouble( ಅಕ್ಷರ* buf, ದ್ವಿಗುಣ* value, ಅಕ್ಷರ** endptr );

    ದ್ವಿಗುಣ strtod( ಅಕ್ಷರ* ptr, ಅಕ್ಷರ** endptr );

    ಶೂನ್ಯ convert_ಗಮ್ಯ_Collection(ಪೂರ್ಣಾಂಕ type, File_ಗಂಟು& ಗಂಟು);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ಗಂಟು_Space(File_ಗಂಟು& ಗಂಟು, size_t sz);

    ಚಿಹ್ನೆರಹಿತ getStringOfs( ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ ) ಸ್ಥಿರಾಂಕ;

    File_ಗಂಟು ಕೂಡಿಸು_ಗಂಟು( File_ಗಂಟು& collection, ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ,
                      ಪೂರ್ಣಾಂಕ elem_type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* value, ಪೂರ್ಣಾಂಕ len );

    ಶೂನ್ಯ finalizeCollection( File_ಗಂಟು& collection );

    ಶೂನ್ಯ normalize_ಗಂಟು_Ofs(size_t& ಖಂಡ_Idx, size_t& ofs) ಸ್ಥಿರಾಂಕ;

    Base64State get_state_ದ_writing_base64();

    ಪೂರ್ಣಾಂಕ get_space();

    ವರ್ಗ Base64Decoder
    {
    ಸಾರ್ವಜನಿಕ:
        Base64Decoder();
        ಶೂನ್ಯ init(ಸ್ಥಿರಾಂಕ Ptr<FileStorageParser>& _parser, ಅಕ್ಷರ* _ptr, ಪೂರ್ಣಾಂಕ _indent);

        ತಾರ್ಕಿಕ read_ಇನ್ನಷ್ಟು(ಪೂರ್ಣಾಂಕ needed);

        uchar getUInt8();

        ushort getUInt16();

        ಪೂರ್ಣಾಂಕ getInt32();

        int64_t getInt64();

        ದ್ವಿಗುಣ getFloat64();

        ತಾರ್ಕಿಕ ಕೊನೆ_ದ_ಹರಿವು() ಸ್ಥಿರಾಂಕ;
        ಅಕ್ಷರ* getPtr() ಸ್ಥಿರಾಂಕ;
    ಸಂರಕ್ಷಿತ:

        Ptr<FileStorageParser> parser_do_not_use_ನೇರ_dereference;
        FileStorageParser& getParser() ಸ್ಥಿರಾಂಕ
        {
            ಆದರೆ (!parser_do_not_use_ನೇರ_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ಹಿಂತಿರುಗಿ *parser_do_not_use_ನೇರ_dereference;
        }
        ಅಕ್ಷರ* ptr;
        ಪೂರ್ಣಾಂಕ indent;
        std::vector<ಅಕ್ಷರ> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        ತಾರ್ಕಿಕ eos;
    };

    ಅಕ್ಷರ* parseBase64(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ indent, File_ಗಂಟು& collection);

    ಶೂನ್ಯ parseError( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* func_name, ಸ್ಥಿರಾಂಕ std::string& err_ಸಂದೇಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮೂಲ_file, ಪೂರ್ಣಾಂಕ ಮೂಲ_ಸಾಲು );

    ಸ್ಥಿರಾಂಕ uchar* get_ಗಂಟು_Ptr(size_t ಖಂಡ_Idx, size_t ofs) ಸ್ಥಿರಾಂಕ;

    std::string getName( size_t nameofs ) ಸ್ಥಿರಾಂಕ;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    ಪೂರ್ಣಾಂಕ flags;
    ತಾರ್ಕಿಕ ಖಾಲಿಗಳು_ಹರಿವು;

    FILE* file;
    gzFile gzfile;

    ತಾರ್ಕಿಕ is_opened;
    ತಾರ್ಕಿಕ dummy_eof;
    ತಾರ್ಕಿಕ write_ಬಹುಲಕ;
    ತಾರ್ಕಿಕ mem_ಬಹುಲಕ;
    ಪೂರ್ಣಾಂಕ fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    ತಾರ್ಕಿಕ is_using_base64;
    ತಾರ್ಕಿಕ is_write_struct_delayed;
    ಅಕ್ಷರ* delayed_struct_ಕೀಲಿ;
    ಪೂರ್ಣಾಂಕ   delayed_struct_flags;
    ಅಕ್ಷರ* delayed_type_name;
    FileStorage_API::Base64State state_ದ_writing_base64;

    ಪೂರ್ಣಾಂಕ space, wrap_margin;
    std::deque<FStruct_ದತ್ತಾಂಶ> write_ರಾಶಿ;
    std::vector<ಅಕ್ಷರ> ಬಫರ್;
    size_t bufofs;

    std::deque<ಅಕ್ಷರ> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_ನೇರ_dereference;
    FileStorageEmitter& getEmitter()
    {
        ಆದರೆ (!emitter_do_not_use_ನೇರ_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ಹಿಂತಿರುಗಿ *emitter_do_not_use_ನೇರ_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_ನೇರ_dereference;
    FileStorageParser& getParser() ಸ್ಥಿರಾಂಕ
    {
        ಆದರೆ (!parser_do_not_use_ನೇರ_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ಹಿಂತಿರುಗಿ *parser_do_not_use_ನೇರ_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ಗಂಟು> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_ದತ್ತಾಂಶ;
    std::vector<uchar*> fs_ದತ್ತಾಂಶ_ptrs;
    std::vector<size_t> fs_ದತ್ತಾಂಶ_blksz;
    size_t freeSpaceOfs;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::unordered_map<std::string, ಚಿಹ್ನೆರಹಿತ> str_hash_t;
    str_hash_t str_hash;
    std::vector<ಅಕ್ಷರ> str_hash_ದತ್ತಾಂಶ;

    std::vector<ಅಕ್ಷರ> strbufv;
    ಅಕ್ಷರ* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    ಪೂರ್ಣಾಂಕ lineno;
};

}

#ಆದರೆ_ಅಂತ್ಯ
