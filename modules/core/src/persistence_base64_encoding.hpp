// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#که_نه_تعریف_شوی OPENCV_CORE_BASE64_ENCODING_HPP
#تعریف_کړه OPENCV_CORE_BASE64_ENCODING_HPP

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

std::string make_base64_سرلیک(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحيح icv_Calc_Struct_اندازه( ثابت حرف* dt, صحيح initial_اندازه );

صنف Base64ContextEmitter;
صنف Impl;

صنف Base64Writer
{
عام:
    Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent);
    ~Base64Writer();
    فراغ اكتب(ثابت فراغ* معلومات_3, size_t len, ثابت حرف* dt);
    کينډۍ<ډول_نوم منزل_دوګونی_convertor_t> فراغ اكتب(منزل_دوګونی_convertor_t & convertor, ثابت حرف* dt);

خاص:
    فراغ check_dt(ثابت حرف* dt);

خاص:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & چلوونکی=(ثابت Base64Writer &);

خاص:

    Base64ContextEmitter * emitter;
    std::string معلومات_type_string;
};

size_t base64_encode_بفر_اندازه(size_t cnt, منطقي is_پای_ټکی_with_zero = ریښتیا);

کينډۍ<ډول_نوم _uint_t> مضمن size_t
منزل_دوګونی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    طالما (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    أرجع حجم(_uint_t);
}

کينډۍ<> مضمن size_t منزل_دوګونی(منطقي val, uchar * cur)
{
    أرجع منزل_دوګونی(static_cast<uchar>(val), cur);
}

کينډۍ<> مضمن size_t منزل_دوګونی(مزدوج val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    أرجع منزل_دوګونی(bit64.u, cur);
}

کينډۍ<> مضمن size_t منزل_دوګونی(عائم val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    أرجع منزل_دوګونی(bit32.u, cur);
}

کينډۍ<ډول_نوم _primitive_t> مضمن size_t
منزل_دوګونی(uchar ثابت * val, uchar * cur)
{
    أرجع منزل_دوګونی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



صنف Raw_معلومات_منزل_دوګونی_Convertor
{
عام:
    // NOTE: len is already multiplied by element size here
    Raw_معلومات_منزل_دوګونی_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt);

    مضمن Raw_معلومات_منزل_دوګونی_Convertor & چلوونکی >>(uchar * & dst);
    مضمن چلوونکی منطقي() ثابت;

خاص:
    تعريفنوع size_t(*منزل_دوګونی_t)(ثابت uchar *, uchar *);
    بنية elem_منزل_دوګونی_t
    {
        size_t      offset;
        size_t      دننه_واټن_packed;
        منزل_دوګونی_t func;
    };

خاص:
    size_t make_منزل_دوګونی_funcs(ثابت std::string &dt);

خاص:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t ګام;
    size_t ګام_packed;
    std::vector<elem_منزل_دوګونی_t> منزل_دوګونی_funcs;
};

}

}
#د_که_پای
