// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#எனில் 1 // if not already in precompiled headers
#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#அந்தர்பாவ <string>
#endif

நாமாகாஶ cv {
நாமாகாஶ utils {
நாமாகாஶ logging {

கட்டமைப்பு LogTagConfig
{
    std::string namePart;
    LogLevel level;
    தருக்க isGlobal;
    தருக்க hasPrefixWildcard;
    தருக்க hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(மாறிலி std::string& _namePart, LogLevel _level, தருக்க _isGlobal = பொய்,
        தருக்க _hasPrefixWildcard = பொய், தருக்க _hasSuffixWildcard = பொய்)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(மாறிலி LogTagConfig&) = இயல்பு;
    LogTagConfig(LogTagConfig&&) = இயல்பு;
    ~LogTagConfig() = இயல்பு;
};

}}}

#endif
