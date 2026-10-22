// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#ଯଦି 1 // if not already in precompiled headers
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#ଅନ୍ତର୍ଭାଵ <string>
#endif

ନାମାକାଶ cv {
ନାମାକାଶ utils {
ନାମାକାଶ logging {

ସଂରଚନା LogTagConfig
{
    std::string namePart;
    LogLevel level;
    ତାର୍କିକ isGlobal;
    ତାର୍କିକ hasPrefixWildcard;
    ତାର୍କିକ hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ଅଚଳ std::string& _namePart, LogLevel _level, ତାର୍କିକ _isGlobal = ମିଥ୍ୟା,
        ତାର୍କିକ _hasPrefixWildcard = ମିଥ୍ୟା, ତାର୍କିକ _hasSuffixWildcard = ମିଥ୍ୟା)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ଅଚଳ LogTagConfig&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    LogTagConfig(LogTagConfig&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    ~LogTagConfig() = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

}}}

#endif
