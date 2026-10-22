// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_LOGTAGCONFIG_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGCONFIG_HPP

#اگر 1 // if not already in precompiled headers
#شامل <opencv2/core/utils/logger.defines.hpp>
#شامل <string>
#اگر_ختم

نامخلا cv {
نامخلا utils {
نامخلا logging {

ساخت Log_لیبل_Config
{
    std::string namePart;
    LogLevel level;
    منطقی isGlobal;
    منطقی has_سابقہ_Wildcard;
    منطقی hasSuffixWildcard;

    Log_لیبل_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_سابقہ_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_لیبل_Config(ثابت std::string& _namePart, LogLevel _level, منطقی _isGlobal = جھوٹ,
        منطقی has_سابقہ_Wildcard_2 = جھوٹ, منطقی _hasSuffixWildcard = جھوٹ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_سابقہ_Wildcard(has_سابقہ_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_لیبل_Config(ثابت Log_لیبل_Config&) = طےشدہ;
    Log_لیبل_Config(Log_لیبل_Config&&) = طےشدہ;
    ~Log_لیبل_Config() = طےشدہ;
};

}}}

#اگر_ختم
