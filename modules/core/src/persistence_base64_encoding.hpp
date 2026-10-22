// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अगर_अघोषित OPENCV_CORE_BASE64_ENCODING_HPP
#घोषणा OPENCV_CORE_BASE64_ENCODING_HPP

namespace cv
{

namespace base64
{
/* A decorator for CvFileStorage
* - no copyable
* - not safe for now
* - move constructor may be needed if C++11
*/
uint8_t स्थिर base64_mapping[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

uint8_t स्थिर base64_padding = '=';

std::string make_base64_header(स्थिर अक्षर * dt);

माप_प्रकार base64_encode(uint8_t स्थिर * src, uint8_t * dst, माप_प्रकार off, माप_प्रकार cnt);


पूर्णांक icv_Calc_Struct_आकार( स्थिर अक्षर* dt, पूर्णांक initial_आकार );

श्रेणी Base64ContextEmitter;
श्रेणी Impl;

श्रेणी Base64Writer
{
खुला:
    Base64Writer(cv::FileStorage::Impl& fs, bool can_indent);
    ~Base64Writer();
    व्योम ग_लिखो(स्थिर व्योम* तथ्य_3, माप_प्रकार len, स्थिर अक्षर* dt);
    ढाँचा<typename to_द्विआधारी_convertor_t> व्योम ग_लिखो(to_द्विआधारी_convertor_t & convertor, स्थिर अक्षर* dt);

निजी:
    व्योम check_dt(स्थिर अक्षर* dt);

निजी:
    // disable copy and assignment
    Base64Writer(स्थिर Base64Writer &);
    Base64Writer & चालक=(स्थिर Base64Writer &);

निजी:

    Base64ContextEmitter * emitter;
    std::string तथ्य_type_string;
};

माप_प्रकार base64_encode_बफर_आकार(माप_प्रकार cnt, bool is_end_with_zero = true);

ढाँचा<typename _uint_t> अंतरभूत माप_प्रकार
to_द्विआधारी(_uint_t val, uchar * cur)
{
    माप_प्रकार delta = अक्षर_बिट;
    माप_प्रकार cnt = माप(_uint_t);
    जबतक (cnt --> static_cast<माप_प्रकार>(0U)) {
        *cur++ = static_cast<uchar>(val);
        val >>= delta;
    }
    वापस माप(_uint_t);
}

ढाँचा<> अंतरभूत माप_प्रकार to_द्विआधारी(bool val, uchar * cur)
{
    वापस to_द्विआधारी(static_cast<uchar>(val), cur);
}

ढाँचा<> अंतरभूत माप_प्रकार to_द्विआधारी(द्विगुन val, uchar * cur)
{
    Cv64suf bit64;
    bit64.f = val;
    वापस to_द्विआधारी(bit64.u, cur);
}

ढाँचा<> अंतरभूत माप_प्रकार to_द्विआधारी(भग्न val, uchar * cur)
{
    Cv32suf bit32;
    bit32.f = val;
    वापस to_द्विआधारी(bit32.u, cur);
}

ढाँचा<typename _primitive_t> अंतरभूत माप_प्रकार
to_द्विआधारी(uchar स्थिर * val, uchar * cur)
{
    वापस to_द्विआधारी<_primitive_t>(*reinterpret_cast<_primitive_t स्थिर *>(val), cur);
}



श्रेणी Raw_तथ्य_To_द्विआधारी_Convertor
{
खुला:
    // NOTE: len is already multiplied by element size here
    Raw_तथ्य_To_द्विआधारी_Convertor(स्थिर व्योम* src, पूर्णांक len, स्थिर std::string & dt);

    अंतरभूत Raw_तथ्य_To_द्विआधारी_Convertor & चालक >>(uchar * & dst);
    अंतरभूत चालक bool() स्थिर;

निजी:
    प्रकार माप_प्रकार(*to_द्विआधारी_t)(स्थिर uchar *, uchar *);
    काष्ठा elem_to_द्विआधारी_t
    {
        माप_प्रकार      offset;
        माप_प्रकार      offset_packed;
        to_द्विआधारी_t func;
    };

निजी:
    माप_प्रकार make_to_द्विआधारी_funcs(स्थिर std::string &dt);

निजी:
    स्थिर uchar * beg;
    स्थिर uchar * cur;
    स्थिर uchar * end;

    माप_प्रकार step;
    माप_प्रकार step_packed;
    std::vector<elem_to_द्विआधारी_t> to_द्विआधारी_funcs;
};

}

}
#पूर्ण_अगर
