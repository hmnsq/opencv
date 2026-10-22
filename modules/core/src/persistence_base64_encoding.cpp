// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence_impl.hpp"
#அந்தர்பாவ "persistence_base64_encoding.hpp"

நாமாகாஶ cv
{

வகுப்பு base64::Base64ContextEmitter
{
பொது:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, தருக்க needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , இருமம்_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = இருமம்_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        எனில் (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        எனில் (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & லிக(மாறிலி uchar * beg, மாறிலி uchar * end)
    {
        எனில் (beg >= end)
            திரும்பு *இது;

        போது (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            எனில் (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        திரும்பு *இது;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_இருமம்_convertor_t> உள்வரி
    Base64ContextEmitter & லிக(to_இருமம்_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        போது (convertor) {
            convertor >> end;
            லிக(beg, end);
            end = beg;
        }

        திரும்பு *இது;
    }

    தருக்க flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        எனில் (len == 0U)
            திரும்பு பொய்;

        src_cur = src_beg;

        எனில் ( !needs_indent)
        {
            file_storage.puts((மாறிலி எழுத்து*)base64_buffer.data());
        }
        இல்லையெனில்
        {
            மாறிலி எழுத்து newline[] = "\n";
            எழுத்து space[80];
            முழு ident = file_storage.write_அடுக்கு.back().indent;
            memset(space, ' ', static_cast<முழு>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((மாறிலி எழுத்து*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        திரும்பு உண்மை;
    }

தனிப்பட்ட:
    /* because of Base64, we must keep its length a multiple of 3 */
    நிலையான மாறிலி size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

தனிப்பட்ட:
    cv::FileStorage::Impl& file_storage;
    தருக்க needs_indent;

    std::vector<uchar> இருமம்_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(மாறிலி எழுத்து *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    போது (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    திரும்பு buffer;
}

size_t base64::base64_encode(மாறிலி uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    எனில் (!src || !dst || !cnt)
        திரும்பு 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t மாறிலி * src_beg = src + off;
    uint8_t மாறிலி * src_cur = src_beg;
    uint8_t மாறிலி * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    போது (src_cur < src_end) {
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
    எனில் (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } இல்லையெனில் எனில் (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    தேர்வு (rst)
    {
        நிலை 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        நிலை 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        இயல்பு: *dst_cur   = 0;
            நிறுத்து;
    }

    திரும்பு static_cast<size_t>(dst_cur - dst_beg);
}

முழு base64::icvCalcStructSize(மாறிலி எழுத்து *dt, முழு initial_size) {
    முழு size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    க்கு ( மாறிலி எழுத்து * type = dt; *type != '\0'; type++ ) {
        தேர்வு ( *type )
        {
            நிலை 'u': { elem_max_size = std::max( elem_max_size, அளவு(uchar ) ); நிறுத்து; }
            நிலை 'b': { elem_max_size = std::max( elem_max_size, அளவு(uchar ) ); நிறுத்து; }
            நிலை 'c': { elem_max_size = std::max( elem_max_size, அளவு(schar ) ); நிறுத்து; }
            நிலை 'w': { elem_max_size = std::max( elem_max_size, அளவு(ushort) ); நிறுத்து; }
            நிலை 's': { elem_max_size = std::max( elem_max_size, அளவு(short ) ); நிறுத்து; }
            நிலை 'i': { elem_max_size = std::max( elem_max_size, அளவு(முழு   ) ); நிறுத்து; }
            நிலை 'n': { elem_max_size = std::max( elem_max_size, அளவு(unsigned) ); நிறுத்து; }
            நிலை 'f': { elem_max_size = std::max( elem_max_size, அளவு(தசம ) ); நிறுத்து; }
            நிலை 'd': { elem_max_size = std::max( elem_max_size, அளவு(இரட்டை) ); நிறுத்து; }
            நிலை 'I': { elem_max_size = std::max( elem_max_size, அளவு(int64_t)); நிறுத்து; }
            நிலை 'U': { elem_max_size = std::max( elem_max_size, அளவு(uint64_t)); நிறுத்து; }
            நிலை 'e': { elem_max_size = std::max( elem_max_size, அளவு(uchar) ); நிறுத்து; }
            நிலை 'E': { elem_max_size = std::max( elem_max_size, அளவு(uchar) ); நிறுத்து; }
            இயல்பு: நிறுத்து;
        }
    }
    size = cvAlign( size, static_cast<முழு>(elem_max_size) );
    திரும்பு size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, தருக்க is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == உண்மை);
    திரும்பு (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, தருக்க can_indent)
        : emitter(புதிய Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

வெறுமை base64::Base64Writer::லிக(மாறிலி வெறுமை* _data, size_t len, மாறிலி எழுத்து* dt)
{
    check_dt(dt);
    Raw_Data_To_இருமம்_Convertor convertor(_data, static_cast<முழு>(len), data_type_string);
    emitter->லிக(convertor);
}

template<typename to_இருமம்_convertor_t> உள்வரி
வெறுமை base64::Base64Writer::லிக(to_இருமம்_convertor_t & convertor, மாறிலி எழுத்து* dt)
{
    check_dt(dt);
    emitter->லிக(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

வெறுமை base64::Base64Writer::check_dt(மாறிலி எழுத்து* dt)
{
    எனில் ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    இல்லையெனில் எனில் (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        மாறிலி uchar * beg = reinterpret_cast<மாறிலி uchar *>(buffer.data());
        மாறிலி uchar * end = beg + buffer.size();

        emitter->லிக(beg, end);
    } இல்லையெனில் எனில் ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_இருமம்_Convertor::Raw_Data_To_இருமம்_Convertor(மாறிலி வெறுமை* src, முழு len, மாறிலி std::string & dt)
        : beg(reinterpret_cast<மாறிலி uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_இருமம்_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

உள்வரி  base64::Raw_Data_To_இருமம்_Convertor&  base64::Raw_Data_To_இருமம்_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*இது);

    க்கு (size_t i = 0U, n = to_இருமம்_funcs.size(); i < n; i++) {
        elem_to_இருமம்_t & pack = to_இருமம்_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    திரும்பு *இது;
}

உள்வரி  base64::Raw_Data_To_இருமம்_Convertor::operator தருக்க() மாறிலி
{
    திரும்பு cur < end;
}

size_t base64::Raw_Data_To_இருமம்_Convertor::make_to_இருமம்_funcs(மாறிலி std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    எழுத்து type = '\0';

    std::istringstream iss(dt);
    போது (!iss.eof()) {
        எனில் (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        எனில் (!(iss >> type))
            நிறுத்து;

        போது (cnt-- > 0)
        {
            elem_to_இருமம்_t pack;

            size_t size = 0;
            தேர்வு (type)
            {
                நிலை 'u':
                நிலை 'c':
                    size = அளவு(uchar);
                    pack.func = to_இருமம்<uchar>;
                    நிறுத்து;
                நிலை 'b':
                    size = அளவு(uchar);
                    pack.func = to_இருமம்<தருக்க>;
                    நிறுத்து;
                நிலை 'w':
                நிலை 's':
                    size = அளவு(ushort);
                    pack.func = to_இருமம்<ushort>;
                    நிறுத்து;
                நிலை 'n':
                நிலை 'i':
                    size = அளவு(uint);
                    pack.func = to_இருமம்<uint>;
                    நிறுத்து;
                நிலை 'f':
                    size = அளவு(தசம);
                    pack.func = to_இருமம்<தசம>;
                    நிறுத்து;
                நிலை 'd':
                    size = அளவு(இரட்டை);
                    pack.func = to_இருமம்<இரட்டை>;
                    நிறுத்து;
                நிலை 'I':
                நிலை 'U':
                    size = அளவு(uint64_t);
                    pack.func = to_இருமம்<uint64_t>;
                    நிறுத்து;
                நிலை 'e':
                நிலை 'E':
                    size = அளவு(uchar);
                    pack.func = to_இருமம்<uchar>;
                    நிறுத்து;
                நிலை 'r':
                இயல்பு:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<முழு>(offset), static_cast<முழு>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_இருமம்_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    திரும்பு offset_packed;
}

}
