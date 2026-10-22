// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#define OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <vector>
#ضمن <functional>
#endif

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

صنف LogTagConfigParser
{
عام:
    LogTagConfigParser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    explicit LogTagConfigParser(ثابت std::string& داخلہ_2);
    ~LogTagConfigParser();

عام:
    منطقي parse(ثابت std::string& داخلہ_2);
    منطقي hasMalformed() ثابت;
    ثابت LogTagConfig& getGlobalConfig() ثابت;
    ثابت std::vector<LogTagConfig>& getFullNameConfigs() ثابت;
    ثابت std::vector<LogTagConfig>& getFirstPartConfigs() ثابت;
    ثابت std::vector<LogTagConfig>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خاص:
    فراغ segmentTokens();
    فراغ parseNameAndLevel(ثابت std::string& s);
    فراغ parseWildcard(ثابت std::string& name, LogLevel level);
    ساكن std::pair<LogLevel, منطقي> parseLogLevel(ثابت std::string& s);
    ساكن std::string toString(LogLevel level);

خاص:
    std::string m_داخلہ;
    LogTagConfig m_parsedGlobal;
    std::vector<LogTagConfig> m_parsedFullName;
    std::vector<LogTagConfig> m_parsedFirstPart;
    std::vector<LogTagConfig> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#endif
