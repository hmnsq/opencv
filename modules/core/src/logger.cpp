// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>
#ಅನ್ತರ್ಭಾವ "utils/logtagmanager.hpp"
#ಅನ್ತರ್ಭಾವ "utils/logtagconfigparser.hpp"

#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ <fstream>
#ಅನ್ತರ್ಭಾವ <atomic>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
# include <android/log.h>
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

ನಾಮಾಕಾಶ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ರಚನೆ GlobalLoggingInitStruct
{
ಸಾರ್ವಜನಿಕ:
#ಆದರೆ defined NDEBUG
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ m_isDebugBuild = ಸುಳ್ಳು;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ m_isDebugBuild = ನಿಜ;
#ಆದರೆ_ಅಂತ್ಯ

ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ LogLevel m_defaultUnconfiguredGlobalLevel;

ಸಾರ್ವಜನಿಕ:
    Log_ಟ್ಯಾಗ್_Manager log_ಟ್ಯಾಗ್_Manager;

    GlobalLoggingInitStruct()
        : log_ಟ್ಯಾಗ್_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (ಶೂನ್ಯ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ಖಾಸಗಿ:
    ಶೂನ್ಯ applyConfigString()
    {
        log_ಟ್ಯಾಗ್_Manager.ಗಣ_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    ಶೂನ್ಯ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ಆದರೆ (m_isDebugBuild)
        {
            ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& parser = log_ಟ್ಯಾಗ್_Manager.getConfigParser();
            ಆದರೆ (parser.hasMalformed())
            {
                ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& malformedList = parser.getMalformed();
                ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& malformed : malformedList)
                {
                    std::ಹೊರಗೆ << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::ಹೊರಗೆ.flush();
            }
        }
    }
};

LogLevel GlobalLoggingInitStruct::m_defaultUnconfiguredGlobalLevel = GlobalLoggingInitStruct::m_isDebugBuild
                ? LOG_LEVEL_INFO
                : LOG_LEVEL_WARNING;


// Static dynamic initialization guard function for the combined struct
// just defined above
//
// An initialization guard function guarantees that outside code cannot
// accidentally see not-yet-dynamically-initialized data, by routing
// all outside access request to this function, so that this function
// has a chance to run the initialization code if necessary.
//
// An initialization guard function only guarantees initialization upon
// the first call to this function.
//
ಸ್ಥಿರ GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, ಹೊಸ GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ರಚನೆ GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (ಶೂನ್ಯ)get_Global_Log_ಟ್ಯಾಗ್();  // complete initialization of logger structures
    }
};

ಸ್ಥಿರ GlobalLoggingInitCall globalLoggingInitCall;

ಸ್ಥಿರ Log_ಟ್ಯಾಗ್_Manager& get_Log_ಟ್ಯಾಗ್_Manager()
{
    ಸ್ಥಿರ Log_ಟ್ಯಾಗ್_Manager& log_ಟ್ಯಾಗ್_Manager_Instance = getGlobalLoggingInitStruct().log_ಟ್ಯಾಗ್_Manager;
    ಹಿಂತಿರುಗಿ log_ಟ್ಯಾಗ್_Manager_Instance;
}

ಸ್ಥಿರ LogLevel& getLogLevelVariable()
{
    ಸ್ಥಿರ LogLevel& refGlobalLogLevel = get_Global_Log_ಟ್ಯಾಗ್()->level;
    ಹಿಂತಿರುಗಿ refGlobalLogLevel;
}

Log_ಟ್ಯಾಗ್* get_Global_Log_ಟ್ಯಾಗ್()
{
    ಸ್ಥಿರ Log_ಟ್ಯಾಗ್* global_Log_ಟ್ಯಾಗ್_Ptr = getGlobalLoggingInitStruct().log_ಟ್ಯಾಗ್_Manager.get("global");
    ಹಿಂತಿರುಗಿ global_Log_ಟ್ಯಾಗ್_Ptr;
}

} // namespace

ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_Log_ಟ್ಯಾಗ್(Log_ಟ್ಯಾಗ್* plogtag)
{
    ಆದರೆ (!plogtag || !plogtag->name)
    {
        ಹಿಂತಿರುಗಿ;
    }
    internal::get_Log_ಟ್ಯಾಗ್_Manager().assign(plogtag->name, plogtag);
}

ಶೂನ್ಯ ಗಣ_Log_ಟ್ಯಾಗ್_Level(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಟ್ಯಾಗ್, LogLevel level)
{
    ಆದರೆ (!ಟ್ಯಾಗ್)
    {
        ಹಿಂತಿರುಗಿ;
    }
    internal::get_Log_ಟ್ಯಾಗ್_Manager().ಗಣ_Level_By_ತುಂಬಿದ_Name(std::string(ಟ್ಯಾಗ್), level);
}

LogLevel get_Log_ಟ್ಯಾಗ್_Level(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಟ್ಯಾಗ್)
{
    ಆದರೆ (!ಟ್ಯಾಗ್)
    {
        ಹಿಂತಿರುಗಿ getLogLevel();
    }
    ಸ್ಥಿರಾಂಕ Log_ಟ್ಯಾಗ್* ptr = internal::get_Log_ಟ್ಯಾಗ್_Manager().get(std::string(ಟ್ಯಾಗ್));
    ಆದರೆ (!ptr)
    {
        ಹಿಂತಿರುಗಿ getLogLevel();
    }
    ಹಿಂತಿರುಗಿ ptr->level;
}

LogLevel ಗಣ_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ಸ್ಥಿರಾಂಕ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ಹಿಂತಿರುಗಿ old;
}

LogLevel getLogLevel()
{
    ಹಿಂತಿರುಗಿ internal::getLogLevelVariable();
}

ನಾಮಾಕಾಶ internal {

ನಾಮಾಕಾಶ //unnamed
{
    std::atomic<Write_Log_ಸಂದೇಶ_Func_Type> stc_user_Write_Log_ಸಂದೇಶ_Func{};
    std::atomic<Write_Log_ಸಂದೇಶ_Ex_Func_Type> stc_user_Write_Log_ಸಂದೇಶ_Ex_Func{};
} //unnamed

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_Show_Timestamp_ಬಹುಲಕ()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", ನಿಜ);
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", ಸುಳ್ಳು);
    ಹಿಂತಿರುಗಿ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

ಶೂನ್ಯ write_Log_ಸಂದೇಶ(LogLevel logLevel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* message)
{
    Write_Log_ಸಂದೇಶ_Func_Type userFunc = stc_user_Write_Log_ಸಂದೇಶ_Func.load();
    ಆದರೆ (userFunc && userFunc != write_Log_ಸಂದೇಶ)
    {
        (*userFunc)(logLevel, message);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ threadID = cv::utils::getThreadID();

    std::string ಸಂದೇಶ_id;
    ಆಯ್ಕೆ (get_Show_Timestamp_ಬಹುಲಕ())
    {
        ಸ್ಥಿತಿ 1: ಸಂದೇಶ_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 1+2: ಸಂದೇಶ_id = cv::format("%d@%llu", threadID, (ದೀರ್ಘ ದೀರ್ಘ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)getTimestampNS()); ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ: ಸಂದೇಶ_id = cv::format("%d", threadID); ನಿಲ್ಲಿಸು;
    }

    std::ostringstream ss;
    ಆಯ್ಕೆ (logLevel)
    {
    ಸ್ಥಿತಿ LOG_LEVEL_FATAL:   ss << "[FATAL:" << ಸಂದೇಶ_id << "] " << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_ERROR:   ss << "[ERROR:" << ಸಂದೇಶ_id << "] " << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_WARNING: ss << "[ WARN:" << ಸಂದೇಶ_id << "] " << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_INFO:    ss << "[ INFO:" << ಸಂದೇಶ_id << "] " << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << ಸಂದೇಶ_id << "] " << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_VERBOSE: ss << message << std::ಸಾಲು_ಅಂತ್ಯ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_SILENT: ಹಿಂತಿರುಗಿ;  // avoid compiler warning about incomplete switch
    ಸ್ಥಿತಿ ENUM_LOG_LEVEL_FORCE_INT: ಹಿಂತಿರುಗಿ;  // avoid compiler warning about incomplete switch
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    ಪೂರ್ಣಾಂಕ android_logLevel = ANDROID_LOG_INFO;
    ಆಯ್ಕೆ (logLevel)
    {
    ಸ್ಥಿತಿ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_ಆವೃತ್ತಿ, "%s", ss.str().c_str());
#ಆದರೆ_ಅಂತ್ಯ
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::ಹೊರಗೆ;
    (*out) << ss.str();
    ಆದರೆ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* strip_ಮೂಲ_File_ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* file)
{
    CV_Assert(file);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* pos = file;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* strip_pos = ಖಾಲಿ;
    ಅಕ್ಷರ ch = 0;
    ಆಗ ((ch = pos[0]) != 0)
    {
        ++pos;
        ಆದರೆ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ಆದರೆ (strip_pos == ಖಾಲಿ || strip_pos == pos/*eos*/)
        ಹಿಂತಿರುಗಿ file;
    ಹಿಂತಿರುಗಿ strip_pos;
}

ಶೂನ್ಯ write_Log_ಸಂದೇಶ_Ex(LogLevel logLevel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಟ್ಯಾಗ್, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* file, ಪೂರ್ಣಾಂಕ ಸಾಲು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* func, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* message)
{
    Write_Log_ಸಂದೇಶ_Ex_Func_Type userFunc = stc_user_Write_Log_ಸಂದೇಶ_Ex_Func.load();
    ಆದರೆ (userFunc && userFunc != write_Log_ಸಂದೇಶ_Ex)
    {
        (*userFunc)(logLevel, ಟ್ಯಾಗ್, file, ಸಾಲು, func, message);
        ಹಿಂತಿರುಗಿ;
    }

    std::ostringstream strm;
    ಆದರೆ (ಟ್ಯಾಗ್)
    {
        strm << ಟ್ಯಾಗ್ << ' ';
    }
    ಆದರೆ (file)
    {
        strm << strip_ಮೂಲ_File_ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ(file);
        ಆದರೆ (ಸಾಲು > 0)
        {
            strm << ':' << ಸಾಲು;
        }
        strm << ' ';
    }
    ಆದರೆ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_ಸಂದೇಶ(logLevel, strm.str().c_str());
}

ಶೂನ್ಯ replace_Write_Log_ಸಂದೇಶ(Write_Log_ಸಂದೇಶ_Func_Type f)
{
    ಆದರೆ (f == write_Log_ಸಂದೇಶ)
    {
        f = ಶೂನ್ಯಸೂಚಕ;
    }
    stc_user_Write_Log_ಸಂದೇಶ_Func.store(f);
}

ಶೂನ್ಯ replace_Write_Log_ಸಂದೇಶ_Ex(Write_Log_ಸಂದೇಶ_Ex_Func_Type f)
{
    ಆದರೆ (f == write_Log_ಸಂದೇಶ_Ex)
    {
        f = ಶೂನ್ಯಸೂಚಕ;
    }
    stc_user_Write_Log_ಸಂದೇಶ_Ex_Func.store(f);
}

} // namespace

}}} // namespace
