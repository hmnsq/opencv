// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>
#அந்தர்பாவ "utils/logtagmanager.hpp"
#அந்தர்பாவ "utils/logtagconfigparser.hpp"

#அந்தர்பாவ <sstream>
#அந்தர்பாவ <iostream>
#அந்தர்பாவ <fstream>
#அந்தர்பாவ <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

நாமாகாஶ cv {
நாமாகாஶ utils {
நாமாகாஶ logging {

நாமாகாஶ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
கட்டமைப்பு GlobalLoggingInitStruct
{
பொது:
#எனில் defined NDEBUG
    நிலையான மாறிலி தருக்க m_isDebugBuild = பொய்;
#இல்லையெனில்
    நிலையான மாறிலி தருக்க m_isDebugBuild = உண்மை;
#endif

பொது:
    நிலையான LogLevel m_defaultUnconfiguredGlobalLevel;

பொது:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (வெறுமை)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

தனிப்பட்ட:
    வெறுமை applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    வெறுமை handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        எனில் (m_isDebugBuild)
        {
            மாறிலி auto& parser = logTagManager.getConfigParser();
            எனில் (parser.hasMalformed())
            {
                மாறிலி auto& malformedList = parser.getMalformed();
                க்கு (மாறிலி auto& malformed : malformedList)
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
நிலையான GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, புதிய GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
கட்டமைப்பு GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (வெறுமை)getGlobalLogTag();  // complete initialization of logger structures
    }
};

நிலையான GlobalLoggingInitCall globalLoggingInitCall;

நிலையான LogTagManager& getLogTagManager()
{
    நிலையான LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    திரும்பு logTagManagerInstance;
}

நிலையான LogLevel& getLogLevelVariable()
{
    நிலையான LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    திரும்பு refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    நிலையான LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    திரும்பு globalLogTagPtr;
}

} // namespace

வெறுமை registerLogTag(LogTag* plogtag)
{
    எனில் (!plogtag || !plogtag->name)
    {
        திரும்பு;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

வெறுமை setLogTagLevel(மாறிலி எழுத்து* tag, LogLevel level)
{
    எனில் (!tag)
    {
        திரும்பு;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(மாறிலி எழுத்து* tag)
{
    எனில் (!tag)
    {
        திரும்பு getLogLevel();
    }
    மாறிலி LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    எனில் (!ptr)
    {
        திரும்பு getLogLevel();
    }
    திரும்பு ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    மாறிலி LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    திரும்பு old;
}

LogLevel getLogLevel()
{
    திரும்பு internal::getLogLevelVariable();
}

நாமாகாஶ internal {

நாமாகாஶ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

நிலையான முழு getShowTimestampMode()
{
    நிலையான தருக்க param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", உண்மை);
    நிலையான தருக்க param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", பொய்);
    திரும்பு (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

வெறுமை writeLogMessage(LogLevel logLevel, மாறிலி எழுத்து* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    எனில் (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        திரும்பு;
    }

    மாறிலி முழு threadID = cv::utils::getThreadID();

    std::string message_id;
    தேர்வு (getShowTimestampMode())
    {
        நிலை 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); நிறுத்து;
        நிலை 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned முழு)getTimestampNS()); நிறுத்து;
        இயல்பு: message_id = cv::format("%d", threadID); நிறுத்து;
    }

    std::ostringstream ss;
    தேர்வு (logLevel)
    {
    நிலை LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_VERBOSE: ss << message << std::endl; நிறுத்து;
    நிலை LOG_LEVEL_SILENT: திரும்பு;  // avoid compiler warning about incomplete switch
    நிலை ENUM_LOG_LEVEL_FORCE_INT: திரும்பு;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    முழு android_logLevel = ANDROID_LOG_INFO;
    தேர்வு (logLevel)
    {
    நிலை LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; நிறுத்து;
    நிலை LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; நிறுத்து;
    நிலை LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; நிறுத்து;
    நிலை LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; நிறுத்து;
    நிலை LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; நிறுத்து;
    நிலை LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; நிறுத்து;
    இயல்பு:
        நிறுத்து;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    எனில் (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

நிலையான மாறிலி எழுத்து* stripSourceFilePathPrefix(மாறிலி எழுத்து* file)
{
    CV_Assert(file);
    மாறிலி எழுத்து* pos = file;
    மாறிலி எழுத்து* strip_pos = வெற்று;
    எழுத்து ch = 0;
    போது ((ch = pos[0]) != 0)
    {
        ++pos;
        எனில் (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    எனில் (strip_pos == வெற்று || strip_pos == pos/*eos*/)
        திரும்பு file;
    திரும்பு strip_pos;
}

வெறுமை writeLogMessageEx(LogLevel logLevel, மாறிலி எழுத்து* tag, மாறிலி எழுத்து* file, முழு வரி, மாறிலி எழுத்து* func, மாறிலி எழுத்து* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    எனில் (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, வரி, func, message);
        திரும்பு;
    }

    std::ostringstream strm;
    எனில் (tag)
    {
        strm << tag << ' ';
    }
    எனில் (file)
    {
        strm << stripSourceFilePathPrefix(file);
        எனில் (வரி > 0)
        {
            strm << ':' << வரி;
        }
        strm << ' ';
    }
    எனில் (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

வெறுமை replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    எனில் (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

வெறுமை replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    எனில் (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
