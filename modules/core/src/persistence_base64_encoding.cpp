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
    ಸ್ಪಷ್ಟ Base64ContextEmitter(cv::FileStorage::Impl& fs, ತಾರ್ಕಿಕ needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ದ್ವಿಮಾನ_ಬಫರ್(ಬಫರ್_LEN)
            , base64_ಬಫರ್(base64_encode_ಬಫರ್_ಗಾತ್ರ(ಬಫರ್_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_ಕೊನೆ(0)
    {
        src_beg = ದ್ವಿಮಾನ_ಬಫರ್.ದತ್ತಾಂಶ();
        src_ಕೊನೆ = src_beg + ಬಫರ್_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_ಬಹುಲಕ);

        ಆದರೆ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ಆದರೆ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ಲಿಖ(ಸ್ಥಿರಾಂಕ uchar * beg, ಸ್ಥಿರಾಂಕ uchar * end)
    {
        ಆದರೆ (beg >= end)
            ಹಿಂತಿರುಗಿ *ಇದು;

        ಆಗ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_ಕೊನೆ - src_cur);
            std::ಸ್ಮರಣೆ_ನಕಲು(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ಆದರೆ (src_cur >= src_ಕೊನೆ) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        ಹಿಂತಿರುಗಿ *ಇದು;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t> ಸಾಲೊಳಗೆ
    Base64ContextEmitter & ಲಿಖ(ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t & convertor)
    {
        constexpr size_t ಬಫರ್_MAX_LEN = 1024U;

        uchar ಬಫರ್[ಬಫರ್_MAX_LEN];
        uchar * beg = ಬಫರ್;
        uchar * end = beg;

        ಆಗ (convertor) {
            convertor >> end;
            ಲಿಖ(beg, end);
            end = beg;
        }

        ಹಿಂತಿರುಗಿ *ಇದು;
    }

    ತಾರ್ಕಿಕ flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_ಬಫರ್.ದತ್ತಾಂಶ(), 0U, src_cur - src_beg);
        ಆದರೆ (len == 0U)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        src_cur = src_beg;

        ಆದರೆ ( !needs_indent)
        {
            file_storage.puts((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)base64_ಬಫರ್.ದತ್ತಾಂಶ());
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ newline[] = "\n";
            ಅಕ್ಷರ space[80];
            ಪೂರ್ಣಾಂಕ ident = file_storage.write_ರಾಶಿ.back().indent;
            memset(space, ' ', static_cast<ಪೂರ್ಣಾಂಕ>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)base64_ಬಫರ್.ದತ್ತಾಂಶ());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

ಖಾಸಗಿ:
    /* because of Base64, we must keep its length a multiple of 3 */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ಖಾಸಗಿ:
    cv::FileStorage::Impl& file_storage;
    ತಾರ್ಕಿಕ needs_indent;

    std::vector<uchar> ದ್ವಿಮಾನ_ಬಫರ್;
    std::vector<uchar> base64_ಬಫರ್;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_ಕೊನೆ;
};

std::string base64::make_base64_ಶೀರ್ಷಿಕೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string ಬಫರ್(oss.str());
    CV_Assert(ಬಫರ್.size() < ::base64::ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ);

    ಬಫರ್.reserve(::base64::ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ);
    ಆಗ (ಬಫರ್.size() < ::base64::ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ)
        ಬಫರ್ += ' ';

    ಹಿಂತಿರುಗಿ ಬಫರ್;
}

size_t base64::base64_encode(ಸ್ಥಿರಾಂಕ uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ಆದರೆ (!src || !dst || !cnt)
        ಹಿಂತಿರುಗಿ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ಸ್ಥಿರಾಂಕ * src_beg = src + off;
    uint8_t ಸ್ಥಿರಾಂಕ * src_cur = src_beg;
    uint8_t ಸ್ಥಿರಾಂಕ * src_ಕೊನೆ = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ಆಗ (src_cur < src_ಕೊನೆ) {
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
    ಆದರೆ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    ಆಯ್ಕೆ (rst)
    {
        ಸ್ಥಿತಿ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ಸ್ಥಿತಿ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ಪೂರ್ವನಿಯೋಜಿತ: *dst_cur   = 0;
            ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ static_cast<size_t>(dst_cur - dst_beg);
}

ಪೂರ್ಣಾಂಕ base64::icv_Calc_Struct_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ) {
    ಪೂರ್ಣಾಂಕ size = cv::fs::calc_Elem_ಗಾತ್ರ( dt, initial_ಗಾತ್ರ );
    size_t elem_max_ಗಾತ್ರ = 0;
    ಗಾಗಿ ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * type = dt; *type != '\0'; type++ ) {
        ಆಯ್ಕೆ ( *type )
        {
            ಸ್ಥಿತಿ 'u': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uchar ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'b': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uchar ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'c': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(schar ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'w': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ushort) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 's': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಗಿಡ್ಡ ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'i': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ   ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'n': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'f': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'd': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ದ್ವಿಗುಣ) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'I': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(int64_t)); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'U': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uint64_t)); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'e': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uchar) ); ನಿಲ್ಲಿಸು; }
            ಸ್ಥಿತಿ 'E': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uchar) ); ನಿಲ್ಲಿಸು; }
            ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
        }
    }
    size = cvAlign( size, static_cast<ಪೂರ್ಣಾಂಕ>(elem_max_ಗಾತ್ರ) );
    ಹಿಂತಿರುಗಿ size;
}

size_t base64::base64_encode_ಬಫರ್_ಗಾತ್ರ(size_t cnt, ತಾರ್ಕಿಕ is_ಕೊನೆ_with_zero) {
    size_t additional = static_cast<size_t>(is_ಕೊನೆ_with_zero == ನಿಜ);
    ಹಿಂತಿರುಗಿ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, ತಾರ್ಕಿಕ can_indent)
        : emitter(ಹೊಸ Base64ContextEmitter(fs, can_indent))
        , ದತ್ತಾಂಶ_type_string()
{
    CV_Assert(fs.write_ಬಹುಲಕ);
}

ಶೂನ್ಯ base64::Base64Writer::ಲಿಖ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt)
{
    check_dt(dt);
    Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor convertor(ದತ್ತಾಂಶ_3, static_cast<ಪೂರ್ಣಾಂಕ>(len), ದತ್ತಾಂಶ_type_string);
    emitter->ಲಿಖ(convertor);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t> ಸಾಲೊಳಗೆ
ಶೂನ್ಯ base64::Base64Writer::ಲಿಖ(ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t & convertor, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt)
{
    check_dt(dt);
    emitter->ಲಿಖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    ಅಳಿಸು emitter;
}

ಶೂನ್ಯ base64::Base64Writer::check_dt(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt)
{
    ಆದರೆ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದತ್ತಾಂಶ_type_string.empty()) {
        ದತ್ತಾಂಶ_type_string = dt;

        /* output header */
        std::string ಬಫರ್ = make_base64_ಶೀರ್ಷಿಕೆ(dt);
        ಸ್ಥಿರಾಂಕ uchar * beg = reinterpret_cast<ಸ್ಥಿರಾಂಕ uchar *>(ಬಫರ್.ದತ್ತಾಂಶ());
        ಸ್ಥಿರಾಂಕ uchar * end = beg + ಬಫರ್.size();

        emitter->ಲಿಖ(beg, end);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ದತ್ತಾಂಶ_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ std::string & dt)
        : beg(reinterpret_cast<ಸ್ಥಿರಾಂಕ uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    ಹೆಜ್ಜೆ_packed = make_ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs(dt);

    end = beg;
    cur = beg;

    ಹೆಜ್ಜೆ = icv_Calc_Struct_ಗಾತ್ರ(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

ಸಾಲೊಳಗೆ  base64::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor&  base64::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor::ಕ್ರಿಯಾಕಾರಕ >>(uchar * & dst)
{
    CV_DbgAssert(*ಇದು);

    ಗಾಗಿ (size_t i = 0U, n = ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs.size(); i < n; i++) {
        elem_ಗಮ್ಯ_ದ್ವಿಮಾನ_t & pack = ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs[i];
        pack.func(cur + pack.offset, dst + pack.ಸ್ಥಾನಾಂತರ_packed);
    }
    cur += ಹೆಜ್ಜೆ;
    dst += ಹೆಜ್ಜೆ_packed;

    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಸಾಲೊಳಗೆ  base64::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor::ಕ್ರಿಯಾಕಾರಕ ತಾರ್ಕಿಕ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ cur < end;
}

size_t base64::Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor::make_ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs(ಸ್ಥಿರಾಂಕ std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t ಸ್ಥಾನಾಂತರ_packed = 0;
    ಅಕ್ಷರ type = '\0';

    std::istringstream iss(dt);
    ಆಗ (!iss.eof()) {
        ಆದರೆ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ಆದರೆ (!(iss >> type))
            ನಿಲ್ಲಿಸು;

        ಆಗ (cnt-- > 0)
        {
            elem_ಗಮ್ಯ_ದ್ವಿಮಾನ_t pack;

            size_t size = 0;
            ಆಯ್ಕೆ (type)
            {
                ಸ್ಥಿತಿ 'u':
                ಸ್ಥಿತಿ 'c':
                    size = ಗಾತ್ರ(uchar);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<uchar>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'b':
                    size = ಗಾತ್ರ(uchar);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<ತಾರ್ಕಿಕ>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'w':
                ಸ್ಥಿತಿ 's':
                    size = ಗಾತ್ರ(ushort);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<ushort>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'n':
                ಸ್ಥಿತಿ 'i':
                    size = ಗಾತ್ರ(uint);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<uint>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'f':
                    size = ಗಾತ್ರ(ದಶಮಾಂಶ);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<ದಶಮಾಂಶ>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'd':
                    size = ಗಾತ್ರ(ದ್ವಿಗುಣ);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<ದ್ವಿಗುಣ>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'I':
                ಸ್ಥಿತಿ 'U':
                    size = ಗಾತ್ರ(uint64_t);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<uint64_t>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'e':
                ಸ್ಥಿತಿ 'E':
                    size = ಗಾತ್ರ(uchar);
                    pack.func = ಗಮ್ಯ_ದ್ವಿಮಾನ<uchar>;
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'r':
                ಪೂರ್ವನಿಯೋಜಿತ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<ಪೂರ್ಣಾಂಕ>(offset), static_cast<ಪೂರ್ಣಾಂಕ>(size)));
            pack.offset = offset;
            offset += size;

            pack.ಸ್ಥಾನಾಂತರ_packed = ಸ್ಥಾನಾಂತರ_packed;
            ಸ್ಥಾನಾಂತರ_packed += size;

            ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ_packed;
}

}
