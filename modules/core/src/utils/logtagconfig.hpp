// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#যদি 1 // if not already in precompiled headers
#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#অন্তর্ভাব <string>
#endif

নামাকাশ cv {
নামাকাশ utils {
নামাকাশ logging {

কাঠামো LogTagConfig
{
    std::string namePart;
    LogLevel level;
    যৌক্তিক isGlobal;
    যৌক্তিক hasPrefixWildcard;
    যৌক্তিক hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ধ্রুবক std::string& _namePart, LogLevel _level, যৌক্তিক _isGlobal = মিথ্যা,
        যৌক্তিক _hasPrefixWildcard = মিথ্যা, যৌক্তিক _hasSuffixWildcard = মিথ্যা)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ধ্রুবক LogTagConfig&) = পূর্বনির্ধারিত;
    LogTagConfig(LogTagConfig&&) = পূর্বনির্ধারিত;
    ~LogTagConfig() = পূর্বনির্ধারিত;
};

}}}

#endif
