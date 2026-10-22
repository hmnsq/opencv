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
    ᤀᤡᤴ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ᤀᤡᤴ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ᤀᤡᤴ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ᤀᤡᤴ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ᤀᤡᤴ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ᤀᤡᤴ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ᤀᤡᤴ m_malformed;
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
        ᤀᤡ᤺ (!isSeparator)
        {
            ᤀᤡ᤺ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ᤀᤡ᤺ᤆᤡ
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
    ᤀᤡ᤺ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ᤀᤡ᤺ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ᤀᤡᤴ;
        }
        ᤀᤡ᤺ᤆᤡ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ᤀᤡᤴ;
        }
    }
    ᤀᤡ᤺ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ᤀᤡᤴ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ᤀᤡ᤺ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ᤀᤡᤴ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ᤀᤡ᤺ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ᤀᤡᤴ;
    }
    ᤀᤡ᤺ᤆᤡ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ᤀᤡᤴ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ᤀᤡ᤺ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ᤀᤡᤴ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ᤀᤡ᤺ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ᤀᤡᤴ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ᤀᤡ᤺ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ᤀᤡᤴ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ᤀᤡ᤺ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ᤀᤡᤴ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ᤀᤡ᤺ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ᤀᤡ᤺ᤆᤡ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ᤀᤡᤴ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ᤀᤡ᤺ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ᤀᤡ᤺ (len == 1u)
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ᤀᤡ᤺ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ᤀᤡ᤺ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ᤀᤡ᤺ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ᤀᤡ᤺ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ᤀᤡ᤺ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ᤀᤡ᤺ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ᤀᤡ᤺ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ᤀᤡ᤺ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ᤀᤡ᤺ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ᤀᤡᤴ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ᤀᤡᤴ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ᤀᤡᤴ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ᤀᤡᤴ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ᤀᤡᤴ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ᤀᤡᤴ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ᤀᤡᤴ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ᤀᤡᤴ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ᤀᤡᤴ "VERBOSE";
    default:
        ᤀᤡᤴ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
