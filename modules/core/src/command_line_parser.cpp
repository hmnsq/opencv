// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <sstream>

ନାମାକାଶ cv
{

ନାମାକାଶ {
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* none_ମୂଲ୍ୟ = "<none>";

ସ୍ଥିର String cat_string(ଅଚଳ String& str)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ = 0, ଡାହାଣ = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)str.length();
    ଯେତେବେଳେ( ବାମ < ଡାହାଣ && str[ବାମ] == ' ' )
        ବାମ++;
    ଯେତେବେଳେ( ଡାହାଣ > ବାମ && str[ଡାହାଣ-1] == ' ' )
        ଡାହାଣ--;
    ଫେରନ୍ତୁ ବାମ >= ଡାହାଣ ? String("") : str.substr(ବାମ, ଡାହାଣ-ବାମ);
}
}

ସଂରଚନା Command_ଧାଡ଼ି_Parser_Params
{
ସର୍ବସାଧାରଣ:
    String help_message;
    String def_ମୂଲ୍ୟ;
    std::vector<String> ଚାବିଗୁଡ଼ିକ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା;
};


ସଂରଚନା Command_ଧାଡ଼ି_Parser::Impl
{
    ତାର୍କିକ error;
    String error_message;
    String about_message;

    String path_to_app;
    String app_name;

    std::vector<Command_ଧାଡ଼ି_Parser_Params> data;

    std::vector<String> split_range_string(ଅଚଳ String& str, ଅକ୍ଷର fs, ଅକ୍ଷର ss) ଅଚଳ;
    std::vector<String> split_string(ଅଚଳ String& str, ଅକ୍ଷର ପ୍ରତୀକ = ' ', ତାର୍କିକ create_empty_item = ମିଥ୍ୟା) ଅଚଳ;

    ଶୂନ୍ୟ apply_params(ଅଚଳ String& ଚାବି, ଅଚଳ String& value);
    ଶୂନ୍ୟ apply_params(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, String value);

    ଶୂନ୍ୟ sort_params();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount;
};


ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* get_type_name(Param type)
{
    ଯଦି( type == Param::INT )
        ଫେରନ୍ତୁ "int";
    ଯଦି( type == Param::BOOLEAN )
        ଫେରନ୍ତୁ "bool";
    ଯଦି( type == Param::UNSIGNED_INT )
        ଫେରନ୍ତୁ "unsigned";
    ଯଦି( type == Param::UINT64 )
        ଫେରନ୍ତୁ "unsigned long long";
    ଯଦି( type == Param::FLOAT )
        ଫେରନ୍ତୁ "float";
    ଯଦି( type == Param::REAL )
        ଫେରନ୍ତୁ "double";
    ଯଦି( type == Param::STRING )
        ଫେରନ୍ତୁ "string";
    ଫେରନ୍ତୁ "unknown";
}

ସ୍ଥିର ତାର୍କିକ parse_bool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), details::char_tolower);
    std::istringstream is(str);
    ତାର୍କିକ value = ମିଥ୍ୟା;
    is >> (str.size() > 1 ? std::boolalpha : std::noboolalpha) >> value;

    ଯଦି (is.fail())
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [bool]", str.c_str()));

    ଫେରନ୍ତୁ value;
}

ସ୍ଥିର ଶୂନ୍ୟ from_str(ଅଚଳ String& str, Param type, ଶୂନ୍ୟ* dst)
{
    std::stringstream ss(str.c_str());
    ଯଦି( type == Param::INT )
        ss >> *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst;
    ଅନ୍ୟଥା ଯଦି( type == Param::BOOLEAN )
    {
        std::string temp;
        ss >> temp;
        *(ତାର୍କିକ*) dst = parse_bool(temp);
    }
    ଅନ୍ୟଥା ଯଦି( type == Param::UNSIGNED_INT )
        ss >> *(unsigned*)dst;
    ଅନ୍ୟଥା ଯଦି( type == Param::UINT64 )
        ss >> *(uint64*)dst;
    ଅନ୍ୟଥା ଯଦି( type == Param::FLOAT )
        ss >> *(ଦଶମିକ*)dst;
    ଅନ୍ୟଥା ଯଦି( type == Param::REAL )
        ss >> *(ଦ୍ୱିଗୁଣ*)dst;
    ଅନ୍ୟଥା ଯଦି( type == Param::STRING )
        *(String*)dst = str;
    ଅନ୍ୟଥା ଯଦି( type == Param::SCALAR)
    {
        Scalar& scalar = *(Scalar*)dst;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 4 && !ss.eof(); ++i)
            ss >> scalar[i];
    }
    ଅନ୍ୟଥା
        CV_Error(Error::StsBadArg, "unknown/unsupported parameter type");

    ଯଦି (ss.fail())
    {
        CV_Error_(Error::StsBadArg, ("can not convert: [%s] to [%s]", str.c_str(), get_type_name(type)));
    }
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::getByName(ଅଚଳ String& name, ତାର୍କିକ space_delete, Param type, ଶୂନ୍ୟ* dst) ଅଚଳ
{
    ଚେଷ୍ଟା
    {
        ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
        {
            ପାଇଁ (size_t j = 0; j < impl->data[i].ଚାବିଗୁଡ଼ିକ.size(); j++)
            {
                ଯଦି (name == impl->data[i].ଚାବିଗୁଡ଼ିକ[j])
                {
                    String v = impl->data[i].def_ମୂଲ୍ୟ;
                    ଯଦି (space_delete)
                        v = cat_string(v);

                    // the key was neither specified nor has a default value
                    ଯଦି((v.empty() && type != Param::STRING) || v == none_ମୂଲ୍ୟ) {
                        impl->error = ସତ୍ୟ;
                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
                        ଫେରନ୍ତୁ;
                    }

                    from_str(v, type, dst);
                    ଫେରନ୍ତୁ;
                }
            }
        }
    }
    ଧର (ଅଚଳ Exception& e)
    {
        impl->error = ସତ୍ୟ;
        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
        ଫେରନ୍ତୁ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}


ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::getByIndex(ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ତାର୍କିକ space_delete, Param type, ଶୂନ୍ୟ* dst) ଅଚଳ
{
    ଚେଷ୍ଟା
    {
        ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
        {
            ଯଦି (impl->data[i].ସଂଖ୍ୟା == index)
            {
                String v = impl->data[i].def_ମୂଲ୍ୟ;
                ଯଦି (space_delete == ସତ୍ୟ) v = cat_string(v);

                // the key was neither specified nor has a default value
                ଯଦି((v.empty() && type != Param::STRING) || v == none_ମୂଲ୍ୟ) {
                    impl->error = ସତ୍ୟ;
                    impl->error_message = impl->error_message + format("Missing parameter #%d\n", index);
                    ଫେରନ୍ତୁ;
                }
                from_str(v, type, dst);
                ଫେରନ୍ତୁ;
            }
        }
    }
    ଧର (ଅଚଳ Exception& e)
    {
        impl->error = ସତ୍ୟ;
        impl->error_message = impl->error_message + format("Parameter #%d: ", index) + e.err + "\n";
        ଫେରନ୍ତୁ;
    }

    CV_Error_(Error::StsBadArg, ("undeclared position %d requested", index));
}

ସ୍ଥିର ତାର୍କିକ cmp_params(ଅଚଳ Command_ଧାଡ଼ି_Parser_Params & p1, ଅଚଳ Command_ଧାଡ଼ି_Parser_Params & p2)
{
    ଯଦି (p1.ସଂଖ୍ୟା < p2.ସଂଖ୍ୟା)
        ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି (p1.ସଂଖ୍ୟା > p2.ସଂଖ୍ୟା)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ p1.ଚାବିଗୁଡ଼ିକ[0].compare(p2.ଚାବିଗୁଡ଼ିକ[0]) < 0;
}

Command_ଧାଡ଼ି_Parser::Command_ଧାଡ଼ି_Parser(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର* ଅଚଳ argv[], ଅଚଳ String& ଚାବିଗୁଡ଼ିକ)
{
    impl = ନୂତନ Impl;
    impl->refcount = 1;

    // path to application
    size_t pos_s = String(argv[0]).find_last_of("/\\");
    ଯଦି (pos_s == String::npos)
    {
        impl->path_to_app = "";
        impl->app_name = String(argv[0]);
    }
    ଅନ୍ୟଥା
    {
        impl->path_to_app = String(argv[0]).substr(0, pos_s);
        impl->app_name = String(argv[0]).substr(pos_s + 1, String(argv[0]).length() - pos_s);
    }

    impl->error = ମିଥ୍ୟା;
    impl->error_message = "";

    // parse keys
    std::vector<String> k = impl->split_range_string(ଚାବିଗୁଡ଼ିକ, '{', '}');

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା jj = 0;
    ପାଇଁ (size_t i = 0; i < k.size(); i++)
    {
        std::vector<String> l = impl->split_string(k[i], '|', ସତ୍ୟ);
        Command_ଧାଡ଼ି_Parser_Params p;
        p.ଚାବିଗୁଡ଼ିକ = impl->split_string(l[0]);
        p.def_ମୂଲ୍ୟ = l[1];
        p.help_message = cat_string(l[2]);
        p.ସଂଖ୍ୟା = -1;
        ଯଦି (p.ଚାବିଗୁଡ଼ିକ.size() <= 0)
        {
            impl->error = ସତ୍ୟ;
            impl->error_message = "Field KEYS could not be empty\n";
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (p.ଚାବିଗୁଡ଼ିକ[0][0] == '@')
            {
                p.ସଂଖ୍ୟା = jj;
                jj++;
            }

            impl->data.push_back(p);
        }
    }

    // parse argv
    jj = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++)
    {
        String s(argv[i]);
        ତାର୍କିକ hasSingleDash = s.length() > 1 && s[0] == '-';

        ଯଦି (hasSingleDash)
        {
            ତାର୍କିକ hasDoubleDash = s.length() > 2 && s[1] == '-';
            String ଚାବି = s.substr(hasDoubleDash ? 2 : 1);
            String value = "true";
            size_t equalsPos = ଚାବି.find('=');

            ଯଦି(equalsPos != String::npos) {
                value = ଚାବି.substr(equalsPos + 1);
                ଚାବି = ଚାବି.substr(0, equalsPos);
            }
            impl->apply_params(ଚାବି, value);
        }
        ଅନ୍ୟଥା
        {
            impl->apply_params(jj, s);
            jj++;
        }
    }

    impl->sort_params();
}

Command_ଧାଡ଼ି_Parser::~Command_ଧାଡ଼ି_Parser()
{
    ଯଦି (CV_XADD(&impl->refcount, -1) == 1)
        delete impl;
}

Command_ଧାଡ଼ି_Parser::Command_ଧାଡ଼ି_Parser(ଅଚଳ Command_ଧାଡ଼ି_Parser& parser)
{
    impl = parser.impl;
    CV_XADD(&impl->refcount, 1);
}

Command_ଧାଡ଼ି_Parser& Command_ଧାଡ଼ି_Parser::operator = (ଅଚଳ Command_ଧାଡ଼ି_Parser& parser)
{
    ଯଦି( ଏହି != &parser )
    {
        CV_XADD(&parser.impl->refcount, 1);
        ଯଦି(CV_XADD(&impl->refcount, -1) == 1)
            delete impl;
        impl = parser.impl;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::about(ଅଚଳ String& message)
{
    impl->about_message = message;
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::Impl::apply_params(ଅଚଳ String& ଚାବି, ଅଚଳ String& value)
{
    ପାଇଁ (size_t i = 0; i < data.size(); i++)
    {
        ପାଇଁ (size_t k = 0; k < data[i].ଚାବିଗୁଡ଼ିକ.size(); k++)
        {
            ଯଦି (ଚାବି.compare(data[i].ଚାବିଗୁଡ଼ିକ[k]) == 0)
            {
                data[i].def_ମୂଲ୍ୟ = value;
                ଭାଙ୍ଗ;
            }
        }
    }
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::Impl::apply_params(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, String value)
{
    ପାଇଁ (size_t j = 0; j < data.size(); j++)
    {
        ଯଦି (data[j].ସଂଖ୍ୟା == i)
        {
            data[j].def_ମୂଲ୍ୟ = value;
            ଭାଙ୍ଗ;
        }
    }
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::Impl::sort_params()
{
    ପାଇଁ (size_t i = 0; i < data.size(); i++)
    {
        std::sort(data[i].ଚାବିଗୁଡ଼ିକ.begin(), data[i].ଚାବିଗୁଡ଼ିକ.end());
    }

    std::sort (data.begin(), data.end(), cmp_params);
}

String Command_ଧାଡ଼ି_Parser::getPathToApplication() ଅଚଳ
{
    ଫେରନ୍ତୁ impl->path_to_app;
}

ତାର୍କିକ Command_ଧାଡ଼ି_Parser::has(ଅଚଳ String& name) ଅଚଳ
{
    ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
    {
        ପାଇଁ (size_t j = 0; j < impl->data[i].ଚାବିଗୁଡ଼ିକ.size(); j++)
        {
            ଯଦି (name == impl->data[i].ଚାବିଗୁଡ଼ିକ[j])
            {
                ଅଚଳ String v = cat_string(impl->data[i].def_ମୂଲ୍ୟ);
                ଫେରନ୍ତୁ !v.empty() && v != none_ମୂଲ୍ୟ;
            }
        }
    }

    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
}

ତାର୍କିକ Command_ଧାଡ଼ି_Parser::check() ଅଚଳ
{
    ଫେରନ୍ତୁ impl->error == ମିଥ୍ୟା;
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::printErrors() ଅଚଳ
{
    ଯଦି (impl->error)
    {
        printf("\nERRORS:\n%s\n", impl->error_message.c_str());
        fflush(stdout);
    }
}

ଶୂନ୍ୟ Command_ଧାଡ଼ି_Parser::printMessage() ଅଚଳ
{
    ଯଦି (impl->about_message != "")
        printf("%s\n", impl->about_message.c_str());

    printf("Usage: %s [params] ", impl->app_name.c_str());

    ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
    {
        ଯଦି (impl->data[i].ସଂଖ୍ୟା > -1)
        {
            String name = impl->data[i].ଚାବିଗୁଡ଼ିକ[0].substr(1, impl->data[i].ଚାବିଗୁଡ଼ିକ[0].length() - 1);
            printf("%s ", name.c_str());
        }
    }

    printf("\n\n");

    ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
    {
        ଯଦି (impl->data[i].ସଂଖ୍ୟା == -1)
        {
            printf("\t");
            ପାଇଁ (size_t j = 0; j < impl->data[i].ଚାବିଗୁଡ଼ିକ.size(); j++)
            {
                String k = impl->data[i].ଚାବିଗୁଡ଼ିକ[j];
                ଯଦି (k.length() > 1)
                {
                    printf("--");
                }
                ଅନ୍ୟଥା
                {
                    printf("-");
                }
                printf("%s", k.c_str());

                ଯଦି (j != impl->data[i].ଚାବିଗୁଡ଼ିକ.size() - 1)
                {
                    printf(", ");
                }
            }
            String dv = cat_string(impl->data[i].def_ମୂଲ୍ୟ);
            ଯଦି (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
    printf("\n");

    ପାଇଁ (size_t i = 0; i < impl->data.size(); i++)
    {
        ଯଦି (impl->data[i].ସଂଖ୍ୟା != -1)
        {
            printf("\t");
            String k = impl->data[i].ଚାବିଗୁଡ଼ିକ[0];
            k = k.substr(1, k.length() - 1);

            printf("%s", k.c_str());

            String dv = cat_string(impl->data[i].def_ମୂଲ୍ୟ);
            ଯଦି (dv.compare("") != 0)
            {
                printf(" (value:%s)", dv.c_str());
            }
            printf("\n\t\t%s\n", impl->data[i].help_message.c_str());
        }
    }
}

std::vector<String> Command_ଧାଡ଼ି_Parser::Impl::split_range_string(ଅଚଳ String& _str, ଅକ୍ଷର fs, ଅକ୍ଷର ss) ଅଚଳ
{
    String str = _str;
    std::vector<String> vec;
    String word = "";
    ତାର୍କିକ begin = ମିଥ୍ୟା;

    ଯେତେବେଳେ (!str.empty())
    {
        ଯଦି (str[0] == fs)
        {
            ଯଦି (begin == ସତ୍ୟ)
            {
                ଫିଙ୍ଗ cv::Exception(cv::Error::StsParseError,
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
            begin = ସତ୍ୟ;
            word = "";
            str = str.substr(1, str.length() - 1);
        }

        ଯଦି (str[0] == ss)
        {
            ଯଦି (begin == ମିଥ୍ୟା)
            {
                ଫିଙ୍ଗ cv::Exception(cv::Error::StsParseError,
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
            begin = ମିଥ୍ୟା;
            vec.push_back(word);
        }

        ଯଦି (begin == ସତ୍ୟ)
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ଯଦି (begin == ସତ୍ୟ)
    {
        ଫିଙ୍ଗ cv::Exception(cv::Error::StsParseError,
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

    ଫେରନ୍ତୁ vec;
}

std::vector<String> Command_ଧାଡ଼ି_Parser::Impl::split_string(ଅଚଳ String& _str, ଅକ୍ଷର ପ୍ରତୀକ, ତାର୍କିକ create_empty_item) ଅଚଳ
{
    String str = _str;
    std::vector<String> vec;
    String word = "";

    ଯେତେବେଳେ (!str.empty())
    {
        ଯଦି (str[0] == ପ୍ରତୀକ)
        {
            ଯଦି (!word.empty() || create_empty_item)
            {
                vec.push_back(word);
                word = "";
            }
        }
        ଅନ୍ୟଥା
        {
            word = word + str[0];
        }
        str = str.substr(1, str.length() - 1);
    }

    ଯଦି (word != "" || create_empty_item)
    {
        vec.push_back(word);
    }

    ଫେରନ୍ତୁ vec;
}

}
