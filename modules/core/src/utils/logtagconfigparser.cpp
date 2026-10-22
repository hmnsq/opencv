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
    ᯀᯮᯞ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ᯀᯮᯞ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ᯀᯮᯞ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ᯀᯮᯞ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ᯀᯮᯞ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ᯀᯮᯞ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ᯀᯮᯞ m_malformed;
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
        ᯀᯮᯝ (!isSeparator)
        {
            ᯀᯮᯝ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ᯀᯮᯝᯂᯪ
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
    ᯀᯮᯝ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ᯀᯮᯝ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ᯀᯮᯞ;
        }
        ᯀᯮᯝᯂᯪ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ᯀᯮᯞ;
        }
    }
    ᯀᯮᯝ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ᯀᯮᯞ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ᯀᯮᯝ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ᯀᯮᯞ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ᯀᯮᯝ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ᯀᯮᯞ;
    }
    ᯀᯮᯝᯂᯪ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ᯀᯮᯞ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ᯀᯮᯝ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ᯀᯮᯞ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ᯀᯮᯝ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ᯀᯮᯞ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ᯀᯮᯝ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ᯀᯮᯞ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ᯀᯮᯝ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ᯀᯮᯞ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ᯀᯮᯝ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ᯀᯮᯝᯂᯪ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ᯀᯮᯞ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ᯀᯮᯝ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ᯀᯮᯝ (len == 1u)
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ᯀᯮᯝ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ᯀᯮᯝ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ᯀᯮᯝ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ᯀᯮᯝ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ᯀᯮᯝ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ᯀᯮᯝ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ᯀᯮᯝ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ᯀᯮᯝ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ᯀᯮᯝ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ᯀᯮᯞ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ᯀᯮᯞ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ᯀᯮᯞ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ᯀᯮᯞ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ᯀᯮᯞ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ᯀᯮᯞ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ᯀᯮᯞ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ᯀᯮᯞ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ᯀᯮᯞ "VERBOSE";
    default:
        ᯀᯮᯞ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
