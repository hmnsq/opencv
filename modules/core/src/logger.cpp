// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>
#ଅନ୍ତର୍ଭାଵ "utils/logtagmanager.hpp"
#ଅନ୍ତର୍ଭାଵ "utils/logtagconfigparser.hpp"

#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ <fstream>
#ଅନ୍ତର୍ଭାଵ <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

ନାମାକାଶ cv {
ନାମାକାଶ utils {
ନାମାକାଶ logging {

ନାମାକାଶ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ସଂରଚନା GlobalLoggingInitStruct
{
ସର୍ବସାଧାରଣ:
#ଯଦି defined NDEBUG
    ସ୍ଥିର ଅଚଳ ତାର୍କିକ m_isDebugBuild = ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    ସ୍ଥିର ଅଚଳ ତାର୍କିକ m_isDebugBuild = ସତ୍ୟ;
#endif

ସର୍ବସାଧାରଣ:
    ସ୍ଥିର LogLevel m_defaultUnconfiguredGlobalLevel;

ସର୍ବସାଧାରଣ:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (ଶୂନ୍ୟ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ବ୍ୟକ୍ତିଗତ:
    ଶୂନ୍ୟ applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    ଶୂନ୍ୟ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ଯଦି (m_isDebugBuild)
        {
            ଅଚଳ auto& parser = logTagManager.getConfigParser();
            ଯଦି (parser.hasMalformed())
            {
                ଅଚଳ auto& malformedList = parser.getMalformed();
                ପାଇଁ (ଅଚଳ auto& malformed : malformedList)
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
ସ୍ଥିର GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, ନୂତନ GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ସଂରଚନା GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (ଶୂନ୍ୟ)getGlobalLogTag();  // complete initialization of logger structures
    }
};

ସ୍ଥିର GlobalLoggingInitCall globalLoggingInitCall;

ସ୍ଥିର LogTagManager& getLogTagManager()
{
    ସ୍ଥିର LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    ଫେରନ୍ତୁ logTagManagerInstance;
}

ସ୍ଥିର LogLevel& getLogLevelVariable()
{
    ସ୍ଥିର LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    ଫେରନ୍ତୁ refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    ସ୍ଥିର LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    ଫେରନ୍ତୁ globalLogTagPtr;
}

} // namespace

ଶୂନ୍ୟ registerLogTag(LogTag* plogtag)
{
    ଯଦି (!plogtag || !plogtag->name)
    {
        ଫେରନ୍ତୁ;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

ଶୂନ୍ୟ setLogTagLevel(ଅଚଳ ଅକ୍ଷର* tag, LogLevel level)
{
    ଯଦି (!tag)
    {
        ଫେରନ୍ତୁ;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ଅଚଳ ଅକ୍ଷର* tag)
{
    ଯଦି (!tag)
    {
        ଫେରନ୍ତୁ getLogLevel();
    }
    ଅଚଳ LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    ଯଦି (!ptr)
    {
        ଫେରନ୍ତୁ getLogLevel();
    }
    ଫେରନ୍ତୁ ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ଅଚଳ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ଫେରନ୍ତୁ old;
}

LogLevel getLogLevel()
{
    ଫେରନ୍ତୁ internal::getLogLevelVariable();
}

ନାମାକାଶ internal {

ନାମାକାଶ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା getShowTimestampMode()
{
    ସ୍ଥିର ତାର୍କିକ param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", ସତ୍ୟ);
    ସ୍ଥିର ତାର୍କିକ param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", ମିଥ୍ୟା);
    ଫେରନ୍ତୁ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

ଶୂନ୍ୟ writeLogMessage(LogLevel logLevel, ଅଚଳ ଅକ୍ଷର* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    ଯଦି (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା threadID = cv::utils::getThreadID();

    std::string message_id;
    ବାଛ (getShowTimestampMode())
    {
        ସ୍ଥିତି 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ଭାଙ୍ଗ;
        ସ୍ଥିତି 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)getTimestampNS()); ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: message_id = cv::format("%d", threadID); ଭାଙ୍ଗ;
    }

    std::ostringstream ss;
    ବାଛ (logLevel)
    {
    ସ୍ଥିତି LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_VERBOSE: ss << message << std::endl; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_SILENT: ଫେରନ୍ତୁ;  // avoid compiler warning about incomplete switch
    ସ୍ଥିତି ENUM_LOG_LEVEL_FORCE_INT: ଫେରନ୍ତୁ;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା android_logLevel = ANDROID_LOG_INFO;
    ବାଛ (logLevel)
    {
    ସ୍ଥିତି LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ଭାଙ୍ଗ;
    ସ୍ଥିତି LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଭାଙ୍ଗ;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    ଯଦି (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* stripSourceFilePathPrefix(ଅଚଳ ଅକ୍ଷର* file)
{
    CV_Assert(file);
    ଅଚଳ ଅକ୍ଷର* pos = file;
    ଅଚଳ ଅକ୍ଷର* strip_pos = ଖାଲି;
    ଅକ୍ଷର ch = 0;
    ଯେତେବେଳେ ((ch = pos[0]) != 0)
    {
        ++pos;
        ଯଦି (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ଯଦି (strip_pos == ଖାଲି || strip_pos == pos/*eos*/)
        ଫେରନ୍ତୁ file;
    ଫେରନ୍ତୁ strip_pos;
}

ଶୂନ୍ୟ writeLogMessageEx(LogLevel logLevel, ଅଚଳ ଅକ୍ଷର* tag, ଅଚଳ ଅକ୍ଷର* file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି, ଅଚଳ ଅକ୍ଷର* func, ଅଚଳ ଅକ୍ଷର* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    ଯଦି (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, ଧାଡ଼ି, func, message);
        ଫେରନ୍ତୁ;
    }

    std::ostringstream strm;
    ଯଦି (tag)
    {
        strm << tag << ' ';
    }
    ଯଦି (file)
    {
        strm << stripSourceFilePathPrefix(file);
        ଯଦି (ଧାଡ଼ି > 0)
        {
            strm << ':' << ଧାଡ଼ି;
        }
        strm << ' ';
    }
    ଯଦି (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

ଶୂନ୍ୟ replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    ଯଦି (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

ଶୂନ୍ୟ replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    ଯଦି (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
