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
    ສົ່ງຄືນ (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ສົ່ງຄືນ !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ສົ່ງຄືນ m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ສົ່ງຄືນ m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ສົ່ງຄືນ m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ສົ່ງຄືນ m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ສົ່ງຄືນ m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    ສຳລັບ (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        ຖ້າ (!isSeparator)
        {
            ຖ້າ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    ສຳລັບ (const auto& startStop : startStops)
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
    ຖ້າ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        ຖ້າ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ສົ່ງຄືນ;
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ສົ່ງຄືນ;
        }
    }
    ຖ້າ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ສົ່ງຄືນ;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    ຖ້າ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ສົ່ງຄືນ;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    ຖ້າ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ສົ່ງຄືນ;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ສົ່ງຄືນ;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    ຖ້າ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ສົ່ງຄືນ;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    ຖ້າ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ສົ່ງຄືນ;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    ຖ້າ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ສົ່ງຄືນ;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    ຖ້າ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ສົ່ງຄືນ;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    ຖ້າ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ສົ່ງຄືນ std::make_pair(lev, true);
    };
    const size_t len = s.length();
    ຖ້າ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            ຖ້າ (len == 1u)
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            ຖ້າ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_DEBUG);
            }
            ຖ້າ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            ຖ້າ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            ຖ້າ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            ຖ້າ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            ຖ້າ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            ຖ້າ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            ຖ້າ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            ຖ້າ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ສົ່ງຄືນ make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ສົ່ງຄືນ falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ສົ່ງຄືນ "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ສົ່ງຄືນ "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ສົ່ງຄືນ "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ສົ່ງຄືນ "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ສົ່ງຄືນ "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ສົ່ງຄືນ "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ສົ່ງຄືນ "VERBOSE";
    default:
        ສົ່ງຄືນ std::to_string((ຈຳນວນເຕັມ)level);
    }
}

}}} //namespace
