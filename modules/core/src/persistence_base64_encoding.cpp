// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence_impl.hpp"
#অন্তর্ভাব "persistence_base64_encoding.hpp"

নামাকাশ cv
{

বর্গ base64::Base64ContextEmitter
{
সার্বজনিক:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, তার্কিক needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , দ্বিমিক_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = দ্বিমিক_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        যদি (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        যদি (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & লিখ(ধ্রুব uchar * beg, ধ্রুব uchar * end)
    {
        যদি (beg >= end)
            প্রত্যাগম *এতত্;

        যাবত্ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            যদি (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        প্রত্যাগম *এতত্;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_দ্বিমিক_convertor_t> অন্তঃস্থ
    Base64ContextEmitter & লিখ(to_দ্বিমিক_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        যাবত্ (convertor) {
            convertor >> end;
            লিখ(beg, end);
            end = beg;
        }

        প্রত্যাগম *এতত্;
    }

    তার্কিক flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        যদি (len == 0U)
            প্রত্যাগম অসত্য;

        src_cur = src_beg;

        যদি ( !needs_indent)
        {
            file_storage.puts((ধ্রুব বর্ণ*)base64_buffer.data());
        }
        অন্যথা
        {
            ধ্রুব বর্ণ newline[] = "\n";
            বর্ণ space[80];
            পূর্ণাঙ্ক ident = file_storage.write_স্তূপ.back().indent;
            memset(space, ' ', static_cast<পূর্ণাঙ্ক>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ধ্রুব বর্ণ*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        প্রত্যাগম সত্য;
    }

গুপ্ত:
    /* because of Base64, we must keep its length a multiple of 3 */
    স্থির ধ্রুব size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

গুপ্ত:
    cv::FileStorage::Impl& file_storage;
    তার্কিক needs_indent;

    std::vector<uchar> দ্বিমিক_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ধ্রুব বর্ণ *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    যাবত্ (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    প্রত্যাগম buffer;
}

size_t base64::base64_encode(ধ্রুব uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    যদি (!src || !dst || !cnt)
        প্রত্যাগম 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ধ্রুব * src_beg = src + off;
    uint8_t ধ্রুব * src_cur = src_beg;
    uint8_t ধ্রুব * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    যাবত্ (src_cur < src_end) {
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
    যদি (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } অন্যথা যদি (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    বিকল্প (rst)
    {
        পক্ষ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        পক্ষ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        স্বভাব: *dst_cur   = 0;
            ভঙ্গ;
    }

    প্রত্যাগম static_cast<size_t>(dst_cur - dst_beg);
}

পূর্ণাঙ্ক base64::icvCalcStructSize(ধ্রুব বর্ণ *dt, পূর্ণাঙ্ক initial_size) {
    পূর্ণাঙ্ক size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    প্রতি ( ধ্রুব বর্ণ * type = dt; *type != '\0'; type++ ) {
        বিকল্প ( *type )
        {
            পক্ষ 'u': { elem_max_size = std::max( elem_max_size, পরিমাণ(uchar ) ); ভঙ্গ; }
            পক্ষ 'b': { elem_max_size = std::max( elem_max_size, পরিমাণ(uchar ) ); ভঙ্গ; }
            পক্ষ 'c': { elem_max_size = std::max( elem_max_size, পরিমাণ(schar ) ); ভঙ্গ; }
            পক্ষ 'w': { elem_max_size = std::max( elem_max_size, পরিমাণ(ushort) ); ভঙ্গ; }
            পক্ষ 's': { elem_max_size = std::max( elem_max_size, পরিমাণ(short ) ); ভঙ্গ; }
            পক্ষ 'i': { elem_max_size = std::max( elem_max_size, পরিমাণ(পূর্ণাঙ্ক   ) ); ভঙ্গ; }
            পক্ষ 'n': { elem_max_size = std::max( elem_max_size, পরিমাণ(unsigned) ); ভঙ্গ; }
            পক্ষ 'f': { elem_max_size = std::max( elem_max_size, পরিমাণ(ভিন্ন ) ); ভঙ্গ; }
            পক্ষ 'd': { elem_max_size = std::max( elem_max_size, পরিমাণ(দ্বিগুণ) ); ভঙ্গ; }
            পক্ষ 'I': { elem_max_size = std::max( elem_max_size, পরিমাণ(int64_t)); ভঙ্গ; }
            পক্ষ 'U': { elem_max_size = std::max( elem_max_size, পরিমাণ(uint64_t)); ভঙ্গ; }
            পক্ষ 'e': { elem_max_size = std::max( elem_max_size, পরিমাণ(uchar) ); ভঙ্গ; }
            পক্ষ 'E': { elem_max_size = std::max( elem_max_size, পরিমাণ(uchar) ); ভঙ্গ; }
            স্বভাব: ভঙ্গ;
        }
    }
    size = cvAlign( size, static_cast<পূর্ণাঙ্ক>(elem_max_size) );
    প্রত্যাগম size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, তার্কিক is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == সত্য);
    প্রত্যাগম (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, তার্কিক can_indent)
        : emitter(নূতন Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

শূন্য base64::Base64Writer::লিখ(ধ্রুব শূন্য* _data, size_t len, ধ্রুব বর্ণ* dt)
{
    check_dt(dt);
    Raw_Data_To_দ্বিমিক_Convertor convertor(_data, static_cast<পূর্ণাঙ্ক>(len), data_type_string);
    emitter->লিখ(convertor);
}

template<typename to_দ্বিমিক_convertor_t> অন্তঃস্থ
শূন্য base64::Base64Writer::লিখ(to_দ্বিমিক_convertor_t & convertor, ধ্রুব বর্ণ* dt)
{
    check_dt(dt);
    emitter->লিখ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

শূন্য base64::Base64Writer::check_dt(ধ্রুব বর্ণ* dt)
{
    যদি ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    অন্যথা যদি (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ধ্রুব uchar * beg = reinterpret_cast<ধ্রুব uchar *>(buffer.data());
        ধ্রুব uchar * end = beg + buffer.size();

        emitter->লিখ(beg, end);
    } অন্যথা যদি ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_দ্বিমিক_Convertor::Raw_Data_To_দ্বিমিক_Convertor(ধ্রুব শূন্য* src, পূর্ণাঙ্ক len, ধ্রুব std::string & dt)
        : beg(reinterpret_cast<ধ্রুব uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_দ্বিমিক_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

অন্তঃস্থ  base64::Raw_Data_To_দ্বিমিক_Convertor&  base64::Raw_Data_To_দ্বিমিক_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*এতত্);

    প্রতি (size_t i = 0U, n = to_দ্বিমিক_funcs.size(); i < n; i++) {
        elem_to_দ্বিমিক_t & pack = to_দ্বিমিক_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    প্রত্যাগম *এতত্;
}

অন্তঃস্থ  base64::Raw_Data_To_দ্বিমিক_Convertor::operator তার্কিক() ধ্রুব
{
    প্রত্যাগম cur < end;
}

size_t base64::Raw_Data_To_দ্বিমিক_Convertor::make_to_দ্বিমিক_funcs(ধ্রুব std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    বর্ণ type = '\0';

    std::istringstream iss(dt);
    যাবত্ (!iss.eof()) {
        যদি (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        যদি (!(iss >> type))
            ভঙ্গ;

        যাবত্ (cnt-- > 0)
        {
            elem_to_দ্বিমিক_t pack;

            size_t size = 0;
            বিকল্প (type)
            {
                পক্ষ 'u':
                পক্ষ 'c':
                    size = পরিমাণ(uchar);
                    pack.func = to_দ্বিমিক<uchar>;
                    ভঙ্গ;
                পক্ষ 'b':
                    size = পরিমাণ(uchar);
                    pack.func = to_দ্বিমিক<তার্কিক>;
                    ভঙ্গ;
                পক্ষ 'w':
                পক্ষ 's':
                    size = পরিমাণ(ushort);
                    pack.func = to_দ্বিমিক<ushort>;
                    ভঙ্গ;
                পক্ষ 'n':
                পক্ষ 'i':
                    size = পরিমাণ(uint);
                    pack.func = to_দ্বিমিক<uint>;
                    ভঙ্গ;
                পক্ষ 'f':
                    size = পরিমাণ(ভিন্ন);
                    pack.func = to_দ্বিমিক<ভিন্ন>;
                    ভঙ্গ;
                পক্ষ 'd':
                    size = পরিমাণ(দ্বিগুণ);
                    pack.func = to_দ্বিমিক<দ্বিগুণ>;
                    ভঙ্গ;
                পক্ষ 'I':
                পক্ষ 'U':
                    size = পরিমাণ(uint64_t);
                    pack.func = to_দ্বিমিক<uint64_t>;
                    ভঙ্গ;
                পক্ষ 'e':
                পক্ষ 'E':
                    size = পরিমাণ(uchar);
                    pack.func = to_দ্বিমিক<uchar>;
                    ভঙ্গ;
                পক্ষ 'r':
                স্বভাব:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<পূর্ণাঙ্ক>(offset), static_cast<পূর্ণাঙ্ক>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_দ্বিমিক_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    প্রত্যাগম offset_packed;
}

}
