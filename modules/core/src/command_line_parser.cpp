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
    цяло left = 0, right = (цяло)str.length();
    докато( left < right && str[left] == ' ' )
        left++;
    докато( right > left && str[right-1] == ' ' )
        right--;
    върни left >= right ? String("") : str.substr(left, right-left);
}
}

struct CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    цяло number;
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
    void apply_params(цяло i, String value);

    void sort_params();
    цяло refcount;
};


static const char* get_type_name(Param type)
{
    ако( type == Param::INT )
        върни "int";
    ако( type == Param::BOOLEAN )
        върни "bool";
    ако( type == Param::UNSIGNED_INT )
        върни "unsigned";
    ако( type == Param::UINT64 )
        върни "unsigned long long";
    ако( type == Param::FLOAT )
        върни "float";
    ако( type == Param::REAL )
        върни "double";
    ако( type == Param::STRING )
        върни "string";
    върни "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ако (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    върни value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ако( type == Param::INT )
        ss >> *(цяло*)dst;
    иначе ако( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    иначе ако( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    иначе ако( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    иначе ако( type == Param::FLOAT )
        ss >> *(float*)dst;
    иначе ако( type == Param::REAL )
        ss >> *(double*)dst;
    иначе ако( type == Param::STRING )
        *(String*)dst = str;
    иначе ако( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        за (цяло i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    иначе
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ако (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        за (size_t i = 0; i < impl->data.size(); i++)
        {
            за (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ако (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ако (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ако((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        върни;
                    }

                    from_str(v, type, dst);
                    върни;
                }
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        върни;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(цяло index, bool space_delete, Param type, void* dst) const
{
    try
    {
        за (size_t i = 0; i < impl->data.size(); i++)
        {
            ако (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ако (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ако((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    върни;
                }
                from_str(v, type, dst);
                върни;
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        върни;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ако (p1.number < p2.number)
        върни true;

    ако (p1.number > p2.number)
        върни false;

    върни p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(цяло argc, const char* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ако (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    иначе
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    цяло jj = 0;
    за (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ако (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        иначе
        {
            ако (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    за (цяло i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ако (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ако(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        иначе
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ако (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ако( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ако(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    върни *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    за (size_t i = 0; i < data.size(); i++)
    {
        за (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ако (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                break;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(цяло i, String value)
{
    за (size_t j = 0; j < data.size(); j++)
    {
        ако (data[j].number == i)
        {
            data[j].def_value = value;
            break;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    за (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    върни impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    за (size_t i = 0; i < impl->data.size(); i++)
    {
        за (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ако (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                върни !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    върни impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ако (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ако (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    за (size_t i = 0; i < impl->data.size(); i++)
    {
        ако (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    за (size_t i = 0; i < impl->data.size(); i++)
    {
        ако (impl->data[i].number == -1)
        {
            printf("\t");
            за (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ако (k.length() > 1)
                {
                    printf("--");
                }
                иначе
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ако (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ако (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    за (size_t i = 0; i < impl->data.size(); i++)
    {
        ако (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ако (dv.compare("") != 0)
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

    докато (!str.empty())
    {
        ако (str[0] == fs)
        {
            ако (begin == true)
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

        ако (str[0] == ss)
        {
            ако (begin == false)
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

        ако (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ако (begin == true)
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

    върни vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, char symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    докато (!str.empty())
    {
        ако (str[0] == symbol)
        {
            ако (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        иначе
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ако (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    върни vec;
}

}
