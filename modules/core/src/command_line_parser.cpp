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
    puur_naa_nka left = 0, right = (puur_naa_nka)str.length();
    ၶၢဝ်း( left < right && str[left] == ' ' )
        left++;
    ၶၢဝ်း( right > left && str[right-1] == ' ' )
        right--;
    ပွၵ်ႈ left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    puur_naa_nka number;
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
    void apply_params(puur_naa_nka i, String value);

    void sort_params();
    puur_naa_nka refcount;
};


static const var_na* get_type_name(Param type)
{
    ပေႃး( type == Param::INT )
        ပွၵ်ႈ "int";
    ပေႃး( type == Param::BOOLEAN )
        ပွၵ်ႈ "bool";
    ပေႃး( type == Param::UNSIGNED_INT )
        ပွၵ်ႈ "unsigned";
    ပေႃး( type == Param::UINT64 )
        ပွၵ်ႈ "unsigned long long";
    ပေႃး( type == Param::FLOAT )
        ပွၵ်ႈ "float";
    ပေႃး( type == Param::REAL )
        ပွၵ်ႈ "double";
    ပေႃး( type == Param::STRING )
        ပွၵ်ႈ "string";
    ပွၵ်ႈ "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ပေႃး (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ပွၵ်ႈ value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ပေႃး( type == Param::INT )
        ss >> *(puur_naa_nka*)dst;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    ပေႃးဢမ်ႇ ပေႃး( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::FLOAT )
        ss >> *(float*)dst;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::STRING )
        *(String*)dst = str;
    ပေႃးဢမ်ႇ ပေႃး( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        for (puur_naa_nka i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ပေႃးဢမ်ႇ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ပေႃး (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            for (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ပေႃး (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ပေႃး (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ပေႃး((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ပွၵ်ႈ;
                    }

                    from_str(v, type, dst);
                    ပွၵ်ႈ;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ပွၵ်ႈ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(puur_naa_nka index, bool space_delete, Param type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            ပေႃး (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ပေႃး (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ပေႃး((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ပွၵ်ႈ;
                }
                from_str(v, type, dst);
                ပွၵ်ႈ;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ပွၵ်ႈ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ပေႃး (p1.number < p2.number)
        ပွၵ်ႈ true;

    ပေႃး (p1.number > p2.number)
        ပွၵ်ႈ false;

    ပွၵ်ႈ p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(puur_naa_nka argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ပေႃး (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    ပေႃးဢမ်ႇ
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    puur_naa_nka jj = 0;
    for (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ပေႃး (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        ပေႃးဢမ်ႇ
        {
            ပေႃး (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    for (puur_naa_nka i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ပေႃး (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ပေႃး(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        ပေႃးဢမ်ႇ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ပေႃး (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ပေႃး( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ပေႃး(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ပွၵ်ႈ *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ပေႃး (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(puur_naa_nka i, String value)
{
    for (size_t j = 0; j < data.size(); j++)
    {
        ပေႃး (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    for (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ပွၵ်ႈ impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    for (size_t i = 0; i < impl->data.size(); i++)
    {
        for (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ပေႃး (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ပွၵ်ႈ !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ပွၵ်ႈ impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ပေႃး (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ပေႃး (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        ပေႃး (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        ပေႃး (impl->data[i].number == -1)
        {
            printf("\t");
            for (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ပေႃး (k.length() > 1)
                {
                    printf("--");
                }
                ပေႃးဢမ်ႇ
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ပေႃး (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ပေႃး (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        ပေႃး (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ပေႃး (dv.compare("") != 0)
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

    ၶၢဝ်း (!str.empty())
    {
        ပေႃး (str[0] == fs)
        {
            ပေႃး (begin == true)
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

        ပေႃး (str[0] == ss)
        {
            ပေႃး (begin == false)
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

        ပေႃး (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ပေႃး (begin == true)
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

    ပွၵ်ႈ vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ၶၢဝ်း (!str.empty())
    {
        ပေႃး (str[0] == symbol)
        {
            ပေႃး (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        ပေႃးဢမ်ႇ
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ပေႃး (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ပွၵ်ႈ vec;
}

}
