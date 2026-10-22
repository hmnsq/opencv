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
    정수 left = 0, right = (정수)str.length();
    동안( left < right && str[left] == ' ' )
        left++;
    동안( right > left && str[right-1] == ' ' )
        right--;
    반환 left >= right ? String("") : str.substr(left, right-left);
}
}

struct CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    정수 number;
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
    void apply_params(정수 i, String value);

    void sort_params();
    정수 refcount;
};


static const char* get_type_name(Param type)
{
    만약( type == Param::INT )
        반환 "int";
    만약( type == Param::BOOLEAN )
        반환 "bool";
    만약( type == Param::UNSIGNED_INT )
        반환 "unsigned";
    만약( type == Param::UINT64 )
        반환 "unsigned long long";
    만약( type == Param::FLOAT )
        반환 "float";
    만약( type == Param::REAL )
        반환 "double";
    만약( type == Param::STRING )
        반환 "string";
    반환 "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    만약 (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    반환 value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    만약( type == Param::INT )
        ss >> *(정수*)dst;
    아니면 만약( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    아니면 만약( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    아니면 만약( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    아니면 만약( type == Param::FLOAT )
        ss >> *(float*)dst;
    아니면 만약( type == Param::REAL )
        ss >> *(double*)dst;
    아니면 만약( type == Param::STRING )
        *(String*)dst = str;
    아니면 만약( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        위해 (정수 i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    아니면
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    만약 (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        위해 (size_t i = 0; i < impl->data.size(); i++)
        {
            위해 (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                만약 (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    만약 (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    만약((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        반환;
                    }

                    from_str(v, type, dst);
                    반환;
                }
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        반환;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(정수 index, bool space_delete, Param type, void* dst) const
{
    try
    {
        위해 (size_t i = 0; i < impl->data.size(); i++)
        {
            만약 (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                만약 (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                만약((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    반환;
                }
                from_str(v, type, dst);
                반환;
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        반환;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    만약 (p1.number < p2.number)
        반환 true;

    만약 (p1.number > p2.number)
        반환 false;

    반환 p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(정수 argc, const char* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    만약 (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    아니면
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    정수 jj = 0;
    위해 (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        만약 (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        아니면
        {
            만약 (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    위해 (정수 i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        만약 (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            만약(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        아니면
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    만약 (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    만약( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        만약(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    반환 *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    위해 (size_t i = 0; i < data.size(); i++)
    {
        위해 (size_t k = 0; k < data[i].keys.size(); k++)
        {
            만약 (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                break;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(정수 i, String value)
{
    위해 (size_t j = 0; j < data.size(); j++)
    {
        만약 (data[j].number == i)
        {
            data[j].def_value = value;
            break;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    위해 (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    반환 impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    위해 (size_t i = 0; i < impl->data.size(); i++)
    {
        위해 (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            만약 (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                반환 !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    반환 impl->error == false;
}

void CommandLineParser::printErrors() const
{
    만약 (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    만약 (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    위해 (size_t i = 0; i < impl->data.size(); i++)
    {
        만약 (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    위해 (size_t i = 0; i < impl->data.size(); i++)
    {
        만약 (impl->data[i].number == -1)
        {
            printf("\t");
            위해 (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                만약 (k.length() > 1)
                {
                    printf("--");
                }
                아니면
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                만약 (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            만약 (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    위해 (size_t i = 0; i < impl->data.size(); i++)
    {
        만약 (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            만약 (dv.compare("") != 0)
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

    동안 (!str.empty())
    {
        만약 (str[0] == fs)
        {
            만약 (begin == true)
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

        만약 (str[0] == ss)
        {
            만약 (begin == false)
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

        만약 (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    만약 (begin == true)
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

    반환 vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, char symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    동안 (!str.empty())
    {
        만약 (str[0] == symbol)
        {
            만약 (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        아니면
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    만약 (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    반환 vec;
}

}
