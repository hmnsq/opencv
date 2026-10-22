// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#که_نه_تعریف_شوی OPENCV_CORE_LOGTAGCONFIG_HPP
#تعریف_کړه OPENCV_CORE_LOGTAGCONFIG_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ضمن <string>
#د_که_پای

نطاق cv {
نطاق utils {
نطاق logging {

بنية Log_نښان_Config
{
    std::string namePart;
    LogLevel level;
    منطقي isGlobal;
    منطقي has_مختاړی_Wildcard;
    منطقي hasSuffixWildcard;

    Log_نښان_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_مختاړی_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_نښان_Config(ثابت std::string& _namePart, LogLevel _level, منطقي _isGlobal = خطأ,
        منطقي has_مختاړی_Wildcard_2 = خطأ, منطقي _hasSuffixWildcard = خطأ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_مختاړی_Wildcard(has_مختاړی_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_نښان_Config(ثابت Log_نښان_Config&) = افتراضي;
    Log_نښان_Config(Log_نښان_Config&&) = افتراضي;
    ~Log_نښان_Config() = افتراضي;
};

}}}

#د_که_پای
