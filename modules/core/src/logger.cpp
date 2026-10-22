// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"

#অন্তর্ভাৱ <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>
#অন্তর্ভাৱ "utils/logtagmanager.hpp"
#অন্তর্ভাৱ "utils/logtagconfigparser.hpp"

#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <iostream>
#অন্তর্ভাৱ <fstream>
#অন্তর্ভাৱ <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

নামাকাশ cv {
নামাকাশ utils {
নামাকাশ logging {

নামাকাশ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
সংরচনা GlobalLoggingInitStruct
{
সার্ৱজনিক:
#যদি defined NDEBUG
    স্থির ধ্রুৱ তার্কিক m_isDebugBuild = মিছা;
#অন্যথা
    স্থির ধ্রুৱ তার্কিক m_isDebugBuild = সঁচা;
#endif

সার্ৱজনিক:
    স্থির LogLevel m_defaultUnconfiguredGlobalLevel;

সার্ৱজনিক:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (শূন্য)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

গুপ্ত:
    শূন্য applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    শূন্য handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        যদি (m_isDebugBuild)
        {
            ধ্রুৱ auto& parser = logTagManager.getConfigParser();
            যদি (parser.hasMalformed())
            {
                ধ্রুৱ auto& malformedList = parser.getMalformed();
                বাবে (ধ্রুৱ auto& malformed : malformedList)
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
স্থির GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, নূতন GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
সংরচনা GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (শূন্য)getGlobalLogTag();  // complete initialization of logger structures
    }
};

স্থির GlobalLoggingInitCall globalLoggingInitCall;

স্থির LogTagManager& getLogTagManager()
{
    স্থির LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    ঘূৰাই logTagManagerInstance;
}

স্থির LogLevel& getLogLevelVariable()
{
    স্থির LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    ঘূৰাই refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    স্থির LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    ঘূৰাই globalLogTagPtr;
}

} // namespace

শূন্য registerLogTag(LogTag* plogtag)
{
    যদি (!plogtag || !plogtag->name)
    {
        ঘূৰাই;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

শূন্য setLogTagLevel(ধ্রুৱ ৱর্ণ* tag, LogLevel level)
{
    যদি (!tag)
    {
        ঘূৰাই;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ধ্রুৱ ৱর্ণ* tag)
{
    যদি (!tag)
    {
        ঘূৰাই getLogLevel();
    }
    ধ্রুৱ LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    যদি (!ptr)
    {
        ঘূৰাই getLogLevel();
    }
    ঘূৰাই ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ধ্রুৱ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ঘূৰাই old;
}

LogLevel getLogLevel()
{
    ঘূৰাই internal::getLogLevelVariable();
}

নামাকাশ internal {

নামাকাশ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

স্থির পূর্ণাঙ্ক getShowTimestampMode()
{
    স্থির তার্কিক param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", সঁচা);
    স্থির তার্কিক param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", মিছা);
    ঘূৰাই (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

শূন্য writeLogMessage(LogLevel logLevel, ধ্রুৱ ৱর্ণ* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    যদি (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        ঘূৰাই;
    }

    ধ্রুৱ পূর্ণাঙ্ক threadID = cv::utils::getThreadID();

    std::string message_id;
    ৱিকল্প (getShowTimestampMode())
    {
        পক্ষ 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ভঙ্গ;
        পক্ষ 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned পূর্ণাঙ্ক)getTimestampNS()); ভঙ্গ;
        স্ৱভাৱ: message_id = cv::format("%d", threadID); ভঙ্গ;
    }

    std::ostringstream ss;
    ৱিকল্প (logLevel)
    {
    পক্ষ LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_VERBOSE: ss << message << std::endl; ভঙ্গ;
    পক্ষ LOG_LEVEL_SILENT: ঘূৰাই;  // avoid compiler warning about incomplete switch
    পক্ষ ENUM_LOG_LEVEL_FORCE_INT: ঘূৰাই;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    পূর্ণাঙ্ক android_logLevel = ANDROID_LOG_INFO;
    ৱিকল্প (logLevel)
    {
    পক্ষ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ভঙ্গ;
    পক্ষ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ভঙ্গ;
    পক্ষ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ভঙ্গ;
    পক্ষ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ভঙ্গ;
    পক্ষ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ভঙ্গ;
    পক্ষ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ভঙ্গ;
    স্ৱভাৱ:
        ভঙ্গ;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    যদি (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

স্থির ধ্রুৱ ৱর্ণ* stripSourceFilePathPrefix(ধ্রুৱ ৱর্ণ* file)
{
    CV_Assert(file);
    ধ্রুৱ ৱর্ণ* pos = file;
    ধ্রুৱ ৱর্ণ* strip_pos = রিক্ত;
    ৱর্ণ ch = 0;
    যেতিয়ালৈ ((ch = pos[0]) != 0)
    {
        ++pos;
        যদি (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    যদি (strip_pos == রিক্ত || strip_pos == pos/*eos*/)
        ঘূৰাই file;
    ঘূৰাই strip_pos;
}

শূন্য writeLogMessageEx(LogLevel logLevel, ধ্রুৱ ৱর্ণ* tag, ধ্রুৱ ৱর্ণ* file, পূর্ণাঙ্ক সাৰি, ধ্রুৱ ৱর্ণ* func, ধ্রুৱ ৱর্ণ* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    যদি (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, সাৰি, func, message);
        ঘূৰাই;
    }

    std::ostringstream strm;
    যদি (tag)
    {
        strm << tag << ' ';
    }
    যদি (file)
    {
        strm << stripSourceFilePathPrefix(file);
        যদি (সাৰি > 0)
        {
            strm << ':' << সাৰি;
        }
        strm << ' ';
    }
    যদি (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

শূন্য replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    যদি (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

শূন্য replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    যদি (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
