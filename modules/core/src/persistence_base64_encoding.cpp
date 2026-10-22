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
            , द्विआधारी_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = द्विआधारी_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        यदि (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        यदि (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & लिख(अचर uchar * beg, अचर uchar * end)
    {
        यदि (beg >= end)
            फर्काऊ *एतत्;

        जबसम्म (beg < end) {
            /* collect binary data and copy to binary buffer */
            माप_प्रकार len = std::min(end - beg, src_end - src_cur);
            std::स_नकल(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            यदि (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        फर्काऊ *एतत्;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    ढाँचा<typename to_द्विआधारी_convertor_t> अन्तःस्थ
    Base64ContextEmitter & लिख(to_द्विआधारी_convertor_t & convertor)
    {
        constexpr माप_प्रकार BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        जबसम्म (convertor) {
            convertor >> end;
            लिख(beg, end);
            end = beg;
        }

        फर्काऊ *एतत्;
    }

    तार्किक flush()
    {
        /* control line width, so on. */
        माप_प्रकार len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        यदि (len == 0U)
            फर्काऊ असत्य;

        src_cur = src_beg;

        यदि ( !needs_indent)
        {
            file_storage.माला_दो((अचर वर्ण*)base64_buffer.data());
        }
        अन्यथा
        {
            अचर वर्ण newline[] = "\n";
            वर्ण space[80];
            पूर्णाङ्क ident = file_storage.write_थाक.back().indent;
            स_रखो(space, ' ', static_cast<पूर्णाङ्क>(ident));
            space[ident] = '\0';

            file_storage.माला_दो(space);
            file_storage.माला_दो((अचर वर्ण*)base64_buffer.data());
            file_storage.माला_दो(newline);
            file_storage.flush();
        }

        फर्काऊ सत्य;
    }

गुप्त:
    /* because of Base64, we must keep its length a multiple of 3 */
    स्थिर अचर माप_प्रकार BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

गुप्त:
    cv::FileStorage::Impl& file_storage;
    तार्किक needs_indent;

    std::vector<uchar> द्विआधारी_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(अचर वर्ण *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    जबसम्म (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    फर्काऊ buffer;
}

माप_प्रकार base64::base64_encode(अचर uint8_t *src, uint8_t *dst, माप_प्रकार off, माप_प्रकार cnt) {
    यदि (!src || !dst || !cnt)
        फर्काऊ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t अचर * src_beg = src + off;
    uint8_t अचर * src_cur = src_beg;
    uint8_t अचर * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    जबसम्म (src_cur < src_end) {
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
    यदि (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } अन्यथा यदि (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    विकल्प (rst)
    {
        पक्ष 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        पक्ष 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        स्वभाव: *dst_cur   = 0;
            रोक;
    }

    फर्काऊ static_cast<माप_प्रकार>(dst_cur - dst_beg);
}

पूर्णाङ्क base64::icvCalcStructSize(अचर वर्ण *dt, पूर्णाङ्क initial_size) {
    पूर्णाङ्क size = cv::fs::calcElemSize( dt, initial_size );
    माप_प्रकार elem_max_size = 0;
    लागि ( अचर वर्ण * type = dt; *type != '\0'; type++ ) {
        विकल्प ( *type )
        {
            पक्ष 'u': { elem_max_size = std::max( elem_max_size, परिमाण(uchar ) ); रोक; }
            पक्ष 'b': { elem_max_size = std::max( elem_max_size, परिमाण(uchar ) ); रोक; }
            पक्ष 'c': { elem_max_size = std::max( elem_max_size, परिमाण(schar ) ); रोक; }
            पक्ष 'w': { elem_max_size = std::max( elem_max_size, परिमाण(ushort) ); रोक; }
            पक्ष 's': { elem_max_size = std::max( elem_max_size, परिमाण(लघु ) ); रोक; }
            पक्ष 'i': { elem_max_size = std::max( elem_max_size, परिमाण(पूर्णाङ्क   ) ); रोक; }
            पक्ष 'n': { elem_max_size = std::max( elem_max_size, परिमाण(अचिन्हित) ); रोक; }
            पक्ष 'f': { elem_max_size = std::max( elem_max_size, परिमाण(भिन्न ) ); रोक; }
            पक्ष 'd': { elem_max_size = std::max( elem_max_size, परिमाण(द्विगुण) ); रोक; }
            पक्ष 'I': { elem_max_size = std::max( elem_max_size, परिमाण(int64_t)); रोक; }
            पक्ष 'U': { elem_max_size = std::max( elem_max_size, परिमाण(uint64_t)); रोक; }
            पक्ष 'e': { elem_max_size = std::max( elem_max_size, परिमाण(uchar) ); रोक; }
            पक्ष 'E': { elem_max_size = std::max( elem_max_size, परिमाण(uchar) ); रोक; }
            स्वभाव: रोक;
        }
    }
    size = cvAlign( size, static_cast<पूर्णाङ्क>(elem_max_size) );
    फर्काऊ size;
}

माप_प्रकार base64::base64_encode_buffer_size(माप_प्रकार cnt, तार्किक is_end_with_zero) {
    माप_प्रकार additional = static_cast<माप_प्रकार>(is_end_with_zero == सत्य);
    फर्काऊ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, तार्किक can_indent)
        : emitter(नूतन Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

शून्य base64::Base64Writer::लिख(अचर शून्य* _data, माप_प्रकार len, अचर वर्ण* dt)
{
    check_dt(dt);
    Raw_Data_To_द्विआधारी_Convertor convertor(_data, static_cast<पूर्णाङ्क>(len), data_type_string);
    emitter->लिख(convertor);
}

ढाँचा<typename to_द्विआधारी_convertor_t> अन्तःस्थ
शून्य base64::Base64Writer::लिख(to_द्विआधारी_convertor_t & convertor, अचर वर्ण* dt)
{
    check_dt(dt);
    emitter->लिख(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

शून्य base64::Base64Writer::check_dt(अचर वर्ण* dt)
{
    यदि ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    अन्यथा यदि (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        अचर uchar * beg = reinterpret_cast<अचर uchar *>(buffer.data());
        अचर uchar * end = beg + buffer.size();

        emitter->लिख(beg, end);
    } अन्यथा यदि ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_Data_To_द्विआधारी_Convertor::Raw_Data_To_द्विआधारी_Convertor(अचर शून्य* src, पूर्णाङ्क len, अचर std::string & dt)
        : beg(reinterpret_cast<अचर uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_द्विआधारी_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<माप_प्रकार>(len);
}

अन्तःस्थ  base64::Raw_Data_To_द्विआधारी_Convertor&  base64::Raw_Data_To_द्विआधारी_Convertor::चालक >>(uchar * & dst)
{
    CV_DbgAssert(*एतत्);

    लागि (माप_प्रकार i = 0U, n = to_द्विआधारी_funcs.size(); i < n; i++) {
        elem_to_द्विआधारी_t & pack = to_द्विआधारी_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    फर्काऊ *एतत्;
}

अन्तःस्थ  base64::Raw_Data_To_द्विआधारी_Convertor::चालक तार्किक() अचर
{
    फर्काऊ cur < end;
}

माप_प्रकार base64::Raw_Data_To_द्विआधारी_Convertor::make_to_द्विआधारी_funcs(अचर std::string &dt)
{
    माप_प्रकार cnt = 0;
    माप_प्रकार offset = 0;
    माप_प्रकार offset_packed = 0;
    वर्ण type = '\0';

    std::istringstream iss(dt);
    जबसम्म (!iss.eof()) {
        यदि (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        यदि (!(iss >> type))
            रोक;

        जबसम्म (cnt-- > 0)
        {
            elem_to_द्विआधारी_t pack;

            माप_प्रकार size = 0;
            विकल्प (type)
            {
                पक्ष 'u':
                पक्ष 'c':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    रोक;
                पक्ष 'b':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<तार्किक>;
                    रोक;
                पक्ष 'w':
                पक्ष 's':
                    size = परिमाण(ushort);
                    pack.func = to_द्विआधारी<ushort>;
                    रोक;
                पक्ष 'n':
                पक्ष 'i':
                    size = परिमाण(uint);
                    pack.func = to_द्विआधारी<uint>;
                    रोक;
                पक्ष 'f':
                    size = परिमाण(भिन्न);
                    pack.func = to_द्विआधारी<भिन्न>;
                    रोक;
                पक्ष 'd':
                    size = परिमाण(द्विगुण);
                    pack.func = to_द्विआधारी<द्विगुण>;
                    रोक;
                पक्ष 'I':
                पक्ष 'U':
                    size = परिमाण(uint64_t);
                    pack.func = to_द्विआधारी<uint64_t>;
                    रोक;
                पक्ष 'e':
                पक्ष 'E':
                    size = परिमाण(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    रोक;
                पक्ष 'r':
                स्वभाव:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<माप_प्रकार>(cvAlign(static_cast<पूर्णाङ्क>(offset), static_cast<पूर्णाङ्क>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_द्विआधारी_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    फर्काऊ offset_packed;
}

}
