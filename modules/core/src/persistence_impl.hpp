// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अगर_अघोषित OPENCV_CORE_PERSISTENCE_IMPL_HPP
#घोषणा OPENCV_CORE_PERSISTENCE_IMPL_HPP

#समावेश "persistence.hpp"
#समावेश "persistence_base64_encoding.hpp"
#समावेश <unordered_map>
#समावेश <iterator>


namespace cv
{

क्रमागत Base64State{
    Uncertain,
    NotUse,
    InUse,
};

श्रेणी cv::FileStorage::Impl : खुला FileStorage_API
{
खुला:
    व्योम init();

    Impl(FileStorage* _fs);

    भव ~Impl();

    व्योम release(String* out=0);

    व्योम analyze_file_name( स्थिर std::string& file_name, std::vector<std::string>& params );

    bool खोलो( स्थिर अक्षर* filename_or_buf, पूर्णांक _flags, स्थिर अक्षर* encoding );

    व्योम माला_दो( स्थिर अक्षर* str );

    अक्षर* getsFromFile( अक्षर* buf, पूर्णांक गिनती_2 );

    अक्षर* माला_लो( माप_प्रकार max_गिनती );

    अक्षर* माला_लो();

    bool खप();

    व्योम setEof();

    व्योम closeFile();

    व्योम शुरुआत();

    अक्षर* resize_Write_बफर( अक्षर* ptr, पूर्णांक len );

    अक्षर* सफाई();

    व्योम endWriteStruct();

    व्योम startWriteStruct_helper( स्थिर अक्षर* कुंजी, पूर्णांक struct_flags,
                                  स्थिर अक्षर* type_name );

    व्योम startWriteStruct( स्थिर अक्षर* कुंजी, पूर्णांक struct_flags,
                           स्थिर अक्षर* type_name );

    व्योम writeComment( स्थिर अक्षर* comment, bool eol_comment );

    व्योम start_अग्र_Stream();

    व्योम ग_लिखो( स्थिर String& कुंजी, पूर्णांक value );

    व्योम ग_लिखो( स्थिर String& कुंजी, bool value );

    व्योम ग_लिखो( स्थिर String& कुंजी, int64_t value );

    व्योम ग_लिखो( स्थिर String& कुंजी, द्विगुन value );

    व्योम ग_लिखो( स्थिर String& कुंजी, स्थिर String& value );

    व्योम write_Raw_तथ्य( स्थिर std::string& dt, स्थिर व्योम* तथ्य_3, माप_प्रकार len );

    व्योम workaround();

    व्योम switch_to_Base64_state( FileStorage_API::Base64State new_state);

    व्योम make_write_struct_delayed( स्थिर अक्षर* कुंजी, पूर्णांक struct_flags, स्थिर अक्षर* type_name );

    व्योम check_if_write_struct_is_delayed( bool change_type_to_base64 );

    व्योम write_Raw_तथ्य_Base64(स्थिर व्योम* तथ्य_3, माप_प्रकार len, स्थिर अक्षर* dt );

    String releaseAndGetString();

    File_गाँठ get_First_शीर्ष_Level_गाँठ() स्थिर;

    File_गाँठ मूल(पूर्णांक streamIdx=0) स्थिर;

    File_गाँठ चालक[](स्थिर String& nodename) स्थिर;

    File_गाँठ चालक[](स्थिर अक्षर* /*nodename*/) स्थिर;

    भव पूर्णांक getFormat() स्थिर CV_OVERRIDE;

    अक्षर* बफर_Ptr() स्थिर;
    अक्षर* बफर_Start() स्थिर;
    अक्षर* बफर_End() स्थिर;
    व्योम set_बफर_Ptr(अक्षर* ptr);
    पूर्णांक wrapMargin() स्थिर;

    FStruct_तथ्य& getCurrentStruct();

    व्योम setNonEmpty();

    व्योम processSpecialDouble( अक्षर* buf, द्विगुन* value, अक्षर** endptr );

    द्विगुन म_से_भग्न( अक्षर* ptr, अक्षर** endptr );

    व्योम convertToCollection(पूर्णांक type, File_गाँठ& गाँठ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_गाँठ_Space(File_गाँठ& गाँठ, माप_प्रकार sz);

    अचिन्हित getStringOfs( स्थिर std::string& कुंजी ) स्थिर;

    File_गाँठ add_गाँठ( File_गाँठ& collection, स्थिर std::string& कुंजी,
                      पूर्णांक elem_type, स्थिर व्योम* value, पूर्णांक len );

    व्योम finalizeCollection( File_गाँठ& collection );

    व्योम normalize_गाँठ_Ofs(माप_प्रकार& blockIdx, माप_प्रकार& ofs) स्थिर;

    Base64State get_state_of_writing_base64();

    पूर्णांक get_space();

    श्रेणी Base64Decoder
    {
    खुला:
        Base64Decoder();
        व्योम init(स्थिर Ptr<FileStorageParser>& _parser, अक्षर* _ptr, पूर्णांक _indent);

        bool readMore(पूर्णांक needed);

        uchar getUInt8();

        ushort getUInt16();

        पूर्णांक getInt32();

        int64_t getInt64();

        द्विगुन getFloat64();

        bool endOfStream() स्थिर;
        अक्षर* getPtr() स्थिर;
    रक्षित:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() स्थिर
        {
            अगर (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            वापस *parser_do_not_use_direct_dereference;
        }
        अक्षर* ptr;
        पूर्णांक indent;
        std::vector<अक्षर> encoded;
        std::vector<uchar> decoded;
        माप_प्रकार ofs;
        माप_प्रकार totalchars;
        bool eos;
    };

    अक्षर* parseBase64(अक्षर* ptr, पूर्णांक indent, File_गाँठ& collection);

    व्योम parseError( स्थिर अक्षर* func_name, स्थिर std::string& err_संदेश, स्थिर अक्षर* source_file, पूर्णांक source_पंक्ति );

    स्थिर uchar* get_गाँठ_Ptr(माप_प्रकार blockIdx, माप_प्रकार ofs) स्थिर;

    std::string getName( माप_प्रकार nameofs ) स्थिर;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    पूर्णांक flags;
    bool empty_stream;

    खाता* file;
    gzFile gzfile;

    bool is_opened;
    bool dummy_eof;
    bool write_mode;
    bool mem_mode;
    पूर्णांक fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    bool is_using_base64;
    bool is_write_struct_delayed;
    अक्षर* delayed_struct_कुंजी;
    पूर्णांक   delayed_struct_flags;
    अक्षर* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    पूर्णांक space, wrap_margin;
    std::deque<FStruct_तथ्य> write_ढेर;
    std::vector<अक्षर> बफर;
    माप_प्रकार bufofs;

    std::deque<अक्षर> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        अगर (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        वापस *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() स्थिर
    {
        अगर (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        वापस *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_गाँठ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_तथ्य;
    std::vector<uchar*> fs_तथ्य_ptrs;
    std::vector<माप_प्रकार> fs_तथ्य_blksz;
    माप_प्रकार freeSpaceOfs;
    प्रकार std::unordered_map<std::string, अचिन्हित> str_hash_t;
    str_hash_t str_hash;
    std::vector<अक्षर> str_hash_तथ्य;

    std::vector<अक्षर> strbufv;
    अक्षर* strbuf;
    माप_प्रकार strbufsize;
    माप_प्रकार strbufpos;
    पूर्णांक lineno;
};

}

#पूर्ण_अगर
