// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>

ਨਾਮਾਕਾਸ਼ cv
{

ਨਾਮਾਕਾਸ਼ {
ਸਥਿਰ ਅਚਲ ਅੱਖਰ* none_ਮੁੱਲ = "<none>";

ਸਥਿਰ String cat_string(ਅਚਲ String& str)
{
    ਪੂਰਨ ਖੱਬੇ = 0, ਸੱਜੇ = (ਪੂਰਨ)str.length();
    ਜਦੋਂ( ਖੱਬੇ < ਸੱਜੇ && str[ਖੱਬੇ] == ' ' )
        ਖੱਬੇ++;
    ਜਦੋਂ( ਸੱਜੇ > ਖੱਬੇ && str[ਸੱਜੇ-1] == ' ' )
        ਸੱਜੇ--;
    ਵਾਪਸ ਖੱਬੇ >= ਸੱਜੇ ? String("") : str.substr(ਖੱਬੇ, ਸੱਜੇ-ਖੱਬੇ);
}
}

ਬਣਤਰ Command_ਸਤਰ_Parser_Params
{
ਜਨਤਕ:
    String help_ਸੁਨੇਹਾ;
    String def_ਮੁੱਲ;
    std::vector<String> ਕੁੰਜੀਆਂ;
    ਪੂਰਨ ਸੰਖਿਆ;
};


ਬਣਤਰ Command_ਸਤਰ_Parser::Impl
{
    ਤਾਰਕਿਕ error;
    String error_ਸੁਨੇਹਾ;
    String about_ਸੁਨੇਹਾ;

    String ਰਸਤਾ_ਮੰਜ਼ਿਲ_app;
    String app_name;

    std::vector<Command_ਸਤਰ_Parser_Params> ਅੰਕੜੇ;

    std::vector<String> ਵੰਡੋ_range_string(ਅਚਲ String& str, ਅੱਖਰ fs, ਅੱਖਰ ss) ਅਚਲ;
    std::vector<String> ਵੰਡੋ_string(ਅਚਲ String& str, ਅੱਖਰ ਚਿੰਨ੍ਹ = ' ', ਤਾਰਕਿਕ create_ਖਾਲੀ_ਥਾਵਾਂ_ਵਸਤੂ = ਝੂਠ) ਅਚਲ;

    ਖਾਲੀ apply_params(ਅਚਲ String& ਕੁੰਜੀ, ਅਚਲ String& value);
    ਖਾਲੀ apply_params(ਪੂਰਨ i, String value);

    ਖਾਲੀ ਛਾਂਟੋ_params();
    ਪੂਰਨ refcount;
};


ਸਥਿਰ ਅਚਲ ਅੱਖਰ* get_type_name(Param type)
{
    ਜੇ( type == Param::INT )
        ਵਾਪਸ "int";
    ਜੇ( type == Param::BOOLEAN )
        ਵਾਪਸ "bool";
    ਜੇ( type == Param::UNSIGNED_INT )
        ਵਾਪਸ "unsigned";
    ਜੇ( type == Param::UINT64 )
        ਵਾਪਸ "unsigned long long";
    ਜੇ( type == Param::FLOAT )
        ਵਾਪਸ "float";
    ਜੇ( type == Param::REAL )
        ਵਾਪਸ "double";
    ਜੇ( type == Param::STRING )
        ਵਾਪਸ "string";
    ਵਾਪਸ "unknown";
}

ਸਥਿਰ ਤਾਰਕਿਕ parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    ਤਾਰਕਿਕ value = ਝੂਠ;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ਜੇ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ਵਾਪਸ value;
}

ਸਥਿਰ ਖਾਲੀ from_str(ਅਚਲ String& str, Param type, ਖਾਲੀ* dst)
{
    std::stringstream ss(str.c_str());
    ਜੇ( type == Param::INT )
        ss >> *(ਪੂਰਨ*)dst;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::BOOLEAN )
    {
        std::string ਆਰਜ਼ੀ_2;
        ss >> ਆਰਜ਼ੀ_2;
        *(ਤਾਰਕਿਕ*) dst = parse_bool(ਆਰਜ਼ੀ_2);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::UNSIGNED_INT )
        ss >> *(ਬਿਨਾਨਿਸ਼ਾਨੀ*)dst;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::FLOAT )
        ss >> *(ਦਸ਼ਮਲਵ*)dst;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::REAL )
        ss >> *(ਦੁੱਗਣਾ*)dst;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::STRING )
        *(String*)dst = str;
    ਨਹੀਂਤਾਂ ਜੇ( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ਲਈ (ਪੂਰਨ i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ਜੇ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

ਖਾਲੀ Command_ਸਤਰ_Parser::getByName(ਅਚਲ String& name, ਤਾਰਕਿਕ space_delete, Param type, ਖਾਲੀ* dst) ਅਚਲ
{
    ਕੋਸ਼ਿਸ਼
    {
        ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
        {
            ਲਈ (size_t j = 0; j < impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.size(); j++)
            {
                ਜੇ (name == impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[j])
                {
                    String v = impl->ਅੰਕੜੇ[i].def_ਮੁੱਲ;
                    ਜੇ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ਜੇ((v.empty() && type != Param::STRING) || v == none_ਮੁੱਲ) {
                        impl->error = ਸੱਚ;
                        impl->error_ਸੁਨੇਹਾ = impl->error_ਸੁਨੇਹਾ + "Missing parameter: '" + name + "'\n";
                        ਵਾਪਸ;
                    }

                    from_str(v, type, dst);
                    ਵਾਪਸ;
                }
            }
        }
    }
    ਫੜੋ (ਅਚਲ Exception& e)
    {
        impl->error = ਸੱਚ;
        impl->error_ਸੁਨੇਹਾ = impl->error_ਸੁਨੇਹਾ + "Parameter '"+ name + "': " + e.err + "\n";
        ਵਾਪਸ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


ਖਾਲੀ Command_ਸਤਰ_Parser::get_By_ਸੂਚਕਾਂਕ(ਪੂਰਨ ਸੂਚਕਾਂਕ, ਤਾਰਕਿਕ space_delete, Param type, ਖਾਲੀ* dst) ਅਚਲ
{
    ਕੋਸ਼ਿਸ਼
    {
        ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
        {
            ਜੇ (impl->ਅੰਕੜੇ[i].ਸੰਖਿਆ == ਸੂਚਕਾਂਕ)
            {
                String v = impl->ਅੰਕੜੇ[i].def_ਮੁੱਲ;
                ਜੇ (space_delete == ਸੱਚ) v = cat_string(v);

                // the key was neither specified nor has a default value
                ਜੇ((v.empty() && type != Param::STRING) || v == none_ਮੁੱਲ) {
                    impl->error = ਸੱਚ;
                    impl->error_ਸੁਨੇਹਾ = impl->error_ਸੁਨੇਹਾ + format("Missing parameter #%d\n", ਸੂਚਕਾਂਕ);
                    ਵਾਪਸ;
                }
                from_str(v, type, dst);
                ਵਾਪਸ;
            }
        }
    }
    ਫੜੋ (ਅਚਲ Exception& e)
    {
        impl->error = ਸੱਚ;
        impl->error_ਸੁਨੇਹਾ = impl->error_ਸੁਨੇਹਾ + format("Parameter #%d: ", ਸੂਚਕਾਂਕ) + e.err + "\n";
        ਵਾਪਸ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", ਸੂਚਕਾਂਕ));
}

ਸਥਿਰ ਤਾਰਕਿਕ cmp_params(ਅਚਲ Command_ਸਤਰ_Parser_Params & p1, ਅਚਲ Command_ਸਤਰ_Parser_Params & p2)
{
    ਜੇ (p1.ਸੰਖਿਆ < p2.ਸੰਖਿਆ)
        ਵਾਪਸ ਸੱਚ;

    ਜੇ (p1.ਸੰਖਿਆ > p2.ਸੰਖਿਆ)
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ p1.ਕੁੰਜੀਆਂ[0].compare(p2.ਕੁੰਜੀਆਂ[0]) < 0;
}

Command_ਸਤਰ_Parser::Command_ਸਤਰ_Parser(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ* ਅਚਲ argv[], ਅਚਲ String& ਕੁੰਜੀਆਂ)
{
    impl = ਨਵਾਂ Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ਜੇ (pos_s == String::npos)
    {
        impl->ਰਸਤਾ_ਮੰਜ਼ਿਲ_app = "";
        impl->app_name = String(argv[0]);
    }
    ਨਹੀਂਤਾਂ
    {
        impl->ਰਸਤਾ_ਮੰਜ਼ਿਲ_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = ਝੂਠ;
    impl->error_ਸੁਨੇਹਾ = "";

    // parse keys
    std::vector<String> k = impl->ਵੰਡੋ_range_string(ਕੁੰਜੀਆਂ, '{', '}');

    ਪੂਰਨ jj = 0;
    ਲਈ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->ਵੰਡੋ_string(k[i], '|', ਸੱਚ);
        Command_ਸਤਰ_Parser_Params p;
        p.ਕੁੰਜੀਆਂ = impl->ਵੰਡੋ_string(l[0]);
        p.def_ਮੁੱਲ = l[1];
        p.help_ਸੁਨੇਹਾ = cat_string(l[2]);
        p.ਸੰਖਿਆ = -1;
        ਜੇ (p.ਕੁੰਜੀਆਂ.size() <= 0)
        {
            impl->error = ਸੱਚ;
            impl->error_ਸੁਨੇਹਾ = "Field KEYS could not be empty\n";
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (p.ਕੁੰਜੀਆਂ[0][0] == '@')
            {
                p.ਸੰਖਿਆ = jj;
                jj++;
            }

            impl->ਅੰਕੜੇ.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ਲਈ (ਪੂਰਨ i = 1; i < argc; i++)
    {
        String s(argv[i]);
        ਤਾਰਕਿਕ hasSingleDash = s.length() > 1 && s[0] == '-';

        ਜੇ (hasSingleDash)
        {
            ਤਾਰਕਿਕ hasDoubleDash = s.length() > 2 && s[1] == '-';
            String ਕੁੰਜੀ = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = ਕੁੰਜੀ.find('=');

            ਜੇ(equalsPos != String::npos) {
                value = ਕੁੰਜੀ.substr(equalsPos + 1);
                ਕੁੰਜੀ = ਕੁੰਜੀ.substr(0, equalsPos);
            }
            impl->apply_params(ਕੁੰਜੀ, value);
        }
        ਨਹੀਂਤਾਂ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->ਛਾਂਟੋ_params();
}

Command_ਸਤਰ_Parser::~Command_ਸਤਰ_Parser()
{
    ਜੇ (CV_XADD(&impl->refcount, -1) == 1)
        ਮਿਟਾਓ impl;
}

Command_ਸਤਰ_Parser::Command_ਸਤਰ_Parser(ਅਚਲ Command_ਸਤਰ_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_ਸਤਰ_Parser& Command_ਸਤਰ_Parser::ਸੰਚਾਲਕ = (ਅਚਲ Command_ਸਤਰ_Parser& parser)
{
    ਜੇ( ਇਹ != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ਜੇ(CV_XADD(&impl->refcount, -1) == 1)
            ਮਿਟਾਓ impl;
        impl = parser.impl;
    }
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ Command_ਸਤਰ_Parser::about(ਅਚਲ String& message)
{
    impl->about_ਸੁਨੇਹਾ = message;
}

ਖਾਲੀ Command_ਸਤਰ_Parser::Impl::apply_params(ਅਚਲ String& ਕੁੰਜੀ, ਅਚਲ String& value)
{
    ਲਈ (size_t i = 0; i < ਅੰਕੜੇ.size(); i++)
    {
        ਲਈ (size_t k = 0; k < ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.size(); k++)
        {
            ਜੇ (ਕੁੰਜੀ.compare(ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[k]) == 0)
            {
                ਅੰਕੜੇ[i].def_ਮੁੱਲ = value;
                ਰੋਕੋ;
            }
        }
    }
}

ਖਾਲੀ Command_ਸਤਰ_Parser::Impl::apply_params(ਪੂਰਨ i, String value)
{
    ਲਈ (size_t j = 0; j < ਅੰਕੜੇ.size(); j++)
    {
        ਜੇ (ਅੰਕੜੇ[j].ਸੰਖਿਆ == i)
        {
            ਅੰਕੜੇ[j].def_ਮੁੱਲ = value;
            ਰੋਕੋ;
        }
    }
}

ਖਾਲੀ Command_ਸਤਰ_Parser::Impl::ਛਾਂਟੋ_params()
{
    ਲਈ (size_t i = 0; i < ਅੰਕੜੇ.size(); i++)
    {
        std::sort(ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.begin(), ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.end());
    }

    std::sort (ਅੰਕੜੇ.begin(), ਅੰਕੜੇ.end(), cmp_params);
}

String Command_ਸਤਰ_Parser::get_ਰਸਤਾ_ਮੰਜ਼ਿਲ_Application() ਅਚਲ
{
    ਵਾਪਸ impl->ਰਸਤਾ_ਮੰਜ਼ਿਲ_app;
}

ਤਾਰਕਿਕ Command_ਸਤਰ_Parser::has(ਅਚਲ String& name) ਅਚਲ
{
    ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
    {
        ਲਈ (size_t j = 0; j < impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.size(); j++)
        {
            ਜੇ (name == impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[j])
            {
                ਅਚਲ String v = cat_string(impl->ਅੰਕੜੇ[i].def_ਮੁੱਲ);
                ਵਾਪਸ !v.empty() && v != none_ਮੁੱਲ;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

ਤਾਰਕਿਕ Command_ਸਤਰ_Parser::check() ਅਚਲ
{
    ਵਾਪਸ impl->error == ਝੂਠ;
}

ਖਾਲੀ Command_ਸਤਰ_Parser::printErrors() ਅਚਲ
{
    ਜੇ (impl->error)
    {
        ਲਿਖੋ("\nERRORS:\n%s\n", impl->error_ਸੁਨੇਹਾ.c_str());
        fflush(stdout);
    }
}

ਖਾਲੀ Command_ਸਤਰ_Parser::print_ਸੁਨੇਹਾ() ਅਚਲ
{
    ਜੇ (impl->about_ਸੁਨੇਹਾ != "")
        ਲਿਖੋ("%s\n", impl->about_ਸੁਨੇਹਾ.c_str());

    ਲਿਖੋ("Usage: %s [params] ", impl->app_name.c_str());

    ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
    {
        ਜੇ (impl->ਅੰਕੜੇ[i].ਸੰਖਿਆ > -1)
        {
            String name = impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[0].substr(1, impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[0].length() - 1);
            ਲਿਖੋ("%s ", name.c_str());
        }
    }

    ਲਿਖੋ("\n\n");

    ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
    {
        ਜੇ (impl->ਅੰਕੜੇ[i].ਸੰਖਿਆ == -1)
        {
            ਲਿਖੋ("\t");
            ਲਈ (size_t j = 0; j < impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.size(); j++)
            {
                String k = impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[j];
                ਜੇ (k.length() > 1)
                {
                    ਲਿਖੋ("--");
                }
                ਨਹੀਂਤਾਂ
                {
                    ਲਿਖੋ("-");
                }
                ਲਿਖੋ("%s", k.c_str());

                ਜੇ (j != impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ.size() - 1)
                {
                    ਲਿਖੋ(", ");
                }
            }
            String dv = cat_string(impl->ਅੰਕੜੇ[i].def_ਮੁੱਲ);
            ਜੇ (dv.compare("") != 0)
            {
                ਲਿਖੋ(" (value:%s)", dv.c_str());
            }
            ਲਿਖੋ("\n\t\t%s\n", impl->ਅੰਕੜੇ[i].help_ਸੁਨੇਹਾ.c_str());
        }
    }
    ਲਿਖੋ("\n");

    ਲਈ (size_t i = 0; i < impl->ਅੰਕੜੇ.size(); i++)
    {
        ਜੇ (impl->ਅੰਕੜੇ[i].ਸੰਖਿਆ != -1)
        {
            ਲਿਖੋ("\t");
            String k = impl->ਅੰਕੜੇ[i].ਕੁੰਜੀਆਂ[0];
            k = k.substr(1, k.length() - 1);

            ਲਿਖੋ("%s", k.c_str());

            String dv = cat_string(impl->ਅੰਕੜੇ[i].def_ਮੁੱਲ);
            ਜੇ (dv.compare("") != 0)
            {
                ਲਿਖੋ(" (value:%s)", dv.c_str());
            }
            ਲਿਖੋ("\n\t\t%s\n", impl->ਅੰਕੜੇ[i].help_ਸੁਨੇਹਾ.c_str());
        }
    }
}

std::vector<String> Command_ਸਤਰ_Parser::Impl::ਵੰਡੋ_range_string(ਅਚਲ String& _str, ਅੱਖਰ fs, ਅੱਖਰ ss) ਅਚਲ
{
    String str = _str;
    std::vector<String> vec;
    String ਸ਼ਬਦ = "";
    ਤਾਰਕਿਕ begin = ਝੂਠ;

    ਜਦੋਂ (!str.empty())
    {
        ਜੇ (str[0] == fs)
        {
            ਜੇ (begin == ਸੱਚ)
            {
                ਸੁੱਟੋ cv::Exception(cv::Error::StsParseError,
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
            begin = ਸੱਚ;
            ਸ਼ਬਦ = "";
            str = str.substr(1, str.length() - 1);
        }

        ਜੇ (str[0] == ss)
        {
            ਜੇ (begin == ਝੂਠ)
            {
                ਸੁੱਟੋ cv::Exception(cv::Error::StsParseError,
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
            begin = ਝੂਠ;
            vec.push_back(ਸ਼ਬਦ);
        }

        ਜੇ (begin == ਸੱਚ)
        {
            ਸ਼ਬਦ = ਸ਼ਬਦ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ਜੇ (begin == ਸੱਚ)
    {
        ਸੁੱਟੋ cv::Exception(cv::Error::StsParseError,
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

    ਵਾਪਸ vec;
}

std::vector<String> Command_ਸਤਰ_Parser::Impl::ਵੰਡੋ_string(ਅਚਲ String& _str, ਅੱਖਰ ਚਿੰਨ੍ਹ, ਤਾਰਕਿਕ create_ਖਾਲੀ_ਥਾਵਾਂ_ਵਸਤੂ) ਅਚਲ
{
    String str = _str;
    std::vector<String> vec;
    String ਸ਼ਬਦ = "";

    ਜਦੋਂ (!str.empty())
    {
        ਜੇ (str[0] == ਚਿੰਨ੍ਹ)
        {
            ਜੇ (!ਸ਼ਬਦ.empty() || create_ਖਾਲੀ_ਥਾਵਾਂ_ਵਸਤੂ)
            {
                vec.push_back(ਸ਼ਬਦ);
                ਸ਼ਬਦ = "";
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਸ਼ਬਦ = ਸ਼ਬਦ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ਜੇ (ਸ਼ਬਦ != "" || create_ਖਾਲੀ_ਥਾਵਾਂ_ਵਸਤੂ)
    {
        vec.push_back(ਸ਼ਬਦ);
    }

    ਵਾਪਸ vec;
}

}
