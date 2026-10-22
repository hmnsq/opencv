// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "../precomp.hpp"
#include "logtagconfigparser.hpp"

namespace cv {
namespace utils {
namespace logging {

LogTagConfigParser::LogTagConfigParser(LogLevel defaultUnconfiguredGlobalLevel)
{
    m_parsedGlobal.namePart = "global";
    m_parsedGlobal.isGlobal = true;
    m_parsedGlobal.hasPrefixWildcard = false;
    m_parsedGlobal.hasSuffixWildcard = false;
    m_parsedGlobal.level = defaultUnconfiguredGlobalLevel;
}

LogTagConfigParser::LogTagConfigParser(const std::string& input)
{
    parse(input);
}

LogTagConfigParser::~LogTagConfigParser()
{
}

bool LogTagConfigParser::parse(const std::string& input)
{
    m_input = input;
    segmentTokens();
    trả_về (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    trả_về !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    trả_về m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    trả_về m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    trả_về m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    trả_về m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    trả_về m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    cho (size_t pos = 0u; pos < len; ++pos)
    {
        char c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        nếu (!isSeparator)
        {
            nếu (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            không_thì
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    cho (const auto& startStop : startStops)
    {
        const auto s = m_input.substr(startStop.first, startStop.second - startStop.first);
        parseNameAndLevel(s);
    }
}

void LogTagConfigParser::parseNameAndLevel(const std::string& s)
{
    const size_t npos = std::string::npos;
    const size_t len = s.length();
    size_t colonIdx = s.find_first_of(":=");
    nếu (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        nếu (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            trả_về;
        }
        không_thì
        {
            // not sure what to do.
            m_malformed.push_back(s);
            trả_về;
        }
    }
    nếu (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        trả_về;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    nếu (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        trả_về;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    nếu (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        trả_về;
    }
    không_thì
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        trả_về;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    nếu (len == 0u)
    {
        m_parsedGlobal.level = level;
        trả_về;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    nếu (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        trả_về;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    nếu (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        trả_về;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    nếu (isGlobal)
    {
        m_parsedGlobal.level = level;
        trả_về;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    nếu (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    không_thì nếu (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    không_thì
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        trả_về std::make_pair(lev, true);
    };
    const size_t len = s.length();
    nếu (len >= 1u)
    {
        const char c = (char)std::toupper(s[0]);
        switch (c)
        {
        case '0':
            nếu (len == 1u)
            {
                trả_về make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'D':
            nếu (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                trả_về make_parsed_result(LOG_LEVEL_DEBUG);
            }
            nếu ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                trả_về make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'E':
            nếu (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                trả_về make_parsed_result(LOG_LEVEL_ERROR);
            }
            break;
        case 'F':
            nếu (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                trả_về make_parsed_result(LOG_LEVEL_FATAL);
            }
            break;
        case 'I':
            nếu (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                trả_về make_parsed_result(LOG_LEVEL_INFO);
            }
            break;
        case 'O':
            nếu (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                trả_về make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'S':
            nếu (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                trả_về make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'V':
            nếu (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                trả_về make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            break;
        case 'W':
            nếu (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                trả_về make_parsed_result(LOG_LEVEL_WARNING);
            }
            break;
        default:
            break;
        }
        // fall through
    }
    trả_về falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_SILENT:
        trả_về "SILENT";
    case LOG_LEVEL_FATAL:
        trả_về "FATAL";
    case LOG_LEVEL_ERROR:
        trả_về "ERROR";
    case LOG_LEVEL_WARNING:
        trả_về "WARNING";
    case LOG_LEVEL_INFO:
        trả_về "INFO";
    case LOG_LEVEL_DEBUG:
        trả_về "DEBUG";
    case LOG_LEVEL_VERBOSE:
        trả_về "VERBOSE";
    default:
        trả_về std::to_string((số_nguyên)level);
    }
}

}}} //namespace
