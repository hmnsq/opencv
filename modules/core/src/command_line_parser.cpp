// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "precomp.hpp"
#ضمن <sstream>

نطاق cv
{

نطاق {
ساكن ثابت حرف* none_قيمة = "<none>";

ساكن String cat_string(ثابت String& str)
{
    صحيح يسار = 0, يمين = (صحيح)str.length();
    بينما( يسار < يمين && str[يسار] == ' ' )
        يسار++;
    بينما( يمين > يسار && str[يمين-1] == ' ' )
        يمين--;
    إرجاع يسار >= يمين ? String("") : str.substr(يسار, يمين-يسار);
}
}

بنية Command_سطر_Parser_Params
{
عام:
    String help_message;
    String def_قيمة;
    std::vector<String> مفاتيح;
    صحيح العدد;
};


بنية Command_سطر_Parser::Impl
{
    منطقي error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_سطر_Parser_Params> data;

    std::vector<String> split_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> split_string(ثابت String& str, حرف رمز = ' ', منطقي create_empty_item = خطأ) ثابت;

    فراغ apply_params(ثابت String& مفتاح, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ sort_params();
    صحيح refcount;
};


ساكن ثابت حرف* get_type_name(Param type)
{
    إذا( type == Param::INT )
        إرجاع "int";
    إذا( type == Param::BOOLEAN )
        إرجاع "bool";
    إذا( type == Param::UNSIGNED_INT )
        إرجاع "unsigned";
    إذا( type == Param::UINT64 )
        إرجاع "unsigned long long";
    إذا( type == Param::FLOAT )
        إرجاع "float";
    إذا( type == Param::REAL )
        إرجاع "double";
    إذا( type == Param::STRING )
        إرجاع "string";
    إرجاع "unknown";
}

ساكن منطقي parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقي value = خطأ;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    إذا (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    إرجاع value;
}

ساكن فراغ from_str(ثابت String& str, Param type, فراغ* dst)
{
    std::stringstream ss(str.c_str());
    إذا( type == Param::INT )
        ss >> *(صحيح*)dst;
    وإلا إذا( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(منطقي*) dst = parse_bool(temp);
    }
    وإلا إذا( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    وإلا إذا( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    وإلا إذا( type == Param::FLOAT )
        ss >> *(عائم*)dst;
    وإلا إذا( type == Param::REAL )
        ss >> *(مزدوج*)dst;
    وإلا إذا( type == Param::STRING )
        *(String*)dst = str;
    وإلا إذا( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        لكل (صحيح i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    وإلا
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    إذا (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

فراغ Command_سطر_Parser::getByName(ثابت String& name, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->data.size(); i++)
        {
            لكل (size_t j = 0; j < impl->data[i].مفاتيح.size(); j++)
            {
                إذا (name == impl->data[i].مفاتيح[j])
                {
                    String v = impl->data[i].def_قيمة;
                    إذا (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    إذا((v.empty() && type != Param::STRING) || v == none_قيمة) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        إرجاع;
                    }

                    from_str(v, type, dst);
                    إرجاع;
                }
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        إرجاع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ Command_سطر_Parser::getByIndex(صحيح index, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->data.size(); i++)
        {
            إذا (impl->data[i].العدد == index)
            {
                String v = impl->data[i].def_قيمة;
                إذا (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                إذا((v.empty() && type != Param::STRING) || v == none_قيمة) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    إرجاع;
                }
                from_str(v, type, dst);
                إرجاع;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        إرجاع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

ساكن منطقي cmp_params(ثابت Command_سطر_Parser_Params & p1, ثابت Command_سطر_Parser_Params & p2)
{
    إذا (p1.العدد < p2.العدد)
        إرجاع true;

    إذا (p1.العدد > p2.العدد)
        إرجاع خطأ;

    إرجاع p1.مفاتيح[0].compare(p2.مفاتيح[0]) < 0;
}

Command_سطر_Parser::Command_سطر_Parser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& مفاتيح)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    إذا (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    وإلا
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = خطأ;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(مفاتيح, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        Command_سطر_Parser_Params p;
        p.مفاتيح = impl->split_string(l[0]);
        p.def_قيمة = l[1];
        p.help_message = cat_string(l[2]);
        p.العدد = -1;
        إذا (p.مفاتيح.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            إذا (p.مفاتيح[0][0] == '@')
            {
                p.العدد = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    لكل (صحيح i = 1; i < argc; i++)
    {
        String s(argv[i]);
        منطقي hasSingleDash = s.length() > 1 && s[0] == '-';

        إذا (hasSingleDash)
        {
            منطقي hasDoubleDash = s.length() > 2 && s[1] == '-';
            String مفتاح = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = مفتاح.find('=');

            إذا(equalsPos != String::npos) {
                value = مفتاح.substr(equalsPos + 1);
                مفتاح = مفتاح.substr(0, equalsPos);
            }
            impl->apply_params(مفتاح, value);
        }
        وإلا
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_سطر_Parser::~Command_سطر_Parser()
{
    إذا (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

Command_سطر_Parser::Command_سطر_Parser(ثابت Command_سطر_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سطر_Parser& Command_سطر_Parser::operator = (ثابت Command_سطر_Parser& parser)
{
    إذا( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        إذا(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    إرجاع *هذا;
}

فراغ Command_سطر_Parser::about(ثابت String& message)
{
    impl->about_message = message;
}

فراغ Command_سطر_Parser::Impl::apply_params(ثابت String& مفتاح, ثابت String& value)
{
    لكل (size_t i = 0; i < data.size(); i++)
    {
        لكل (size_t k = 0; k < data[i].مفاتيح.size(); k++)
        {
            إذا (مفتاح.compare(data[i].مفاتيح[k]) == 0)
            {
                data[i].def_قيمة = value;
                اكسر;
            }
        }
    }
}

فراغ Command_سطر_Parser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < data.size(); j++)
    {
        إذا (data[j].العدد == i)
        {
            data[j].def_قيمة = value;
            اكسر;
        }
    }
}

فراغ Command_سطر_Parser::Impl::sort_params()
{
    لكل (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].مفاتيح.begin(), data[i].مفاتيح.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String Command_سطر_Parser::getPathToApplication() ثابت
{
    إرجاع impl->path_to_app;
}

منطقي Command_سطر_Parser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        لكل (size_t j = 0; j < impl->data[i].مفاتيح.size(); j++)
        {
            إذا (name == impl->data[i].مفاتيح[j])
            {
                ثابت String v = cat_string(impl->data[i].def_قيمة);
                إرجاع !v.empty() && v != none_قيمة;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي Command_سطر_Parser::check() ثابت
{
    إرجاع impl->error == خطأ;
}

فراغ Command_سطر_Parser::printErrors() ثابت
{
    إذا (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

فراغ Command_سطر_Parser::printMessage() ثابت
{
    إذا (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        إذا (impl->data[i].العدد > -1)
        {
            String name = impl->data[i].مفاتيح[0].substr(1, impl->data[i].مفاتيح[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        إذا (impl->data[i].العدد == -1)
        {
            printf("\t");
            لكل (size_t j = 0; j < impl->data[i].مفاتيح.size(); j++)
            {
                String k = impl->data[i].مفاتيح[j];
                إذا (k.length() > 1)
                {
                    printf("--");
                }
                وإلا
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                إذا (j != impl->data[i].مفاتيح.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_قيمة);
            إذا (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        إذا (impl->data[i].العدد != -1)
        {
            printf("\t");
            String k = impl->data[i].مفاتيح[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_قيمة);
            إذا (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_سطر_Parser::Impl::split_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    منطقي begin = خطأ;

    بينما (!str.empty())
    {
        إذا (str[0] == fs)
        {
            إذا (begin == true)
            {
                ارم cv::Exception(cv::Error::StsParseError,
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

        إذا (str[0] == ss)
        {
            إذا (begin == خطأ)
            {
                ارم cv::Exception(cv::Error::StsParseError,
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
            begin = خطأ;
            vec.push_back(word);
        }

        إذا (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (begin == true)
    {
        ارم cv::Exception(cv::Error::StsParseError,
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

    إرجاع vec;
}

std::vector<String> Command_سطر_Parser::Impl::split_string(ثابت String& _str, حرف رمز, منطقي create_empty_item) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    بينما (!str.empty())
    {
        إذا (str[0] == رمز)
        {
            إذا (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        وإلا
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    إرجاع vec;
}

}
