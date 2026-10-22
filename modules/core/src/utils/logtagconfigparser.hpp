// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#define OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#اگر 1 // if not already in precompiled headers
#شامل <string>
#شامل <vector>
#شامل <functional>
#endif

#شامل <opencv2/core/utils/logtag.hpp>
#شامل "logtagconfig.hpp"

فضاینام cv {
فضاینام utils {
فضاینام logging {

رده LogTagConfigParser
{
عمومی:
    LogTagConfigParser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    explicit LogTagConfigParser(ثابت std::string& درون_داد_2);
    ~LogTagConfigParser();

عمومی:
    منطقی parse(ثابت std::string& درون_داد_2);
    منطقی hasMalformed() ثابت;
    ثابت LogTagConfig& getGlobalConfig() ثابت;
    ثابت std::vector<LogTagConfig>& getFullNameConfigs() ثابت;
    ثابت std::vector<LogTagConfig>& getFirstPartConfigs() ثابت;
    ثابت std::vector<LogTagConfig>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خصوصی:
    تهی segmentTokens();
    تهی parseNameAndLevel(ثابت std::string& s);
    تهی parseWildcard(ثابت std::string& name, LogLevel level);
    ایستا std::pair<LogLevel, منطقی> parseLogLevel(ثابت std::string& s);
    ایستا std::string toString(LogLevel level);

خصوصی:
    std::string m_درون_داد;
    LogTagConfig m_parsedGlobal;
    std::vector<LogTagConfig> m_parsedFullName;
    std::vector<LogTagConfig> m_parsedFirstPart;
    std::vector<LogTagConfig> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#endif
