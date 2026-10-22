// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_LOGTAGCONFIG_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_LOGTAGCONFIG_HPP

#ਜੇ 1 // if not already in precompiled headers
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ logging {

ਬਣਤਰ Log_ਟੈਗ_Config
{
    std::string namePart;
    LogLevel level;
    ਤਾਰਕਿਕ isGlobal;
    ਤਾਰਕਿਕ has_ਅਗੇਤਰ_Wildcard;
    ਤਾਰਕਿਕ hasSuffixWildcard;

    Log_ਟੈਗ_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_ਅਗੇਤਰ_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_ਟੈਗ_Config(ਅਚਲ std::string& _namePart, LogLevel _level, ਤਾਰਕਿਕ _isGlobal = ਝੂਠ,
        ਤਾਰਕਿਕ has_ਅਗੇਤਰ_Wildcard_2 = ਝੂਠ, ਤਾਰਕਿਕ _hasSuffixWildcard = ਝੂਠ)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_ਅਗੇਤਰ_Wildcard(has_ਅਗੇਤਰ_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_ਟੈਗ_Config(ਅਚਲ Log_ਟੈਗ_Config&) = ਮੂਲ;
    Log_ਟੈਗ_Config(Log_ਟੈਗ_Config&&) = ਮੂਲ;
    ~Log_ਟੈਗ_Config() = ਮੂਲ;
};

}}}

#ਜੇ_ਅੰਤ
