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
    ཧྲིལ left = 0, right = (ཧྲིལ)str.length();
    སྐབས( left < right && str[left] == ' ' )
        left++;
    སྐབས( right > left && str[right-1] == ' ' )
        right--;
    ཕྱིར left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    ཧྲིལ number;
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
    void apply_params(ཧྲིལ i, String value);

    void sort_params();
    ཧྲིལ refcount;
};


static const var_na* get_type_name(Param type)
{
    གལ་ཏེ( type == Param::INT )
        ཕྱིར "int";
    གལ་ཏེ( type == Param::BOOLEAN )
        ཕྱིར "bool";
    གལ་ཏེ( type == Param::UNSIGNED_INT )
        ཕྱིར "unsigned";
    གལ་ཏེ( type == Param::UINT64 )
        ཕྱིར "unsigned long long";
    གལ་ཏེ( type == Param::FLOAT )
        ཕྱིར "float";
    གལ་ཏེ( type == Param::REAL )
        ཕྱིར "double";
    གལ་ཏེ( type == Param::STRING )
        ཕྱིར "string";
    ཕྱིར "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    གལ་ཏེ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ཕྱིར value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    གལ་ཏེ( type == Param::INT )
        ss >> *(ཧྲིལ*)dst;
    དེ་མིན གལ་ཏེ( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    དེ་མིན གལ་ཏེ( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    དེ་མིན གལ་ཏེ( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    དེ་མིན གལ་ཏེ( type == Param::FLOAT )
        ss >> *(float*)dst;
    དེ་མིན གལ་ཏེ( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    དེ་མིན གལ་ཏེ( type == Param::STRING )
        *(String*)dst = str;
    དེ་མིན གལ་ཏེ( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ཆེད་དུ (ཧྲིལ i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    དེ་མིན
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    གལ་ཏེ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
        {
            ཆེད་དུ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                གལ་ཏེ (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    གལ་ཏེ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    གལ་ཏེ((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ཕྱིར;
                    }

                    from_str(v, type, dst);
                    ཕྱིར;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ཕྱིར;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(ཧྲིལ index, bool space_delete, Param type, void* dst) const
{
    try
    {
        ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
        {
            གལ་ཏེ (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                གལ་ཏེ (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                གལ་ཏེ((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ཕྱིར;
                }
                from_str(v, type, dst);
                ཕྱིར;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ཕྱིར;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    གལ་ཏེ (p1.number < p2.number)
        ཕྱིར true;

    གལ་ཏེ (p1.number > p2.number)
        ཕྱིར false;

    ཕྱིར p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(ཧྲིལ argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    གལ་ཏེ (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    དེ་མིན
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    ཧྲིལ jj = 0;
    ཆེད་དུ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        གལ་ཏེ (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        དེ་མིན
        {
            གལ་ཏེ (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ཆེད་དུ (ཧྲིལ i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        གལ་ཏེ (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            གལ་ཏེ(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        དེ་མིན
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    གལ་ཏེ (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    གལ་ཏེ( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        གལ་ཏེ(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ཕྱིར *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    ཆེད་དུ (size_t i = 0; i < data.size(); i++)
    {
        ཆེད་དུ (size_t k = 0; k < data[i].keys.size(); k++)
        {
            གལ་ཏེ (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(ཧྲིལ i, String value)
{
    ཆེད་དུ (size_t j = 0; j < data.size(); j++)
    {
        གལ་ཏེ (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    ཆེད་དུ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ཕྱིར impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
    {
        ཆེད་དུ (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            གལ་ཏེ (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ཕྱིར !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ཕྱིར impl->error == false;
}

void CommandLineParser::printErrors() const
{
    གལ་ཏེ (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    གལ་ཏེ (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
    {
        གལ་ཏེ (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
    {
        གལ་ཏེ (impl->data[i].number == -1)
        {
            printf("\t");
            ཆེད་དུ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                གལ་ཏེ (k.length() > 1)
                {
                    printf("--");
                }
                དེ་མིན
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                གལ་ཏེ (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            གལ་ཏེ (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    ཆེད་དུ (size_t i = 0; i < impl->data.size(); i++)
    {
        གལ་ཏེ (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            གལ་ཏེ (dv.compare("") != 0)
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

    སྐབས (!str.empty())
    {
        གལ་ཏེ (str[0] == fs)
        {
            གལ་ཏེ (begin == true)
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

        གལ་ཏེ (str[0] == ss)
        {
            གལ་ཏེ (begin == false)
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

        གལ་ཏེ (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    གལ་ཏེ (begin == true)
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

    ཕྱིར vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    སྐབས (!str.empty())
    {
        གལ་ཏེ (str[0] == symbol)
        {
            གལ་ཏེ (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        དེ་མིན
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    གལ་ཏེ (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ཕྱིར vec;
}

}
