// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "precomp.hpp"
#include <sstream>

namespace cv
{

namespace {
static const char* none_हालागा = "<none>";

static String cat_string(const String& str)
{
    int कालिवा = 0, कानान = (int)str.length();
    while( कालिवा < कानान && str[कालिवा] == ' ' )
        कालिवा++;
    while( कानान > कालिवा && str[कानान-1] == ' ' )
        कानान--;
    return कालिवा >= कानान ? String("") : str.substr(कालिवा, कानान-कालिवा);
}
}

struct Command_लिन्या_Parser_Params
{
public:
    String help_message;
    String def_हालागा;
    std::vector<String> मगा_सुसि;
    int नुमेरो;
};


struct Command_लिन्या_Parser::Impl
{
    bool error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_लिन्या_Parser_Params> data;

    std::vector<String> split_range_string(const String& str, char fs, char ss) const;
    std::vector<String> split_string(const String& str, char सिम्बोलो = ' ', bool create_empty_item = false) const;

    void apply_params(const String& सुसि, const String& value);
    void apply_params(int i, String value);

    void sort_params();
    int refcount;
};


static const char* get_type_name(Param type)
{
    if( type == Param::INT )
        return "int";
    if( type == Param::BOOLEAN )
        return "bool";
    if( type == Param::UNSIGNED_INT )
        return "unsigned";
    if( type == Param::UINT64 )
        return "unsigned long long";
    if( type == Param::FLOAT )
        return "float";
    if( type == Param::REAL )
        return "double";
    if( type == Param::STRING )
        return "string";
    return "unknown";
}

static bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    if (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    return value;
}

static void from_str(const String& str, Param type, void* dst)
{
    std::stringstream ss(str.c_str());
    if( type == Param::INT )
        ss >> *(int*)dst;
    else if( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(bool*) dst = parse_bool(temp);
    }
    else if( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    else if( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    else if( type == Param::FLOAT )
        ss >> *(float*)dst;
    else if( type == Param::REAL )
        ss >> *(double*)dst;
    else if( type == Param::STRING )
        *(String*)dst = str;
    else if( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        for (int i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    else
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    if (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

void Command_लिन्या_Parser::getByName(const String& name, bool space_delete, Param type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            for (size_t j = 0; j < impl->data[i].मगा_सुसि.size(); j++)
            {
                if (name == impl->data[i].मगा_सुसि[j])
                {
                    String v = impl->data[i].def_हालागा;
                    if (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    if((v.empty() && type != Param::STRING) || v == none_हालागा) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        return;
                    }

                    from_str(v, type, dst);
                    return;
                }
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        return;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


void Command_लिन्या_Parser::getByIndex(int index, bool space_delete, Param type, void* dst) const
{
    try
    {
        for (size_t i = 0; i < impl->data.size(); i++)
        {
            if (impl->data[i].नुमेरो == index)
            {
                String v = impl->data[i].def_हालागा;
                if (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                if((v.empty() && type != Param::STRING) || v == none_हालागा) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    return;
                }
                from_str(v, type, dst);
                return;
            }
        }
    }
    catch (const Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        return;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

static bool cmp_params(const Command_लिन्या_Parser_Params & p1, const Command_लिन्या_Parser_Params & p2)
{
    if (p1.नुमेरो < p2.नुमेरो)
        return true;

    if (p1.नुमेरो > p2.नुमेरो)
        return false;

    return p1.मगा_सुसि[0].compare(p2.मगा_सुसि[0]) < 0;
}

Command_लिन्या_Parser::Command_लिन्या_Parser(int argc, const char* const argv[], const String& मगा_सुसि)
{
    impl = new Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    if (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    else
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(मगा_सुसि, '{', '}');

    int jj = 0;
    for (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        Command_लिन्या_Parser_Params p;
        p.मगा_सुसि = impl->split_string(l[0]);
        p.def_हालागा = l[1];
        p.help_message = cat_string(l[2]);
        p.नुमेरो = -1;
        if (p.मगा_सुसि.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        else
        {
            if (p.मगा_सुसि[0][0] == '@')
            {
                p.नुमेरो = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    for (int i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        if (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String सुसि = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = सुसि.find('=');

            if(equalsPos != String::npos) {
                value = सुसि.substr(equalsPos + 1);
                सुसि = सुसि.substr(0, equalsPos);
            }
            impl->apply_params(सुसि, value);
        }
        else
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_लिन्या_Parser::~Command_लिन्या_Parser()
{
    if (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

Command_लिन्या_Parser::Command_लिन्या_Parser(const Command_लिन्या_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_लिन्या_Parser& Command_लिन्या_Parser::operator = (const Command_लिन्या_Parser& parser)
{
    if( this != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        if(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    return *this;
}

void Command_लिन्या_Parser::about(const String& message)
{
    impl->about_message = message;
}

void Command_लिन्या_Parser::Impl::apply_params(const String& सुसि, const String& value)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t k = 0; k < data[i].मगा_सुसि.size(); k++)
        {
            if (सुसि.compare(data[i].मगा_सुसि[k]) == 0)
            {
                data[i].def_हालागा = value;
                break;
            }
        }
    }
}

void Command_लिन्या_Parser::Impl::apply_params(int i, String value)
{
    for (size_t j = 0; j < data.size(); j++)
    {
        if (data[j].नुमेरो == i)
        {
            data[j].def_हालागा = value;
            break;
        }
    }
}

void Command_लिन्या_Parser::Impl::sort_params()
{
    for (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].मगा_सुसि.begin(), data[i].मगा_सुसि.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String Command_लिन्या_Parser::getPathToApplication() const
{
    return impl->path_to_app;
}

bool Command_लिन्या_Parser::has(const String& name) const
{
    for (size_t i = 0; i < impl->data.size(); i++)
    {
        for (size_t j = 0; j < impl->data[i].मगा_सुसि.size(); j++)
        {
            if (name == impl->data[i].मगा_सुसि[j])
            {
                const String v = cat_string(impl->data[i].def_हालागा);
                return !v.empty() && v != none_हालागा;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool Command_लिन्या_Parser::check() const
{
    return impl->error == false;
}

void Command_लिन्या_Parser::इलिम्बाग_Errors() const
{
    if (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

void Command_लिन्या_Parser::इलिम्बाग_Message() const
{
    if (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].नुमेरो > -1)
        {
            String name = impl->data[i].मगा_सुसि[0].substr(1, impl->data[i].मगा_सुसि[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].नुमेरो == -1)
        {
            printf("\t");
            for (size_t j = 0; j < impl->data[i].मगा_सुसि.size(); j++)
            {
                String k = impl->data[i].मगा_सुसि[j];
                if (k.length() > 1)
                {
                    printf("--");
                }
                else
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                if (j != impl->data[i].मगा_सुसि.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_हालागा);
            if (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    for (size_t i = 0; i < impl->data.size(); i++)
    {
        if (impl->data[i].नुमेरो != -1)
        {
            printf("\t");
            String k = impl->data[i].मगा_सुसि[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_हालागा);
            if (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_लिन्या_Parser::Impl::split_range_string(const String& _str, char fs, char ss) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    bool begin = false;

    while (!str.empty())
    {
        if (str[0] == fs)
        {
            if (begin == true)
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

        if (str[0] == ss)
        {
            if (begin == false)
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

        if (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    if (begin == true)
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

    return vec;
}

std::vector<String> Command_लिन्या_Parser::Impl::split_string(const String& _str, char सिम्बोलो, bool create_empty_item) const
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    while (!str.empty())
    {
        if (str[0] == सिम्बोलो)
        {
            if (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        else
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    if (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    return vec;
}

}
