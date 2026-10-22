// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "precomp.hpp"
#ضمن <sstream>

نطاق cv
{

نطاق {
ساكن ثابت حرف* none_قدر = "<none>";

ساكن String cat_string(ثابت String& str)
{
    صحيح کاٻي = 0, ساڄي = (صحيح)str.length();
    طالما( کاٻي < ساڄي && str[کاٻي] == ' ' )
        کاٻي++;
    طالما( ساڄي > کاٻي && str[ساڄي-1] == ' ' )
        ساڄي--;
    أرجع کاٻي >= ساڄي ? String("") : str.substr(کاٻي, ساڄي-کاٻي);
}
}

بنية Command_سٽ_Parser_Params
{
عام:
    String help_پيغام;
    String def_قدر;
    std::vector<String> ڪنجيون;
    صحيح عدد;
};


بنية Command_سٽ_Parser::Impl
{
    منطقي error;
    String error_پيغام;
    String about_پيغام;

    String رستو_منزل_app;
    String app_name;

    std::vector<Command_سٽ_Parser_Params> معلومات;

    std::vector<String> ورهايو_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> ورهايو_string(ثابت String& str, حرف نشان = ' ', منطقي create_خالي_جايون_شي = خطأ) ثابت;

    فراغ apply_params(ثابت String& ڪنجي, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ ترتيب_ڏيو_params();
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
        std::string عارضي_2;
        ss >> عارضي_2;
        *(منطقي*) dst = parse_bool(عارضي_2);
    }
    وإلا إذا( type == Param::UNSIGNED_INT )
        ss >> *(بنانشان*)dst;
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

فراغ Command_سٽ_Parser::getByName(ثابت String& name, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->معلومات.size(); i++)
        {
            لكل (size_t j = 0; j < impl->معلومات[i].ڪنجيون.size(); j++)
            {
                إذا (name == impl->معلومات[i].ڪنجيون[j])
                {
                    String v = impl->معلومات[i].def_قدر;
                    إذا (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    إذا((v.empty() && type != Param::STRING) || v == none_قدر) {
                        impl->error = سچ;
                        impl->error_پيغام = impl->error_پيغام + "Missing parameter: '" + name + "'\n";
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
        impl->error = سچ;
        impl->error_پيغام = impl->error_پيغام + "Parameter '"+ name + "': " + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ Command_سٽ_Parser::get_By_اشاريو(صحيح اشاريو, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->معلومات.size(); i++)
        {
            إذا (impl->معلومات[i].عدد == اشاريو)
            {
                String v = impl->معلومات[i].def_قدر;
                إذا (space_delete == سچ) v = cat_string(v);

                // the key was neither specified nor has a default value
                إذا((v.empty() && type != Param::STRING) || v == none_قدر) {
                    impl->error = سچ;
                    impl->error_پيغام = impl->error_پيغام + format("Missing parameter #%d\n", اشاريو);
                    أرجع;
                }
                from_str(v, type, dst);
                أرجع;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = سچ;
        impl->error_پيغام = impl->error_پيغام + format("Parameter #%d: ", اشاريو) + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", اشاريو));
}

ساكن منطقي cmp_params(ثابت Command_سٽ_Parser_Params & p1, ثابت Command_سٽ_Parser_Params & p2)
{
    إذا (p1.عدد < p2.عدد)
        أرجع سچ;

    إذا (p1.عدد > p2.عدد)
        أرجع خطأ;

    أرجع p1.ڪنجيون[0].compare(p2.ڪنجيون[0]) < 0;
}

Command_سٽ_Parser::Command_سٽ_Parser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& ڪنجيون)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    إذا (pos_s == String::npos)
    {
        impl->رستو_منزل_app = "";
        impl->app_name = String(argv[0]);
    }
    وإلا
    {
        impl->رستو_منزل_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = خطأ;
    impl->error_پيغام = "";

    // parse keys
    std::vector<String> k = impl->ورهايو_range_string(ڪنجيون, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->ورهايو_string(k[i], '|', سچ);
        Command_سٽ_Parser_Params p;
        p.ڪنجيون = impl->ورهايو_string(l[0]);
        p.def_قدر = l[1];
        p.help_پيغام = cat_string(l[2]);
        p.عدد = -1;
        إذا (p.ڪنجيون.size() <= 0)
        {
            impl->error = سچ;
            impl->error_پيغام = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            إذا (p.ڪنجيون[0][0] == '@')
            {
                p.عدد = jj;
                jj++;
            }

            impl->معلومات.push_back(p);
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
            String ڪنجي = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = ڪنجي.find('=');

            إذا(equalsPos != String::npos) {
                value = ڪنجي.substr(equalsPos + 1);
                ڪنجي = ڪنجي.substr(0, equalsPos);
            }
            impl->apply_params(ڪنجي, value);
        }
        وإلا
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->ترتيب_ڏيو_params();
}

Command_سٽ_Parser::~Command_سٽ_Parser()
{
    إذا (CV_XADD(&impl->refcount, -1) == 1)
        ميٽيو impl;
}

Command_سٽ_Parser::Command_سٽ_Parser(ثابت Command_سٽ_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سٽ_Parser& Command_سٽ_Parser::عامل = (ثابت Command_سٽ_Parser& parser)
{
    إذا( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        إذا(CV_XADD(&impl->refcount, -1) == 1)
            ميٽيو impl;
        impl = parser.impl;
    }
    أرجع *هذا;
}

فراغ Command_سٽ_Parser::about(ثابت String& message)
{
    impl->about_پيغام = message;
}

فراغ Command_سٽ_Parser::Impl::apply_params(ثابت String& ڪنجي, ثابت String& value)
{
    لكل (size_t i = 0; i < معلومات.size(); i++)
    {
        لكل (size_t k = 0; k < معلومات[i].ڪنجيون.size(); k++)
        {
            إذا (ڪنجي.compare(معلومات[i].ڪنجيون[k]) == 0)
            {
                معلومات[i].def_قدر = value;
                اكسر;
            }
        }
    }
}

فراغ Command_سٽ_Parser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < معلومات.size(); j++)
    {
        إذا (معلومات[j].عدد == i)
        {
            معلومات[j].def_قدر = value;
            اكسر;
        }
    }
}

فراغ Command_سٽ_Parser::Impl::ترتيب_ڏيو_params()
{
    لكل (size_t i = 0; i < معلومات.size(); i++)
    {
        std::sort(معلومات[i].ڪنجيون.begin(), معلومات[i].ڪنجيون.end());
    }

    std::sort (معلومات.begin(), معلومات.end(), cmp_params);
}

String Command_سٽ_Parser::get_رستو_منزل_Application() ثابت
{
    أرجع impl->رستو_منزل_app;
}

منطقي Command_سٽ_Parser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        لكل (size_t j = 0; j < impl->معلومات[i].ڪنجيون.size(); j++)
        {
            إذا (name == impl->معلومات[i].ڪنجيون[j])
            {
                ثابت String v = cat_string(impl->معلومات[i].def_قدر);
                أرجع !v.empty() && v != none_قدر;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي Command_سٽ_Parser::check() ثابت
{
    أرجع impl->error == خطأ;
}

فراغ Command_سٽ_Parser::printErrors() ثابت
{
    إذا (impl->error)
    {
        لکو("\nERRORS:\n%s\n", impl->error_پيغام.c_str());
        fflush(stdout);
    }
}

فراغ Command_سٽ_Parser::print_پيغام() ثابت
{
    إذا (impl->about_پيغام != "")
        لکو("%s\n", impl->about_پيغام.c_str());

    لکو("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].عدد > -1)
        {
            String name = impl->معلومات[i].ڪنجيون[0].substr(1, impl->معلومات[i].ڪنجيون[0].length() - 1);
            لکو("%s ", name.c_str());
        }
    }

    لکو("\n\n");

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].عدد == -1)
        {
            لکو("\t");
            لكل (size_t j = 0; j < impl->معلومات[i].ڪنجيون.size(); j++)
            {
                String k = impl->معلومات[i].ڪنجيون[j];
                إذا (k.length() > 1)
                {
                    لکو("--");
                }
                وإلا
                {
                    لکو("-");
                }
                لکو("%s", k.c_str());

                إذا (j != impl->معلومات[i].ڪنجيون.size() - 1)
                {
                    لکو(", ");
                }
            }
            String dv = cat_string(impl->معلومات[i].def_قدر);
            إذا (dv.compare("") != 0)
            {
                لکو(" (value:%s)", dv.c_str());
            }
            لکو("\n\t\t%s\n", impl->معلومات[i].help_پيغام.c_str());
        }
    }
    لکو("\n");

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].عدد != -1)
        {
            لکو("\t");
            String k = impl->معلومات[i].ڪنجيون[0];
            k = k.substr(1, k.length() - 1);

            لکو("%s", k.c_str());

            String dv = cat_string(impl->معلومات[i].def_قدر);
            إذا (dv.compare("") != 0)
            {
                لکو(" (value:%s)", dv.c_str());
            }
            لکو("\n\t\t%s\n", impl->معلومات[i].help_پيغام.c_str());
        }
    }
}

std::vector<String> Command_سٽ_Parser::Impl::ورهايو_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String لفظ = "";
    منطقي begin = خطأ;

    طالما (!str.empty())
    {
        إذا (str[0] == fs)
        {
            إذا (begin == سچ)
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
            begin = سچ;
            لفظ = "";
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
            vec.push_back(لفظ);
        }

        إذا (begin == سچ)
        {
            لفظ = لفظ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (begin == سچ)
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

std::vector<String> Command_سٽ_Parser::Impl::ورهايو_string(ثابت String& _str, حرف نشان, منطقي create_خالي_جايون_شي) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String لفظ = "";

    طالما (!str.empty())
    {
        إذا (str[0] == نشان)
        {
            إذا (!لفظ.empty() || create_خالي_جايون_شي)
            {
                vec.push_back(لفظ);
                لفظ = "";
            }
        }
        وإلا
        {
            لفظ = لفظ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (لفظ != "" || create_خالي_جايون_شي)
    {
        vec.push_back(لفظ);
    }

    أرجع vec;
}

}
