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
    صحيح کھبے = 0, سجے = (صحيح)str.length();
    طالما( کھبے < سجے && str[کھبے] == ' ' )
        کھبے++;
    طالما( سجے > کھبے && str[سجے-1] == ' ' )
        سجے--;
    أرجع کھبے >= سجے ? String("") : str.substr(کھبے, سجے-کھبے);
}
}

بنية Command_سطر_Parser_Params
{
عام:
    String help_سنیہا;
    String def_قدر;
    std::vector<String> کلیدیں;
    صحيح نمبر;
};


بنية Command_سطر_Parser::Impl
{
    منطقي error;
    String error_سنیہا;
    String about_سنیہا;

    String رستہ_منزل_app;
    String app_name;

    std::vector<Command_سطر_Parser_Params> انکڑے;

    std::vector<String> ونڈو_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> ونڈو_string(ثابت String& str, حرف علامت = ' ', منطقي create_خالی_تھاواں_شے = خطأ) ثابت;

    فراغ apply_params(ثابت String& کلید, ثابت String& value);
    فراغ apply_params(صحيح i, String value);

    فراغ ترتیب_دیو_params();
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
        std::string عارضی_2;
        ss >> عارضی_2;
        *(منطقي*) dst = parse_bool(عارضی_2);
    }
    وإلا إذا( type == Param::UNSIGNED_INT )
        ss >> *(بنانشانی*)dst;
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
        لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
        {
            لكل (size_t j = 0; j < impl->انکڑے[i].کلیدیں.size(); j++)
            {
                إذا (name == impl->انکڑے[i].کلیدیں[j])
                {
                    String v = impl->انکڑے[i].def_قدر;
                    إذا (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    إذا((v.empty() && type != Param::STRING) || v == none_قدر) {
                        impl->error = سچ;
                        impl->error_سنیہا = impl->error_سنیہا + "Missing parameter: '" + name + "'\n";
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
        impl->error_سنیہا = impl->error_سنیہا + "Parameter '"+ name + "': " + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


فراغ Command_سطر_Parser::get_By_اشاریہ(صحيح اشاریہ, منطقي space_delete, Param type, فراغ* dst) ثابت
{
    حاول
    {
        لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
        {
            إذا (impl->انکڑے[i].نمبر == اشاریہ)
            {
                String v = impl->انکڑے[i].def_قدر;
                إذا (space_delete == سچ) v = cat_string(v);

                // the key was neither specified nor has a default value
                إذا((v.empty() && type != Param::STRING) || v == none_قدر) {
                    impl->error = سچ;
                    impl->error_سنیہا = impl->error_سنیہا + format("Missing parameter #%d\n", اشاریہ);
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
        impl->error_سنیہا = impl->error_سنیہا + format("Parameter #%d: ", اشاریہ) + e.err + "\n";
        أرجع;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", اشاریہ));
}

ساكن منطقي cmp_params(ثابت Command_سطر_Parser_Params & p1, ثابت Command_سطر_Parser_Params & p2)
{
    إذا (p1.نمبر < p2.نمبر)
        أرجع سچ;

    إذا (p1.نمبر > p2.نمبر)
        أرجع خطأ;

    أرجع p1.کلیدیں[0].compare(p2.کلیدیں[0]) < 0;
}

Command_سطر_Parser::Command_سطر_Parser(صحيح argc, ثابت حرف* ثابت argv[], ثابت String& کلیدیں)
{
    impl = جديد Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    إذا (pos_s == String::npos)
    {
        impl->رستہ_منزل_app = "";
        impl->app_name = String(argv[0]);
    }
    وإلا
    {
        impl->رستہ_منزل_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = خطأ;
    impl->error_سنیہا = "";

    // parse keys
    std::vector<String> k = impl->ونڈو_range_string(کلیدیں, '{', '}');

    صحيح jj = 0;
    لكل (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->ونڈو_string(k[i], '|', سچ);
        Command_سطر_Parser_Params p;
        p.کلیدیں = impl->ونڈو_string(l[0]);
        p.def_قدر = l[1];
        p.help_سنیہا = cat_string(l[2]);
        p.نمبر = -1;
        إذا (p.کلیدیں.size() <= 0)
        {
            impl->error = سچ;
            impl->error_سنیہا = "Field KEYS could not be empty\n";
        }
        وإلا
        {
            إذا (p.کلیدیں[0][0] == '@')
            {
                p.نمبر = jj;
                jj++;
            }

            impl->انکڑے.push_back(p);
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

    impl->ترتیب_دیو_params();
}

Command_سطر_Parser::~Command_سطر_Parser()
{
    إذا (CV_XADD(&impl->refcount, -1) == 1)
        مٹاؤ impl;
}

Command_سطر_Parser::Command_سطر_Parser(ثابت Command_سطر_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سطر_Parser& Command_سطر_Parser::عامل = (ثابت Command_سطر_Parser& parser)
{
    إذا( هذا != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        إذا(CV_XADD(&impl->refcount, -1) == 1)
            مٹاؤ impl;
        impl = parser.impl;
    }
    أرجع *هذا;
}

فراغ Command_سطر_Parser::about(ثابت String& message)
{
    impl->about_سنیہا = message;
}

فراغ Command_سطر_Parser::Impl::apply_params(ثابت String& کلید, ثابت String& value)
{
    لكل (size_t i = 0; i < انکڑے.size(); i++)
    {
        لكل (size_t k = 0; k < انکڑے[i].کلیدیں.size(); k++)
        {
            إذا (کلید.compare(انکڑے[i].کلیدیں[k]) == 0)
            {
                انکڑے[i].def_قدر = value;
                اكسر;
            }
        }
    }
}

فراغ Command_سطر_Parser::Impl::apply_params(صحيح i, String value)
{
    لكل (size_t j = 0; j < انکڑے.size(); j++)
    {
        إذا (انکڑے[j].نمبر == i)
        {
            انکڑے[j].def_قدر = value;
            اكسر;
        }
    }
}

فراغ Command_سطر_Parser::Impl::ترتیب_دیو_params()
{
    لكل (size_t i = 0; i < انکڑے.size(); i++)
    {
        std::sort(انکڑے[i].کلیدیں.begin(), انکڑے[i].کلیدیں.end());
    }

    std::sort (انکڑے.begin(), انکڑے.end(), cmp_params);
}

String Command_سطر_Parser::get_رستہ_منزل_Application() ثابت
{
    أرجع impl->رستہ_منزل_app;
}

منطقي Command_سطر_Parser::has(ثابت String& name) ثابت
{
    لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
    {
        لكل (size_t j = 0; j < impl->انکڑے[i].کلیدیں.size(); j++)
        {
            إذا (name == impl->انکڑے[i].کلیدیں[j])
            {
                ثابت String v = cat_string(impl->انکڑے[i].def_قدر);
                أرجع !v.empty() && v != none_قدر;
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
        لکھو("\nERRORS:\n%s\n", impl->error_سنیہا.c_str());
        fflush(stdout);
    }
}

فراغ Command_سطر_Parser::print_سنیہا() ثابت
{
    إذا (impl->about_سنیہا != "")
        لکھو("%s\n", impl->about_سنیہا.c_str());

    لکھو("Usage: %s [params] ", impl->app_name.c_str());

    لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
    {
        إذا (impl->انکڑے[i].نمبر > -1)
        {
            String name = impl->انکڑے[i].کلیدیں[0].substr(1, impl->انکڑے[i].کلیدیں[0].length() - 1);
            لکھو("%s ", name.c_str());
        }
    }

    لکھو("\n\n");

    لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
    {
        إذا (impl->انکڑے[i].نمبر == -1)
        {
            لکھو("\t");
            لكل (size_t j = 0; j < impl->انکڑے[i].کلیدیں.size(); j++)
            {
                String k = impl->انکڑے[i].کلیدیں[j];
                إذا (k.length() > 1)
                {
                    لکھو("--");
                }
                وإلا
                {
                    لکھو("-");
                }
                لکھو("%s", k.c_str());

                إذا (j != impl->انکڑے[i].کلیدیں.size() - 1)
                {
                    لکھو(", ");
                }
            }
            String dv = cat_string(impl->انکڑے[i].def_قدر);
            إذا (dv.compare("") != 0)
            {
                لکھو(" (value:%s)", dv.c_str());
            }
            لکھو("\n\t\t%s\n", impl->انکڑے[i].help_سنیہا.c_str());
        }
    }
    لکھو("\n");

    لكل (size_t i = 0; i < impl->انکڑے.size(); i++)
    {
        إذا (impl->انکڑے[i].نمبر != -1)
        {
            لکھو("\t");
            String k = impl->انکڑے[i].کلیدیں[0];
            k = k.substr(1, k.length() - 1);

            لکھو("%s", k.c_str());

            String dv = cat_string(impl->انکڑے[i].def_قدر);
            إذا (dv.compare("") != 0)
            {
                لکھو(" (value:%s)", dv.c_str());
            }
            لکھو("\n\t\t%s\n", impl->انکڑے[i].help_سنیہا.c_str());
        }
    }
}

std::vector<String> Command_سطر_Parser::Impl::ونڈو_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
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

std::vector<String> Command_سطر_Parser::Impl::ونڈو_string(ثابت String& _str, حرف علامت, منطقي create_خالی_تھاواں_شے) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String لفظ = "";

    طالما (!str.empty())
    {
        إذا (str[0] == علامت)
        {
            إذا (!لفظ.empty() || create_خالی_تھاواں_شے)
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

    إذا (لفظ != "" || create_خالی_تھاواں_شے)
    {
        vec.push_back(لفظ);
    }

    أرجع vec;
}

}
