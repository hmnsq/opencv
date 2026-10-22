// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#اگر 1 // if not already in precompiled headers
#شامل <string>
#شامل <vector>
#شامل <functional>
#اگر_ختم

#شامل <opencv2/core/utils/logtag.hpp>
#شامل "logtagconfig.hpp"

نامخلا cv {
نامخلا utils {
نامخلا logging {

جماعت Log_لیبل_Config_Parser
{
عوامی:
    Log_لیبل_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    صریح Log_لیبل_Config_Parser(ثابت std::string& داخلہ_2);
    ~Log_لیبل_Config_Parser();

عوامی:
    منطقی parse(ثابت std::string& داخلہ_2);
    منطقی hasMalformed() ثابت;
    ثابت Log_لیبل_Config& getGlobalConfig() ثابت;
    ثابت std::vector<Log_لیبل_Config>& get_بھرا_Name_Configs() ثابت;
    ثابت std::vector<Log_لیبل_Config>& get_پہلا_Part_Configs() ثابت;
    ثابت std::vector<Log_لیبل_Config>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

نجی:
    خالی segment_علامات();
    خالی parseNameAndLevel(ثابت std::string& s);
    خالی parseWildcard(ثابت std::string& name, LogLevel level);
    ساکن std::pair<LogLevel, منطقی> parseLogLevel(ثابت std::string& s);
    ساکن std::string مقصد_String(LogLevel level);

نجی:
    std::string m_داخلہ;
    Log_لیبل_Config m_parsedGlobal;
    std::vector<Log_لیبل_Config> m_parsed_بھرا_Name;
    std::vector<Log_لیبل_Config> m_parsed_پہلا_Part;
    std::vector<Log_لیبل_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#اگر_ختم
