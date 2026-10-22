// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence_impl.hpp"
#অন্তর্ভাব "persistence_base64_encoding.hpp"

নামাকাশ cv
{

শ্রেণী base64::Base64ContextEmitter
{
সর্বজনীন:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, যৌক্তিক needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , দ্বিমিক_বাফার(বাফার_LEN)
            , base64_বাফার(base64_encode_বাফার_আকার(বাফার_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = দ্বিমিক_বাফার.তথ্য();
        src_end = src_beg + বাফার_LEN;
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

    Base64ContextEmitter & লিখ(ধ্রুবক uchar * beg, ধ্রুবক uchar * end)
    {
        যদি (beg >= end)
            ফেরত *এই;

        যখন (beg < end) {
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

        ফেরত *এই;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_দ্বিমিক_convertor_t> অন্তঃরেখ
    Base64ContextEmitter & লিখ(to_দ্বিমিক_convertor_t & convertor)
    {
        constexpr size_t বাফার_MAX_LEN = 1024U;

        uchar বাফার[বাফার_MAX_LEN];
        uchar * beg = বাফার;
        uchar * end = beg;

        যখন (convertor) {
            convertor >> end;
            লিখ(beg, end);
            end = beg;
        }

        ফেরত *এই;
    }

    যৌক্তিক flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_বাফার.তথ্য(), 0U, src_cur - src_beg);
        যদি (len == 0U)
            ফেরত মিথ্যা;

        src_cur = src_beg;

        যদি ( !needs_indent)
        {
            file_storage.puts((ধ্রুবক অক্ষর*)base64_বাফার.তথ্য());
        }
        অন্যথা
        {
            ধ্রুবক অক্ষর newline[] = "\n";
            অক্ষর space[80];
            পূর্ণসংখ্যা ident = file_storage.write_স্তূপ.back().indent;
            memset(space, ' ', static_cast<পূর্ণসংখ্যা>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ধ্রুবক অক্ষর*)base64_বাফার.তথ্য());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ফেরত সত্য;
    }

ব্যক্তিগত:
    /* because of Base64, we must keep its length a multiple of 3 */
    স্থির ধ্রুবক size_t বাফার_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

ব্যক্তিগত:
    cv::FileStorage::Impl& file_storage;
    যৌক্তিক needs_indent;

    std::vector<uchar> দ্বিমিক_বাফার;
    std::vector<uchar> base64_বাফার;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ধ্রুবক অক্ষর *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string বাফার(oss.str());
    CV_Assert(বাফার.size() < ::base64::HEADER_আকার);

    বাফার.reserve(::base64::HEADER_আকার);
    যখন (বাফার.size() < ::base64::HEADER_আকার)
        বাফার += ' ';

    ফেরত বাফার;
}

size_t base64::base64_encode(ধ্রুবক uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    যদি (!src || !dst || !cnt)
        ফেরত 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ধ্রুবক * src_beg = src + off;
    uint8_t ধ্রুবক * src_cur = src_beg;
    uint8_t ধ্রুবক * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    যখন (src_cur < src_end) {
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
    নির্বাচন (rst)
    {
        ক্ষেত্র 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        ক্ষেত্র 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        পূর্বনির্ধারিত: *dst_cur   = 0;
            বিরতি;
    }

    ফেরত static_cast<size_t>(dst_cur - dst_beg);
}

পূর্ণসংখ্যা base64::icv_Calc_Struct_আকার(ধ্রুবক অক্ষর *dt, পূর্ণসংখ্যা initial_আকার) {
    পূর্ণসংখ্যা size = cv::fs::calc_Elem_আকার( dt, initial_আকার );
    size_t elem_max_আকার = 0;
    জন্য ( ধ্রুবক অক্ষর * type = dt; *type != '\0'; type++ ) {
        নির্বাচন ( *type )
        {
            ক্ষেত্র 'u': { elem_max_আকার = std::max( elem_max_আকার, আকার(uchar ) ); বিরতি; }
            ক্ষেত্র 'b': { elem_max_আকার = std::max( elem_max_আকার, আকার(uchar ) ); বিরতি; }
            ক্ষেত্র 'c': { elem_max_আকার = std::max( elem_max_আকার, আকার(schar ) ); বিরতি; }
            ক্ষেত্র 'w': { elem_max_আকার = std::max( elem_max_আকার, আকার(ushort) ); বিরতি; }
            ক্ষেত্র 's': { elem_max_আকার = std::max( elem_max_আকার, আকার(short ) ); বিরতি; }
            ক্ষেত্র 'i': { elem_max_আকার = std::max( elem_max_আকার, আকার(পূর্ণসংখ্যা   ) ); বিরতি; }
            ক্ষেত্র 'n': { elem_max_আকার = std::max( elem_max_আকার, আকার(unsigned) ); বিরতি; }
            ক্ষেত্র 'f': { elem_max_আকার = std::max( elem_max_আকার, আকার(দশমিক ) ); বিরতি; }
            ক্ষেত্র 'd': { elem_max_আকার = std::max( elem_max_আকার, আকার(দ্বিগুণ) ); বিরতি; }
            ক্ষেত্র 'I': { elem_max_আকার = std::max( elem_max_আকার, আকার(int64_t)); বিরতি; }
            ক্ষেত্র 'U': { elem_max_আকার = std::max( elem_max_আকার, আকার(uint64_t)); বিরতি; }
            ক্ষেত্র 'e': { elem_max_আকার = std::max( elem_max_আকার, আকার(uchar) ); বিরতি; }
            ক্ষেত্র 'E': { elem_max_আকার = std::max( elem_max_আকার, আকার(uchar) ); বিরতি; }
            পূর্বনির্ধারিত: বিরতি;
        }
    }
    size = cvAlign( size, static_cast<পূর্ণসংখ্যা>(elem_max_আকার) );
    ফেরত size;
}

size_t base64::base64_encode_বাফার_আকার(size_t cnt, যৌক্তিক is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == সত্য);
    ফেরত (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, যৌক্তিক can_indent)
        : emitter(নতুন Base64ContextEmitter(fs, can_indent))
        , তথ্য_type_string()
{
    CV_Assert(fs.write_mode);
}

শূন্য base64::Base64Writer::লিখ(ধ্রুবক শূন্য* তথ্য_3, size_t len, ধ্রুবক অক্ষর* dt)
{
    check_dt(dt);
    Raw_তথ্য_To_দ্বিমিক_Convertor convertor(তথ্য_3, static_cast<পূর্ণসংখ্যা>(len), তথ্য_type_string);
    emitter->লিখ(convertor);
}

template<typename to_দ্বিমিক_convertor_t> অন্তঃরেখ
শূন্য base64::Base64Writer::লিখ(to_দ্বিমিক_convertor_t & convertor, ধ্রুবক অক্ষর* dt)
{
    check_dt(dt);
    emitter->লিখ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

শূন্য base64::Base64Writer::check_dt(ধ্রুবক অক্ষর* dt)
{
    যদি ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    অন্যথা যদি (তথ্য_type_string.empty()) {
        তথ্য_type_string = dt;

        /* output header */
        std::string বাফার = make_base64_header(dt);
        ধ্রুবক uchar * beg = reinterpret_cast<ধ্রুবক uchar *>(বাফার.তথ্য());
        ধ্রুবক uchar * end = beg + বাফার.size();

        emitter->লিখ(beg, end);
    } অন্যথা যদি ( তথ্য_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_তথ্য_To_দ্বিমিক_Convertor::Raw_তথ্য_To_দ্বিমিক_Convertor(ধ্রুবক শূন্য* src, পূর্ণসংখ্যা len, ধ্রুবক std::string & dt)
        : beg(reinterpret_cast<ধ্রুবক uchar *>(src))
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

    step = icv_Calc_Struct_আকার(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

অন্তঃরেখ  base64::Raw_তথ্য_To_দ্বিমিক_Convertor&  base64::Raw_তথ্য_To_দ্বিমিক_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*এই);

    জন্য (size_t i = 0U, n = to_দ্বিমিক_funcs.size(); i < n; i++) {
        elem_to_দ্বিমিক_t & pack = to_দ্বিমিক_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    ফেরত *এই;
}

অন্তঃরেখ  base64::Raw_তথ্য_To_দ্বিমিক_Convertor::operator যৌক্তিক() ধ্রুবক
{
    ফেরত cur < end;
}

size_t base64::Raw_তথ্য_To_দ্বিমিক_Convertor::make_to_দ্বিমিক_funcs(ধ্রুবক std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    অক্ষর type = '\0';

    std::istringstream iss(dt);
    যখন (!iss.eof()) {
        যদি (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        যদি (!(iss >> type))
            বিরতি;

        যখন (cnt-- > 0)
        {
            elem_to_দ্বিমিক_t pack;

            size_t size = 0;
            নির্বাচন (type)
            {
                ক্ষেত্র 'u':
                ক্ষেত্র 'c':
                    size = আকার(uchar);
                    pack.func = to_দ্বিমিক<uchar>;
                    বিরতি;
                ক্ষেত্র 'b':
                    size = আকার(uchar);
                    pack.func = to_দ্বিমিক<যৌক্তিক>;
                    বিরতি;
                ক্ষেত্র 'w':
                ক্ষেত্র 's':
                    size = আকার(ushort);
                    pack.func = to_দ্বিমিক<ushort>;
                    বিরতি;
                ক্ষেত্র 'n':
                ক্ষেত্র 'i':
                    size = আকার(uint);
                    pack.func = to_দ্বিমিক<uint>;
                    বিরতি;
                ক্ষেত্র 'f':
                    size = আকার(দশমিক);
                    pack.func = to_দ্বিমিক<দশমিক>;
                    বিরতি;
                ক্ষেত্র 'd':
                    size = আকার(দ্বিগুণ);
                    pack.func = to_দ্বিমিক<দ্বিগুণ>;
                    বিরতি;
                ক্ষেত্র 'I':
                ক্ষেত্র 'U':
                    size = আকার(uint64_t);
                    pack.func = to_দ্বিমিক<uint64_t>;
                    বিরতি;
                ক্ষেত্র 'e':
                ক্ষেত্র 'E':
                    size = আকার(uchar);
                    pack.func = to_দ্বিমিক<uchar>;
                    বিরতি;
                ক্ষেত্র 'r':
                পূর্বনির্ধারিত:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<পূর্ণসংখ্যা>(offset), static_cast<পূর্ণসংখ্যা>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_দ্বিমিক_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ফেরত offset_packed;
}

}
