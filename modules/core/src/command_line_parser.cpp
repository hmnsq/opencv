// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "precomp.hpp"
#ضمن <sstream>

نطاق cv
{

نطاق {
ساكن ثابت حرف* noneValue = "<none>";

ساكن String cat_string(ثابت String& str)
{
    صحيح left = 0, right = (صحيح)str.length();
    כלעוד( left < right && str[left] == ' ' )
        left++;
    כלעוד( right > left && str[right-1] == ' ' )
        right--;
    החזר left >= right ? String("") : str.substr(left, right-left);
}
}

بنية CommandLineParserParams
{
عام:
    String help_message;
    String def_value;
    std::vector<String> keys;
    صحيح number;
};


بنية CommandLineParser::Impl
{
    منطقي error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<CommandLineParserParams> data;

    std::vector<String> split_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> split_string(ثابت String& str, حرف symbol = ' ', منطقي create_empty_item = שקר) ثابت;

    فراغ apply_params(ثابت String& key, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ sort_params();
    صحيح refcount;
};


ساكن ثابت حرف* get_type_name(Param type)
{
    אם( type == Param::INT )
        החזר "int";
    אם( type == Param::BOOLEAN )
        החזר "bool";
    אם( type == Param::UNSIGNED_INT )
        החזר "unsigned";
    אם( type == Param::UINT64 )
        החזר "unsigned long long";
    אם( type == Param::FLOAT )
        החזר "float";
    אם( type == Param::REAL )
        החזר "double";
    אם( type == Param::STRING )
        החזר "string";
    החזר "unknown";
}

ساكن منطقي parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقي value = שקר;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    אם (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    החזר value;
}

ساكن فراغ from_str(ثابت String& str, Param type, فراغ* dst)
{
    std::stringstream ss(str.c_str());
    אם( type == Param::INT )
        ss >> *(صحيح*)dst;
    אחרת אם( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(منطقي*) dst = parse_bool(temp);
    }
    אחרת אם( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    אחרת אם( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    אחרת אם( type == Param::FLOAT )
        ss >> *(عائم*)dst;
    אחרת אם( type == Param::REAL )
        ss >> *(مزدوج*)dst;
    אחרת אם( type == Param::STRING )
        *(String*)dst = str;
    אחרת אם( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        עבור (صحيح i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    אחרת
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    אם (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

فراغ CommandLineParser::getByName(ثابت String& name, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        עבור (size_t i = 0; i < impl->data.size(); i++)
        {
            עבור (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                אם (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    אם (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    אם((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = אמת;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        החזר;
                    }

                    from_str(v, type, dst);
                    החזר;
                }
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = אמת;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        החזר;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ CommandLineParser::getByIndex(صحيح index, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        עבור (size_t i = 0; i < impl->data.size(); i++)
        {
            אם (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                אם (space_delete == אמת) v = cat_string(v);

                // the key was neither specified nor has a default value
                אם((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = אמת;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    החזר;
                }
                from_str(v, type, dst);
                החזר;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = אמת;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        החזר;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

ساكن منطقي cmp_params(ثابت CommandLineParserParams & p1, ثابت CommandLineParserParams & p2)
{
    אם (p1.number < p2.number)
        החזר אמת;

    אם (p1.number > p2.number)
        החזר שקר;

    החזר p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& keys)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    אם (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    אחרת
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = שקר;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    صحيح jj = 0;
    עבור (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', אמת);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        אם (p.keys.size() <= 0)
        {
            impl->error = אמת;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        אחרת
        {
            אם (p.keys[0][0] == '@')
            {
                p.number = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    עבור (صحيح i = 1; i < argc; i++)
    {
        String s(argv[i]);
        منطقي hasSingleDash = s.length() > 1 && s[0] == '-';

        אם (hasSingleDash)
        {
            منطقي hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            אם(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        אחרת
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    אם (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(ثابت CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (ثابت CommandLineParser& parser)
{
    אם( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        אם(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    החזר *هذا;
}

فراغ CommandLineParser::about(ثابت String& message)
{
    impl->about_message = message;
}

فراغ CommandLineParser::Impl::apply_params(ثابت String& key, ثابت String& value)
{
    עבור (size_t i = 0; i < data.size(); i++)
    {
        עבור (size_t k = 0; k < data[i].keys.size(); k++)
        {
            אם (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                اكسر;
            }
        }
    }
}

فراغ CommandLineParser::Impl::apply_params(صحيح i, String value)
{
    עבור (size_t j = 0; j < data.size(); j++)
    {
        אם (data[j].number == i)
        {
            data[j].def_value = value;
            اكسر;
        }
    }
}

فراغ CommandLineParser::Impl::sort_params()
{
    עבור (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() ثابت
{
    החזר impl->path_to_app;
}

منطقي CommandLineParser::has(ثابت String& name) ثابت
{
    עבור (size_t i = 0; i < impl->data.size(); i++)
    {
        עבור (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            אם (name == impl->data[i].keys[j])
            {
                ثابت String v = cat_string(impl->data[i].def_value);
                החזר !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي CommandLineParser::check() ثابت
{
    החזר impl->error == שקר;
}

فراغ CommandLineParser::printErrors() ثابت
{
    אם (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

فراغ CommandLineParser::printMessage() ثابت
{
    אם (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    עבור (size_t i = 0; i < impl->data.size(); i++)
    {
        אם (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    עבור (size_t i = 0; i < impl->data.size(); i++)
    {
        אם (impl->data[i].number == -1)
        {
            printf("\t");
            עבור (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                אם (k.length() > 1)
                {
                    printf("--");
                }
                אחרת
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                אם (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            אם (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    עבור (size_t i = 0; i < impl->data.size(); i++)
    {
        אם (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            אם (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> CommandLineParser::Impl::split_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    منطقي begin = שקר;

    כלעוד (!str.empty())
    {
        אם (str[0] == fs)
        {
            אם (begin == אמת)
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
            begin = אמת;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        אם (str[0] == ss)
        {
            אם (begin == שקר)
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
            begin = שקר;
            vec.push_back(word);
        }

        אם (begin == אמת)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    אם (begin == אמת)
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

    החזר vec;
}

std::vector<String> CommandLineParser::Impl::split_string(ثابت String& _str, حرف symbol, منطقي create_empty_item) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    כלעוד (!str.empty())
    {
        אם (str[0] == symbol)
        {
            אם (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        אחרת
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    אם (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    החזר vec;
}

}
