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
    ꦱꦼꦢꦼꦭ( left < right && str[left] == ' ' )
        left++;
    ꦱꦼꦢꦼꦭ( right > left && str[right-1] == ' ' )
        right--;
    ꦮꦭꦶ left >= right ? String("") : str.substr(left, right-left);
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
    ꦪꦼꦤ꧀( type == Param::INT )
        ꦮꦭꦶ "int";
    ꦪꦼꦤ꧀( type == Param::BOOLEAN )
        ꦮꦭꦶ "bool";
    ꦪꦼꦤ꧀( type == Param::UNSIGNED_INT )
        ꦮꦭꦶ "unsigned";
    ꦪꦼꦤ꧀( type == Param::UINT64 )
        ꦮꦭꦶ "unsigned long long";
    ꦪꦼꦤ꧀( type == Param::FLOAT )
        ꦮꦭꦶ "float";
    ꦪꦼꦤ꧀( type == Param::REAL )
        ꦮꦭꦶ "double";
    ꦪꦼꦤ꧀( type == Param::STRING )
        ꦮꦭꦶ "string";
    ꦮꦭꦶ "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ꦪꦼꦤ꧀ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ꦮꦭꦶ value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ꦪꦼꦤ꧀( type == Param::INT )
        ss >> *(puur_naa_nka*)dst;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::FLOAT )
        ss >> *(float*)dst;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::STRING )
        *(String*)dst = str;
    ꦭꦶꦪ ꦪꦼꦤ꧀( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ꦭꦶꦪ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ꦪꦼꦤ꧀ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
        {
            ꦏꦁꦒꦺꦴ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ꦪꦼꦤ꧀ (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ꦪꦼꦤ꧀ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ꦪꦼꦤ꧀((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ꦮꦭꦶ;
                    }

                    from_str(v, type, dst);
                    ꦮꦭꦶ;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ꦮꦭꦶ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(puur_naa_nka index, bool space_delete, Param type, void* dst) const
{
    try
    {
        ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
        {
            ꦪꦼꦤ꧀ (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ꦪꦼꦤ꧀ (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ꦪꦼꦤ꧀((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ꦮꦭꦶ;
                }
                from_str(v, type, dst);
                ꦮꦭꦶ;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ꦮꦭꦶ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ꦪꦼꦤ꧀ (p1.number < p2.number)
        ꦮꦭꦶ true;

    ꦪꦼꦤ꧀ (p1.number > p2.number)
        ꦮꦭꦶ false;

    ꦮꦭꦶ p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(puur_naa_nka argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ꦪꦼꦤ꧀ (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    ꦭꦶꦪ
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    puur_naa_nka jj = 0;
    ꦏꦁꦒꦺꦴ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ꦪꦼꦤ꧀ (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        ꦭꦶꦪ
        {
            ꦪꦼꦤ꧀ (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ꦪꦼꦤ꧀ (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ꦪꦼꦤ꧀(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        ꦭꦶꦪ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ꦪꦼꦤ꧀ (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ꦪꦼꦤ꧀( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ꦪꦼꦤ꧀(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ꦮꦭꦶ *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    ꦏꦁꦒꦺꦴ (size_t i = 0; i < data.size(); i++)
    {
        ꦏꦁꦒꦺꦴ (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ꦪꦼꦤ꧀ (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(puur_naa_nka i, String value)
{
    ꦏꦁꦒꦺꦴ (size_t j = 0; j < data.size(); j++)
    {
        ꦪꦼꦤ꧀ (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    ꦏꦁꦒꦺꦴ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ꦮꦭꦶ impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
    {
        ꦏꦁꦒꦺꦴ (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ꦪꦼꦤ꧀ (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ꦮꦭꦶ !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ꦮꦭꦶ impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ꦪꦼꦤ꧀ (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ꦪꦼꦤ꧀ (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
    {
        ꦪꦼꦤ꧀ (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
    {
        ꦪꦼꦤ꧀ (impl->data[i].number == -1)
        {
            printf("\t");
            ꦏꦁꦒꦺꦴ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ꦪꦼꦤ꧀ (k.length() > 1)
                {
                    printf("--");
                }
                ꦭꦶꦪ
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ꦪꦼꦤ꧀ (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ꦪꦼꦤ꧀ (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    ꦏꦁꦒꦺꦴ (size_t i = 0; i < impl->data.size(); i++)
    {
        ꦪꦼꦤ꧀ (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ꦪꦼꦤ꧀ (dv.compare("") != 0)
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

    ꦱꦼꦢꦼꦭ (!str.empty())
    {
        ꦪꦼꦤ꧀ (str[0] == fs)
        {
            ꦪꦼꦤ꧀ (begin == true)
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

        ꦪꦼꦤ꧀ (str[0] == ss)
        {
            ꦪꦼꦤ꧀ (begin == false)
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

        ꦪꦼꦤ꧀ (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ꦪꦼꦤ꧀ (begin == true)
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

    ꦮꦭꦶ vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ꦱꦼꦢꦼꦭ (!str.empty())
    {
        ꦪꦼꦤ꧀ (str[0] == symbol)
        {
            ꦪꦼꦤ꧀ (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        ꦭꦶꦪ
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ꦪꦼꦤ꧀ (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ꦮꦭꦶ vec;
}

}
