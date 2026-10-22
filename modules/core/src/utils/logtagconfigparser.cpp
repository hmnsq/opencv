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
    ආපසු (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ආපසු !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ආපසු m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ආපසු m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ආපසු m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ආපසු m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ආපසු m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    සඳහා (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        නම් (!isSeparator)
        {
            නම් (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            නැතහොත්
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    සඳහා (const auto& startStop : startStops)
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
    නම් (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        නම් (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ආපසු;
        }
        නැතහොත්
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ආපසු;
        }
    }
    නම් (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ආපසු;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    නම් (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ආපසු;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    නම් (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ආපසු;
    }
    නැතහොත්
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ආපසු;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    නම් (len == 0u)
    {
        m_parsedGlobal.level = level;
        ආපසු;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    නම් (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ආපසු;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    නම් (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ආපසු;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    නම් (isGlobal)
    {
        m_parsedGlobal.level = level;
        ආපසු;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    නම් (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    නැතහොත් නම් (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    නැතහොත්
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ආපසු std::make_pair(lev, true);
    };
    const size_t len = s.length();
    නම් (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            නම් (len == 1u)
            {
                ආපසු make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            නම් (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_DEBUG);
            }
            නම් ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            නම් (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            නම් (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            නම් (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            නම් (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ආපසු make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            නම් (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            නම් (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            නම් (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ආපසු make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ආපසු falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ආපසු "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ආපසු "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ආපසු "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ආපසු "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ආපසු "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ආපසු "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ආපසු "VERBOSE";
    default:
        ආපසු std::to_string((පූර්ණ_සංඛ්‍යා)level);
    }
}

}}} //namespace
