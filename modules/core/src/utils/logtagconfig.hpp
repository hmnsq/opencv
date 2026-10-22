// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جے_بنا_تعریف OPENCV_CORE_LOGTAGCONFIG_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGCONFIG_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ضمن <string>
#جے_انت

نطاق cv {
نطاق utils {
نطاق logging {

بنية Log_ٹیگ_Config
{
    std::string namePart;
    LogLevel level;
    منطقي isGlobal;
    منطقي has_اگیتر_Wildcard;
    منطقي hasSuffixWildcard;

    Log_ٹیگ_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_اگیتر_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_ٹیگ_Config(ثابت std::string& _namePart, LogLevel _level, منطقي _isGlobal = خطأ,
        منطقي has_اگیتر_Wildcard_2 = خطأ, منطقي _hasSuffixWildcard = خطأ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_اگیتر_Wildcard(has_اگیتر_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_ٹیگ_Config(ثابت Log_ٹیگ_Config&) = افتراضي;
    Log_ٹیگ_Config(Log_ٹیگ_Config&&) = افتراضي;
    ~Log_ٹیگ_Config() = افتراضي;
};

}}}

#جے_انت
