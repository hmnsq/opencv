// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence_impl.hpp"
#شامل "persistence_base64_encoding.hpp"

نامخلا cv
{

جماعت base64::Base64ContextEmitter
{
عوامی:
    صریح Base64ContextEmitter(cv::FileStorage::Impl& fs, منطقی needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ثنائی_بفر(بفر_LEN)
            , base64_بفر(base64_encode_بفر_سائز(بفر_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_انتہا(0)
    {
        src_beg = ثنائی_بفر.ڈیٹا();
        src_انتہا = src_beg + بفر_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_منوال);

        اگر (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        اگر (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & اكتب(ثابت uchar * beg, ثابت uchar * end)
    {
        اگر (beg >= end)
            واپس *یہ;

        جب (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_انتہا - src_cur);
            std::حافظہ_نقل(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            اگر (src_cur >= src_انتہا) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        واپس *یہ;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    سانچہ<قسم_نام مقصد_ثنائی_convertor_t> اندرونی
    Base64ContextEmitter & اكتب(مقصد_ثنائی_convertor_t & convertor)
    {
        constexpr size_t بفر_MAX_LEN = 1024U;

        uchar بفر[بفر_MAX_LEN];
        uchar * beg = بفر;
        uchar * end = beg;

        جب (convertor) {
            convertor >> end;
            اكتب(beg, end);
            end = beg;
        }

        واپس *یہ;
    }

    منطقی flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_بفر.ڈیٹا(), 0U, src_cur - src_beg);
        اگر (len == 0U)
            واپس جھوٹ;

        src_cur = src_beg;

        اگر ( !needs_indent)
        {
            file_storage.puts((ثابت حرف*)base64_بفر.ڈیٹا());
        }
        ورنہ
        {
            ثابت حرف newline[] = "\n";
            حرف space[80];
            صحیح ident = file_storage.write_ڈھیر.back().indent;
            memset(space, ' ', static_cast<صحیح>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ثابت حرف*)base64_بفر.ڈیٹا());
            file_storage.puts(newline);
            file_storage.flush();
        }

        واپس سچ;
    }

نجی:
    /* because of Base64, we must keep its length a multiple of 3 */
    ساکن ثابت size_t بفر_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

نجی:
    cv::FileStorage::Impl& file_storage;
    منطقی needs_indent;

    std::vector<uchar> ثنائی_بفر;
    std::vector<uchar> base64_بفر;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_انتہا;
};

std::string base64::make_base64_سرخی(ثابت حرف *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string بفر(oss.str());
    CV_Assert(بفر.size() < ::base64::سرخی_سائز);

    بفر.reserve(::base64::سرخی_سائز);
    جب (بفر.size() < ::base64::سرخی_سائز)
        بفر += ' ';

    واپس بفر;
}

size_t base64::base64_encode(ثابت uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    اگر (!src || !dst || !cnt)
        واپس 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ثابت * src_beg = src + off;
    uint8_t ثابت * src_cur = src_beg;
    uint8_t ثابت * src_انتہا = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    جب (src_cur < src_انتہا) {
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
    اگر (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ورنہ اگر (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    انتخاب (rst)
    {
        حالت 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        حالت 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        طےشدہ: *dst_cur   = 0;
            توڑو;
    }

    واپس static_cast<size_t>(dst_cur - dst_beg);
}

صحیح base64::icv_Calc_Struct_سائز(ثابت حرف *dt, صحیح initial_سائز) {
    صحیح size = cv::fs::calc_Elem_سائز( dt, initial_سائز );
    size_t elem_max_سائز = 0;
    برائے ( ثابت حرف * type = dt; *type != '\0'; type++ ) {
        انتخاب ( *type )
        {
            حالت 'u': { elem_max_سائز = std::max( elem_max_سائز, حجم(uchar ) ); توڑو; }
            حالت 'b': { elem_max_سائز = std::max( elem_max_سائز, حجم(uchar ) ); توڑو; }
            حالت 'c': { elem_max_سائز = std::max( elem_max_سائز, حجم(schar ) ); توڑو; }
            حالت 'w': { elem_max_سائز = std::max( elem_max_سائز, حجم(ushort) ); توڑو; }
            حالت 's': { elem_max_سائز = std::max( elem_max_سائز, حجم(مختصر ) ); توڑو; }
            حالت 'i': { elem_max_سائز = std::max( elem_max_سائز, حجم(صحیح   ) ); توڑو; }
            حالت 'n': { elem_max_سائز = std::max( elem_max_سائز, حجم(غیرعلامتی) ); توڑو; }
            حالت 'f': { elem_max_سائز = std::max( elem_max_سائز, حجم(اعشاری ) ); توڑو; }
            حالت 'd': { elem_max_سائز = std::max( elem_max_سائز, حجم(دوہرا) ); توڑو; }
            حالت 'I': { elem_max_سائز = std::max( elem_max_سائز, حجم(int64_t)); توڑو; }
            حالت 'U': { elem_max_سائز = std::max( elem_max_سائز, حجم(uint64_t)); توڑو; }
            حالت 'e': { elem_max_سائز = std::max( elem_max_سائز, حجم(uchar) ); توڑو; }
            حالت 'E': { elem_max_سائز = std::max( elem_max_سائز, حجم(uchar) ); توڑو; }
            طےشدہ: توڑو;
        }
    }
    size = cvAlign( size, static_cast<صحیح>(elem_max_سائز) );
    واپس size;
}

size_t base64::base64_encode_بفر_سائز(size_t cnt, منطقی is_انتہا_with_zero) {
    size_t additional = static_cast<size_t>(is_انتہا_with_zero == سچ);
    واپس (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, منطقی can_indent)
        : emitter(نیا Base64ContextEmitter(fs, can_indent))
        , ڈیٹا_type_string()
{
    CV_Assert(fs.write_منوال);
}

خالی base64::Base64Writer::اكتب(ثابت خالی* ڈیٹا_3, size_t len, ثابت حرف* dt)
{
    check_dt(dt);
    Raw_ڈیٹا_مقصد_ثنائی_Convertor convertor(ڈیٹا_3, static_cast<صحیح>(len), ڈیٹا_type_string);
    emitter->اكتب(convertor);
}

سانچہ<قسم_نام مقصد_ثنائی_convertor_t> اندرونی
خالی base64::Base64Writer::اكتب(مقصد_ثنائی_convertor_t & convertor, ثابت حرف* dt)
{
    check_dt(dt);
    emitter->اكتب(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    حذف emitter;
}

خالی base64::Base64Writer::check_dt(ثابت حرف* dt)
{
    اگر ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ورنہ اگر (ڈیٹا_type_string.empty()) {
        ڈیٹا_type_string = dt;

        /* output header */
        std::string بفر = make_base64_سرخی(dt);
        ثابت uchar * beg = reinterpret_cast<ثابت uchar *>(بفر.ڈیٹا());
        ثابت uchar * end = beg + بفر.size();

        emitter->اكتب(beg, end);
    } ورنہ اگر ( ڈیٹا_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_ڈیٹا_مقصد_ثنائی_Convertor::Raw_ڈیٹا_مقصد_ثنائی_Convertor(ثابت خالی* src, صحیح len, ثابت std::string & dt)
        : beg(reinterpret_cast<ثابت uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    قدم_packed = make_مقصد_ثنائی_funcs(dt);

    end = beg;
    cur = beg;

    قدم = icv_Calc_Struct_سائز(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

اندرونی  base64::Raw_ڈیٹا_مقصد_ثنائی_Convertor&  base64::Raw_ڈیٹا_مقصد_ثنائی_Convertor::عامل >>(uchar * & dst)
{
    CV_DbgAssert(*یہ);

    برائے (size_t i = 0U, n = مقصد_ثنائی_funcs.size(); i < n; i++) {
        elem_مقصد_ثنائی_t & pack = مقصد_ثنائی_funcs[i];
        pack.func(cur + pack.offset, dst + pack.انحراف_packed);
    }
    cur += قدم;
    dst += قدم_packed;

    واپس *یہ;
}

اندرونی  base64::Raw_ڈیٹا_مقصد_ثنائی_Convertor::عامل منطقی() ثابت
{
    واپس cur < end;
}

size_t base64::Raw_ڈیٹا_مقصد_ثنائی_Convertor::make_مقصد_ثنائی_funcs(ثابت std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t انحراف_packed = 0;
    حرف type = '\0';

    std::istringstream iss(dt);
    جب (!iss.eof()) {
        اگر (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        اگر (!(iss >> type))
            توڑو;

        جب (cnt-- > 0)
        {
            elem_مقصد_ثنائی_t pack;

            size_t size = 0;
            انتخاب (type)
            {
                حالت 'u':
                حالت 'c':
                    size = حجم(uchar);
                    pack.func = مقصد_ثنائی<uchar>;
                    توڑو;
                حالت 'b':
                    size = حجم(uchar);
                    pack.func = مقصد_ثنائی<منطقی>;
                    توڑو;
                حالت 'w':
                حالت 's':
                    size = حجم(ushort);
                    pack.func = مقصد_ثنائی<ushort>;
                    توڑو;
                حالت 'n':
                حالت 'i':
                    size = حجم(uint);
                    pack.func = مقصد_ثنائی<uint>;
                    توڑو;
                حالت 'f':
                    size = حجم(اعشاری);
                    pack.func = مقصد_ثنائی<اعشاری>;
                    توڑو;
                حالت 'd':
                    size = حجم(دوہرا);
                    pack.func = مقصد_ثنائی<دوہرا>;
                    توڑو;
                حالت 'I':
                حالت 'U':
                    size = حجم(uint64_t);
                    pack.func = مقصد_ثنائی<uint64_t>;
                    توڑو;
                حالت 'e':
                حالت 'E':
                    size = حجم(uchar);
                    pack.func = مقصد_ثنائی<uchar>;
                    توڑو;
                حالت 'r':
                طےشدہ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<صحیح>(offset), static_cast<صحیح>(size)));
            pack.offset = offset;
            offset += size;

            pack.انحراف_packed = انحراف_packed;
            انحراف_packed += size;

            مقصد_ثنائی_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    واپس انحراف_packed;
}

}
