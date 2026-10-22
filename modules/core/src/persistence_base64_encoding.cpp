// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence_impl.hpp"
#include "persistence_base64_encoding.hpp"

namespace cv
{

class base64::Base64ContextEmitter
{
public:
    explicit Base64ContextEmitter(cv::FileStorage::Impl& fs, bool needs_indent_)
            : file_storage(fs)
            , needs_indent(needs_indent_)
            , binary_buffer(BUFFER_LEN)
            , base64_buffer(base64_encode_buffer_size(BUFFER_LEN))
            , src_beg(0)
            , src_cur(0)
            , src_end(0)
    {
        src_beg = binary_buffer.data();
        src_end = src_beg + BUFFER_LEN;
        src_cur = src_beg;

        CV_Assert(fs.write_mode);

        ꦪꦼꦤ꧀ (needs_indent)
        {
            file_storage.flush();
        }
    }

    ~Base64ContextEmitter()
    {
        /* cleaning */
        ꦪꦼꦤ꧀ (src_cur != src_beg)
            flush();    /* encode the rest binary data to base64 buffer */
    }

    Base64ContextEmitter & write(const uchar * beg, const uchar * end)
    {
        ꦪꦼꦤ꧀ (beg >= end)
            ꦮꦭꦶ *this;

        ꦱꦼꦢꦼꦭ (beg < end) {
            /* collect binary data and copy to binary buffer */
            size_t len = std::min(end - beg, src_end - src_cur);
            std::memcpy(src_cur, beg, len);
            beg     += len;
            src_cur += len;

            ꦪꦼꦤ꧀ (src_cur >= src_end) {
                /* binary buffer is full. */
                /* encode it to base64 and send result to fs */
                flush();
            }
        }

        ꦮꦭꦶ *this;
    }

    /*
     * a convertor must provide :
     * - `operator >> (uchar * & dst)` for writing current binary data to `dst` and moving to next data.
     * - `operator bool` for checking if current location is valid and not the end.
     */
    template<typename _to_binary_convertor_t> anta_hstha
    Base64ContextEmitter & write(_to_binary_convertor_t & convertor)
    {
        constexpr size_t BUFFER_MAX_LEN = 1024U;

        uchar buffer[BUFFER_MAX_LEN];
        uchar * beg = buffer;
        uchar * end = beg;

        ꦱꦼꦢꦼꦭ (convertor) {
            convertor >> end;
            write(beg, end);
            end = beg;
        }

        ꦮꦭꦶ *this;
    }

    bool flush()
    {
        /* control line width, so on. */
        size_t len = base64_encode(src_beg, base64_buffer.data(), 0U, src_cur - src_beg);
        ꦪꦼꦤ꧀ (len == 0U)
            ꦮꦭꦶ false;

        src_cur = src_beg;

        ꦪꦼꦤ꧀ ( !needs_indent)
        {
            file_storage.puts((const var_na*)base64_buffer.data());
        }
        ꦭꦶꦪ
        {
            const var_na newline[] = "\n";
            var_na space[80];
            puur_naa_nka ident = file_storage.write_stack.back().indent;
            memset(space, ' ', static_cast<puur_naa_nka>(ident));
            space[ident] = '\0';

            file_storage.puts(space);
            file_storage.puts((const var_na*)base64_buffer.data());
            file_storage.puts(newline);
            file_storage.flush();
        }

        ꦮꦭꦶ true;
    }

private:
    /* because of Base64, we must keep its length a multiple of 3 */
    static const size_t BUFFER_LEN = 48U;
    // static_assert(BUFFER_LEN % 3 == 0, "BUFFER_LEN is invalid");

private:
    cv::FileStorage::Impl& file_storage;
    bool needs_indent;

    std::vector<uchar> binary_buffer;
    std::vector<uchar> base64_buffer;
    uchar * src_beg;
    uchar * src_cur;
    uchar * src_end;
};

std::string base64::make_base64_header(const var_na *dt) {
    std::ostringstream oss;
    oss << dt   << ' ';
    std::string buffer(oss.str());
    CV_Assert(buffer.size() < ::base64::HEADER_SIZE);

    buffer.reserve(::base64::HEADER_SIZE);
    ꦱꦼꦢꦼꦭ (buffer.size() < ::base64::HEADER_SIZE)
        buffer += ' ';

    ꦮꦭꦶ buffer;
}

size_t base64::base64_encode(const uint8_t *src, uint8_t *dst, size_t off, size_t cnt) {
    ꦪꦼꦤ꧀ (!src || !dst || !cnt)
        ꦮꦭꦶ 0;

    /* initialize beginning and end */
    uint8_t       * dst_beg = dst;
    uint8_t       * dst_cur = dst_beg;

    uint8_t const * src_beg = src + off;
    uint8_t const * src_cur = src_beg;
    uint8_t const * src_end = src_cur + cnt / 3U * 3U;

    /* integer multiples part */
    ꦱꦼꦢꦼꦭ (src_cur < src_end) {
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
    ꦪꦼꦤ꧀ (rst == 1U) {
        uint8_t _2 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U];
    } ꦭꦶꦪ ꦪꦼꦤ꧀ (rst == 2U) {
        uint8_t _2 = *src_cur++;
        uint8_t _1 = *src_cur++;
        *dst_cur++ = base64_mapping[ _2          >> 2U];
        *dst_cur++ = base64_mapping[(_2 & 0x03U) << 4U | (_1 & 0xF0U) >> 4U];
        *dst_cur++ = base64_mapping[(_1 & 0x0FU) << 2U];
    }

    /* padding */
    switch (rst)
    {
        pak_sa 1U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        pak_sa 2U: *dst_cur++ = base64_padding;
            [[fallthrough]];
        default: *dst_cur   = 0;
            bha_nga;
    }

    ꦮꦭꦶ static_cast<size_t>(dst_cur - dst_beg);
}

puur_naa_nka base64::icvCalcStructSize(const var_na *dt, puur_naa_nka initial_size) {
    puur_naa_nka size = cv::fs::calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    ꦏꦁꦒꦺꦴ ( const var_na * type = dt; *type != '\0'; type++ ) {
        switch ( *type )
        {
            pak_sa 'u': { elem_max_size = std::max( elem_max_size, parimaa_na(uchar ) ); bha_nga; }
            pak_sa 'b': { elem_max_size = std::max( elem_max_size, parimaa_na(uchar ) ); bha_nga; }
            pak_sa 'c': { elem_max_size = std::max( elem_max_size, parimaa_na(schar ) ); bha_nga; }
            pak_sa 'w': { elem_max_size = std::max( elem_max_size, parimaa_na(ushort) ); bha_nga; }
            pak_sa 's': { elem_max_size = std::max( elem_max_size, parimaa_na(short ) ); bha_nga; }
            pak_sa 'i': { elem_max_size = std::max( elem_max_size, parimaa_na(puur_naa_nka   ) ); bha_nga; }
            pak_sa 'n': { elem_max_size = std::max( elem_max_size, parimaa_na(unsigned) ); bha_nga; }
            pak_sa 'f': { elem_max_size = std::max( elem_max_size, parimaa_na(float ) ); bha_nga; }
            pak_sa 'd': { elem_max_size = std::max( elem_max_size, parimaa_na(dviguu_na) ); bha_nga; }
            pak_sa 'I': { elem_max_size = std::max( elem_max_size, parimaa_na(int64_t)); bha_nga; }
            pak_sa 'U': { elem_max_size = std::max( elem_max_size, parimaa_na(uint64_t)); bha_nga; }
            pak_sa 'e': { elem_max_size = std::max( elem_max_size, parimaa_na(uchar) ); bha_nga; }
            pak_sa 'E': { elem_max_size = std::max( elem_max_size, parimaa_na(uchar) ); bha_nga; }
            default: bha_nga;
        }
    }
    size = cvAlign( size, static_cast<puur_naa_nka>(elem_max_size) );
    ꦮꦭꦶ size;
}

size_t base64::base64_encode_buffer_size(size_t cnt, bool is_end_with_zero) {
    size_t additional = static_cast<size_t>(is_end_with_zero == true);
    ꦮꦭꦶ (cnt + 2U) / 3U * 4U + additional;
}

base64::Base64Writer::Base64Writer(cv::FileStorage::Impl& fs, bool can_indent)
        : emitter(new Base64ContextEmitter(fs, can_indent))
        , data_type_string()
{
    CV_Assert(fs.write_mode);
}

void base64::Base64Writer::write(const void* _data, size_t len, const var_na* dt)
{
    check_dt(dt);
    RawDataToBinaryConvertor convertor(_data, static_cast<puur_naa_nka>(len), data_type_string);
    emitter->write(convertor);
}

template<typename _to_binary_convertor_t> anta_hstha
void base64::Base64Writer::write(_to_binary_convertor_t & convertor, const var_na* dt)
{
    check_dt(dt);
    emitter->write(convertor);
}

base64::Base64Writer::~Base64Writer()
{
    delete emitter;
}

void base64::Base64Writer::check_dt(const var_na* dt)
{
    ꦪꦼꦤ꧀ ( dt == 0 )
        CV_Error( cv::Error::StsBadArg, "Invalid \'dt\'." );
    ꦭꦶꦪ ꦪꦼꦤ꧀ (data_type_string.empty()) {
        data_type_string = dt;

        /* output header */
        std::string buffer = make_base64_header(dt);
        const uchar * beg = reinterpret_cast<const uchar *>(buffer.data());
        const uchar * end = beg + buffer.size();

        emitter->write(beg, end);
    } ꦭꦶꦪ ꦪꦼꦤ꧀ ( data_type_string != dt )
        CV_Error( cv::Error::StsBadArg, "\'dt\' does not match." );
}

base64::RawDataToBinaryConvertor::RawDataToBinaryConvertor(const void* src, puur_naa_nka len, const std::string & dt)
        : beg(reinterpret_cast<const uchar *>(src))
        , cur(0)
        , end(0)
{
    CV_Assert(src);
    CV_Assert(!dt.empty());
    CV_Assert(len > 0);

    /* calc step and to_binary_funcs */
    step_packed = make_to_binary_funcs(dt);

    end = beg;
    cur = beg;

    step = icvCalcStructSize(dt.c_str(), 0);
    end = beg + static_cast<size_t>(len);
}

anta_hstha  base64::RawDataToBinaryConvertor&  base64::RawDataToBinaryConvertor::operator >>(uchar * & dst)
{
    CV_DbgAssert(*this);

    ꦏꦁꦒꦺꦴ (size_t i = 0U, n = to_binary_funcs.size(); i < n; i++) {
        elem_to_binary_t & pack = to_binary_funcs[i];
        pack.func(cur + pack.offset, dst + pack.offset_packed);
    }
    cur += step;
    dst += step_packed;

    ꦮꦭꦶ *this;
}

anta_hstha  base64::RawDataToBinaryConvertor::operator bool() const
{
    ꦮꦭꦶ cur < end;
}

size_t base64::RawDataToBinaryConvertor::make_to_binary_funcs(const std::string &dt)
{
    size_t cnt = 0;
    size_t offset = 0;
    size_t offset_packed = 0;
    var_na type = '\0';

    std::istringstream iss(dt);
    ꦱꦼꦢꦼꦭ (!iss.eof()) {
        ꦪꦼꦤ꧀ (!(iss >> cnt)) {
            iss.clear();
            cnt = 1;
        }
        CV_Assert(cnt > 0U);
        ꦪꦼꦤ꧀ (!(iss >> type))
            bha_nga;

        ꦱꦼꦢꦼꦭ (cnt-- > 0)
        {
            elem_to_binary_t pack;

            size_t size = 0;
            switch (type)
            {
                pak_sa 'u':
                pak_sa 'c':
                    size = parimaa_na(uchar);
                    pack.func = to_binary<uchar>;
                    bha_nga;
                pak_sa 'b':
                    size = parimaa_na(uchar);
                    pack.func = to_binary<bool>;
                    bha_nga;
                pak_sa 'w':
                pak_sa 's':
                    size = parimaa_na(ushort);
                    pack.func = to_binary<ushort>;
                    bha_nga;
                pak_sa 'n':
                pak_sa 'i':
                    size = parimaa_na(uint);
                    pack.func = to_binary<uint>;
                    bha_nga;
                pak_sa 'f':
                    size = parimaa_na(float);
                    pack.func = to_binary<float>;
                    bha_nga;
                pak_sa 'd':
                    size = parimaa_na(dviguu_na);
                    pack.func = to_binary<dviguu_na>;
                    bha_nga;
                pak_sa 'I':
                pak_sa 'U':
                    size = parimaa_na(uint64_t);
                    pack.func = to_binary<uint64_t>;
                    bha_nga;
                pak_sa 'e':
                pak_sa 'E':
                    size = parimaa_na(uchar);
                    pack.func = to_binary<uchar>;
                    bha_nga;
                pak_sa 'r':
                default:
                    CV_Error(cv::Error::StsError, "type is not supported");
            };

            offset = static_cast<size_t>(cvAlign(static_cast<puur_naa_nka>(offset), static_cast<puur_naa_nka>(size)));
            pack.offset = offset;
            offset += size;

            pack.offset_packed = offset_packed;
            offset_packed += size;

            to_binary_funcs.push_back(pack);
        }
    }

    CV_Assert(iss.eof());
    ꦮꦭꦶ offset_packed;
}

}
