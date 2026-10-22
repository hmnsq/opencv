// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#اگر_تعریف_نشده OPENCV_CORE_PERSISTENCE_IMPL_HPP
#تعریف_کن OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ضمن "persistence.hpp"
#ضمن "persistence_base64_encoding.hpp"
#ضمن <unordered_map>
#ضمن <iterator>


نطاق cv
{

تعداد Base64State{
    Uncertain,
    NotUse,
    درون_Use,
};

صنف cv::FileStorage::Impl : عام FileStorage_API
{
عام:
    فراغ init();

    Impl(FileStorage* _fs);

    مجازی ~Impl();

    فراغ release(String* out=0);

    فراغ analyze_file_name( ثابت std::string& file_name, std::vector<std::string>& params );

    منطقي open( ثابت حرف* filename_or_buf, صحيح _flags, ثابت حرف* encoding );

    فراغ puts( ثابت حرف* str );

    حرف* getsFromFile( حرف* buf, صحيح شمار_2 );

    حرف* gets( size_t max_شمار );

    حرف* gets();

    منطقي eof();

    فراغ مجموعه_Eof();

    فراغ closeFile();

    فراغ rewind();

    حرف* resize_Write_میانگیر( حرف* ptr, صحيح len );

    حرف* flush();

    فراغ انتها_Write_Struct();

    فراغ شروع_Write_Struct_helper( ثابت حرف* کلید, صحيح struct_flags,
                                  ثابت حرف* type_name );

    فراغ شروع_Write_Struct( ثابت حرف* کلید, صحيح struct_flags,
                           ثابت حرف* type_name );

    فراغ writeComment( ثابت حرف* comment, منطقي eol_comment );

    فراغ شروع_بعدی_جریان();

    فراغ اكتب( ثابت String& کلید, صحيح value );

    فراغ اكتب( ثابت String& کلید, منطقي value );

    فراغ اكتب( ثابت String& کلید, int64_t value );

    فراغ اكتب( ثابت String& کلید, مزدوج value );

    فراغ اكتب( ثابت String& کلید, ثابت String& value );

    فراغ write_Raw_داده( ثابت std::string& dt, ثابت فراغ* داده_3, size_t len );

    فراغ workaround();

    فراغ switch_مقصد_Base64_state( FileStorage_API::Base64State new_state);

    فراغ make_write_struct_delayed( ثابت حرف* کلید, صحيح struct_flags, ثابت حرف* type_name );

    فراغ check_if_write_struct_is_delayed( منطقي change_type_مقصد_base64 );

    فراغ write_Raw_داده_Base64(ثابت فراغ* داده_3, size_t len, ثابت حرف* dt );

    String releaseAndGetString();

    File_گره get_اولی_بالاترین_Level_گره() ثابت;

    File_گره ریشه(صحيح جریان_Idx=0) ثابت;

    File_گره عملگر[](ثابت String& nodename) ثابت;

    File_گره عملگر[](ثابت حرف* /*nodename*/) ثابت;

    مجازی صحيح getFormat() ثابت CV_OVERRIDE;

    حرف* میانگیر_Ptr() ثابت;
    حرف* میانگیر_شروع() ثابت;
    حرف* میانگیر_انتها_2() ثابت;
    فراغ مجموعه_میانگیر_Ptr(حرف* ptr);
    صحيح wrapMargin() ثابت;

    FStruct_داده& getCurrentStruct();

    فراغ مجموعه_Non_خالی();

    فراغ processSpecialDouble( حرف* buf, مزدوج* value, حرف** endptr );

    مزدوج strtod( حرف* ptr, حرف** endptr );

    فراغ convert_مقصد_Collection(صحيح type, File_گره& گره);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_گره_Space(File_گره& گره, size_t sz);

    بیعلامت getStringOfs( ثابت std::string& کلید ) ثابت;

    File_گره جمع_کن_گره( File_گره& collection, ثابت std::string& کلید,
                      صحيح elem_type, ثابت فراغ* value, صحيح len );

    فراغ finalizeCollection( File_گره& collection );

    فراغ normalize_گره_Ofs(size_t& بلوک_Idx, size_t& ofs) ثابت;

    Base64State get_state_از_writing_base64();

    صحيح get_space();

    صنف Base64Decoder
    {
    عام:
        Base64Decoder();
        فراغ init(ثابت Ptr<FileStorageParser>& _parser, حرف* _ptr, صحيح _indent);

        منطقي read_بیشتر(صحيح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحيح getInt32();

        int64_t getInt64();

        مزدوج getFloat64();

        منطقي انتها_از_جریان() ثابت;
        حرف* getPtr() ثابت;
    محمي:

        Ptr<FileStorageParser> parser_do_not_use_مستقیم_dereference;
        FileStorageParser& getParser() ثابت
        {
            إذا (!parser_do_not_use_مستقیم_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            أرجع *parser_do_not_use_مستقیم_dereference;
        }
        حرف* ptr;
        صحيح indent;
        std::vector<حرف> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقي eos;
    };

    حرف* parseBase64(حرف* ptr, صحيح indent, File_گره& collection);

    فراغ parseError( ثابت حرف* func_name, ثابت std::string& err_پیام, ثابت حرف* منبع_file, صحيح منبع_سطر );

    ثابت uchar* get_گره_Ptr(size_t بلوک_Idx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحيح flags;
    منطقي خالی_جریان;

    FILE* file;
    gzFile gzfile;

    منطقي is_opened;
    منطقي dummy_eof;
    منطقي write_نما;
    منطقي mem_نما;
    صحيح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقي is_using_base64;
    منطقي is_write_struct_delayed;
    حرف* delayed_struct_کلید;
    صحيح   delayed_struct_flags;
    حرف* delayed_type_name;
    FileStorage_API::Base64State state_از_writing_base64;

    صحيح space, wrap_margin;
    std::deque<FStruct_داده> write_پشته;
    std::vector<حرف> میانگیر;
    size_t bufofs;

    std::deque<حرف> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_مستقیم_dereference;
    FileStorageEmitter& getEmitter()
    {
        إذا (!emitter_do_not_use_مستقیم_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        أرجع *emitter_do_not_use_مستقیم_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_مستقیم_dereference;
    FileStorageParser& getParser() ثابت
    {
        إذا (!parser_do_not_use_مستقیم_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        أرجع *parser_do_not_use_مستقیم_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_گره> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_داده;
    std::vector<uchar*> fs_داده_ptrs;
    std::vector<size_t> fs_داده_blksz;
    size_t freeSpaceOfs;
    تعريفنوع std::unordered_map<std::string, بیعلامت> str_hash_t;
    str_hash_t str_hash;
    std::vector<حرف> str_hash_داده;

    std::vector<حرف> strbufv;
    حرف* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحيح lineno;
};

}

#پایان_اگر
