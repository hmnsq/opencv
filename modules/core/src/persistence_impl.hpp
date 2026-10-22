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

    शून्य analyze_file_name( ध्रुव std::string& file_name, std::vector<std::string>& params );

    तार्किक open( ध्रुव वर्ण* filename_or_buf, पूर्णाङ्क _flags, ध्रुव वर्ण* encoding );

    शून्य puts( ध्रुव वर्ण* str );

    वर्ण* getsFromFile( वर्ण* buf, पूर्णाङ्क गिनती_2 );

    वर्ण* gets( size_t max_गिनती );

    वर्ण* gets();

    तार्किक eof();

    शून्य setEof();

    शून्य closeFile();

    शून्य rewind();

    वर्ण* resizeWriteBuffer( वर्ण* ptr, पूर्णाङ्क len );

    वर्ण* flush();

    शून्य endWriteStruct();

    शून्य startWriteStruct_helper( ध्रुव वर्ण* कुंजी, पूर्णाङ्क struct_flags,
                                  ध्रुव वर्ण* type_name );

    शून्य startWriteStruct( ध्रुव वर्ण* कुंजी, पूर्णाङ्क struct_flags,
                           ध्रुव वर्ण* type_name );

    शून्य writeComment( ध्रुव वर्ण* comment, तार्किक eol_comment );

    शून्य startNextStream();

    शून्य लिख( ध्रुव String& कुंजी, पूर्णाङ्क value );

    शून्य लिख( ध्रुव String& कुंजी, तार्किक value );

    शून्य लिख( ध्रुव String& कुंजी, int64_t value );

    शून्य लिख( ध्रुव String& कुंजी, द्विगुण value );

    शून्य लिख( ध्रुव String& कुंजी, ध्रुव String& value );

    शून्य writeRawData( ध्रुव std::string& dt, ध्रुव शून्य* _data, size_t len );

    शून्य workaround();

    शून्य switch_to_Base64_state( FileStorage_API::Base64State new_state);

    शून्य make_write_struct_delayed( ध्रुव वर्ण* कुंजी, पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name );

    शून्य check_if_write_struct_is_delayed( तार्किक change_type_to_base64 );

    शून्य writeRawDataBase64(ध्रुव शून्य* _data, size_t len, ध्रुव वर्ण* dt );

    String releaseAndGetString();

    File_गाँठ get_First_शीर्ष_Level_गाँठ() ध्रुव;

    File_गाँठ मूल(पूर्णाङ्क streamIdx=0) ध्रुव;

    File_गाँठ operator[](ध्रुव String& nodename) ध्रुव;

    File_गाँठ operator[](ध्रुव वर्ण* /*nodename*/) ध्रुव;

    आभास पूर्णाङ्क getFormat() ध्रुव CV_OVERRIDE;

    वर्ण* bufferPtr() ध्रुव;
    वर्ण* bufferStart() ध्रुव;
    वर्ण* bufferEnd() ध्रुव;
    शून्य setBufferPtr(वर्ण* ptr);
    पूर्णाङ्क wrapMargin() ध्रुव;

    FStructData& getCurrentStruct();

    शून्य setNonEmpty();

    शून्य processSpecialDouble( वर्ण* buf, द्विगुण* value, वर्ण** endptr );

    द्विगुण strtod( वर्ण* ptr, वर्ण** endptr );

    शून्य convertToCollection(पूर्णाङ्क type, File_गाँठ& गाँठ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_गाँठ_Space(File_गाँठ& गाँठ, size_t sz);

    unsigned getStringOfs( ध्रुव std::string& कुंजी ) ध्रुव;

    File_गाँठ add_गाँठ( File_गाँठ& collection, ध्रुव std::string& कुंजी,
                      पूर्णाङ्क elem_type, ध्रुव शून्य* value, पूर्णाङ्क len );

    शून्य finalizeCollection( File_गाँठ& collection );

    शून्य normalize_गाँठ_Ofs(size_t& blockIdx, size_t& ofs) ध्रुव;

    Base64State get_state_of_writing_base64();

    पूर्णाङ्क get_space();

    वर्ग Base64Decoder
    {
    सार्वजनिक:
        Base64Decoder();
        शून्य init(ध्रुव Ptr<FileStorageParser>& _parser, वर्ण* _ptr, पूर्णाङ्क _indent);

        तार्किक readMore(पूर्णाङ्क needed);

        uchar getUInt8();

        ushort getUInt16();

        पूर्णाङ्क getInt32();

        int64_t getInt64();

        द्विगुण getFloat64();

        तार्किक endOfStream() ध्रुव;
        वर्ण* getPtr() ध्रुव;
    रक्षित:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() ध्रुव
        {
            यदि (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            प्रत्यागम *parser_do_not_use_direct_dereference;
        }
        वर्ण* ptr;
        पूर्णाङ्क indent;
        std::vector<वर्ण> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        तार्किक eos;
    };

    वर्ण* parseBase64(वर्ण* ptr, पूर्णाङ्क indent, File_गाँठ& collection);

    शून्य parseError( ध्रुव वर्ण* func_name, ध्रुव std::string& err_संदेश, ध्रुव वर्ण* source_file, पूर्णाङ्क source_पंक्ति );

    ध्रुव uchar* get_गाँठ_Ptr(size_t blockIdx, size_t ofs) ध्रुव;

    std::string getName( size_t nameofs ) ध्रुव;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    पूर्णाङ्क flags;
    तार्किक empty_stream;

    FILE* file;
    gzFile gzfile;

    तार्किक is_opened;
    तार्किक dummy_eof;
    तार्किक write_mode;
    तार्किक mem_mode;
    पूर्णाङ्क fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    तार्किक is_using_base64;
    तार्किक is_write_struct_delayed;
    वर्ण* delayed_struct_कुंजी;
    पूर्णाङ्क   delayed_struct_flags;
    वर्ण* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    पूर्णाङ्क space, wrap_margin;
    std::deque<FStructData> write_ढेर;
    std::vector<वर्ण> buffer;
    size_t bufofs;

    std::deque<वर्ण> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        यदि (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        प्रत्यागम *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() ध्रुव
    {
        यदि (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        प्रत्यागम *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_गाँठ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    प्रकारनिर्देश std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<वर्ण> str_hash_data;

    std::vector<वर्ण> strbufv;
    वर्ण* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    पूर्णाङ्क lineno;
};

}

#endif
