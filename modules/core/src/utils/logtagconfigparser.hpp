// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#وصف_ڪريو OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <vector>
#ضمن <functional>
#جيڪڏهن_پورو

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

صنف Log_ٽيگ_Config_Parser
{
عام:
    Log_ٽيگ_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    واضح Log_ٽيگ_Config_Parser(ثابت std::string& داخلا_2);
    ~Log_ٽيگ_Config_Parser();

عام:
    منطقي parse(ثابت std::string& داخلا_2);
    منطقي hasMalformed() ثابت;
    ثابت Log_ٽيگ_Config& getGlobalConfig() ثابت;
    ثابت std::vector<Log_ٽيگ_Config>& get_ڀريل_Name_Configs() ثابت;
    ثابت std::vector<Log_ٽيگ_Config>& get_پهريون_Part_Configs() ثابت;
    ثابت std::vector<Log_ٽيگ_Config>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خاص:
    فراغ segment_ٽوڪن();
    فراغ parseNameAndLevel(ثابت std::string& s);
    فراغ parseWildcard(ثابت std::string& name, LogLevel level);
    ساكن std::pair<LogLevel, منطقي> parseLogLevel(ثابت std::string& s);
    ساكن std::string منزل_String(LogLevel level);

خاص:
    std::string m_داخلا;
    Log_ٽيگ_Config m_parsedGlobal;
    std::vector<Log_ٽيگ_Config> m_parsed_ڀريل_Name;
    std::vector<Log_ٽيگ_Config> m_parsed_پهريون_Part;
    std::vector<Log_ٽيگ_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#جيڪڏهن_پورو
