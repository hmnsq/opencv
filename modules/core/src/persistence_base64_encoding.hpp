// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_BASE64_ENCODING_HPP
#define OPENCV_CORE_BASE64_ENCODING_HPP

فضاینام cv
{

فضاینام base64
{
/* A decorator for CvFileStorage
* - no copyable
* - not safe for now
* - move constructor may be needed if C++11
*/
uint8_t ثابت base64_mapping[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

uint8_t ثابت base64_padding = '=';

std::string make_base64_header(ثابت نویسه * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحیح icvCalcStructSize( ثابت نویسه* dt, صحیح initial_size );

رده Base64ContextEmitter;
رده Impl;

رده Base64Writer
{
عمومی:
    Base64Writer(cv::FileStorage::Impl& fs, منطقی can_indent);
    ~Base64Writer();
    تهی اكتب(ثابت تهی* _data, size_t len, ثابت نویسه* dt);
    template<typename to_دودویی_convertor_t> تهی اكتب(to_دودویی_convertor_t & convertor, ثابت نویسه* dt);

خصوصی:
    تهی check_dt(ثابت نویسه* dt);

خصوصی:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & operator=(ثابت Base64Writer &);

خصوصی:

    Base64ContextEmitter * emitter;
    std::string data_type_string;
};

size_t base64_encode_buffer_size(size_t cnt, منطقی is_end_with_zero = درست);

template<typename _uint_t> درونخطی size_t
to_دودویی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = اندازه(_uint_t);
    وقتی (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    بازگشت اندازه(_uint_t);
}

template<> درونخطی size_t to_دودویی(منطقی val, uchar * cur)
{
    بازگشت to_دودویی(static_cast<uchar>(val), cur);
}

template<> درونخطی size_t to_دودویی(دوگانه val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    بازگشت to_دودویی(bit64.u, cur);
}

template<> درونخطی size_t to_دودویی(اعشاری val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    بازگشت to_دودویی(bit32.u, cur);
}

template<typename _primitive_t> درونخطی size_t
to_دودویی(uchar ثابت * val, uchar * cur)
{
    بازگشت to_دودویی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



رده Raw_Data_To_دودویی_Convertor
{
عمومی:
    // NOTE: len is already multiplied by element size here
    Raw_Data_To_دودویی_Convertor(ثابت تهی* src, صحیح len, ثابت std::string & dt);

    درونخطی Raw_Data_To_دودویی_Convertor & operator >>(uchar * & dst);
    درونخطی operator منطقی() ثابت;

خصوصی:
    تعریفنوع size_t(*to_دودویی_t)(ثابت uchar *, uchar *);
    ساختار elem_to_دودویی_t
    {
        size_t      offset;
        size_t      offset_packed;
        to_دودویی_t func;
    };

خصوصی:
    size_t make_to_دودویی_funcs(ثابت std::string &dt);

خصوصی:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t step;
    size_t step_packed;
    std::vector<elem_to_دودویی_t> to_دودویی_funcs;
};

}

}
#endif
