// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#నిర్వచితం_కాకపోతే OPENCV_CORE_LOGTAGCONFIG_HPP
#నిర్వచించు OPENCV_CORE_LOGTAGCONFIG_HPP

#ఒకవేళ 1 // if not already in precompiled headers
#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#అన్తర్భావ <string>
#అయితే_ముగింపు

నామాకాశ cv {
నామాకాశ utils {
నామాకాశ logging {

నిర్మాణం Log_ట్యాగ్_Config
{
    std::string namePart;
    LogLevel level;
    తార్కిక isGlobal;
    తార్కిక has_ఉపసర్గ_Wildcard;
    తార్కిక hasSuffixWildcard;

    Log_ట్యాగ్_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_ఉపసర్గ_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_ట్యాగ్_Config(స్థిరరాశి std::string& _namePart, LogLevel _level, తార్కిక _isGlobal = అబద్ధం,
        తార్కిక has_ఉపసర్గ_Wildcard_2 = అబద్ధం, తార్కిక _hasSuffixWildcard = అబద్ధం)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_ఉపసర్గ_Wildcard(has_ఉపసర్గ_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_ట్యాగ్_Config(స్థిరరాశి Log_ట్యాగ్_Config&) = అప్రమేయ;
    Log_ట్యాగ్_Config(Log_ట్యాగ్_Config&&) = అప్రమేయ;
    ~Log_ట్యాగ్_Config() = అప్రమేయ;
};

}}}

#అయితే_ముగింపు
