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
    ᬯᬮᬶ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ᬯᬮᬶ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ᬯᬮᬶ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ᬯᬮᬶ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ᬯᬮᬶ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ᬯᬮᬶ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ᬯᬮᬶ m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    for (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        ᬬᬾᬦ᭄ (!isSeparator)
        {
            ᬬᬾᬦ᭄ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ᬮᬶᬬ
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    for (const auto& startStop : startStops)
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
    ᬬᬾᬦ᭄ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ᬬᬾᬦ᭄ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ᬯᬮᬶ;
        }
        ᬮᬶᬬ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ᬯᬮᬶ;
        }
    }
    ᬬᬾᬦ᭄ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ᬯᬮᬶ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ᬬᬾᬦ᭄ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ᬯᬮᬶ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ᬬᬾᬦ᭄ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ᬯᬮᬶ;
    }
    ᬮᬶᬬ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ᬯᬮᬶ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ᬬᬾᬦ᭄ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ᬯᬮᬶ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ᬬᬾᬦ᭄ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ᬯᬮᬶ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ᬬᬾᬦ᭄ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ᬯᬮᬶ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ᬬᬾᬦ᭄ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ᬯᬮᬶ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ᬬᬾᬦ᭄ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ᬮᬶᬬ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ᬯᬮᬶ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ᬬᬾᬦ᭄ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ᬬᬾᬦ᭄ (len == 1u)
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ᬬᬾᬦ᭄ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ᬬᬾᬦ᭄ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ᬬᬾᬦ᭄ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ᬯᬮᬶ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ᬯᬮᬶ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ᬯᬮᬶ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ᬯᬮᬶ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ᬯᬮᬶ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ᬯᬮᬶ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ᬯᬮᬶ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ᬯᬮᬶ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ᬯᬮᬶ "VERBOSE";
    default:
        ᬯᬮᬶ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
