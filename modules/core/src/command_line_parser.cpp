// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <sstream>

नामाकाश cv
{

नामाकाश {
स्थिर स्थिरांक अक्षर* none_मूल्य = "<none>";

स्थिर String cat_string(स्थिरांक String& str)
{
    पूर्णांक डावा = 0, उजवा = (पूर्णांक)str.length();
    पर्यंत( डावा < उजवा && str[डावा] == ' ' )
        डावा++;
    पर्यंत( उजवा > डावा && str[उजवा-1] == ' ' )
        उजवा--;
    परत डावा >= उजवा ? String("") : str.substr(डावा, उजवा-डावा);
}
}

संरचना Command_ओळ_Parser_Params
{
सार्वजनिक:
    String help_message;
    String def_मूल्य;
    std::vector<String> किल्ल्या;
    पूर्णांक संख्या;
};


संरचना Command_ओळ_Parser::Impl
{
    तार्किक error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_ओळ_Parser_Params> data;

    std::vector<String> split_range_string(स्थिरांक String& str, अक्षर fs, अक्षर ss) स्थिरांक;
    std::vector<String> split_string(स्थिरांक String& str, अक्षर चिन्ह = ' ', तार्किक create_empty_item = असत्य) स्थिरांक;

    शून्य apply_params(स्थिरांक String& किल्ली, स्थिरांक String& value);
    शून्य apply_params(पूर्णांक i, String value);

    शून्य sort_params();
    पूर्णांक refcount;
};


स्थिर स्थिरांक अक्षर* get_type_name(Param type)
{
    जर( type == Param::INT )
        परत "int";
    जर( type == Param::BOOLEAN )
        परत "bool";
    जर( type == Param::UNSIGNED_INT )
        परत "unsigned";
    जर( type == Param::UINT64 )
        परत "unsigned long long";
    जर( type == Param::FLOAT )
        परत "float";
    जर( type == Param::REAL )
        परत "double";
    जर( type == Param::STRING )
        परत "string";
    परत "unknown";
}

स्थिर तार्किक parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    तार्किक value = असत्य;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    जर (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    परत value;
}

स्थिर शून्य from_str(स्थिरांक String& str, Param type, शून्य* dst)
{
    std::stringstream ss(str.c_str());
    जर( type == Param::INT )
        ss >> *(पूर्णांक*)dst;
    अन्यथा जर( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(तार्किक*) dst = parse_bool(temp);
    }
    अन्यथा जर( type == Param::UNSIGNED_INT )
        ss >> *(अचिन्हित*)dst;
    अन्यथा जर( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    अन्यथा जर( type == Param::FLOAT )
        ss >> *(दशांश*)dst;
    अन्यथा जर( type == Param::REAL )
        ss >> *(द्विगुण*)dst;
    अन्यथा जर( type == Param::STRING )
        *(String*)dst = str;
    अन्यथा जर( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        साठी (पूर्णांक i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    अन्यथा
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    जर (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

शून्य Command_ओळ_Parser::getByName(स्थिरांक String& name, तार्किक space_delete, Param type, शून्य* dst) स्थिरांक
{
    प्रयत्न
    {
        साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
        {
            साठी (माप_प्रकार j = 0; j < impl->data[i].किल्ल्या.size(); j++)
            {
                जर (name == impl->data[i].किल्ल्या[j])
                {
                    String v = impl->data[i].def_मूल्य;
                    जर (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    जर((v.empty() && type != Param::STRING) || v == none_मूल्य) {
                        impl->error = सत्य;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        परत;
                    }

                    from_str(v, type, dst);
                    परत;
                }
            }
        }
    }
    पकडा (स्थिरांक Exception& e)
    {
        impl->error = सत्य;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        परत;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


शून्य Command_ओळ_Parser::getByIndex(पूर्णांक index, तार्किक space_delete, Param type, शून्य* dst) स्थिरांक
{
    प्रयत्न
    {
        साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
        {
            जर (impl->data[i].संख्या == index)
            {
                String v = impl->data[i].def_मूल्य;
                जर (space_delete == सत्य) v = cat_string(v);

                // the key was neither specified nor has a default value
                जर((v.empty() && type != Param::STRING) || v == none_मूल्य) {
                    impl->error = सत्य;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    परत;
                }
                from_str(v, type, dst);
                परत;
            }
        }
    }
    पकडा (स्थिरांक Exception& e)
    {
        impl->error = सत्य;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        परत;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

स्थिर तार्किक cmp_params(स्थिरांक Command_ओळ_Parser_Params & p1, स्थिरांक Command_ओळ_Parser_Params & p2)
{
    जर (p1.संख्या < p2.संख्या)
        परत सत्य;

    जर (p1.संख्या > p2.संख्या)
        परत असत्य;

    परत p1.किल्ल्या[0].compare(p2.किल्ल्या[0]) < 0;
}

Command_ओळ_Parser::Command_ओळ_Parser(पूर्णांक argc, स्थिरांक अक्षर* स्थिरांक argv[], स्थिरांक String& किल्ल्या)
{
    impl = नवीन Impl;
    impl->refcount = 1;

    // path to application
    माप_प्रकार pos_s = String(argv[0]).find_last_of("/\\");
    जर (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    अन्यथा
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = असत्य;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(किल्ल्या, '{', '}');

    पूर्णांक jj = 0;
    साठी (माप_प्रकार i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', सत्य);
        Command_ओळ_Parser_Params p;
        p.किल्ल्या = impl->split_string(l[0]);
        p.def_मूल्य = l[1];
        p.help_message = cat_string(l[2]);
        p.संख्या = -1;
        जर (p.किल्ल्या.size() <= 0)
        {
            impl->error = सत्य;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        अन्यथा
        {
            जर (p.किल्ल्या[0][0] == '@')
            {
                p.संख्या = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    साठी (पूर्णांक i = 1; i < argc; i++)
    {
        String s(argv[i]);
        तार्किक hasSingleDash = s.length() > 1 && s[0] == '-';

        जर (hasSingleDash)
        {
            तार्किक hasDoubleDash = s.length() > 2 && s[1] == '-';
            String किल्ली = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            माप_प्रकार equalsPos = किल्ली.find('=');

            जर(equalsPos != String::npos) {
                value = किल्ली.substr(equalsPos + 1);
                किल्ली = किल्ली.substr(0, equalsPos);
            }
            impl->apply_params(किल्ली, value);
        }
        अन्यथा
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_ओळ_Parser::~Command_ओळ_Parser()
{
    जर (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

Command_ओळ_Parser::Command_ओळ_Parser(स्थिरांक Command_ओळ_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_ओळ_Parser& Command_ओळ_Parser::चालक = (स्थिरांक Command_ओळ_Parser& parser)
{
    जर( हे != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        जर(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    परत *हे;
}

शून्य Command_ओळ_Parser::about(स्थिरांक String& message)
{
    impl->about_message = message;
}

शून्य Command_ओळ_Parser::Impl::apply_params(स्थिरांक String& किल्ली, स्थिरांक String& value)
{
    साठी (माप_प्रकार i = 0; i < data.size(); i++)
    {
        साठी (माप_प्रकार k = 0; k < data[i].किल्ल्या.size(); k++)
        {
            जर (किल्ली.compare(data[i].किल्ल्या[k]) == 0)
            {
                data[i].def_मूल्य = value;
                थांबा;
            }
        }
    }
}

शून्य Command_ओळ_Parser::Impl::apply_params(पूर्णांक i, String value)
{
    साठी (माप_प्रकार j = 0; j < data.size(); j++)
    {
        जर (data[j].संख्या == i)
        {
            data[j].def_मूल्य = value;
            थांबा;
        }
    }
}

शून्य Command_ओळ_Parser::Impl::sort_params()
{
    साठी (माप_प्रकार i = 0; i < data.size(); i++)
    {
        std::sort(data[i].किल्ल्या.begin(), data[i].किल्ल्या.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String Command_ओळ_Parser::getPathToApplication() स्थिरांक
{
    परत impl->path_to_app;
}

तार्किक Command_ओळ_Parser::has(स्थिरांक String& name) स्थिरांक
{
    साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
    {
        साठी (माप_प्रकार j = 0; j < impl->data[i].किल्ल्या.size(); j++)
        {
            जर (name == impl->data[i].किल्ल्या[j])
            {
                स्थिरांक String v = cat_string(impl->data[i].def_मूल्य);
                परत !v.empty() && v != none_मूल्य;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

तार्किक Command_ओळ_Parser::check() स्थिरांक
{
    परत impl->error == असत्य;
}

शून्य Command_ओळ_Parser::printErrors() स्थिरांक
{
    जर (impl->error)
    {
        म_लिखो("\nERRORS:\n%s\n", impl->error_message.c_str());
        ख_साफ(मानक_निकास);
    }
}

शून्य Command_ओळ_Parser::printMessage() स्थिरांक
{
    जर (impl->about_message != "")
        म_लिखो("%s\n", impl->about_message.c_str());

    म_लिखो("Usage: %s [params] ", impl->app_name.c_str());

    साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
    {
        जर (impl->data[i].संख्या > -1)
        {
            String name = impl->data[i].किल्ल्या[0].substr(1, impl->data[i].किल्ल्या[0].length() - 1);
            म_लिखो("%s ", name.c_str());
        }
    }

    म_लिखो("\n\n");

    साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
    {
        जर (impl->data[i].संख्या == -1)
        {
            म_लिखो("\t");
            साठी (माप_प्रकार j = 0; j < impl->data[i].किल्ल्या.size(); j++)
            {
                String k = impl->data[i].किल्ल्या[j];
                जर (k.length() > 1)
                {
                    म_लिखो("--");
                }
                अन्यथा
                {
                    म_लिखो("-");
                }
                म_लिखो("%s", k.c_str());

                जर (j != impl->data[i].किल्ल्या.size() - 1)
                {
                    म_लिखो(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_मूल्य);
            जर (dv.compare("") != 0)
            {
                म_लिखो(" (value:%s)", dv.c_str());
            }
            म_लिखो("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    म_लिखो("\n");

    साठी (माप_प्रकार i = 0; i < impl->data.size(); i++)
    {
        जर (impl->data[i].संख्या != -1)
        {
            म_लिखो("\t");
            String k = impl->data[i].किल्ल्या[0];
            k = k.substr(1, k.length() - 1);

            म_लिखो("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_मूल्य);
            जर (dv.compare("") != 0)
            {
                म_लिखो(" (value:%s)", dv.c_str());
            }
            म_लिखो("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_ओळ_Parser::Impl::split_range_string(स्थिरांक String& _str, अक्षर fs, अक्षर ss) स्थिरांक
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    तार्किक begin = असत्य;

    पर्यंत (!str.empty())
    {
        जर (str[0] == fs)
        {
            जर (begin == सत्य)
            {
                फेका cv::Exception(cv::Error::StsParseError,
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
            begin = सत्य;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        जर (str[0] == ss)
        {
            जर (begin == असत्य)
            {
                फेका cv::Exception(cv::Error::StsParseError,
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
            begin = असत्य;
            vec.push_back(word);
        }

        जर (begin == सत्य)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    जर (begin == सत्य)
    {
        फेका cv::Exception(cv::Error::StsParseError,
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

    परत vec;
}

std::vector<String> Command_ओळ_Parser::Impl::split_string(स्थिरांक String& _str, अक्षर चिन्ह, तार्किक create_empty_item) स्थिरांक
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    पर्यंत (!str.empty())
    {
        जर (str[0] == चिन्ह)
        {
            जर (!word.empty() || create_empty_item)
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

    जर (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    परत vec;
}

}
