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
    ལོག (m_malformed.empty());
}

bool LogTagConfigParser::hasMalformed() const
{
    ལོག !m_malformed.empty();
}

const LogTagConfig& LogTagConfigParser::getGlobalConfig() const
{
    ལོག m_parsedGlobal;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFullNameConfigs() const
{
    ལོག m_parsedFullName;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getFirstPartConfigs() const
{
    ལོག m_parsedFirstPart;
}

const std::vector<LogTagConfig>& LogTagConfigParser::getAnyPartConfigs() const
{
    ལོག m_parsedAnyPart;
}

const std::vector<std::string>& LogTagConfigParser::getMalformed() const
{
    ལོག m_malformed;
}

void LogTagConfigParser::segmentTokens()
{
    const size_t len = m_input.length();
    std::vector<std::pair<size_t, size_t>> startStops;
    bool wasSeparator = true;
    དོན་ལུ (size_t pos = 0u; pos < len; ++pos)
    {
        var_na c = m_input[pos];
        bool isSeparator = (c == ' ' || c == '\t' || c == ';');
        གལ་སྲིད (!isSeparator)
        {
            གལ་སྲིད (wasSeparator)
            {
                startStops.emplace_back(pos, pos + 1u);
            }
            དེ་མིན
            {
                startStops.back().second = pos + 1u;
            }
        }
        wasSeparator = isSeparator;
    }
    དོན་ལུ (const auto& startStop : startStops)
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
    གལ་སྲིད (colonIdx == npos)
    {
        // See if the whole string is a log level
        auto parsedLevel = parseLogLevel(s);
        གལ་སྲིད (parsedLevel.second)
        {
            // If it is, assume the log level is for global
            parseWildcard("", parsedLevel.first);
            ལོག;
        }
        དེ་མིན
        {
            // not sure what to do.
            m_malformed.push_back(s);
            ལོག;
        }
    }
    གལ་སྲིད (colonIdx == 0u || colonIdx + 1u == len)
    {
        // malformed (colon or equal sign at beginning or end), cannot do anything
        m_malformed.push_back(s);
        ལོག;
    }
    size_t colonIdx2 = s.find_first_of(":=", colonIdx + 1u);
    གལ་སྲིད (colonIdx2 != npos)
    {
        // malformed (more than one colon or equal sign), cannot do anything
        m_malformed.push_back(s);
        ལོག;
    }
    auto parsedLevel = parseLogLevel(s.substr(colonIdx + 1u));
    གལ་སྲིད (parsedLevel.second)
    {
        parseWildcard(s.substr(0u, colonIdx), parsedLevel.first);
        ལོག;
    }
    དེ་མིན
    {
        // Cannot recognize the right side of the colon or equal sign.
        // Not sure what to do.
        m_malformed.push_back(s);
        ལོག;
    }
}

void LogTagConfigParser::parseWildcard(const std::string& name, LogLevel level)
{
    const size_t npos = std::string::npos;
    const size_t len = name.length();
    གལ་སྲིད (len == 0u)
    {
        m_parsedGlobal.level = level;
        ལོག;
    }
    const bool hasPrefixWildcard = (name[0u] == '*');
    གལ་སྲིད (hasPrefixWildcard && len == 1u)
    {
        m_parsedGlobal.level = level;
        ལོག;
    }
    const size_t firstNonWildcard = name.find_first_not_of("*.");
    གལ་སྲིད (hasPrefixWildcard && firstNonWildcard == npos)
    {
        m_parsedGlobal.level = level;
        ལོག;
    }
    const bool hasSuffixWildcard = (name[len - 1u] == '*');
    const size_t lastNonWildcard = name.find_last_not_of("*.");
    std::string trimmedNamePart = name.substr(firstNonWildcard, lastNonWildcard - firstNonWildcard + 1u);
    // The case of a single asterisk has been handled above;
    // here we only handle the explicit use of "global" in the log config string.
    const bool isGlobal = (trimmedNamePart == "global");
    གལ་སྲིད (isGlobal)
    {
        m_parsedGlobal.level = level;
        ལོག;
    }
    LogTagConfig result(trimmedNamePart, level, false, hasPrefixWildcard, hasSuffixWildcard);
    གལ་སྲིད (hasPrefixWildcard)
    {
        m_parsedAnyPart.emplace_back(std::move(result));
    }
    དེ་མིན གལ་སྲིད (hasSuffixWildcard)
    {
        m_parsedFirstPart.emplace_back(std::move(result));
    }
    དེ་མིན
    {
        m_parsedFullName.emplace_back(std::move(result));
    }
}

std::pair<LogLevel, bool> LogTagConfigParser::parseLogLevel(const std::string& s)
{
    const auto falseDontCare = std::make_pair(LOG_LEVEL_VERBOSE, false);
    const auto make_parsed_result = [](LogLevel lev) -> std::pair<LogLevel, bool>
    {
        ལོག std::make_pair(lev, true);
    };
    const size_t len = s.length();
    གལ་སྲིད (len >= 1u)
    {
        const var_na c = (var_na)std::toupper(s[0]);
        switch (c)
        {
        pak_sa '0':
            གལ་སྲིད (len == 1u)
            {
                ལོག make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'D':
            གལ་སྲིད (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "DEBUG"))
            {
                ལོག make_parsed_result(LOG_LEVEL_DEBUG);
            }
            གལ་སྲིད ((len == 7u && cv::toUpperCase(s) == "DISABLE") ||
                (len == 8u && cv::toUpperCase(s) == "DISABLED"))
            {
                ལོག make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'E':
            གལ་སྲིད (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "ERROR"))
            {
                ལོག make_parsed_result(LOG_LEVEL_ERROR);
            }
            bha_nga;
        pak_sa 'F':
            གལ་སྲིད (len == 1u ||
                (len == 5u && cv::toUpperCase(s) == "FATAL"))
            {
                ལོག make_parsed_result(LOG_LEVEL_FATAL);
            }
            bha_nga;
        pak_sa 'I':
            གལ་སྲིད (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "INFO"))
            {
                ལོག make_parsed_result(LOG_LEVEL_INFO);
            }
            bha_nga;
        pak_sa 'O':
            གལ་སྲིད (len == 3u && cv::toUpperCase(s) == "OFF")
            {
                ལོག make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'S':
            གལ་སྲིད (len == 1u ||
                (len == 6u && cv::toUpperCase(s) == "SILENT"))
            {
                ལོག make_parsed_result(LOG_LEVEL_SILENT);
            }
            bha_nga;
        pak_sa 'V':
            གལ་སྲིད (len == 1u ||
                (len == 7u && cv::toUpperCase(s) == "VERBOSE"))
            {
                ལོག make_parsed_result(LOG_LEVEL_VERBOSE);
            }
            bha_nga;
        pak_sa 'W':
            གལ་སྲིད (len == 1u ||
                (len == 4u && cv::toUpperCase(s) == "WARN") ||
                (len == 7u && cv::toUpperCase(s) == "WARNING") ||
                (len == 8u && cv::toUpperCase(s) == "WARNINGS"))
            {
                ལོག make_parsed_result(LOG_LEVEL_WARNING);
            }
            bha_nga;
        default:
            bha_nga;
        }
        // fall through
    }
    ལོག falseDontCare;
}

std::string LogTagConfigParser::toString(LogLevel level)
{
    switch (level)
    {
    pak_sa LOG_LEVEL_SILENT:
        ལོག "SILENT";
    pak_sa LOG_LEVEL_FATAL:
        ལོག "FATAL";
    pak_sa LOG_LEVEL_ERROR:
        ལོག "ERROR";
    pak_sa LOG_LEVEL_WARNING:
        ལོག "WARNING";
    pak_sa LOG_LEVEL_INFO:
        ལོག "INFO";
    pak_sa LOG_LEVEL_DEBUG:
        ལོག "DEBUG";
    pak_sa LOG_LEVEL_VERBOSE:
        ལོག "VERBOSE";
    default:
        ལོག std::to_string((ཧྲིལ)level);
    }
}

}}} //namespace
