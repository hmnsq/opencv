// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#جے_بنا_تعریف OPENCV_CORE_BASE64_ENCODING_HPP
#تعریف_کرو OPENCV_CORE_BASE64_ENCODING_HPP

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

std::string make_base64_سرخی(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحيح icv_Calc_Struct_ناپ( ثابت حرف* dt, صحيح initial_ناپ );

صنف Base64ContextEmitter;
صنف Impl;

صنف Base64Writer
{
عام:
    Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent);
    ~Base64Writer();
    فراغ اكتب(ثابت فراغ* انکڑے_3, size_t len, ثابت حرف* dt);
    سانچا<قسم_ناں منزل_ثنائی_convertor_t> فراغ اكتب(منزل_ثنائی_convertor_t & convertor, ثابت حرف* dt);

خاص:
    فراغ check_dt(ثابت حرف* dt);

خاص:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & عامل=(ثابت Base64Writer &);

خاص:

    Base64ContextEmitter * emitter;
    std::string انکڑے_type_string;
};

size_t base64_encode_بفر_ناپ(size_t cnt, منطقي is_سرا_with_zero = سچ);

سانچا<قسم_ناں _uint_t> مضمن size_t
منزل_ثنائی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    طالما (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    أرجع حجم(_uint_t);
}

سانچا<> مضمن size_t منزل_ثنائی(منطقي val, uchar * cur)
{
    أرجع منزل_ثنائی(static_cast<uchar>(val), cur);
}

سانچا<> مضمن size_t منزل_ثنائی(مزدوج val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    أرجع منزل_ثنائی(bit64.u, cur);
}

سانچا<> مضمن size_t منزل_ثنائی(عائم val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    أرجع منزل_ثنائی(bit32.u, cur);
}

سانچا<قسم_ناں _primitive_t> مضمن size_t
منزل_ثنائی(uchar ثابت * val, uchar * cur)
{
    أرجع منزل_ثنائی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



صنف Raw_انکڑے_منزل_ثنائی_Convertor
{
عام:
    // NOTE: len is already multiplied by element size here
    Raw_انکڑے_منزل_ثنائی_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt);

    مضمن Raw_انکڑے_منزل_ثنائی_Convertor & عامل >>(uchar * & dst);
    مضمن عامل منطقي() ثابت;

خاص:
    تعريفنوع size_t(*منزل_ثنائی_t)(ثابت uchar *, uchar *);
    بنية elem_منزل_ثنائی_t
    {
        size_t      offset;
        size_t      وتھ_packed;
        منزل_ثنائی_t func;
    };

خاص:
    size_t make_منزل_ثنائی_funcs(ثابت std::string &dt);

خاص:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t قدم;
    size_t قدم_packed;
    std::vector<elem_منزل_ثنائی_t> منزل_ثنائی_funcs;
};

}

}
#جے_انت
