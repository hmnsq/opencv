// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_تعریف_نشده OPENCV_CORE_LOGTAGCONFIG_HPP
#تعریف_کن OPENCV_CORE_LOGTAGCONFIG_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ضمن <string>
#پایان_اگر

نطاق cv {
نطاق utils {
نطاق logging {

بنية Log_برچسب_Config
{
    std::string namePart;
    LogLevel level;
    منطقي isGlobal;
    منطقي has_پیشوند_Wildcard;
    منطقي hasSuffixWildcard;

    Log_برچسب_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_پیشوند_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_برچسب_Config(ثابت std::string& _namePart, LogLevel _level, منطقي _isGlobal = خطأ,
        منطقي has_پیشوند_Wildcard_2 = خطأ, منطقي _hasSuffixWildcard = خطأ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_پیشوند_Wildcard(has_پیشوند_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_برچسب_Config(ثابت Log_برچسب_Config&) = افتراضي;
    Log_برچسب_Config(Log_برچسب_Config&&) = افتراضي;
    ~Log_برچسب_Config() = افتراضي;
};

}}}

#پایان_اگر
