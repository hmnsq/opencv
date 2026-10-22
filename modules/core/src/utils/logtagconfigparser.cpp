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
    върни (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    върни !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    върни m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    върни m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    върни m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    върни m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    върни m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    за (size_t pos = 0u; pos < len; ++pos)
    {
        char c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        ако (!isSeparator)
        {
            ако (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            иначе
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    за (const auto& startStop : startStops)
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
    ако (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ако (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            върни;
        }
        иначе
        {
            // not sure what to do.
            m_malformed.push_back(s);
            върни;
        }
    }
    ако (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        върни;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ако (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        върни;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ако (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        върни;
    }
    иначе
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        върни;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ако (len == 0u)
    {
        m_parsedGlobal.level = level;
        върни;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ако (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        върни;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ако (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        върни;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ако (isGlobal)
    {
        m_parsedGlobal.level = level;
        върни;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ако (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    иначе ако (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    иначе
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        върни std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ако (len >= 1u)
    {
        const char c = (char)std::toupper(s[0]);
        switch (c)
        {
        case '0':
            ако (len == 1u)
            {
                върни make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'D':
            ако (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                върни make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ако ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                върни make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'E':
            ако (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                върни make_parsed_result(LOG_LEVEL_ERROR);
            }
            break;
        case 'F':
            ако (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                върни make_parsed_result(LOG_LEVEL_FATAL);
            }
            break;
        case 'I':
            ако (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                върни make_parsed_result(LOG_LEVEL_INFO);
            }
            break;
        case 'O':
            ако (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                върни make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'S':
            ако (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                върни make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'V':
            ако (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                върни make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            break;
        case 'W':
            ако (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                върни make_parsed_result(LOG_LEVEL_WARNING);
            }
            break;
        default:
            break;
        }
        // fall through
    }
    върни falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_SILENT:
        върни "SILENT";
    case LOG_LEVEL_FATAL:
        върни "FATAL";
    case LOG_LEVEL_ERROR:
        върни "ERROR";
    case LOG_LEVEL_WARNING:
        върни "WARNING";
    case LOG_LEVEL_INFO:
        върни "INFO";
    case LOG_LEVEL_DEBUG:
        върни "DEBUG";
    case LOG_LEVEL_VERBOSE:
        върни "VERBOSE";
    default:
        върни std::to_string((цяло)level);
    }
}

}}} //namespace
