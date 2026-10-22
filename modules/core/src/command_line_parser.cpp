// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "precomp.hpp"
#include <sstream>

namespace cv
{

namespace {
static const char* noneValue = "<none>";

static String cat_string(const String& str)
{
    số_nguyên left = 0, right = (số_nguyên)str.length();
    trong_khi( left < right && str[left] == ' ' )
        left++;
    trong_khi( right > left && str[right-1] == ' ' )
        right--;
    trả_về left >= right ? String("") : str.substr(left, right-left);
}
}

struct CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    số_nguyên number;
};


struct CommandLineParser::Impl
{
    bool error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<CommandLineParserParams> data;

    std::vector<String> split_range_string(const String& str, char fs, char ss) const;
    std::vector<String> split_string(const String& str, char symbol = ' ', bool create_empty_item = false) const;

    void apply_params(const String& key, const String& value);
    void apply_params(số_nguyên i, String value);

    void sort_params();
    số_nguyên refcount;
};


static const char* get_type_name(Param type)
{
    nếu( type == Param::INT )
        trả_về "int";
    nếu( type == Param::BOOLEAN )
        trả_về "bool";
    nếu( type == Param::UNSIGNED_INT )
        trả_về "unsigned";
    nếu( type == Param::UINT64 )
        trả_về "unsigned long long";
    nếu( type == Param::FLOAT )
        trả_về "float";
    nếu( type == Param::REAL )
        trả_về "double";
    nếu( type == Param::STRING )
        trả_về "string";
    trả_về "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    nếu (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    trả_về value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    nếu( type == Param::INT )
        ss >> *(số_nguyên*)dst;
    không_thì nếu( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    không_thì nếu( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    không_thì nếu( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    không_thì nếu( type == Param::FLOAT )
        ss >> *(float*)dst;
    không_thì nếu( type == Param::REAL )
        ss >> *(double*)dst;
    không_thì nếu( type == Param::STRING )
        *(String*)dst = str;
    không_thì nếu( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        cho (số_nguyên i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    không_thì
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    nếu (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        cho (size_t i = 0; i < impl->data.size(); i++)
        {
            cho (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                nếu (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    nếu (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    nếu((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        trả_về;
                    }

                    from_str(v, type, dst);
                    trả_về;
                }
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        trả_về;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(số_nguyên index, bool space_delete, Param type, void* dst) const
{
    try
    {
        cho (size_t i = 0; i < impl->data.size(); i++)
        {
            nếu (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                nếu (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                nếu((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    trả_về;
                }
                from_str(v, type, dst);
                trả_về;
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        trả_về;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    nếu (p1.number < p2.number)
        trả_về true;

    nếu (p1.number > p2.number)
        trả_về false;

    trả_về p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(số_nguyên argc, const char* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    nếu (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    không_thì
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    số_nguyên jj = 0;
    cho (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        nếu (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        không_thì
        {
            nếu (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    cho (số_nguyên i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        nếu (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            nếu(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        không_thì
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    nếu (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    nếu( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        nếu(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    trả_về *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    cho (size_t i = 0; i < data.size(); i++)
    {
        cho (size_t k = 0; k < data[i].keys.size(); k++)
        {
            nếu (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                break;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(số_nguyên i, String value)
{
    cho (size_t j = 0; j < data.size(); j++)
    {
        nếu (data[j].number == i)
        {
            data[j].def_value = value;
            break;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    cho (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    trả_về impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    cho (size_t i = 0; i < impl->data.size(); i++)
    {
        cho (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            nếu (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                trả_về !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    trả_về impl->error == false;
}

void CommandLineParser::printErrors() const
{
    nếu (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    nếu (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    cho (size_t i = 0; i < impl->data.size(); i++)
    {
        nếu (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    cho (size_t i = 0; i < impl->data.size(); i++)
    {
        nếu (impl->data[i].number == -1)
        {
            printf("\t");
            cho (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                nếu (k.length() > 1)
                {
                    printf("--");
                }
                không_thì
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                nếu (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            nếu (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    cho (size_t i = 0; i < impl->data.size(); i++)
    {
        nếu (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            nếu (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> CommandLineParser::Impl::split_range_string(const String& _str, char fs, char ss) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    bool begin = false;

    trong_khi (!str.empty())
    {
        nếu (str[0] == fs)
        {
            nếu (begin == true)
            {
                throw cv::Exception(cv::Error::StsParseError,
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

        nếu (str[0] == ss)
        {
            nếu (begin == false)
            {
                throw cv::Exception(cv::Error::StsParseError,
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

        nếu (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    nếu (begin == true)
    {
        throw cv::Exception(cv::Error::StsParseError,
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

    trả_về vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, char symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    trong_khi (!str.empty())
    {
        nếu (str[0] == symbol)
        {
            nếu (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        không_thì
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    nếu (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    trả_về vec;
}

}
