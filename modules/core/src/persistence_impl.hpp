// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ضمن "persistence.hpp"
#ضمن "persistence_base64_encoding.hpp"
#ضمن <unordered_map>
#ضمن <iterator>


نطاق cv
{

تعداد Base64State{
    Uncertain,
    NotUse,
    InUse,
};

صنف cv::FileStorage::Impl : عام FileStorage_API
{
عام:
    فراغ init();

    Impl(FileStorage* _fs);

    virtual ~Impl();

    فراغ release(String* out=0);

    فراغ analyze_file_name( ثابت std::string& file_name, std::vector<std::string>& params );

    منطقي open( ثابت حرف* filename_or_buf, صحيح _flags, ثابت حرف* encoding );

    فراغ puts( ثابت حرف* str );

    حرف* getsFromFile( حرف* buf, صحيح تعداد_3 );

    حرف* gets( size_t max_تعداد );

    حرف* gets();

    منطقي eof();

    فراغ setEof();

    فراغ closeFile();

    فراغ rewind();

    حرف* resizeWriteBuffer( حرف* ptr, صحيح len );

    حرف* flush();

    فراغ endWriteStruct();

    فراغ startWriteStruct_helper( ثابت حرف* مفتاح, صحيح struct_flags,
                                  ثابت حرف* type_name );

    فراغ startWriteStruct( ثابت حرف* مفتاح, صحيح struct_flags,
                           ثابت حرف* type_name );

    فراغ writeComment( ثابت حرف* comment, منطقي eol_comment );

    فراغ startNextStream();

    فراغ اكتب( ثابت String& مفتاح, صحيح value );

    فراغ اكتب( ثابت String& مفتاح, منطقي value );

    فراغ اكتب( ثابت String& مفتاح, int64_t value );

    فراغ اكتب( ثابت String& مفتاح, مزدوج value );

    فراغ اكتب( ثابت String& مفتاح, ثابت String& value );

    فراغ writeRawData( ثابت std::string& dt, ثابت فراغ* _data, size_t len );

    فراغ workaround();

    فراغ switch_to_Base64_state( FileStorage_API::Base64State new_state);

    فراغ make_write_struct_delayed( ثابت حرف* مفتاح, صحيح struct_flags, ثابت حرف* type_name );

    فراغ check_if_write_struct_is_delayed( منطقي change_type_to_base64 );

    فراغ writeRawDataBase64(ثابت فراغ* _data, size_t len, ثابت حرف* dt );

    String releaseAndGetString();

    File_عقدة get_First_قمة_Level_عقدة() ثابت;

    File_عقدة جذر(صحيح streamIdx=0) ثابت;

    File_عقدة operator[](ثابت String& nodename) ثابت;

    File_عقدة operator[](ثابت حرف* /*nodename*/) ثابت;

    virtual صحيح getFormat() ثابت CV_OVERRIDE;

    حرف* bufferPtr() ثابت;
    حرف* bufferStart() ثابت;
    حرف* bufferEnd() ثابت;
    فراغ setBufferPtr(حرف* ptr);
    صحيح wrapMargin() ثابت;

    FStructData& getCurrentStruct();

    فراغ setNonEmpty();

    فراغ processSpecialDouble( حرف* buf, مزدوج* value, حرف** endptr );

    مزدوج strtod( حرف* ptr, حرف** endptr );

    فراغ convertToCollection(صحيح type, File_عقدة& عقدة);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_عقدة_Space(File_عقدة& عقدة, size_t sz);

    unsigned getStringOfs( ثابت std::string& مفتاح ) ثابت;

    File_عقدة add_عقدة( File_عقدة& collection, ثابت std::string& مفتاح,
                      صحيح elem_type, ثابت فراغ* value, صحيح len );

    فراغ finalizeCollection( File_عقدة& collection );

    فراغ normalize_عقدة_Ofs(size_t& blockIdx, size_t& ofs) ثابت;

    Base64State get_state_of_writing_base64();

    صحيح get_space();

    صنف Base64Decoder
    {
    عام:
        Base64Decoder();
        فراغ init(ثابت Ptr<FileStorageParser>& _parser, حرف* _ptr, صحيح _indent);

        منطقي readMore(صحيح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحيح getInt32();

        int64_t getInt64();

        مزدوج getFloat64();

        منطقي endOfStream() ثابت;
        حرف* getPtr() ثابت;
    محمي:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ثابت
        {
            إذا (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            إرجاع *parser_do_not_use_direct_dereference;
        }
        حرف* ptr;
        صحيح indent;
        std::vector<حرف> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقي eos;
    };

    حرف* parseBase64(حرف* ptr, صحيح indent, File_عقدة& collection);

    فراغ parseError( ثابت حرف* func_name, ثابت std::string& err_رسالة, ثابت حرف* source_file, صحيح source_سطر );

    ثابت uchar* get_عقدة_Ptr(size_t blockIdx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحيح flags;
    منطقي empty_stream;

    FILE* file;
    gzFile gzfile;

    منطقي is_opened;
    منطقي dummy_eof;
    منطقي write_mode;
    منطقي mem_mode;
    صحيح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقي is_using_base64;
    منطقي is_write_struct_delayed;
    حرف* delayed_struct_مفتاح;
    صحيح   delayed_struct_flags;
    حرف* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    صحيح space, wrap_margin;
    std::deque<FStructData> write_مكدس;
    std::vector<حرف> buffer;
    size_t bufofs;

    std::deque<حرف> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        إذا (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        إرجاع *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ثابت
    {
        إذا (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        إرجاع *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_عقدة> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    تعريفنوع std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<حرف> str_hash_data;

    std::vector<حرف> strbufv;
    حرف* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحيح lineno;
};

}

#endif
