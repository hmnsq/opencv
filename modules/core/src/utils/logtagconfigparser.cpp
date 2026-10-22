// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"
#شامل "logtagconfigparser.hpp"

فضاینام cv {
فضاینام utils {
فضاینام logging {

LogTagConfigParser::LogTagConfigParser(LogLevel defaultUnconfiguredGlobalLevel)
{
    m_parsedGlobal.namePart = "global";
    m_parsedGlobal.isGlobal = درست;
    m_parsedGlobal.hasPrefixWildcard = نادرست;
    m_parsedGlobal.hasSuffixWildcard = نادرست;
    m_parsedGlobal.level = defaultUnconfiguredGlobalLevel;
}

LogTagConfigParser::LogTagConfigParser(ثابت std::string& درون_داد_2)
{
    parse(درون_داد_2);
}

LogTagConfigParser::~LogTagConfigParser()
{
}

منطقی LogTagConfigParser::parse(ثابت std::string& درون_داد_2)
{
    m_درون_داد = درون_داد_2;
    segmentTokens();
    بازگشت (m_malformed.empty());
}

منطقی LogTagConfigParser::hasMalformed() ثابت
{
    بازگشت !m_malformed.empty();
}

ثابت LogTagConfig& LogTagConfigParser::getGlobalConfig() ثابت
{
    بازگشت m_parsedGlobal;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() ثابت
{
    بازگشت m_parsedFullName;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() ثابت
{
    بازگشت m_parsedFirstPart;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() ثابت
{
    بازگشت m_parsedAnyPart;
}

ثابت std::vector<std::string>& LogTagConfigParser::getMalformed() ثابت
{
    بازگشت m_malformed;
}

تهی LogTagConfigParser::segmentTokens()
{
    ثابت size_t len = m_درون_داد.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    منطقی wasSeparator = درست;
    برای (size_t pos = 0u; pos < len; ++pos)
    {
        نویسه c = m_درون_داد[pos];
        منطقی isSeparator = (c == ' ' || c == '\t' || c == ';');
        اگر (!isSeparator)
        {
            اگر (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            وگرنه
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    برای (ثابت auto& startStop : startStops)
    {
        ثابت auto s = m_درون_داد.substr(startStop.first, startStop.second - startStop.first);
        parseNameAndLevel(s);
    }
}

تهی LogTagConfigParser::parseNameAndLevel(ثابت std::string& s)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = s.length();
    size_t colonIdx = s.find_first_of(":=");
    اگر (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        اگر (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            بازگشت;
        }
        وگرنه
        {
            // not sure what to do.
            m_malformed.push_back(s);
            بازگشت;
        }
    }
    اگر (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        بازگشت;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    اگر (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        بازگشت;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    اگر (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        بازگشت;
    }
    وگرنه
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        بازگشت;
    }
}

تهی LogTagConfigParser::parseWildcard(ثابت std::string& name, LogLevel level)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = name.length();
    اگر (len == 0u)
    {
        m_parsedGlobal.level = level;
        بازگشت;
    }
    ثابت منطقی hasPrefixWildcard = (name[0u] == '*');
    اگر (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        بازگشت;
    }
    ثابت size_t firstNonWildcard = name.find_first_not_of("*.");
    اگر (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        بازگشت;
    }
    ثابت منطقی hasSuffixWildcard = (name[len - 1u] == '*');
    ثابت size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    ثابت منطقی isGlobal = (trimmedNamePart == "global");
    اگر (isGlobal)
    {
        m_parsedGlobal.level = level;
        بازگشت;
    }
    LogTagConfig result(trimmedNamePart, level, نادرست, hasPrefixWildcard, hasSuffixWildcard);
    اگر (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    وگرنه اگر (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    وگرنه
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, منطقی> LogTagConfigParser::parseLogLevel(ثابت std::string& s)
{
    ثابت auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, نادرست);
    ثابت auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, منطقی>
    {
        بازگشت std::make_pair(lev, درست);
    };
    ثابت size_t len = s.length();
    اگر (len >= 1u)
    {
        ثابت نویسه c = (نویسه)std::toupper(s[0]);
        انتخاب (c)
        {
        حالت '0':
            اگر (len == 1u)
            {
                بازگشت make_parsed_result(LOG_LEVEL_SILENT);
            }
            بشکن;
        حالت 'D':
            اگر (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_DEBUG);
            }
            اگر ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_SILENT);
            }
            بشکن;
        حالت 'E':
            اگر (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_ERROR);
            }
            بشکن;
        حالت 'F':
            اگر (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_FATAL);
            }
            بشکن;
        حالت 'I':
            اگر (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_INFO);
            }
            بشکن;
        حالت 'O':
            اگر (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                بازگشت make_parsed_result(LOG_LEVEL_SILENT);
            }
            بشکن;
        حالت 'S':
            اگر (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_SILENT);
            }
            بشکن;
        حالت 'V':
            اگر (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            بشکن;
        حالت 'W':
            اگر (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                بازگشت make_parsed_result(LOG_LEVEL_WARNING);
            }
            بشکن;
        پیشفرض:
            بشکن;
        }
        // fall through
    }
    بازگشت falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    انتخاب (level)
    {
    حالت LOG_LEVEL_SILENT:
        بازگشت "SILENT";
    حالت LOG_LEVEL_FATAL:
        بازگشت "FATAL";
    حالت LOG_LEVEL_ERROR:
        بازگشت "ERROR";
    حالت LOG_LEVEL_WARNING:
        بازگشت "WARNING";
    حالت LOG_LEVEL_INFO:
        بازگشت "INFO";
    حالت LOG_LEVEL_DEBUG:
        بازگشت "DEBUG";
    حالت LOG_LEVEL_VERBOSE:
        بازگشت "VERBOSE";
    پیشفرض:
        بازگشت std::to_string((صحیح)level);
    }
}

}}} //namespace
