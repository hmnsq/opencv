// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence_impl.hpp"
#شامل "persistence_base64_encoding.hpp"

فضاینام cv
{

رده base64::Base64ContextEmitter
{
عمومی:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, منطقی needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , دودویی_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = دودویی_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

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
            بازگشت *این;

        وقتی (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            اگر (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        بازگشت *این;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_دودویی_convertor_t> درونخطی
    Base64ContextEmitter & اكتب(to_دودویی_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        وقتی (convertor) {
            convertor >> end;
            اكتب(beg, end);
            end = beg;
        }

        بازگشت *این;
    }

    منطقی flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        اگر (len == 0U)
            بازگشت نادرست;

        src_cur = src_beg;

        اگر ( !needs_indent)
        {
            file_storage.puts((ثابت نویسه*)base64_buffer.data());
        }
        وگرنه
        {
            ثابت نویسه newline[] = "\n";
            نویسه space[80];
            صحیح ident = file_storage.write_پشته.back().indent;
            memset(space, ' ', static_cast<صحیح>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((ثابت نویسه*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        بازگشت درست;
    }

خصوصی:
    /* because of Base64, we must keep its length a multiple of 3 */
    ایستا ثابت size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

خصوصی:
    cv::FileStorage::Impl& file_storage;
    منطقی needs_indent;

    std::vector<uchar> دودویی_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(ثابت نویسه *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    وقتی (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    بازگشت buffer;
}

size_t base64::base64_encode(ثابت uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    اگر (!src || !dst || !cnt)
        بازگشت 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t ثابت * src_beg = src + off;
    uint8_t ثابت * src_cur = src_beg;
    uint8_t ثابت * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    وقتی (src_cur < src_end) {
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
    } وگرنه اگر (rst == 2U) {
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
        پیشفرض: *dst_cur   = 0;
            بشکن;
    }

    بازگشت static_cast<size_t>(dst_cur - dst_beg);
}

صحیح base64::icvCalcStructSize(ثابت نویسه *dt, صحیح initial_size) {
    صحیح size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    برای ( ثابت نویسه * type = dt; *type != '\0'; type++ ) {
        انتخاب ( *type )
        {
            حالت 'u': { elem_max_size = std::max( elem_max_size, اندازه(uchar ) ); بشکن; }
            حالت 'b': { elem_max_size = std::max( elem_max_size, اندازه(uchar ) ); بشکن; }
            حالت 'c': { elem_max_size = std::max( elem_max_size, اندازه(schar ) ); بشکن; }
            حالت 'w': { elem_max_size = std::max( elem_max_size, اندازه(ushort) ); بشکن; }
            حالت 's': { elem_max_size = std::max( elem_max_size, اندازه(short ) ); بشکن; }
            حالت 'i': { elem_max_size = std::max( elem_max_size, اندازه(صحیح   ) ); بشکن; }
            حالت 'n': { elem_max_size = std::max( elem_max_size, اندازه(unsigned) ); بشکن; }
            حالت 'f': { elem_max_size = std::max( elem_max_size, اندازه(اعشاری ) ); بشکن; }
            حالت 'd': { elem_max_size = std::max( elem_max_size, اندازه(دوگانه) ); بشکن; }
            حالت 'I': { elem_max_size = std::max( elem_max_size, اندازه(int64_t)); بشکن; }
            حالت 'U': { elem_max_size = std::max( elem_max_size, اندازه(uint64_t)); بشکن; }
            حالت 'e': { elem_max_size = std::max( elem_max_size, اندازه(uchar) ); بشکن; }
            حالت 'E': { elem_max_size = std::max( elem_max_size, اندازه(uchar) ); بشکن; }
            پیشفرض: بشکن;
        }
    }
    size = cvAlign( size, static_cast<صحیح>(elem_max_size) );
    بازگشت size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, منطقی is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == درست);
    بازگشت (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, منطقی can_indent)
        : emitter(جدید Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

تهی base64::Base64Writer::اكتب(ثابت تهی* _data, size_t len, ثابت نویسه* dt)
{
    check_dt(dt);
    Raw_Data_To_دودویی_Convertor convertor(_data, static_cast<صحیح>(len), data_type_string);
    emitter->اكتب(convertor);
}

template<typename to_دودویی_convertor_t> درونخطی
تهی base64::Base64Writer::اكتب(to_دودویی_convertor_t & convertor, ثابت نویسه* dt)
{
    check_dt(dt);
    emitter->اكتب(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

تهی base64::Base64Writer::check_dt(ثابت نویسه* dt)
{
    اگر ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    وگرنه اگر (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        ثابت uchar * beg = reinterpret_cast<ثابت uchar *>(buffer.data());
        ثابت uchar * end = beg + buffer.size();

        emitter->اكتب(beg, end);
    } وگرنه اگر ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_دودویی_Convertor::Raw_Data_To_دودویی_Convertor(ثابت تهی* src, صحیح len, ثابت std::string & dt)
        : beg(reinterpret_cast<ثابت uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_دودویی_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

درونخطی  base64::Raw_Data_To_دودویی_Convertor&  base64::Raw_Data_To_دودویی_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*این);

    برای (size_t i = 0U, n = to_دودویی_funcs.size(); i < n; i++) {
        elem_to_دودویی_t & pack = to_دودویی_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    بازگشت *این;
}

درونخطی  base64::Raw_Data_To_دودویی_Convertor::operator منطقی() ثابت
{
    بازگشت cur < end;
}

size_t base64::Raw_Data_To_دودویی_Convertor::make_to_دودویی_funcs(ثابت std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    نویسه type = '\0';

    std::istringstream iss(dt);
    وقتی (!iss.eof()) {
        اگر (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        اگر (!(iss >> type))
            بشکن;

        وقتی (cnt-- > 0)
        {
            elem_to_دودویی_t pack;

            size_t size = 0;
            انتخاب (type)
            {
                حالت 'u':
                حالت 'c':
                    size = اندازه(uchar);
                    pack.func = to_دودویی<uchar>;
                    بشکن;
                حالت 'b':
                    size = اندازه(uchar);
                    pack.func = to_دودویی<منطقی>;
                    بشکن;
                حالت 'w':
                حالت 's':
                    size = اندازه(ushort);
                    pack.func = to_دودویی<ushort>;
                    بشکن;
                حالت 'n':
                حالت 'i':
                    size = اندازه(uint);
                    pack.func = to_دودویی<uint>;
                    بشکن;
                حالت 'f':
                    size = اندازه(اعشاری);
                    pack.func = to_دودویی<اعشاری>;
                    بشکن;
                حالت 'd':
                    size = اندازه(دوگانه);
                    pack.func = to_دودویی<دوگانه>;
                    بشکن;
                حالت 'I':
                حالت 'U':
                    size = اندازه(uint64_t);
                    pack.func = to_دودویی<uint64_t>;
                    بشکن;
                حالت 'e':
                حالت 'E':
                    size = اندازه(uchar);
                    pack.func = to_دودویی<uchar>;
                    بشکن;
                حالت 'r':
                پیشفرض:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<صحیح>(offset), static_cast<صحیح>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_دودویی_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    بازگشت offset_packed;
}

}
