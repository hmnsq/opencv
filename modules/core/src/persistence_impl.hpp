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

শ্রেণী cv::FileStorage::Impl : সর্বজনীন FileStorage_API
{
সর্বজনীন:
    শূন্য init();

    Impl(FileStorage* _fs);

    আভাসী ~Impl();

    শূন্য release(String* out=0);

    শূন্য analyze_file_name( ধ্রুবক std::string& file_name, std::vector<std::string>& params );

    যৌক্তিক open( ধ্রুবক অক্ষর* filename_or_buf, পূর্ণসংখ্যা _flags, ধ্রুবক অক্ষর* encoding );

    শূন্য puts( ধ্রুবক অক্ষর* str );

    অক্ষর* getsFromFile( অক্ষর* buf, পূর্ণসংখ্যা গুনতি_2 );

    অক্ষর* gets( size_t max_গুনতি );

    অক্ষর* gets();

    যৌক্তিক eof();

    শূন্য setEof();

    শূন্য closeFile();

    শূন্য rewind();

    অক্ষর* resize_Write_বাফার( অক্ষর* ptr, পূর্ণসংখ্যা len );

    অক্ষর* flush();

    শূন্য endWriteStruct();

    শূন্য startWriteStruct_helper( ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা struct_flags,
                                  ধ্রুবক অক্ষর* type_name );

    শূন্য startWriteStruct( ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা struct_flags,
                           ধ্রুবক অক্ষর* type_name );

    শূন্য writeComment( ধ্রুবক অক্ষর* comment, যৌক্তিক eol_comment );

    শূন্য start_পরবর্তী_Stream();

    শূন্য লিখ( ধ্রুবক String& চাবি, পূর্ণসংখ্যা value );

    শূন্য লিখ( ধ্রুবক String& চাবি, যৌক্তিক value );

    শূন্য লিখ( ধ্রুবক String& চাবি, int64_t value );

    শূন্য লিখ( ধ্রুবক String& চাবি, দ্বিগুণ value );

    শূন্য লিখ( ধ্রুবক String& চাবি, ধ্রুবক String& value );

    শূন্য write_Raw_তথ্য( ধ্রুবক std::string& dt, ধ্রুবক শূন্য* তথ্য_3, size_t len );

    শূন্য workaround();

    শূন্য switch_to_Base64_state( FileStorage_API::Base64State new_state);

    শূন্য make_write_struct_delayed( ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর* type_name );

    শূন্য check_if_write_struct_is_delayed( যৌক্তিক change_type_to_base64 );

    শূন্য write_Raw_তথ্য_Base64(ধ্রুবক শূন্য* তথ্য_3, size_t len, ধ্রুবক অক্ষর* dt );

    String releaseAndGetString();

    File_গ্রন্থি get_First_শীর্ষ_Level_গ্রন্থি() ধ্রুবক;

    File_গ্রন্থি মূল(পূর্ণসংখ্যা streamIdx=0) ধ্রুবক;

    File_গ্রন্থি operator[](ধ্রুবক String& nodename) ধ্রুবক;

    File_গ্রন্থি operator[](ধ্রুবক অক্ষর* /*nodename*/) ধ্রুবক;

    আভাসী পূর্ণসংখ্যা getFormat() ধ্রুবক CV_OVERRIDE;

    অক্ষর* বাফার_Ptr() ধ্রুবক;
    অক্ষর* বাফার_Start() ধ্রুবক;
    অক্ষর* বাফার_End() ধ্রুবক;
    শূন্য set_বাফার_Ptr(অক্ষর* ptr);
    পূর্ণসংখ্যা wrapMargin() ধ্রুবক;

    FStruct_তথ্য& getCurrentStruct();

    শূন্য setNonEmpty();

    শূন্য processSpecialDouble( অক্ষর* buf, দ্বিগুণ* value, অক্ষর** endptr );

    দ্বিগুণ strtod( অক্ষর* ptr, অক্ষর** endptr );

    শূন্য convertToCollection(পূর্ণসংখ্যা type, File_গ্রন্থি& গ্রন্থি);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_গ্রন্থি_Space(File_গ্রন্থি& গ্রন্থি, size_t sz);

    unsigned getStringOfs( ধ্রুবক std::string& চাবি ) ধ্রুবক;

    File_গ্রন্থি add_গ্রন্থি( File_গ্রন্থি& collection, ধ্রুবক std::string& চাবি,
                      পূর্ণসংখ্যা elem_type, ধ্রুবক শূন্য* value, পূর্ণসংখ্যা len );

    শূন্য finalizeCollection( File_গ্রন্থি& collection );

    শূন্য normalize_গ্রন্থি_Ofs(size_t& blockIdx, size_t& ofs) ধ্রুবক;

    Base64State get_state_of_writing_base64();

    পূর্ণসংখ্যা get_space();

    শ্রেণী Base64Decoder
    {
    সর্বজনীন:
        Base64Decoder();
        শূন্য init(ধ্রুবক Ptr<FileStorageParser>& _parser, অক্ষর* _ptr, পূর্ণসংখ্যা _indent);

        যৌক্তিক readMore(পূর্ণসংখ্যা needed);

        uchar getUInt8();

        ushort getUInt16();

        পূর্ণসংখ্যা getInt32();

        int64_t getInt64();

        দ্বিগুণ getFloat64();

        যৌক্তিক endOfStream() ধ্রুবক;
        অক্ষর* getPtr() ধ্রুবক;
    সুরক্ষিত:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ধ্রুবক
        {
            যদি (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ফেরত *parser_do_not_use_direct_dereference;
        }
        অক্ষর* ptr;
        পূর্ণসংখ্যা indent;
        std::vector<অক্ষর> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        যৌক্তিক eos;
    };

    অক্ষর* parseBase64(অক্ষর* ptr, পূর্ণসংখ্যা indent, File_গ্রন্থি& collection);

    শূন্য parseError( ধ্রুবক অক্ষর* func_name, ধ্রুবক std::string& err_বার্তা, ধ্রুবক অক্ষর* source_file, পূর্ণসংখ্যা source_সারি );

    ধ্রুবক uchar* get_গ্রন্থি_Ptr(size_t blockIdx, size_t ofs) ধ্রুবক;

    std::string getName( size_t nameofs ) ধ্রুবক;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    পূর্ণসংখ্যা flags;
    যৌক্তিক empty_stream;

    FILE* file;
    gzFile gzfile;

    যৌক্তিক is_opened;
    যৌক্তিক dummy_eof;
    যৌক্তিক write_mode;
    যৌক্তিক mem_mode;
    পূর্ণসংখ্যা fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    যৌক্তিক is_using_base64;
    যৌক্তিক is_write_struct_delayed;
    অক্ষর* delayed_struct_চাবি;
    পূর্ণসংখ্যা   delayed_struct_flags;
    অক্ষর* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    পূর্ণসংখ্যা space, wrap_margin;
    std::deque<FStruct_তথ্য> write_স্তূপ;
    std::vector<অক্ষর> বাফার;
    size_t bufofs;

    std::deque<অক্ষর> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        যদি (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ফেরত *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ধ্রুবক
    {
        যদি (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ফেরত *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_গ্রন্থি> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_তথ্য;
    std::vector<uchar*> fs_তথ্য_ptrs;
    std::vector<size_t> fs_তথ্য_blksz;
    size_t freeSpaceOfs;
    প্রকারলেখ std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<অক্ষর> str_hash_তথ্য;

    std::vector<অক্ষর> strbufv;
    অক্ষর* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    পূর্ণসংখ্যা lineno;
};

}

#endif
