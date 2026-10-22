// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#شامل "persistence.hpp"
#شامل "persistence_base64_encoding.hpp"
#شامل <unordered_map>
#شامل <iterator>


فضاینام cv
{

شمارش Base64State{
    Uncertain,
    NotUse,
    InUse,
};

رده cv::FileStorage::Impl : عمومی FileStorage_API
{
عمومی:
    تهی init();

    Impl(FileStorage* _fs);

    مجازی ~Impl();

    تهی release(String* out=0);

    تهی analyze_file_name( ثابت std::string& file_name, std::vector<std::string>& params );

    منطقی open( ثابت نویسه* filename_or_buf, صحیح _flags, ثابت نویسه* encoding );

    تهی puts( ثابت نویسه* str );

    نویسه* getsFromFile( نویسه* buf, صحیح شمار_2 );

    نویسه* gets( size_t max_شمار );

    نویسه* gets();

    منطقی eof();

    تهی setEof();

    تهی closeFile();

    تهی rewind();

    نویسه* resizeWriteBuffer( نویسه* ptr, صحیح len );

    نویسه* flush();

    تهی endWriteStruct();

    تهی startWriteStruct_helper( ثابت نویسه* کلید, صحیح struct_flags,
                                  ثابت نویسه* type_name );

    تهی startWriteStruct( ثابت نویسه* کلید, صحیح struct_flags,
                           ثابت نویسه* type_name );

    تهی writeComment( ثابت نویسه* comment, منطقی eol_comment );

    تهی startNextStream();

    تهی اكتب( ثابت String& کلید, صحیح value );

    تهی اكتب( ثابت String& کلید, منطقی value );

    تهی اكتب( ثابت String& کلید, int64_t value );

    تهی اكتب( ثابت String& کلید, دوگانه value );

    تهی اكتب( ثابت String& کلید, ثابت String& value );

    تهی writeRawData( ثابت std::string& dt, ثابت تهی* _data, size_t len );

    تهی workaround();

    تهی switch_to_Base64_state( FileStorage_API::Base64State new_state);

    تهی make_write_struct_delayed( ثابت نویسه* کلید, صحیح struct_flags, ثابت نویسه* type_name );

    تهی check_if_write_struct_is_delayed( منطقی change_type_to_base64 );

    تهی writeRawDataBase64(ثابت تهی* _data, size_t len, ثابت نویسه* dt );

    String releaseAndGetString();

    File_گره get_First_بالاترین_Level_گره() ثابت;

    File_گره ریشه(صحیح streamIdx=0) ثابت;

    File_گره operator[](ثابت String& nodename) ثابت;

    File_گره operator[](ثابت نویسه* /*nodename*/) ثابت;

    مجازی صحیح getFormat() ثابت CV_OVERRIDE;

    نویسه* bufferPtr() ثابت;
    نویسه* bufferStart() ثابت;
    نویسه* bufferEnd() ثابت;
    تهی setBufferPtr(نویسه* ptr);
    صحیح wrapMargin() ثابت;

    FStructData& getCurrentStruct();

    تهی setNonEmpty();

    تهی processSpecialDouble( نویسه* buf, دوگانه* value, نویسه** endptr );

    دوگانه strtod( نویسه* ptr, نویسه** endptr );

    تهی convertToCollection(صحیح type, File_گره& گره);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_گره_Space(File_گره& گره, size_t sz);

    unsigned getStringOfs( ثابت std::string& کلید ) ثابت;

    File_گره add_گره( File_گره& collection, ثابت std::string& کلید,
                      صحیح elem_type, ثابت تهی* value, صحیح len );

    تهی finalizeCollection( File_گره& collection );

    تهی normalize_گره_Ofs(size_t& blockIdx, size_t& ofs) ثابت;

    Base64State get_state_of_writing_base64();

    صحیح get_space();

    رده Base64Decoder
    {
    عمومی:
        Base64Decoder();
        تهی init(ثابت Ptr<FileStorageParser>& _parser, نویسه* _ptr, صحیح _indent);

        منطقی readMore(صحیح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحیح getInt32();

        int64_t getInt64();

        دوگانه getFloat64();

        منطقی endOfStream() ثابت;
        نویسه* getPtr() ثابت;
    محافظتشده:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ثابت
        {
            اگر (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            بازگشت *parser_do_not_use_direct_dereference;
        }
        نویسه* ptr;
        صحیح indent;
        std::vector<نویسه> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقی eos;
    };

    نویسه* parseBase64(نویسه* ptr, صحیح indent, File_گره& collection);

    تهی parseError( ثابت نویسه* func_name, ثابت std::string& err_پیام, ثابت نویسه* source_file, صحیح source_سطر );

    ثابت uchar* get_گره_Ptr(size_t blockIdx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحیح flags;
    منطقی empty_stream;

    FILE* file;
    gzFile gzfile;

    منطقی is_opened;
    منطقی dummy_eof;
    منطقی write_mode;
    منطقی mem_mode;
    صحیح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقی is_using_base64;
    منطقی is_write_struct_delayed;
    نویسه* delayed_struct_کلید;
    صحیح   delayed_struct_flags;
    نویسه* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    صحیح space, wrap_margin;
    std::deque<FStructData> write_پشته;
    std::vector<نویسه> buffer;
    size_t bufofs;

    std::deque<نویسه> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        اگر (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        بازگشت *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ثابت
    {
        اگر (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        بازگشت *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_گره> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    تعریفنوع std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<نویسه> str_hash_data;

    std::vector<نویسه> strbufv;
    نویسه* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحیح lineno;
};

}

#endif
