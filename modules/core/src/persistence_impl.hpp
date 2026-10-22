// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#جے_بنا_تعریف OPENCV_CORE_PERSISTENCE_IMPL_HPP
#تعریف_کرو OPENCV_CORE_PERSISTENCE_IMPL_HPP

#ضمن "persistence.hpp"
#ضمن "persistence_base64_encoding.hpp"
#ضمن <unordered_map>
#ضمن <iterator>


نطاق cv
{

تعداد Base64State{
    Uncertain,
    NotUse,
    وچ_Use,
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

    حرف* getsFromFile( حرف* buf, صحيح گنتی_2 );

    حرف* gets( size_t max_گنتی );

    حرف* gets();

    منطقي eof();

    فراغ سیٹ_Eof();

    فراغ closeFile();

    فراغ rewind();

    حرف* resize_Write_بفر( حرف* ptr, صحيح len );

    حرف* flush();

    فراغ سرا_Write_Struct();

    فراغ شروع_Write_Struct_helper( ثابت حرف* کلید, صحيح struct_flags,
                                  ثابت حرف* type_name );

    فراغ شروع_Write_Struct( ثابت حرف* کلید, صحيح struct_flags,
                           ثابت حرف* type_name );

    فراغ writeComment( ثابت حرف* comment, منطقي eol_comment );

    فراغ شروع_اگلا_دھارا();

    فراغ اكتب( ثابت String& کلید, صحيح value );

    فراغ اكتب( ثابت String& کلید, منطقي value );

    فراغ اكتب( ثابت String& کلید, int64_t value );

    فراغ اكتب( ثابت String& کلید, مزدوج value );

    فراغ اكتب( ثابت String& کلید, ثابت String& value );

    فراغ write_Raw_انکڑے( ثابت std::string& dt, ثابت فراغ* انکڑے_3, size_t len );

    فراغ workaround();

    فراغ switch_منزل_Base64_state( FileStorage_API::Base64State new_state);

    فراغ make_write_struct_delayed( ثابت حرف* کلید, صحيح struct_flags, ثابت حرف* type_name );

    فراغ check_if_write_struct_is_delayed( منطقي change_type_منزل_base64 );

    فراغ write_Raw_انکڑے_Base64(ثابت فراغ* انکڑے_3, size_t len, ثابت حرف* dt );

    String releaseAndGetString();

    File_گرہ get_پہلا_چوٹی_Level_گرہ() ثابت;

    File_گرہ جڑ(صحيح دھارا_Idx=0) ثابت;

    File_گرہ عامل[](ثابت String& nodename) ثابت;

    File_گرہ عامل[](ثابت حرف* /*nodename*/) ثابت;

    مجازی صحيح getFormat() ثابت CV_OVERRIDE;

    حرف* بفر_Ptr() ثابت;
    حرف* بفر_شروع() ثابت;
    حرف* بفر_سرا_2() ثابت;
    فراغ سیٹ_بفر_Ptr(حرف* ptr);
    صحيح wrapMargin() ثابت;

    FStruct_انکڑے& getCurrentStruct();

    فراغ سیٹ_Non_خالی_تھاواں();

    فراغ processSpecialDouble( حرف* buf, مزدوج* value, حرف** endptr );

    مزدوج strtod( حرف* ptr, حرف** endptr );

    فراغ convert_منزل_Collection(صحيح type, File_گرہ& گرہ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_گرہ_Space(File_گرہ& گرہ, size_t sz);

    بنانشانی getStringOfs( ثابت std::string& کلید ) ثابت;

    File_گرہ جوڑو_گرہ( File_گرہ& collection, ثابت std::string& کلید,
                      صحيح elem_type, ثابت فراغ* value, صحيح len );

    فراغ finalizeCollection( File_گرہ& collection );

    فراغ normalize_گرہ_Ofs(size_t& بلاک_Idx, size_t& ofs) ثابت;

    Base64State get_state_دا_writing_base64();

    صحيح get_space();

    صنف Base64Decoder
    {
    عام:
        Base64Decoder();
        فراغ init(ثابت Ptr<FileStorageParser>& _parser, حرف* _ptr, صحيح _indent);

        منطقي read_ہور(صحيح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحيح getInt32();

        int64_t getInt64();

        مزدوج getFloat64();

        منطقي سرا_دا_دھارا() ثابت;
        حرف* getPtr() ثابت;
    محمي:

        Ptr<FileStorageParser> parser_do_not_use_سدھا_dereference;
        FileStorageParser& getParser() ثابت
        {
            إذا (!parser_do_not_use_سدھا_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            أرجع *parser_do_not_use_سدھا_dereference;
        }
        حرف* ptr;
        صحيح indent;
        std::vector<حرف> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقي eos;
    };

    حرف* parseBase64(حرف* ptr, صحيح indent, File_گرہ& collection);

    فراغ parseError( ثابت حرف* func_name, ثابت std::string& err_پیغام, ثابت حرف* سروت_file, صحيح سروت_سطر );

    ثابت uchar* get_گرہ_Ptr(size_t بلاک_Idx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحيح flags;
    منطقي خالی_تھاواں_دھارا;

    FILE* file;
    gzFile gzfile;

    منطقي is_opened;
    منطقي dummy_eof;
    منطقي write_منوال;
    منطقي mem_منوال;
    صحيح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقي is_using_base64;
    منطقي is_write_struct_delayed;
    حرف* delayed_struct_کلید;
    صحيح   delayed_struct_flags;
    حرف* delayed_type_name;
    FileStorage_API::Base64State state_دا_writing_base64;

    صحيح space, wrap_margin;
    std::deque<FStruct_انکڑے> write_ڈھیر;
    std::vector<حرف> بفر;
    size_t bufofs;

    std::deque<حرف> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_سدھا_dereference;
    FileStorageEmitter& getEmitter()
    {
        إذا (!emitter_do_not_use_سدھا_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        أرجع *emitter_do_not_use_سدھا_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_سدھا_dereference;
    FileStorageParser& getParser() ثابت
    {
        إذا (!parser_do_not_use_سدھا_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        أرجع *parser_do_not_use_سدھا_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_گرہ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_انکڑے;
    std::vector<uchar*> fs_انکڑے_ptrs;
    std::vector<size_t> fs_انکڑے_blksz;
    size_t freeSpaceOfs;
    تعريفنوع std::unordered_map<std::string, بنانشانی> str_hash_t;
    str_hash_t str_hash;
    std::vector<حرف> str_hash_انکڑے;

    std::vector<حرف> strbufv;
    حرف* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحيح lineno;
};

}

#جے_انت
