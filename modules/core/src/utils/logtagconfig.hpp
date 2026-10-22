// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#ಯದಿ 1 // if not already in precompiled headers
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ಅನ್ತರ್ಭಾವ <string>
#endif

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

ಸಂರಚನಾ LogTagConfig
{
    std::string namePart;
    LogLevel level;
    ತಾರ್ಕಿಕ isGlobal;
    ತಾರ್ಕಿಕ hasPrefixWildcard;
    ತಾರ್ಕಿಕ hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ಧ್ರುವ std::string& _namePart, LogLevel _level, ತಾರ್ಕಿಕ _isGlobal = ಅಸತ್ಯ,
        ತಾರ್ಕಿಕ _hasPrefixWildcard = ಅಸತ್ಯ, ತಾರ್ಕಿಕ _hasSuffixWildcard = ಅಸತ್ಯ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ಧ್ರುವ LogTagConfig&) = ಸ್ವಭಾವ;
    LogTagConfig(LogTagConfig&&) = ಸ್ವಭಾವ;
    ~LogTagConfig() = ಸ್ವಭಾವ;
};

}}}

#endif
