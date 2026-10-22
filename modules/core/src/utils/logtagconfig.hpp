// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#যদি 1 // if not already in precompiled headers
#অন্তর্ভাৱ <opencv2/core/utils/logger.defines.hpp>
#অন্তর্ভাৱ <string>
#endif

নামাকাশ cv {
নামাকাশ utils {
নামাকাশ logging {

সংরচনা LogTagConfig
{
    std::string namePart;
    LogLevel level;
    তার্কিক isGlobal;
    তার্কিক hasPrefixWildcard;
    তার্কিক hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ধ্রুৱ std::string& _namePart, LogLevel _level, তার্কিক _isGlobal = মিছা,
        তার্কিক _hasPrefixWildcard = মিছা, তার্কিক _hasSuffixWildcard = মিছা)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ধ্রুৱ LogTagConfig&) = স্ৱভাৱ;
    LogTagConfig(LogTagConfig&&) = স্ৱভাৱ;
    ~LogTagConfig() = স্ৱভাৱ;
};

}}}

#endif
