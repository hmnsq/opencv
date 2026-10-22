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
    ꦮꦭꦶ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ꦮꦭꦶ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ꦮꦭꦶ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ꦮꦭꦶ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ꦮꦭꦶ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ꦮꦭꦶ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ꦮꦭꦶ m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    ꦏꦁꦒꦺꦴ (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        ꦪꦼꦤ꧀ (!isSeparator)
        {
            ꦪꦼꦤ꧀ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ꦭꦶꦪ
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    ꦏꦁꦒꦺꦴ (const auto& startStop : startStops)
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
    ꦪꦼꦤ꧀ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ꦪꦼꦤ꧀ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ꦮꦭꦶ;
        }
        ꦭꦶꦪ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ꦮꦭꦶ;
        }
    }
    ꦪꦼꦤ꧀ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ꦮꦭꦶ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ꦪꦼꦤ꧀ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ꦮꦭꦶ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ꦪꦼꦤ꧀ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ꦮꦭꦶ;
    }
    ꦭꦶꦪ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ꦮꦭꦶ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ꦪꦼꦤ꧀ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ꦮꦭꦶ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ꦪꦼꦤ꧀ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ꦮꦭꦶ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ꦪꦼꦤ꧀ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ꦮꦭꦶ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ꦪꦼꦤ꧀ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ꦮꦭꦶ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ꦪꦼꦤ꧀ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ꦭꦶꦪ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ꦮꦭꦶ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ꦪꦼꦤ꧀ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ꦪꦼꦤ꧀ (len == 1u)
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ꦪꦼꦤ꧀ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ꦪꦼꦤ꧀ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ꦪꦼꦤ꧀ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ꦮꦭꦶ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ꦮꦭꦶ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ꦮꦭꦶ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ꦮꦭꦶ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ꦮꦭꦶ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ꦮꦭꦶ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ꦮꦭꦶ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ꦮꦭꦶ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ꦮꦭꦶ "VERBOSE";
    default:
        ꦮꦭꦶ std::to_string((puur_naa_nka)level);
    }
}

}}} //namespace
