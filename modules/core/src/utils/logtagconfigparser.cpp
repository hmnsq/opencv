// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

Log_برچسب_Config_Parser::Log_برچسب_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel)
{
    m_parsedGlobal.namePart = "global";
    m_parsedGlobal.isGlobal = درست;
    m_parsedGlobal.has_پیشوند_Wildcard = خطأ;
    m_parsedGlobal.hasSuffixWildcard = خطأ;
    m_parsedGlobal.level = defaultUnconfiguredGlobalLevel;
}

Log_برچسب_Config_Parser::Log_برچسب_Config_Parser(ثابت std::string& درون_داد_2)
{
    parse(درون_داد_2);
}

Log_برچسب_Config_Parser::~Log_برچسب_Config_Parser()
{
}

منطقي Log_برچسب_Config_Parser::parse(ثابت std::string& درون_داد_2)
{
    m_درون_داد = درون_داد_2;
    segment_نشانهها();
    أرجع (m_malformed.empty());
}

منطقي Log_برچسب_Config_Parser::hasMalformed() ثابت
{
    أرجع !m_malformed.empty();
}

ثابت Log_برچسب_Config& Log_برچسب_Config_Parser::getGlobalConfig() ثابت
{
    أرجع m_parsedGlobal;
}

ثابت std::vector<Log_برچسب_Config>& Log_برچسب_Config_Parser::get_پر_Name_Configs() ثابت
{
    أرجع m_parsed_پر_Name;
}

ثابت std::vector<Log_برچسب_Config>& Log_برچسب_Config_Parser::get_اولی_Part_Configs() ثابت
{
    أرجع m_parsed_اولی_Part;
}

ثابت std::vector<Log_برچسب_Config>& Log_برچسب_Config_Parser::getAnyPartConfigs() ثابت
{
    أرجع m_parsedAnyPart;
}

ثابت std::vector<std::string>& Log_برچسب_Config_Parser::getMalformed() ثابت
{
    أرجع m_malformed;
}

فراغ Log_برچسب_Config_Parser::segment_نشانهها()
{
    ثابت size_t len = m_درون_داد.length();
    std::vector<std::pair<size_t, size_t>> شروع_Stops;
    منطقي wasSeparator = درست;
    لكل (size_t pos = 0u; pos < len; ++pos)
    {
        حرف c = m_درون_داد[pos];
        منطقي isSeparator = (c == ' ' || c == '\t' || c == ';');
        إذا (!isSeparator)
        {
            إذا (wasSeparator)
            {
                شروع_Stops.emplace_back(pos, pos + 1u);
            }
            وإلا
            {
                شروع_Stops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    لكل (ثابت خودکار& شروع_Stop : شروع_Stops)
    {
        ثابت خودکار s = m_درون_داد.substr(شروع_Stop.first, شروع_Stop.second - شروع_Stop.first);
        parseNameAndLevel(s);
    }
}

فراغ Log_برچسب_Config_Parser::parseNameAndLevel(ثابت std::string& s)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = s.length();
    size_t colonIdx = s.find_first_of(":=");
    إذا (colonIdx == npos)
    {
        // See if the whole string is a log level
        خودکار parsedLevel = parseLogLevel(s);
        إذا (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            أرجع;
        }
        وإلا
        {
            // not sure what to do.
            m_malformed.push_back(s);
            أرجع;
        }
    }
    إذا (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        أرجع;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    إذا (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        أرجع;
    }
    خودکار parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    إذا (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        أرجع;
    }
    وإلا
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        أرجع;
    }
}

فراغ Log_برچسب_Config_Parser::parseWildcard(ثابت std::string& name, LogLevel level)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = name.length();
    إذا (len == 0u)
    {
        m_parsedGlobal.level = level;
        أرجع;
    }
    ثابت منطقي has_پیشوند_Wildcard = (name[0u] == '*');
    إذا (has_پیشوند_Wildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        أرجع;
    }
    ثابت size_t اولی_Non_Wildcard = name.find_first_not_of("*.");
    إذا (has_پیشوند_Wildcard && اولی_Non_Wildcard == npos)
    {
        m_parsedGlobal.level = level;
        أرجع;
    }
    ثابت منطقي hasSuffixWildcard = (name[len - 1u] == '*');
    ثابت size_t آخرین_Non_Wildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(اولی_Non_Wildcard, آخرین_Non_Wildcard - اولی_Non_Wildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    ثابت منطقي isGlobal = (trimmedNamePart == "global");
    إذا (isGlobal)
    {
        m_parsedGlobal.level = level;
        أرجع;
    }
    Log_برچسب_Config نتیجه(trimmedNamePart, level, خطأ, has_پیشوند_Wildcard, hasSuffixWildcard);
    إذا (has_پیشوند_Wildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(نتیجه));
    }
    وإلا إذا (hasSuffixWildcard)
    {
        m_parsed_اولی_Part.emplace_back(std::move(نتیجه));
    }
    وإلا
    {
        m_parsed_پر_Name.emplace_back(std::move(نتیجه));
    }
}

std::pair<LogLevel, منطقي> Log_برچسب_Config_Parser::parseLogLevel(ثابت std::string& s)
{
    ثابت خودکار falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, خطأ);
    ثابت خودکار make_parsed_نتیجه = [](LogLevel lev) -> std::pair<LogLevel, منطقي>
    {
        أرجع std::make_pair(lev, درست);
    };
    ثابت size_t len = s.length();
    إذا (len >= 1u)
    {
        ثابت حرف c = (حرف)std::به_بزرگ(s[0]);
        بدل (c)
        {
        حالة '0':
            إذا (len == 1u)
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'D':
            إذا (len == 1u ||
                (len == 5u && cv::مقصد_فوقانی_Case(s) == "DEBUG"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_DEBUG);
            }
            إذا ((len == 7u && cv::مقصد_فوقانی_Case(s) == "DISABLE") ||
                (len == 8u && cv::مقصد_فوقانی_Case(s) == "DISABLED"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'E':
            إذا (len == 1u ||
                (len == 5u && cv::مقصد_فوقانی_Case(s) == "ERROR"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_ERROR);
            }
            اكسر;
        حالة 'F':
            إذا (len == 1u ||
                (len == 5u && cv::مقصد_فوقانی_Case(s) == "FATAL"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_FATAL);
            }
            اكسر;
        حالة 'I':
            إذا (len == 1u ||
                (len == 4u && cv::مقصد_فوقانی_Case(s) == "INFO"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_INFO);
            }
            اكسر;
        حالة 'O':
            إذا (len == 3u && cv::مقصد_فوقانی_Case(s) == "OFF")
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'S':
            إذا (len == 1u ||
                (len == 6u && cv::مقصد_فوقانی_Case(s) == "SILENT"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'V':
            إذا (len == 1u ||
                (len == 7u && cv::مقصد_فوقانی_Case(s) == "VERBOSE"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_VERBOSE);
            }
            اكسر;
        حالة 'W':
            إذا (len == 1u ||
                (len == 4u && cv::مقصد_فوقانی_Case(s) == "WARN") ||
                (len == 7u && cv::مقصد_فوقانی_Case(s) == "WARNING") ||
                (len == 8u && cv::مقصد_فوقانی_Case(s) == "WARNINGS"))
            {
                أرجع make_parsed_نتیجه(LOG_LEVEL_WARNING);
            }
            اكسر;
        افتراضي:
            اكسر;
        }
        // fall through
    }
    أرجع falseDontCare;
}

std::string Log_برچسب_Config_Parser::مقصد_String(LogLevel level)
{
    بدل (level)
    {
    حالة LOG_LEVEL_SILENT:
        أرجع "SILENT";
    حالة LOG_LEVEL_FATAL:
        أرجع "FATAL";
    حالة LOG_LEVEL_ERROR:
        أرجع "ERROR";
    حالة LOG_LEVEL_WARNING:
        أرجع "WARNING";
    حالة LOG_LEVEL_INFO:
        أرجع "INFO";
    حالة LOG_LEVEL_DEBUG:
        أرجع "DEBUG";
    حالة LOG_LEVEL_VERBOSE:
        أرجع "VERBOSE";
    افتراضي:
        أرجع std::to_string((صحيح)level);
    }
}

}}} //namespace
