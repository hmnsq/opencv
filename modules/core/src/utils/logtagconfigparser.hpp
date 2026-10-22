// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#که_نه_تعریف_شوی OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#تعریف_کړه OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <vector>
#ضمن <functional>
#د_که_پای

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

صنف Log_نښان_Config_Parser
{
عام:
    Log_نښان_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    څرګند Log_نښان_Config_Parser(ثابت std::string& ننوت_2);
    ~Log_نښان_Config_Parser();

عام:
    منطقي parse(ثابت std::string& ننوت_2);
    منطقي hasMalformed() ثابت;
    ثابت Log_نښان_Config& getGlobalConfig() ثابت;
    ثابت std::vector<Log_نښان_Config>& get_ډک_Name_Configs() ثابت;
    ثابت std::vector<Log_نښان_Config>& get_لومړی_Part_Configs() ثابت;
    ثابت std::vector<Log_نښان_Config>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خاص:
    فراغ segment_نښې();
    فراغ parseNameAndLevel(ثابت std::string& s);
    فراغ parseWildcard(ثابت std::string& name, LogLevel level);
    ساكن std::pair<LogLevel, منطقي> parseLogLevel(ثابت std::string& s);
    ساكن std::string منزل_String(LogLevel level);

خاص:
    std::string m_ننوت;
    Log_نښان_Config m_parsedGlobal;
    std::vector<Log_نښان_Config> m_parsed_ډک_Name;
    std::vector<Log_نښان_Config> m_parsed_لومړی_Part;
    std::vector<Log_نښان_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#د_که_پای
