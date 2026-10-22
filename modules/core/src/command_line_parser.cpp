// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "precomp.hpp"
#ضمن <sstream>

نطاق cv
{

نطاق {
ساكن ثابت حرف* none_مقدار = "<none>";

ساكن String cat_string(ثابت String& str)
{
    صحيح چپ = 0, راست = (صحيح)str.length();
    طالما( چپ < راست && str[چپ] == ' ' )
        چپ++;
    طالما( راست > چپ && str[راست-1] == ' ' )
        راست--;
    أرجع چپ >= راست ? String("") : str.substr(چپ, راست-چپ);
}
}

بنية Command_سطر_Parser_Params
{
عام:
    String help_پیام;
    String def_مقدار;
    std::vector<String> کلیدها;
    صحيح عدد;
};


بنية Command_سطر_Parser::Impl
{
    منطقي error;
    String error_پیام;
    String about_پیام;

    String مسیر_مقصد_app;
    String app_name;

    std::vector<Command_سطر_Parser_Params> داده;

    std::vector<String> تقسیم_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> تقسیم_string(ثابت String& str, حرف نماد = ' ', منطقي create_خالی_قلم = خطأ) ثابت;

    فراغ apply_params(ثابت String& کلید, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ مرتب_سازی_params();
    صحيح refcount;
};


ساكن ثابت حرف* get_type_name(Param type)
{
    إذا( type == Param::INT )
        أرجع "int";
    إذا( type == Param::BOOLEAN )
        أرجع "bool";
    إذا( type == Param::UNSIGNED_INT )
        أرجع "unsigned";
    إذا( type == Param::UINT64 )
        أرجع "unsigned long long";
    إذا( type == Param::FLOAT )
        أرجع "float";
    إذا( type == Param::REAL )
        أرجع "double";
    إذا( type == Param::STRING )
        أرجع "string";
    أرجع "unknown";
}

ساكن منطقي parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقي value = خطأ;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    إذا (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    أرجع value;
}

ساكن فراغ from_str(ثابت String& str, Param type, فراغ* dst)
{
    std::stringstream ss(str.c_str());
    إذا( type == Param::INT )
        ss >> *(صحيح*)dst;
    وإلا إذا( type == Param::BOOLEAN )
    {
        std::string موقت_2;
        ss >> موقت_2;
        *(منطقي*) dst = parse_bool(موقت_2);
    }
    وإلا إذا( type == Param::UNSIGNED_INT )
        ss >> *(بیعلامت*)dst;
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
        لكل (size_t i = 0; i < impl->داده.size(); i++)
        {
            لكل (size_t j = 0; j < impl->داده[i].کلیدها.size(); j++)
            {
                إذا (name == impl->داده[i].کلیدها[j])
                {
                    String v = impl->داده[i].def_مقدار;
                    إذا (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    إذا((v.empty() && type != Param::STRING) || v == none_مقدار) {
                        impl->error = درست;
                        impl->error_پیام = impl->error_پیام + "Missing parameter: '" + name + "'\n";
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
        impl->error = درست;
        impl->error_پیام = impl->error_پیام + "Parameter '"+ name + "': " + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ Command_سطر_Parser::get_By_شاخص(صحيح شاخص, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->داده.size(); i++)
        {
            إذا (impl->داده[i].عدد == شاخص)
            {
                String v = impl->داده[i].def_مقدار;
                إذا (space_delete == درست) v = cat_string(v);

                // the key was neither specified nor has a default value
                إذا((v.empty() && type != Param::STRING) || v == none_مقدار) {
                    impl->error = درست;
                    impl->error_پیام = impl->error_پیام + format("Missing parameter #%d\n", شاخص);
                    أرجع;
                }
                from_str(v, type, dst);
                أرجع;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = درست;
        impl->error_پیام = impl->error_پیام + format("Parameter #%d: ", شاخص) + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", شاخص));
}

ساكن منطقي cmp_params(ثابت Command_سطر_Parser_Params & p1, ثابت Command_سطر_Parser_Params & p2)
{
    إذا (p1.عدد < p2.عدد)
        أرجع درست;

    إذا (p1.عدد > p2.عدد)
        أرجع خطأ;

    أرجع p1.کلیدها[0].compare(p2.کلیدها[0]) < 0;
}

Command_سطر_Parser::Command_سطر_Parser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& کلیدها)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    إذا (pos_s == String::npos)
    {
        impl->مسیر_مقصد_app = "";
        impl->app_name = String(argv[0]);
    }
    وإلا
    {
        impl->مسیر_مقصد_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = خطأ;
    impl->error_پیام = "";

    // parse keys
    std::vector<String> k = impl->تقسیم_range_string(کلیدها, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->تقسیم_string(k[i], '|', درست);
        Command_سطر_Parser_Params p;
        p.کلیدها = impl->تقسیم_string(l[0]);
        p.def_مقدار = l[1];
        p.help_پیام = cat_string(l[2]);
        p.عدد = -1;
        إذا (p.کلیدها.size() <= 0)
        {
            impl->error = درست;
            impl->error_پیام = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            إذا (p.کلیدها[0][0] == '@')
            {
                p.عدد = jj;
                jj++;
            }

            impl->داده.push_back(p);
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
            String کلید = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = کلید.find('=');

            إذا(equalsPos != String::npos) {
                value = کلید.substr(equalsPos + 1);
                کلید = کلید.substr(0, equalsPos);
            }
            impl->apply_params(کلید, value);
        }
        وإلا
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->مرتب_سازی_params();
}

Command_سطر_Parser::~Command_سطر_Parser()
{
    إذا (CV_XADD(&impl->refcount, -1) == 1)
        حذف impl;
}

Command_سطر_Parser::Command_سطر_Parser(ثابت Command_سطر_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سطر_Parser& Command_سطر_Parser::عملگر = (ثابت Command_سطر_Parser& parser)
{
    إذا( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        إذا(CV_XADD(&impl->refcount, -1) == 1)
            حذف impl;
        impl = parser.impl;
    }
    أرجع *هذا;
}

فراغ Command_سطر_Parser::about(ثابت String& message)
{
    impl->about_پیام = message;
}

فراغ Command_سطر_Parser::Impl::apply_params(ثابت String& کلید, ثابت String& value)
{
    لكل (size_t i = 0; i < داده.size(); i++)
    {
        لكل (size_t k = 0; k < داده[i].کلیدها.size(); k++)
        {
            إذا (کلید.compare(داده[i].کلیدها[k]) == 0)
            {
                داده[i].def_مقدار = value;
                اكسر;
            }
        }
    }
}

فراغ Command_سطر_Parser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < داده.size(); j++)
    {
        إذا (داده[j].عدد == i)
        {
            داده[j].def_مقدار = value;
            اكسر;
        }
    }
}

فراغ Command_سطر_Parser::Impl::مرتب_سازی_params()
{
    لكل (size_t i = 0; i < داده.size(); i++)
    {
        std::sort(داده[i].کلیدها.begin(), داده[i].کلیدها.end());
    }

    std::sort (داده.begin(), داده.end(), cmp_params);
}

String Command_سطر_Parser::get_مسیر_مقصد_Application() ثابت
{
    أرجع impl->مسیر_مقصد_app;
}

منطقي Command_سطر_Parser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->داده.size(); i++)
    {
        لكل (size_t j = 0; j < impl->داده[i].کلیدها.size(); j++)
        {
            إذا (name == impl->داده[i].کلیدها[j])
            {
                ثابت String v = cat_string(impl->داده[i].def_مقدار);
                أرجع !v.empty() && v != none_مقدار;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي Command_سطر_Parser::check() ثابت
{
    أرجع impl->error == خطأ;
}

فراغ Command_سطر_Parser::printErrors() ثابت
{
    إذا (impl->error)
    {
        چاپ_کن("\nERRORS:\n%s\n", impl->error_پیام.c_str());
        fflush(stdout);
    }
}

فراغ Command_سطر_Parser::print_پیام() ثابت
{
    إذا (impl->about_پیام != "")
        چاپ_کن("%s\n", impl->about_پیام.c_str());

    چاپ_کن("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->داده.size(); i++)
    {
        إذا (impl->داده[i].عدد > -1)
        {
            String name = impl->داده[i].کلیدها[0].substr(1, impl->داده[i].کلیدها[0].length() - 1);
            چاپ_کن("%s ", name.c_str());
        }
    }

    چاپ_کن("\n\n");

    لكل (size_t i = 0; i < impl->داده.size(); i++)
    {
        إذا (impl->داده[i].عدد == -1)
        {
            چاپ_کن("\t");
            لكل (size_t j = 0; j < impl->داده[i].کلیدها.size(); j++)
            {
                String k = impl->داده[i].کلیدها[j];
                إذا (k.length() > 1)
                {
                    چاپ_کن("--");
                }
                وإلا
                {
                    چاپ_کن("-");
                }
                چاپ_کن("%s", k.c_str());

                إذا (j != impl->داده[i].کلیدها.size() - 1)
                {
                    چاپ_کن(", ");
                }
            }
            String dv = cat_string(impl->داده[i].def_مقدار);
            إذا (dv.compare("") != 0)
            {
                چاپ_کن(" (value:%s)", dv.c_str());
            }
            چاپ_کن("\n\t\t%s\n", impl->داده[i].help_پیام.c_str());
        }
    }
    چاپ_کن("\n");

    لكل (size_t i = 0; i < impl->داده.size(); i++)
    {
        إذا (impl->داده[i].عدد != -1)
        {
            چاپ_کن("\t");
            String k = impl->داده[i].کلیدها[0];
            k = k.substr(1, k.length() - 1);

            چاپ_کن("%s", k.c_str());

            String dv = cat_string(impl->داده[i].def_مقدار);
            إذا (dv.compare("") != 0)
            {
                چاپ_کن(" (value:%s)", dv.c_str());
            }
            چاپ_کن("\n\t\t%s\n", impl->داده[i].help_پیام.c_str());
        }
    }
}

std::vector<String> Command_سطر_Parser::Impl::تقسیم_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String کلمه = "";
    منطقي begin = خطأ;

    طالما (!str.empty())
    {
        إذا (str[0] == fs)
        {
            إذا (begin == درست)
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
            begin = درست;
            کلمه = "";
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
            vec.push_back(کلمه);
        }

        إذا (begin == درست)
        {
            کلمه = کلمه + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (begin == درست)
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

std::vector<String> Command_سطر_Parser::Impl::تقسیم_string(ثابت String& _str, حرف نماد, منطقي create_خالی_قلم) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String کلمه = "";

    طالما (!str.empty())
    {
        إذا (str[0] == نماد)
        {
            إذا (!کلمه.empty() || create_خالی_قلم)
            {
                vec.push_back(کلمه);
                کلمه = "";
            }
        }
        وإلا
        {
            کلمه = کلمه + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (کلمه != "" || create_خالی_قلم)
    {
        vec.push_back(کلمه);
    }

    أرجع vec;
}

}
