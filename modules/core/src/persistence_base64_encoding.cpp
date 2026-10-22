// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence_impl.hpp"
#അന്തര്ഭാവ "persistence_base64_encoding.hpp"

നാമാകാശ cv
{

വർഗ്ഗം base64::Base64ContextEmitter
{
പൊതു:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, തർക്കം needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ദ്വിമാന_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = ദ്വിമാന_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        എങ്കിൽ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        എങ്കിൽ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ലിഖ(സ്ഥിരം uchar * beg, സ്ഥിരം uchar * end)
    {
        എങ്കിൽ (beg >= end)
            തിരികെ *ഇത്;

        ആകുമ്പോൾ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            എങ്കിൽ (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        തിരികെ *ഇത്;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename to_ദ്വിമാന_convertor_t> വരിയിൽ
    Base64ContextEmitter & ലിഖ(to_ദ്വിമാന_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        ആകുമ്പോൾ (convertor) {
            convertor >> end;
            ലിഖ(beg, end);
            end = beg;
        }

        തിരികെ *ഇത്;
    }

    തർക്കം flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        എങ്കിൽ (len == 0U)
            തിരികെ അസത്യം;

        src_cur = src_beg;

        എങ്കിൽ ( !needs_indent)
        {
            file_storage.puts((സ്ഥിരം അക്ഷരം*)base64_buffer.data());
        }
        അല്ലെങ്കിൽ
        {
            സ്ഥിരം അക്ഷരം newline[] = "\n";
            അക്ഷരം space[80];
            പൂർണ്ണസംഖ്യ ident = file_storage.write_അട്ടി.back().indent;
            memset(space, ' ', static_cast<പൂർണ്ണസംഖ്യ>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((സ്ഥിരം അക്ഷരം*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        തിരികെ സത്യം;
    }

സ്വകാര്യ:
    /* because of Base64, we must keep its length a multiple of 3 */
    സ്ഥിതം സ്ഥിരം size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

സ്വകാര്യ:
    cv::FileStorage::Impl& file_storage;
    തർക്കം needs_indent;

    std::vector<uchar> ദ്വിമാന_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(സ്ഥിരം അക്ഷരം *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    ആകുമ്പോൾ (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    തിരികെ buffer;
}

size_t base64::base64_encode(സ്ഥിരം uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    എങ്കിൽ (!src || !dst || !cnt)
        തിരികെ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t സ്ഥിരം * src_beg = src + off;
    uint8_t സ്ഥിരം * src_cur = src_beg;
    uint8_t സ്ഥിരം * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ആകുമ്പോൾ (src_cur < src_end) {
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
    എങ്കിൽ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } അല്ലെങ്കിൽ എങ്കിൽ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    തിരഞ്ഞെടുപ്പ് (rst)
    {
        അവസ്ഥ 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        അവസ്ഥ 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        സ്ഥിരസ്ഥിതി: *dst_cur   = 0;
            നിർത്തുക;
    }

    തിരികെ static_cast<size_t>(dst_cur - dst_beg);
}

പൂർണ്ണസംഖ്യ base64::icvCalcStructSize(സ്ഥിരം അക്ഷരം *dt, പൂർണ്ണസംഖ്യ initial_size) {
    പൂർണ്ണസംഖ്യ size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    വേണ്ടി ( സ്ഥിരം അക്ഷരം * type = dt; *type != '\0'; type++ ) {
        തിരഞ്ഞെടുപ്പ് ( *type )
        {
            അവസ്ഥ 'u': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uchar ) ); നിർത്തുക; }
            അവസ്ഥ 'b': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uchar ) ); നിർത്തുക; }
            അവസ്ഥ 'c': { elem_max_size = std::max( elem_max_size, വലുപ്പം(schar ) ); നിർത്തുക; }
            അവസ്ഥ 'w': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ushort) ); നിർത്തുക; }
            അവസ്ഥ 's': { elem_max_size = std::max( elem_max_size, വലുപ്പം(short ) ); നിർത്തുക; }
            അവസ്ഥ 'i': { elem_max_size = std::max( elem_max_size, വലുപ്പം(പൂർണ്ണസംഖ്യ   ) ); നിർത്തുക; }
            അവസ്ഥ 'n': { elem_max_size = std::max( elem_max_size, വലുപ്പം(unsigned) ); നിർത്തുക; }
            അവസ്ഥ 'f': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ദശാംശം ) ); നിർത്തുക; }
            അവസ്ഥ 'd': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ഇരട്ട) ); നിർത്തുക; }
            അവസ്ഥ 'I': { elem_max_size = std::max( elem_max_size, വലുപ്പം(int64_t)); നിർത്തുക; }
            അവസ്ഥ 'U': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uint64_t)); നിർത്തുക; }
            അവസ്ഥ 'e': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uchar) ); നിർത്തുക; }
            അവസ്ഥ 'E': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uchar) ); നിർത്തുക; }
            സ്ഥിരസ്ഥിതി: നിർത്തുക;
        }
    }
    size = cvAlign( size, static_cast<പൂർണ്ണസംഖ്യ>(elem_max_size) );
    തിരികെ size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, തർക്കം is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == സത്യം);
    തിരികെ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, തർക്കം can_indent)
        : emitter(പുതിയ Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

ശൂന്യം base64::Base64Writer::ലിഖ(സ്ഥിരം ശൂന്യം* _data, size_t len, സ്ഥിരം അക്ഷരം* dt)
{
    check_dt(dt);
    Raw_Data_To_ദ്വിമാന_Convertor convertor(_data, static_cast<പൂർണ്ണസംഖ്യ>(len), data_type_string);
    emitter->ലിഖ(convertor);
}

template<typename to_ദ്വിമാന_convertor_t> വരിയിൽ
ശൂന്യം base64::Base64Writer::ലിഖ(to_ദ്വിമാന_convertor_t & convertor, സ്ഥിരം അക്ഷരം* dt)
{
    check_dt(dt);
    emitter->ലിഖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

ശൂന്യം base64::Base64Writer::check_dt(സ്ഥിരം അക്ഷരം* dt)
{
    എങ്കിൽ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    അല്ലെങ്കിൽ എങ്കിൽ (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        സ്ഥിരം uchar * beg = reinterpret_cast<സ്ഥിരം uchar *>(buffer.data());
        സ്ഥിരം uchar * end = beg + buffer.size();

        emitter->ലിഖ(beg, end);
    } അല്ലെങ്കിൽ എങ്കിൽ ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_ദ്വിമാന_Convertor::Raw_Data_To_ദ്വിമാന_Convertor(സ്ഥിരം ശൂന്യം* src, പൂർണ്ണസംഖ്യ len, സ്ഥിരം std::string & dt)
        : beg(reinterpret_cast<സ്ഥിരം uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_ദ്വിമാന_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

വരിയിൽ  base64::Raw_Data_To_ദ്വിമാന_Convertor&  base64::Raw_Data_To_ദ്വിമാന_Convertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*ഇത്);

    വേണ്ടി (size_t i = 0U, n = to_ദ്വിമാന_funcs.size(); i < n; i++) {
        elem_to_ദ്വിമാന_t & pack = to_ദ്വിമാന_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    തിരികെ *ഇത്;
}

വരിയിൽ  base64::Raw_Data_To_ദ്വിമാന_Convertor::operator തർക്കം() സ്ഥിരം
{
    തിരികെ cur < end;
}

size_t base64::Raw_Data_To_ദ്വിമാന_Convertor::make_to_ദ്വിമാന_funcs(സ്ഥിരം std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    അക്ഷരം type = '\0';

    std::istringstream iss(dt);
    ആകുമ്പോൾ (!iss.eof()) {
        എങ്കിൽ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        എങ്കിൽ (!(iss >> type))
            നിർത്തുക;

        ആകുമ്പോൾ (cnt-- > 0)
        {
            elem_to_ദ്വിമാന_t pack;

            size_t size = 0;
            തിരഞ്ഞെടുപ്പ് (type)
            {
                അവസ്ഥ 'u':
                അവസ്ഥ 'c':
                    size = വലുപ്പം(uchar);
                    pack.func = to_ദ്വിമാന<uchar>;
                    നിർത്തുക;
                അവസ്ഥ 'b':
                    size = വലുപ്പം(uchar);
                    pack.func = to_ദ്വിമാന<തർക്കം>;
                    നിർത്തുക;
                അവസ്ഥ 'w':
                അവസ്ഥ 's':
                    size = വലുപ്പം(ushort);
                    pack.func = to_ദ്വിമാന<ushort>;
                    നിർത്തുക;
                അവസ്ഥ 'n':
                അവസ്ഥ 'i':
                    size = വലുപ്പം(uint);
                    pack.func = to_ദ്വിമാന<uint>;
                    നിർത്തുക;
                അവസ്ഥ 'f':
                    size = വലുപ്പം(ദശാംശം);
                    pack.func = to_ദ്വിമാന<ദശാംശം>;
                    നിർത്തുക;
                അവസ്ഥ 'd':
                    size = വലുപ്പം(ഇരട്ട);
                    pack.func = to_ദ്വിമാന<ഇരട്ട>;
                    നിർത്തുക;
                അവസ്ഥ 'I':
                അവസ്ഥ 'U':
                    size = വലുപ്പം(uint64_t);
                    pack.func = to_ദ്വിമാന<uint64_t>;
                    നിർത്തുക;
                അവസ്ഥ 'e':
                അവസ്ഥ 'E':
                    size = വലുപ്പം(uchar);
                    pack.func = to_ദ്വിമാന<uchar>;
                    നിർത്തുക;
                അവസ്ഥ 'r':
                സ്ഥിരസ്ഥിതി:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<പൂർണ്ണസംഖ്യ>(offset), static_cast<പൂർണ്ണസംഖ്യ>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_ദ്വിമാന_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    തിരികെ offset_packed;
}

}
