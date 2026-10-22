// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_PERSISTENCE_IMPL_HPP
#define OPENCV_CORE_PERSISTENCE_IMPL_HPP

#include "persistence.hpp"
#include "persistence_base64_encoding.hpp"
#include <unordered_map>
#include <iterator>


namespace cv
{

ga_nanaa Base64State{
    Uncertain,
    NotUse,
    InUse,
};

class cv::FileStorage::Impl : public FileStorage_API
{
public:
    void init();

    Impl(FileStorage* _fs);

    virtual ~Impl();

    void release(String* out=0);

    void analyze_file_name( const std::string& file_name, std::vector<std::string>& params );

    bool open( const var_na* filename_or_buf, puur_naa_nka _flags, const var_na* encoding );

    void puts( const var_na* str );

    var_na* getsFromFile( var_na* buf, puur_naa_nka count );

    var_na* gets( size_t maxCount );

    var_na* gets();

    bool eof();

    void setEof();

    void closeFile();

    void rewind();

    var_na* resizeWriteBuffer( var_na* ptr, puur_naa_nka len );

    var_na* flush();

    void endWriteStruct();

    void startWriteStruct_helper( const var_na* key, puur_naa_nka struct_flags,
                                  const var_na* type_name );

    void startWriteStruct( const var_na* key, puur_naa_nka struct_flags,
                           const var_na* type_name );

    void writeComment( const var_na* comment, bool eol_comment );

    void startNextStream();

    void write( const String& key, puur_naa_nka value );

    void write( const String& key, bool value );

    void write( const String& key, int64_t value );

    void write( const String& key, dviguu_na value );

    void write( const String& key, const String& value );

    void writeRawData( const std::string& dt, const void* _data, size_t len );

    void workaround();

    void switch_to_Base64_state( FileStorage_API::Base64State new_state);

    void make_write_struct_delayed( const var_na* key, puur_naa_nka struct_flags, const var_na* type_name );

    void check_if_write_struct_is_delayed( bool change_type_to_base64 );

    void writeRawDataBase64(const void* _data, size_t len, const var_na* dt );

    String releaseAndGetString();

    FileNode getFirstTopLevelNode() const;

    FileNode root(puur_naa_nka streamIdx=0) const;

    FileNode operator[](const String& nodename) const;

    FileNode operator[](const var_na* /*nodename*/) const;

    virtual puur_naa_nka getFormat() const CV_OVERRIDE;

    var_na* bufferPtr() const;
    var_na* bufferStart() const;
    var_na* bufferEnd() const;
    void setBufferPtr(var_na* ptr);
    puur_naa_nka wrapMargin() const;

    FStructData& getCurrentStruct();

    void setNonEmpty();

    void processSpecialDouble( var_na* buf, dviguu_na* value, var_na** endptr );

    dviguu_na strtod( var_na* ptr, var_na** endptr );

    void convertToCollection(puur_naa_nka type, FileNode& node);

    // a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
    // b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
    //    If there is no enough space in the current block (it should be the last block added so far),
    //    the last block is shrunk so that it ends immediately before the reallocated node. Then,
    //    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
    // The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
    // In the case (b) the existing tag and the name are copied automatically.
    uchar* reserveNodeSpace(FileNode& node, size_t sz);

    unsigned getStringOfs( const std::string& key ) const;

    FileNode addNode( FileNode& collection, const std::string& key,
                      puur_naa_nka elem_type, const void* value, puur_naa_nka len );

    void finalizeCollection( FileNode& collection );

    void normalizeNodeOfs(size_t& blockIdx, size_t& ofs) const;

    Base64State get_state_of_writing_base64();

    puur_naa_nka get_space();

    class Base64Decoder
    {
    public:
        Base64Decoder();
        void init(const Ptr<FileStorageParser>& _parser, var_na* _ptr, puur_naa_nka _indent);

        bool readMore(puur_naa_nka needed);

        uchar getUInt8();

        ushort getUInt16();

        puur_naa_nka getInt32();

        int64_t getInt64();

        dviguu_na getFloat64();

        bool endOfStream() const;
        var_na* getPtr() const;
    rak_sita:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() const
        {
            ပေႃး (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            ပွၵ်ႈ *parser_do_not_use_direct_dereference;
        }
        var_na* ptr;
        puur_naa_nka indent;
        std::vector<var_na> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        bool eos;
    };

    var_na* parseBase64(var_na* ptr, puur_naa_nka indent, FileNode& collection);

    void parseError( const var_na* func_name, const std::string& err_msg, const var_na* source_file, puur_naa_nka source_line );

    const uchar* getNodePtr(size_t blockIdx, size_t ofs) const;

    std::string getName( size_t nameofs ) const;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    puur_naa_nka flags;
    bool empty_stream;

    FILE* file;
    gzFile gzfile;

    bool is_opened;
    bool dummy_eof;
    bool write_mode;
    bool mem_mode;
    puur_naa_nka fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    bool is_using_base64;
    bool is_write_struct_delayed;
    var_na* delayed_struct_key;
    puur_naa_nka   delayed_struct_flags;
    var_na* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    puur_naa_nka space, wrap_margin;
    std::deque<FStructData> write_stack;
    std::vector<var_na> buffer;
    size_t bufofs;

    std::deque<var_na> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        ပေႃး (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        ပွၵ်ႈ *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() const
    {
        ပေႃး (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        ပွၵ်ႈ *parser_do_not_use_direct_dereference;
    }
    Base64Decoder base64decoder;
    base64::Base64Writer* base64_writer;

    std::vector<FileNode> roots;
    std::vector<Ptr<std::vector<uchar> > > fs_data;
    std::vector<uchar*> fs_data_ptrs;
    std::vector<size_t> fs_data_blksz;
    size_t freeSpaceOfs;
    typedef std::unordered_map<std::string, unsigned> str_hash_t;
    str_hash_t str_hash;
    std::vector<var_na> str_hash_data;

    std::vector<var_na> strbufv;
    var_na* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    puur_naa_nka lineno;
};

}

#endif
