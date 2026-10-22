// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#समावेश "precomp.hpp"
#समावेश <sstream>

namespace cv
{

namespace {
अटल स्थिर अक्षर* none_मूल्य = "<none>";

अटल String cat_string(स्थिर String& str)
{
    पूर्णांक बाँए = 0, दाँए = (पूर्णांक)str.length();
    जबतक( बाँए < दाँए && str[बाँए] == ' ' )
        बाँए++;
    जबतक( दाँए > बाँए && str[दाँए-1] == ' ' )
        दाँए--;
    वापस बाँए >= दाँए ? String("") : str.substr(बाँए, दाँए-बाँए);
}
}

काष्ठा Command_पंक्ति_Parser_Params
{
खुला:
    String help_message;
    String def_मूल्य;
    std::vector<String> कुंजियाँ;
    पूर्णांक संख्या;
};


काष्ठा Command_पंक्ति_Parser::Impl
{
    bool error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_पंक्ति_Parser_Params> तथ्य;

    std::vector<String> split_range_string(स्थिर String& str, अक्षर fs, अक्षर ss) स्थिर;
    std::vector<String> split_string(स्थिर String& str, अक्षर प्रतीक = ' ', bool create_empty_item = false) स्थिर;

    व्योम apply_params(स्थिर String& कुंजी, स्थिर String& value);
    व्योम apply_params(पूर्णांक i, String value);

    व्योम sort_params();
    पूर्णांक refcount;
};


अटल स्थिर अक्षर* get_type_name(Param type)
{
    अगर( type == Param::INT )
        वापस "int";
    अगर( type == Param::BOOLEAN )
        वापस "bool";
    अगर( type == Param::UNSIGNED_INT )
        वापस "unsigned";
    अगर( type == Param::UINT64 )
        वापस "unsigned long long";
    अगर( type == Param::FLOAT )
        वापस "float";
    अगर( type == Param::REAL )
        वापस "double";
    अगर( type == Param::STRING )
        वापस "string";
    वापस "unknown";
}

अटल bool parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    bool value = false;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    अगर (is.हार())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    वापस value;
}

अटल व्योम from_str(स्थिर String& str, Param type, व्योम* dst)
{
    std::stringstream ss(str.c_str());
    अगर( type == Param::INT )
        ss >> *(पूर्णांक*)dst;
    अन्यथा अगर( type == Param::BOOLEAN )
    {
        std::string अस्थायी_2;
        ss >> अस्थायी_2;
        *(bool*) dst = parse_bool(अस्थायी_2);
    }
    अन्यथा अगर( type == Param::UNSIGNED_INT )
        ss >> *(अचिन्हित*)dst;
    अन्यथा अगर( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    अन्यथा अगर( type == Param::FLOAT )
        ss >> *(भग्न*)dst;
    अन्यथा अगर( type == Param::REAL )
        ss >> *(द्विगुन*)dst;
    अन्यथा अगर( type == Param::STRING )
        *(String*)dst = str;
    अन्यथा अगर( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        क्रम (पूर्णांक i = 0; i < 4 && !ss.खप(); ++i)
            ss >> scalar[i];
    }
    अन्यथा
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    अगर (ss.हार())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

व्योम Command_पंक्ति_Parser::getByName(स्थिर String& name, bool space_delete, Param type, व्योम* dst) स्थिर
{
    प्रयास
    {
        क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
        {
            क्रम (माप_प्रकार j = 0; j < impl->तथ्य[i].कुंजियाँ.size(); j++)
            {
                अगर (name == impl->तथ्य[i].कुंजियाँ[j])
                {
                    String v = impl->तथ्य[i].def_मूल्य;
                    अगर (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    अगर((v.empty() && type != Param::STRING) || v == none_मूल्य) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        वापस;
                    }

                    from_str(v, type, dst);
                    वापस;
                }
            }
        }
    }
    पकड़ो (स्थिर Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        वापस;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


व्योम Command_पंक्ति_Parser::get_By_सूचकांक(पूर्णांक सूचकांक, bool space_delete, Param type, व्योम* dst) स्थिर
{
    प्रयास
    {
        क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
        {
            अगर (impl->तथ्य[i].संख्या == सूचकांक)
            {
                String v = impl->तथ्य[i].def_मूल्य;
                अगर (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                अगर((v.empty() && type != Param::STRING) || v == none_मूल्य) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", सूचकांक);
                    वापस;
                }
                from_str(v, type, dst);
                वापस;
            }
        }
    }
    पकड़ो (स्थिर Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", सूचकांक) + e.err + "\n";
        वापस;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", सूचकांक));
}

अटल bool cmp_params(स्थिर Command_पंक्ति_Parser_Params & p1, स्थिर Command_पंक्ति_Parser_Params & p2)
{
    अगर (p1.संख्या < p2.संख्या)
        वापस true;

    अगर (p1.संख्या > p2.संख्या)
        वापस false;

    वापस p1.कुंजियाँ[0].compare(p2.कुंजियाँ[0]) < 0;
}

Command_पंक्ति_Parser::Command_पंक्ति_Parser(पूर्णांक argc, स्थिर अक्षर* स्थिर argv[], स्थिर String& कुंजियाँ)
{
    impl = नया Impl;
    impl->refcount = 1;

    // path to application
    माप_प्रकार pos_s = String(argv[0]).find_last_of("/\\");
    अगर (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    अन्यथा
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = false;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(कुंजियाँ, '{', '}');

    पूर्णांक jj = 0;
    क्रम (माप_प्रकार i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        Command_पंक्ति_Parser_Params p;
        p.कुंजियाँ = impl->split_string(l[0]);
        p.def_मूल्य = l[1];
        p.help_message = cat_string(l[2]);
        p.संख्या = -1;
        अगर (p.कुंजियाँ.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        अन्यथा
        {
            अगर (p.कुंजियाँ[0][0] == '@')
            {
                p.संख्या = jj;
                jj++;
            }

            impl->तथ्य.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    क्रम (पूर्णांक i = 1; i < argc; i++)
    {
        String s(argv[i]);
        bool hasSingleDash = s.length() > 1 && s[0] == '-';

        अगर (hasSingleDash)
        {
            bool hasDoubleDash = s.length() > 2 && s[1] == '-';
            String कुंजी = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            माप_प्रकार equalsPos = कुंजी.find('=');

            अगर(equalsPos != String::npos) {
                value = कुंजी.substr(equalsPos + 1);
                कुंजी = कुंजी.substr(0, equalsPos);
            }
            impl->apply_params(कुंजी, value);
        }
        अन्यथा
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_पंक्ति_Parser::~Command_पंक्ति_Parser()
{
    अगर (CV_XADD(&impl->refcount, -1) == 1)
        मिटाओ impl;
}

Command_पंक्ति_Parser::Command_पंक्ति_Parser(स्थिर Command_पंक्ति_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_पंक्ति_Parser& Command_पंक्ति_Parser::चालक = (स्थिर Command_पंक्ति_Parser& parser)
{
    अगर( यह != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        अगर(CV_XADD(&impl->refcount, -1) == 1)
            मिटाओ impl;
        impl = parser.impl;
    }
    वापस *यह;
}

व्योम Command_पंक्ति_Parser::about(स्थिर String& message)
{
    impl->about_message = message;
}

व्योम Command_पंक्ति_Parser::Impl::apply_params(स्थिर String& कुंजी, स्थिर String& value)
{
    क्रम (माप_प्रकार i = 0; i < तथ्य.size(); i++)
    {
        क्रम (माप_प्रकार k = 0; k < तथ्य[i].कुंजियाँ.size(); k++)
        {
            अगर (कुंजी.compare(तथ्य[i].कुंजियाँ[k]) == 0)
            {
                तथ्य[i].def_मूल्य = value;
                अवरोध;
            }
        }
    }
}

व्योम Command_पंक्ति_Parser::Impl::apply_params(पूर्णांक i, String value)
{
    क्रम (माप_प्रकार j = 0; j < तथ्य.size(); j++)
    {
        अगर (तथ्य[j].संख्या == i)
        {
            तथ्य[j].def_मूल्य = value;
            अवरोध;
        }
    }
}

व्योम Command_पंक्ति_Parser::Impl::sort_params()
{
    क्रम (माप_प्रकार i = 0; i < तथ्य.size(); i++)
    {
        std::sort(तथ्य[i].कुंजियाँ.begin(), तथ्य[i].कुंजियाँ.end());
    }

    std::sort (तथ्य.begin(), तथ्य.end(), cmp_params);
}

String Command_पंक्ति_Parser::getPathToApplication() स्थिर
{
    वापस impl->path_to_app;
}

bool Command_पंक्ति_Parser::has(स्थिर String& name) स्थिर
{
    क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
    {
        क्रम (माप_प्रकार j = 0; j < impl->तथ्य[i].कुंजियाँ.size(); j++)
        {
            अगर (name == impl->तथ्य[i].कुंजियाँ[j])
            {
                स्थिर String v = cat_string(impl->तथ्य[i].def_मूल्य);
                वापस !v.empty() && v != none_मूल्य;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

bool Command_पंक्ति_Parser::check() स्थिर
{
    वापस impl->error == false;
}

व्योम Command_पंक्ति_Parser::printErrors() स्थिर
{
    अगर (impl->error)
    {
        म_लिखो("\nERRORS:\n%s\n", impl->error_message.c_str());
        ख_साफ(मानक_निकास);
    }
}

व्योम Command_पंक्ति_Parser::printMessage() स्थिर
{
    अगर (impl->about_message != "")
        म_लिखो("%s\n", impl->about_message.c_str());

    म_लिखो("Usage: %s [params] ", impl->app_name.c_str());

    क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
    {
        अगर (impl->तथ्य[i].संख्या > -1)
        {
            String name = impl->तथ्य[i].कुंजियाँ[0].substr(1, impl->तथ्य[i].कुंजियाँ[0].length() - 1);
            म_लिखो("%s ", name.c_str());
        }
    }

    म_लिखो("\n\n");

    क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
    {
        अगर (impl->तथ्य[i].संख्या == -1)
        {
            म_लिखो("\t");
            क्रम (माप_प्रकार j = 0; j < impl->तथ्य[i].कुंजियाँ.size(); j++)
            {
                String k = impl->तथ्य[i].कुंजियाँ[j];
                अगर (k.length() > 1)
                {
                    म_लिखो("--");
                }
                अन्यथा
                {
                    म_लिखो("-");
                }
                म_लिखो("%s", k.c_str());

                अगर (j != impl->तथ्य[i].कुंजियाँ.size() - 1)
                {
                    म_लिखो(", ");
                }
            }
            String dv = cat_string(impl->तथ्य[i].def_मूल्य);
            अगर (dv.compare("") != 0)
            {
                म_लिखो(" (value:%s)", dv.c_str());
            }
            म_लिखो("\n\t\t%s\n", impl->तथ्य[i].help_message.c_str());
        }
    }
    म_लिखो("\n");

    क्रम (माप_प्रकार i = 0; i < impl->तथ्य.size(); i++)
    {
        अगर (impl->तथ्य[i].संख्या != -1)
        {
            म_लिखो("\t");
            String k = impl->तथ्य[i].कुंजियाँ[0];
            k = k.substr(1, k.length() - 1);

            म_लिखो("%s", k.c_str());

            String dv = cat_string(impl->तथ्य[i].def_मूल्य);
            अगर (dv.compare("") != 0)
            {
                म_लिखो(" (value:%s)", dv.c_str());
            }
            म_लिखो("\n\t\t%s\n", impl->तथ्य[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_पंक्ति_Parser::Impl::split_range_string(स्थिर String& _str, अक्षर fs, अक्षर ss) स्थिर
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    bool begin = false;

    जबतक (!str.empty())
    {
        अगर (str[0] == fs)
        {
            अगर (begin == true)
            {
                फेंको cv::Exception(cv::Error::StsParseError,
                         String("error in split_range_string(")
                         + str
                         + String(", ")
                         + String(1, fs)
                         + String(", ")
                         + String(1, ss)
                         + String(")"),
                         "", __खाता__, __LINE__
                         );
            }
            begin = true;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        अगर (str[0] == ss)
        {
            अगर (begin == false)
            {
                फेंको cv::Exception(cv::Error::StsParseError,
                         String("error in split_range_string(")
                         + str
                         + String(", ")
                         + String(1, fs)
                         + String(", ")
                         + String(1, ss)
                         + String(")"),
                         "", __खाता__, __LINE__
                         );
            }
            begin = false;
            vec.push_back(word);
        }

        अगर (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    अगर (begin == true)
    {
        फेंको cv::Exception(cv::Error::StsParseError,
                 String("error in split_range_string(")
                 + str
                 + String(", ")
                 + String(1, fs)
                 + String(", ")
                 + String(1, ss)
                 + String(")"),
                 "", __खाता__, __LINE__
                );
    }

    वापस vec;
}

std::vector<String> Command_पंक्ति_Parser::Impl::split_string(स्थिर String& _str, अक्षर प्रतीक, bool create_empty_item) स्थिर
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    जबतक (!str.empty())
    {
        अगर (str[0] == प्रतीक)
        {
            अगर (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        अन्यथा
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    अगर (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    वापस vec;
}

}
