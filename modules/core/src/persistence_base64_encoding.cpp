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
    څرګند Base64ContextEmitter(cv::FileStorage::Impl& fs, منطقي needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , دوګونی_بفر(بفر_LEN)
            , base64_بفر(base64_encode_بفر_اندازه(بفر_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_پای_ټکی(0)
    {
        src_beg = دوګونی_بفر.معلومات();
        src_پای_ټکی = src_beg + بفر_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_ډېرپېښ);

        إذا (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        إذا (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & اكتب(ثابت uchar * beg, ثابت uchar * end)
    {
        إذا (beg >= end)
            أرجع *هذا;

        طالما (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_پای_ټکی - src_cur);
            std::حافظه_نقل(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            إذا (src_cur >= src_پای_ټکی) {
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
    کينډۍ<ډول_نوم منزل_دوګونی_convertor_t> مضمن
    Base64ContextEmitter & اكتب(منزل_دوګونی_convertor_t & convertor)
    {
        constexpr size_t بفر_MAX_LEN = 1024U;

        uchar بفر[بفر_MAX_LEN];
        uchar * beg = بفر;
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
        size_t len = base64_encode(src_beg, base64_بفر.معلومات(), 0U, src_cur - src_beg);
        إذا (len == 0U)
            أرجع خطأ;

        src_cur = src_beg;

        إذا ( !needs_indent)
        {
            file_storage.puts((ثابت حرف*)base64_بفر.معلومات());
        }
        وإلا
        {
            ثابت حرف newline[] = "\n";
            حرف space[80];
            صحيح ident = file_storage.write_ډېری.back().indent;
            memset(space, ' ', static_cast<صحيح>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ثابت حرف*)base64_بفر.معلومات());
            file_storage.puts(newline);
            file_storage.flush();
        }

        أرجع ریښتیا;
    }

خاص:
    /* because of Base64, we must keep its length a multiple of 3 */
    ساكن ثابت size_t بفر_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

خاص:
    cv::FileStorage::Impl& file_storage;
    منطقي needs_indent;

    std::vector<uchar> دوګونی_بفر;
    std::vector<uchar> base64_بفر;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_پای_ټکی;
};

std::string base64::make_base64_سرلیک(ثابت حرف *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string بفر(oss.str());
    CV_Assert(بفر.size() < ::base64::سرلیک_اندازه);

    بفر.reserve(::base64::سرلیک_اندازه);
    طالما (بفر.size() < ::base64::سرلیک_اندازه)
        بفر += ' ';

    أرجع بفر;
}

size_t base64::base64_encode(ثابت uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    إذا (!src || !dst || !cnt)
        أرجع 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ثابت * src_beg = src + off;
    uint8_t ثابت * src_cur = src_beg;
    uint8_t ثابت * src_پای_ټکی = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    طالما (src_cur < src_پای_ټکی) {
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
    إذا (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } وإلا إذا (rst == 2U) {
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

صحيح base64::icv_Calc_Struct_اندازه(ثابت حرف *dt, صحيح initial_اندازه) {
    صحيح size = cv::fs::calc_Elem_اندازه( dt, initial_اندازه );
    size_t elem_max_اندازه = 0;
    لكل ( ثابت حرف * type = dt; *type != '\0'; type++ ) {
        بدل ( *type )
        {
            حالة 'u': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uchar ) ); اكسر; }
            حالة 'b': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uchar ) ); اكسر; }
            حالة 'c': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(schar ) ); اكسر; }
            حالة 'w': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(ushort) ); اكسر; }
            حالة 's': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(لنډ ) ); اكسر; }
            حالة 'i': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(صحيح   ) ); اكسر; }
            حالة 'n': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(بینښه) ); اكسر; }
            حالة 'f': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(عائم ) ); اكسر; }
            حالة 'd': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(مزدوج) ); اكسر; }
            حالة 'I': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(int64_t)); اكسر; }
            حالة 'U': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uint64_t)); اكسر; }
            حالة 'e': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uchar) ); اكسر; }
            حالة 'E': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uchar) ); اكسر; }
            افتراضي: اكسر;
        }
    }
    size = cvAlign( size, static_cast<صحيح>(elem_max_اندازه) );
    أرجع size;
}

size_t base64::base64_encode_بفر_اندازه(size_t cnt, منطقي is_پای_ټکی_with_zero) {
    size_t additional = static_cast<size_t>(is_پای_ټکی_with_zero == ریښتیا);
    أرجع (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent)
        : emitter(جديد Base64ContextEmitter(fs, can_indent))
        , معلومات_type_string()
{
    CV_Assert(fs.write_ډېرپېښ);
}

فراغ base64::Base64Writer::اكتب(ثابت فراغ* معلومات_3, size_t len, ثابت حرف* dt)
{
    check_dt(dt);
    Raw_معلومات_منزل_دوګونی_Convertor convertor(معلومات_3, static_cast<صحيح>(len), معلومات_type_string);
    emitter->اكتب(convertor);
}

کينډۍ<ډول_نوم منزل_دوګونی_convertor_t> مضمن
فراغ base64::Base64Writer::اكتب(منزل_دوګونی_convertor_t & convertor, ثابت حرف* dt)
{
    check_dt(dt);
    emitter->اكتب(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    ړنګول emitter;
}

فراغ base64::Base64Writer::check_dt(ثابت حرف* dt)
{
    إذا ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    وإلا إذا (معلومات_type_string.empty()) {
        معلومات_type_string = dt;

        /* output header */
        std::string بفر = make_base64_سرلیک(dt);
        ثابت uchar * beg = reinterpret_cast<ثابت uchar *>(بفر.معلومات());
        ثابت uchar * end = beg + بفر.size();

        emitter->اكتب(beg, end);
    } وإلا إذا ( معلومات_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_معلومات_منزل_دوګونی_Convertor::Raw_معلومات_منزل_دوګونی_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt)
        : beg(reinterpret_cast<ثابت uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    ګام_packed = make_منزل_دوګونی_funcs(dt);

    end = beg;
    cur = beg;

    ګام = icv_Calc_Struct_اندازه(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

مضمن  base64::Raw_معلومات_منزل_دوګونی_Convertor&  base64::Raw_معلومات_منزل_دوګونی_Convertor::چلوونکی >>(uchar * & dst)
{
    CV_DbgAssert(*هذا);

    لكل (size_t i = 0U, n = منزل_دوګونی_funcs.size(); i < n; i++) {
        elem_منزل_دوګونی_t & pack = منزل_دوګونی_funcs[i];
        pack.func(cur + pack.offset, dst + pack.دننه_واټن_packed);
    }
    cur += ګام;
    dst += ګام_packed;

    أرجع *هذا;
}

مضمن  base64::Raw_معلومات_منزل_دوګونی_Convertor::چلوونکی منطقي() ثابت
{
    أرجع cur < end;
}

size_t base64::Raw_معلومات_منزل_دوګونی_Convertor::make_منزل_دوګونی_funcs(ثابت std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t دننه_واټن_packed = 0;
    حرف type = '\0';

    std::istringstream iss(dt);
    طالما (!iss.eof()) {
        إذا (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        إذا (!(iss >> type))
            اكسر;

        طالما (cnt-- > 0)
        {
            elem_منزل_دوګونی_t pack;

            size_t size = 0;
            بدل (type)
            {
                حالة 'u':
                حالة 'c':
                    size = حجم(uchar);
                    pack.func = منزل_دوګونی<uchar>;
                    اكسر;
                حالة 'b':
                    size = حجم(uchar);
                    pack.func = منزل_دوګونی<منطقي>;
                    اكسر;
                حالة 'w':
                حالة 's':
                    size = حجم(ushort);
                    pack.func = منزل_دوګونی<ushort>;
                    اكسر;
                حالة 'n':
                حالة 'i':
                    size = حجم(uint);
                    pack.func = منزل_دوګونی<uint>;
                    اكسر;
                حالة 'f':
                    size = حجم(عائم);
                    pack.func = منزل_دوګونی<عائم>;
                    اكسر;
                حالة 'd':
                    size = حجم(مزدوج);
                    pack.func = منزل_دوګونی<مزدوج>;
                    اكسر;
                حالة 'I':
                حالة 'U':
                    size = حجم(uint64_t);
                    pack.func = منزل_دوګونی<uint64_t>;
                    اكسر;
                حالة 'e':
                حالة 'E':
                    size = حجم(uchar);
                    pack.func = منزل_دوګونی<uchar>;
                    اكسر;
                حالة 'r':
                افتراضي:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<صحيح>(offset), static_cast<صحيح>(size)));
            pack.offset = offset;
            offset += size;

            pack.دننه_واټن_packed = دننه_واټن_packed;
            دننه_واټن_packed += size;

            منزل_دوګونی_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    أرجع دننه_واټن_packed;
}

}
