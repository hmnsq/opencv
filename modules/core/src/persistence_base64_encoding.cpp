// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence_impl.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence_base64_encoding.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਗ base64::Base64ContextEmitter
{
ਜਨਤਕ:
    ਸਪਸ਼ਟ Base64ContextEmitter(cv::FileStorage::Impl& fs, ਤਾਰਕਿਕ needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ਦੁਆਧਾਰੀ_ਬਫਰ(ਬਫਰ_LEN)
            , base64_ਬਫਰ(base64_encode_ਬਫਰ_ਮਾਪ(ਬਫਰ_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_ਸਿਰਾ(0)
    {
        src_beg = ਦੁਆਧਾਰੀ_ਬਫਰ.ਅੰਕੜੇ();
        src_ਸਿਰਾ = src_beg + ਬਫਰ_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_ਬਹੁਲਕ);

        ਜੇ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ਜੇ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ਲਿਖ(ਅਚਲ uchar * beg, ਅਚਲ uchar * end)
    {
        ਜੇ (beg >= end)
            ਵਾਪਸ *ਇਹ;

        ਜਦੋਂ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_ਸਿਰਾ - src_cur);
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ਜੇ (src_cur >= src_ਸਿਰਾ) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        ਵਾਪਸ *ਇਹ;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t> ਸਤਰਅੰਦਰ
    Base64ContextEmitter & ਲਿਖ(ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t & convertor)
    {
        constexpr size_t ਬਫਰ_MAX_LEN = 1024U;

        uchar ਬਫਰ[ਬਫਰ_MAX_LEN];
        uchar * beg = ਬਫਰ;
        uchar * end = beg;

        ਜਦੋਂ (convertor) {
            convertor >> end;
            ਲਿਖ(beg, end);
            end = beg;
        }

        ਵਾਪਸ *ਇਹ;
    }

    ਤਾਰਕਿਕ flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_ਬਫਰ.ਅੰਕੜੇ(), 0U, src_cur - src_beg);
        ਜੇ (len == 0U)
            ਵਾਪਸ ਝੂਠ;

        src_cur = src_beg;

        ਜੇ ( !needs_indent)
        {
            file_storage.puts((ਅਚਲ ਅੱਖਰ*)base64_ਬਫਰ.ਅੰਕੜੇ());
        }
        ਨਹੀਂਤਾਂ
        {
            ਅਚਲ ਅੱਖਰ newline[] = "\n";
            ਅੱਖਰ space[80];
            ਪੂਰਨ ident = file_storage.write_ਢੇਰ.back().indent;
            memset(space, ' ', static_cast<ਪੂਰਨ>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ਅਚਲ ਅੱਖਰ*)base64_ਬਫਰ.ਅੰਕੜੇ());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ਵਾਪਸ ਸੱਚ;
    }

ਨਿਜੀ:
    /* because of Base64, we must keep its length a multiple of 3 */
    ਸਥਿਰ ਅਚਲ size_t ਬਫਰ_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ਨਿਜੀ:
    cv::FileStorage::Impl& file_storage;
    ਤਾਰਕਿਕ needs_indent;

    std::vector<uchar> ਦੁਆਧਾਰੀ_ਬਫਰ;
    std::vector<uchar> base64_ਬਫਰ;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_ਸਿਰਾ;
};

std::string base64::make_base64_ਸਿਰਲੇਖ(ਅਚਲ ਅੱਖਰ *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string ਬਫਰ(oss.str());
    CV_Assert(ਬਫਰ.size() < ::base64::ਸਿਰਲੇਖ_ਮਾਪ);

    ਬਫਰ.reserve(::base64::ਸਿਰਲੇਖ_ਮਾਪ);
    ਜਦੋਂ (ਬਫਰ.size() < ::base64::ਸਿਰਲੇਖ_ਮਾਪ)
        ਬਫਰ += ' ';

    ਵਾਪਸ ਬਫਰ;
}

size_t base64::base64_encode(ਅਚਲ uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ਜੇ (!src || !dst || !cnt)
        ਵਾਪਸ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ਅਚਲ * src_beg = src + off;
    uint8_t ਅਚਲ * src_cur = src_beg;
    uint8_t ਅਚਲ * src_ਸਿਰਾ = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ਜਦੋਂ (src_cur < src_ਸਿਰਾ) {
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
    ਜੇ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ਨਹੀਂਤਾਂ ਜੇ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    ਚੋਣ (rst)
    {
        ਹਾਲਤ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ਹਾਲਤ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ਮੂਲ: *dst_cur   = 0;
            ਰੋਕੋ;
    }

    ਵਾਪਸ static_cast<size_t>(dst_cur - dst_beg);
}

ਪੂਰਨ base64::icv_Calc_Struct_ਮਾਪ(ਅਚਲ ਅੱਖਰ *dt, ਪੂਰਨ initial_ਮਾਪ) {
    ਪੂਰਨ size = cv::fs::calc_Elem_ਮਾਪ( dt, initial_ਮਾਪ );
    size_t elem_max_ਮਾਪ = 0;
    ਲਈ ( ਅਚਲ ਅੱਖਰ * type = dt; *type != '\0'; type++ ) {
        ਚੋਣ ( *type )
        {
            ਹਾਲਤ 'u': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uchar ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'b': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uchar ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'c': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(schar ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'w': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ushort) ); ਰੋਕੋ; }
            ਹਾਲਤ 's': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਛੋਟਾ ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'i': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਪੂਰਨ   ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'n': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'f': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'd': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਦੁੱਗਣਾ) ); ਰੋਕੋ; }
            ਹਾਲਤ 'I': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(int64_t)); ਰੋਕੋ; }
            ਹਾਲਤ 'U': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uint64_t)); ਰੋਕੋ; }
            ਹਾਲਤ 'e': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uchar) ); ਰੋਕੋ; }
            ਹਾਲਤ 'E': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uchar) ); ਰੋਕੋ; }
            ਮੂਲ: ਰੋਕੋ;
        }
    }
    size = cvAlign( size, static_cast<ਪੂਰਨ>(elem_max_ਮਾਪ) );
    ਵਾਪਸ size;
}

size_t base64::base64_encode_ਬਫਰ_ਮਾਪ(size_t cnt, ਤਾਰਕਿਕ is_ਸਿਰਾ_with_zero) {
    size_t additional = static_cast<size_t>(is_ਸਿਰਾ_with_zero == ਸੱਚ);
    ਵਾਪਸ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, ਤਾਰਕਿਕ can_indent)
        : emitter(ਨਵਾਂ Base64ContextEmitter(fs, can_indent))
        , ਅੰਕੜੇ_type_string()
{
    CV_Assert(fs.write_ਬਹੁਲਕ);
}

ਖਾਲੀ base64::Base64Writer::ਲਿਖ(ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ_3, size_t len, ਅਚਲ ਅੱਖਰ* dt)
{
    check_dt(dt);
    Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor convertor(ਅੰਕੜੇ_3, static_cast<ਪੂਰਨ>(len), ਅੰਕੜੇ_type_string);
    emitter->ਲਿਖ(convertor);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t> ਸਤਰਅੰਦਰ
ਖਾਲੀ base64::Base64Writer::ਲਿਖ(ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t & convertor, ਅਚਲ ਅੱਖਰ* dt)
{
    check_dt(dt);
    emitter->ਲਿਖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    ਮਿਟਾਓ emitter;
}

ਖਾਲੀ base64::Base64Writer::check_dt(ਅਚਲ ਅੱਖਰ* dt)
{
    ਜੇ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕੜੇ_type_string.empty()) {
        ਅੰਕੜੇ_type_string = dt;

        /* output header */
        std::string ਬਫਰ = make_base64_ਸਿਰਲੇਖ(dt);
        ਅਚਲ uchar * beg = reinterpret_cast<ਅਚਲ uchar *>(ਬਫਰ.ਅੰਕੜੇ());
        ਅਚਲ uchar * end = beg + ਬਫਰ.size();

        emitter->ਲਿਖ(beg, end);
    } ਨਹੀਂਤਾਂ ਜੇ ( ਅੰਕੜੇ_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor(ਅਚਲ ਖਾਲੀ* src, ਪੂਰਨ len, ਅਚਲ std::string & dt)
        : beg(reinterpret_cast<ਅਚਲ uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    ਕਦਮ_packed = make_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs(dt);

    end = beg;
    cur = beg;

    ਕਦਮ = icv_Calc_Struct_ਮਾਪ(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

ਸਤਰਅੰਦਰ  base64::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor&  base64::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor::ਸੰਚਾਲਕ >>(uchar * & dst)
{
    CV_DbgAssert(*ਇਹ);

    ਲਈ (size_t i = 0U, n = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs.size(); i < n; i++) {
        elem_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t & pack = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs[i];
        pack.func(cur + pack.offset, dst + pack.ਵਿੱਥ_packed);
    }
    cur += ਕਦਮ;
    dst += ਕਦਮ_packed;

    ਵਾਪਸ *ਇਹ;
}

ਸਤਰਅੰਦਰ  base64::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor::ਸੰਚਾਲਕ ਤਾਰਕਿਕ() ਅਚਲ
{
    ਵਾਪਸ cur < end;
}

size_t base64::Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor::make_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs(ਅਚਲ std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t ਵਿੱਥ_packed = 0;
    ਅੱਖਰ type = '\0';

    std::istringstream iss(dt);
    ਜਦੋਂ (!iss.eof()) {
        ਜੇ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ਜੇ (!(iss >> type))
            ਰੋਕੋ;

        ਜਦੋਂ (cnt-- > 0)
        {
            elem_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t pack;

            size_t size = 0;
            ਚੋਣ (type)
            {
                ਹਾਲਤ 'u':
                ਹਾਲਤ 'c':
                    size = ਆਕਾਰ(uchar);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<uchar>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'b':
                    size = ਆਕਾਰ(uchar);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<ਤਾਰਕਿਕ>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'w':
                ਹਾਲਤ 's':
                    size = ਆਕਾਰ(ushort);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<ushort>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'n':
                ਹਾਲਤ 'i':
                    size = ਆਕਾਰ(uint);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<uint>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'f':
                    size = ਆਕਾਰ(ਦਸ਼ਮਲਵ);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<ਦਸ਼ਮਲਵ>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'd':
                    size = ਆਕਾਰ(ਦੁੱਗਣਾ);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<ਦੁੱਗਣਾ>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'I':
                ਹਾਲਤ 'U':
                    size = ਆਕਾਰ(uint64_t);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<uint64_t>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'e':
                ਹਾਲਤ 'E':
                    size = ਆਕਾਰ(uchar);
                    pack.func = ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<uchar>;
                    ਰੋਕੋ;
                ਹਾਲਤ 'r':
                ਮੂਲ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<ਪੂਰਨ>(offset), static_cast<ਪੂਰਨ>(size)));
            pack.offset = offset;
            offset += size;

            pack.ਵਿੱਥ_packed = ਵਿੱਥ_packed;
            ਵਿੱਥ_packed += size;

            ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ਵਾਪਸ ਵਿੱਥ_packed;
}

}
