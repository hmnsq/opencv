// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#അന്തര്ഭാവ "persistence.hpp"
#അന്തര്ഭാവ "persistence_base64_encoding.hpp"
#അന്തര്ഭാവ <unordered_map>
#അന്തര്ഭാവ <iterator>


നാമാകാശ cv
{

എണ്ണൽ Base64State{
    Uncertain,
    NotUse,
    InUse,
};

വർഗ്ഗം cv::FileStorage::Impl : പൊതു FileStorage_API
{
പൊതു:
    ശൂന്യം init();

    Impl(FileStorage* _fs);

    മായാ ~Impl();

    ശൂന്യം release(String* out=0);

    ശൂന്യം analyze_file_name( സ്ഥിരം std::string& file_name, std::vector<std::string>& params );

    തർക്കം open( സ്ഥിരം അക്ഷരം* filename_or_buf, പൂർണ്ണസംഖ്യ _flags, സ്ഥിരം അക്ഷരം* encoding );

    ശൂന്യം puts( സ്ഥിരം അക്ഷരം* str );

    അക്ഷരം* getsFromFile( അക്ഷരം* buf, പൂർണ്ണസംഖ്യ എണ്ണം_2 );

    അക്ഷരം* gets( size_t max_എണ്ണം );

    അക്ഷരം* gets();

    തർക്കം eof();

    ശൂന്യം setEof();

    ശൂന്യം closeFile();

    ശൂന്യം rewind();

    അക്ഷരം* resizeWriteBuffer( അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ len );

    അക്ഷരം* flush();

    ശൂന്യം endWriteStruct();

    ശൂന്യം startWriteStruct_helper( സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags,
                                  സ്ഥിരം അക്ഷരം* type_name );

    ശൂന്യം startWriteStruct( സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags,
                           സ്ഥിരം അക്ഷരം* type_name );

    ശൂന്യം writeComment( സ്ഥിരം അക്ഷരം* comment, തർക്കം eol_comment );

    ശൂന്യം startNextStream();

    ശൂന്യം ലിഖ( സ്ഥിരം String& താക്കോൽ, പൂർണ്ണസംഖ്യ value );

    ശൂന്യം ലിഖ( സ്ഥിരം String& താക്കോൽ, തർക്കം value );

    ശൂന്യം ലിഖ( സ്ഥിരം String& താക്കോൽ, int64_t value );

    ശൂന്യം ലിഖ( സ്ഥിരം String& താക്കോൽ, ഇരട്ട value );

    ശൂന്യം ലിഖ( സ്ഥിരം String& താക്കോൽ, സ്ഥിരം String& value );

    ശൂന്യം writeRawData( സ്ഥിരം std::string& dt, സ്ഥിരം ശൂന്യം* _data, size_t len );

    ശൂന്യം workaround();

    ശൂന്യം switch_to_Base64_state( FileStorage_API::Base64State new_state);

    ശൂന്യം make_write_struct_delayed( സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം* type_name );

    ശൂന്യം check_if_write_struct_is_delayed( തർക്കം change_type_to_base64 );

    ശൂന്യം writeRawDataBase64(സ്ഥിരം ശൂന്യം* _data, size_t len, സ്ഥിരം അക്ഷരം* dt );

    String releaseAndGetString();

    File_കണ്ണി get_First_മുകൾ_Level_കണ്ണി() സ്ഥിരം;

    File_കണ്ണി വേര്(പൂർണ്ണസംഖ്യ streamIdx=0) സ്ഥിരം;

    File_കണ്ണി operator[](സ്ഥിരം String& nodename) സ്ഥിരം;

    File_കണ്ണി operator[](സ്ഥിരം അക്ഷരം* /*nodename*/) സ്ഥിരം;

    മായാ പൂർണ്ണസംഖ്യ getFormat() സ്ഥിരം CV_OVERRIDE;

    അക്ഷരം* bufferPtr() സ്ഥിരം;
    അക്ഷരം* bufferStart() സ്ഥിരം;
    അക്ഷരം* bufferEnd() സ്ഥിരം;
    ശൂന്യം setBufferPtr(അക്ഷരം* ptr);
    പൂർണ്ണസംഖ്യ wrapMargin() സ്ഥിരം;

    FStructData& getCurrentStruct();

    ശൂന്യം setNonEmpty();

    ശൂന്യം processSpecialDouble( അക്ഷരം* buf, ഇരട്ട* value, അക്ഷരം** endptr );

    ഇരട്ട strtod( അക്ഷരം* ptr, അക്ഷരം** endptr );

    ശൂന്യം convertToCollection(പൂർണ്ണസംഖ്യ type, File_കണ്ണി& കണ്ണി);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_കണ്ണി_Space(File_കണ്ണി& കണ്ണി, size_t sz);

    unsigned getStringOfs( സ്ഥിരം std::string& താക്കോൽ ) സ്ഥിരം;

    File_കണ്ണി add_കണ്ണി( File_കണ്ണി& collection, സ്ഥിരം std::string& താക്കോൽ,
                      പൂർണ്ണസംഖ്യ elem_type, സ്ഥിരം ശൂന്യം* value, പൂർണ്ണസംഖ്യ len );

    ശൂന്യം finalizeCollection( File_കണ്ണി& collection );

    ശൂന്യം normalize_കണ്ണി_Ofs(size_t& blockIdx, size_t& ofs) സ്ഥിരം;

    Base64State get_state_of_writing_base64();

    പൂർണ്ണസംഖ്യ get_space();

    വർഗ്ഗം Base64Decoder
    {
    പൊതു:
        Base64Decoder();
        ശൂന്യം init(സ്ഥിരം Ptr<FileStorageParser>& _parser, അക്ഷരം* _ptr, പൂർണ്ണസംഖ്യ _indent);

        തർക്കം readMore(പൂർണ്ണസംഖ്യ needed);

        uchar getUInt8();

        ushort getUInt16();

        പൂർണ്ണസംഖ്യ getInt32();

        int64_t getInt64();

        ഇരട്ട getFloat64();

        തർക്കം endOfStream() സ്ഥിരം;
        അക്ഷരം* getPtr() സ്ഥിരം;
    സംരക്ഷിത:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() സ്ഥിരം
        {
            എങ്കിൽ (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            തിരികെ *parser_do_not_use_direct_dereference;
        }
        അക്ഷരം* ptr;
        പൂർണ്ണസംഖ്യ indent;
        std::vector<അക്ഷരം> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        തർക്കം eos;
    };

    അക്ഷരം* parseBase64(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ indent, File_കണ്ണി& collection);

    ശൂന്യം parseError( സ്ഥിരം അക്ഷരം* func_name, സ്ഥിരം std::string& err_സന്ദേശം, സ്ഥിരം അക്ഷരം* source_file, പൂർണ്ണസംഖ്യ source_വരി );

    സ്ഥിരം uchar* get_കണ്ണി_Ptr(size_t blockIdx, size_t ofs) സ്ഥിരം;

    std::string getName( size_t nameofs ) സ്ഥിരം;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    പൂർണ്ണസംഖ്യ flags;
    തർക്കം empty_stream;

    FILE* file;
    gzFile gzfile;

    തർക്കം is_opened;
    തർക്കം dummy_eof;
    തർക്കം write_mode;
    തർക്കം mem_mode;
    പൂർണ്ണസംഖ്യ fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    തർക്കം is_using_base64;
    തർക്കം is_write_struct_delayed;
    അക്ഷരം* delayed_struct_താക്കോൽ;
    പൂർണ്ണസംഖ്യ   delayed_struct_flags;
    അക്ഷരം* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    പൂർണ്ണസംഖ്യ space, wrap_margin;
    std::deque<FStructData> write_അട്ടി;
    std::vector<അക്ഷരം> buffer;
    size_t bufofs;

    std::deque<അക്ഷരം> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        എങ്കിൽ (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        തിരികെ *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() സ്ഥിരം
    {
        എങ്കിൽ (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        തിരികെ *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_കണ്ണി> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    തരനിർവചനം std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<അക്ഷരം> str_hash_data;

    std::vector<അക്ഷരം> strbufv;
    അക്ഷരം* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    പൂർണ്ണസംഖ്യ lineno;
};

}

#endif
