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
    戻る (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    戻る !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    戻る m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    戻る m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    戻る m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    戻る m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    戻る m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    ために (size_t pos = 0u; pos < len; ++pos)
    {
        char c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        もし (!isSeparator)
        {
            もし (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            否则
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    ために (const auto& startStop : startStops)
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
    もし (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        もし (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            戻る;
        }
        否则
        {
            // not sure what to do.
            m_malformed.push_back(s);
            戻る;
        }
    }
    もし (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        戻る;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    もし (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        戻る;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    もし (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        戻る;
    }
    否则
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        戻る;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    もし (len == 0u)
    {
        m_parsedGlobal.level = level;
        戻る;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    もし (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        戻る;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    もし (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        戻る;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    もし (isGlobal)
    {
        m_parsedGlobal.level = level;
        戻る;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    もし (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    否则 もし (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    否则
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        戻る std::make_pair(lev, true);
    };
    const size_t len = s.length();
    もし (len >= 1u)
    {
        const char c = (char)std::toupper(s[0]);
        switch (c)
        {
        case '0':
            もし (len == 1u)
            {
                戻る make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'D':
            もし (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                戻る make_parsed_result(LOG_LEVEL_DEBUG);
            }
            もし ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                戻る make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'E':
            もし (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                戻る make_parsed_result(LOG_LEVEL_ERROR);
            }
            break;
        case 'F':
            もし (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                戻る make_parsed_result(LOG_LEVEL_FATAL);
            }
            break;
        case 'I':
            もし (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                戻る make_parsed_result(LOG_LEVEL_INFO);
            }
            break;
        case 'O':
            もし (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                戻る make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'S':
            もし (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                戻る make_parsed_result(LOG_LEVEL_SILENT);
            }
            break;
        case 'V':
            もし (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                戻る make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            break;
        case 'W':
            もし (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                戻る make_parsed_result(LOG_LEVEL_WARNING);
            }
            break;
        default:
            break;
        }
        // fall through
    }
    戻る falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_SILENT:
        戻る "SILENT";
    case LOG_LEVEL_FATAL:
        戻る "FATAL";
    case LOG_LEVEL_ERROR:
        戻る "ERROR";
    case LOG_LEVEL_WARNING:
        戻る "WARNING";
    case LOG_LEVEL_INFO:
        戻る "INFO";
    case LOG_LEVEL_DEBUG:
        戻る "DEBUG";
    case LOG_LEVEL_VERBOSE:
        戻る "VERBOSE";
    default:
        戻る std::to_string((整数)level);
    }
}

}}} //namespace
