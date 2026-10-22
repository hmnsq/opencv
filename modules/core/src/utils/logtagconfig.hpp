// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGCONFIG_HPP
#define OPENCV_CORE_LOGTAGCONFIG_HPP

#यदि 1 // if not already in precompiled headers
#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अन्तर्भाव <string>
#endif

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {

संरचना LogTagConfig
{
    std::string namePart;
    LogLevel level;
    तार्किक isGlobal;
    तार्किक hasPrefixWildcard;
    तार्किक hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(ध्रुव std::string& _namePart, LogLevel _level, तार्किक _isGlobal = असत्य,
        तार्किक _hasPrefixWildcard = असत्य, तार्किक _hasSuffixWildcard = असत्य)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(ध्रुव LogTagConfig&) = स्वभाव;
    LogTagConfig(LogTagConfig&&) = स्वभाव;
    ~LogTagConfig() = स्वभाव;
};

}}}

#endif
