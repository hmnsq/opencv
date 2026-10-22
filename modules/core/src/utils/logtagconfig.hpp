// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#𐤀𐤌 1 // if not already in precompiled headers
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ضمن <string>
#endif

نطاق cv {
نطاق utils {
نطاق logging {

بنية LogTagConfig
{
    std::string namePart;
    LogLevel level;
    منطقي isGlobal;
    منطقي hasPrefixWildcard;
    منطقي hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ثابت std::string& _namePart, LogLevel _level, منطقي _isGlobal = خطأ,
        منطقي _hasPrefixWildcard = خطأ, منطقي _hasSuffixWildcard = خطأ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ثابت LogTagConfig&) = افتراضي;
    LogTagConfig(LogTagConfig&&) = افتراضي;
    ~LogTagConfig() = افتراضي;
};

}}}

#endif
