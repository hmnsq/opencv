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
    整数 left = 0, right = (整数)str.length();
    ながら( left < right && str[left] == ' ' )
        left++;
    ながら( right > left && str[right-1] == ' ' )
        right--;
    戻る left >= right ? String("") : str.substr(left, right-left);
}
}

struct CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    整数 number;
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
    void apply_params(整数 i, String value);

    void sort_params();
    整数 refcount;
};


static const char* get_type_name(Param type)
{
    もし( type == Param::INT )
        戻る "int";
    もし( type == Param::BOOLEAN )
        戻る "bool";
    もし( type == Param::UNSIGNED_INT )
        戻る "unsigned";
    もし( type == Param::UINT64 )
        戻る "unsigned long long";
    もし( type == Param::FLOAT )
        戻る "float";
    もし( type == Param::REAL )
        戻る "double";
    もし( type == Param::STRING )
        戻る "string";
    戻る "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    もし (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    戻る value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    もし( type == Param::INT )
        ss >> *(整数*)dst;
    否则 もし( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    否则 もし( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    否则 もし( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    否则 もし( type == Param::FLOAT )
        ss >> *(float*)dst;
    否则 もし( type == Param::REAL )
        ss >> *(double*)dst;
    否则 もし( type == Param::STRING )
        *(String*)dst = str;
    否则 もし( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ために (整数 i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    否则
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    もし (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        ために (size_t i = 0; i < impl->data.size(); i++)
        {
            ために (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                もし (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    もし (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    もし((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        戻る;
                    }

                    from_str(v, type, dst);
                    戻る;
                }
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        戻る;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(整数 index, bool space_delete, Param type, void* dst) const
{
    try
    {
        ために (size_t i = 0; i < impl->data.size(); i++)
        {
            もし (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                もし (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                もし((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    戻る;
                }
                from_str(v, type, dst);
                戻る;
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        戻る;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    もし (p1.number < p2.number)
        戻る true;

    もし (p1.number > p2.number)
        戻る false;

    戻る p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(整数 argc, const char* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    もし (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    否则
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    整数 jj = 0;
    ために (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        もし (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        否则
        {
            もし (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ために (整数 i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        もし (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            もし(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        否则
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    もし (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    もし( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        もし(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    戻る *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    ために (size_t i = 0; i < data.size(); i++)
    {
        ために (size_t k = 0; k < data[i].keys.size(); k++)
        {
            もし (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                break;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(整数 i, String value)
{
    ために (size_t j = 0; j < data.size(); j++)
    {
        もし (data[j].number == i)
        {
            data[j].def_value = value;
            break;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    ために (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    戻る impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    ために (size_t i = 0; i < impl->data.size(); i++)
    {
        ために (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            もし (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                戻る !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    戻る impl->error == false;
}

void CommandLineParser::printErrors() const
{
    もし (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    もし (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    ために (size_t i = 0; i < impl->data.size(); i++)
    {
        もし (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    ために (size_t i = 0; i < impl->data.size(); i++)
    {
        もし (impl->data[i].number == -1)
        {
            printf("\t");
            ために (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                もし (k.length() > 1)
                {
                    printf("--");
                }
                否则
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                もし (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            もし (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    ために (size_t i = 0; i < impl->data.size(); i++)
    {
        もし (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            もし (dv.compare("") != 0)
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

    ながら (!str.empty())
    {
        もし (str[0] == fs)
        {
            もし (begin == true)
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

        もし (str[0] == ss)
        {
            もし (begin == false)
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

        もし (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    もし (begin == true)
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

    戻る vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, char symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ながら (!str.empty())
    {
        もし (str[0] == symbol)
        {
            もし (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        否则
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    もし (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    戻る vec;
}

}
