// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "precomp.hpp"
#include <sstream>

namespace cv
{

namespace {
static const var_na* noneValue = "<none>";

static String cat_string(const String& str)
{
    จำนวนเต็ม left = 0, right = (จำนวนเต็ม)str.length();
    ขณะ( left < right && str[left] == ' ' )
        left++;
    ขณะ( right > left && str[right-1] == ' ' )
        right--;
    คืน left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    จำนวนเต็ม number;
};


sa_nracanaa CommandLineParser::Impl
{
    bool error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<CommandLineParserParams> data;

    std::vector<String> split_range_string(const String& str, var_na fs, var_na ss) const;
    std::vector<String> split_string(const String& str, var_na symbol = ' ', bool create_empty_item = false) const;

    void apply_params(const String& key, const String& value);
    void apply_params(จำนวนเต็ม i, String value);

    void sort_params();
    จำนวนเต็ม refcount;
};


static const var_na* get_type_name(Param type)
{
    ถ้า( type == Param::INT )
        คืน "int";
    ถ้า( type == Param::BOOLEAN )
        คืน "bool";
    ถ้า( type == Param::UNSIGNED_INT )
        คืน "unsigned";
    ถ้า( type == Param::UINT64 )
        คืน "unsigned long long";
    ถ้า( type == Param::FLOAT )
        คืน "float";
    ถ้า( type == Param::REAL )
        คืน "double";
    ถ้า( type == Param::STRING )
        คืน "string";
    คืน "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ถ้า (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    คืน value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ถ้า( type == Param::INT )
        ss >> *(จำนวนเต็ม*)dst;
    มิฉะนั้น ถ้า( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    มิฉะนั้น ถ้า( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    มิฉะนั้น ถ้า( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    มิฉะนั้น ถ้า( type == Param::FLOAT )
        ss >> *(float*)dst;
    มิฉะนั้น ถ้า( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    มิฉะนั้น ถ้า( type == Param::STRING )
        *(String*)dst = str;
    มิฉะนั้น ถ้า( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        สำหรับ (จำนวนเต็ม i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    มิฉะนั้น
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ถ้า (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        สำหรับ (size_t i = 0; i < impl->data.size(); i++)
        {
            สำหรับ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ถ้า (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ถ้า (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ถ้า((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        คืน;
                    }

                    from_str(v, type, dst);
                    คืน;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        คืน;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(จำนวนเต็ม index, bool space_delete, Param type, void* dst) const
{
    try
    {
        สำหรับ (size_t i = 0; i < impl->data.size(); i++)
        {
            ถ้า (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ถ้า (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ถ้า((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    คืน;
                }
                from_str(v, type, dst);
                คืน;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        คืน;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ถ้า (p1.number < p2.number)
        คืน true;

    ถ้า (p1.number > p2.number)
        คืน false;

    คืน p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(จำนวนเต็ม argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ถ้า (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    มิฉะนั้น
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    จำนวนเต็ม jj = 0;
    สำหรับ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ถ้า (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        มิฉะนั้น
        {
            ถ้า (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    สำหรับ (จำนวนเต็ม i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ถ้า (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ถ้า(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        มิฉะนั้น
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ถ้า (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ถ้า( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ถ้า(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    คืน *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    สำหรับ (size_t i = 0; i < data.size(); i++)
    {
        สำหรับ (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ถ้า (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(จำนวนเต็ม i, String value)
{
    สำหรับ (size_t j = 0; j < data.size(); j++)
    {
        ถ้า (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    สำหรับ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    คืน impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    สำหรับ (size_t i = 0; i < impl->data.size(); i++)
    {
        สำหรับ (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ถ้า (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                คืน !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    คืน impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ถ้า (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ถ้า (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    สำหรับ (size_t i = 0; i < impl->data.size(); i++)
    {
        ถ้า (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    สำหรับ (size_t i = 0; i < impl->data.size(); i++)
    {
        ถ้า (impl->data[i].number == -1)
        {
            printf("\t");
            สำหรับ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ถ้า (k.length() > 1)
                {
                    printf("--");
                }
                มิฉะนั้น
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ถ้า (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ถ้า (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    สำหรับ (size_t i = 0; i < impl->data.size(); i++)
    {
        ถ้า (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ถ้า (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> CommandLineParser::Impl::split_range_string(const String& _str, var_na fs, var_na ss) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    bool begin = false;

    ขณะ (!str.empty())
    {
        ถ้า (str[0] == fs)
        {
            ถ้า (begin == true)
            {
                k_sipa cv::Exception(cv::Error::StsParseError,
                         String("error in split_range_string(")
                         + str
                         + String(", ")
                         + String(1, fs)
                         + String(", ")
                         + String(1, ss)
                         + String(")"),
                         "", __FILE__, __LINE__
                         );
            }
            begin = true;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        ถ้า (str[0] == ss)
        {
            ถ้า (begin == false)
            {
                k_sipa cv::Exception(cv::Error::StsParseError,
                         String("error in split_range_string(")
                         + str
                         + String(", ")
                         + String(1, fs)
                         + String(", ")
                         + String(1, ss)
                         + String(")"),
                         "", __FILE__, __LINE__
                         );
            }
            begin = false;
            vec.push_back(word);
        }

        ถ้า (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ถ้า (begin == true)
    {
        k_sipa cv::Exception(cv::Error::StsParseError,
                 String("error in split_range_string(")
                 + str
                 + String(", ")
                 + String(1, fs)
                 + String(", ")
                 + String(1, ss)
                 + String(")"),
                 "", __FILE__, __LINE__
                );
    }

    คืน vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ขณะ (!str.empty())
    {
        ถ้า (str[0] == symbol)
        {
            ถ้า (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        มิฉะนั้น
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ถ้า (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    คืน vec;
}

}
