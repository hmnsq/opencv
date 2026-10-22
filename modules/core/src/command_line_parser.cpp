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
    ចំនួនគត់ left = 0, right = (ចំនួនគត់)str.length();
    ខណៈពេល( left < right && str[left] == ' ' )
        left++;
    ខណៈពេល( right > left && str[right-1] == ' ' )
        right--;
    ត្រឡប់ left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    ចំនួនគត់ number;
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
    void apply_params(ចំនួនគត់ i, String value);

    void sort_params();
    ចំនួនគត់ refcount;
};


static const var_na* get_type_name(Param type)
{
    ប្រសិនបើ( type == Param::INT )
        ត្រឡប់ "int";
    ប្រសិនបើ( type == Param::BOOLEAN )
        ត្រឡប់ "bool";
    ប្រសិនបើ( type == Param::UNSIGNED_INT )
        ត្រឡប់ "unsigned";
    ប្រសិនបើ( type == Param::UINT64 )
        ត្រឡប់ "unsigned long long";
    ប្រសិនបើ( type == Param::FLOAT )
        ត្រឡប់ "float";
    ប្រសិនបើ( type == Param::REAL )
        ត្រឡប់ "double";
    ប្រសិនបើ( type == Param::STRING )
        ត្រឡប់ "string";
    ត្រឡប់ "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ប្រសិនបើ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ត្រឡប់ value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ប្រសិនបើ( type == Param::INT )
        ss >> *(ចំនួនគត់*)dst;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::FLOAT )
        ss >> *(float*)dst;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::STRING )
        *(String*)dst = str;
    បើមិនដូច្នេះ ប្រសិនបើ( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        សម្រាប់ (ចំនួនគត់ i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    បើមិនដូច្នេះ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ប្រសិនបើ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
        {
            សម្រាប់ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ប្រសិនបើ (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ប្រសិនបើ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ប្រសិនបើ((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ត្រឡប់;
                    }

                    from_str(v, type, dst);
                    ត្រឡប់;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ត្រឡប់;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(ចំនួនគត់ index, bool space_delete, Param type, void* dst) const
{
    try
    {
        សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
        {
            ប្រសិនបើ (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ប្រសិនបើ (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ប្រសិនបើ((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ត្រឡប់;
                }
                from_str(v, type, dst);
                ត្រឡប់;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ត្រឡប់;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ប្រសិនបើ (p1.number < p2.number)
        ត្រឡប់ true;

    ប្រសិនបើ (p1.number > p2.number)
        ត្រឡប់ false;

    ត្រឡប់ p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(ចំនួនគត់ argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ប្រសិនបើ (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    បើមិនដូច្នេះ
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    ចំនួនគត់ jj = 0;
    សម្រាប់ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ប្រសិនបើ (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        បើមិនដូច្នេះ
        {
            ប្រសិនបើ (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    សម្រាប់ (ចំនួនគត់ i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ប្រសិនបើ (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ប្រសិនបើ(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        បើមិនដូច្នេះ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ប្រសិនបើ (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ប្រសិនបើ( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ប្រសិនបើ(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ត្រឡប់ *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    សម្រាប់ (size_t i = 0; i < data.size(); i++)
    {
        សម្រាប់ (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ប្រសិនបើ (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(ចំនួនគត់ i, String value)
{
    សម្រាប់ (size_t j = 0; j < data.size(); j++)
    {
        ប្រសិនបើ (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    សម្រាប់ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ត្រឡប់ impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
    {
        សម្រាប់ (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ប្រសិនបើ (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ត្រឡប់ !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ត្រឡប់ impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ប្រសិនបើ (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ប្រសិនបើ (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
    {
        ប្រសិនបើ (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
    {
        ប្រសិនបើ (impl->data[i].number == -1)
        {
            printf("\t");
            សម្រាប់ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ប្រសិនបើ (k.length() > 1)
                {
                    printf("--");
                }
                បើមិនដូច្នេះ
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ប្រសិនបើ (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ប្រសិនបើ (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    សម្រាប់ (size_t i = 0; i < impl->data.size(); i++)
    {
        ប្រសិនបើ (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ប្រសិនបើ (dv.compare("") != 0)
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

    ខណៈពេល (!str.empty())
    {
        ប្រសិនបើ (str[0] == fs)
        {
            ប្រសិនបើ (begin == true)
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

        ប្រសិនបើ (str[0] == ss)
        {
            ប្រសិនបើ (begin == false)
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

        ប្រសិនបើ (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ប្រសិនបើ (begin == true)
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

    ត្រឡប់ vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ខណៈពេល (!str.empty())
    {
        ប្រសិនបើ (str[0] == symbol)
        {
            ប្រសិនបើ (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        បើមិនដូច្នេះ
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ប្រសិនបើ (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ត្រឡប់ vec;
}

}
