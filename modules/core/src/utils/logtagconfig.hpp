// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#اگر 1 // if not already in precompiled headers
#شامل <opencv2/core/utils/logger.defines.hpp>
#شامل <string>
#endif

فضاینام cv {
فضاینام utils {
فضاینام logging {

ساختار LogTagConfig
{
    std::string namePart;
    LogLevel level;
    منطقی isGlobal;
    منطقی hasPrefixWildcard;
    منطقی hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ثابت std::string& _namePart, LogLevel _level, منطقی _isGlobal = نادرست,
        منطقی _hasPrefixWildcard = نادرست, منطقی _hasSuffixWildcard = نادرست)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ثابت LogTagConfig&) = پیشفرض;
    LogTagConfig(LogTagConfig&&) = پیشفرض;
    ~LogTagConfig() = پیشفرض;
};

}}}

#endif
