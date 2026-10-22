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
    m_parsedGlobal.isGlobal = true;
    m_parsedGlobal.hasPrefixWildcard = خطأ;
    m_parsedGlobal.hasSuffixWildcard = خطأ;
    m_parsedGlobal.level = defaultUnconfiguredGlobalLevel;
}

LogTagConfigParser::LogTagConfigParser(ثابت std::string& إدخال_2)
{
    parse(إدخال_2);
}

LogTagConfigParser::~LogTagConfigParser()
{
}

منطقي LogTagConfigParser::parse(ثابت std::string& إدخال_2)
{
    m_إدخال = إدخال_2;
    segmentTokens();
    إرجاع (m_malformed.empty());
}

منطقي LogTagConfigParser::hasMalformed() ثابت
{
    إرجاع !m_malformed.empty();
}

ثابت LogTagConfig& LogTagConfigParser::getGlobalConfig() ثابت
{
    إرجاع m_parsedGlobal;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() ثابت
{
    إرجاع m_parsedFullName;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() ثابت
{
    إرجاع m_parsedFirstPart;
}

ثابت std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() ثابت
{
    إرجاع m_parsedAnyPart;
}

ثابت std::vector<std::string>& LogTagConfigParser::getMalformed() ثابت
{
    إرجاع m_malformed;
}

فراغ LogTagConfigParser::segmentTokens()
{
    ثابت size_t len = m_إدخال.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    منطقي wasSeparator = true;
    لكل (size_t pos = 0u; pos < len; ++pos)
    {
        حرف c = m_إدخال[pos];
        منطقي isSeparator = (c == ' ' || c == '\t' || c == ';');
        إذا (!isSeparator)
        {
            إذا (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            وإلا
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    لكل (ثابت auto& startStop : startStops)
    {
        ثابت auto s = m_إدخال.substr(startStop.first, startStop.second - startStop.first);
        parseNameAndLevel(s);
    }
}

فراغ LogTagConfigParser::parseNameAndLevel(ثابت std::string& s)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = s.length();
    size_t colonIdx = s.find_first_of(":=");
    إذا (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        إذا (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            إرجاع;
        }
        وإلا
        {
            // not sure what to do.
            m_malformed.push_back(s);
            إرجاع;
        }
    }
    إذا (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        إرجاع;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    إذا (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        إرجاع;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    إذا (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        إرجاع;
    }
    وإلا
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        إرجاع;
    }
}

فراغ LogTagConfigParser::parseWildcard(ثابت std::string& name, LogLevel level)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = name.length();
    إذا (len == 0u)
    {
        m_parsedGlobal.level = level;
        إرجاع;
    }
    ثابت منطقي hasPrefixWildcard = (name[0u] == '*');
    إذا (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        إرجاع;
    }
    ثابت size_t firstNonWildcard = name.find_first_not_of("*.");
    إذا (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        إرجاع;
    }
    ثابت منطقي hasSuffixWildcard = (name[len - 1u] == '*');
    ثابت size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    ثابت منطقي isGlobal = (trimmedNamePart == "global");
    إذا (isGlobal)
    {
        m_parsedGlobal.level = level;
        إرجاع;
    }
    LogTagConfig result(trimmedNamePart, level, خطأ, hasPrefixWildcard, hasSuffixWildcard);
    إذا (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    وإلا إذا (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    وإلا
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, منطقي> LogTagConfigParser::parseLogLevel(ثابت std::string& s)
{
    ثابت auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, خطأ);
    ثابت auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, منطقي>
    {
        إرجاع std::make_pair(lev, true);
    };
    ثابت size_t len = s.length();
    إذا (len >= 1u)
    {
        ثابت حرف c = (حرف)std::toupper(s[0]);
        بدل (c)
        {
        حالة '0':
            إذا (len == 1u)
            {
                إرجاع make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'D':
            إذا (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_DEBUG);
            }
            إذا ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'E':
            إذا (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_ERROR);
            }
            اكسر;
        حالة 'F':
            إذا (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_FATAL);
            }
            اكسر;
        حالة 'I':
            إذا (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_INFO);
            }
            اكسر;
        حالة 'O':
            إذا (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                إرجاع make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'S':
            إذا (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_SILENT);
            }
            اكسر;
        حالة 'V':
            إذا (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            اكسر;
        حالة 'W':
            إذا (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                إرجاع make_parsed_result(LOG_LEVEL_WARNING);
            }
            اكسر;
        افتراضي:
            اكسر;
        }
        // fall through
    }
    إرجاع falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    بدل (level)
    {
    حالة LOG_LEVEL_SILENT:
        إرجاع "SILENT";
    حالة LOG_LEVEL_FATAL:
        إرجاع "FATAL";
    حالة LOG_LEVEL_ERROR:
        إرجاع "ERROR";
    حالة LOG_LEVEL_WARNING:
        إرجاع "WARNING";
    حالة LOG_LEVEL_INFO:
        إرجاع "INFO";
    حالة LOG_LEVEL_DEBUG:
        إرجاع "DEBUG";
    حالة LOG_LEVEL_VERBOSE:
        إرجاع "VERBOSE";
    افتراضي:
        إرجاع std::to_string((صحيح)level);
    }
}

}}} //namespace
