// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_BASE64_ENCODING_HPP
#وصف_ڪريو OPENCV_CORE_BASE64_ENCODING_HPP

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

std::string make_base64_سرو(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحيح icv_Calc_Struct_سائيز( ثابت حرف* dt, صحيح initial_سائيز );

صنف Base64ContextEmitter;
صنف Impl;

صنف Base64Writer
{
عام:
    Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent);
    ~Base64Writer();
    فراغ اكتب(ثابت فراغ* معلومات_3, size_t len, ثابت حرف* dt);
    سانچو<قسم_نالو منزل_ٻٽو_convertor_t> فراغ اكتب(منزل_ٻٽو_convertor_t & convertor, ثابت حرف* dt);

خاص:
    فراغ check_dt(ثابت حرف* dt);

خاص:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & عامل=(ثابت Base64Writer &);

خاص:

    Base64ContextEmitter * emitter;
    std::string معلومات_type_string;
};

size_t base64_encode_بفر_سائيز(size_t cnt, منطقي is_پڇاڙي_with_zero = سچ);

سانچو<قسم_نالو _uint_t> مضمن size_t
منزل_ٻٽو(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    طالما (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    أرجع حجم(_uint_t);
}

سانچو<> مضمن size_t منزل_ٻٽو(منطقي val, uchar * cur)
{
    أرجع منزل_ٻٽو(static_cast<uchar>(val), cur);
}

سانچو<> مضمن size_t منزل_ٻٽو(مزدوج val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    أرجع منزل_ٻٽو(bit64.u, cur);
}

سانچو<> مضمن size_t منزل_ٻٽو(عائم val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    أرجع منزل_ٻٽو(bit32.u, cur);
}

سانچو<قسم_نالو _primitive_t> مضمن size_t
منزل_ٻٽو(uchar ثابت * val, uchar * cur)
{
    أرجع منزل_ٻٽو<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



صنف Raw_معلومات_منزل_ٻٽو_Convertor
{
عام:
    // NOTE: len is already multiplied by element size here
    Raw_معلومات_منزل_ٻٽو_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt);

    مضمن Raw_معلومات_منزل_ٻٽو_Convertor & عامل >>(uchar * & dst);
    مضمن عامل منطقي() ثابت;

خاص:
    تعريفنوع size_t(*منزل_ٻٽو_t)(ثابت uchar *, uchar *);
    بنية elem_منزل_ٻٽو_t
    {
        size_t      offset;
        size_t      وٿي_packed;
        منزل_ٻٽو_t func;
    };

خاص:
    size_t make_منزل_ٻٽو_funcs(ثابت std::string &dt);

خاص:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t قدم;
    size_t قدم_packed;
    std::vector<elem_منزل_ٻٽو_t> منزل_ٻٽو_funcs;
};

}

}
#جيڪڏهن_پورو
