// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/logger.hpp>
#شامل "utils/logtagmanager.hpp"
#شامل "utils/logtagconfigparser.hpp"

#شامل <sstream>
#شامل <iostream>
#شامل <fstream>
#شامل <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

فضاینام cv {
فضاینام utils {
فضاینام logging {

فضاینام internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ساختار GlobalLoggingInitStruct
{
عمومی:
#اگر defined NDEBUG
    ایستا ثابت منطقی m_isDebugBuild = نادرست;
#وگرنه
    ایستا ثابت منطقی m_isDebugBuild = درست;
#endif

عمومی:
    ایستا LogLevel m_defaultUnconfiguredGlobalLevel;

عمومی:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (تهی)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

خصوصی:
    تهی applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    تهی handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        اگر (m_isDebugBuild)
        {
            ثابت auto& parser = logTagManager.getConfigParser();
            اگر (parser.hasMalformed())
            {
                ثابت auto& malformedList = parser.getMalformed();
                برای (ثابت auto& malformed : malformedList)
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
ایستا GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, جدید GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ساختار GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (تهی)getGlobalLogTag();  // complete initialization of logger structures
    }
};

ایستا GlobalLoggingInitCall globalLoggingInitCall;

ایستا LogTagManager& getLogTagManager()
{
    ایستا LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    بازگشت logTagManagerInstance;
}

ایستا LogLevel& getLogLevelVariable()
{
    ایستا LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    بازگشت refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    ایستا LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    بازگشت globalLogTagPtr;
}

} // namespace

تهی registerLogTag(LogTag* plogtag)
{
    اگر (!plogtag || !plogtag->name)
    {
        بازگشت;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

تهی setLogTagLevel(ثابت نویسه* tag, LogLevel level)
{
    اگر (!tag)
    {
        بازگشت;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ثابت نویسه* tag)
{
    اگر (!tag)
    {
        بازگشت getLogLevel();
    }
    ثابت LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    اگر (!ptr)
    {
        بازگشت getLogLevel();
    }
    بازگشت ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ثابت LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    بازگشت old;
}

LogLevel getLogLevel()
{
    بازگشت internal::getLogLevelVariable();
}

فضاینام internal {

فضاینام //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

ایستا صحیح getShowTimestampMode()
{
    ایستا منطقی param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", درست);
    ایستا منطقی param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", نادرست);
    بازگشت (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

تهی writeLogMessage(LogLevel logLevel, ثابت نویسه* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    اگر (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        بازگشت;
    }

    ثابت صحیح threadID = cv::utils::getThreadID();

    std::string message_id;
    انتخاب (getShowTimestampMode())
    {
        حالت 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); بشکن;
        حالت 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned صحیح)getTimestampNS()); بشکن;
        پیشفرض: message_id = cv::format("%d", threadID); بشکن;
    }

    std::ostringstream ss;
    انتخاب (logLevel)
    {
    حالت LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; بشکن;
    حالت LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; بشکن;
    حالت LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; بشکن;
    حالت LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; بشکن;
    حالت LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; بشکن;
    حالت LOG_LEVEL_VERBOSE: ss << message << std::endl; بشکن;
    حالت LOG_LEVEL_SILENT: بازگشت;  // avoid compiler warning about incomplete switch
    حالت ENUM_LOG_LEVEL_FORCE_INT: بازگشت;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    صحیح android_logLevel = ANDROID_LOG_INFO;
    انتخاب (logLevel)
    {
    حالت LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; بشکن;
    حالت LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; بشکن;
    حالت LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; بشکن;
    حالت LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; بشکن;
    حالت LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; بشکن;
    حالت LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; بشکن;
    پیشفرض:
        بشکن;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    اگر (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ایستا ثابت نویسه* stripSourceFilePathPrefix(ثابت نویسه* file)
{
    CV_Assert(file);
    ثابت نویسه* pos = file;
    ثابت نویسه* strip_pos = پوچ;
    نویسه ch = 0;
    وقتی ((ch = pos[0]) != 0)
    {
        ++pos;
        اگر (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    اگر (strip_pos == پوچ || strip_pos == pos/*eos*/)
        بازگشت file;
    بازگشت strip_pos;
}

تهی writeLogMessageEx(LogLevel logLevel, ثابت نویسه* tag, ثابت نویسه* file, صحیح سطر, ثابت نویسه* func, ثابت نویسه* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    اگر (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, سطر, func, message);
        بازگشت;
    }

    std::ostringstream strm;
    اگر (tag)
    {
        strm << tag << ' ';
    }
    اگر (file)
    {
        strm << stripSourceFilePathPrefix(file);
        اگر (سطر > 0)
        {
            strm << ':' << سطر;
        }
        strm << ' ';
    }
    اگر (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

تهی replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    اگر (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

تهی replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    اگر (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
