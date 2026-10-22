// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_PERSISTENCE_IMPL_HPP
#وصف_ڪريو OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ضمن "persistence.hpp"
#ضمن "persistence_base64_encoding.hpp"
#ضمن <unordered_map>
#ضمن <iterator>


نطاق cv
{

تعداد Base64State{
    Uncertain,
    NotUse,
    اندريون_Use,
};

صنف cv::FileStorage::Impl : عام FileStorage_API
{
عام:
    فراغ init();

    Impl(FileStorage* _fs);

    مجازي ~Impl();

    فراغ release(String* out=0);

    فراغ analyze_file_name( ثابت std::string& file_name, std::vector<std::string>& params );

    منطقي open( ثابت حرف* filename_or_buf, صحيح _flags, ثابت حرف* encoding );

    فراغ puts( ثابت حرف* str );

    حرف* getsFromFile( حرف* buf, صحيح ڳڻپ_2 );

    حرف* gets( size_t max_ڳڻپ );

    حرف* gets();

    منطقي eof();

    فراغ سيٽ_Eof();

    فراغ closeFile();

    فراغ rewind();

    حرف* resize_Write_بفر( حرف* ptr, صحيح len );

    حرف* flush();

    فراغ پڇاڙي_Write_Struct();

    فراغ شروعات_Write_Struct_helper( ثابت حرف* ڪنجي, صحيح struct_flags,
                                  ثابت حرف* type_name );

    فراغ شروعات_Write_Struct( ثابت حرف* ڪنجي, صحيح struct_flags,
                           ثابت حرف* type_name );

    فراغ writeComment( ثابت حرف* comment, منطقي eol_comment );

    فراغ شروعات_ايندڙ_وهڪرو();

    فراغ اكتب( ثابت String& ڪنجي, صحيح value );

    فراغ اكتب( ثابت String& ڪنجي, منطقي value );

    فراغ اكتب( ثابت String& ڪنجي, int64_t value );

    فراغ اكتب( ثابت String& ڪنجي, مزدوج value );

    فراغ اكتب( ثابت String& ڪنجي, ثابت String& value );

    فراغ write_Raw_معلومات( ثابت std::string& dt, ثابت فراغ* معلومات_3, size_t len );

    فراغ workaround();

    فراغ switch_منزل_Base64_state( FileStorage_API::Base64State new_state);

    فراغ make_write_struct_delayed( ثابت حرف* ڪنجي, صحيح struct_flags, ثابت حرف* type_name );

    فراغ check_if_write_struct_is_delayed( منطقي change_type_منزل_base64 );

    فراغ write_Raw_معلومات_Base64(ثابت فراغ* معلومات_3, size_t len, ثابت حرف* dt );

    String releaseAndGetString();

    File_ڳنڍ get_پهريون_چوٽي_Level_ڳنڍ() ثابت;

    File_ڳنڍ پاڙ(صحيح وهڪرو_Idx=0) ثابت;

    File_ڳنڍ عامل[](ثابت String& nodename) ثابت;

    File_ڳنڍ عامل[](ثابت حرف* /*nodename*/) ثابت;

    مجازي صحيح getFormat() ثابت CV_OVERRIDE;

    حرف* بفر_Ptr() ثابت;
    حرف* بفر_شروعات() ثابت;
    حرف* بفر_پڇاڙي_2() ثابت;
    فراغ سيٽ_بفر_Ptr(حرف* ptr);
    صحيح wrapMargin() ثابت;

    FStruct_معلومات& getCurrentStruct();

    فراغ سيٽ_Non_خالي_جايون();

    فراغ processSpecialDouble( حرف* buf, مزدوج* value, حرف** endptr );

    مزدوج strtod( حرف* ptr, حرف** endptr );

    فراغ convert_منزل_Collection(صحيح type, File_ڳنڍ& ڳنڍ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ڳنڍ_Space(File_ڳنڍ& ڳنڍ, size_t sz);

    بنانشان getStringOfs( ثابت std::string& ڪنجي ) ثابت;

    File_ڳنڍ جوڙيو_ڳنڍ( File_ڳنڍ& collection, ثابت std::string& ڪنجي,
                      صحيح elem_type, ثابت فراغ* value, صحيح len );

    فراغ finalizeCollection( File_ڳنڍ& collection );

    فراغ normalize_ڳنڍ_Ofs(size_t& بلاڪ_Idx, size_t& ofs) ثابت;

    Base64State get_state_جو_writing_base64();

    صحيح get_space();

    صنف Base64Decoder
    {
    عام:
        Base64Decoder();
        فراغ init(ثابت Ptr<FileStorageParser>& _parser, حرف* _ptr, صحيح _indent);

        منطقي read_وڌيڪ(صحيح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحيح getInt32();

        int64_t getInt64();

        مزدوج getFloat64();

        منطقي پڇاڙي_جو_وهڪرو() ثابت;
        حرف* getPtr() ثابت;
    محمي:

        Ptr<FileStorageParser> parser_do_not_use_سڌو_dereference;
        FileStorageParser& getParser() ثابت
        {
            إذا (!parser_do_not_use_سڌو_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            أرجع *parser_do_not_use_سڌو_dereference;
        }
        حرف* ptr;
        صحيح indent;
        std::vector<حرف> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقي eos;
    };

    حرف* parseBase64(حرف* ptr, صحيح indent, File_ڳنڍ& collection);

    فراغ parseError( ثابت حرف* func_name, ثابت std::string& err_نياپو, ثابت حرف* ذريعو_file, صحيح ذريعو_سٽ );

    ثابت uchar* get_ڳنڍ_Ptr(size_t بلاڪ_Idx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحيح flags;
    منطقي خالي_جايون_وهڪرو;

    FILE* file;
    gzFile gzfile;

    منطقي is_opened;
    منطقي dummy_eof;
    منطقي write_گهڻائي;
    منطقي mem_گهڻائي;
    صحيح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقي is_using_base64;
    منطقي is_write_struct_delayed;
    حرف* delayed_struct_ڪنجي;
    صحيح   delayed_struct_flags;
    حرف* delayed_type_name;
    FileStorage_API::Base64State state_جو_writing_base64;

    صحيح space, wrap_margin;
    std::deque<FStruct_معلومات> write_ڍير;
    std::vector<حرف> بفر;
    size_t bufofs;

    std::deque<حرف> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_سڌو_dereference;
    FileStorageEmitter& getEmitter()
    {
        إذا (!emitter_do_not_use_سڌو_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        أرجع *emitter_do_not_use_سڌو_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_سڌو_dereference;
    FileStorageParser& getParser() ثابت
    {
        إذا (!parser_do_not_use_سڌو_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        أرجع *parser_do_not_use_سڌو_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ڳنڍ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_معلومات;
    std::vector<uchar*> fs_معلومات_ptrs;
    std::vector<size_t> fs_معلومات_blksz;
    size_t freeSpaceOfs;
    تعريفنوع std::unordered_map<std::string, بنانشان> str_hash_t;
    str_hash_t str_hash;
    std::vector<حرف> str_hash_معلومات;

    std::vector<حرف> strbufv;
    حرف* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحيح lineno;
};

}

#جيڪڏهن_پورو
