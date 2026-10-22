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
    طالما( left < right && str[left] == ' ' )
        left++;
    طالما( right > left && str[right-1] == ' ' )
        right--;
    أرجع left >= right ? String("") : str.substr(left, right-left);
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
    std::vector<String> split_string(ثابت String& str, حرف symbol = ' ', منطقي create_empty_item = خطأ) ثابت;

    فراغ apply_params(ثابت String& key, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ sort_params();
    صحيح refcount;
};


ساكن ثابت حرف* get_type_name(Param type)
{
    𐤀𐤌( type == Param::INT )
        أرجع "int";
    𐤀𐤌( type == Param::BOOLEAN )
        أرجع "bool";
    𐤀𐤌( type == Param::UNSIGNED_INT )
        أرجع "unsigned";
    𐤀𐤌( type == Param::UINT64 )
        أرجع "unsigned long long";
    𐤀𐤌( type == Param::FLOAT )
        أرجع "float";
    𐤀𐤌( type == Param::REAL )
        أرجع "double";
    𐤀𐤌( type == Param::STRING )
        أرجع "string";
    أرجع "unknown";
}

ساكن منطقي parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقي value = خطأ;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    𐤀𐤌 (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    أرجع value;
}

ساكن فراغ from_str(ثابت String& str, Param type, فراغ* dst)
{
    std::stringstream ss(str.c_str());
    𐤀𐤌( type == Param::INT )
        ss >> *(صحيح*)dst;
    وإلا 𐤀𐤌( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(منطقي*) dst = parse_bool(temp);
    }
    وإلا 𐤀𐤌( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    وإلا 𐤀𐤌( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    وإلا 𐤀𐤌( type == Param::FLOAT )
        ss >> *(عائم*)dst;
    وإلا 𐤀𐤌( type == Param::REAL )
        ss >> *(مزدوج*)dst;
    وإلا 𐤀𐤌( type == Param::STRING )
        *(String*)dst = str;
    وإلا 𐤀𐤌( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        لكل (صحيح i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    وإلا
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    𐤀𐤌 (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

فراغ CommandLineParser::getByName(ثابت String& name, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->data.size(); i++)
        {
            لكل (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                𐤀𐤌 (name == impl->data[i].keys[j])
                {
                    String v = impl->data[i].def_value;
                    𐤀𐤌 (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    𐤀𐤌((v.empty() && type != Param::STRING) || v == noneValue) {
                        impl->error = true;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        أرجع;
                    }

                    from_str(v, type, dst);
                    أرجع;
                }
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ CommandLineParser::getByIndex(صحيح index, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->data.size(); i++)
        {
            𐤀𐤌 (impl->data[i].number == index)
            {
                String v = impl->data[i].def_value;
                𐤀𐤌 (space_delete == true) v = cat_string(v);

                // the key was neither specified nor has a default value
                𐤀𐤌((v.empty() && type != Param::STRING) || v == noneValue) {
                    impl->error = true;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    أرجع;
                }
                from_str(v, type, dst);
                أرجع;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = true;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

ساكن منطقي cmp_params(ثابت CommandLineParserParams & p1, ثابت CommandLineParserParams & p2)
{
    𐤀𐤌 (p1.number < p2.number)
        أرجع true;

    𐤀𐤌 (p1.number > p2.number)
        أرجع خطأ;

    أرجع p1.keys[0].compare(p2.keys[0]) < 0;
}

CommandLineParser::CommandLineParser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& keys)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    𐤀𐤌 (pos_s == String::npos)
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
    std::vector<String> k = impl->split_range_string(keys, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', true);
        CommandLineParserParams p;
        p.keys = impl->split_string(l[0]);
        p.def_value = l[1];
        p.help_message = cat_string(l[2]);
        p.number = -1;
        𐤀𐤌 (p.keys.size() <= 0)
        {
            impl->error = true;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            𐤀𐤌 (p.keys[0][0] == '@')
            {
                p.number = jj;
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

        𐤀𐤌 (hasSingleDash)
        {
            منطقي hasDoubleDash = s.length() > 2 && s[1] == '-';
            String key = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = key.find('=');

            𐤀𐤌(equalsPos != String::npos) {
                value = key.substr(equalsPos + 1);
                key = key.substr(0, equalsPos);
            }
            impl->apply_params(key, value);
        }
        وإلا
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

CommandLineParser::~CommandLineParser()
{
    𐤀𐤌 (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

CommandLineParser::CommandLineParser(ثابت CommandLineParser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

CommandLineParser& CommandLineParser::operator = (ثابت CommandLineParser& parser)
{
    𐤀𐤌( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        𐤀𐤌(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    أرجع *هذا;
}

فراغ CommandLineParser::about(ثابت String& message)
{
    impl->about_message = message;
}

فراغ CommandLineParser::Impl::apply_params(ثابت String& key, ثابت String& value)
{
    لكل (size_t i = 0; i < data.size(); i++)
    {
        لكل (size_t k = 0; k < data[i].keys.size(); k++)
        {
            𐤀𐤌 (key.compare(data[i].keys[k]) == 0)
            {
                data[i].def_value = value;
                اكسر;
            }
        }
    }
}

فراغ CommandLineParser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < data.size(); j++)
    {
        𐤀𐤌 (data[j].number == i)
        {
            data[j].def_value = value;
            اكسر;
        }
    }
}

فراغ CommandLineParser::Impl::sort_params()
{
    لكل (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].keys.begin(), data[i].keys.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String CommandLineParser::getPathToApplication() ثابت
{
    أرجع impl->path_to_app;
}

منطقي CommandLineParser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        لكل (size_t j = 0; j < impl->data[i].keys.size(); j++)
        {
            𐤀𐤌 (name == impl->data[i].keys[j])
            {
                ثابت String v = cat_string(impl->data[i].def_value);
                أرجع !v.empty() && v != noneValue;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي CommandLineParser::check() ثابت
{
    أرجع impl->error == خطأ;
}

فراغ CommandLineParser::printErrors() ثابت
{
    𐤀𐤌 (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

فراغ CommandLineParser::printMessage() ثابت
{
    𐤀𐤌 (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        𐤀𐤌 (impl->data[i].number > -1)
        {
            String name = impl->data[i].keys[0].substr(1, impl->data[i].keys[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        𐤀𐤌 (impl->data[i].number == -1)
        {
            printf("\t");
            لكل (size_t j = 0; j < impl->data[i].keys.size(); j++)
            {
                String k = impl->data[i].keys[j];
                𐤀𐤌 (k.length() > 1)
                {
                    printf("--");
                }
                وإلا
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                𐤀𐤌 (j != impl->data[i].keys.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_value);
            𐤀𐤌 (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    لكل (size_t i = 0; i < impl->data.size(); i++)
    {
        𐤀𐤌 (impl->data[i].number != -1)
        {
            printf("\t");
            String k = impl->data[i].keys[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_value);
            𐤀𐤌 (dv.compare("") != 0)
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
    منطقي begin = خطأ;

    طالما (!str.empty())
    {
        𐤀𐤌 (str[0] == fs)
        {
            𐤀𐤌 (begin == true)
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

        𐤀𐤌 (str[0] == ss)
        {
            𐤀𐤌 (begin == خطأ)
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

        𐤀𐤌 (begin == true)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    𐤀𐤌 (begin == true)
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

    أرجع vec;
}

std::vector<String> CommandLineParser::Impl::split_string(ثابت String& _str, حرف symbol, منطقي create_empty_item) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    طالما (!str.empty())
    {
        𐤀𐤌 (str[0] == symbol)
        {
            𐤀𐤌 (!word.empty() || create_empty_item)
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

    𐤀𐤌 (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    أرجع vec;
}

}
