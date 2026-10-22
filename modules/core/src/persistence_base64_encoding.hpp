// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_BASE64_ENCODING_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_BASE64_ENCODING_HPP

ਨਾਮਾਕਾਸ਼ cv
{

ਨਾਮਾਕਾਸ਼ base64
{
/* A decorator for CvFileStorage
* - no copyable
* - not safe for now
* - move constructor may be needed if C++11
*/
uint8_t ਅਚਲ base64_mapping[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

uint8_t ਅਚਲ base64_padding = '=';

std::string make_base64_ਸਿਰਲੇਖ(ਅਚਲ ਅੱਖਰ * dt);

size_t base64_encode(uint8_t ਅਚਲ * src, uint8_t * dst, size_t off, size_t cnt);


ਪੂਰਨ icv_Calc_Struct_ਮਾਪ( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ initial_ਮਾਪ );

ਵਰਗ Base64ContextEmitter;
ਵਰਗ Impl;

ਵਰਗ Base64Writer
{
ਜਨਤਕ:
    Base64Writer(cv::FileStorage::Impl& fs, ਤਾਰਕਿਕ can_indent);
    ~Base64Writer();
    ਖਾਲੀ ਲਿਖ(ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ_3, size_t len, ਅਚਲ ਅੱਖਰ* dt);
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t> ਖਾਲੀ ਲਿਖ(ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_convertor_t & convertor, ਅਚਲ ਅੱਖਰ* dt);

ਨਿਜੀ:
    ਖਾਲੀ check_dt(ਅਚਲ ਅੱਖਰ* dt);

ਨਿਜੀ:
    // disable copy and assignment
    Base64Writer(ਅਚਲ Base64Writer &);
    Base64Writer & ਸੰਚਾਲਕ=(ਅਚਲ Base64Writer &);

ਨਿਜੀ:

    Base64ContextEmitter * emitter;
    std::string ਅੰਕੜੇ_type_string;
};

size_t base64_encode_ਬਫਰ_ਮਾਪ(size_t cnt, ਤਾਰਕਿਕ is_ਸਿਰਾ_with_zero = ਸੱਚ);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _uint_t> ਸਤਰਅੰਦਰ size_t
ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = ਆਕਾਰ(_uint_t);
    ਜਦੋਂ (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    ਵਾਪਸ ਆਕਾਰ(_uint_t);
}

ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ size_t ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(ਤਾਰਕਿਕ val, uchar * cur)
{
    ਵਾਪਸ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(static_cast<uchar>(val), cur);
}

ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ size_t ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(ਦੁੱਗਣਾ val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    ਵਾਪਸ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(bit64.u, cur);
}

ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ size_t ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(ਦਸ਼ਮਲਵ val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    ਵਾਪਸ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(bit32.u, cur);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _primitive_t> ਸਤਰਅੰਦਰ size_t
ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ(uchar ਅਚਲ * val, uchar * cur)
{
    ਵਾਪਸ ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ<_primitive_t>(*reinterpret_cast<_primitive_t ਅਚਲ *>(val), cur);
}



ਵਰਗ Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor
{
ਜਨਤਕ:
    // NOTE: len is already multiplied by element size here
    Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor(ਅਚਲ ਖਾਲੀ* src, ਪੂਰਨ len, ਅਚਲ std::string & dt);

    ਸਤਰਅੰਦਰ Raw_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_Convertor & ਸੰਚਾਲਕ >>(uchar * & dst);
    ਸਤਰਅੰਦਰ ਸੰਚਾਲਕ ਤਾਰਕਿਕ() ਅਚਲ;

ਨਿਜੀ:
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ size_t(*ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t)(ਅਚਲ uchar *, uchar *);
    ਬਣਤਰ elem_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t
    {
        size_t      offset;
        size_t      ਵਿੱਥ_packed;
        ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t func;
    };

ਨਿਜੀ:
    size_t make_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs(ਅਚਲ std::string &dt);

ਨਿਜੀ:
    ਅਚਲ uchar * beg;
    ਅਚਲ uchar * cur;
    ਅਚਲ uchar * end;

    size_t ਕਦਮ;
    size_t ਕਦਮ_packed;
    std::vector<elem_ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_t> ਮੰਜ਼ਿਲ_ਦੁਆਧਾਰੀ_funcs;
};

}

}
#ਜੇ_ਅੰਤ
