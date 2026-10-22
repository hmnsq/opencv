// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ଅନ୍ତର୍ଭାଵ "persistence.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence_base64_encoding.hpp"
#ଅନ୍ତର୍ଭାଵ <unordered_map>
#ଅନ୍ତର୍ଭାଵ <iterator>


ନାମାକାଶ cv
{

ଗଣନା Base64State{
    Uncertain,
    NotUse,
    InUse,
};

ଶ୍ରେଣୀ cv::FileStorage::Impl : ସର୍ବସାଧାରଣ FileStorage_API
{
ସର୍ବସାଧାରଣ:
    ଶୂନ୍ୟ init();

    Impl(FileStorage* _fs);

    ଆଭାସୀ ~Impl();

    ଶୂନ୍ୟ release(String* out=0);

    ଶୂନ୍ୟ analyze_file_name( ଅଚଳ std::string& file_name, std::vector<std::string>& params );

    ତାର୍କିକ open( ଅଚଳ ଅକ୍ଷର* filename_or_buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _flags, ଅଚଳ ଅକ୍ଷର* encoding );

    ଶୂନ୍ୟ puts( ଅଚଳ ଅକ୍ଷର* str );

    ଅକ୍ଷର* getsFromFile( ଅକ୍ଷର* buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 );

    ଅକ୍ଷର* gets( size_t max_ଗଣନା );

    ଅକ୍ଷର* gets();

    ତାର୍କିକ eof();

    ଶୂନ୍ୟ setEof();

    ଶୂନ୍ୟ closeFile();

    ଶୂନ୍ୟ rewind();

    ଅକ୍ଷର* resizeWriteBuffer( ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len );

    ଅକ୍ଷର* flush();

    ଶୂନ୍ୟ endWriteStruct();

    ଶୂନ୍ୟ startWriteStruct_helper( ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags,
                                  ଅଚଳ ଅକ୍ଷର* type_name );

    ଶୂନ୍ୟ startWriteStruct( ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags,
                           ଅଚଳ ଅକ୍ଷର* type_name );

    ଶୂନ୍ୟ writeComment( ଅଚଳ ଅକ୍ଷର* comment, ତାର୍କିକ eol_comment );

    ଶୂନ୍ୟ startNextStream();

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ String& ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value );

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ String& ଚାବି, ତାର୍କିକ value );

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ String& ଚାବି, int64_t value );

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ String& ଚାବି, ଦ୍ୱିଗୁଣ value );

    ଶୂନ୍ୟ ଲିଖ( ଅଚଳ String& ଚାବି, ଅଚଳ String& value );

    ଶୂନ୍ୟ writeRawData( ଅଚଳ std::string& dt, ଅଚଳ ଶୂନ୍ୟ* _data, size_t len );

    ଶୂନ୍ୟ workaround();

    ଶୂନ୍ୟ switch_to_Base64_state( FileStorage_API::Base64State new_state);

    ଶୂନ୍ୟ make_write_struct_delayed( ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର* type_name );

    ଶୂନ୍ୟ check_if_write_struct_is_delayed( ତାର୍କିକ change_type_to_base64 );

    ଶୂନ୍ୟ writeRawDataBase64(ଅଚଳ ଶୂନ୍ୟ* _data, size_t len, ଅଚଳ ଅକ୍ଷର* dt );

    String releaseAndGetString();

    File_ଗଣ୍ଠି get_First_ଶୀର୍ଷ_Level_ଗଣ୍ଠି() ଅଚଳ;

    File_ଗଣ୍ଠି ମୂଳ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା streamIdx=0) ଅଚଳ;

    File_ଗଣ୍ଠି operator[](ଅଚଳ String& nodename) ଅଚଳ;

    File_ଗଣ୍ଠି operator[](ଅଚଳ ଅକ୍ଷର* /*nodename*/) ଅଚଳ;

    ଆଭାସୀ ପୂର୍ଣ୍ଣସଂଖ୍ୟା getFormat() ଅଚଳ CV_OVERRIDE;

    ଅକ୍ଷର* bufferPtr() ଅଚଳ;
    ଅକ୍ଷର* bufferStart() ଅଚଳ;
    ଅକ୍ଷର* bufferEnd() ଅଚଳ;
    ଶୂନ୍ୟ setBufferPtr(ଅକ୍ଷର* ptr);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wrapMargin() ଅଚଳ;

    FStructData& getCurrentStruct();

    ଶୂନ୍ୟ setNonEmpty();

    ଶୂନ୍ୟ processSpecialDouble( ଅକ୍ଷର* buf, ଦ୍ୱିଗୁଣ* value, ଅକ୍ଷର** endptr );

    ଦ୍ୱିଗୁଣ strtod( ଅକ୍ଷର* ptr, ଅକ୍ଷର** endptr );

    ଶୂନ୍ୟ convertToCollection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, File_ଗଣ୍ଠି& ଗଣ୍ଠି);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ଗଣ୍ଠି_Space(File_ଗଣ୍ଠି& ଗଣ୍ଠି, size_t sz);

    unsigned getStringOfs( ଅଚଳ std::string& ଚାବି ) ଅଚଳ;

    File_ଗଣ୍ଠି add_ଗଣ୍ଠି( File_ଗଣ୍ଠି& collection, ଅଚଳ std::string& ଚାବି,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅଚଳ ଶୂନ୍ୟ* value, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len );

    ଶୂନ୍ୟ finalizeCollection( File_ଗଣ୍ଠି& collection );

    ଶୂନ୍ୟ normalize_ଗଣ୍ଠି_Ofs(size_t& blockIdx, size_t& ofs) ଅଚଳ;

    Base64State get_state_of_writing_base64();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_space();

    ଶ୍ରେଣୀ Base64Decoder
    {
    ସର୍ବସାଧାରଣ:
        Base64Decoder();
        ଶୂନ୍ୟ init(ଅଚଳ Ptr<FileStorageParser>& _parser, ଅକ୍ଷର* _ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _indent);

        ତାର୍କିକ readMore(ପୂର୍ଣ୍ଣସଂଖ୍ୟା needed);

        uchar getUInt8();

        ushort getUInt16();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା getInt32();

        int64_t getInt64();

        ଦ୍ୱିଗୁଣ getFloat64();

        ତାର୍କିକ endOfStream() ଅଚଳ;
        ଅକ୍ଷର* getPtr() ଅଚଳ;
    ସୁରକ୍ଷିତ:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ଅଚଳ
        {
            ଯଦି (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ଫେରନ୍ତୁ *parser_do_not_use_direct_dereference;
        }
        ଅକ୍ଷର* ptr;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent;
        std::vector<ଅକ୍ଷର> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        ତାର୍କିକ eos;
    };

    ଅକ୍ଷର* parseBase64(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, File_ଗଣ୍ଠି& collection);

    ଶୂନ୍ୟ parseError( ଅଚଳ ଅକ୍ଷର* func_name, ଅଚଳ std::string& err_ସନ୍ଦେଶ, ଅଚଳ ଅକ୍ଷର* source_file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_ଧାଡ଼ି );

    ଅଚଳ uchar* get_ଗଣ୍ଠି_Ptr(size_t blockIdx, size_t ofs) ଅଚଳ;

    std::string getName( size_t nameofs ) ଅଚଳ;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
    ତାର୍କିକ empty_stream;

    FILE* file;
    gzFile gzfile;

    ତାର୍କିକ is_opened;
    ତାର୍କିକ dummy_eof;
    ତାର୍କିକ write_mode;
    ତାର୍କିକ mem_mode;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    ତାର୍କିକ is_using_base64;
    ତାର୍କିକ is_write_struct_delayed;
    ଅକ୍ଷର* delayed_struct_ଚାବି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା   delayed_struct_flags;
    ଅକ୍ଷର* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା space, wrap_margin;
    std::deque<FStructData> write_ଗଦା;
    std::vector<ଅକ୍ଷର> buffer;
    size_t bufofs;

    std::deque<ଅକ୍ଷର> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        ଯଦି (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ଫେରନ୍ତୁ *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ଅଚଳ
    {
        ଯଦି (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ଫେରନ୍ତୁ *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ଗଣ୍ଠି> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<ଅକ୍ଷର> str_hash_data;

    std::vector<ଅକ୍ଷର> strbufv;
    ଅକ୍ଷର* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lineno;
};

}

#endif
