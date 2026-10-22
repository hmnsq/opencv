// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_LOGTAGCONFIG_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_LOGTAGCONFIG_HPP

#ಆದರೆ 1 // if not already in precompiled headers
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ಅನ್ತರ್ಭಾವ <string>
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

ರಚನೆ Log_ಟ್ಯಾಗ್_Config
{
    std::string namePart;
    LogLevel level;
    ತಾರ್ಕಿಕ isGlobal;
    ತಾರ್ಕಿಕ has_ಪೂರ್ವಪ್ರತ್ಯಯ_Wildcard;
    ತಾರ್ಕಿಕ hasSuffixWildcard;

    Log_ಟ್ಯಾಗ್_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_ಪೂರ್ವಪ್ರತ್ಯಯ_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_ಟ್ಯಾಗ್_Config(ಸ್ಥಿರಾಂಕ std::string& _namePart, LogLevel _level, ತಾರ್ಕಿಕ _isGlobal = ಸುಳ್ಳು,
        ತಾರ್ಕಿಕ has_ಪೂರ್ವಪ್ರತ್ಯಯ_Wildcard_2 = ಸುಳ್ಳು, ತಾರ್ಕಿಕ _hasSuffixWildcard = ಸುಳ್ಳು)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_ಪೂರ್ವಪ್ರತ್ಯಯ_Wildcard(has_ಪೂರ್ವಪ್ರತ್ಯಯ_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_ಟ್ಯಾಗ್_Config(ಸ್ಥಿರಾಂಕ Log_ಟ್ಯಾಗ್_Config&) = ಪೂರ್ವನಿಯೋಜಿತ;
    Log_ಟ್ಯಾಗ್_Config(Log_ಟ್ಯಾಗ್_Config&&) = ಪೂರ್ವನಿಯೋಜಿತ;
    ~Log_ಟ್ಯಾಗ್_Config() = ಪೂರ್ವನಿಯೋಜಿತ;
};

}}}

#ಆದರೆ_ಅಂತ್ಯ
