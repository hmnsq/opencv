// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#എങ്കിൽ 1 // if not already in precompiled headers
#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#അന്തര്ഭാവ <string>
#endif

നാമാകാശ cv {
നാമാകാശ utils {
നാമാകാശ logging {

ഘടന LogTagConfig
{
    std::string namePart;
    LogLevel level;
    തർക്കം isGlobal;
    തർക്കം hasPrefixWildcard;
    തർക്കം hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(സ്ഥിരം std::string& _namePart, LogLevel _level, തർക്കം _isGlobal = അസത്യം,
        തർക്കം _hasPrefixWildcard = അസത്യം, തർക്കം _hasSuffixWildcard = അസത്യം)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(സ്ഥിരം LogTagConfig&) = സ്ഥിരസ്ഥിതി;
    LogTagConfig(LogTagConfig&&) = സ്ഥിരസ്ഥിതി;
    ~LogTagConfig() = സ്ഥിരസ്ഥിതി;
};

}}}

#endif
