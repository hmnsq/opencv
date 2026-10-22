// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_تعریف_نشده OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#تعریف_کن OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <vector>
#ضمن <functional>
#پایان_اگر

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

صنف Log_برچسب_Config_Parser
{
عام:
    Log_برچسب_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    صریح Log_برچسب_Config_Parser(ثابت std::string& درون_داد_2);
    ~Log_برچسب_Config_Parser();

عام:
    منطقي parse(ثابت std::string& درون_داد_2);
    منطقي hasMalformed() ثابت;
    ثابت Log_برچسب_Config& getGlobalConfig() ثابت;
    ثابت std::vector<Log_برچسب_Config>& get_پر_Name_Configs() ثابت;
    ثابت std::vector<Log_برچسب_Config>& get_اولی_Part_Configs() ثابت;
    ثابت std::vector<Log_برچسب_Config>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خاص:
    فراغ segment_نشانهها();
    فراغ parseNameAndLevel(ثابت std::string& s);
    فراغ parseWildcard(ثابت std::string& name, LogLevel level);
    ساكن std::pair<LogLevel, منطقي> parseLogLevel(ثابت std::string& s);
    ساكن std::string مقصد_String(LogLevel level);

خاص:
    std::string m_درون_داد;
    Log_برچسب_Config m_parsedGlobal;
    std::vector<Log_برچسب_Config> m_parsed_پر_Name;
    std::vector<Log_برچسب_Config> m_parsed_اولی_Part;
    std::vector<Log_برچسب_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#پایان_اگر
