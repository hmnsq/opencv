// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_LOGTAGCONFIGPARSER_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_LOGTAGCONFIGPARSER_HPP

#ਜੇ 1 // if not already in precompiled headers
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <vector>
#ਅਨ੍ਤਰ੍ਭਾਵ <functional>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logtag.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ "logtagconfig.hpp"

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ logging {

ਵਰਗ Log_ਟੈਗ_Config_Parser
{
ਜਨਤਕ:
    Log_ਟੈਗ_Config_Parser(LogLevel defaultUnconfiguredGlobalLevel = LOG_LEVEL_VERBOSE);
    ਸਪਸ਼ਟ Log_ਟੈਗ_Config_Parser(ਅਚਲ std::string& ਨਿਵੇਸ਼_2);
    ~Log_ਟੈਗ_Config_Parser();

ਜਨਤਕ:
    ਤਾਰਕਿਕ parse(ਅਚਲ std::string& ਨਿਵੇਸ਼_2);
    ਤਾਰਕਿਕ hasMalformed() ਅਚਲ;
    ਅਚਲ Log_ਟੈਗ_Config& getGlobalConfig() ਅਚਲ;
    ਅਚਲ std::vector<Log_ਟੈਗ_Config>& get_ਭਰਿਆ_Name_Configs() ਅਚਲ;
    ਅਚਲ std::vector<Log_ਟੈਗ_Config>& get_ਪਹਿਲਾ_Part_Configs() ਅਚਲ;
    ਅਚਲ std::vector<Log_ਟੈਗ_Config>& getAnyPartConfigs() ਅਚਲ;
    ਅਚਲ std::vector<std::string>& getMalformed() ਅਚਲ;

ਨਿਜੀ:
    ਖਾਲੀ segment_ਟੋਕਨ();
    ਖਾਲੀ parseNameAndLevel(ਅਚਲ std::string& s);
    ਖਾਲੀ parseWildcard(ਅਚਲ std::string& name, LogLevel level);
    ਸਥਿਰ std::pair<LogLevel, ਤਾਰਕਿਕ> parseLogLevel(ਅਚਲ std::string& s);
    ਸਥਿਰ std::string ਮੰਜ਼ਿਲ_String(LogLevel level);

ਨਿਜੀ:
    std::string m_ਨਿਵੇਸ਼;
    Log_ਟੈਗ_Config m_parsedGlobal;
    std::vector<Log_ਟੈਗ_Config> m_parsed_ਭਰਿਆ_Name;
    std::vector<Log_ਟੈਗ_Config> m_parsed_ਪਹਿਲਾ_Part;
    std::vector<Log_ਟੈਗ_Config> m_parsedAnyPart;
    std::vector<std::string> m_malformed;
};

}}} //namespace

#ਜੇ_ਅੰਤ
