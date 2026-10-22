// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#اگر_غیر_تعریف_شدہ OPENCV_CORE_BASE64_ENCODING_HPP
#تعریف_کرو OPENCV_CORE_BASE64_ENCODING_HPP

نامخلا cv
{

نامخلا base64
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

std::string make_base64_سرخی(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحیح icv_Calc_Struct_سائز( ثابت حرف* dt, صحیح initial_سائز );

جماعت Base64ContextEmitter;
جماعت Impl;

جماعت Base64Writer
{
عوامی:
    Base64Writer(cv::FileStorage::Impl& fs, منطقی can_indent);
    ~Base64Writer();
    خالی اكتب(ثابت خالی* ڈیٹا_3, size_t len, ثابت حرف* dt);
    سانچہ<قسم_نام مقصد_ثنائی_convertor_t> خالی اكتب(مقصد_ثنائی_convertor_t & convertor, ثابت حرف* dt);

نجی:
    خالی check_dt(ثابت حرف* dt);

نجی:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & عامل=(ثابت Base64Writer &);

نجی:

    Base64ContextEmitter * emitter;
    std::string ڈیٹا_type_string;
};

size_t base64_encode_بفر_سائز(size_t cnt, منطقی is_انتہا_with_zero = سچ);

سانچہ<قسم_نام _uint_t> اندرونی size_t
مقصد_ثنائی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    جب (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    واپس حجم(_uint_t);
}

سانچہ<> اندرونی size_t مقصد_ثنائی(منطقی val, uchar * cur)
{
    واپس مقصد_ثنائی(static_cast<uchar>(val), cur);
}

سانچہ<> اندرونی size_t مقصد_ثنائی(دوہرا val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    واپس مقصد_ثنائی(bit64.u, cur);
}

سانچہ<> اندرونی size_t مقصد_ثنائی(اعشاری val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    واپس مقصد_ثنائی(bit32.u, cur);
}

سانچہ<قسم_نام _primitive_t> اندرونی size_t
مقصد_ثنائی(uchar ثابت * val, uchar * cur)
{
    واپس مقصد_ثنائی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



جماعت Raw_ڈیٹا_مقصد_ثنائی_Convertor
{
عوامی:
    // NOTE: len is already multiplied by element size here
    Raw_ڈیٹا_مقصد_ثنائی_Convertor(ثابت خالی* src, صحیح len, ثابت std::string & dt);

    اندرونی Raw_ڈیٹا_مقصد_ثنائی_Convertor & عامل >>(uchar * & dst);
    اندرونی عامل منطقی() ثابت;

نجی:
    قسمتعریف size_t(*مقصد_ثنائی_t)(ثابت uchar *, uchar *);
    ساخت elem_مقصد_ثنائی_t
    {
        size_t      offset;
        size_t      انحراف_packed;
        مقصد_ثنائی_t func;
    };

نجی:
    size_t make_مقصد_ثنائی_funcs(ثابت std::string &dt);

نجی:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t قدم;
    size_t قدم_packed;
    std::vector<elem_مقصد_ثنائی_t> مقصد_ثنائی_funcs;
};

}

}
#اگر_ختم
