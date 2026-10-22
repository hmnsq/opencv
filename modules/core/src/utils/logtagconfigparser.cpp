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
    ပြန်ပေး (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ပြန်ပေး !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ပြန်ပေး m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ပြန်ပေး m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ပြန်ပေး m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ပြန်ပေး m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ပြန်ပေး m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    အတွက် (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        အကယ်၍ (!isSeparator)
        {
            အကယ်၍ (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            မဟုတ်ပါက
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    အတွက် (const auto& startStop : startStops)
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
    အကယ်၍ (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        အကယ်၍ (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ပြန်ပေး;
        }
        မဟုတ်ပါက
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ပြန်ပေး;
        }
    }
    အကယ်၍ (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ပြန်ပေး;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    အကယ်၍ (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ပြန်ပေး;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    အကယ်၍ (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ပြန်ပေး;
    }
    မဟုတ်ပါက
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ပြန်ပေး;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    အကယ်၍ (len == 0u)
    {
        m_parsedGlobal.level = level;
        ပြန်ပေး;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    အကယ်၍ (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ပြန်ပေး;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    အကယ်၍ (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ပြန်ပေး;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    အကယ်၍ (isGlobal)
    {
        m_parsedGlobal.level = level;
        ပြန်ပေး;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    အကယ်၍ (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    မဟုတ်ပါက အကယ်၍ (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    မဟုတ်ပါက
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ပြန်ပေး std::make_pair(lev, true);
    };
    const size_t len = s.length();
    အကယ်၍ (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            အကယ်၍ (len == 1u)
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            အကယ်၍ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_DEBUG);
            }
            အကယ်၍ ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            အကယ်၍ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            အကယ်၍ (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            အကယ်၍ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            အကယ်၍ (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            အကယ်၍ (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            အကယ်၍ (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            အကယ်၍ (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ပြန်ပေး make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ပြန်ပေး falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ပြန်ပေး "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ပြန်ပေး "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ပြန်ပေး "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ပြန်ပေး "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ပြန်ပေး "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ပြန်ပေး "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ပြန်ပေး "VERBOSE";
    default:
        ပြန်ပေး std::to_string((ကိန်းပြည့်)level);
    }
}

}}} //namespace
