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
    คืน (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    คืน !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    คืน m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    คืน m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    คืน m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    คืน m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    คืน m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    สำหรับ (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        ถ้า (!isSeparator)
        {
            ถ้า (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            มิฉะนั้น
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    สำหรับ (const auto& startStop : startStops)
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
    ถ้า (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ถ้า (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            คืน;
        }
        มิฉะนั้น
        {
            // not sure what to do.
            m_malformed.push_back(s);
            คืน;
        }
    }
    ถ้า (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        คืน;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ถ้า (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        คืน;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ถ้า (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        คืน;
    }
    มิฉะนั้น
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        คืน;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ถ้า (len == 0u)
    {
        m_parsedGlobal.level = level;
        คืน;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ถ้า (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        คืน;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ถ้า (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        คืน;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ถ้า (isGlobal)
    {
        m_parsedGlobal.level = level;
        คืน;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ถ้า (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    มิฉะนั้น ถ้า (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    มิฉะนั้น
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        คืน std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ถ้า (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ถ้า (len == 1u)
            {
                คืน make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ถ้า (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                คืน make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ถ้า ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                คืน make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ถ้า (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                คืน make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ถ้า (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                คืน make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ถ้า (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                คืน make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ถ้า (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                คืน make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ถ้า (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                คืน make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ถ้า (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                คืน make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ถ้า (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                คืน make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    คืน falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        คืน "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        คืน "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        คืน "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        คืน "WARNING";
    pak_sa LOG_LEVEL_INFO:
        คืน "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        คืน "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        คืน "VERBOSE";
    default:
        คืน std::to_string((จำนวนเต็ม)level);
    }
}

}}} //namespace
