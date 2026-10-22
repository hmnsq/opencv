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

enum Base64State{
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

    bool open( const char* filename_or_buf, số_nguyên _flags, const char* encoding );

    void puts( const char* str );

    char* getsFromFile( char* buf, số_nguyên count );

    char* gets( size_t maxCount );

    char* gets();

    bool eof();

    void setEof();

    void closeFile();

    void rewind();

    char* resizeWriteBuffer( char* ptr, số_nguyên len );

    char* flush();

    void endWriteStruct();

    void startWriteStruct_helper( const char* key, số_nguyên struct_flags,
                                  const char* type_name );

    void startWriteStruct( const char* key, số_nguyên struct_flags,
                           const char* type_name );

    void writeComment( const char* comment, bool eol_comment );

    void startNextStream();

    void write( const String& key, số_nguyên value );

    void write( const String& key, bool value );

    void write( const String& key, int64_t value );

    void write( const String& key, double value );

    void write( const String& key, const String& value );

    void writeRawData( const std::string& dt, const void* _data, size_t len );

    void workaround();

    void switch_to_Base64_state( FileStorage_API::Base64State new_state);

    void make_write_struct_delayed( const char* key, số_nguyên struct_flags, const char* type_name );

    void check_if_write_struct_is_delayed( bool change_type_to_base64 );

    void writeRawDataBase64(const void* _data, size_t len, const char* dt );

    String releaseAndGetString();

    FileNode getFirstTopLevelNode() const;

    FileNode root(số_nguyên streamIdx=0) const;

    FileNode operator[](const String& nodename) const;

    FileNode operator[](const char* /*nodename*/) const;

    virtual số_nguyên getFormat() const CV_OVERRIDE;

    char* bufferPtr() const;
    char* bufferStart() const;
    char* bufferEnd() const;
    void setBufferPtr(char* ptr);
    số_nguyên wrapMargin() const;

    FStructData& getCurrentStruct();

    void setNonEmpty();

    void processSpecialDouble( char* buf, double* value, char** endptr );

    double strtod( char* ptr, char** endptr );

    void convertToCollection(số_nguyên type, FileNode& node);

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
                      số_nguyên elem_type, const void* value, số_nguyên len );

    void finalizeCollection( FileNode& collection );

    void normalizeNodeOfs(size_t& blockIdx, size_t& ofs) const;

    Base64State get_state_of_writing_base64();

    số_nguyên get_space();

    class Base64Decoder
    {
    public:
        Base64Decoder();
        void init(const Ptr<FileStorageParser>& _parser, char* _ptr, số_nguyên _indent);

        bool readMore(số_nguyên needed);

        uchar getUInt8();

        ushort getUInt16();

        số_nguyên getInt32();

        int64_t getInt64();

        double getFloat64();

        bool endOfStream() const;
        char* getPtr() const;
    protected:

        Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
        FileStorageParser& getParser() const
        {
            nếu (!parser_do_not_use_direct_dereference)
                CV_Error(Error::StsNullPtr, "Parser is not available");
            trả_về *parser_do_not_use_direct_dereference;
        }
        char* ptr;
        số_nguyên indent;
        std::vector<char> encoded;
        std::vector<uchar> decoded;
        size_t ofs;
        size_t totalchars;
        bool eos;
    };

    char* parseBase64(char* ptr, số_nguyên indent, FileNode& collection);

    void parseError( const char* func_name, const std::string& err_msg, const char* source_file, số_nguyên source_line );

    const uchar* getNodePtr(size_t blockIdx, size_t ofs) const;

    std::string getName( size_t nameofs ) const;

    FileStorage* getFS();

    FileStorage* fs_ext;

    std::string filename;
    số_nguyên flags;
    bool empty_stream;

    FILE* file;
    gzFile gzfile;

    bool is_opened;
    bool dummy_eof;
    bool write_mode;
    bool mem_mode;
    số_nguyên fmt;

    State state; //!< current state of the FileStorage (used only for writing)
    bool is_using_base64;
    bool is_write_struct_delayed;
    char* delayed_struct_key;
    số_nguyên   delayed_struct_flags;
    char* delayed_type_name;
    FileStorage_API::Base64State state_of_writing_base64;

    số_nguyên space, wrap_margin;
    std::deque<FStructData> write_stack;
    std::vector<char> buffer;
    size_t bufofs;

    std::deque<char> outbuf;

    Ptr<FileStorageEmitter> emitter_do_not_use_direct_dereference;
    FileStorageEmitter& getEmitter()
    {
        nếu (!emitter_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Emitter is not available");
        trả_về *emitter_do_not_use_direct_dereference;
    }
    Ptr<FileStorageParser> parser_do_not_use_direct_dereference;
    FileStorageParser& getParser() const
    {
        nếu (!parser_do_not_use_direct_dereference)
            CV_Error(Error::StsNullPtr, "Parser is not available");
        trả_về *parser_do_not_use_direct_dereference;
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
    std::vector<char> str_hash_data;

    std::vector<char> strbufv;
    char* strbuf;
    size_t strbufsize;
    size_t strbufpos;
    số_nguyên lineno;
};

}

#endif
