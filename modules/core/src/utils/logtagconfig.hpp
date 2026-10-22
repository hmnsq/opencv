// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_LOGTAGCONFIG_HPP
#وصف_ڪريو OPENCV_CORE_LOGTAGCONFIG_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ضمن <string>
#جيڪڏهن_پورو

نطاق cv {
نطاق utils {
نطاق logging {

بنية Log_ٽيگ_Config
{
    std::string namePart;
    LogLevel level;
    منطقي isGlobal;
    منطقي has_اڳياڙي_Wildcard;
    منطقي hasSuffixWildcard;

    Log_ٽيگ_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_اڳياڙي_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_ٽيگ_Config(ثابت std::string& _namePart, LogLevel _level, منطقي _isGlobal = خطأ,
        منطقي has_اڳياڙي_Wildcard_2 = خطأ, منطقي _hasSuffixWildcard = خطأ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_اڳياڙي_Wildcard(has_اڳياڙي_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_ٽيگ_Config(ثابت Log_ٽيگ_Config&) = افتراضي;
    Log_ٽيگ_Config(Log_ٽيگ_Config&&) = افتراضي;
    ~Log_ٽيگ_Config() = افتراضي;
};

}}}

#جيڪڏهن_پورو
