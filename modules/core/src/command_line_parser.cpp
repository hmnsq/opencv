// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "precomp.hpp"
#شامل <sstream>

نامخلا cv
{

نامخلا {
ساکن ثابت حرف* none_قدر = "<none>";

ساکن String cat_string(ثابت String& str)
{
    صحیح بائیں = 0, دائیں = (صحیح)str.length();
    جب( بائیں < دائیں && str[بائیں] == ' ' )
        بائیں++;
    جب( دائیں > بائیں && str[دائیں-1] == ' ' )
        دائیں--;
    واپس بائیں >= دائیں ? String("") : str.substr(بائیں, دائیں-بائیں);
}
}

ساخت Command_سطر_Parser_Params
{
عوامی:
    String help_پیغام;
    String def_قدر;
    std::vector<String> کلیدیں;
    صحیح نمبر;
};


ساخت Command_سطر_Parser::Impl
{
    منطقی error;
    String error_پیغام;
    String about_پیغام;

    String راستہ_مقصد_app;
    String app_name;

    std::vector<Command_سطر_Parser_Params> ڈیٹا;

    std::vector<String> بانٹو_range_string(ثابت String& str, حرف fs, حرف ss) ثابت;
    std::vector<String> بانٹو_string(ثابت String& str, حرف علامت = ' ', منطقی create_خالی_خانے_شے = جھوٹ) ثابت;

    خالی apply_params(ثابت String& کلید, ثابت String& value);
    خالی apply_params(صحیح i, String value);

    خالی ترتیب_دو_params();
    صحیح refcount;
};


ساکن ثابت حرف* get_type_name(Param type)
{
    اگر( type == Param::INT )
        واپس "int";
    اگر( type == Param::BOOLEAN )
        واپس "bool";
    اگر( type == Param::UNSIGNED_INT )
        واپس "unsigned";
    اگر( type == Param::UINT64 )
        واپس "unsigned long long";
    اگر( type == Param::FLOAT )
        واپس "float";
    اگر( type == Param::REAL )
        واپس "double";
    اگر( type == Param::STRING )
        واپس "string";
    واپس "unknown";
}

ساکن منطقی parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    منطقی value = جھوٹ;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    اگر (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    واپس value;
}

ساکن خالی from_str(ثابت String& str, Param type, خالی* dst)
{
    std::stringstream ss(str.c_str());
    اگر( type == Param::INT )
        ss >> *(صحیح*)dst;
    ورنہ اگر( type == Param::BOOLEAN )
    {
        std::string عارضی_2;
        ss >> عارضی_2;
        *(منطقی*) dst = parse_bool(عارضی_2);
    }
    ورنہ اگر( type == Param::UNSIGNED_INT )
        ss >> *(غیرعلامتی*)dst;
    ورنہ اگر( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ورنہ اگر( type == Param::FLOAT )
        ss >> *(اعشاری*)dst;
    ورنہ اگر( type == Param::REAL )
        ss >> *(دوہرا*)dst;
    ورنہ اگر( type == Param::STRING )
        *(String*)dst = str;
    ورنہ اگر( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        برائے (صحیح i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ورنہ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    اگر (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

خالی Command_سطر_Parser::getByName(ثابت String& name, منطقی space_delete, Param type, خالی* dst) ثابت
{
    کوشش
    {
        برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
        {
            برائے (size_t j = 0; j < impl->ڈیٹا[i].کلیدیں.size(); j++)
            {
                اگر (name == impl->ڈیٹا[i].کلیدیں[j])
                {
                    String v = impl->ڈیٹا[i].def_قدر;
                    اگر (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    اگر((v.empty() && type != Param::STRING) || v == none_قدر) {
                        impl->error = سچ;
                        impl->error_پیغام = impl->error_پیغام + "Missing parameter: '" + name + "'\n";
                        واپس;
                    }

                    from_str(v, type, dst);
                    واپس;
                }
            }
        }
    }
    پکڑو (ثابت Exception& e)
    {
        impl->error = سچ;
        impl->error_پیغام = impl->error_پیغام + "Parameter '"+ name + "': " + e.err + "\n";
        واپس;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


خالی Command_سطر_Parser::get_By_اشاریہ(صحیح اشاریہ, منطقی space_delete, Param type, خالی* dst) ثابت
{
    کوشش
    {
        برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
        {
            اگر (impl->ڈیٹا[i].نمبر == اشاریہ)
            {
                String v = impl->ڈیٹا[i].def_قدر;
                اگر (space_delete == سچ) v = cat_string(v);

                // the key was neither specified nor has a default value
                اگر((v.empty() && type != Param::STRING) || v == none_قدر) {
                    impl->error = سچ;
                    impl->error_پیغام = impl->error_پیغام + format("Missing parameter #%d\n", اشاریہ);
                    واپس;
                }
                from_str(v, type, dst);
                واپس;
            }
        }
    }
    پکڑو (ثابت Exception& e)
    {
        impl->error = سچ;
        impl->error_پیغام = impl->error_پیغام + format("Parameter #%d: ", اشاریہ) + e.err + "\n";
        واپس;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", اشاریہ));
}

ساکن منطقی cmp_params(ثابت Command_سطر_Parser_Params & p1, ثابت Command_سطر_Parser_Params & p2)
{
    اگر (p1.نمبر < p2.نمبر)
        واپس سچ;

    اگر (p1.نمبر > p2.نمبر)
        واپس جھوٹ;

    واپس p1.کلیدیں[0].compare(p2.کلیدیں[0]) < 0;
}

Command_سطر_Parser::Command_سطر_Parser(صحیح argc, ثابت حرف* ثابت argv[], ثابت String& کلیدیں)
{
    impl = نیا Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    اگر (pos_s == String::npos)
    {
        impl->راستہ_مقصد_app = "";
        impl->app_name = String(argv[0]);
    }
    ورنہ
    {
        impl->راستہ_مقصد_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = جھوٹ;
    impl->error_پیغام = "";

    // parse keys
    std::vector<String> k = impl->بانٹو_range_string(کلیدیں, '{', '}');

    صحیح jj = 0;
    برائے (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->بانٹو_string(k[i], '|', سچ);
        Command_سطر_Parser_Params p;
        p.کلیدیں = impl->بانٹو_string(l[0]);
        p.def_قدر = l[1];
        p.help_پیغام = cat_string(l[2]);
        p.نمبر = -1;
        اگر (p.کلیدیں.size() <= 0)
        {
            impl->error = سچ;
            impl->error_پیغام = "Field KEYS could not be empty\n";
        }
        ورنہ
        {
            اگر (p.کلیدیں[0][0] == '@')
            {
                p.نمبر = jj;
                jj++;
            }

            impl->ڈیٹا.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    برائے (صحیح i = 1; i < argc; i++)
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
        ورنہ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->ترتیب_دو_params();
}

Command_سطر_Parser::~Command_سطر_Parser()
{
    اگر (CV_XADD(&impl->refcount, -1) == 1)
        حذف impl;
}

Command_سطر_Parser::Command_سطر_Parser(ثابت Command_سطر_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_سطر_Parser& Command_سطر_Parser::عامل = (ثابت Command_سطر_Parser& parser)
{
    اگر( یہ != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        اگر(CV_XADD(&impl->refcount, -1) == 1)
            حذف impl;
        impl = parser.impl;
    }
    واپس *یہ;
}

خالی Command_سطر_Parser::about(ثابت String& message)
{
    impl->about_پیغام = message;
}

خالی Command_سطر_Parser::Impl::apply_params(ثابت String& کلید, ثابت String& value)
{
    برائے (size_t i = 0; i < ڈیٹا.size(); i++)
    {
        برائے (size_t k = 0; k < ڈیٹا[i].کلیدیں.size(); k++)
        {
            اگر (کلید.compare(ڈیٹا[i].کلیدیں[k]) == 0)
            {
                ڈیٹا[i].def_قدر = value;
                توڑو;
            }
        }
    }
}

خالی Command_سطر_Parser::Impl::apply_params(صحیح i, String value)
{
    برائے (size_t j = 0; j < ڈیٹا.size(); j++)
    {
        اگر (ڈیٹا[j].نمبر == i)
        {
            ڈیٹا[j].def_قدر = value;
            توڑو;
        }
    }
}

خالی Command_سطر_Parser::Impl::ترتیب_دو_params()
{
    برائے (size_t i = 0; i < ڈیٹا.size(); i++)
    {
        std::sort(ڈیٹا[i].کلیدیں.begin(), ڈیٹا[i].کلیدیں.end());
    }

    std::sort (ڈیٹا.begin(), ڈیٹا.end(), cmp_params);
}

String Command_سطر_Parser::get_راستہ_مقصد_Application() ثابت
{
    واپس impl->راستہ_مقصد_app;
}

منطقی Command_سطر_Parser::has(ثابت String& name) ثابت
{
    برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
    {
        برائے (size_t j = 0; j < impl->ڈیٹا[i].کلیدیں.size(); j++)
        {
            اگر (name == impl->ڈیٹا[i].کلیدیں[j])
            {
                ثابت String v = cat_string(impl->ڈیٹا[i].def_قدر);
                واپس !v.empty() && v != none_قدر;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

منطقی Command_سطر_Parser::check() ثابت
{
    واپس impl->error == جھوٹ;
}

خالی Command_سطر_Parser::printErrors() ثابت
{
    اگر (impl->error)
    {
        لکھو("\nERRORS:\n%s\n", impl->error_پیغام.c_str());
        fflush(stdout);
    }
}

خالی Command_سطر_Parser::print_پیغام() ثابت
{
    اگر (impl->about_پیغام != "")
        لکھو("%s\n", impl->about_پیغام.c_str());

    لکھو("Usage: %s [params] ", impl->app_name.c_str());

    برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
    {
        اگر (impl->ڈیٹا[i].نمبر > -1)
        {
            String name = impl->ڈیٹا[i].کلیدیں[0].substr(1, impl->ڈیٹا[i].کلیدیں[0].length() - 1);
            لکھو("%s ", name.c_str());
        }
    }

    لکھو("\n\n");

    برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
    {
        اگر (impl->ڈیٹا[i].نمبر == -1)
        {
            لکھو("\t");
            برائے (size_t j = 0; j < impl->ڈیٹا[i].کلیدیں.size(); j++)
            {
                String k = impl->ڈیٹا[i].کلیدیں[j];
                اگر (k.length() > 1)
                {
                    لکھو("--");
                }
                ورنہ
                {
                    لکھو("-");
                }
                لکھو("%s", k.c_str());

                اگر (j != impl->ڈیٹا[i].کلیدیں.size() - 1)
                {
                    لکھو(", ");
                }
            }
            String dv = cat_string(impl->ڈیٹا[i].def_قدر);
            اگر (dv.compare("") != 0)
            {
                لکھو(" (value:%s)", dv.c_str());
            }
            لکھو("\n\t\t%s\n", impl->ڈیٹا[i].help_پیغام.c_str());
        }
    }
    لکھو("\n");

    برائے (size_t i = 0; i < impl->ڈیٹا.size(); i++)
    {
        اگر (impl->ڈیٹا[i].نمبر != -1)
        {
            لکھو("\t");
            String k = impl->ڈیٹا[i].کلیدیں[0];
            k = k.substr(1, k.length() - 1);

            لکھو("%s", k.c_str());

            String dv = cat_string(impl->ڈیٹا[i].def_قدر);
            اگر (dv.compare("") != 0)
            {
                لکھو(" (value:%s)", dv.c_str());
            }
            لکھو("\n\t\t%s\n", impl->ڈیٹا[i].help_پیغام.c_str());
        }
    }
}

std::vector<String> Command_سطر_Parser::Impl::بانٹو_range_string(ثابت String& _str, حرف fs, حرف ss) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String لفظ = "";
    منطقی begin = جھوٹ;

    جب (!str.empty())
    {
        اگر (str[0] == fs)
        {
            اگر (begin == سچ)
            {
                پھینکو cv::Exception(cv::Error::StsParseError,
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

        اگر (str[0] == ss)
        {
            اگر (begin == جھوٹ)
            {
                پھینکو cv::Exception(cv::Error::StsParseError,
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
            begin = جھوٹ;
            vec.push_back(لفظ);
        }

        اگر (begin == سچ)
        {
            لفظ = لفظ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    اگر (begin == سچ)
    {
        پھینکو cv::Exception(cv::Error::StsParseError,
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

    واپس vec;
}

std::vector<String> Command_سطر_Parser::Impl::بانٹو_string(ثابت String& _str, حرف علامت, منطقی create_خالی_خانے_شے) ثابت
{
    String str = _str;
    std::vector<String> vec;
    String لفظ = "";

    جب (!str.empty())
    {
        اگر (str[0] == علامت)
        {
            اگر (!لفظ.empty() || create_خالی_خانے_شے)
            {
                vec.push_back(لفظ);
                لفظ = "";
            }
        }
        ورنہ
        {
            لفظ = لفظ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    اگر (لفظ != "" || create_خالی_خانے_شے)
    {
        vec.push_back(لفظ);
    }

    واپس vec;
}

}
