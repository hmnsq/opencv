// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "precomp.hpp"
#ضمن <sstream>

نطاق cv
{

نطاق {
ساكن ثابت حرف* none_ارزښت = "<none>";

ساكن String cat_string(ثابت String& str)
{
    صحيح کیڼ = 0, ښی = (صحيح)str.length();
    طالما( کیڼ < ښی && str[کیڼ] == ' ' )
        کیڼ++;
    طالما( ښی > کیڼ && str[ښی-1] == ' ' )
        ښی--;
    أرجع کیڼ >= ښی ? String("") : str.substr(کیڼ, ښی-کیڼ);
}
}

بنية Command_کرښه_Parser_Params
{
عام:
    String help_پیغام;
    String def_ارزښت;
    std::vector<String> کلیګانې;
    صحيح شمېره;
};


بنية Command_کرښه_Parser::Impl
{
    منطقي error;
    String error_پیغام;
    String about_پیغام;

    String لار_منزل_app;
    String app_name;

    std::vector<Command_کرښه_Parser_Params> معلومات;

    std::vector<String> وېشل_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> وېشل_string(ثابت String& str, حرف نښه = ' ', منطقي create_تشه_توکی = خطأ) ثابت;

    فراغ apply_params(ثابت String& کلی, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ ترتیبول_params();
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
        std::string لنډمهاله_2;
        ss >> لنډمهاله_2;
        *(منطقي*) dst = parse_bool(لنډمهاله_2);
    }
    وإلا إذا( type == Param::UNSIGNED_INT )
        ss >> *(بینښه*)dst;
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

فراغ Command_کرښه_Parser::getByName(ثابت String& name, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->معلومات.size(); i++)
        {
            لكل (size_t j = 0; j < impl->معلومات[i].کلیګانې.size(); j++)
            {
                إذا (name == impl->معلومات[i].کلیګانې[j])
                {
                    String v = impl->معلومات[i].def_ارزښت;
                    إذا (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    إذا((v.empty() && type != Param::STRING) || v == none_ارزښت) {
                        impl->error = ریښتیا;
                        impl->error_پیغام = impl->error_پیغام + "Missing parameter: '" + name + "'\n";
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
        impl->error = ریښتیا;
        impl->error_پیغام = impl->error_پیغام + "Parameter '"+ name + "': " + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ Command_کرښه_Parser::get_By_شاخص(صحيح شاخص, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->معلومات.size(); i++)
        {
            إذا (impl->معلومات[i].شمېره == شاخص)
            {
                String v = impl->معلومات[i].def_ارزښت;
                إذا (space_delete == ریښتیا) v = cat_string(v);

                // the key was neither specified nor has a default value
                إذا((v.empty() && type != Param::STRING) || v == none_ارزښت) {
                    impl->error = ریښتیا;
                    impl->error_پیغام = impl->error_پیغام + format("Missing parameter #%d\n", شاخص);
                    أرجع;
                }
                from_str(v, type, dst);
                أرجع;
            }
        }
    }
    امسك (ثابت Exception& e)
    {
        impl->error = ریښتیا;
        impl->error_پیغام = impl->error_پیغام + format("Parameter #%d: ", شاخص) + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", شاخص));
}

ساكن منطقي cmp_params(ثابت Command_کرښه_Parser_Params & p1, ثابت Command_کرښه_Parser_Params & p2)
{
    إذا (p1.شمېره < p2.شمېره)
        أرجع ریښتیا;

    إذا (p1.شمېره > p2.شمېره)
        أرجع خطأ;

    أرجع p1.کلیګانې[0].compare(p2.کلیګانې[0]) < 0;
}

Command_کرښه_Parser::Command_کرښه_Parser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& کلیګانې)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    إذا (pos_s == String::npos)
    {
        impl->لار_منزل_app = "";
        impl->app_name = String(argv[0]);
    }
    وإلا
    {
        impl->لار_منزل_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = خطأ;
    impl->error_پیغام = "";

    // parse keys
    std::vector<String> k = impl->وېشل_range_string(کلیګانې, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->وېشل_string(k[i], '|', ریښتیا);
        Command_کرښه_Parser_Params p;
        p.کلیګانې = impl->وېشل_string(l[0]);
        p.def_ارزښت = l[1];
        p.help_پیغام = cat_string(l[2]);
        p.شمېره = -1;
        إذا (p.کلیګانې.size() <= 0)
        {
            impl->error = ریښتیا;
            impl->error_پیغام = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            إذا (p.کلیګانې[0][0] == '@')
            {
                p.شمېره = jj;
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
            String کلی = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = کلی.find('=');

            إذا(equalsPos != String::npos) {
                value = کلی.substr(equalsPos + 1);
                کلی = کلی.substr(0, equalsPos);
            }
            impl->apply_params(کلی, value);
        }
        وإلا
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->ترتیبول_params();
}

Command_کرښه_Parser::~Command_کرښه_Parser()
{
    إذا (CV_XADD(&impl->refcount, -1) == 1)
        ړنګول impl;
}

Command_کرښه_Parser::Command_کرښه_Parser(ثابت Command_کرښه_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_کرښه_Parser& Command_کرښه_Parser::چلوونکی = (ثابت Command_کرښه_Parser& parser)
{
    إذا( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        إذا(CV_XADD(&impl->refcount, -1) == 1)
            ړنګول impl;
        impl = parser.impl;
    }
    أرجع *هذا;
}

فراغ Command_کرښه_Parser::about(ثابت String& message)
{
    impl->about_پیغام = message;
}

فراغ Command_کرښه_Parser::Impl::apply_params(ثابت String& کلی, ثابت String& value)
{
    لكل (size_t i = 0; i < معلومات.size(); i++)
    {
        لكل (size_t k = 0; k < معلومات[i].کلیګانې.size(); k++)
        {
            إذا (کلی.compare(معلومات[i].کلیګانې[k]) == 0)
            {
                معلومات[i].def_ارزښت = value;
                اكسر;
            }
        }
    }
}

فراغ Command_کرښه_Parser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < معلومات.size(); j++)
    {
        إذا (معلومات[j].شمېره == i)
        {
            معلومات[j].def_ارزښت = value;
            اكسر;
        }
    }
}

فراغ Command_کرښه_Parser::Impl::ترتیبول_params()
{
    لكل (size_t i = 0; i < معلومات.size(); i++)
    {
        std::sort(معلومات[i].کلیګانې.begin(), معلومات[i].کلیګانې.end());
    }

    std::sort (معلومات.begin(), معلومات.end(), cmp_params);
}

String Command_کرښه_Parser::get_لار_منزل_Application() ثابت
{
    أرجع impl->لار_منزل_app;
}

منطقي Command_کرښه_Parser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        لكل (size_t j = 0; j < impl->معلومات[i].کلیګانې.size(); j++)
        {
            إذا (name == impl->معلومات[i].کلیګانې[j])
            {
                ثابت String v = cat_string(impl->معلومات[i].def_ارزښت);
                أرجع !v.empty() && v != none_ارزښت;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقي Command_کرښه_Parser::check() ثابت
{
    أرجع impl->error == خطأ;
}

فراغ Command_کرښه_Parser::printErrors() ثابت
{
    إذا (impl->error)
    {
        چاپ_کړه("\nERRORS:\n%s\n", impl->error_پیغام.c_str());
        fflush(stdout);
    }
}

فراغ Command_کرښه_Parser::print_پیغام() ثابت
{
    إذا (impl->about_پیغام != "")
        چاپ_کړه("%s\n", impl->about_پیغام.c_str());

    چاپ_کړه("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].شمېره > -1)
        {
            String name = impl->معلومات[i].کلیګانې[0].substr(1, impl->معلومات[i].کلیګانې[0].length() - 1);
            چاپ_کړه("%s ", name.c_str());
        }
    }

    چاپ_کړه("\n\n");

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].شمېره == -1)
        {
            چاپ_کړه("\t");
            لكل (size_t j = 0; j < impl->معلومات[i].کلیګانې.size(); j++)
            {
                String k = impl->معلومات[i].کلیګانې[j];
                إذا (k.length() > 1)
                {
                    چاپ_کړه("--");
                }
                وإلا
                {
                    چاپ_کړه("-");
                }
                چاپ_کړه("%s", k.c_str());

                إذا (j != impl->معلومات[i].کلیګانې.size() - 1)
                {
                    چاپ_کړه(", ");
                }
            }
            String dv = cat_string(impl->معلومات[i].def_ارزښت);
            إذا (dv.compare("") != 0)
            {
                چاپ_کړه(" (value:%s)", dv.c_str());
            }
            چاپ_کړه("\n\t\t%s\n", impl->معلومات[i].help_پیغام.c_str());
        }
    }
    چاپ_کړه("\n");

    لكل (size_t i = 0; i < impl->معلومات.size(); i++)
    {
        إذا (impl->معلومات[i].شمېره != -1)
        {
            چاپ_کړه("\t");
            String k = impl->معلومات[i].کلیګانې[0];
            k = k.substr(1, k.length() - 1);

            چاپ_کړه("%s", k.c_str());

            String dv = cat_string(impl->معلومات[i].def_ارزښت);
            إذا (dv.compare("") != 0)
            {
                چاپ_کړه(" (value:%s)", dv.c_str());
            }
            چاپ_کړه("\n\t\t%s\n", impl->معلومات[i].help_پیغام.c_str());
        }
    }
}

std::vector<String> Command_کرښه_Parser::Impl::وېشل_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String کلمه = "";
    منطقي begin = خطأ;

    طالما (!str.empty())
    {
        إذا (str[0] == fs)
        {
            إذا (begin == ریښتیا)
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
            begin = ریښتیا;
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

        إذا (begin == ریښتیا)
        {
            کلمه = کلمه + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    إذا (begin == ریښتیا)
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

std::vector<String> Command_کرښه_Parser::Impl::وېشل_string(ثابت String& _str, حرف نښه, منطقي create_تشه_توکی) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String کلمه = "";

    طالما (!str.empty())
    {
        إذا (str[0] == نښه)
        {
            إذا (!کلمه.empty() || create_تشه_توکی)
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

    إذا (کلمه != "" || create_تشه_توکی)
    {
        vec.push_back(کلمه);
    }

    أرجع vec;
}

}
