// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence_impl.hpp"
#అన్తర్భావ "persistence_base64_encoding.hpp"

నామాకాశ cv
{

తరగతి base64::Base64ContextEmitter
{
బహిరంగ:
    స్పష్టమైన Base64ContextEmitter(cv::FileStorage::Impl& fs, తార్కిక needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , ద్విమాన_బఫరు(బఫరు_LEN)
            , base64_బఫరు(base64_encode_బఫరు_కొలత(బఫరు_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_చివర(0)
    {
        src_beg = ద్విమాన_బఫరు.దత్తాంశం();
        src_చివర = src_beg + బఫరు_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_బహుళకం);

        ఒకవేళ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ఒకవేళ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & లిఖ(స్థిరరాశి uchar * beg, స్థిరరాశి uchar * end)
    {
        ఒకవేళ (beg >= end)
            తిరిగి *ఇది;

        అయితే (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_చివర - src_cur);
            std::జ్ఞాపకం_నకలు(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ఒకవేళ (src_cur >= src_చివర) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        తిరిగి *ఇది;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    మూస<రకం_పేరు గమ్యం_ద్విమాన_convertor_t> అంతఃవరుస
    Base64ContextEmitter & లిఖ(గమ్యం_ద్విమాన_convertor_t & convertor)
    {
        constexpr size_t బఫరు_MAX_LEN = 1024U;

        uchar బఫరు[బఫరు_MAX_LEN];
        uchar * beg = బఫరు;
        uchar * end = beg;

        అయితే (convertor) {
            convertor >> end;
            లిఖ(beg, end);
            end = beg;
        }

        తిరిగి *ఇది;
    }

    తార్కిక flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_బఫరు.దత్తాంశం(), 0U, src_cur - src_beg);
        ఒకవేళ (len == 0U)
            తిరిగి అబద్ధం;

        src_cur = src_beg;

        ఒకవేళ ( !needs_indent)
        {
            file_storage.puts((స్థిరరాశి అక్షరం*)base64_బఫరు.దత్తాంశం());
        }
        లేకపోతే
        {
            స్థిరరాశి అక్షరం newline[] = "\n";
            అక్షరం space[80];
            పూర్ణాంకం ident = file_storage.write_దొంతర.back().indent;
            memset(space, ' ', static_cast<పూర్ణాంకం>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((స్థిరరాశి అక్షరం*)base64_బఫరు.దత్తాంశం());
            file_storage.puts(newline);
            file_storage.flush();
        }

        తిరిగి నిజం;
    }

అంతరంగ:
    /* because of Base64, we must keep its length a multiple of 3 */
    స్థిర స్థిరరాశి size_t బఫరు_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

అంతరంగ:
    cv::FileStorage::Impl& file_storage;
    తార్కిక needs_indent;

    std::vector<uchar> ద్విమాన_బఫరు;
    std::vector<uchar> base64_బఫరు;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_చివర;
};

std::string base64::make_base64_శీర్షిక(స్థిరరాశి అక్షరం *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string బఫరు(oss.str());
    CV_Assert(బఫరు.size() < ::base64::శీర్షిక_కొలత);

    బఫరు.reserve(::base64::శీర్షిక_కొలత);
    అయితే (బఫరు.size() < ::base64::శీర్షిక_కొలత)
        బఫరు += ' ';

    తిరిగి బఫరు;
}

size_t base64::base64_encode(స్థిరరాశి uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ఒకవేళ (!src || !dst || !cnt)
        తిరిగి 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t స్థిరరాశి * src_beg = src + off;
    uint8_t స్థిరరాశి * src_cur = src_beg;
    uint8_t స్థిరరాశి * src_చివర = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    అయితే (src_cur < src_చివర) {
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
    ఒకవేళ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } లేకపోతే ఒకవేళ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    ఎంపిక (rst)
    {
        స్థితి 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        స్థితి 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        అప్రమేయ: *dst_cur   = 0;
            ఆపు;
    }

    తిరిగి static_cast<size_t>(dst_cur - dst_beg);
}

పూర్ణాంకం base64::icv_Calc_Struct_కొలత(స్థిరరాశి అక్షరం *dt, పూర్ణాంకం initial_కొలత) {
    పూర్ణాంకం size = cv::fs::calc_Elem_కొలత( dt, initial_కొలత );
    size_t elem_max_కొలత = 0;
    కొరకు ( స్థిరరాశి అక్షరం * type = dt; *type != '\0'; type++ ) {
        ఎంపిక ( *type )
        {
            స్థితి 'u': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uchar ) ); ఆపు; }
            స్థితి 'b': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uchar ) ); ఆపు; }
            స్థితి 'c': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(schar ) ); ఆపు; }
            స్థితి 'w': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(ushort) ); ఆపు; }
            స్థితి 's': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(పొట్టి ) ); ఆపు; }
            స్థితి 'i': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(పూర్ణాంకం   ) ); ఆపు; }
            స్థితి 'n': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(గుర్తులేని) ); ఆపు; }
            స్థితి 'f': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(దశాంశం ) ); ఆపు; }
            స్థితి 'd': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(ద్విగుణ) ); ఆపు; }
            స్థితి 'I': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(int64_t)); ఆపు; }
            స్థితి 'U': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uint64_t)); ఆపు; }
            స్థితి 'e': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uchar) ); ఆపు; }
            స్థితి 'E': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uchar) ); ఆపు; }
            అప్రమేయ: ఆపు;
        }
    }
    size = cvAlign( size, static_cast<పూర్ణాంకం>(elem_max_కొలత) );
    తిరిగి size;
}

size_t base64::base64_encode_బఫరు_కొలత(size_t cnt, తార్కిక is_చివర_with_zero) {
    size_t additional = static_cast<size_t>(is_చివర_with_zero == నిజం);
    తిరిగి (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, తార్కిక can_indent)
        : emitter(కొత్త Base64ContextEmitter(fs, can_indent))
        , దత్తాంశం_type_string()
{
    CV_Assert(fs.write_బహుళకం);
}

శూన్య base64::Base64Writer::లిఖ(స్థిరరాశి శూన్య* దత్తాంశం_3, size_t len, స్థిరరాశి అక్షరం* dt)
{
    check_dt(dt);
    Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor convertor(దత్తాంశం_3, static_cast<పూర్ణాంకం>(len), దత్తాంశం_type_string);
    emitter->లిఖ(convertor);
}

మూస<రకం_పేరు గమ్యం_ద్విమాన_convertor_t> అంతఃవరుస
శూన్య base64::Base64Writer::లిఖ(గమ్యం_ద్విమాన_convertor_t & convertor, స్థిరరాశి అక్షరం* dt)
{
    check_dt(dt);
    emitter->లిఖ(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    తొలగించు emitter;
}

శూన్య base64::Base64Writer::check_dt(స్థిరరాశి అక్షరం* dt)
{
    ఒకవేళ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    లేకపోతే ఒకవేళ (దత్తాంశం_type_string.empty()) {
        దత్తాంశం_type_string = dt;

        /* output header */
        std::string బఫరు = make_base64_శీర్షిక(dt);
        స్థిరరాశి uchar * beg = reinterpret_cast<స్థిరరాశి uchar *>(బఫరు.దత్తాంశం());
        స్థిరరాశి uchar * end = beg + బఫరు.size();

        emitter->లిఖ(beg, end);
    } లేకపోతే ఒకవేళ ( దత్తాంశం_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor(స్థిరరాశి శూన్య* src, పూర్ణాంకం len, స్థిరరాశి std::string & dt)
        : beg(reinterpret_cast<స్థిరరాశి uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    అడుగు_packed = make_గమ్యం_ద్విమాన_funcs(dt);

    end = beg;
    cur = beg;

    అడుగు = icv_Calc_Struct_కొలత(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

అంతఃవరుస  base64::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor&  base64::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor::క్రియాకారకం >>(uchar * & dst)
{
    CV_DbgAssert(*ఇది);

    కొరకు (size_t i = 0U, n = గమ్యం_ద్విమాన_funcs.size(); i < n; i++) {
        elem_గమ్యం_ద్విమాన_t & pack = గమ్యం_ద్విమాన_funcs[i];
        pack.func(cur + pack.offset, dst + pack.స్థానభ్రంశం_packed);
    }
    cur += అడుగు;
    dst += అడుగు_packed;

    తిరిగి *ఇది;
}

అంతఃవరుస  base64::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor::క్రియాకారకం తార్కిక() స్థిరరాశి
{
    తిరిగి cur < end;
}

size_t base64::Raw_దత్తాంశం_గమ్యం_ద్విమాన_Convertor::make_గమ్యం_ద్విమాన_funcs(స్థిరరాశి std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t స్థానభ్రంశం_packed = 0;
    అక్షరం type = '\0';

    std::istringstream iss(dt);
    అయితే (!iss.eof()) {
        ఒకవేళ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ఒకవేళ (!(iss >> type))
            ఆపు;

        అయితే (cnt-- > 0)
        {
            elem_గమ్యం_ద్విమాన_t pack;

            size_t size = 0;
            ఎంపిక (type)
            {
                స్థితి 'u':
                స్థితి 'c':
                    size = పరిమాణం(uchar);
                    pack.func = గమ్యం_ద్విమాన<uchar>;
                    ఆపు;
                స్థితి 'b':
                    size = పరిమాణం(uchar);
                    pack.func = గమ్యం_ద్విమాన<తార్కిక>;
                    ఆపు;
                స్థితి 'w':
                స్థితి 's':
                    size = పరిమాణం(ushort);
                    pack.func = గమ్యం_ద్విమాన<ushort>;
                    ఆపు;
                స్థితి 'n':
                స్థితి 'i':
                    size = పరిమాణం(uint);
                    pack.func = గమ్యం_ద్విమాన<uint>;
                    ఆపు;
                స్థితి 'f':
                    size = పరిమాణం(దశాంశం);
                    pack.func = గమ్యం_ద్విమాన<దశాంశం>;
                    ఆపు;
                స్థితి 'd':
                    size = పరిమాణం(ద్విగుణ);
                    pack.func = గమ్యం_ద్విమాన<ద్విగుణ>;
                    ఆపు;
                స్థితి 'I':
                స్థితి 'U':
                    size = పరిమాణం(uint64_t);
                    pack.func = గమ్యం_ద్విమాన<uint64_t>;
                    ఆపు;
                స్థితి 'e':
                స్థితి 'E':
                    size = పరిమాణం(uchar);
                    pack.func = గమ్యం_ద్విమాన<uchar>;
                    ఆపు;
                స్థితి 'r':
                అప్రమేయ:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<పూర్ణాంకం>(offset), static_cast<పూర్ణాంకం>(size)));
            pack.offset = offset;
            offset += size;

            pack.స్థానభ్రంశం_packed = స్థానభ్రంశం_packed;
            స్థానభ్రంశం_packed += size;

            గమ్యం_ద్విమాన_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    తిరిగి స్థానభ్రంశం_packed;
}

}
