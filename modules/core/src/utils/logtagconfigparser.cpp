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
    повернути (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    повернути !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    повернути m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    повернути m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    повернути m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    повернути m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    повернути m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    для (size_t pos = 0u; pos < len; ++pos)
    {
        char c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        якщо (!isSeparator)
        {
            якщо (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            інакше
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    для (const auto& startStop : startStops)
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
    якщо (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        якщо (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            повернути;
        }
        інакше
        {
            // not sure what to do.
            m_malformed.push_back(s);
            повернути;
        }
    }
    якщо (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        повернути;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    якщо (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        повернути;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    якщо (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        повернути;
    }
    інакше
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        повернути;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    якщо (len == 0u)
    {
        m_parsedGlobal.level = level;
        повернути;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    якщо (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        повернути;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    якщо (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        повернути;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    якщо (isGlobal)
    {
        m_parsedGlobal.level = level;
        повернути;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    якщо (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    інакше якщо (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    інакше
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        повернути std::make_pair(lev, true);
    };
    const size_t len = s.length();
    якщо (len >= 1u)
    {
        const char c = (char)std::toupper(s[0]);
        switch (c)
        {
        case '0':
            якщо (len == 1u)
            {
                повернути make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'D':
            якщо (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                повернути make_parsed_result(LOG_LEVEL_DEBUG);
            }
            якщо ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                повернути make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'E':
            якщо (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                повернути make_parsed_result(LOG_LEVEL_ERROR);
            }
            break;
        case 'F':
            якщо (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                повернути make_parsed_result(LOG_LEVEL_FATAL);
            }
            break;
        case 'I':
            якщо (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                повернути make_parsed_result(LOG_LEVEL_INFO);
            }
            break;
        case 'O':
            якщо (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                повернути make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'S':
            якщо (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                повернути make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'V':
            якщо (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                повернути make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            break;
        case 'W':
            якщо (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                повернути make_parsed_result(LOG_LEVEL_WARNING);
            }
            break;
        default:
            break;
        }
        // fall through
    }
    повернути falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_SILENT:
        повернути "SILENT";
    case LOG_LEVEL_FATAL:
        повернути "FATAL";
    case LOG_LEVEL_ERROR:
        повернути "ERROR";
    case LOG_LEVEL_WARNING:
        повернути "WARNING";
    case LOG_LEVEL_INFO:
        повернути "INFO";
    case LOG_LEVEL_DEBUG:
        повернути "DEBUG";
    case LOG_LEVEL_VERBOSE:
        повернути "VERBOSE";
    default:
        повернути std::to_string((ціле)level);
    }
}

}}} //namespace
