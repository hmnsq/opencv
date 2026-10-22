// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>
#অন্তর্ভাব "utils/logtagmanager.hpp"
#অন্তর্ভাব "utils/logtagconfigparser.hpp"

#অন্তর্ভাব <sstream>
#অন্তর্ভাব <iostream>
#অন্তর্ভাব <fstream>
#অন্তর্ভাব <atomic>

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
কাঠামো GlobalLoggingInitStruct
{
সর্বজনীন:
#যদি defined NDEBUG
    স্থির ধ্রুবক যৌক্তিক m_isDebugBuild = মিথ্যা;
#অন্যথা
    স্থির ধ্রুবক যৌক্তিক m_isDebugBuild = সত্য;
#endif

সর্বজনীন:
    স্থির LogLevel m_defaultUnconfiguredGlobalLevel;

সর্বজনীন:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (শূন্য)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ব্যক্তিগত:
    শূন্য applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    শূন্য handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        যদি (m_isDebugBuild)
        {
            ধ্রুবক auto& parser = logTagManager.getConfigParser();
            যদি (parser.hasMalformed())
            {
                ধ্রুবক auto& malformedList = parser.getMalformed();
                জন্য (ধ্রুবক auto& malformed : malformedList)
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
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, নতুন GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
কাঠামো GlobalLoggingInitCall
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
    ফেরত logTagManagerInstance;
}

স্থির LogLevel& getLogLevelVariable()
{
    স্থির LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    ফেরত refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    স্থির LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    ফেরত globalLogTagPtr;
}

} // namespace

শূন্য registerLogTag(LogTag* plogtag)
{
    যদি (!plogtag || !plogtag->name)
    {
        ফেরত;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

শূন্য setLogTagLevel(ধ্রুবক অক্ষর* tag, LogLevel level)
{
    যদি (!tag)
    {
        ফেরত;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ধ্রুবক অক্ষর* tag)
{
    যদি (!tag)
    {
        ফেরত getLogLevel();
    }
    ধ্রুবক LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    যদি (!ptr)
    {
        ফেরত getLogLevel();
    }
    ফেরত ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ধ্রুবক LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ফেরত old;
}

LogLevel getLogLevel()
{
    ফেরত internal::getLogLevelVariable();
}

নামাকাশ internal {

নামাকাশ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

স্থির পূর্ণসংখ্যা getShowTimestampMode()
{
    স্থির যৌক্তিক param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", সত্য);
    স্থির যৌক্তিক param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", মিথ্যা);
    ফেরত (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

শূন্য writeLogMessage(LogLevel logLevel, ধ্রুবক অক্ষর* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    যদি (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        ফেরত;
    }

    ধ্রুবক পূর্ণসংখ্যা threadID = cv::utils::getThreadID();

    std::string message_id;
    নির্বাচন (getShowTimestampMode())
    {
        ক্ষেত্র 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); বিরতি;
        ক্ষেত্র 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned পূর্ণসংখ্যা)getTimestampNS()); বিরতি;
        পূর্বনির্ধারিত: message_id = cv::format("%d", threadID); বিরতি;
    }

    std::ostringstream ss;
    নির্বাচন (logLevel)
    {
    ক্ষেত্র LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_VERBOSE: ss << message << std::endl; বিরতি;
    ক্ষেত্র LOG_LEVEL_SILENT: ফেরত;  // avoid compiler warning about incomplete switch
    ক্ষেত্র ENUM_LOG_LEVEL_FORCE_INT: ফেরত;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    পূর্ণসংখ্যা android_logLevel = ANDROID_LOG_INFO;
    নির্বাচন (logLevel)
    {
    ক্ষেত্র LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; বিরতি;
    ক্ষেত্র LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; বিরতি;
    ক্ষেত্র LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; বিরতি;
    ক্ষেত্র LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; বিরতি;
    ক্ষেত্র LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; বিরতি;
    ক্ষেত্র LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; বিরতি;
    পূর্বনির্ধারিত:
        বিরতি;
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

স্থির ধ্রুবক অক্ষর* stripSourceFilePathPrefix(ধ্রুবক অক্ষর* file)
{
    CV_Assert(file);
    ধ্রুবক অক্ষর* pos = file;
    ধ্রুবক অক্ষর* strip_pos = ফাঁকা;
    অক্ষর ch = 0;
    যখন ((ch = pos[0]) != 0)
    {
        ++pos;
        যদি (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    যদি (strip_pos == ফাঁকা || strip_pos == pos/*eos*/)
        ফেরত file;
    ফেরত strip_pos;
}

শূন্য writeLogMessageEx(LogLevel logLevel, ধ্রুবক অক্ষর* tag, ধ্রুবক অক্ষর* file, পূর্ণসংখ্যা সারি, ধ্রুবক অক্ষর* func, ধ্রুবক অক্ষর* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    যদি (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, সারি, func, message);
        ফেরত;
    }

    std::ostringstream strm;
    যদি (tag)
    {
        strm << tag << ' ';
    }
    যদি (file)
    {
        strm << stripSourceFilePathPrefix(file);
        যদি (সারি > 0)
        {
            strm << ':' << সারি;
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
