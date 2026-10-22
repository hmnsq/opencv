// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <sstream>

ನಾಮಾಕಾಶ cv
{

ನಾಮಾಕಾಶ {
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* none_ಮೌಲ್ಯ = "<none>";

ಸ್ಥಿರ String cat_string(ಸ್ಥಿರಾಂಕ String& str)
{
    ಪೂರ್ಣಾಂಕ ಎಡ = 0, ಬಲ = (ಪೂರ್ಣಾಂಕ)str.length();
    ಆಗ( ಎಡ < ಬಲ && str[ಎಡ] == ' ' )
        ಎಡ++;
    ಆಗ( ಬಲ > ಎಡ && str[ಬಲ-1] == ' ' )
        ಬಲ--;
    ಹಿಂತಿರುಗಿ ಎಡ >= ಬಲ ? String("") : str.substr(ಎಡ, ಬಲ-ಎಡ);
}
}

ರಚನೆ Command_ಸಾಲು_Parser_Params
{
ಸಾರ್ವಜನಿಕ:
    String help_ಸಂದೇಶ;
    String def_ಮೌಲ್ಯ;
    std::vector<String> ಕೀಲಿಗಳು;
    ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ;
};


ರಚನೆ Command_ಸಾಲು_Parser::Impl
{
    ತಾರ್ಕಿಕ error;
    String error_ಸಂದೇಶ;
    String about_ಸಂದೇಶ;

    String ಮಾರ್ಗ_ಗಮ್ಯ_app;
    String app_name;

    std::vector<Command_ಸಾಲು_Parser_Params> ದತ್ತಾಂಶ;

    std::vector<String> ವಿಭಜಿಸು_range_string(ಸ್ಥಿರಾಂಕ String& str, ಅಕ್ಷರ fs, ಅಕ್ಷರ ss) ಸ್ಥಿರಾಂಕ;
    std::vector<String> ವಿಭಜಿಸು_string(ಸ್ಥಿರಾಂಕ String& str, ಅಕ್ಷರ ಚಿಹ್ನೆ = ' ', ತಾರ್ಕಿಕ create_ಖಾಲಿಗಳು_ಅಂಶ = ಸುಳ್ಳು) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ apply_params(ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ಸ್ಥಿರಾಂಕ String& value);
    ಶೂನ್ಯ apply_params(ಪೂರ್ಣಾಂಕ i, String value);

    ಶೂನ್ಯ ವಿಂಗಡಿಸು_params();
    ಪೂರ್ಣಾಂಕ refcount;
};


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* get_type_name(Param type)
{
    ಆದರೆ( type == Param::INT )
        ಹಿಂತಿರುಗಿ "int";
    ಆದರೆ( type == Param::BOOLEAN )
        ಹಿಂತಿರುಗಿ "bool";
    ಆದರೆ( type == Param::UNSIGNED_INT )
        ಹಿಂತಿರುಗಿ "unsigned";
    ಆದರೆ( type == Param::UINT64 )
        ಹಿಂತಿರುಗಿ "unsigned long long";
    ಆದರೆ( type == Param::FLOAT )
        ಹಿಂತಿರುಗಿ "float";
    ಆದರೆ( type == Param::REAL )
        ಹಿಂತಿರುಗಿ "double";
    ಆದರೆ( type == Param::STRING )
        ಹಿಂತಿರುಗಿ "string";
    ಹಿಂತಿರುಗಿ "unknown";
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    ತಾರ್ಕಿಕ value = ಸುಳ್ಳು;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ಆದರೆ (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ಹಿಂತಿರುಗಿ value;
}

ಸ್ಥಿರ ಶೂನ್ಯ from_str(ಸ್ಥಿರಾಂಕ String& str, Param type, ಶೂನ್ಯ* dst)
{
    std::stringstream ss(str.c_str());
    ಆದರೆ( type == Param::INT )
        ss >> *(ಪೂರ್ಣಾಂಕ*)dst;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::BOOLEAN )
    {
        std::string ತಾತ್ಕಾಲಿಕ_2;
        ss >> ತಾತ್ಕಾಲಿಕ_2;
        *(ತಾರ್ಕಿಕ*) dst = parse_bool(ತಾತ್ಕಾಲಿಕ_2);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::UNSIGNED_INT )
        ss >> *(ಚಿಹ್ನೆರಹಿತ*)dst;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::FLOAT )
        ss >> *(ದಶಮಾಂಶ*)dst;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::REAL )
        ss >> *(ದ್ವಿಗುಣ*)dst;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::STRING )
        *(String*)dst = str;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ಆದರೆ (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::getByName(ಸ್ಥಿರಾಂಕ String& name, ತಾರ್ಕಿಕ space_delete, Param type, ಶೂನ್ಯ* dst) ಸ್ಥಿರಾಂಕ
{
    ಪ್ರಯತ್ನಿಸು
    {
        ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
        {
            ಗಾಗಿ (size_t j = 0; j < impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.size(); j++)
            {
                ಆದರೆ (name == impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[j])
                {
                    String v = impl->ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ;
                    ಆದರೆ (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ಆದರೆ((v.empty() && type != Param::STRING) || v == none_ಮೌಲ್ಯ) {
                        impl->error = ನಿಜ;
                        impl->error_ಸಂದೇಶ = impl->error_ಸಂದೇಶ + "Missing parameter: '" + name + "'\n";
                        ಹಿಂತಿರುಗಿ;
                    }

                    from_str(v, type, dst);
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ Exception& e)
    {
        impl->error = ನಿಜ;
        impl->error_ಸಂದೇಶ = impl->error_ಸಂದೇಶ + "Parameter '"+ name + "': " + e.err + "\n";
        ಹಿಂತಿರುಗಿ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


ಶೂನ್ಯ Command_ಸಾಲು_Parser::get_By_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ತಾರ್ಕಿಕ space_delete, Param type, ಶೂನ್ಯ* dst) ಸ್ಥಿರಾಂಕ
{
    ಪ್ರಯತ್ನಿಸು
    {
        ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
        {
            ಆದರೆ (impl->ದತ್ತಾಂಶ[i].ಸಂಖ್ಯೆ == ಸೂಚ್ಯಂಕ)
            {
                String v = impl->ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ;
                ಆದರೆ (space_delete == ನಿಜ) v = cat_string(v);

                // the key was neither specified nor has a default value
                ಆದರೆ((v.empty() && type != Param::STRING) || v == none_ಮೌಲ್ಯ) {
                    impl->error = ನಿಜ;
                    impl->error_ಸಂದೇಶ = impl->error_ಸಂದೇಶ + format("Missing parameter #%d\n", ಸೂಚ್ಯಂಕ);
                    ಹಿಂತಿರುಗಿ;
                }
                from_str(v, type, dst);
                ಹಿಂತಿರುಗಿ;
            }
        }
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ Exception& e)
    {
        impl->error = ನಿಜ;
        impl->error_ಸಂದೇಶ = impl->error_ಸಂದೇಶ + format("Parameter #%d: ", ಸೂಚ್ಯಂಕ) + e.err + "\n";
        ಹಿಂತಿರುಗಿ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", ಸೂಚ್ಯಂಕ));
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ cmp_params(ಸ್ಥಿರಾಂಕ Command_ಸಾಲು_Parser_Params & p1, ಸ್ಥಿರಾಂಕ Command_ಸಾಲು_Parser_Params & p2)
{
    ಆದರೆ (p1.ಸಂಖ್ಯೆ < p2.ಸಂಖ್ಯೆ)
        ಹಿಂತಿರುಗಿ ನಿಜ;

    ಆದರೆ (p1.ಸಂಖ್ಯೆ > p2.ಸಂಖ್ಯೆ)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಹಿಂತಿರುಗಿ p1.ಕೀಲಿಗಳು[0].compare(p2.ಕೀಲಿಗಳು[0]) < 0;
}

Command_ಸಾಲು_Parser::Command_ಸಾಲು_Parser(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ argv[], ಸ್ಥಿರಾಂಕ String& ಕೀಲಿಗಳು)
{
    impl = ಹೊಸ Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ಆದರೆ (pos_s == String::npos)
    {
        impl->ಮಾರ್ಗ_ಗಮ್ಯ_app = "";
        impl->app_name = String(argv[0]);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        impl->ಮಾರ್ಗ_ಗಮ್ಯ_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = ಸುಳ್ಳು;
    impl->error_ಸಂದೇಶ = "";

    // parse keys
    std::vector<String> k = impl->ವಿಭಜಿಸು_range_string(ಕೀಲಿಗಳು, '{', '}');

    ಪೂರ್ಣಾಂಕ jj = 0;
    ಗಾಗಿ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->ವಿಭಜಿಸು_string(k[i], '|', ನಿಜ);
        Command_ಸಾಲು_Parser_Params p;
        p.ಕೀಲಿಗಳು = impl->ವಿಭಜಿಸು_string(l[0]);
        p.def_ಮೌಲ್ಯ = l[1];
        p.help_ಸಂದೇಶ = cat_string(l[2]);
        p.ಸಂಖ್ಯೆ = -1;
        ಆದರೆ (p.ಕೀಲಿಗಳು.size() <= 0)
        {
            impl->error = ನಿಜ;
            impl->error_ಸಂದೇಶ = "Field KEYS could not be empty\n";
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (p.ಕೀಲಿಗಳು[0][0] == '@')
            {
                p.ಸಂಖ್ಯೆ = jj;
                jj++;
            }

            impl->ದತ್ತಾಂಶ.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
    {
        String s(argv[i]);
        ತಾರ್ಕಿಕ hasSingleDash = s.length() > 1 && s[0] == '-';

        ಆದರೆ (hasSingleDash)
        {
            ತಾರ್ಕಿಕ hasDoubleDash = s.length() > 2 && s[1] == '-';
            String ಕೀಲಿ = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = ಕೀಲಿ.find('=');

            ಆದರೆ(equalsPos != String::npos) {
                value = ಕೀಲಿ.substr(equalsPos + 1);
                ಕೀಲಿ = ಕೀಲಿ.substr(0, equalsPos);
            }
            impl->apply_params(ಕೀಲಿ, value);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->ವಿಂಗಡಿಸು_params();
}

Command_ಸಾಲು_Parser::~Command_ಸಾಲು_Parser()
{
    ಆದರೆ (CV_XADD(&impl->refcount, -1) == 1)
        ಅಳಿಸು impl;
}

Command_ಸಾಲು_Parser::Command_ಸಾಲು_Parser(ಸ್ಥಿರಾಂಕ Command_ಸಾಲು_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_ಸಾಲು_Parser& Command_ಸಾಲು_Parser::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Command_ಸಾಲು_Parser& parser)
{
    ಆದರೆ( ಇದು != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ಆದರೆ(CV_XADD(&impl->refcount, -1) == 1)
            ಅಳಿಸು impl;
        impl = parser.impl;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::about(ಸ್ಥಿರಾಂಕ String& message)
{
    impl->about_ಸಂದೇಶ = message;
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::Impl::apply_params(ಸ್ಥಿರಾಂಕ String& ಕೀಲಿ, ಸ್ಥಿರಾಂಕ String& value)
{
    ಗಾಗಿ (size_t i = 0; i < ದತ್ತಾಂಶ.size(); i++)
    {
        ಗಾಗಿ (size_t k = 0; k < ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.size(); k++)
        {
            ಆದರೆ (ಕೀಲಿ.compare(ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[k]) == 0)
            {
                ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ = value;
                ನಿಲ್ಲಿಸು;
            }
        }
    }
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::Impl::apply_params(ಪೂರ್ಣಾಂಕ i, String value)
{
    ಗಾಗಿ (size_t j = 0; j < ದತ್ತಾಂಶ.size(); j++)
    {
        ಆದರೆ (ದತ್ತಾಂಶ[j].ಸಂಖ್ಯೆ == i)
        {
            ದತ್ತಾಂಶ[j].def_ಮೌಲ್ಯ = value;
            ನಿಲ್ಲಿಸು;
        }
    }
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::Impl::ವಿಂಗಡಿಸು_params()
{
    ಗಾಗಿ (size_t i = 0; i < ದತ್ತಾಂಶ.size(); i++)
    {
        std::sort(ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.begin(), ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.end());
    }

    std::sort (ದತ್ತಾಂಶ.begin(), ದತ್ತಾಂಶ.end(), cmp_params);
}

String Command_ಸಾಲು_Parser::get_ಮಾರ್ಗ_ಗಮ್ಯ_Application() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ impl->ಮಾರ್ಗ_ಗಮ್ಯ_app;
}

ತಾರ್ಕಿಕ Command_ಸಾಲು_Parser::has(ಸ್ಥಿರಾಂಕ String& name) ಸ್ಥಿರಾಂಕ
{
    ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
    {
        ಗಾಗಿ (size_t j = 0; j < impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.size(); j++)
        {
            ಆದರೆ (name == impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[j])
            {
                ಸ್ಥಿರಾಂಕ String v = cat_string(impl->ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ);
                ಹಿಂತಿರುಗಿ !v.empty() && v != none_ಮೌಲ್ಯ;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

ತಾರ್ಕಿಕ Command_ಸಾಲು_Parser::check() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ impl->error == ಸುಳ್ಳು;
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::printErrors() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (impl->error)
    {
        ಬರೆ("\nERRORS:\n%s\n", impl->error_ಸಂದೇಶ.c_str());
        fflush(stdout);
    }
}

ಶೂನ್ಯ Command_ಸಾಲು_Parser::print_ಸಂದೇಶ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (impl->about_ಸಂದೇಶ != "")
        ಬರೆ("%s\n", impl->about_ಸಂದೇಶ.c_str());

    ಬರೆ("Usage: %s [params] ", impl->app_name.c_str());

    ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
    {
        ಆದರೆ (impl->ದತ್ತಾಂಶ[i].ಸಂಖ್ಯೆ > -1)
        {
            String name = impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[0].substr(1, impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[0].length() - 1);
            ಬರೆ("%s ", name.c_str());
        }
    }

    ಬರೆ("\n\n");

    ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
    {
        ಆದರೆ (impl->ದತ್ತಾಂಶ[i].ಸಂಖ್ಯೆ == -1)
        {
            ಬರೆ("\t");
            ಗಾಗಿ (size_t j = 0; j < impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.size(); j++)
            {
                String k = impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[j];
                ಆದರೆ (k.length() > 1)
                {
                    ಬರೆ("--");
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಬರೆ("-");
                }
                ಬರೆ("%s", k.c_str());

                ಆದರೆ (j != impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು.size() - 1)
                {
                    ಬರೆ(", ");
                }
            }
            String dv = cat_string(impl->ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ);
            ಆದರೆ (dv.compare("") != 0)
            {
                ಬರೆ(" (value:%s)", dv.c_str());
            }
            ಬರೆ("\n\t\t%s\n", impl->ದತ್ತಾಂಶ[i].help_ಸಂದೇಶ.c_str());
        }
    }
    ಬರೆ("\n");

    ಗಾಗಿ (size_t i = 0; i < impl->ದತ್ತಾಂಶ.size(); i++)
    {
        ಆದರೆ (impl->ದತ್ತಾಂಶ[i].ಸಂಖ್ಯೆ != -1)
        {
            ಬರೆ("\t");
            String k = impl->ದತ್ತಾಂಶ[i].ಕೀಲಿಗಳು[0];
            k = k.substr(1, k.length() - 1);

            ಬರೆ("%s", k.c_str());

            String dv = cat_string(impl->ದತ್ತಾಂಶ[i].def_ಮೌಲ್ಯ);
            ಆದರೆ (dv.compare("") != 0)
            {
                ಬರೆ(" (value:%s)", dv.c_str());
            }
            ಬರೆ("\n\t\t%s\n", impl->ದತ್ತಾಂಶ[i].help_ಸಂದೇಶ.c_str());
        }
    }
}

std::vector<String> Command_ಸಾಲು_Parser::Impl::ವಿಭಜಿಸು_range_string(ಸ್ಥಿರಾಂಕ String& _str, ಅಕ್ಷರ fs, ಅಕ್ಷರ ss) ಸ್ಥಿರಾಂಕ
{
    String str = _str;
    std::vector<String> vec;
    String ಶಬ್ದ = "";
    ತಾರ್ಕಿಕ begin = ಸುಳ್ಳು;

    ಆಗ (!str.empty())
    {
        ಆದರೆ (str[0] == fs)
        {
            ಆದರೆ (begin == ನಿಜ)
            {
                ಎಸೆ cv::Exception(cv::Error::StsParseError,
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
            begin = ನಿಜ;
            ಶಬ್ದ = "";
            str = str.substr(1, str.length() - 1);
        }

        ಆದರೆ (str[0] == ss)
        {
            ಆದರೆ (begin == ಸುಳ್ಳು)
            {
                ಎಸೆ cv::Exception(cv::Error::StsParseError,
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
            begin = ಸುಳ್ಳು;
            vec.push_back(ಶಬ್ದ);
        }

        ಆದರೆ (begin == ನಿಜ)
        {
            ಶಬ್ದ = ಶಬ್ದ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ಆದರೆ (begin == ನಿಜ)
    {
        ಎಸೆ cv::Exception(cv::Error::StsParseError,
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

    ಹಿಂತಿರುಗಿ vec;
}

std::vector<String> Command_ಸಾಲು_Parser::Impl::ವಿಭಜಿಸು_string(ಸ್ಥಿರಾಂಕ String& _str, ಅಕ್ಷರ ಚಿಹ್ನೆ, ತಾರ್ಕಿಕ create_ಖಾಲಿಗಳು_ಅಂಶ) ಸ್ಥಿರಾಂಕ
{
    String str = _str;
    std::vector<String> vec;
    String ಶಬ್ದ = "";

    ಆಗ (!str.empty())
    {
        ಆದರೆ (str[0] == ಚಿಹ್ನೆ)
        {
            ಆದರೆ (!ಶಬ್ದ.empty() || create_ಖಾಲಿಗಳು_ಅಂಶ)
            {
                vec.push_back(ಶಬ್ದ);
                ಶಬ್ದ = "";
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಶಬ್ದ = ಶಬ್ದ + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ಆದರೆ (ಶಬ್ದ != "" || create_ಖಾಲಿಗಳು_ಅಂಶ)
    {
        vec.push_back(ಶಬ್ದ);
    }

    ಹಿಂತಿರುಗಿ vec;
}

}
