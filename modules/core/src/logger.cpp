// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/logger.hpp>
#ضمن "utils/logtagmanager.hpp"
#ضمن "utils/logtagconfigparser.hpp"

#ضمن <sstream>
#ضمن <iostream>
#ضمن <fstream>
#ضمن <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

نطاق cv {
نطاق utils {
نطاق logging {

نطاق internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
بنية GlobalLoggingInitStruct
{
عام:
#إذا defined NDEBUG
    ساكن ثابت منطقي m_isDebugBuild = خطأ;
#وإلا
    ساكن ثابت منطقي m_isDebugBuild = true;
#endif

عام:
    ساكن LogLevel m_defaultUnconfiguredGlobalLevel;

عام:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (فراغ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

خاص:
    فراغ applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    فراغ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        إذا (m_isDebugBuild)
        {
            ثابت auto& parser = logTagManager.getConfigParser();
            إذا (parser.hasMalformed())
            {
                ثابت auto& malformedList = parser.getMalformed();
                لكل (ثابت auto& malformed : malformedList)
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
ساكن GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, جديد GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
بنية GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (فراغ)getGlobalLogTag();  // complete initialization of logger structures
    }
};

ساكن GlobalLoggingInitCall globalLoggingInitCall;

ساكن LogTagManager& getLogTagManager()
{
    ساكن LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    أرجع logTagManagerInstance;
}

ساكن LogLevel& getLogLevelVariable()
{
    ساكن LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    أرجع refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    ساكن LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    أرجع globalLogTagPtr;
}

} // namespace

فراغ registerLogTag(LogTag* plogtag)
{
    إذا (!plogtag || !plogtag->name)
    {
        أرجع;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

فراغ setLogTagLevel(ثابت حرف* tag, LogLevel level)
{
    إذا (!tag)
    {
        أرجع;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ثابت حرف* tag)
{
    إذا (!tag)
    {
        أرجع getLogLevel();
    }
    ثابت LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    إذا (!ptr)
    {
        أرجع getLogLevel();
    }
    أرجع ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ثابت LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    أرجع old;
}

LogLevel getLogLevel()
{
    أرجع internal::getLogLevelVariable();
}

نطاق internal {

نطاق //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

ساكن صحيح getShowTimestampMode()
{
    ساكن منطقي param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", true);
    ساكن منطقي param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", خطأ);
    أرجع (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

فراغ writeLogMessage(LogLevel logLevel, ثابت حرف* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    إذا (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        أرجع;
    }

    ثابت صحيح threadID = cv::utils::getThreadID();

    std::string message_id;
    بدل (getShowTimestampMode())
    {
        حالة 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); اكسر;
        حالة 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned صحيح)getTimestampNS()); اكسر;
        افتراضي: message_id = cv::format("%d", threadID); اكسر;
    }

    std::ostringstream ss;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; اكسر;
    حالة LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; اكسر;
    حالة LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; اكسر;
    حالة LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; اكسر;
    حالة LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; اكسر;
    حالة LOG_LEVEL_VERBOSE: ss << message << std::endl; اكسر;
    حالة LOG_LEVEL_SILENT: أرجع;  // avoid compiler warning about incomplete switch
    حالة ENUM_LOG_LEVEL_FORCE_INT: أرجع;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    صحيح android_logLevel = ANDROID_LOG_INFO;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; اكسر;
    حالة LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; اكسر;
    حالة LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; اكسر;
    حالة LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; اكسر;
    حالة LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; اكسر;
    حالة LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; اكسر;
    افتراضي:
        اكسر;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    إذا (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ساكن ثابت حرف* stripSourceFilePathPrefix(ثابت حرف* file)
{
    CV_Assert(file);
    ثابت حرف* pos = file;
    ثابت حرف* strip_pos = عدم;
    حرف ch = 0;
    طالما ((ch = pos[0]) != 0)
    {
        ++pos;
        إذا (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    إذا (strip_pos == عدم || strip_pos == pos/*eos*/)
        أرجع file;
    أرجع strip_pos;
}

فراغ writeLogMessageEx(LogLevel logLevel, ثابت حرف* tag, ثابت حرف* file, صحيح هێڵ, ثابت حرف* func, ثابت حرف* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    إذا (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, هێڵ, func, message);
        أرجع;
    }

    std::ostringstream strm;
    إذا (tag)
    {
        strm << tag << ' ';
    }
    إذا (file)
    {
        strm << stripSourceFilePathPrefix(file);
        إذا (هێڵ > 0)
        {
            strm << ':' << هێڵ;
        }
        strm << ' ';
    }
    إذا (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

فراغ replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    إذا (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

فراغ replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    إذا (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
