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
    ຈຳນວນເຕັມ left = 0, right = (ຈຳນວນເຕັມ)str.length();
    ໃນຂະນະທີ່( left < right && str[left] == ' ' )
        left++;
    ໃນຂະນະທີ່( right > left && str[right-1] == ' ' )
        right--;
    ສົ່ງຄືນ left >= right ? String("") : str.substr(left, right-left);
}
}

sa_nracanaa CommandLineParserParams
{
public:
    String help_message;
    String def_value;
    std::vector<String> keys;
    ຈຳນວນເຕັມ number;
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
    void apply_params(ຈຳນວນເຕັມ i, String value);

    void sort_params();
    ຈຳນວນເຕັມ refcount;
};


static const var_na* get_type_name(Param type)
{
    ຖ້າ( type == Param::INT )
        ສົ່ງຄືນ "int";
    ຖ້າ( type == Param::BOOLEAN )
        ສົ່ງຄືນ "bool";
    ຖ້າ( type == Param::UNSIGNED_INT )
        ສົ່ງຄືນ "unsigned";
    ຖ້າ( type == Param::UINT64 )
        ສົ່ງຄືນ "unsigned long long";
    ຖ້າ( type == Param::FLOAT )
        ສົ່ງຄືນ "float";
    ຖ້າ( type == Param::REAL )
        ສົ່ງຄືນ "double";
    ຖ້າ( type == Param::STRING )
        ສົ່ງຄືນ "string";
    ສົ່ງຄືນ "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ຖ້າ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ສົ່ງຄືນ value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    ຖ້າ( type == Param::INT )
        ss >> *(ຈຳນວນເຕັມ*)dst;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::FLOAT )
        ss >> *(float*)dst;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::REAL )
        ss >> *(dviguu_na*)dst;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::STRING )
        *(String*)dst = str;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ຖ້າ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void CommandLineParser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
        {
            ສຳລັບ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                ຖ້າ (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    ຖ້າ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ຖ້າ((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ສົ່ງຄືນ;
                    }

                    from_str(v, type, dst);
                    ສົ່ງຄືນ;
                }
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ສົ່ງຄືນ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void CommandLineParser::getByIndex(ຈຳນວນເຕັມ index, bool space_delete, Param type, void* dst) const
{
    try
    {
        ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
        {
            ຖ້າ (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                ຖ້າ (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                ຖ້າ((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ສົ່ງຄືນ;
                }
                from_str(v, type, dst);
                ສົ່ງຄືນ;
            }
        }
    }
    gr_haa_na (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ສົ່ງຄືນ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const CommandLineParserParams & p1, const CommandLineParserParams & p2)
{
    ຖ້າ (p1.number < p2.number)
        ສົ່ງຄືນ true;

    ຖ້າ (p1.number > p2.number)
        ສົ່ງຄືນ false;

    ສົ່ງຄືນ p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(ຈຳນວນເຕັມ argc, const var_na* const argv[], const String& keys)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ຖ້າ (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    ຈຳນວນເຕັມ jj = 0;
    ສຳລັບ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        ຖ້າ (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ຖ້າ (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        ຖ້າ (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            ຖ້າ(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    ຖ້າ (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(const CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (const CommandLineParser& parser)
{
    ຖ້າ( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ຖ້າ(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ສົ່ງຄືນ *this;
}

void CommandLineParser::about(const String& message)
{
    impl->about_message = message;
}

void CommandLineParser::Impl::apply_params(const String& key, const String& value)
{
    ສຳລັບ (size_t i = 0; i < data.size(); i++)
    {
        ສຳລັບ (size_t k = 0; k < data[i].keys.size(); k++)
        {
            ຖ້າ (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                bha_nga;
            }
        }
    }
}

void CommandLineParser::Impl::apply_params(ຈຳນວນເຕັມ i, String value)
{
    ສຳລັບ (size_t j = 0; j < data.size(); j++)
    {
        ຖ້າ (data[j].number == i)
        {
            data[j].def_value = value;
            bha_nga;
        }
    }
}

void CommandLineParser::Impl::sort_params()
{
    ສຳລັບ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() const
{
    ສົ່ງຄືນ impl->path_to_app;
}

bool CommandLineParser::has(const String& name) const
{
    ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
    {
        ສຳລັບ (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            ຖ້າ (name == impl->data[i].keys[j])
            {
                const String v = cat_string(impl->data[i].def_value);
                ສົ່ງຄືນ !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool CommandLineParser::check() const
{
    ສົ່ງຄືນ impl->error == false;
}

void CommandLineParser::printErrors() const
{
    ຖ້າ (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void CommandLineParser::printMessage() const
{
    ຖ້າ (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
    {
        ຖ້າ (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
    {
        ຖ້າ (impl->data[i].number == -1)
        {
            printf("\t");
            ສຳລັບ (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                ຖ້າ (k.length() > 1)
                {
                    printf("--");
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ຖ້າ (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            ຖ້າ (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    ສຳລັບ (size_t i = 0; i < impl->data.size(); i++)
    {
        ຖ້າ (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            ຖ້າ (dv.compare("") != 0)
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

    ໃນຂະນະທີ່ (!str.empty())
    {
        ຖ້າ (str[0] == fs)
        {
            ຖ້າ (begin == true)
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

        ຖ້າ (str[0] == ss)
        {
            ຖ້າ (begin == false)
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

        ຖ້າ (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ຖ້າ (begin == true)
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

    ສົ່ງຄືນ vec;
}

std::vector<String> CommandLineParser::Impl::split_string(const String& _str, var_na symbol, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ໃນຂະນະທີ່ (!str.empty())
    {
        ຖ້າ (str[0] == symbol)
        {
            ຖ້າ (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ຖ້າ (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ສົ່ງຄືນ vec;
}

}
