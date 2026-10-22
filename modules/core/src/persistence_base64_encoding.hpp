// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#यदि_अपरिभाषितम् OPENCV_CORE_BASE64_ENCODING_HPP
#परिभाषय OPENCV_CORE_BASE64_ENCODING_HPP

नामाकाश cv
{

नामाकाश base64
{
/* A decorator for CvFileStorage
* - no copyable
* - not safe for now
* - move constructor may be needed if C++11
*/
uint8_t ध्रुव base64_mapping[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

uint8_t ध्रुव base64_padding = '=';

std::string make_base64_शीर्षकम्(ध्रुव वर्ण * dt);

size_t base64_encode(uint8_t ध्रुव * src, uint8_t * dst, size_t off, size_t cnt);


पूर्णाङ्क icv_Calc_Struct_आकार( ध्रुव वर्ण* dt, पूर्णाङ्क initial_आकार );

वर्ग Base64ContextEmitter;
वर्ग Impl;

वर्ग Base64Writer
{
सार्वजनिक:
    Base64Writer(cv::FileStorage::Impl& fs, तार्किक can_indent);
    ~Base64Writer();
    शून्य लिख(ध्रुव शून्य* दत्त_3, size_t len, ध्रुव वर्ण* dt);
    प्रतिरूपम्<प्रकारनाम गन्तव्यम्_द्विमान_convertor_t> शून्य लिख(गन्तव्यम्_द्विमान_convertor_t & convertor, ध्रुव वर्ण* dt);

गुप्त:
    शून्य check_dt(ध्रुव वर्ण* dt);

गुप्त:
    // disable copy and assignment
    Base64Writer(ध्रुव Base64Writer &);
    Base64Writer & संक्रियकः=(ध्रुव Base64Writer &);

गुप्त:

    Base64ContextEmitter * emitter;
    std::string दत्त_type_string;
};

size_t base64_encode_बफर_आकार(size_t cnt, तार्किक is_अन्तः_with_zero = सत्य);

प्रतिरूपम्<प्रकारनाम _uint_t> अन्तःस्थ size_t
गन्तव्यम्_द्विमान(_uint_t val, uchar * cur)
{
    size_t delta = CHAR_BIT;
    size_t cnt = परिमाण(_uint_t);
    यावत् (cnt --> static_cast<size_t>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    प्रत्यागम परिमाण(_uint_t);
}

प्रतिरूपम्<> अन्तःस्थ size_t गन्तव्यम्_द्विमान(तार्किक val, uchar * cur)
{
    प्रत्यागम गन्तव्यम्_द्विमान(static_cast<uchar>(val), cur);
}

प्रतिरूपम्<> अन्तःस्थ size_t गन्तव्यम्_द्विमान(द्विगुण val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    प्रत्यागम गन्तव्यम्_द्विमान(bit64.u, cur);
}

प्रतिरूपम्<> अन्तःस्थ size_t गन्तव्यम्_द्विमान(भिन्न val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    प्रत्यागम गन्तव्यम्_द्विमान(bit32.u, cur);
}

प्रतिरूपम्<प्रकारनाम _primitive_t> अन्तःस्थ size_t
गन्तव्यम्_द्विमान(uchar ध्रुव * val, uchar * cur)
{
    प्रत्यागम गन्तव्यम्_द्विमान<_primitive_t>(*reinterpret_cast<_primitive_t ध्रुव *>(val), cur);
}



वर्ग Raw_दत्त_गन्तव्यम्_द्विमान_Convertor
{
सार्वजनिक:
    // NOTE: len is already multiplied by element size here
    Raw_दत्त_गन्तव्यम्_द्विमान_Convertor(ध्रुव शून्य* src, पूर्णाङ्क len, ध्रुव std::string & dt);

    अन्तःस्थ Raw_दत्त_गन्तव्यम्_द्विमान_Convertor & संक्रियकः >>(uchar * & dst);
    अन्तःस्थ संक्रियकः तार्किक() ध्रुव;

गुप्त:
    प्रकारनिर्देश size_t(*गन्तव्यम्_द्विमान_t)(ध्रुव uchar *, uchar *);
    संरचना elem_गन्तव्यम्_द्विमान_t
    {
        size_t      offset;
        size_t      विस्थापनम्_packed;
        गन्तव्यम्_द्विमान_t func;
    };

गुप्त:
    size_t make_गन्तव्यम्_द्विमान_funcs(ध्रुव std::string &dt);

गुप्त:
    ध्रुव uchar * beg;
    ध्रुव uchar * cur;
    ध्रुव uchar * end;

    size_t चरणः;
    size_t चरणः_packed;
    std::vector<elem_गन्तव्यम्_द्विमान_t> गन्तव्यम्_द्विमान_funcs;
};

}

}
#यदि_अन्तः
