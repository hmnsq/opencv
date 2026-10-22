// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ಅನ್ತರ್ಭಾವ "persistence.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_base64_encoding.hpp"
#ಅನ್ತರ್ಭಾವ <unordered_map>
#ಅನ್ತರ್ಭಾವ <iterator>


ನಾಮಾಕಾಶ cv
{

ಗಣನಾ Base64State{
    Uncertain,
    NotUse,
    InUse,
};

ವರ್ಗ cv::FileStorage::Impl : ಸಾರ್ವಜನಿಕ FileStorage_API
{
ಸಾರ್ವಜನಿಕ:
    ಶೂನ್ಯ init();

    Impl(FileStorage* _fs);

    ಆಭಾಸ ~Impl();

    ಶೂನ್ಯ release(String* out=0);

    ಶೂನ್ಯ analyze_file_name( ಧ್ರುವ std::string& file_name, std::vector<std::string>& params );

    ತಾರ್ಕಿಕ open( ಧ್ರುವ ವರ್ಣ* filename_or_buf, ಪೂರ್ಣಾಙ್ಕ _flags, ಧ್ರುವ ವರ್ಣ* encoding );

    ಶೂನ್ಯ puts( ಧ್ರುವ ವರ್ಣ* str );

    ವರ್ಣ* getsFromFile( ವರ್ಣ* buf, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 );

    ವರ್ಣ* gets( size_t max_ಎಣಿಕೆ );

    ವರ್ಣ* gets();

    ತಾರ್ಕಿಕ eof();

    ಶೂನ್ಯ setEof();

    ಶೂನ್ಯ closeFile();

    ಶೂನ್ಯ rewind();

    ವರ್ಣ* resizeWriteBuffer( ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ len );

    ವರ್ಣ* flush();

    ಶೂನ್ಯ endWriteStruct();

    ಶೂನ್ಯ startWriteStruct_helper( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags,
                                  ಧ್ರುವ ವರ್ಣ* type_name );

    ಶೂನ್ಯ startWriteStruct( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags,
                           ಧ್ರುವ ವರ್ಣ* type_name );

    ಶೂನ್ಯ writeComment( ಧ್ರುವ ವರ್ಣ* comment, ತಾರ್ಕಿಕ eol_comment );

    ಶೂನ್ಯ startNextStream();

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ String& ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ value );

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ String& ಕೀಲಿ, ತಾರ್ಕಿಕ value );

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ String& ಕೀಲಿ, int64_t value );

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ String& ಕೀಲಿ, ದ್ವಿಗುಣ value );

    ಶೂನ್ಯ ಲಿಖ( ಧ್ರುವ String& ಕೀಲಿ, ಧ್ರುವ String& value );

    ಶೂನ್ಯ writeRawData( ಧ್ರುವ std::string& dt, ಧ್ರುವ ಶೂನ್ಯ* _data, size_t len );

    ಶೂನ್ಯ workaround();

    ಶೂನ್ಯ switch_to_Base64_state( FileStorage_API::Base64State new_state);

    ಶೂನ್ಯ make_write_struct_delayed( ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ* type_name );

    ಶೂನ್ಯ check_if_write_struct_is_delayed( ತಾರ್ಕಿಕ change_type_to_base64 );

    ಶೂನ್ಯ writeRawDataBase64(ಧ್ರುವ ಶೂನ್ಯ* _data, size_t len, ಧ್ರುವ ವರ್ಣ* dt );

    String releaseAndGetString();

    File_ಗಂಟು get_First_ತುದಿ_Level_ಗಂಟು() ಧ್ರುವ;

    File_ಗಂಟು ಬೇರು(ಪೂರ್ಣಾಙ್ಕ streamIdx=0) ಧ್ರುವ;

    File_ಗಂಟು operator[](ಧ್ರುವ String& nodename) ಧ್ರುವ;

    File_ಗಂಟು operator[](ಧ್ರುವ ವರ್ಣ* /*nodename*/) ಧ್ರುವ;

    ಆಭಾಸ ಪೂರ್ಣಾಙ್ಕ getFormat() ಧ್ರುವ CV_OVERRIDE;

    ವರ್ಣ* bufferPtr() ಧ್ರುವ;
    ವರ್ಣ* bufferStart() ಧ್ರುವ;
    ವರ್ಣ* bufferEnd() ಧ್ರುವ;
    ಶೂನ್ಯ setBufferPtr(ವರ್ಣ* ptr);
    ಪೂರ್ಣಾಙ್ಕ wrapMargin() ಧ್ರುವ;

    FStructData& getCurrentStruct();

    ಶೂನ್ಯ setNonEmpty();

    ಶೂನ್ಯ processSpecialDouble( ವರ್ಣ* buf, ದ್ವಿಗುಣ* value, ವರ್ಣ** endptr );

    ದ್ವಿಗುಣ strtod( ವರ್ಣ* ptr, ವರ್ಣ** endptr );

    ಶೂನ್ಯ convertToCollection(ಪೂರ್ಣಾಙ್ಕ type, File_ಗಂಟು& ಗಂಟು);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ಗಂಟು_Space(File_ಗಂಟು& ಗಂಟು, size_t sz);

    unsigned getStringOfs( ಧ್ರುವ std::string& ಕೀಲಿ ) ಧ್ರುವ;

    File_ಗಂಟು add_ಗಂಟು( File_ಗಂಟು& collection, ಧ್ರುವ std::string& ಕೀಲಿ,
                      ಪೂರ್ಣಾಙ್ಕ elem_type, ಧ್ರುವ ಶೂನ್ಯ* value, ಪೂರ್ಣಾಙ್ಕ len );

    ಶೂನ್ಯ finalizeCollection( File_ಗಂಟು& collection );

    ಶೂನ್ಯ normalize_ಗಂಟು_Ofs(size_t& blockIdx, size_t& ofs) ಧ್ರುವ;

    Base64State get_state_of_writing_base64();

    ಪೂರ್ಣಾಙ್ಕ get_space();

    ವರ್ಗ Base64Decoder
    {
    ಸಾರ್ವಜನಿಕ:
        Base64Decoder();
        ಶೂನ್ಯ init(ಧ್ರುವ Ptr<FileStorageParser>& _parser, ವರ್ಣ* _ptr, ಪೂರ್ಣಾಙ್ಕ _indent);

        ತಾರ್ಕಿಕ readMore(ಪೂರ್ಣಾಙ್ಕ needed);

        uchar getUInt8();

        ushort getUInt16();

        ಪೂರ್ಣಾಙ್ಕ getInt32();

        int64_t getInt64();

        ದ್ವಿಗುಣ getFloat64();

        ತಾರ್ಕಿಕ endOfStream() ಧ್ರುವ;
        ವರ್ಣ* getPtr() ಧ್ರುವ;
    ರಕ್ಷಿತ:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ಧ್ರುವ
        {
            ಯದಿ (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ಪ್ರತ್ಯಾಗಮ *parser_do_not_use_direct_dereference;
        }
        ವರ್ಣ* ptr;
        ಪೂರ್ಣಾಙ್ಕ indent;
        std::vector<ವರ್ಣ> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        ತಾರ್ಕಿಕ eos;
    };

    ವರ್ಣ* parseBase64(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ indent, File_ಗಂಟು& collection);

    ಶೂನ್ಯ parseError( ಧ್ರುವ ವರ್ಣ* func_name, ಧ್ರುವ std::string& err_ಸಂದೇಶ, ಧ್ರುವ ವರ್ಣ* source_file, ಪೂರ್ಣಾಙ್ಕ source_ಸಾಲು );

    ಧ್ರುವ uchar* get_ಗಂಟು_Ptr(size_t blockIdx, size_t ofs) ಧ್ರುವ;

    std::string getName( size_t nameofs ) ಧ್ರುವ;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    ಪೂರ್ಣಾಙ್ಕ flags;
    ತಾರ್ಕಿಕ empty_stream;

    FILE* file;
    gzFile gzfile;

    ತಾರ್ಕಿಕ is_opened;
    ತಾರ್ಕಿಕ dummy_eof;
    ತಾರ್ಕಿಕ write_mode;
    ತಾರ್ಕಿಕ mem_mode;
    ಪೂರ್ಣಾಙ್ಕ fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    ತಾರ್ಕಿಕ is_using_base64;
    ತಾರ್ಕಿಕ is_write_struct_delayed;
    ವರ್ಣ* delayed_struct_ಕೀಲಿ;
    ಪೂರ್ಣಾಙ್ಕ   delayed_struct_flags;
    ವರ್ಣ* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    ಪೂರ್ಣಾಙ್ಕ space, wrap_margin;
    std::deque<FStructData> write_ರಾಶಿ;
    std::vector<ವರ್ಣ> buffer;
    size_t bufofs;

    std::deque<ವರ್ಣ> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        ಯದಿ (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ಪ್ರತ್ಯಾಗಮ *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ಧ್ರುವ
    {
        ಯದಿ (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ಪ್ರತ್ಯಾಗಮ *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ಗಂಟು> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    ಪ್ರಕಾರನಿರ್ದೇಶ std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<ವರ್ಣ> str_hash_data;

    std::vector<ವರ್ಣ> strbufv;
    ವರ್ಣ* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    ಪೂರ್ಣಾಙ್ಕ lineno;
};

}

#endif
