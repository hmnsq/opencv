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

#ifdef __ANDROID__
# include <android/log.h>
#endif

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

ನಾಮಾಕಾಶ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ಸಂರಚನಾ GlobalLoggingInitStruct
{
ಸಾರ್ವಜನಿಕ:
#ಯದಿ defined NDEBUG
    ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ m_isDebugBuild = ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ m_isDebugBuild = ಸತ್ಯ;
#endif

ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ LogLevel m_defaultUnconfiguredGlobalLevel;

ಸಾರ್ವಜನಿಕ:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (ಶೂನ್ಯ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ಗುಪ್ತ:
    ಶೂನ್ಯ applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    ಶೂನ್ಯ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ಯದಿ (m_isDebugBuild)
        {
            ಧ್ರುವ auto& parser = logTagManager.getConfigParser();
            ಯದಿ (parser.hasMalformed())
            {
                ಧ್ರುವ auto& malformedList = parser.getMalformed();
                ಪ್ರತಿ (ಧ್ರುವ auto& malformed : malformedList)
                {
                    std::cout << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::cout.flush();
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
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, ನೂತನ GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ಸಂರಚನಾ GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (ಶೂನ್ಯ)getGlobalLogTag();  // complete initialization of logger structures
    }
};

ಸ್ಥಿರ GlobalLoggingInitCall globalLoggingInitCall;

ಸ್ಥಿರ LogTagManager& getLogTagManager()
{
    ಸ್ಥಿರ LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    ಪ್ರತ್ಯಾಗಮ logTagManagerInstance;
}

ಸ್ಥಿರ LogLevel& getLogLevelVariable()
{
    ಸ್ಥಿರ LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    ಪ್ರತ್ಯಾಗಮ refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    ಸ್ಥಿರ LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    ಪ್ರತ್ಯಾಗಮ globalLogTagPtr;
}

} // namespace

ಶೂನ್ಯ registerLogTag(LogTag* plogtag)
{
    ಯದಿ (!plogtag || !plogtag->name)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

ಶೂನ್ಯ setLogTagLevel(ಧ್ರುವ ವರ್ಣ* tag, LogLevel level)
{
    ಯದಿ (!tag)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ಧ್ರುವ ವರ್ಣ* tag)
{
    ಯದಿ (!tag)
    {
        ಪ್ರತ್ಯಾಗಮ getLogLevel();
    }
    ಧ್ರುವ LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    ಯದಿ (!ptr)
    {
        ಪ್ರತ್ಯಾಗಮ getLogLevel();
    }
    ಪ್ರತ್ಯಾಗಮ ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ಧ್ರುವ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ಪ್ರತ್ಯಾಗಮ old;
}

LogLevel getLogLevel()
{
    ಪ್ರತ್ಯಾಗಮ internal::getLogLevelVariable();
}

ನಾಮಾಕಾಶ internal {

ನಾಮಾಕಾಶ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ getShowTimestampMode()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", ಸತ್ಯ);
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", ಅಸತ್ಯ);
    ಪ್ರತ್ಯಾಗಮ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

ಶೂನ್ಯ writeLogMessage(LogLevel logLevel, ಧ್ರುವ ವರ್ಣ* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    ಯದಿ (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ threadID = cv::utils::getThreadID();

    std::string message_id;
    ವಿಕಲ್ಪ (getShowTimestampMode())
    {
        ಪಕ್ಷ 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ಭಙ್ಗ;
        ಪಕ್ಷ 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned ಪೂರ್ಣಾಙ್ಕ)getTimestampNS()); ಭಙ್ಗ;
        ಸ್ವಭಾವ: message_id = cv::format("%d", threadID); ಭಙ್ಗ;
    }

    std::ostringstream ss;
    ವಿಕಲ್ಪ (logLevel)
    {
    ಪಕ್ಷ LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_VERBOSE: ss << message << std::endl; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_SILENT: ಪ್ರತ್ಯಾಗಮ;  // avoid compiler warning about incomplete switch
    ಪಕ್ಷ ENUM_LOG_LEVEL_FORCE_INT: ಪ್ರತ್ಯಾಗಮ;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    ಪೂರ್ಣಾಙ್ಕ android_logLevel = ANDROID_LOG_INFO;
    ವಿಕಲ್ಪ (logLevel)
    {
    ಪಕ್ಷ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ಭಙ್ಗ;
    ಪಕ್ಷ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    ಯದಿ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* stripSourceFilePathPrefix(ಧ್ರುವ ವರ್ಣ* file)
{
    CV_Assert(file);
    ಧ್ರುವ ವರ್ಣ* pos = file;
    ಧ್ರುವ ವರ್ಣ* strip_pos = ರಿಕ್ತ;
    ವರ್ಣ ch = 0;
    ಯಾವತ್ ((ch = pos[0]) != 0)
    {
        ++pos;
        ಯದಿ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ಯದಿ (strip_pos == ರಿಕ್ತ || strip_pos == pos/*eos*/)
        ಪ್ರತ್ಯಾಗಮ file;
    ಪ್ರತ್ಯಾಗಮ strip_pos;
}

ಶೂನ್ಯ writeLogMessageEx(LogLevel logLevel, ಧ್ರುವ ವರ್ಣ* tag, ಧ್ರುವ ವರ್ಣ* file, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು, ಧ್ರುವ ವರ್ಣ* func, ಧ್ರುವ ವರ್ಣ* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    ಯದಿ (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, ಸಾಲು, func, message);
        ಪ್ರತ್ಯಾಗಮ;
    }

    std::ostringstream strm;
    ಯದಿ (tag)
    {
        strm << tag << ' ';
    }
    ಯದಿ (file)
    {
        strm << stripSourceFilePathPrefix(file);
        ಯದಿ (ಸಾಲು > 0)
        {
            strm << ':' << ಸಾಲು;
        }
        strm << ' ';
    }
    ಯದಿ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

ಶೂನ್ಯ replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    ಯದಿ (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

ಶೂನ್ಯ replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    ಯದಿ (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
