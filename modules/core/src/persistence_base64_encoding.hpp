// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_BASE64_ENCODING_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_BASE64_ENCODING_HPP

ನಾಮಾಕಾಶ cv
{

ನಾಮಾಕಾಶ base64
{
/* A decorator for CvFileStorage
* - no copyable
* - not safe for now
* - move constructor may be needed if C++11
*/
uint8_t ಸ್ಥಿರಾಂಕ base64_mapping[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

uint8_t ಸ್ಥಿರಾಂಕ base64_padding = '=';

std::string make_base64_ಶೀರ್ಷಿಕೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * dt);

size_t base64_encode(uint8_t ಸ್ಥಿರಾಂಕ * src, uint8_t * dst, size_t off, size_t cnt);


ಪೂರ್ಣಾಂಕ icv_Calc_Struct_ಗಾತ್ರ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ );

ವರ್ಗ Base64ContextEmitter;
ವರ್ಗ Impl;

ವರ್ಗ Base64Writer
{
ಸಾರ್ವಜನಿಕ:
    Base64Writer(cv::FileStorage::Impl& fs, ತಾರ್ಕಿಕ can_indent);
    ~Base64Writer();
    ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt);
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t> ಶೂನ್ಯ ಲಿಖ(ಗಮ್ಯ_ದ್ವಿಮಾನ_convertor_t & convertor, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt);

ಖಾಸಗಿ:
    ಶೂನ್ಯ check_dt(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt);

ಖಾಸಗಿ:
    // disable copy and assignment
    Base64Writer(ಸ್ಥಿರಾಂಕ Base64Writer &);
    Base64Writer & ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Base64Writer &);

ಖಾಸಗಿ:

    Base64ContextEmitter * emitter;
    std::string ದತ್ತಾಂಶ_type_string;
};

size_t base64_encode_ಬಫರ್_ಗಾತ್ರ(size_t cnt, ತಾರ್ಕಿಕ is_ಕೊನೆ_with_zero = ನಿಜ);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _uint_t> ಸಾಲೊಳಗೆ size_t
ಗಮ್ಯ_ದ್ವಿಮಾನ(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = ಗಾತ್ರ(_uint_t);
    ಆಗ (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    ಹಿಂತಿರುಗಿ ಗಾತ್ರ(_uint_t);
}

ಮಾದರಿ<> ಸಾಲೊಳಗೆ size_t ಗಮ್ಯ_ದ್ವಿಮಾನ(ತಾರ್ಕಿಕ val, uchar * cur)
{
    ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ದ್ವಿಮಾನ(static_cast<uchar>(val), cur);
}

ಮಾದರಿ<> ಸಾಲೊಳಗೆ size_t ಗಮ್ಯ_ದ್ವಿಮಾನ(ದ್ವಿಗುಣ val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ದ್ವಿಮಾನ(bit64.u, cur);
}

ಮಾದರಿ<> ಸಾಲೊಳಗೆ size_t ಗಮ್ಯ_ದ್ವಿಮಾನ(ದಶಮಾಂಶ val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ದ್ವಿಮಾನ(bit32.u, cur);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _primitive_t> ಸಾಲೊಳಗೆ size_t
ಗಮ್ಯ_ದ್ವಿಮಾನ(uchar ಸ್ಥಿರಾಂಕ * val, uchar * cur)
{
    ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ದ್ವಿಮಾನ<_primitive_t>(*reinterpret_cast<_primitive_t ಸ್ಥಿರಾಂಕ *>(val), cur);
}



ವರ್ಗ Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor
{
ಸಾರ್ವಜನಿಕ:
    // NOTE: len is already multiplied by element size here
    Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ std::string & dt);

    ಸಾಲೊಳಗೆ Raw_ದತ್ತಾಂಶ_ಗಮ್ಯ_ದ್ವಿಮಾನ_Convertor & ಕ್ರಿಯಾಕಾರಕ >>(uchar * & dst);
    ಸಾಲೊಳಗೆ ಕ್ರಿಯಾಕಾರಕ ತಾರ್ಕಿಕ() ಸ್ಥಿರಾಂಕ;

ಖಾಸಗಿ:
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ size_t(*ಗಮ್ಯ_ದ್ವಿಮಾನ_t)(ಸ್ಥಿರಾಂಕ uchar *, uchar *);
    ರಚನೆ elem_ಗಮ್ಯ_ದ್ವಿಮಾನ_t
    {
        size_t      offset;
        size_t      ಸ್ಥಾನಾಂತರ_packed;
        ಗಮ್ಯ_ದ್ವಿಮಾನ_t func;
    };

ಖಾಸಗಿ:
    size_t make_ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs(ಸ್ಥಿರಾಂಕ std::string &dt);

ಖಾಸಗಿ:
    ಸ್ಥಿರಾಂಕ uchar * beg;
    ಸ್ಥಿರಾಂಕ uchar * cur;
    ಸ್ಥಿರಾಂಕ uchar * end;

    size_t ಹೆಜ್ಜೆ;
    size_t ಹೆಜ್ಜೆ_packed;
    std::vector<elem_ಗಮ್ಯ_ದ್ವಿಮಾನ_t> ಗಮ್ಯ_ದ್ವಿಮಾನ_funcs;
};

}

}
#ಆದರೆ_ಅಂತ್ಯ
