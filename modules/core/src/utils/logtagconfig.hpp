// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#यदि_अपरिभाषितम् OPENCV_CORE_LOGTAGCONFIG_HPP
#परिभाषय OPENCV_CORE_LOGTAGCONFIG_HPP

#यदि 1 // if not already in precompiled headers
#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अन्तर्भाव <string>
#यदि_अन्तः

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {

संरचना Log_चिह्नकम्_Config
{
    std::string namePart;
    LogLevel level;
    तार्किक isGlobal;
    तार्किक has_उपसर्गः_Wildcard;
    तार्किक hasSuffixWildcard;

    Log_चिह्नकम्_Config()
        : namePart()
        , level()
        , isGlobal()
        , has_उपसर्गः_Wildcard()
        , hasSuffixWildcard()
    {
    }

    Log_चिह्नकम्_Config(ध्रुव std::string& _namePart, LogLevel _level, तार्किक _isGlobal = असत्य,
        तार्किक has_उपसर्गः_Wildcard_2 = असत्य, तार्किक _hasSuffixWildcard = असत्य)
        : namePart(_namePart)
        , level(_level)
        , isGlobal(_isGlobal)
        , has_उपसर्गः_Wildcard(has_उपसर्गः_Wildcard_2)
        , hasSuffixWildcard(_hasSuffixWildcard)
    {
    }

    Log_चिह्नकम्_Config(ध्रुव Log_चिह्नकम्_Config&) = स्वभाव;
    Log_चिह्नकम्_Config(Log_चिह्नकम्_Config&&) = स्वभाव;
    ~Log_चिह्नकम्_Config() = स्वभाव;
};

}}}

#यदि_अन्तः
