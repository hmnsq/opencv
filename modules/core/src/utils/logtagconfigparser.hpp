// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جے_بنا_تعریف OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <vector>
#ضمن <functional>
#جے_انت

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

صنف Log_ٹیگ_Config_Parser
{
عام:
    Log_ٹیگ_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    واضح Log_ٹیگ_Config_Parser(ثابت std::string& داخلہ_2);
    ~Log_ٹیگ_Config_Parser();

عام:
    منطقي parse(ثابت std::string& داخلہ_2);
    منطقي hasMalformed() ثابت;
    ثابت Log_ٹیگ_Config& getGlobalConfig() ثابت;
    ثابت std::vector<Log_ٹیگ_Config>& get_بھریا_Name_Configs() ثابت;
    ثابت std::vector<Log_ٹیگ_Config>& get_پہلا_Part_Configs() ثابت;
    ثابت std::vector<Log_ٹیگ_Config>& getAnyPartConfigs() ثابت;
    ثابت std::vector<std::string>& getMalformed() ثابت;

خاص:
    فراغ segment_ٹوکن();
    فراغ parseNameAndLevel(ثابت std::string& s);
    فراغ parseWildcard(ثابت std::string& name, LogLevel level);
    ساكن std::pair<LogLevel, منطقي> parseLogLevel(ثابت std::string& s);
    ساكن std::string منزل_String(LogLevel level);

خاص:
    std::string m_داخلہ;
    Log_ٹیگ_Config m_parsedGlobal;
    std::vector<Log_ٹیگ_Config> m_parsed_بھریا_Name;
    std::vector<Log_ٹیگ_Config> m_parsed_پہلا_Part;
    std::vector<Log_ٹیگ_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#جے_انت
