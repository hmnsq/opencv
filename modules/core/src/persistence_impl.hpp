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

    आभास ~Impl();

    शून्य release(String* out=0);

    शून्य analyze_file_name( अचर std::string& file_name, std::vector<std::string>& params );

    तार्किक खोलो( अचर वर्ण* filename_or_buf, पूर्णाङ्क _flags, अचर वर्ण* encoding );

    शून्य माला_दो( अचर वर्ण* str );

    वर्ण* getsFromFile( वर्ण* buf, पूर्णाङ्क गन्ती_2 );

    वर्ण* माला_लो( माप_प्रकार max_गन्ती );

    वर्ण* माला_लो();

    तार्किक eof();

    शून्य setEof();

    शून्य closeFile();

    शून्य शुरुआत();

    वर्ण* resizeWriteBuffer( वर्ण* ptr, पूर्णाङ्क len );

    वर्ण* flush();

    शून्य endWriteStruct();

    शून्य startWriteStruct_helper( अचर वर्ण* साँचो, पूर्णाङ्क struct_flags,
                                  अचर वर्ण* type_name );

    शून्य startWriteStruct( अचर वर्ण* साँचो, पूर्णाङ्क struct_flags,
                           अचर वर्ण* type_name );

    शून्य writeComment( अचर वर्ण* comment, तार्किक eol_comment );

    शून्य startNextStream();

    शून्य लिख( अचर String& साँचो, पूर्णाङ्क value );

    शून्य लिख( अचर String& साँचो, तार्किक value );

    शून्य लिख( अचर String& साँचो, int64_t value );

    शून्य लिख( अचर String& साँचो, द्विगुण value );

    शून्य लिख( अचर String& साँचो, अचर String& value );

    शून्य writeRawData( अचर std::string& dt, अचर शून्य* _data, माप_प्रकार len );

    शून्य workaround();

    शून्य switch_to_Base64_state( FileStorage_API::Base64State new_state);

    शून्य make_write_struct_delayed( अचर वर्ण* साँचो, पूर्णाङ्क struct_flags, अचर वर्ण* type_name );

    शून्य check_if_write_struct_is_delayed( तार्किक change_type_to_base64 );

    शून्य writeRawDataBase64(अचर शून्य* _data, माप_प्रकार len, अचर वर्ण* dt );

    String releaseAndGetString();

    File_गाँठो get_First_शीर्ष_Level_गाँठो() अचर;

    File_गाँठो जरा(पूर्णाङ्क streamIdx=0) अचर;

    File_गाँठो चालक[](अचर String& nodename) अचर;

    File_गाँठो चालक[](अचर वर्ण* /*nodename*/) अचर;

    आभास पूर्णाङ्क getFormat() अचर CV_OVERRIDE;

    वर्ण* bufferPtr() अचर;
    वर्ण* bufferStart() अचर;
    वर्ण* bufferEnd() अचर;
    शून्य setBufferPtr(वर्ण* ptr);
    पूर्णाङ्क wrapMargin() अचर;

    FStructData& getCurrentStruct();

    शून्य setNonEmpty();

    शून्य processSpecialDouble( वर्ण* buf, द्विगुण* value, वर्ण** endptr );

    द्विगुण म_से_भग्न( वर्ण* ptr, वर्ण** endptr );

    शून्य convertToCollection(पूर्णाङ्क type, File_गाँठो& गाँठो);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_गाँठो_Space(File_गाँठो& गाँठो, माप_प्रकार sz);

    अचिन्हित getStringOfs( अचर std::string& साँचो ) अचर;

    File_गाँठो add_गाँठो( File_गाँठो& collection, अचर std::string& साँचो,
                      पूर्णाङ्क elem_type, अचर शून्य* value, पूर्णाङ्क len );

    शून्य finalizeCollection( File_गाँठो& collection );

    शून्य normalize_गाँठो_Ofs(माप_प्रकार& blockIdx, माप_प्रकार& ofs) अचर;

    Base64State get_state_of_writing_base64();

    पूर्णाङ्क get_space();

    वर्ग Base64Decoder
    {
    सार्वजनिक:
        Base64Decoder();
        शून्य init(अचर Ptr<FileStorageParser>& _parser, वर्ण* _ptr, पूर्णाङ्क _indent);

        तार्किक readMore(पूर्णाङ्क needed);

        uchar getUInt8();

        ushort getUInt16();

        पूर्णाङ्क getInt32();

        int64_t getInt64();

        द्विगुण getFloat64();

        तार्किक endOfStream() अचर;
        वर्ण* getPtr() अचर;
    रक्षित:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() अचर
        {
            यदि (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            फर्काऊ *parser_do_not_use_direct_dereference;
        }
        वर्ण* ptr;
        पूर्णाङ्क indent;
        std::vector<वर्ण> encoded;
        std::vector<uchar> decoded;
        माप_प्रकार ofs;
        माप_प्रकार totalchars;
        तार्किक eos;
    };

    वर्ण* parseBase64(वर्ण* ptr, पूर्णाङ्क indent, File_गाँठो& collection);

    शून्य parseError( अचर वर्ण* func_name, अचर std::string& err_सन्देश, अचर वर्ण* source_file, पूर्णाङ्क source_पङ्क्ति );

    अचर uchar* get_गाँठो_Ptr(माप_प्रकार blockIdx, माप_प्रकार ofs) अचर;

    std::string getName( माप_प्रकार nameofs ) अचर;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    पूर्णाङ्क flags;
    तार्किक empty_stream;

    खाता* file;
    gzFile gzfile;

    तार्किक is_opened;
    तार्किक dummy_eof;
    तार्किक write_mode;
    तार्किक mem_mode;
    पूर्णाङ्क fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    तार्किक is_using_base64;
    तार्किक is_write_struct_delayed;
    वर्ण* delayed_struct_साँचो;
    पूर्णाङ्क   delayed_struct_flags;
    वर्ण* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    पूर्णाङ्क space, wrap_margin;
    std::deque<FStructData> write_थाक;
    std::vector<वर्ण> buffer;
    माप_प्रकार bufofs;

    std::deque<वर्ण> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        यदि (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        फर्काऊ *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() अचर
    {
        यदि (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        फर्काऊ *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_गाँठो> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<माप_प्रकार> fs_data_blksz;
    माप_प्रकार freeSpaceOfs;
    प्रकारनिर्देश std::unordered_map<std::string, अचिन्हित> str_hash_t;
    str_hash_t str_hash;
    std::vector<वर्ण> str_hash_data;

    std::vector<वर्ण> strbufv;
    वर्ण* strbuf;
    माप_प्रकार strbufsize;
    माप_प्रकार strbufpos;
    पूर्णाङ्क lineno;
};

}

#endif
