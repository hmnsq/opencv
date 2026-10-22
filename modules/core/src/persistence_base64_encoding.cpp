// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence_impl.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"

नामाकाश cv
{

वर्ग base64::Base64ContextEmitter
{
सार्वजनिक:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, तार्किक needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , द्विमान_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = द्विमान_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        जर (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        जर (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & लिख(स्थिरांक uchar * beg, स्थिरांक uchar * end)
    {
        जर (beg >= end)
            परत *हे;

        पर्यंत (beg < end) {
            /* collect binary data and copy to binary buffer */
            माप_प्रकार len = std::min(end - beg, src_end - src_cur);
            std::स_नकल(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            जर (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        परत *हे;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    ढाँचा<typename to_द्विमान_convertor_t> ओळीत
    Base64ContextEmitter & लिख(to_द्विमान_convertor_t & convertor)
    {
        constexpr माप_प्रकार BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        पर्यंत (convertor) {
            convertor >> end;
            लिख(beg, end);
            end = beg;
        }

        परत *हे;
    }

    तार्किक flush()
    {
        /* control line width, so on. */
        माप_प्रकार len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        जर (len == 0U)
            परत असत्य;

        src_cur = src_beg;

        जर ( !needs_indent)
        {
            file_storage.माला_दो((स्थिरांक अक्षर*)base64_buffer.data());
        }
        अन्यथा
        {
            स्थिरांक अक्षर newline[] = "\n";
            अक्षर space[80];
            पूर्णांक ident = file_storage.write_चळत.back().indent;
            स_रखो(space, ' ', static_cast<पूर्णांक>(ident));
            space[ident] = '\0';

            file_storage.माला_दो(space);
            file_storage.माला_दो((स्थिरांक अक्षर*)base64_buffer.data());
            file_storage.माला_दो(newline);
            file_storage.flush();
        }

        परत सत्य;
    }

खाजगी:
    /* because of Base64, we must keep its length a multiple of 3 */
    स्थिर स्थिरांक माप_प्रकार BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

खाजगी:
    cv::FileStorage::Impl& file_storage;
    तार्किक needs_indent;

    std::vector<uchar> द्विमान_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(स्थिरांक अक्षर *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    पर्यंत (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    परत buffer;
}

माप_प्रकार base64::base64_encode(स्थिरांक uint8_t *src, uint8_t *dst, माप_प्रकार off, माप_प्रकार cnt) {
    जर (!src || !dst || !cnt)
        परत 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t स्थिरांक * src_beg = src + off;
    uint8_t स्थिरांक * src_cur = src_beg;
    uint8_t स्थिरांक * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    पर्यंत (src_cur < src_end) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        uint8_t _0 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_1 & 0xF0U) >> 4U | (_2 & 0x03U) << 4U];
        *dst_cur++ = base64_mapping[(_0 & 0xC0U) >> 6U | (_1 & 0x0FU) << 2U];
        *dst_cur++ = base64_mapping[ _0 & 0x3FU];
    }

    /* remainder part */
    माप_प्रकार rst = src_beg + cnt - src_cur;
    जर (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } अन्यथा जर (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    निवड (rst)
    {
        स्थिती 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        स्थिती 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        पूर्वनिर्धारित: *dst_cur   = 0;
            थांबा;
    }

    परत static_cast<माप_प्रकार>(dst_cur - dst_beg);
}

पूर्णांक base64::icvCalcStructSize(स्थिरांक अक्षर *dt, पूर्णांक initial_size) {
    पूर्णांक size = cv::fs::calcElemSize( dt, initial_size );
    माप_प्रकार elem_max_size = 0;
    साठी ( स्थिरांक अक्षर * type = dt; *type != '\0'; type++ ) {
        निवड ( *type )
        {
            स्थिती 'u': { elem_max_size = std::max( elem_max_size, आकार(uchar ) ); थांबा; }
            स्थिती 'b': { elem_max_size = std::max( elem_max_size, आकार(uchar ) ); थांबा; }
            स्थिती 'c': { elem_max_size = std::max( elem_max_size, आकार(schar ) ); थांबा; }
            स्थिती 'w': { elem_max_size = std::max( elem_max_size, आकार(ushort) ); थांबा; }
            स्थिती 's': { elem_max_size = std::max( elem_max_size, आकार(लघु ) ); थांबा; }
            स्थिती 'i': { elem_max_size = std::max( elem_max_size, आकार(पूर्णांक   ) ); थांबा; }
            स्थिती 'n': { elem_max_size = std::max( elem_max_size, आकार(अचिन्हित) ); थांबा; }
            स्थिती 'f': { elem_max_size = std::max( elem_max_size, आकार(दशांश ) ); थांबा; }
            स्थिती 'd': { elem_max_size = std::max( elem_max_size, आकार(द्विगुण) ); थांबा; }
            स्थिती 'I': { elem_max_size = std::max( elem_max_size, आकार(int64_t)); थांबा; }
            स्थिती 'U': { elem_max_size = std::max( elem_max_size, आकार(uint64_t)); थांबा; }
            स्थिती 'e': { elem_max_size = std::max( elem_max_size, आकार(uchar) ); थांबा; }
            स्थिती 'E': { elem_max_size = std::max( elem_max_size, आकार(uchar) ); थांबा; }
            पूर्वनिर्धारित: थांबा;
        }
    }
    size = cvAlign( size, static_cast<पूर्णांक>(elem_max_size) );
    परत size;
}

माप_प्रकार base64::base64_encode_buffer_size(माप_प्रकार cnt, तार्किक is_end_with_zero) {
    माप_प्रकार additional = static_cast<माप_प्रकार>(is_end_with_zero == सत्य);
    परत (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, तार्किक can_indent)
        : emitter(नवीन Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

शून्य base64::Base64Writer::लिख(स्थिरांक शून्य* _data, माप_प्रकार len, स्थिरांक अक्षर* dt)
{
    check_dt(dt);
    Raw_Data_To_द्विमान_Convertor convertor(_data, static_cast<पूर्णांक>(len), data_type_string);
    emitter->लिख(convertor);
}

ढाँचा<typename to_द्विमान_convertor_t> ओळीत
शून्य base64::Base64Writer::लिख(to_द्विमान_convertor_t & convertor, स्थिरांक अक्षर* dt)
{
    check_dt(dt);
    emitter->लिख(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

शून्य base64::Base64Writer::check_dt(स्थिरांक अक्षर* dt)
{
    जर ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    अन्यथा जर (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        स्थिरांक uchar * beg = reinterpret_cast<स्थिरांक uchar *>(buffer.data());
        स्थिरांक uchar * end = beg + buffer.size();

        emitter->लिख(beg, end);
    } अन्यथा जर ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_द्विमान_Convertor::Raw_Data_To_द्विमान_Convertor(स्थिरांक शून्य* src, पूर्णांक len, स्थिरांक std::string & dt)
        : beg(reinterpret_cast<स्थिरांक uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_द्विमान_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<माप_प्रकार>(len);
}

ओळीत  base64::Raw_Data_To_द्विमान_Convertor&  base64::Raw_Data_To_द्विमान_Convertor::चालक >>(uchar * & dst)
{
    CV_DbgAssert(*हे);

    साठी (माप_प्रकार i = 0U, n = to_द्विमान_funcs.size(); i < n; i++) {
        elem_to_द्विमान_t & pack = to_द्विमान_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    परत *हे;
}

ओळीत  base64::Raw_Data_To_द्विमान_Convertor::चालक तार्किक() स्थिरांक
{
    परत cur < end;
}

माप_प्रकार base64::Raw_Data_To_द्विमान_Convertor::make_to_द्विमान_funcs(स्थिरांक std::string &dt)
{
    माप_प्रकार cnt = 0;
    माप_प्रकार offset = 0;
    माप_प्रकार offset_packed = 0;
    अक्षर type = '\0';

    std::istringstream iss(dt);
    पर्यंत (!iss.eof()) {
        जर (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        जर (!(iss >> type))
            थांबा;

        पर्यंत (cnt-- > 0)
        {
            elem_to_द्विमान_t pack;

            माप_प्रकार size = 0;
            निवड (type)
            {
                स्थिती 'u':
                स्थिती 'c':
                    size = आकार(uchar);
                    pack.func = to_द्विमान<uchar>;
                    थांबा;
                स्थिती 'b':
                    size = आकार(uchar);
                    pack.func = to_द्विमान<तार्किक>;
                    थांबा;
                स्थिती 'w':
                स्थिती 's':
                    size = आकार(ushort);
                    pack.func = to_द्विमान<ushort>;
                    थांबा;
                स्थिती 'n':
                स्थिती 'i':
                    size = आकार(uint);
                    pack.func = to_द्विमान<uint>;
                    थांबा;
                स्थिती 'f':
                    size = आकार(दशांश);
                    pack.func = to_द्विमान<दशांश>;
                    थांबा;
                स्थिती 'd':
                    size = आकार(द्विगुण);
                    pack.func = to_द्विमान<द्विगुण>;
                    थांबा;
                स्थिती 'I':
                स्थिती 'U':
                    size = आकार(uint64_t);
                    pack.func = to_द्विमान<uint64_t>;
                    थांबा;
                स्थिती 'e':
                स्थिती 'E':
                    size = आकार(uchar);
                    pack.func = to_द्विमान<uchar>;
                    थांबा;
                स्थिती 'r':
                पूर्वनिर्धारित:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<माप_प्रकार>(cvAlign(static_cast<पूर्णांक>(offset), static_cast<पूर्णांक>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_द्विमान_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    परत offset_packed;
}

}
