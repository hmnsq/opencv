// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#अन्तर्भाव "persistence.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <iterator>


नामाकाश cv
{

गणना Base64State{
    Uncertain,
    NotUse,
    InUse,
};

वर्ग cv::FileStorage::Impl : सार्वजनिक FileStorage_API
{
सार्वजनिक:
    शून्य init();

    Impl(FileStorage* _fs);

    आभासी ~Impl();

    शून्य release(String* out=0);

    शून्य analyze_file_name( स्थिरांक std::string& file_name, std::vector<std::string>& params );

    तार्किक खोलो( स्थिरांक अक्षर* filename_or_buf, पूर्णांक _flags, स्थिरांक अक्षर* encoding );

    शून्य माला_दो( स्थिरांक अक्षर* str );

    अक्षर* getsFromFile( अक्षर* buf, पूर्णांक मोजणी_2 );

    अक्षर* माला_लो( माप_प्रकार max_मोजणी );

    अक्षर* माला_लो();

    तार्किक eof();

    शून्य setEof();

    शून्य closeFile();

    शून्य शुरुआत();

    अक्षर* resizeWriteBuffer( अक्षर* ptr, पूर्णांक len );

    अक्षर* flush();

    शून्य endWriteStruct();

    शून्य startWriteStruct_helper( स्थिरांक अक्षर* किल्ली, पूर्णांक struct_flags,
                                  स्थिरांक अक्षर* type_name );

    शून्य startWriteStruct( स्थिरांक अक्षर* किल्ली, पूर्णांक struct_flags,
                           स्थिरांक अक्षर* type_name );

    शून्य writeComment( स्थिरांक अक्षर* comment, तार्किक eol_comment );

    शून्य startNextStream();

    शून्य लिख( स्थिरांक String& किल्ली, पूर्णांक value );

    शून्य लिख( स्थिरांक String& किल्ली, तार्किक value );

    शून्य लिख( स्थिरांक String& किल्ली, int64_t value );

    शून्य लिख( स्थिरांक String& किल्ली, द्विगुण value );

    शून्य लिख( स्थिरांक String& किल्ली, स्थिरांक String& value );

    शून्य writeRawData( स्थिरांक std::string& dt, स्थिरांक शून्य* _data, माप_प्रकार len );

    शून्य workaround();

    शून्य switch_to_Base64_state( FileStorage_API::Base64State new_state);

    शून्य make_write_struct_delayed( स्थिरांक अक्षर* किल्ली, पूर्णांक struct_flags, स्थिरांक अक्षर* type_name );

    शून्य check_if_write_struct_is_delayed( तार्किक change_type_to_base64 );

    शून्य writeRawDataBase64(स्थिरांक शून्य* _data, माप_प्रकार len, स्थिरांक अक्षर* dt );

    String releaseAndGetString();

    File_गाठ get_First_शीर्ष_Level_गाठ() स्थिरांक;

    File_गाठ मूळ(पूर्णांक streamIdx=0) स्थिरांक;

    File_गाठ चालक[](स्थिरांक String& nodename) स्थिरांक;

    File_गाठ चालक[](स्थिरांक अक्षर* /*nodename*/) स्थिरांक;

    आभासी पूर्णांक getFormat() स्थिरांक CV_OVERRIDE;

    अक्षर* bufferPtr() स्थिरांक;
    अक्षर* bufferStart() स्थिरांक;
    अक्षर* bufferEnd() स्थिरांक;
    शून्य setBufferPtr(अक्षर* ptr);
    पूर्णांक wrapMargin() स्थिरांक;

    FStructData& getCurrentStruct();

    शून्य setNonEmpty();

    शून्य processSpecialDouble( अक्षर* buf, द्विगुण* value, अक्षर** endptr );

    द्विगुण म_से_भग्न( अक्षर* ptr, अक्षर** endptr );

    शून्य convertToCollection(पूर्णांक type, File_गाठ& गाठ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_गाठ_Space(File_गाठ& गाठ, माप_प्रकार sz);

    अचिन्हित getStringOfs( स्थिरांक std::string& किल्ली ) स्थिरांक;

    File_गाठ add_गाठ( File_गाठ& collection, स्थिरांक std::string& किल्ली,
                      पूर्णांक elem_type, स्थिरांक शून्य* value, पूर्णांक len );

    शून्य finalizeCollection( File_गाठ& collection );

    शून्य normalize_गाठ_Ofs(माप_प्रकार& blockIdx, माप_प्रकार& ofs) स्थिरांक;

    Base64State get_state_of_writing_base64();

    पूर्णांक get_space();

    वर्ग Base64Decoder
    {
    सार्वजनिक:
        Base64Decoder();
        शून्य init(स्थिरांक Ptr<FileStorageParser>& _parser, अक्षर* _ptr, पूर्णांक _indent);

        तार्किक readMore(पूर्णांक needed);

        uchar getUInt8();

        ushort getUInt16();

        पूर्णांक getInt32();

        int64_t getInt64();

        द्विगुण getFloat64();

        तार्किक endOfStream() स्थिरांक;
        अक्षर* getPtr() स्थिरांक;
    संरक्षित:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() स्थिरांक
        {
            जर (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            परत *parser_do_not_use_direct_dereference;
        }
        अक्षर* ptr;
        पूर्णांक indent;
        std::vector<अक्षर> encoded;
        std::vector<uchar> decoded;
        माप_प्रकार ofs;
        माप_प्रकार totalchars;
        तार्किक eos;
    };

    अक्षर* parseBase64(अक्षर* ptr, पूर्णांक indent, File_गाठ& collection);

    शून्य parseError( स्थिरांक अक्षर* func_name, स्थिरांक std::string& err_संदेश, स्थिरांक अक्षर* source_file, पूर्णांक source_ओळ );

    स्थिरांक uchar* get_गाठ_Ptr(माप_प्रकार blockIdx, माप_प्रकार ofs) स्थिरांक;

    std::string getName( माप_प्रकार nameofs ) स्थिरांक;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    पूर्णांक flags;
    तार्किक empty_stream;

    खाता* file;
    gzFile gzfile;

    तार्किक is_opened;
    तार्किक dummy_eof;
    तार्किक write_mode;
    तार्किक mem_mode;
    पूर्णांक fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    तार्किक is_using_base64;
    तार्किक is_write_struct_delayed;
    अक्षर* delayed_struct_किल्ली;
    पूर्णांक   delayed_struct_flags;
    अक्षर* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    पूर्णांक space, wrap_margin;
    std::deque<FStructData> write_चळत;
    std::vector<अक्षर> buffer;
    माप_प्रकार bufofs;

    std::deque<अक्षर> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        जर (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        परत *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() स्थिरांक
    {
        जर (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        परत *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_गाठ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<माप_प्रकार> fs_data_blksz;
    माप_प्रकार freeSpaceOfs;
    प्रकारव्याख्या std::unordered_map<std::string, अचिन्हित> str_hash_t;
    str_hash_t str_hash;
    std::vector<अक्षर> str_hash_data;

    std::vector<अक्षर> strbufv;
    अक्षर* strbuf;
    माप_प्रकार strbufsize;
    माप_प्रकार strbufpos;
    पूर्णांक lineno;
};

}

#endif
