// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "persistence_impl.hpp"
#समावेश "persistence_base64_encoding.hpp"

namespace cv
{

श्रेणी base64::Base64ContextEmitter
{
खुला:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, bool needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , द्विआधारी_बफर(बफर_LEN)
            , base64_बफर(base64_encode_बफर_आकार(बफर_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = द्विआधारी_बफर.तथ्य();
        src_end = src_beg + बफर_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        अगर (needs_indent)
        {
            file_storage.सफाई();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        अगर (src_cur != src_beg)
            सफाई();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & ग_लिखो(स्थिर uchar * beg, स्थिर uchar * end)
    {
        अगर (beg >= end)
            वापस *यह;

        जबतक (beg < end) {
            /* collect binary data and copy to binary buffer */
            माप_प्रकार len = std::min(end - beg, src_end - src_cur);
            std::स_नकल(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            अगर (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                सफाई();
            }
        }

        वापस *यह;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    ढाँचा<typename to_द्विआधारी_convertor_t> अंतरभूत
    Base64ContextEmitter & ग_लिखो(to_द्विआधारी_convertor_t & convertor)
    {
        constexpr माप_प्रकार बफर_MAX_LEN = 1024U;

        uchar बफर[बफर_MAX_LEN];
        uchar * beg = बफर;
        uchar * end = beg;

        जबतक (convertor) {
            convertor >> end;
            ग_लिखो(beg, end);
            end = beg;
        }

        वापस *यह;
    }

    bool सफाई()
    {
        /* control line width, so on. */
        माप_प्रकार len = base64_encode(src_beg, base64_बफर.तथ्य(), 0U, src_cur - src_beg);
        अगर (len == 0U)
            वापस false;

        src_cur = src_beg;

        अगर ( !needs_indent)
        {
            file_storage.माला_दो((स्थिर अक्षर*)base64_बफर.तथ्य());
        }
        अन्यथा
        {
            स्थिर अक्षर newline[] = "\n";
            अक्षर space[80];
            पूर्णांक ident = file_storage.write_ढेर.back().indent;
            स_रखो(space, ' ', static_cast<पूर्णांक>(ident));
            space[ident] = '\0';

            file_storage.माला_दो(space);
            file_storage.माला_दो((स्थिर अक्षर*)base64_बफर.तथ्य());
            file_storage.माला_दो(newline);
            file_storage.सफाई();
        }

        वापस true;
    }

निजी:
    /* because of Base64, we must keep its length a multiple of 3 */
    अटल स्थिर माप_प्रकार बफर_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

निजी:
    cv::FileStorage::Impl& file_storage;
    bool needs_indent;

    std::vector<uchar> द्विआधारी_बफर;
    std::vector<uchar> base64_बफर;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(स्थिर अक्षर *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string बफर(oss.str());
    CV_Assert(बफर.size() < ::base64::HEADER_आकार);

    बफर.reserve(::base64::HEADER_आकार);
    जबतक (बफर.size() < ::base64::HEADER_आकार)
        बफर += ' ';

    वापस बफर;
}

माप_प्रकार base64::base64_encode(स्थिर uint8_t *src, uint8_t *dst, माप_प्रकार off, माप_प्रकार cnt) {
    अगर (!src || !dst || !cnt)
        वापस 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t स्थिर * src_beg = src + off;
    uint8_t स्थिर * src_cur = src_beg;
    uint8_t स्थिर * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    जबतक (src_cur < src_end) {
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
    अगर (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } अन्यथा अगर (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    चयन (rst)
    {
        हाल 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        हाल 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        शेष: *dst_cur   = 0;
            अवरोध;
    }

    वापस static_cast<माप_प्रकार>(dst_cur - dst_beg);
}

पूर्णांक base64::icv_Calc_Struct_आकार(स्थिर अक्षर *dt, पूर्णांक initial_आकार) {
    पूर्णांक size = cv::fs::calc_Elem_आकार( dt, initial_आकार );
    माप_प्रकार elem_max_आकार = 0;
    क्रम ( स्थिर अक्षर * type = dt; *type != '\0'; type++ ) {
        चयन ( *type )
        {
            हाल 'u': { elem_max_आकार = std::max( elem_max_आकार, माप(uchar ) ); अवरोध; }
            हाल 'b': { elem_max_आकार = std::max( elem_max_आकार, माप(uchar ) ); अवरोध; }
            हाल 'c': { elem_max_आकार = std::max( elem_max_आकार, माप(schar ) ); अवरोध; }
            हाल 'w': { elem_max_आकार = std::max( elem_max_आकार, माप(ushort) ); अवरोध; }
            हाल 's': { elem_max_आकार = std::max( elem_max_आकार, माप(लघु ) ); अवरोध; }
            हाल 'i': { elem_max_आकार = std::max( elem_max_आकार, माप(पूर्णांक   ) ); अवरोध; }
            हाल 'n': { elem_max_आकार = std::max( elem_max_आकार, माप(अचिन्हित) ); अवरोध; }
            हाल 'f': { elem_max_आकार = std::max( elem_max_आकार, माप(भग्न ) ); अवरोध; }
            हाल 'd': { elem_max_आकार = std::max( elem_max_आकार, माप(द्विगुन) ); अवरोध; }
            हाल 'I': { elem_max_आकार = std::max( elem_max_आकार, माप(int64_t)); अवरोध; }
            हाल 'U': { elem_max_आकार = std::max( elem_max_आकार, माप(uint64_t)); अवरोध; }
            हाल 'e': { elem_max_आकार = std::max( elem_max_आकार, माप(uchar) ); अवरोध; }
            हाल 'E': { elem_max_आकार = std::max( elem_max_आकार, माप(uchar) ); अवरोध; }
            शेष: अवरोध;
        }
    }
    size = cvAlign( size, static_cast<पूर्णांक>(elem_max_आकार) );
    वापस size;
}

माप_प्रकार base64::base64_encode_बफर_आकार(माप_प्रकार cnt, bool is_end_with_zero) {
    माप_प्रकार additional = static_cast<माप_प्रकार>(is_end_with_zero == true);
    वापस (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, bool can_indent)
        : emitter(नया Base64ContextEmitter(fs, can_indent))
        , तथ्य_type_string()
{
    CV_Assert(fs.write_mode);
}

व्योम base64::Base64Writer::ग_लिखो(स्थिर व्योम* तथ्य_3, माप_प्रकार len, स्थिर अक्षर* dt)
{
    check_dt(dt);
    Raw_तथ्य_To_द्विआधारी_Convertor convertor(तथ्य_3, static_cast<पूर्णांक>(len), तथ्य_type_string);
    emitter->ग_लिखो(convertor);
}

ढाँचा<typename to_द्विआधारी_convertor_t> अंतरभूत
व्योम base64::Base64Writer::ग_लिखो(to_द्विआधारी_convertor_t & convertor, स्थिर अक्षर* dt)
{
    check_dt(dt);
    emitter->ग_लिखो(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    मिटाओ emitter;
}

व्योम base64::Base64Writer::check_dt(स्थिर अक्षर* dt)
{
    अगर ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    अन्यथा अगर (तथ्य_type_string.empty()) {
        तथ्य_type_string = dt;

        /* output header */
        std::string बफर = make_base64_header(dt);
        स्थिर uchar * beg = reinterpret_cast<स्थिर uchar *>(बफर.तथ्य());
        स्थिर uchar * end = beg + बफर.size();

        emitter->ग_लिखो(beg, end);
    } अन्यथा अगर ( तथ्य_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_तथ्य_To_द्विआधारी_Convertor::Raw_तथ्य_To_द्विआधारी_Convertor(स्थिर व्योम* src, पूर्णांक len, स्थिर std::string & dt)
        : beg(reinterpret_cast<स्थिर uchar *>(src))
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

    step = icv_Calc_Struct_आकार(dt.c_str(), 0);
    end = beg + static_cast<माप_प्रकार>(len);
}

अंतरभूत  base64::Raw_तथ्य_To_द्विआधारी_Convertor&  base64::Raw_तथ्य_To_द्विआधारी_Convertor::चालक >>(uchar * & dst)
{
    CV_DbgAssert(*यह);

    क्रम (माप_प्रकार i = 0U, n = to_द्विआधारी_funcs.size(); i < n; i++) {
        elem_to_द्विआधारी_t & pack = to_द्विआधारी_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    वापस *यह;
}

अंतरभूत  base64::Raw_तथ्य_To_द्विआधारी_Convertor::चालक bool() स्थिर
{
    वापस cur < end;
}

माप_प्रकार base64::Raw_तथ्य_To_द्विआधारी_Convertor::make_to_द्विआधारी_funcs(स्थिर std::string &dt)
{
    माप_प्रकार cnt = 0;
    माप_प्रकार offset = 0;
    माप_प्रकार offset_packed = 0;
    अक्षर type = '\0';

    std::istringstream iss(dt);
    जबतक (!iss.खप()) {
        अगर (!(iss >> cnt)) {
            iss.सफा();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        अगर (!(iss >> type))
            अवरोध;

        जबतक (cnt-- > 0)
        {
            elem_to_द्विआधारी_t pack;

            माप_प्रकार size = 0;
            चयन (type)
            {
                हाल 'u':
                हाल 'c':
                    size = माप(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    अवरोध;
                हाल 'b':
                    size = माप(uchar);
                    pack.func = to_द्विआधारी<bool>;
                    अवरोध;
                हाल 'w':
                हाल 's':
                    size = माप(ushort);
                    pack.func = to_द्विआधारी<ushort>;
                    अवरोध;
                हाल 'n':
                हाल 'i':
                    size = माप(uint);
                    pack.func = to_द्विआधारी<uint>;
                    अवरोध;
                हाल 'f':
                    size = माप(भग्न);
                    pack.func = to_द्विआधारी<भग्न>;
                    अवरोध;
                हाल 'd':
                    size = माप(द्विगुन);
                    pack.func = to_द्विआधारी<द्विगुन>;
                    अवरोध;
                हाल 'I':
                हाल 'U':
                    size = माप(uint64_t);
                    pack.func = to_द्विआधारी<uint64_t>;
                    अवरोध;
                हाल 'e':
                हाल 'E':
                    size = माप(uchar);
                    pack.func = to_द्विआधारी<uchar>;
                    अवरोध;
                हाल 'r':
                शेष:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<माप_प्रकार>(cvAlign(static_cast<पूर्णांक>(offset), static_cast<पूर्णांक>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_द्विआधारी_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.खप());
    वापस offset_packed;
}

}
