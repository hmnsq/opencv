// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#اگر_غیر_تعریف_شدہ OPENCV_CORE_PERSISTENCE_IMPL_HPP
#تعریف_کرو OPENCV_CORE_PERSISTENCE_IMPL_HPP

#شامل "persistence.hpp"
#شامل "persistence_base64_encoding.hpp"
#شامل <unordered_map>
#شامل <iterator>


نامخلا cv
{

شمار Base64State{
    Uncertain,
    NotUse,
    اندر_Use,
};

جماعت cv::FileStorage::Impl : عوامی FileStorage_API
{
عوامی:
    خالی init();

    Impl(FileStorage* _fs);

    مجازی ~Impl();

    خالی release(String* out=0);

    خالی analyze_file_name( ثابت std::string& file_name, std::vector<std::string>& params );

    منطقی open( ثابت حرف* filename_or_buf, صحیح _flags, ثابت حرف* encoding );

    خالی puts( ثابت حرف* str );

    حرف* getsFromFile( حرف* buf, صحیح گنتی_2 );

    حرف* gets( size_t max_گنتی );

    حرف* gets();

    منطقی eof();

    خالی سیٹ_Eof();

    خالی closeFile();

    خالی rewind();

    حرف* resize_Write_بفر( حرف* ptr, صحیح len );

    حرف* flush();

    خالی انتہا_Write_Struct();

    خالی آغاز_Write_Struct_helper( ثابت حرف* کلید, صحیح struct_flags,
                                  ثابت حرف* type_name );

    خالی آغاز_Write_Struct( ثابت حرف* کلید, صحیح struct_flags,
                           ثابت حرف* type_name );

    خالی writeComment( ثابت حرف* comment, منطقی eol_comment );

    خالی آغاز_اگلا_دھارا();

    خالی اكتب( ثابت String& کلید, صحیح value );

    خالی اكتب( ثابت String& کلید, منطقی value );

    خالی اكتب( ثابت String& کلید, int64_t value );

    خالی اكتب( ثابت String& کلید, دوہرا value );

    خالی اكتب( ثابت String& کلید, ثابت String& value );

    خالی write_Raw_ڈیٹا( ثابت std::string& dt, ثابت خالی* ڈیٹا_3, size_t len );

    خالی workaround();

    خالی switch_مقصد_Base64_state( FileStorage_API::Base64State new_state);

    خالی make_write_struct_delayed( ثابت حرف* کلید, صحیح struct_flags, ثابت حرف* type_name );

    خالی check_if_write_struct_is_delayed( منطقی change_type_مقصد_base64 );

    خالی write_Raw_ڈیٹا_Base64(ثابت خالی* ڈیٹا_3, size_t len, ثابت حرف* dt );

    String releaseAndGetString();

    File_گرہ get_پہلا_چوٹی_Level_گرہ() ثابت;

    File_گرہ جڑ(صحیح دھارا_Idx=0) ثابت;

    File_گرہ عامل[](ثابت String& nodename) ثابت;

    File_گرہ عامل[](ثابت حرف* /*nodename*/) ثابت;

    مجازی صحیح getFormat() ثابت CV_OVERRIDE;

    حرف* بفر_Ptr() ثابت;
    حرف* بفر_آغاز() ثابت;
    حرف* بفر_انتہا_2() ثابت;
    خالی سیٹ_بفر_Ptr(حرف* ptr);
    صحیح wrapMargin() ثابت;

    FStruct_ڈیٹا& getCurrentStruct();

    خالی سیٹ_Non_خالی_خانے();

    خالی processSpecialDouble( حرف* buf, دوہرا* value, حرف** endptr );

    دوہرا strtod( حرف* ptr, حرف** endptr );

    خالی convert_مقصد_Collection(صحیح type, File_گرہ& گرہ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_گرہ_Space(File_گرہ& گرہ, size_t sz);

    غیرعلامتی getStringOfs( ثابت std::string& کلید ) ثابت;

    File_گرہ جوڑو_گرہ( File_گرہ& collection, ثابت std::string& کلید,
                      صحیح elem_type, ثابت خالی* value, صحیح len );

    خالی finalizeCollection( File_گرہ& collection );

    خالی normalize_گرہ_Ofs(size_t& بلاک_Idx, size_t& ofs) ثابت;

    Base64State get_state_کا_writing_base64();

    صحیح get_space();

    جماعت Base64Decoder
    {
    عوامی:
        Base64Decoder();
        خالی init(ثابت Ptr<FileStorageParser>& _parser, حرف* _ptr, صحیح _indent);

        منطقی read_مزید(صحیح needed);

        uchar getUInt8();

        ushort getUInt16();

        صحیح getInt32();

        int64_t getInt64();

        دوہرا getFloat64();

        منطقی انتہا_کا_دھارا() ثابت;
        حرف* getPtr() ثابت;
    محفوظ:

        Ptr<FileStorageParser> parser_do_not_use_براہ_راست_dereference;
        FileStorageParser& getParser() ثابت
        {
            اگر (!parser_do_not_use_براہ_راست_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            واپس *parser_do_not_use_براہ_راست_dereference;
        }
        حرف* ptr;
        صحیح indent;
        std::vector<حرف> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        منطقی eos;
    };

    حرف* parseBase64(حرف* ptr, صحیح indent, File_گرہ& collection);

    خالی parseError( ثابت حرف* func_name, ثابت std::string& err_پیغام, ثابت حرف* ماخذ_file, صحیح ماخذ_سطر );

    ثابت uchar* get_گرہ_Ptr(size_t بلاک_Idx, size_t ofs) ثابت;

    std::string getName( size_t nameofs ) ثابت;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    صحیح flags;
    منطقی خالی_خانے_دھارا;

    FILE* file;
    gzFile gzfile;

    منطقی is_opened;
    منطقی dummy_eof;
    منطقی write_منوال;
    منطقی mem_منوال;
    صحیح fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    منطقی is_using_base64;
    منطقی is_write_struct_delayed;
    حرف* delayed_struct_کلید;
    صحیح   delayed_struct_flags;
    حرف* delayed_type_name;
    FileStorage_API::Base64State state_کا_writing_base64;

    صحیح space, wrap_margin;
    std::deque<FStruct_ڈیٹا> write_ڈھیر;
    std::vector<حرف> بفر;
    size_t bufofs;

    std::deque<حرف> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_براہ_راست_dereference;
    FileStorageEmitter& getEmitter()
    {
        اگر (!emitter_do_not_use_براہ_راست_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        واپس *emitter_do_not_use_براہ_راست_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_براہ_راست_dereference;
    FileStorageParser& getParser() ثابت
    {
        اگر (!parser_do_not_use_براہ_راست_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        واپس *parser_do_not_use_براہ_راست_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_گرہ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_ڈیٹا;
    std::vector<uchar*> fs_ڈیٹا_ptrs;
    std::vector<size_t> fs_ڈیٹا_blksz;
    size_t freeSpaceOfs;
    قسمتعریف std::unordered_map<std::string, غیرعلامتی> str_hash_t;
    str_hash_t str_hash;
    std::vector<حرف> str_hash_ڈیٹا;

    std::vector<حرف> strbufv;
    حرف* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    صحیح lineno;
};

}

#اگر_ختم
