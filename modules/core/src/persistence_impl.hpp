// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#અન્તર્ભાવ "persistence.hpp"
#અન્તર્ભાવ "persistence_base64_encoding.hpp"
#અન્તર્ભાવ <unordered_map>
#અન્તર્ભાવ <iterator>


નામાકાશ cv
{

ગણના Base64State{
    Uncertain,
    NotUse,
    InUse,
};

વર્ગ cv::FileStorage::Impl : જાહેર FileStorage_API
{
જાહેર:
    શૂન્ય init();

    Impl(FileStorage* _fs);

    આભાસી ~Impl();

    શૂન્ય release(String* out=0);

    શૂન્ય analyze_file_name( અચલ std::string& file_name, std::vector<std::string>& params );

    તાર્કિક open( અચલ અક્ષર* filename_or_buf, પૂર્ણાંક _flags, અચલ અક્ષર* encoding );

    શૂન્ય puts( અચલ અક્ષર* str );

    અક્ષર* getsFromFile( અક્ષર* buf, પૂર્ણાંક ગણતરી_2 );

    અક્ષર* gets( size_t max_ગણતરી );

    અક્ષર* gets();

    તાર્કિક eof();

    શૂન્ય setEof();

    શૂન્ય closeFile();

    શૂન્ય rewind();

    અક્ષર* resizeWriteBuffer( અક્ષર* ptr, પૂર્ણાંક len );

    અક્ષર* flush();

    શૂન્ય endWriteStruct();

    શૂન્ય startWriteStruct_helper( અચલ અક્ષર* ચાવી, પૂર્ણાંક struct_flags,
                                  અચલ અક્ષર* type_name );

    શૂન્ય startWriteStruct( અચલ અક્ષર* ચાવી, પૂર્ણાંક struct_flags,
                           અચલ અક્ષર* type_name );

    શૂન્ય writeComment( અચલ અક્ષર* comment, તાર્કિક eol_comment );

    શૂન્ય startNextStream();

    શૂન્ય લિખ( અચલ String& ચાવી, પૂર્ણાંક value );

    શૂન્ય લિખ( અચલ String& ચાવી, તાર્કિક value );

    શૂન્ય લિખ( અચલ String& ચાવી, int64_t value );

    શૂન્ય લિખ( અચલ String& ચાવી, દ્વિગુણ value );

    શૂન્ય લિખ( અચલ String& ચાવી, અચલ String& value );

    શૂન્ય writeRawData( અચલ std::string& dt, અચલ શૂન્ય* _data, size_t len );

    શૂન્ય workaround();

    શૂન્ય switch_to_Base64_state( FileStorage_API::Base64State new_state);

    શૂન્ય make_write_struct_delayed( અચલ અક્ષર* ચાવી, પૂર્ણાંક struct_flags, અચલ અક્ષર* type_name );

    શૂન્ય check_if_write_struct_is_delayed( તાર્કિક change_type_to_base64 );

    શૂન્ય writeRawDataBase64(અચલ શૂન્ય* _data, size_t len, અચલ અક્ષર* dt );

    String releaseAndGetString();

    File_ગાંઠ get_First_ટોચ_Level_ગાંઠ() અચલ;

    File_ગાંઠ મૂળ(પૂર્ણાંક streamIdx=0) અચલ;

    File_ગાંઠ operator[](અચલ String& nodename) અચલ;

    File_ગાંઠ operator[](અચલ અક્ષર* /*nodename*/) અચલ;

    આભાસી પૂર્ણાંક getFormat() અચલ CV_OVERRIDE;

    અક્ષર* bufferPtr() અચલ;
    અક્ષર* bufferStart() અચલ;
    અક્ષર* bufferEnd() અચલ;
    શૂન્ય setBufferPtr(અક્ષર* ptr);
    પૂર્ણાંક wrapMargin() અચલ;

    FStructData& getCurrentStruct();

    શૂન્ય setNonEmpty();

    શૂન્ય processSpecialDouble( અક્ષર* buf, દ્વિગુણ* value, અક્ષર** endptr );

    દ્વિગુણ strtod( અક્ષર* ptr, અક્ષર** endptr );

    શૂન્ય convertToCollection(પૂર્ણાંક type, File_ગાંઠ& ગાંઠ);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserve_ગાંઠ_Space(File_ગાંઠ& ગાંઠ, size_t sz);

    unsigned getStringOfs( અચલ std::string& ચાવી ) અચલ;

    File_ગાંઠ add_ગાંઠ( File_ગાંઠ& collection, અચલ std::string& ચાવી,
                      પૂર્ણાંક elem_type, અચલ શૂન્ય* value, પૂર્ણાંક len );

    શૂન્ય finalizeCollection( File_ગાંઠ& collection );

    શૂન્ય normalize_ગાંઠ_Ofs(size_t& blockIdx, size_t& ofs) અચલ;

    Base64State get_state_of_writing_base64();

    પૂર્ણાંક get_space();

    વર્ગ Base64Decoder
    {
    જાહેર:
        Base64Decoder();
        શૂન્ય init(અચલ Ptr<FileStorageParser>& _parser, અક્ષર* _ptr, પૂર્ણાંક _indent);

        તાર્કિક readMore(પૂર્ણાંક needed);

        uchar getUInt8();

        ushort getUInt16();

        પૂર્ણાંક getInt32();

        int64_t getInt64();

        દ્વિગુણ getFloat64();

        તાર્કિક endOfStream() અચલ;
        અક્ષર* getPtr() અચલ;
    સુરક્ષિત:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() અચલ
        {
            જો (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            પાછા *parser_do_not_use_direct_dereference;
        }
        અક્ષર* ptr;
        પૂર્ણાંક indent;
        std::vector<અક્ષર> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        તાર્કિક eos;
    };

    અક્ષર* parseBase64(અક્ષર* ptr, પૂર્ણાંક indent, File_ગાંઠ& collection);

    શૂન્ય parseError( અચલ અક્ષર* func_name, અચલ std::string& err_સંદેશ, અચલ અક્ષર* source_file, પૂર્ણાંક source_લીટી );

    અચલ uchar* get_ગાંઠ_Ptr(size_t blockIdx, size_t ofs) અચલ;

    std::string getName( size_t nameofs ) અચલ;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    પૂર્ણાંક flags;
    તાર્કિક empty_stream;

    FILE* file;
    gzFile gzfile;

    તાર્કિક is_opened;
    તાર્કિક dummy_eof;
    તાર્કિક write_mode;
    તાર્કિક mem_mode;
    પૂર્ણાંક fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    તાર્કિક is_using_base64;
    તાર્કિક is_write_struct_delayed;
    અક્ષર* delayed_struct_ચાવી;
    પૂર્ણાંક   delayed_struct_flags;
    અક્ષર* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    પૂર્ણાંક space, wrap_margin;
    std::deque<FStructData> write_ઢગલો;
    std::vector<અક્ષર> buffer;
    size_t bufofs;

    std::deque<અક્ષર> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        જો (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        પાછા *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() અચલ
    {
        જો (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        પાછા *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<File_ગાંઠ> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    પ્રકારવ્યાખ્યા std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<અક્ષર> str_hash_data;

    std::vector<અક્ષર> strbufv;
    અક્ષર* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    પૂર્ણાંક lineno;
};

}

#endif
