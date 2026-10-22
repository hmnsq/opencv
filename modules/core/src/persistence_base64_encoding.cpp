// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence_impl.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"

नामाकाश cv
{

वर्ग base64::Base64ContextEmitter
{
सार्वजनिक:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, तार्किक needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , द्विआधारी_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = द्विआधारी_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        यदि (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        यदि (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & लिख(ध्रुव uchar * beg, ध्रुव uchar * end)
    {
        यदि (beg >= end)
            प्रत्यागम *एतत्;

        यावत् (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            यदि (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        प्रत्यागम *एतत्;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_द्विआधारी_convertor_t> अन्तःस्थ
    Base64ContextEmitter & लिख(to_द्विआधारी_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        यावत् (convertor) {
            convertor >> end;
            लिख(beg, end);
            end = beg;
        }

        प्रत्यागम *एतत्;
    }

    तार्किक flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        यदि (len == 0U)
            प्रत्यागम असत्य;

        src_cur = src_beg;

        यदि ( !needs_indent)
        {
            file_storage.puts((ध्रुव वर्ण*)base64_buffer.data());
        }
        अन्यथा
        {
            ध्रुव वर्ण newline[] = "\n";
            वर्ण space[80];
            पूर्णाङ्क ident = file_storage.write_ढेर.back().indent;
            memset(space, ' ', static_cast<पूर्णाङ्क>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ध्रुव वर्ण*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        प्रत्यागम सत्य;
    }

गुप्त:
    /* because of Base64, we must keep its length a multiple of 3 */
    स्थिर ध्रुव size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

गुप्त:
    cv::FileStorage::Impl& file_storage;
    तार्किक needs_indent;

    std::vector<uchar> द्विआधारी_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ध्रुव वर्ण *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    यावत् (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    प्रत्यागम buffer;
}

size_t base64::base64_encode(ध्रुव uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    यदि (!src || !dst || !cnt)
        प्रत्यागम 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ध्रुव * src_beg = src + off;
    uint8_t ध्रुव * src_cur = src_beg;
    uint8_t ध्रुव * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    यावत् (src_cur < src_end) {
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
    यदि (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } अन्यथा यदि (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    विकल्प (rst)
    {
        पक्ष 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        पक्ष 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        स्वभाव: *dst_cur   = 0;
            भङ्ग;
    }

    प्रत्यागम static_cast<size_t>(dst_cur - dst_beg);
}

पूर्णाङ्क base64::icvCalcStructSize(ध्रुव वर्ण *dt, पूर्णाङ्क initial_size) {
    पूर्णाङ्क size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    प्रति ( ध्रुव वर्ण * type = dt; *type != '\0'; type++ ) {
        विकल्प ( *type )
        {
            पक्ष 'u': { elem_max_size = std::max( elem_max_size, परिमाण(uchar ) ); भङ्ग; }
            पक्ष 'b': { elem_max_size = std::max( elem_max_size, परिमाण(uchar ) ); भङ्ग; }
            पक्ष 'c': { elem_max_size = std::max( elem_max_size, परिमाण(schar ) ); भङ्ग; }
            पक्ष 'w': { elem_max_size = std::max( elem_max_size, परिमाण(ushort) ); भङ्ग; }
            पक्ष 's': { elem_max_size = std::max( elem_max_size, परिमाण(short ) ); भङ्ग; }
            पक्ष 'i': { elem_max_size = std::max( elem_max_size, परिमाण(पूर्णाङ्क   ) ); भङ्ग; }
            पक्ष 'n': { elem_max_size = std::max( elem_max_size, परिमाण(unsigned) ); भङ्ग; }
            पक्ष 'f': { elem_max_size = std::max( elem_max_size, परिमाण(भिन्न ) ); भङ्ग; }
            पक्ष 'd': { elem_max_size = std::max( elem_max_size, परिमाण(द्विगुण) ); भङ्ग; }
            पक्ष 'I': { elem_max_size = std::max( elem_max_size, परिमाण(int64_t)); भङ्ग; }
            पक्ष 'U': { elem_max_size = std::max( elem_max_size, परिमाण(uint64_t)); भङ्ग; }
            पक्ष 'e': { elem_max_size = std::max( elem_max_size, परिमाण(uchar) ); भङ्ग; }
            पक्ष 'E': { elem_max_size = std::max( elem_max_size, परिमाण(uchar) ); भङ्ग; }
            स्वभाव: भङ्ग;
        }
    }
    size = cvAlign( size, static_cast<पूर्णाङ्क>(elem_max_size) );
    प्रत्यागम size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, तार्किक is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == सत्य);
    प्रत्यागम (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, तार्किक can_indent)
        : emitter(नूतन Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

शून्य base64::Base64Writer::लिख(ध्रुव शून्य* _data, size_t len, ध्रुव वर्ण* dt)
{
    check_dt(dt);
    Raw_Data_To_द्विआधारी_Convertor convertor(_data, static_cast<पूर्णाङ्क>(len), data_type_string);
    emitter->लिख(convertor);
}

template<typename to_द्विआधारी_convertor_t> अन्तःस्थ
शून्य base64::Base64Writer::लिख(to_द्विआधारी_convertor_t & convertor, ध्रुव वर्ण* dt)
{
    check_dt(dt);
    emitter->लिख(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

शून्य base64::Base64Writer::check_dt(ध्रुव वर्ण* dt)
{
    यदि ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    अन्यथा यदि (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ध्रुव uchar * beg = reinterpret_cast<ध्रुव uchar *>(buffer.data());
        ध्रुव uchar * end = beg + buffer.size();

        emitter->लिख(beg, end);
    } अन्यथा यदि ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_द्विआधारी_Convertor::Raw_Data_To_द्विआधारी_Convertor(ध्रुव शून्य* src, पूर्णाङ्क len, ध्रुव std::string & dt)
        : beg(reinterpret_cast<ध्रुव uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_द्विआधारी_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

अन्तःस्थ  base64::Raw_Data_To_द्विआधारी_Convertor&  base64::Raw_Data_To_द्विआधारी_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*एतत्);

    प्रति (size_t i = 0U, n = to_द्विआधारी_funcs.size(); i < n; i++) {
        elem_to_द्विआधारी_t & pack = to_द्विआधारी_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    प्रत्यागम *एतत्;
}

अन्तःस्थ  base64::Raw_Data_To_द्विआधारी_Convertor::operator तार्किक() ध्रुव
{
    प्रत्यागम cur < end;
}

size_t base64::Raw_Data_To_द्विआधारी_Convertor::make_to_द्विआधारी_funcs(ध्रुव std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    वर्ण type = '\0';

    std::istringstream iss(dt);
    यावत् (!iss.eof()) {
        यदि (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        यदि (!(iss >> type))
            भङ्ग;

        यावत् (cnt-- > 0)
        {
            elem_to_द्विआधारी_t pack;

            size_t size = 0;
            विकल्प (type)
            {
                पक्ष 'u':
                पक्ष 'c':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    भङ्ग;
                पक्ष 'b':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<तार्किक>;
                    भङ्ग;
                पक्ष 'w':
                पक्ष 's':
                    size = परिमाण(ushort);
                    pack.func = to_द्विआधारी<ushort>;
                    भङ्ग;
                पक्ष 'n':
                पक्ष 'i':
                    size = परिमाण(uint);
                    pack.func = to_द्विआधारी<uint>;
                    भङ्ग;
                पक्ष 'f':
                    size = परिमाण(भिन्न);
                    pack.func = to_द्विआधारी<भिन्न>;
                    भङ्ग;
                पक्ष 'd':
                    size = परिमाण(द्विगुण);
                    pack.func = to_द्विआधारी<द्विगुण>;
                    भङ्ग;
                पक्ष 'I':
                पक्ष 'U':
                    size = परिमाण(uint64_t);
                    pack.func = to_द्विआधारी<uint64_t>;
                    भङ्ग;
                पक्ष 'e':
                पक्ष 'E':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    भङ्ग;
                पक्ष 'r':
                स्वभाव:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<पूर्णाङ्क>(offset), static_cast<पूर्णाङ्क>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_द्विआधारी_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    प्रत्यागम offset_packed;
}

}
