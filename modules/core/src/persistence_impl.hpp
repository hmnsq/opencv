// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#অন্তর্ভাব "persistence.hpp"
#অন্তর্ভাব "persistence_base64_encoding.hpp"
#অন্তর্ভাব <unordered_map>
#অন্তর্ভাব <iterator>


নামাকাশ cv
{

গণনা Base64State{
    Uncertain,
    NotUse,
    InUse,
};

বর্গ cv::FileStorage::Impl : সার্বজনিক FileStorage_API
{
সার্বজনিক:
    শূন্য init();

    Impl(FileStorage* _fs);

    আভাস ~Impl();

    শূন্য release(String* out=0);

    শূন্য analyze_file_name( ধ্রুব std::string& file_name, std::vector<std::string>& params );

    তার্কিক open( ধ্রুব বর্ণ* filename_or_buf, পূর্ণাঙ্ক _flags, ধ্রুব বর্ণ* encoding );

    শূন্য puts( ধ্রুব বর্ণ* str );

    বর্ণ* getsFromFile( বর্ণ* buf, পূর্ণাঙ্ক গুনতি_2 );

    বর্ণ* gets( size_t max_গুনতি );

    বর্ণ* gets();

    তার্কিক eof();

    শূন্য setEof();

    শূন্য closeFile();

    শূন্য rewind();

    বর্ণ* resizeWriteBuffer( বর্ণ* ptr, পূর্ণাঙ্ক len );

    বর্ণ* flush();

    শূন্য endWriteStruct();

    শূন্য startWriteStruct_helper( ধ্রুব বর্ণ* চাবি, পূর্ণাঙ্ক struct_flags,
                                  ধ্রুব বর্ণ* type_name );

    শূন্য startWriteStruct( ধ্রুব বর্ণ* চাবি, পূর্ণাঙ্ক struct_flags,
                           ধ্রুব বর্ণ* type_name );

    শূন্য writeComment( ধ্রুব বর্ণ* comment, তার্কিক eol_comment );

    শূন্য startNextStream();

    শূন্য লিখ( ধ্রুব String& চাবি, পূর্ণাঙ্ক value );

    শূন্য লিখ( ধ্রুব String& চাবি, তার্কিক value );

    শূন্য লিখ( ধ্রুব String& চাবি, int64_t value );

    শূন্য লিখ( ধ্রুব String& চাবি, দ্বিগুণ value );

    শূন্য লিখ( ধ্রুব String& চাবি, ধ্রুব String& value );

    শূন্য writeRawData( ধ্রুব std::string& dt, ধ্রুব শূন্য* _data, size_t len );

    শূন্য workaround();

    শূন্য switch_to_Base64_state( FileStorage_API::Base64State new_state);

    শূন্য make_write_struct_delayed( ধ্রুব বর্ণ* চাবি, পূর্ণাঙ্ক struct_flags, ধ্রুব বর্ণ* type_name );

    শূন্য check_if_write_struct_is_delayed( তার্কিক change_type_to_base64 );

    শূন্য writeRawDataBase64(ধ্রুব শূন্য* _data, size_t len, ধ্রুব বর্ণ* dt );

    String releaseAndGetString();

    File_গ্রন্থি get_First_শীর্ষ_Level_গ্রন্থি() ধ্রুব;

    File_গ্রন্থি মূল(পূর্ণাঙ্ক streamIdx=0) ধ্রুব;

    File_গ্রন্থি operator[](ধ্রুব String& nodename) ধ্রুব;

    File_গ্রন্থি operator[](ধ্রুব বর্ণ* /*nodename*/) ধ্রুব;

    আভাস পূর্ণাঙ্ক getFormat() ধ্রুব CV_OVERRIDE;

    বর্ণ* bufferPtr() ধ্রুব;
    বর্ণ* bufferStart() ধ্রুব;
    বর্ণ* bufferEnd() ধ্রুব;
    শূন্য setBufferPtr(বর্ণ* ptr);
    পূর্ণাঙ্ক wrapMargin() ধ্রুব;

    FStructData& getCurrentStruct();

    শূন্য setNonEmpty();

    শূন্য processSpecialDouble( বর্ণ* buf, দ্বিগুণ* value, বর্ণ** endptr );

    দ্বিগুণ strtod( বর্ণ* ptr, বর্ণ** endptr );

    শূন্য convertToCollection(পূর্ণাঙ্ক type, File_গ্রন্থি& গ্রন্থি);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_গ্রন্থি_Space(File_গ্রন্থি& গ্রন্থি, size_t sz);

    unsigned getStringOfs( ধ্রুব std::string& চাবি ) ধ্রুব;

    File_গ্রন্থি add_গ্রন্থি( File_গ্রন্থি& collection, ধ্রুব std::string& চাবি,
                      পূর্ণাঙ্ক elem_type, ধ্রুব শূন্য* value, পূর্ণাঙ্ক len );

    শূন্য finalizeCollection( File_গ্রন্থি& collection );

    শূন্য normalize_গ্রন্থি_Ofs(size_t& blockIdx, size_t& ofs) ধ্রুব;

    Base64State get_state_of_writing_base64();

    পূর্ণাঙ্ক get_space();

    বর্গ Base64Decoder
    {
    সার্বজনিক:
        Base64Decoder();
        শূন্য init(ধ্রুব Ptr<FileStorageParser>& _parser, বর্ণ* _ptr, পূর্ণাঙ্ক _indent);

        তার্কিক readMore(পূর্ণাঙ্ক needed);

        uchar getUInt8();

        ushort getUInt16();

        পূর্ণাঙ্ক getInt32();

        int64_t getInt64();

        দ্বিগুণ getFloat64();

        তার্কিক endOfStream() ধ্রুব;
        বর্ণ* getPtr() ধ্রুব;
    রক্ষিত:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ধ্রুব
        {
            যদি (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            প্রত্যাগম *parser_do_not_use_direct_dereference;
        }
        বর্ণ* ptr;
        পূর্ণাঙ্ক indent;
        std::vector<বর্ণ> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        তার্কিক eos;
    };

    বর্ণ* parseBase64(বর্ণ* ptr, পূর্ণাঙ্ক indent, File_গ্রন্থি& collection);

    শূন্য parseError( ধ্রুব বর্ণ* func_name, ধ্রুব std::string& err_বার্তা, ধ্রুব বর্ণ* source_file, পূর্ণাঙ্ক source_সারি );

    ধ্রুব uchar* get_গ্রন্থি_Ptr(size_t blockIdx, size_t ofs) ধ্রুব;

    std::string getName( size_t nameofs ) ধ্রুব;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    পূর্ণাঙ্ক flags;
    তার্কিক empty_stream;

    FILE* file;
    gzFile gzfile;

    তার্কিক is_opened;
    তার্কিক dummy_eof;
    তার্কিক write_mode;
    তার্কিক mem_mode;
    পূর্ণাঙ্ক fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    তার্কিক is_using_base64;
    তার্কিক is_write_struct_delayed;
    বর্ণ* delayed_struct_চাবি;
    পূর্ণাঙ্ক   delayed_struct_flags;
    বর্ণ* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    পূর্ণাঙ্ক space, wrap_margin;
    std::deque<FStructData> write_স্তূপ;
    std::vector<বর্ণ> buffer;
    size_t bufofs;

    std::deque<বর্ণ> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        যদি (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        প্রত্যাগম *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ধ্রুব
    {
        যদি (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        প্রত্যাগম *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_গ্রন্থি> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    প্রকারনির্দেশ std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<বর্ণ> str_hash_data;

    std::vector<বর্ণ> strbufv;
    বর্ণ* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    পূর্ণাঙ্ক lineno;
};

}

#endif
