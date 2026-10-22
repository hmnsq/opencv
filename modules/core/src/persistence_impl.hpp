// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#அந்தர்பாவ "persistence.hpp"
#அந்தர்பாவ "persistence_base64_encoding.hpp"
#அந்தர்பாவ <unordered_map>
#அந்தர்பாவ <iterator>


நாமாகாஶ cv
{

எண்ணிக்கை Base64State{
    Uncertain,
    NotUse,
    InUse,
};

வகுப்பு cv::FileStorage::Impl : பொது FileStorage_API
{
பொது:
    வெறுமை init();

    Impl(FileStorage* _fs);

    மெய்நிகர் ~Impl();

    வெறுமை release(String* out=0);

    வெறுமை analyze_file_name( மாறிலி std::string& file_name, std::vector<std::string>& params );

    தருக்க open( மாறிலி எழுத்து* filename_or_buf, முழு _flags, மாறிலி எழுத்து* encoding );

    வெறுமை puts( மாறிலி எழுத்து* str );

    எழுத்து* getsFromFile( எழுத்து* buf, முழு மொத்தம்_2 );

    எழுத்து* gets( size_t max_மொத்தம் );

    எழுத்து* gets();

    தருக்க eof();

    வெறுமை setEof();

    வெறுமை closeFile();

    வெறுமை rewind();

    எழுத்து* resizeWriteBuffer( எழுத்து* ptr, முழு len );

    எழுத்து* flush();

    வெறுமை endWriteStruct();

    வெறுமை startWriteStruct_helper( மாறிலி எழுத்து* சாவி, முழு struct_flags,
                                  மாறிலி எழுத்து* type_name );

    வெறுமை startWriteStruct( மாறிலி எழுத்து* சாவி, முழு struct_flags,
                           மாறிலி எழுத்து* type_name );

    வெறுமை writeComment( மாறிலி எழுத்து* comment, தருக்க eol_comment );

    வெறுமை startNextStream();

    வெறுமை லிக( மாறிலி String& சாவி, முழு value );

    வெறுமை லிக( மாறிலி String& சாவி, தருக்க value );

    வெறுமை லிக( மாறிலி String& சாவி, int64_t value );

    வெறுமை லிக( மாறிலி String& சாவி, இரட்டை value );

    வெறுமை லிக( மாறிலி String& சாவி, மாறிலி String& value );

    வெறுமை writeRawData( மாறிலி std::string& dt, மாறிலி வெறுமை* _data, size_t len );

    வெறுமை workaround();

    வெறுமை switch_to_Base64_state( FileStorage_API::Base64State new_state);

    வெறுமை make_write_struct_delayed( மாறிலி எழுத்து* சாவி, முழு struct_flags, மாறிலி எழுத்து* type_name );

    வெறுமை check_if_write_struct_is_delayed( தருக்க change_type_to_base64 );

    வெறுமை writeRawDataBase64(மாறிலி வெறுமை* _data, size_t len, மாறிலி எழுத்து* dt );

    String releaseAndGetString();

    File_முனை get_First_உச்சி_Level_முனை() மாறிலி;

    File_முனை வேர்(முழு streamIdx=0) மாறிலி;

    File_முனை operator[](மாறிலி String& nodename) மாறிலி;

    File_முனை operator[](மாறிலி எழுத்து* /*nodename*/) மாறிலி;

    மெய்நிகர் முழு getFormat() மாறிலி CV_OVERRIDE;

    எழுத்து* bufferPtr() மாறிலி;
    எழுத்து* bufferStart() மாறிலி;
    எழுத்து* bufferEnd() மாறிலி;
    வெறுமை setBufferPtr(எழுத்து* ptr);
    முழு wrapMargin() மாறிலி;

    FStructData& getCurrentStruct();

    வெறுமை setNonEmpty();

    வெறுமை processSpecialDouble( எழுத்து* buf, இரட்டை* value, எழுத்து** endptr );

    இரட்டை strtod( எழுத்து* ptr, எழுத்து** endptr );

    வெறுமை convertToCollection(முழு type, File_முனை& முனை);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_முனை_Space(File_முனை& முனை, size_t sz);

    unsigned getStringOfs( மாறிலி std::string& சாவி ) மாறிலி;

    File_முனை add_முனை( File_முனை& collection, மாறிலி std::string& சாவி,
                      முழு elem_type, மாறிலி வெறுமை* value, முழு len );

    வெறுமை finalizeCollection( File_முனை& collection );

    வெறுமை normalize_முனை_Ofs(size_t& blockIdx, size_t& ofs) மாறிலி;

    Base64State get_state_of_writing_base64();

    முழு get_space();

    வகுப்பு Base64Decoder
    {
    பொது:
        Base64Decoder();
        வெறுமை init(மாறிலி Ptr<FileStorageParser>& _parser, எழுத்து* _ptr, முழு _indent);

        தருக்க readMore(முழு needed);

        uchar getUInt8();

        ushort getUInt16();

        முழு getInt32();

        int64_t getInt64();

        இரட்டை getFloat64();

        தருக்க endOfStream() மாறிலி;
        எழுத்து* getPtr() மாறிலி;
    பாதுகாக்கப்பட்ட:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() மாறிலி
        {
            எனில் (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            திரும்பு *parser_do_not_use_direct_dereference;
        }
        எழுத்து* ptr;
        முழு indent;
        std::vector<எழுத்து> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        தருக்க eos;
    };

    எழுத்து* parseBase64(எழுத்து* ptr, முழு indent, File_முனை& collection);

    வெறுமை parseError( மாறிலி எழுத்து* func_name, மாறிலி std::string& err_செய்தி, மாறிலி எழுத்து* source_file, முழு source_வரி );

    மாறிலி uchar* get_முனை_Ptr(size_t blockIdx, size_t ofs) மாறிலி;

    std::string getName( size_t nameofs ) மாறிலி;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    முழு flags;
    தருக்க empty_stream;

    FILE* file;
    gzFile gzfile;

    தருக்க is_opened;
    தருக்க dummy_eof;
    தருக்க write_mode;
    தருக்க mem_mode;
    முழு fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    தருக்க is_using_base64;
    தருக்க is_write_struct_delayed;
    எழுத்து* delayed_struct_சாவி;
    முழு   delayed_struct_flags;
    எழுத்து* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    முழு space, wrap_margin;
    std::deque<FStructData> write_அடுக்கு;
    std::vector<எழுத்து> buffer;
    size_t bufofs;

    std::deque<எழுத்து> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        எனில் (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        திரும்பு *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() மாறிலி
    {
        எனில் (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        திரும்பு *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_முனை> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    வகைவரை std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<எழுத்து> str_hash_data;

    std::vector<எழுத்து> strbufv;
    எழுத்து* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    முழு lineno;
};

}

#endif
