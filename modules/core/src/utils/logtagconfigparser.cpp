// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

LogTagConfigParser::LogTagConfigParser(LogLevel defaultUnconfiguredGlobalLevel)
{
    m_parsedGlobal.namePart = "global";
    m_parsedGlobal.isGlobal = אמת;
    m_parsedGlobal.hasPrefixWildcard = שקר;
    m_parsedGlobal.hasSuffixWildcard = שקר;
    m_parsedGlobal.level = defaultUnconfiguredGlobalLevel;
}

LogTagConfigParser::LogTagConfigParser(ثابت std::string& input)
{
    parse(input);
}

LogTagConfigParser::~LogTagConfigParser()
{
}

منطقي LogTagConfigParser::parse(ثابت std::string& input)
{
    m_input = input;
    segmentTokens();
    החזר (m_malformed.empty());
}

منطقي LogTagConfigParser::hasMalformed() ثابت
{
    החזר !m_malformed.empty();
}

ثابت LogTagConfig& LogTagConfigParser::getGlobalConfig() ثابت
{
    החזר m_parsedGlobal;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() ثابت
{
    החזר m_parsedFullName;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() ثابت
{
    החזר m_parsedFirstPart;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() ثابت
{
    החזר m_parsedAnyPart;
}

ثابت std::vector<std::string>& LogTagConfigParser::getMalformed() ثابت
{
    החזר m_malformed;
}

فراغ LogTagConfigParser::segmentTokens()
{
    ثابت size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    منطقي wasSeparator = אמת;
    עבור (size_t pos = 0u; pos < len; ++pos)
    {
        حرف c = m_input[pos];
        منطقي isSeparator = (c == ' ' || c == '\t' || c == ';');
        אם (!isSeparator)
        {
            אם (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            אחרת
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    עבור (ثابت auto& startStop : startStops)
    {
        ثابت auto s = m_input.substr(startStop.first, startStop.second - startStop.first);
        parseNameAndLevel(s);
    }
}

فراغ LogTagConfigParser::parseNameAndLevel(ثابت std::string& s)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = s.length();
    size_t colonIdx = s.find_first_of(":=");
    אם (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        אם (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            החזר;
        }
        אחרת
        {
            // not sure what to do.
            m_malformed.push_back(s);
            החזר;
        }
    }
    אם (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        החזר;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    אם (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        החזר;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    אם (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        החזר;
    }
    אחרת
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        החזר;
    }
}

فراغ LogTagConfigParser::parseWildcard(ثابت std::string& name, LogLevel level)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = name.length();
    אם (len == 0u)
    {
        m_parsedGlobal.level = level;
        החזר;
    }
    ثابت منطقي hasPrefixWildcard = (name[0u] == '*');
    אם (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        החזר;
    }
    ثابت size_t firstNonWildcard = name.find_first_not_of("*.");
    אם (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        החזר;
    }
    ثابت منطقي hasSuffixWildcard = (name[len - 1u] == '*');
    ثابت size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    ثابت منطقي isGlobal = (trimmedNamePart == "global");
    אם (isGlobal)
    {
        m_parsedGlobal.level = level;
        החזר;
    }
    LogTagConfig result(trimmedNamePart, level, שקר, hasPrefixWildcard, hasSuffixWildcard);
    אם (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    אחרת אם (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    אחרת
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, منطقي> LogTagConfigParser::parseLogLevel(ثابت std::string& s)
{
    ثابت auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, שקר);
    ثابت auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, منطقي>
    {
        החזר std::make_pair(lev, אמת);
    };
    ثابت size_t len = s.length();
    אם (len >= 1u)
    {
        ثابت حرف c = (حرف)std::toupper(s[0]);
        بدل (c)
        {
        حالة '0':
            אם (len == 1u)
            {
                החזר make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'D':
            אם (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                החזר make_parsed_result(LOG_LEVEL_DEBUG);
            }
            אם ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                החזר make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'E':
            אם (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                החזר make_parsed_result(LOG_LEVEL_ERROR);
            }
            اكسر;
        حالة 'F':
            אם (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                החזר make_parsed_result(LOG_LEVEL_FATAL);
            }
            اكسر;
        حالة 'I':
            אם (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                החזר make_parsed_result(LOG_LEVEL_INFO);
            }
            اكسر;
        حالة 'O':
            אם (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                החזר make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'S':
            אם (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                החזר make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'V':
            אם (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                החזר make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            اكسر;
        حالة 'W':
            אם (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                החזר make_parsed_result(LOG_LEVEL_WARNING);
            }
            اكسر;
        افتراضي:
            اكسر;
        }
        // fall through
    }
    החזר falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    بدل (level)
    {
    حالة LOG_LEVEL_SILENT:
        החזר "SILENT";
    حالة LOG_LEVEL_FATAL:
        החזר "FATAL";
    حالة LOG_LEVEL_ERROR:
        החזר "ERROR";
    حالة LOG_LEVEL_WARNING:
        החזר "WARNING";
    حالة LOG_LEVEL_INFO:
        החזר "INFO";
    حالة LOG_LEVEL_DEBUG:
        החזר "DEBUG";
    حالة LOG_LEVEL_VERBOSE:
        החזר "VERBOSE";
    افتراضي:
        החזר std::to_string((صحيح)level);
    }
}

}}} //namespace
