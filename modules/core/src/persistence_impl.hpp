// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#నిర్వచితం_కాకపోతే OPENCV_CORE_PERSISTENCE_IMPL_HPP
#నిర్వచించు OPENCV_CORE_PERSISTENCE_IMPL_HPP

#అన్తర్భావ "persistence.hpp"
#అన్తర్భావ "persistence_base64_encoding.hpp"
#అన్తర్భావ <unordered_map>
#అన్తర్భావ <iterator>


నామాకాశ cv
{

గణన Base64State{
    Uncertain,
    NotUse,
    లోపల_Use,
};

తరగతి cv::FileStorage::Impl : బహిరంగ FileStorage_API
{
బహిరంగ:
    శూన్య init();

    Impl(FileStorage* _fs);

    మాయా ~Impl();

    శూన్య release(String* out=0);

    శూన్య analyze_file_name( స్థిరరాశి std::string& file_name, std::vector<std::string>& params );

    తార్కిక open( స్థిరరాశి అక్షరం* filename_or_buf, పూర్ణాంకం _flags, స్థిరరాశి అక్షరం* encoding );

    శూన్య puts( స్థిరరాశి అక్షరం* str );

    అక్షరం* getsFromFile( అక్షరం* buf, పూర్ణాంకం లెక్క_2 );

    అక్షరం* gets( size_t max_లెక్క );

    అక్షరం* gets();

    తార్కిక eof();

    శూన్య సమితి_Eof();

    శూన్య closeFile();

    శూన్య rewind();

    అక్షరం* resize_Write_బఫరు( అక్షరం* ptr, పూర్ణాంకం len );

    అక్షరం* flush();

    శూన్య చివర_Write_Struct();

    శూన్య ప్రారంభం_Write_Struct_helper( స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం struct_flags,
                                  స్థిరరాశి అక్షరం* type_name );

    శూన్య ప్రారంభం_Write_Struct( స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం struct_flags,
                           స్థిరరాశి అక్షరం* type_name );

    శూన్య writeComment( స్థిరరాశి అక్షరం* comment, తార్కిక eol_comment );

    శూన్య ప్రారంభం_తదుపరి_ప్రవాహం();

    శూన్య లిఖ( స్థిరరాశి String& కీలకం, పూర్ణాంకం value );

    శూన్య లిఖ( స్థిరరాశి String& కీలకం, తార్కిక value );

    శూన్య లిఖ( స్థిరరాశి String& కీలకం, int64_t value );

    శూన్య లిఖ( స్థిరరాశి String& కీలకం, ద్విగుణ value );

    శూన్య లిఖ( స్థిరరాశి String& కీలకం, స్థిరరాశి String& value );

    శూన్య write_Raw_దత్తాంశం( స్థిరరాశి std::string& dt, స్థిరరాశి శూన్య* దత్తాంశం_3, size_t len );

    శూన్య workaround();

    శూన్య switch_గమ్యం_Base64_state( FileStorage_API::Base64State new_state);

    శూన్య make_write_struct_delayed( స్థిరరాశి అక్షరం* కీలకం, పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం* type_name );

    శూన్య check_if_write_struct_is_delayed( తార్కిక change_type_గమ్యం_base64 );

    శూన్య write_Raw_దత్తాంశం_Base64(స్థిరరాశి శూన్య* దత్తాంశం_3, size_t len, స్థిరరాశి అక్షరం* dt );

    String releaseAndGetString();

    File_కణుపు get_మొదటి_శిఖరం_Level_కణుపు() స్థిరరాశి;

    File_కణుపు మూలం(పూర్ణాంకం ప్రవాహం_Idx=0) స్థిరరాశి;

    File_కణుపు క్రియాకారకం[](స్థిరరాశి String& nodename) స్థిరరాశి;

    File_కణుపు క్రియాకారకం[](స్థిరరాశి అక్షరం* /*nodename*/) స్థిరరాశి;

    మాయా పూర్ణాంకం getFormat() స్థిరరాశి CV_OVERRIDE;

    అక్షరం* బఫరు_Ptr() స్థిరరాశి;
    అక్షరం* బఫరు_ప్రారంభం() స్థిరరాశి;
    అక్షరం* బఫరు_చివర_2() స్థిరరాశి;
    శూన్య సమితి_బఫరు_Ptr(అక్షరం* ptr);
    పూర్ణాంకం wrapMargin() స్థిరరాశి;

    FStruct_దత్తాంశం& getCurrentStruct();

    శూన్య సమితి_Non_ఖాళీలు();

    శూన్య processSpecialDouble( అక్షరం* buf, ద్విగుణ* value, అక్షరం** endptr );

    ద్విగుణ strtod( అక్షరం* ptr, అక్షరం** endptr );

    శూన్య convert_గమ్యం_Collection(పూర్ణాంకం type, File_కణుపు& కణుపు);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_కణుపు_Space(File_కణుపు& కణుపు, size_t sz);

    గుర్తులేని getStringOfs( స్థిరరాశి std::string& కీలకం ) స్థిరరాశి;

    File_కణుపు కూడు_కణుపు( File_కణుపు& collection, స్థిరరాశి std::string& కీలకం,
                      పూర్ణాంకం elem_type, స్థిరరాశి శూన్య* value, పూర్ణాంకం len );

    శూన్య finalizeCollection( File_కణుపు& collection );

    శూన్య normalize_కణుపు_Ofs(size_t& ఖండం_Idx, size_t& ofs) స్థిరరాశి;

    Base64State get_state_యొక్క_writing_base64();

    పూర్ణాంకం get_space();

    తరగతి Base64Decoder
    {
    బహిరంగ:
        Base64Decoder();
        శూన్య init(స్థిరరాశి Ptr<FileStorageParser>& _parser, అక్షరం* _ptr, పూర్ణాంకం _indent);

        తార్కిక read_మరిన్ని(పూర్ణాంకం needed);

        uchar getUInt8();

        ushort getUInt16();

        పూర్ణాంకం getInt32();

        int64_t getInt64();

        ద్విగుణ getFloat64();

        తార్కిక చివర_యొక్క_ప్రవాహం() స్థిరరాశి;
        అక్షరం* getPtr() స్థిరరాశి;
    రక్షిత:

        Ptr<FileStorageParser> parser_do_not_use_ప్రత్యక్ష_dereference;
        FileStorageParser& getParser() స్థిరరాశి
        {
            ఒకవేళ (!parser_do_not_use_ప్రత్యక్ష_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            తిరిగి *parser_do_not_use_ప్రత్యక్ష_dereference;
        }
        అక్షరం* ptr;
        పూర్ణాంకం indent;
        std::vector<అక్షరం> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        తార్కిక eos;
    };

    అక్షరం* parseBase64(అక్షరం* ptr, పూర్ణాంకం indent, File_కణుపు& collection);

    శూన్య parseError( స్థిరరాశి అక్షరం* func_name, స్థిరరాశి std::string& err_సందేశం, స్థిరరాశి అక్షరం* ఆధారం_file, పూర్ణాంకం ఆధారం_పంక్తి );

    స్థిరరాశి uchar* get_కణుపు_Ptr(size_t ఖండం_Idx, size_t ofs) స్థిరరాశి;

    std::string getName( size_t nameofs ) స్థిరరాశి;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    పూర్ణాంకం flags;
    తార్కిక ఖాళీలు_ప్రవాహం;

    FILE* file;
    gzFile gzfile;

    తార్కిక is_opened;
    తార్కిక dummy_eof;
    తార్కిక write_బహుళకం;
    తార్కిక mem_బహుళకం;
    పూర్ణాంకం fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    తార్కిక is_using_base64;
    తార్కిక is_write_struct_delayed;
    అక్షరం* delayed_struct_కీలకం;
    పూర్ణాంకం   delayed_struct_flags;
    అక్షరం* delayed_type_name;
    FileStorage_API::Base64State state_యొక్క_writing_base64;

    పూర్ణాంకం space, wrap_margin;
    std::deque<FStruct_దత్తాంశం> write_దొంతర;
    std::vector<అక్షరం> బఫరు;
    size_t bufofs;

    std::deque<అక్షరం> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_ప్రత్యక్ష_dereference;
    FileStorageEmitter& getEmitter()
    {
        ఒకవేళ (!emitter_do_not_use_ప్రత్యక్ష_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        తిరిగి *emitter_do_not_use_ప్రత్యక్ష_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_ప్రత్యక్ష_dereference;
    FileStorageParser& getParser() స్థిరరాశి
    {
        ఒకవేళ (!parser_do_not_use_ప్రత్యక్ష_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        తిరిగి *parser_do_not_use_ప్రత్యక్ష_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_కణుపు> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_దత్తాంశం;
    std::vector<uchar*> fs_దత్తాంశం_ptrs;
    std::vector<size_t> fs_దత్తాంశం_blksz;
    size_t freeSpaceOfs;
    రకనిర్వచనం std::unordered_map<std::string, గుర్తులేని> str_hash_t;
    str_hash_t str_hash;
    std::vector<అక్షరం> str_hash_దత్తాంశం;

    std::vector<అక్షరం> strbufv;
    అక్షరం* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    పూర్ణాంకం lineno;
};

}

#అయితే_ముగింపు
