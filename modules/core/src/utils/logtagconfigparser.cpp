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
    ᱨᱩᱣᱟᱹᱲ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ᱨᱩᱣᱟᱹᱲ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ᱨᱩᱣᱟᱹᱲ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ᱨᱩᱣᱟᱹᱲ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ᱨᱩᱣᱟᱹᱲ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ᱨᱩᱣᱟᱹᱲ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ᱨᱩᱣᱟᱹᱲ m_malformed;
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
        ᱡᱩᱫᱤ (!isSeparator)
        {
            ᱡᱩᱫᱤ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ᱵᱟᱝ
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
    ᱡᱩᱫᱤ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ᱡᱩᱫᱤ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ᱨᱩᱣᱟᱹᱲ;
        }
        ᱵᱟᱝ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ᱨᱩᱣᱟᱹᱲ;
        }
    }
    ᱡᱩᱫᱤ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ᱨᱩᱣᱟᱹᱲ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ᱡᱩᱫᱤ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ᱨᱩᱣᱟᱹᱲ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ᱡᱩᱫᱤ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ᱨᱩᱣᱟᱹᱲ;
    }
    ᱵᱟᱝ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ᱨᱩᱣᱟᱹᱲ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ᱡᱩᱫᱤ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ᱨᱩᱣᱟᱹᱲ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ᱡᱩᱫᱤ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ᱨᱩᱣᱟᱹᱲ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ᱡᱩᱫᱤ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ᱨᱩᱣᱟᱹᱲ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ᱡᱩᱫᱤ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ᱨᱩᱣᱟᱹᱲ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ᱡᱩᱫᱤ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ᱵᱟᱝ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ᱨᱩᱣᱟᱹᱲ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ᱡᱩᱫᱤ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ᱡᱩᱫᱤ (len == 1u)
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ᱡᱩᱫᱤ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ᱡᱩᱫᱤ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ᱡᱩᱫᱤ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ᱨᱩᱣᱟᱹᱲ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ᱨᱩᱣᱟᱹᱲ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ᱨᱩᱣᱟᱹᱲ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ᱨᱩᱣᱟᱹᱲ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ᱨᱩᱣᱟᱹᱲ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ᱨᱩᱣᱟᱹᱲ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ᱨᱩᱣᱟᱹᱲ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ᱨᱩᱣᱟᱹᱲ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ᱨᱩᱣᱟᱹᱲ "VERBOSE";
    default:
        ᱨᱩᱣᱟᱹᱲ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
