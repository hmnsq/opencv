// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अगर_अघोषित OPENCV_CORE_LOGTAGCONFIG_HPP
#घोषणा OPENCV_CORE_LOGTAGCONFIG_HPP

#अगर 1 // if not already in precompiled headers
#समावेश <opencv2/core/utils/logger.defines.hpp>
#समावेश <string>
#पूर्ण_अगर

namespace cv {
namespace utils {
namespace logging {

काष्ठा LogTagConfig
{
    std::string namePart;
    LogLevel level;
    bool isGlobal;
    bool hasPrefixWildcard;
    bool hasSuffixWildcard;

    LogTagConfig()
        : namePart()
        , level()
        , isGlobal()
        , hasPrefixWildcard()
        , hasSuffixWildcard()
    {
    }

    LogTagConfig(स्थिर std::string& _namePart, LogLevel _level, bool _isGlobal = false,
        bool _hasPrefixWildcard = false, bool _hasSuffixWildcard = false)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , hasPrefixWildcard(_hasPrefixWildcard)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    LogTagConfig(स्थिर LogTagConfig&) = शेष;
    LogTagConfig(LogTagConfig&&) = शेष;
    ~LogTagConfig() = शेष;
};

}}}

#पूर्ण_अगर
