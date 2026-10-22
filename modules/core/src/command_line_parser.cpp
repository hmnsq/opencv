// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "precomp.hpp"
#شامل <sstream>

فضاینام cv
{

فضاینام {
ایستا ثابت نویسه* none_مقدار = "<none>";

ایستا String cat_string(ثابت String& str)
{
    صحیح چپ = 0, راست = (صحیح)str.length();
    وقتی( چپ < راست && str[چپ] == ' ' )
        چپ++;
    وقتی( راست > چپ && str[راست-1] == ' ' )
        راست--;
    بازگشت چپ >= راست ? String("") : str.substr(چپ, راست-چپ);
}
}

ساختار Command_سطر_Parser_Params
{
عمومی:
    String help_message;
    String def_مقدار;
    std::vector<String> کلیدها;
    صحیح عدد;
};


ساختار Command_سطر_Parser::Impl
{
    منطقی error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_سطر_Parser_Params> data;

    std::vector<String> split_range_string(ثابت String& str, نویسه fs, نویسه ss) ثابت;
    std::vector<String> split_string(ثابت String& str, نویسه نماد = ' ', منطقی create_empty_item = نادرست) ثابت;

    تهی apply_params(ثابت String& کلید, ثابت String& value);
    تهی apply_params(صحیح i, String value);

    تهی sort_params();
    صحیح refcount;
};


ایستا ثابت نویسه* get_type_name(Param type)
{
    اگر( type == Param::INT )
        بازگشت "int";
    اگر( type == Param::BOOLEAN )
        بازگشت "bool";
    اگر( type == Param::UNSIGNED_INT )
        بازگشت "unsigned";
    اگر( type == Param::UINT64 )
        بازگشت "unsigned long long";
    اگر( type == Param::FLOAT )
        بازگشت "float";
    اگر( type == Param::REAL )
        بازگشت "double";
    اگر( type == Param::STRING )
        بازگشت "string";
    بازگشت "unknown";
}

ایستا منطقی parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقی value = نادرست;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    اگر (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    بازگشت value;
}

ایستا تهی from_str(ثابت String& str, Param type, تهی* dst)
{
    std::stringstream ss(str.c_str());
    اگر( type == Param::INT )
        ss >> *(صحیح*)dst;
    وگرنه اگر( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(منطقی*) dst = parse_bool(temp);
    }
    وگرنه اگر( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    وگرنه اگر( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    وگرنه اگر( type == Param::FLOAT )
        ss >> *(اعشاری*)dst;
    وگرنه اگر( type == Param::REAL )
        ss >> *(دوگانه*)dst;
    وگرنه اگر( type == Param::STRING )
        *(String*)dst = str;
    وگرنه اگر( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        برای (صحیح i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    وگرنه
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    اگر (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

تهی Command_سطر_Parser::getByName(ثابت String& name, منطقی space_delete, Param type, تهی* dst) ثابت
{
    تلاش
    {
        برای (size_t i = 0; i < impl->data.size(); i++)
        {
            برای (size_t j = 0; j < impl->data[i].کلیدها.size(); j++)
            {
                اگر (name == impl->data[i].کلیدها[j])
                {
                    String v = impl->data[i].def_مقدار;
                    اگر (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    اگر((v.empty() && type != Param::STRING) || v == none_مقدار) {
                        impl->error = درست;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        بازگشت;
                    }

                    from_str(v, type, dst);
                    بازگشت;
                }
            }
        }
    }
    بگیر (ثابت Exception& e)
    {
        impl->error = درست;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        بازگشت;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


تهی Command_سطر_Parser::getByIndex(صحیح index, منطقی space_delete, Param type, تهی* dst) ثابت
{
    تلاش
    {
        برای (size_t i = 0; i < impl->data.size(); i++)
        {
            اگر (impl->data[i].عدد == index)
            {
                String v = impl->data[i].def_مقدار;
                اگر (space_delete == درست) v = cat_string(v);

                // the key was neither specified nor has a default value
                اگر((v.empty() && type != Param::STRING) || v == none_مقدار) {
                    impl->error = درست;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    بازگشت;
                }
                from_str(v, type, dst);
                بازگشت;
            }
        }
    }
    بگیر (ثابت Exception& e)
    {
        impl->error = درست;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        بازگشت;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

ایستا منطقی cmp_params(ثابت Command_سطر_Parser_Params & p1, ثابت Command_سطر_Parser_Params & p2)
{
    اگر (p1.عدد < p2.عدد)
        بازگشت درست;

    اگر (p1.عدد > p2.عدد)
        بازگشت نادرست;

    بازگشت p1.کلیدها[0].compare(p2.کلیدها[0]) < 0;
}

Command_سطر_Parser::Command_سطر_Parser(صحیح argc, ثابت نویسه* ثابت argv[], ثابت String& کلیدها)
{
    impl = جدید Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    اگر (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    وگرنه
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = نادرست;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(کلیدها, '{', '}');

    صحیح jj = 0;
    برای (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', درست);
        Command_سطر_Parser_Params p;
        p.کلیدها = impl->split_string(l[0]);
        p.def_مقدار = l[1];
        p.help_message = cat_string(l[2]);
        p.عدد = -1;
        اگر (p.کلیدها.size() <= 0)
        {
            impl->error = درست;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        وگرنه
        {
            اگر (p.کلیدها[0][0] == '@')
            {
                p.عدد = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    برای (صحیح i = 1; i < argc; i++)
    {
        String s(argv[i]);
        منطقی hasSingleDash = s.length() > 1 && s[0] == '-';

        اگر (hasSingleDash)
        {
            منطقی hasDoubleDash = s.length() > 2 && s[1] == '-';
            String کلید = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = کلید.find('=');

            اگر(equalsPos != String::npos) {
                value = کلید.substr(equalsPos + 1);
                کلید = کلید.substr(0, equalsPos);
            }
            impl->apply_params(کلید, value);
        }
        وگرنه
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_سطر_Parser::~Command_سطر_Parser()
{
    اگر (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

Command_سطر_Parser::Command_سطر_Parser(ثابت Command_سطر_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سطر_Parser& Command_سطر_Parser::operator = (ثابت Command_سطر_Parser& parser)
{
    اگر( این != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        اگر(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    بازگشت *این;
}

تهی Command_سطر_Parser::about(ثابت String& message)
{
    impl->about_message = message;
}

تهی Command_سطر_Parser::Impl::apply_params(ثابت String& کلید, ثابت String& value)
{
    برای (size_t i = 0; i < data.size(); i++)
    {
        برای (size_t k = 0; k < data[i].کلیدها.size(); k++)
        {
            اگر (کلید.compare(data[i].کلیدها[k]) == 0)
            {
                data[i].def_مقدار = value;
                بشکن;
            }
        }
    }
}

تهی Command_سطر_Parser::Impl::apply_params(صحیح i, String value)
{
    برای (size_t j = 0; j < data.size(); j++)
    {
        اگر (data[j].عدد == i)
        {
            data[j].def_مقدار = value;
            بشکن;
        }
    }
}

تهی Command_سطر_Parser::Impl::sort_params()
{
    برای (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].کلیدها.begin(), data[i].کلیدها.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String Command_سطر_Parser::getPathToApplication() ثابت
{
    بازگشت impl->path_to_app;
}

منطقی Command_سطر_Parser::has(ثابت String& name) ثابت
{
    برای (size_t i = 0; i < impl->data.size(); i++)
    {
        برای (size_t j = 0; j < impl->data[i].کلیدها.size(); j++)
        {
            اگر (name == impl->data[i].کلیدها[j])
            {
                ثابت String v = cat_string(impl->data[i].def_مقدار);
                بازگشت !v.empty() && v != none_مقدار;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقی Command_سطر_Parser::check() ثابت
{
    بازگشت impl->error == نادرست;
}

تهی Command_سطر_Parser::printErrors() ثابت
{
    اگر (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

تهی Command_سطر_Parser::printMessage() ثابت
{
    اگر (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    برای (size_t i = 0; i < impl->data.size(); i++)
    {
        اگر (impl->data[i].عدد > -1)
        {
            String name = impl->data[i].کلیدها[0].substr(1, impl->data[i].کلیدها[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    برای (size_t i = 0; i < impl->data.size(); i++)
    {
        اگر (impl->data[i].عدد == -1)
        {
            printf("\t");
            برای (size_t j = 0; j < impl->data[i].کلیدها.size(); j++)
            {
                String k = impl->data[i].کلیدها[j];
                اگر (k.length() > 1)
                {
                    printf("--");
                }
                وگرنه
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                اگر (j != impl->data[i].کلیدها.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_مقدار);
            اگر (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    برای (size_t i = 0; i < impl->data.size(); i++)
    {
        اگر (impl->data[i].عدد != -1)
        {
            printf("\t");
            String k = impl->data[i].کلیدها[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_مقدار);
            اگر (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_سطر_Parser::Impl::split_range_string(ثابت String& _str, نویسه fs, نویسه ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    منطقی begin = نادرست;

    وقتی (!str.empty())
    {
        اگر (str[0] == fs)
        {
            اگر (begin == درست)
            {
                پرتابکن cv::Exception(cv::Error::StsParseError,
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
            begin = درست;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        اگر (str[0] == ss)
        {
            اگر (begin == نادرست)
            {
                پرتابکن cv::Exception(cv::Error::StsParseError,
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
            begin = نادرست;
            vec.push_back(word);
        }

        اگر (begin == درست)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    اگر (begin == درست)
    {
        پرتابکن cv::Exception(cv::Error::StsParseError,
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

    بازگشت vec;
}

std::vector<String> Command_سطر_Parser::Impl::split_string(ثابت String& _str, نویسه نماد, منطقی create_empty_item) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    وقتی (!str.empty())
    {
        اگر (str[0] == نماد)
        {
            اگر (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        وگرنه
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    اگر (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    بازگشت vec;
}

}
