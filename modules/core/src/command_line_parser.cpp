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
    ကိန်းပြည့် left = 0, right = (ကိန်းပြည့်)str.length();
    နေစဉ်( left < right && str[left] == ' ' )
        left++;
    နေစဉ်( right > left && str[right-1] == ' ' )
        right--;
    ပြန်ပေး left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    ကိန်းပြည့် number;
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
    void apply_params(ကိန်းပြည့် i, String value);

    void sort_params();
    ကိန်းပြည့် refcount;
};


static const var_na* get_type_name(Param type)
{
    အကယ်၍( type == Param::INT )
        ပြန်ပေး "int";
    အကယ်၍( type == Param::BOOLEAN )
        ပြန်ပေး "bool";
    အကယ်၍( type == Param::UNSIGNED_INT )
        ပြန်ပေး "unsigned";
    အကယ်၍( type == Param::UINT64 )
        ပြန်ပေး "unsigned long long";
    အကယ်၍( type == Param::FLOAT )
        ပြန်ပေး "float";
    အကယ်၍( type == Param::REAL )
        ပြန်ပေး "double";
    အကယ်၍( type == Param::STRING )
        ပြန်ပေး "string";
    ပြန်ပေး "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    အကယ်၍ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ပြန်ပေး value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    အကယ်၍( type == Param::INT )
        ss >> *(ကိန်းပြည့်*)dst;
    မဟုတ်ပါက အကယ်၍( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    မဟုတ်ပါက အကယ်၍( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    မဟုတ်ပါက အကယ်၍( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    မဟုတ်ပါက အကယ်၍( type == Param::FLOAT )
        ss >> *(float*)dst;
    မဟုတ်ပါက အကယ်၍( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    မဟုတ်ပါက အကယ်၍( type == Param::STRING )
        *(String*)dst = str;
    မဟုတ်ပါက အကယ်၍( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        အတွက် (ကိန်းပြည့် i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    မဟုတ်ပါက
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    အကယ်၍ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        အတွက် (size_t i = 0; i < impl->data.size(); i++)
        {
            အတွက် (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                အကယ်၍ (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    အကယ်၍ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    အကယ်၍((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ပြန်ပေး;
                    }

                    from_str(v, type, dst);
                    ပြန်ပေး;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ပြန်ပေး;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(ကိန်းပြည့် index, bool space_delete, Param type, void* dst) const
{
    try
    {
        အတွက် (size_t i = 0; i < impl->data.size(); i++)
        {
            အကယ်၍ (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                အကယ်၍ (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                အကယ်၍((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ပြန်ပေး;
                }
                from_str(v, type, dst);
                ပြန်ပေး;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ပြန်ပေး;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    အကယ်၍ (p1.number < p2.number)
        ပြန်ပေး true;

    အကယ်၍ (p1.number > p2.number)
        ပြန်ပေး false;

    ပြန်ပေး p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(ကိန်းပြည့် argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    အကယ်၍ (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    မဟုတ်ပါက
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    ကိန်းပြည့် jj = 0;
    အတွက် (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        အကယ်၍ (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        မဟုတ်ပါက
        {
            အကယ်၍ (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    အတွက် (ကိန်းပြည့် i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        အကယ်၍ (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            အကယ်၍(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        မဟုတ်ပါက
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    အကယ်၍ (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    အကယ်၍( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        အကယ်၍(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ပြန်ပေး *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    အတွက် (size_t i = 0; i < data.size(); i++)
    {
        အတွက် (size_t k = 0; k < data[i].keys.size(); k++)
        {
            အကယ်၍ (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(ကိန်းပြည့် i, String value)
{
    အတွက် (size_t j = 0; j < data.size(); j++)
    {
        အကယ်၍ (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    အတွက် (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ပြန်ပေး impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    အတွက် (size_t i = 0; i < impl->data.size(); i++)
    {
        အတွက် (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            အကယ်၍ (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ပြန်ပေး !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ပြန်ပေး impl->error == false;
}

void CommandLineParser::printErrors() const
{
    အကယ်၍ (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    အကယ်၍ (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    အတွက် (size_t i = 0; i < impl->data.size(); i++)
    {
        အကယ်၍ (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    အတွက် (size_t i = 0; i < impl->data.size(); i++)
    {
        အကယ်၍ (impl->data[i].number == -1)
        {
            printf("\t");
            အတွက် (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                အကယ်၍ (k.length() > 1)
                {
                    printf("--");
                }
                မဟုတ်ပါက
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                အကယ်၍ (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            အကယ်၍ (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    အတွက် (size_t i = 0; i < impl->data.size(); i++)
    {
        အကယ်၍ (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            အကယ်၍ (dv.compare("") != 0)
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

    နေစဉ် (!str.empty())
    {
        အကယ်၍ (str[0] == fs)
        {
            အကယ်၍ (begin == true)
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

        အကယ်၍ (str[0] == ss)
        {
            အကယ်၍ (begin == false)
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

        အကယ်၍ (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    အကယ်၍ (begin == true)
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

    ပြန်ပေး vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    နေစဉ် (!str.empty())
    {
        အကယ်၍ (str[0] == symbol)
        {
            အကယ်၍ (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        မဟုတ်ပါက
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    အကယ်၍ (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ပြန်ပေး vec;
}

}
