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
    ᰀᰧᰌ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ᰀᰧᰌ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ᰀᰧᰌ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ᰀᰧᰌ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ᰀᰧᰌ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ᰀᰧᰌ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ᰀᰧᰌ m_malformed;
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
        ᰀᰧ (!isSeparator)
        {
            ᰀᰧ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ᰀᰧᰀ
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
    ᰀᰧ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ᰀᰧ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ᰀᰧᰌ;
        }
        ᰀᰧᰀ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ᰀᰧᰌ;
        }
    }
    ᰀᰧ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ᰀᰧᰌ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ᰀᰧ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ᰀᰧᰌ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ᰀᰧ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ᰀᰧᰌ;
    }
    ᰀᰧᰀ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ᰀᰧᰌ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ᰀᰧ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ᰀᰧᰌ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ᰀᰧ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ᰀᰧᰌ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ᰀᰧ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ᰀᰧᰌ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ᰀᰧ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ᰀᰧᰌ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ᰀᰧ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ᰀᰧᰀ ᰀᰧ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ᰀᰧᰀ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ᰀᰧᰌ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ᰀᰧ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ᰀᰧ (len == 1u)
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ᰀᰧ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ᰀᰧ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ᰀᰧ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ᰀᰧ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ᰀᰧ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ᰀᰧ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ᰀᰧ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ᰀᰧ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ᰀᰧ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ᰀᰧᰌ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ᰀᰧᰌ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ᰀᰧᰌ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ᰀᰧᰌ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ᰀᰧᰌ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ᰀᰧᰌ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ᰀᰧᰌ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ᰀᰧᰌ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ᰀᰧᰌ "VERBOSE";
    default:
        ᰀᰧᰌ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
