// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence_impl.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence_base64_encoding.hpp"

ନାମାକାଶ cv
{

ଶ୍ରେଣୀ base64::Base64ContextEmitter
{
ସର୍ବସାଧାରଣ:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, ତାର୍କିକ needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ଦ୍ୱିମିକ_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = ଦ୍ୱିମିକ_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        ଯଦି (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ଯଦି (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ଲିଖ(ଅଚଳ uchar * beg, ଅଚଳ uchar * end)
    {
        ଯଦି (beg >= end)
            ଫେରନ୍ତୁ *ଏହି;

        ଯେତେବେଳେ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ଯଦି (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        ଫେରନ୍ତୁ *ଏହି;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_ଦ୍ୱିମିକ_convertor_t> ଧାଡ଼ିଭିତରେ
    Base64ContextEmitter & ଲିଖ(to_ଦ୍ୱିମିକ_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        ଯେତେବେଳେ (convertor) {
            convertor >> end;
            ଲିଖ(beg, end);
            end = beg;
        }

        ଫେରନ୍ତୁ *ଏହି;
    }

    ତାର୍କିକ flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        ଯଦି (len == 0U)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        src_cur = src_beg;

        ଯଦି ( !needs_indent)
        {
            file_storage.puts((ଅଚଳ ଅକ୍ଷର*)base64_buffer.data());
        }
        ଅନ୍ୟଥା
        {
            ଅଚଳ ଅକ୍ଷର newline[] = "\n";
            ଅକ୍ଷର space[80];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident = file_storage.write_ଗଦା.back().indent;
            memset(space, ' ', static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ଅଚଳ ଅକ୍ଷର*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

ବ୍ୟକ୍ତିଗତ:
    /* because of Base64, we must keep its length a multiple of 3 */
    ସ୍ଥିର ଅଚଳ size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ବ୍ୟକ୍ତିଗତ:
    cv::FileStorage::Impl& file_storage;
    ତାର୍କିକ needs_indent;

    std::vector<uchar> ଦ୍ୱିମିକ_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ଅଚଳ ଅକ୍ଷର *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    ଯେତେବେଳେ (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    ଫେରନ୍ତୁ buffer;
}

size_t base64::base64_encode(ଅଚଳ uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ଯଦି (!src || !dst || !cnt)
        ଫେରନ୍ତୁ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ଅଚଳ * src_beg = src + off;
    uint8_t ଅଚଳ * src_cur = src_beg;
    uint8_t ଅଚଳ * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ଯେତେବେଳେ (src_cur < src_end) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        uint8_t _0 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_1 & 0xF0U) >> 4U | (_2 & 0x03U) << 4U];
        *dst_cur++ = base64_mapping[(_0 & 0xC0U) >> 6U | (_1 & 0x0FU) << 2U];
        *dst_cur++ = base64_mapping[ _0 & 0x3FU];
    }

    /* remainder part */
    size_t rst = src_beg + cnt - src_cur;
    ଯଦି (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ଅନ୍ୟଥା ଯଦି (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    ବାଛ (rst)
    {
        ସ୍ଥିତି 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ସ୍ଥିତି 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: *dst_cur   = 0;
            ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ static_cast<size_t>(dst_cur - dst_beg);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା base64::icvCalcStructSize(ଅଚଳ ଅକ୍ଷର *dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size) {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    ପାଇଁ ( ଅଚଳ ଅକ୍ଷର * type = dt; *type != '\0'; type++ ) {
        ବାଛ ( *type )
        {
            ସ୍ଥିତି 'u': { elem_max_size = std::max( elem_max_size, ଆକାର(uchar ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'b': { elem_max_size = std::max( elem_max_size, ଆକାର(uchar ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'c': { elem_max_size = std::max( elem_max_size, ଆକାର(schar ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'w': { elem_max_size = std::max( elem_max_size, ଆକାର(ushort) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 's': { elem_max_size = std::max( elem_max_size, ଆକାର(short ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'i': { elem_max_size = std::max( elem_max_size, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା   ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'n': { elem_max_size = std::max( elem_max_size, ଆକାର(unsigned) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'f': { elem_max_size = std::max( elem_max_size, ଆକାର(ଦଶମିକ ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'd': { elem_max_size = std::max( elem_max_size, ଆକାର(ଦ୍ୱିଗୁଣ) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'I': { elem_max_size = std::max( elem_max_size, ଆକାର(int64_t)); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'U': { elem_max_size = std::max( elem_max_size, ଆକାର(uint64_t)); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'e': { elem_max_size = std::max( elem_max_size, ଆକାର(uchar) ); ଭାଙ୍ଗ; }
            ସ୍ଥିତି 'E': { elem_max_size = std::max( elem_max_size, ଆକାର(uchar) ); ଭାଙ୍ଗ; }
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଭାଙ୍ଗ;
        }
    }
    size = cvAlign( size, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(elem_max_size) );
    ଫେରନ୍ତୁ size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, ତାର୍କିକ is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == ସତ୍ୟ);
    ଫେରନ୍ତୁ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, ତାର୍କିକ can_indent)
        : emitter(ନୂତନ Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

ଶୂନ୍ୟ base64::Base64Writer::ଲିଖ(ଅଚଳ ଶୂନ୍ୟ* _data, size_t len, ଅଚଳ ଅକ୍ଷର* dt)
{
    check_dt(dt);
    Raw_Data_To_ଦ୍ୱିମିକ_Convertor convertor(_data, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(len), data_type_string);
    emitter->ଲିଖ(convertor);
}

template<typename to_ଦ୍ୱିମିକ_convertor_t> ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ base64::Base64Writer::ଲିଖ(to_ଦ୍ୱିମିକ_convertor_t & convertor, ଅଚଳ ଅକ୍ଷର* dt)
{
    check_dt(dt);
    emitter->ଲିଖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

ଶୂନ୍ୟ base64::Base64Writer::check_dt(ଅଚଳ ଅକ୍ଷର* dt)
{
    ଯଦି ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ଅନ୍ୟଥା ଯଦି (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ଅଚଳ uchar * beg = reinterpret_cast<ଅଚଳ uchar *>(buffer.data());
        ଅଚଳ uchar * end = beg + buffer.size();

        emitter->ଲିଖ(beg, end);
    } ଅନ୍ୟଥା ଯଦି ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_ଦ୍ୱିମିକ_Convertor::Raw_Data_To_ଦ୍ୱିମିକ_Convertor(ଅଚଳ ଶୂନ୍ୟ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ std::string & dt)
        : beg(reinterpret_cast<ଅଚଳ uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_ଦ୍ୱିମିକ_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

ଧାଡ଼ିଭିତରେ  base64::Raw_Data_To_ଦ୍ୱିମିକ_Convertor&  base64::Raw_Data_To_ଦ୍ୱିମିକ_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*ଏହି);

    ପାଇଁ (size_t i = 0U, n = to_ଦ୍ୱିମିକ_funcs.size(); i < n; i++) {
        elem_to_ଦ୍ୱିମିକ_t & pack = to_ଦ୍ୱିମିକ_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    ଫେରନ୍ତୁ *ଏହି;
}

ଧାଡ଼ିଭିତରେ  base64::Raw_Data_To_ଦ୍ୱିମିକ_Convertor::operator ତାର୍କିକ() ଅଚଳ
{
    ଫେରନ୍ତୁ cur < end;
}

size_t base64::Raw_Data_To_ଦ୍ୱିମିକ_Convertor::make_to_ଦ୍ୱିମିକ_funcs(ଅଚଳ std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    ଅକ୍ଷର type = '\0';

    std::istringstream iss(dt);
    ଯେତେବେଳେ (!iss.eof()) {
        ଯଦି (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ଯଦି (!(iss >> type))
            ଭାଙ୍ଗ;

        ଯେତେବେଳେ (cnt-- > 0)
        {
            elem_to_ଦ୍ୱିମିକ_t pack;

            size_t size = 0;
            ବାଛ (type)
            {
                ସ୍ଥିତି 'u':
                ସ୍ଥିତି 'c':
                    size = ଆକାର(uchar);
                    pack.func = to_ଦ୍ୱିମିକ<uchar>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'b':
                    size = ଆକାର(uchar);
                    pack.func = to_ଦ୍ୱିମିକ<ତାର୍କିକ>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'w':
                ସ୍ଥିତି 's':
                    size = ଆକାର(ushort);
                    pack.func = to_ଦ୍ୱିମିକ<ushort>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'n':
                ସ୍ଥିତି 'i':
                    size = ଆକାର(uint);
                    pack.func = to_ଦ୍ୱିମିକ<uint>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'f':
                    size = ଆକାର(ଦଶମିକ);
                    pack.func = to_ଦ୍ୱିମିକ<ଦଶମିକ>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'd':
                    size = ଆକାର(ଦ୍ୱିଗୁଣ);
                    pack.func = to_ଦ୍ୱିମିକ<ଦ୍ୱିଗୁଣ>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'I':
                ସ୍ଥିତି 'U':
                    size = ଆକାର(uint64_t);
                    pack.func = to_ଦ୍ୱିମିକ<uint64_t>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'e':
                ସ୍ଥିତି 'E':
                    size = ଆକାର(uchar);
                    pack.func = to_ଦ୍ୱିମିକ<uchar>;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି 'r':
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(offset), static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_ଦ୍ୱିମିକ_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ଫେରନ୍ତୁ offset_packed;
}

}
