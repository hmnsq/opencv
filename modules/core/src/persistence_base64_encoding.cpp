// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "persistence_impl.hpp"
#ضمن "persistence_base64_encoding.hpp"

نطاق cv
{

صنف base64::Base64ContextEmitter
{
عام:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, منطقي needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , binary_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = binary_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        𐡄𐡍 (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        𐡄𐡍 (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & اكتب(ثابت uchar * beg, ثابت uchar * end)
    {
        𐡄𐡍 (beg >= end)
            أرجع *هذا;

        طالما (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            𐡄𐡍 (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        أرجع *هذا;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename _to_binary_convertor_t> مضمن
    Base64ContextEmitter & اكتب(_to_binary_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        طالما (convertor) {
            convertor >> end;
            اكتب(beg, end);
            end = beg;
        }

        أرجع *هذا;
    }

    منطقي flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        𐡄𐡍 (len == 0U)
            أرجع خطأ;

        src_cur = src_beg;

        𐡄𐡍 ( !needs_indent)
        {
            file_storage.puts((ثابت حرف*)base64_buffer.data());
        }
        وإلا
        {
            ثابت حرف newline[] = "\n";
            حرف space[80];
            صحيح ident = file_storage.write_stack.back().indent;
            memset(space, ' ', static_cast<صحيح>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ثابت حرف*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        أرجع 𐡒𐡔𐡈;
    }

خاص:
    /* because of Base64, we must keep its length a multiple of 3 */
    ساكن ثابت size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

خاص:
    cv::FileStorage::Impl& file_storage;
    منطقي needs_indent;

    std::vector<uchar> binary_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ثابت حرف *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    طالما (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    أرجع buffer;
}

size_t base64::base64_encode(ثابت uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    𐡄𐡍 (!src || !dst || !cnt)
        أرجع 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ثابت * src_beg = src + off;
    uint8_t ثابت * src_cur = src_beg;
    uint8_t ثابت * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    طالما (src_cur < src_end) {
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
    𐡄𐡍 (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } وإلا 𐡄𐡍 (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    بدل (rst)
    {
        حالة 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        حالة 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        افتراضي: *dst_cur   = 0;
            اكسر;
    }

    أرجع static_cast<size_t>(dst_cur - dst_beg);
}

صحيح base64::icvCalcStructSize(ثابت حرف *dt, صحيح initial_size) {
    صحيح size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    لكل ( ثابت حرف * type = dt; *type != '\0'; type++ ) {
        بدل ( *type )
        {
            حالة 'u': { elem_max_size = std::max( elem_max_size, حجم(uchar ) ); اكسر; }
            حالة 'b': { elem_max_size = std::max( elem_max_size, حجم(uchar ) ); اكسر; }
            حالة 'c': { elem_max_size = std::max( elem_max_size, حجم(schar ) ); اكسر; }
            حالة 'w': { elem_max_size = std::max( elem_max_size, حجم(ushort) ); اكسر; }
            حالة 's': { elem_max_size = std::max( elem_max_size, حجم(short ) ); اكسر; }
            حالة 'i': { elem_max_size = std::max( elem_max_size, حجم(صحيح   ) ); اكسر; }
            حالة 'n': { elem_max_size = std::max( elem_max_size, حجم(unsigned) ); اكسر; }
            حالة 'f': { elem_max_size = std::max( elem_max_size, حجم(عائم ) ); اكسر; }
            حالة 'd': { elem_max_size = std::max( elem_max_size, حجم(مزدوج) ); اكسر; }
            حالة 'I': { elem_max_size = std::max( elem_max_size, حجم(int64_t)); اكسر; }
            حالة 'U': { elem_max_size = std::max( elem_max_size, حجم(uint64_t)); اكسر; }
            حالة 'e': { elem_max_size = std::max( elem_max_size, حجم(uchar) ); اكسر; }
            حالة 'E': { elem_max_size = std::max( elem_max_size, حجم(uchar) ); اكسر; }
            افتراضي: اكسر;
        }
    }
    size = cvAlign( size, static_cast<صحيح>(elem_max_size) );
    أرجع size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, منطقي is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == 𐡒𐡔𐡈);
    أرجع (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent)
        : emitter(جديد Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

فراغ base64::Base64Writer::اكتب(ثابت فراغ* _data, size_t len, ثابت حرف* dt)
{
    check_dt(dt);
    RawDataToBinaryConvertor convertor(_data, static_cast<صحيح>(len), data_type_string);
    emitter->اكتب(convertor);
}

template<typename _to_binary_convertor_t> مضمن
فراغ base64::Base64Writer::اكتب(_to_binary_convertor_t & convertor, ثابت حرف* dt)
{
    check_dt(dt);
    emitter->اكتب(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

فراغ base64::Base64Writer::check_dt(ثابت حرف* dt)
{
    𐡄𐡍 ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    وإلا 𐡄𐡍 (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ثابت uchar * beg = reinterpret_cast<ثابت uchar *>(buffer.data());
        ثابت uchar * end = beg + buffer.size();

        emitter->اكتب(beg, end);
    } وإلا 𐡄𐡍 ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::RawDataToBinaryConvertor::RawDataToBinaryConvertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt)
        : beg(reinterpret_cast<ثابت uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_binary_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

مضمن  base64::RawDataToBinaryConvertor&  base64::RawDataToBinaryConvertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*هذا);

    لكل (size_t i = 0U, n = to_binary_funcs.size(); i < n; i++) {
        elem_to_binary_t & pack = to_binary_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    أرجع *هذا;
}

مضمن  base64::RawDataToBinaryConvertor::operator منطقي() ثابت
{
    أرجع cur < end;
}

size_t base64::RawDataToBinaryConvertor::make_to_binary_funcs(ثابت std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    حرف type = '\0';

    std::istringstream iss(dt);
    طالما (!iss.eof()) {
        𐡄𐡍 (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        𐡄𐡍 (!(iss >> type))
            اكسر;

        طالما (cnt-- > 0)
        {
            elem_to_binary_t pack;

            size_t size = 0;
            بدل (type)
            {
                حالة 'u':
                حالة 'c':
                    size = حجم(uchar);
                    pack.func = to_binary<uchar>;
                    اكسر;
                حالة 'b':
                    size = حجم(uchar);
                    pack.func = to_binary<منطقي>;
                    اكسر;
                حالة 'w':
                حالة 's':
                    size = حجم(ushort);
                    pack.func = to_binary<ushort>;
                    اكسر;
                حالة 'n':
                حالة 'i':
                    size = حجم(uint);
                    pack.func = to_binary<uint>;
                    اكسر;
                حالة 'f':
                    size = حجم(عائم);
                    pack.func = to_binary<عائم>;
                    اكسر;
                حالة 'd':
                    size = حجم(مزدوج);
                    pack.func = to_binary<مزدوج>;
                    اكسر;
                حالة 'I':
                حالة 'U':
                    size = حجم(uint64_t);
                    pack.func = to_binary<uint64_t>;
                    اكسر;
                حالة 'e':
                حالة 'E':
                    size = حجم(uchar);
                    pack.func = to_binary<uchar>;
                    اكسر;
                حالة 'r':
                افتراضي:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<صحيح>(offset), static_cast<صحيح>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_binary_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    أرجع offset_packed;
}

}
