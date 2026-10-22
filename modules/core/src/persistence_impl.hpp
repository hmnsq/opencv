// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#यदि_अपरिभाषितम् OPENCV_CORE_PERSISTENCE_IMPL_HPP
#परिभाषय OPENCV_CORE_PERSISTENCE_IMPL_HPP

#अन्तर्भाव "persistence.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <iterator>


नामाकाश cv
{

गणना Base64State{
    Uncertain,
    NotUse,
    अन्तर्_Use,
};

वर्ग cv::FileStorage::Impl : सार्वजनिक FileStorage_API
{
सार्वजनिक:
    शून्य init();

    Impl(FileStorage* _fs);

    आभास ~Impl();

    शून्य release(String* out=0);

    शून्य analyze_file_name( ध्रुव std::string& file_name, std::vector<std::string>& params );

    तार्किक उद्घाटय( ध्रुव वर्ण* filename_or_buf, पूर्णाङ्क _flags, ध्रुव वर्ण* encoding );

    शून्य पङ्क्तिं_लिख( ध्रुव वर्ण* str );

    वर्ण* getsFromFile( वर्ण* buf, पूर्णाङ्क गणनम्_2 );

    वर्ण* gets( size_t max_गणनम् );

    वर्ण* gets();

    तार्किक eof();

    शून्य समुच्चयः_Eof();

    शून्य closeFile();

    शून्य rewind();

    वर्ण* resize_Write_बफर( वर्ण* ptr, पूर्णाङ्क len );

    वर्ण* flush();

    शून्य अन्तः_Write_Struct();

    शून्य startWriteStruct_helper( ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क struct_flags,
                                  ध्रुव वर्ण* type_name );

    शून्य startWriteStruct( ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क struct_flags,
                           ध्रुव वर्ण* type_name );

    शून्य writeComment( ध्रुव वर्ण* comment, तार्किक eol_comment );

    शून्य start_अग्र_प्रवाहः();

    शून्य लिख( ध्रुव String& कुञ्चिका, पूर्णाङ्क value );

    शून्य लिख( ध्रुव String& कुञ्चिका, तार्किक value );

    शून्य लिख( ध्रुव String& कुञ्चिका, int64_t value );

    शून्य लिख( ध्रुव String& कुञ्चिका, द्विगुण value );

    शून्य लिख( ध्रुव String& कुञ्चिका, ध्रुव String& value );

    शून्य write_Raw_दत्त( ध्रुव std::string& dt, ध्रुव शून्य* दत्त_3, size_t len );

    शून्य workaround();

    शून्य switch_गन्तव्यम्_Base64_state( FileStorage_API::Base64State new_state);

    शून्य make_write_struct_delayed( ध्रुव वर्ण* कुञ्चिका, पूर्णाङ्क struct_flags, ध्रुव वर्ण* type_name );

    शून्य check_if_write_struct_is_delayed( तार्किक change_type_गन्तव्यम्_base64 );

    शून्य write_Raw_दत्त_Base64(ध्रुव शून्य* दत्त_3, size_t len, ध्रुव वर्ण* dt );

    String releaseAndGetString();

    File_ग्रन्थिः get_First_शिखरम्_Level_ग्रन्थिः() ध्रुव;

    File_ग्रन्थिः मूलम्(पूर्णाङ्क प्रवाहः_Idx=0) ध्रुव;

    File_ग्रन्थिः संक्रियकः[](ध्रुव String& nodename) ध्रुव;

    File_ग्रन्थिः संक्रियकः[](ध्रुव वर्ण* /*nodename*/) ध्रुव;

    आभास पूर्णाङ्क getFormat() ध्रुव CV_OVERRIDE;

    वर्ण* बफर_Ptr() ध्रुव;
    वर्ण* बफर_Start() ध्रुव;
    वर्ण* बफर_अन्तः_2() ध्रुव;
    शून्य समुच्चयः_बफर_Ptr(वर्ण* ptr);
    पूर्णाङ्क wrapMargin() ध्रुव;

    FStruct_दत्त& getCurrentStruct();

    शून्य समुच्चयः_Non_रिक्तानि();

    शून्य processSpecialDouble( वर्ण* buf, द्विगुण* value, वर्ण** endptr );

    द्विगुण strtod( वर्ण* ptr, वर्ण** endptr );

    शून्य convert_गन्तव्यम्_Collection(पूर्णाङ्क type, File_ग्रन्थिः& ग्रन्थिः);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ग्रन्थिः_Space(File_ग्रन्थिः& ग्रन्थिः, size_t sz);

    अचिह्नित getStringOfs( ध्रुव std::string& कुञ्चिका ) ध्रुव;

    File_ग्रन्थिः सङ्कलय_ग्रन्थिः( File_ग्रन्थिः& collection, ध्रुव std::string& कुञ्चिका,
                      पूर्णाङ्क elem_type, ध्रुव शून्य* value, पूर्णाङ्क len );

    शून्य finalizeCollection( File_ग्रन्थिः& collection );

    शून्य normalize_ग्रन्थिः_Ofs(size_t& खण्डम्_Idx, size_t& ofs) ध्रुव;

    Base64State get_state_सम्बन्धि_writing_base64();

    पूर्णाङ्क get_space();

    वर्ग Base64Decoder
    {
    सार्वजनिक:
        Base64Decoder();
        शून्य init(ध्रुव Ptr<FileStorageParser>& _parser, वर्ण* _ptr, पूर्णाङ्क _indent);

        तार्किक read_अधिकम्(पूर्णाङ्क needed);

        uchar getUInt8();

        ushort getUInt16();

        पूर्णाङ्क getInt32();

        int64_t getInt64();

        द्विगुण getFloat64();

        तार्किक अन्तः_सम्बन्धि_प्रवाहः() ध्रुव;
        वर्ण* getPtr() ध्रुव;
    रक्षित:

        Ptr<FileStorageParser> parser_do_not_use_प्रत्यक्षम्_dereference;
        FileStorageParser& getParser() ध्रुव
        {
            यदि (!parser_do_not_use_प्रत्यक्षम्_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            प्रत्यागम *parser_do_not_use_प्रत्यक्षम्_dereference;
        }
        वर्ण* ptr;
        पूर्णाङ्क indent;
        std::vector<वर्ण> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        तार्किक eos;
    };

    वर्ण* parseBase64(वर्ण* ptr, पूर्णाङ्क indent, File_ग्रन्थिः& collection);

    शून्य parseError( ध्रुव वर्ण* func_name, ध्रुव std::string& err_सन्देशः, ध्रुव वर्ण* source_file, पूर्णाङ्क source_पङ्क्तिः );

    ध्रुव uchar* get_ग्रन्थिः_Ptr(size_t खण्डम्_Idx, size_t ofs) ध्रुव;

    std::string getName( size_t nameofs ) ध्रुव;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    पूर्णाङ्क flags;
    तार्किक रिक्तानि_प्रवाहः;

    FILE* file;
    gzFile gzfile;

    तार्किक is_opened;
    तार्किक dummy_eof;
    तार्किक write_बहुलकम्;
    तार्किक mem_बहुलकम्;
    पूर्णाङ्क fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    तार्किक is_using_base64;
    तार्किक is_write_struct_delayed;
    वर्ण* delayed_struct_कुञ्चिका;
    पूर्णाङ्क   delayed_struct_flags;
    वर्ण* delayed_type_name;
    FileStorage_API::Base64State state_सम्बन्धि_writing_base64;

    पूर्णाङ्क space, wrap_margin;
    std::deque<FStruct_दत्त> write_राशिः;
    std::vector<वर्ण> बफर;
    size_t bufofs;

    std::deque<वर्ण> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_प्रत्यक्षम्_dereference;
    FileStorageEmitter& getEmitter()
    {
        यदि (!emitter_do_not_use_प्रत्यक्षम्_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        प्रत्यागम *emitter_do_not_use_प्रत्यक्षम्_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_प्रत्यक्षम्_dereference;
    FileStorageParser& getParser() ध्रुव
    {
        यदि (!parser_do_not_use_प्रत्यक्षम्_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        प्रत्यागम *parser_do_not_use_प्रत्यक्षम्_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ग्रन्थिः> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_दत्त;
    std::vector<uchar*> fs_दत्त_ptrs;
    std::vector<size_t> fs_दत्त_blksz;
    size_t freeSpaceOfs;
    प्रकारनिर्देश std::unordered_map<std::string, अचिह्नित> str_hash_t;
    str_hash_t str_hash;
    std::vector<वर्ण> str_hash_दत्त;

    std::vector<वर्ण> strbufv;
    वर्ण* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    पूर्णाङ्क lineno;
};

}

#यदि_अन्तः
