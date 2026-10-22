// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_CORE_BASE64_ENCODING_HPP
#define OPENCV_CORE_BASE64_ENCODING_HPP

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

std::string make_base64_header(ثابت حرف * dt);

size_t base64_encode(uint8_t ثابت * src, uint8_t * dst, size_t off, size_t cnt);


صحيح icvCalcStructSize( ثابت حرف* dt, صحيح initial_size );

صنف Base64ContextEmitter;
صنف Impl;

صنف Base64Writer
{
عام:
    Base64Writer(cv::FileStorage::Impl& fs, منطقي can_indent);
    ~Base64Writer();
    فراغ اكتب(ثابت فراغ* _data, size_t len, ثابت حرف* dt);
    template<typename to_ثنائی_convertor_t> فراغ اكتب(to_ثنائی_convertor_t & convertor, ثابت حرف* dt);

خاص:
    فراغ check_dt(ثابت حرف* dt);

خاص:
    // disable copy and assignment
    Base64Writer(ثابت Base64Writer &);
    Base64Writer & operator=(ثابت Base64Writer &);

خاص:

    Base64ContextEmitter * emitter;
    std::string data_type_string;
};

size_t base64_encode_buffer_size(size_t cnt, منطقي is_end_with_zero = true);

template<typename _uint_t> مضمن size_t
to_ثنائی(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = حجم(_uint_t);
    طالما (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    أرجع حجم(_uint_t);
}

template<> مضمن size_t to_ثنائی(منطقي val, uchar * cur)
{
    أرجع to_ثنائی(static_cast<uchar>(val), cur);
}

template<> مضمن size_t to_ثنائی(مزدوج val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    أرجع to_ثنائی(bit64.u, cur);
}

template<> مضمن size_t to_ثنائی(عائم val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    أرجع to_ثنائی(bit32.u, cur);
}

template<typename _primitive_t> مضمن size_t
to_ثنائی(uchar ثابت * val, uchar * cur)
{
    أرجع to_ثنائی<_primitive_t>(*reinterpret_cast<_primitive_t ثابت *>(val), cur);
}



صنف Raw_Data_To_ثنائی_Convertor
{
عام:
    // NOTE: len is already multiplied by element size here
    Raw_Data_To_ثنائی_Convertor(ثابت فراغ* src, صحيح len, ثابت std::string & dt);

    مضمن Raw_Data_To_ثنائی_Convertor & operator >>(uchar * & dst);
    مضمن operator منطقي() ثابت;

خاص:
    تعريفنوع size_t(*to_ثنائی_t)(ثابت uchar *, uchar *);
    بنية elem_to_ثنائی_t
    {
        size_t      offset;
        size_t      offset_packed;
        to_ثنائی_t func;
    };

خاص:
    size_t make_to_ثنائی_funcs(ثابت std::string &dt);

خاص:
    ثابت uchar * beg;
    ثابت uchar * cur;
    ثابت uchar * end;

    size_t step;
    size_t step_packed;
    std::vector<elem_to_ثنائی_t> to_ثنائی_funcs;
};

}

}
#endif
