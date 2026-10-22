// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#જો 1 // if not already in precompiled headers
#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#અન્તર્ભાવ <string>
#endif

નામાકાશ cv {
નામાકાશ utils {
નામાકાશ logging {

સંરચના LogTagConfig
{
    std::string namePart;
    LogLevel level;
    તાર્કિક isGlobal;
    તાર્કિક hasPrefixWildcard;
    તાર્કિક hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(અચલ std::string& _namePart, LogLevel _level, તાર્કિક _isGlobal = ખોટું,
        તાર્કિક _hasPrefixWildcard = ખોટું, તાર્કિક _hasSuffixWildcard = ખોટું)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(અચલ LogTagConfig&) = મૂળભૂત;
    LogTagConfig(LogTagConfig&&) = મૂળભૂત;
    ~LogTagConfig() = મૂળભૂત;
};

}}}

#endif
