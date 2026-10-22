// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence_impl.hpp"
#અન્તર્ભાવ "persistence_base64_encoding.hpp"

નામાકાશ cv
{

વર્ગ base64::Base64ContextEmitter
{
જાહેર:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, તાર્કિક needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , દ્વિઅંકી_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = દ્વિઅંકી_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        જો (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        જો (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & લિખ(અચલ uchar * beg, અચલ uchar * end)
    {
        જો (beg >= end)
            પાછા *આ;

        જ્યારે (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            જો (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        પાછા *આ;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_દ્વિઅંકી_convertor_t> અંતઃરેખ
    Base64ContextEmitter & લિખ(to_દ્વિઅંકી_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        જ્યારે (convertor) {
            convertor >> end;
            લિખ(beg, end);
            end = beg;
        }

        પાછા *આ;
    }

    તાર્કિક flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        જો (len == 0U)
            પાછા ખોટું;

        src_cur = src_beg;

        જો ( !needs_indent)
        {
            file_storage.puts((અચલ અક્ષર*)base64_buffer.data());
        }
        નહીંતર
        {
            અચલ અક્ષર newline[] = "\n";
            અક્ષર space[80];
            પૂર્ણાંક ident = file_storage.write_ઢગલો.back().indent;
            memset(space, ' ', static_cast<પૂર્ણાંક>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((અચલ અક્ષર*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        પાછા સાચું;
    }

ખાનગી:
    /* because of Base64, we must keep its length a multiple of 3 */
    સ્થિર અચલ size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ખાનગી:
    cv::FileStorage::Impl& file_storage;
    તાર્કિક needs_indent;

    std::vector<uchar> દ્વિઅંકી_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(અચલ અક્ષર *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    જ્યારે (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    પાછા buffer;
}

size_t base64::base64_encode(અચલ uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    જો (!src || !dst || !cnt)
        પાછા 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t અચલ * src_beg = src + off;
    uint8_t અચલ * src_cur = src_beg;
    uint8_t અચલ * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    જ્યારે (src_cur < src_end) {
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
    જો (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } નહીંતર જો (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    પસંદગી (rst)
    {
        સ્થિતિ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        સ્થિતિ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        મૂળભૂત: *dst_cur   = 0;
            તોડો;
    }

    પાછા static_cast<size_t>(dst_cur - dst_beg);
}

પૂર્ણાંક base64::icvCalcStructSize(અચલ અક્ષર *dt, પૂર્ણાંક initial_size) {
    પૂર્ણાંક size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    માટે ( અચલ અક્ષર * type = dt; *type != '\0'; type++ ) {
        પસંદગી ( *type )
        {
            સ્થિતિ 'u': { elem_max_size = std::max( elem_max_size, કદ(uchar ) ); તોડો; }
            સ્થિતિ 'b': { elem_max_size = std::max( elem_max_size, કદ(uchar ) ); તોડો; }
            સ્થિતિ 'c': { elem_max_size = std::max( elem_max_size, કદ(schar ) ); તોડો; }
            સ્થિતિ 'w': { elem_max_size = std::max( elem_max_size, કદ(ushort) ); તોડો; }
            સ્થિતિ 's': { elem_max_size = std::max( elem_max_size, કદ(short ) ); તોડો; }
            સ્થિતિ 'i': { elem_max_size = std::max( elem_max_size, કદ(પૂર્ણાંક   ) ); તોડો; }
            સ્થિતિ 'n': { elem_max_size = std::max( elem_max_size, કદ(unsigned) ); તોડો; }
            સ્થિતિ 'f': { elem_max_size = std::max( elem_max_size, કદ(દશાંશ ) ); તોડો; }
            સ્થિતિ 'd': { elem_max_size = std::max( elem_max_size, કદ(દ્વિગુણ) ); તોડો; }
            સ્થિતિ 'I': { elem_max_size = std::max( elem_max_size, કદ(int64_t)); તોડો; }
            સ્થિતિ 'U': { elem_max_size = std::max( elem_max_size, કદ(uint64_t)); તોડો; }
            સ્થિતિ 'e': { elem_max_size = std::max( elem_max_size, કદ(uchar) ); તોડો; }
            સ્થિતિ 'E': { elem_max_size = std::max( elem_max_size, કદ(uchar) ); તોડો; }
            મૂળભૂત: તોડો;
        }
    }
    size = cvAlign( size, static_cast<પૂર્ણાંક>(elem_max_size) );
    પાછા size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, તાર્કિક is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == સાચું);
    પાછા (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, તાર્કિક can_indent)
        : emitter(નવું Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

શૂન્ય base64::Base64Writer::લિખ(અચલ શૂન્ય* _data, size_t len, અચલ અક્ષર* dt)
{
    check_dt(dt);
    Raw_Data_To_દ્વિઅંકી_Convertor convertor(_data, static_cast<પૂર્ણાંક>(len), data_type_string);
    emitter->લિખ(convertor);
}

template<typename to_દ્વિઅંકી_convertor_t> અંતઃરેખ
શૂન્ય base64::Base64Writer::લિખ(to_દ્વિઅંકી_convertor_t & convertor, અચલ અક્ષર* dt)
{
    check_dt(dt);
    emitter->લિખ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

શૂન્ય base64::Base64Writer::check_dt(અચલ અક્ષર* dt)
{
    જો ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    નહીંતર જો (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        અચલ uchar * beg = reinterpret_cast<અચલ uchar *>(buffer.data());
        અચલ uchar * end = beg + buffer.size();

        emitter->લિખ(beg, end);
    } નહીંતર જો ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_દ્વિઅંકી_Convertor::Raw_Data_To_દ્વિઅંકી_Convertor(અચલ શૂન્ય* src, પૂર્ણાંક len, અચલ std::string & dt)
        : beg(reinterpret_cast<અચલ uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_દ્વિઅંકી_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

અંતઃરેખ  base64::Raw_Data_To_દ્વિઅંકી_Convertor&  base64::Raw_Data_To_દ્વિઅંકી_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*આ);

    માટે (size_t i = 0U, n = to_દ્વિઅંકી_funcs.size(); i < n; i++) {
        elem_to_દ્વિઅંકી_t & pack = to_દ્વિઅંકી_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    પાછા *આ;
}

અંતઃરેખ  base64::Raw_Data_To_દ્વિઅંકી_Convertor::operator તાર્કિક() અચલ
{
    પાછા cur < end;
}

size_t base64::Raw_Data_To_દ્વિઅંકી_Convertor::make_to_દ્વિઅંકી_funcs(અચલ std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    અક્ષર type = '\0';

    std::istringstream iss(dt);
    જ્યારે (!iss.eof()) {
        જો (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        જો (!(iss >> type))
            તોડો;

        જ્યારે (cnt-- > 0)
        {
            elem_to_દ્વિઅંકી_t pack;

            size_t size = 0;
            પસંદગી (type)
            {
                સ્થિતિ 'u':
                સ્થિતિ 'c':
                    size = કદ(uchar);
                    pack.func = to_દ્વિઅંકી<uchar>;
                    તોડો;
                સ્થિતિ 'b':
                    size = કદ(uchar);
                    pack.func = to_દ્વિઅંકી<તાર્કિક>;
                    તોડો;
                સ્થિતિ 'w':
                સ્થિતિ 's':
                    size = કદ(ushort);
                    pack.func = to_દ્વિઅંકી<ushort>;
                    તોડો;
                સ્થિતિ 'n':
                સ્થિતિ 'i':
                    size = કદ(uint);
                    pack.func = to_દ્વિઅંકી<uint>;
                    તોડો;
                સ્થિતિ 'f':
                    size = કદ(દશાંશ);
                    pack.func = to_દ્વિઅંકી<દશાંશ>;
                    તોડો;
                સ્થિતિ 'd':
                    size = કદ(દ્વિગુણ);
                    pack.func = to_દ્વિઅંકી<દ્વિગુણ>;
                    તોડો;
                સ્થિતિ 'I':
                સ્થિતિ 'U':
                    size = કદ(uint64_t);
                    pack.func = to_દ્વિઅંકી<uint64_t>;
                    તોડો;
                સ્થિતિ 'e':
                સ્થિતિ 'E':
                    size = કદ(uchar);
                    pack.func = to_દ્વિઅંકી<uchar>;
                    તોડો;
                સ્થિતિ 'r':
                મૂળભૂત:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<પૂર્ણાંક>(offset), static_cast<પૂર્ણાંક>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_દ્વિઅંકી_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    પાછા offset_packed;
}

}
