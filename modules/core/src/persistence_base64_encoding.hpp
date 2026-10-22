// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#اگر_تعریف_نشده OPENCV_CORE_BASE64_ENCODING_HPP
#تعریف_کن OPENCV_CORE_BASE64_ENCODING_HPP

نطاق cv
{

نطاق base64
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

std::string make_base64_سرآیند(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحيح icv_Calc_Struct_اندازه( ثابت حرف* dt, صحيح initial_اندازه );

صنف Base64ContextEmitter;
صنف Impl;

صنف Base64Writer
{
عام:
    Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent);
    ~Base64Writer();
    فراغ اكتب(ثابت فراغ* داده_3, size_t len, ثابت حرف* dt);
    قالب<نام_نوع مقصد_دودویی_convertor_t> فراغ اكتب(مقصد_دودویی_convertor_t & convertor, ثابت حرف* dt);

خاص:
    فراغ check_dt(ثابت حرف* dt);

خاص:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & عملگر=(ثابت Base64Writer &);

خاص:

    Base64ContextEmitter * emitter;
    std::string داده_type_string;
};

size_t base64_encode_میانگیر_اندازه(size_t cnt, منطقي is_انتها_with_zero = درست);

قالب<نام_نوع _uint_t> مضمن size_t
مقصد_دودویی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    طالما (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    أرجع حجم(_uint_t);
}

قالب<> مضمن size_t مقصد_دودویی(منطقي val, uchar * cur)
{
    أرجع مقصد_دودویی(static_cast<uchar>(val), cur);
}

قالب<> مضمن size_t مقصد_دودویی(مزدوج val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    أرجع مقصد_دودویی(bit64.u, cur);
}

قالب<> مضمن size_t مقصد_دودویی(عائم val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    أرجع مقصد_دودویی(bit32.u, cur);
}

قالب<نام_نوع _primitive_t> مضمن size_t
مقصد_دودویی(uchar ثابت * val, uchar * cur)
{
    أرجع مقصد_دودویی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



صنف Raw_داده_مقصد_دودویی_Convertor
{
عام:
    // NOTE: len is already multiplied by element size here
    Raw_داده_مقصد_دودویی_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt);

    مضمن Raw_داده_مقصد_دودویی_Convertor & عملگر >>(uchar * & dst);
    مضمن عملگر منطقي() ثابت;

خاص:
    تعريفنوع size_t(*مقصد_دودویی_t)(ثابت uchar *, uchar *);
    بنية elem_مقصد_دودویی_t
    {
        size_t      offset;
        size_t      جابه_جایی_packed;
        مقصد_دودویی_t func;
    };

خاص:
    size_t make_مقصد_دودویی_funcs(ثابت std::string &dt);

خاص:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t گام;
    size_t گام_packed;
    std::vector<elem_مقصد_دودویی_t> مقصد_دودویی_funcs;
};

}

}
#پایان_اگر
