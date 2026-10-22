// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_impl.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_base64_encoding.hpp"

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ base64::Base64ContextEmitter
{
ಸಾರ್ವಜನಿಕ:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, ತಾರ್ಕಿಕ needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ದ್ವಿಮಾನ_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = ದ್ವಿಮಾನ_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        ಯದಿ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ಯದಿ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ಲಿಖ(ಧ್ರುವ uchar * beg, ಧ್ರುವ uchar * end)
    {
        ಯದಿ (beg >= end)
            ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

        ಯಾವತ್ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ಯದಿ (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_ದ್ವಿಮಾನ_convertor_t> ಅನ್ತಃಸ್ಥ
    Base64ContextEmitter & ಲಿಖ(to_ದ್ವಿಮಾನ_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        ಯಾವತ್ (convertor) {
            convertor >> end;
            ಲಿಖ(beg, end);
            end = beg;
        }

        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    }

    ತಾರ್ಕಿಕ flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        ಯದಿ (len == 0U)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        src_cur = src_beg;

        ಯದಿ ( !needs_indent)
        {
            file_storage.puts((ಧ್ರುವ ವರ್ಣ*)base64_buffer.data());
        }
        ಅನ್ಯಥಾ
        {
            ಧ್ರುವ ವರ್ಣ newline[] = "\n";
            ವರ್ಣ space[80];
            ಪೂರ್ಣಾಙ್ಕ ident = file_storage.write_ರಾಶಿ.back().indent;
            memset(space, ' ', static_cast<ಪೂರ್ಣಾಙ್ಕ>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ಧ್ರುವ ವರ್ಣ*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

ಗುಪ್ತ:
    /* because of Base64, we must keep its length a multiple of 3 */
    ಸ್ಥಿರ ಧ್ರುವ size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ಗುಪ್ತ:
    cv::FileStorage::Impl& file_storage;
    ತಾರ್ಕಿಕ needs_indent;

    std::vector<uchar> ದ್ವಿಮಾನ_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ಧ್ರುವ ವರ್ಣ *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    ಯಾವತ್ (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    ಪ್ರತ್ಯಾಗಮ buffer;
}

size_t base64::base64_encode(ಧ್ರುವ uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ಯದಿ (!src || !dst || !cnt)
        ಪ್ರತ್ಯಾಗಮ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ಧ್ರುವ * src_beg = src + off;
    uint8_t ಧ್ರುವ * src_cur = src_beg;
    uint8_t ಧ್ರುವ * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ಯಾವತ್ (src_cur < src_end) {
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
    ಯದಿ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ಅನ್ಯಥಾ ಯದಿ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    ವಿಕಲ್ಪ (rst)
    {
        ಪಕ್ಷ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ಪಕ್ಷ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ಸ್ವಭಾವ: *dst_cur   = 0;
            ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ static_cast<size_t>(dst_cur - dst_beg);
}

ಪೂರ್ಣಾಙ್ಕ base64::icvCalcStructSize(ಧ್ರುವ ವರ್ಣ *dt, ಪೂರ್ಣಾಙ್ಕ initial_size) {
    ಪೂರ್ಣಾಙ್ಕ size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    ಪ್ರತಿ ( ಧ್ರುವ ವರ್ಣ * type = dt; *type != '\0'; type++ ) {
        ವಿಕಲ್ಪ ( *type )
        {
            ಪಕ್ಷ 'u': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uchar ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'b': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uchar ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'c': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(schar ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'w': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ushort) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 's': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(short ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'i': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ   ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'n': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(unsigned) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'f': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ಭಿನ್ನ ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'd': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ದ್ವಿಗುಣ) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'I': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(int64_t)); ಭಙ್ಗ; }
            ಪಕ್ಷ 'U': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uint64_t)); ಭಙ್ಗ; }
            ಪಕ್ಷ 'e': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uchar) ); ಭಙ್ಗ; }
            ಪಕ್ಷ 'E': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uchar) ); ಭಙ್ಗ; }
            ಸ್ವಭಾವ: ಭಙ್ಗ;
        }
    }
    size = cvAlign( size, static_cast<ಪೂರ್ಣಾಙ್ಕ>(elem_max_size) );
    ಪ್ರತ್ಯಾಗಮ size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, ತಾರ್ಕಿಕ is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == ಸತ್ಯ);
    ಪ್ರತ್ಯಾಗಮ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, ತಾರ್ಕಿಕ can_indent)
        : emitter(ನೂತನ Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

ಶೂನ್ಯ base64::Base64Writer::ಲಿಖ(ಧ್ರುವ ಶೂನ್ಯ* _data, size_t len, ಧ್ರುವ ವರ್ಣ* dt)
{
    check_dt(dt);
    Raw_Data_To_ದ್ವಿಮಾನ_Convertor convertor(_data, static_cast<ಪೂರ್ಣಾಙ್ಕ>(len), data_type_string);
    emitter->ಲಿಖ(convertor);
}

template<typename to_ದ್ವಿಮಾನ_convertor_t> ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ base64::Base64Writer::ಲಿಖ(to_ದ್ವಿಮಾನ_convertor_t & convertor, ಧ್ರುವ ವರ್ಣ* dt)
{
    check_dt(dt);
    emitter->ಲಿಖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

ಶೂನ್ಯ base64::Base64Writer::check_dt(ಧ್ರುವ ವರ್ಣ* dt)
{
    ಯದಿ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ಅನ್ಯಥಾ ಯದಿ (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ಧ್ರುವ uchar * beg = reinterpret_cast<ಧ್ರುವ uchar *>(buffer.data());
        ಧ್ರುವ uchar * end = beg + buffer.size();

        emitter->ಲಿಖ(beg, end);
    } ಅನ್ಯಥಾ ಯದಿ ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_ದ್ವಿಮಾನ_Convertor::Raw_Data_To_ದ್ವಿಮಾನ_Convertor(ಧ್ರುವ ಶೂನ್ಯ* src, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ std::string & dt)
        : beg(reinterpret_cast<ಧ್ರುವ uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_ದ್ವಿಮಾನ_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

ಅನ್ತಃಸ್ಥ  base64::Raw_Data_To_ದ್ವಿಮಾನ_Convertor&  base64::Raw_Data_To_ದ್ವಿಮಾನ_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*ಏತತ್);

    ಪ್ರತಿ (size_t i = 0U, n = to_ದ್ವಿಮಾನ_funcs.size(); i < n; i++) {
        elem_to_ದ್ವಿಮಾನ_t & pack = to_ದ್ವಿಮಾನ_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಅನ್ತಃಸ್ಥ  base64::Raw_Data_To_ದ್ವಿಮಾನ_Convertor::operator ತಾರ್ಕಿಕ() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ cur < end;
}

size_t base64::Raw_Data_To_ದ್ವಿಮಾನ_Convertor::make_to_ದ್ವಿಮಾನ_funcs(ಧ್ರುವ std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    ವರ್ಣ type = '\0';

    std::istringstream iss(dt);
    ಯಾವತ್ (!iss.eof()) {
        ಯದಿ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ಯದಿ (!(iss >> type))
            ಭಙ್ಗ;

        ಯಾವತ್ (cnt-- > 0)
        {
            elem_to_ದ್ವಿಮಾನ_t pack;

            size_t size = 0;
            ವಿಕಲ್ಪ (type)
            {
                ಪಕ್ಷ 'u':
                ಪಕ್ಷ 'c':
                    size = ಪರಿಮಾಣ(uchar);
                    pack.func = to_ದ್ವಿಮಾನ<uchar>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'b':
                    size = ಪರಿಮಾಣ(uchar);
                    pack.func = to_ದ್ವಿಮಾನ<ತಾರ್ಕಿಕ>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'w':
                ಪಕ್ಷ 's':
                    size = ಪರಿಮಾಣ(ushort);
                    pack.func = to_ದ್ವಿಮಾನ<ushort>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'n':
                ಪಕ್ಷ 'i':
                    size = ಪರಿಮಾಣ(uint);
                    pack.func = to_ದ್ವಿಮಾನ<uint>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'f':
                    size = ಪರಿಮಾಣ(ಭಿನ್ನ);
                    pack.func = to_ದ್ವಿಮಾನ<ಭಿನ್ನ>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'd':
                    size = ಪರಿಮಾಣ(ದ್ವಿಗುಣ);
                    pack.func = to_ದ್ವಿಮಾನ<ದ್ವಿಗುಣ>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'I':
                ಪಕ್ಷ 'U':
                    size = ಪರಿಮಾಣ(uint64_t);
                    pack.func = to_ದ್ವಿಮಾನ<uint64_t>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'e':
                ಪಕ್ಷ 'E':
                    size = ಪರಿಮಾಣ(uchar);
                    pack.func = to_ದ್ವಿಮಾನ<uchar>;
                    ಭಙ್ಗ;
                ಪಕ್ಷ 'r':
                ಸ್ವಭಾವ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<ಪೂರ್ಣಾಙ್ಕ>(offset), static_cast<ಪೂರ್ಣಾಙ್ಕ>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_ದ್ವಿಮಾನ_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ಪ್ರತ್ಯಾಗಮ offset_packed;
}

}
