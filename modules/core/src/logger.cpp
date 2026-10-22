// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>
#અન્તર્ભાવ "utils/logtagmanager.hpp"
#અન્તર્ભાવ "utils/logtagconfigparser.hpp"

#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <iostream>
#અન્તર્ભાવ <fstream>
#અન્તર્ભાવ <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

નામાકાશ cv {
નામાકાશ utils {
નામાકાશ logging {

નામાકાશ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
સંરચના GlobalLoggingInitStruct
{
જાહેર:
#જો defined NDEBUG
    સ્થિર અચલ તાર્કિક m_isDebugBuild = ખોટું;
#નહીંતર
    સ્થિર અચલ તાર્કિક m_isDebugBuild = સાચું;
#endif

જાહેર:
    સ્થિર LogLevel m_defaultUnconfiguredGlobalLevel;

જાહેર:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (શૂન્ય)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ખાનગી:
    શૂન્ય applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    શૂન્ય handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        જો (m_isDebugBuild)
        {
            અચલ auto& parser = logTagManager.getConfigParser();
            જો (parser.hasMalformed())
            {
                અચલ auto& malformedList = parser.getMalformed();
                માટે (અચલ auto& malformed : malformedList)
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
સ્થિર GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, નવું GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
સંરચના GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (શૂન્ય)getGlobalLogTag();  // complete initialization of logger structures
    }
};

સ્થિર GlobalLoggingInitCall globalLoggingInitCall;

સ્થિર LogTagManager& getLogTagManager()
{
    સ્થિર LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    પાછા logTagManagerInstance;
}

સ્થિર LogLevel& getLogLevelVariable()
{
    સ્થિર LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    પાછા refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    સ્થિર LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    પાછા globalLogTagPtr;
}

} // namespace

શૂન્ય registerLogTag(LogTag* plogtag)
{
    જો (!plogtag || !plogtag->name)
    {
        પાછા;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

શૂન્ય setLogTagLevel(અચલ અક્ષર* tag, LogLevel level)
{
    જો (!tag)
    {
        પાછા;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(અચલ અક્ષર* tag)
{
    જો (!tag)
    {
        પાછા getLogLevel();
    }
    અચલ LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    જો (!ptr)
    {
        પાછા getLogLevel();
    }
    પાછા ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    અચલ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    પાછા old;
}

LogLevel getLogLevel()
{
    પાછા internal::getLogLevelVariable();
}

નામાકાશ internal {

નામાકાશ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

સ્થિર પૂર્ણાંક getShowTimestampMode()
{
    સ્થિર તાર્કિક param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", સાચું);
    સ્થિર તાર્કિક param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", ખોટું);
    પાછા (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

શૂન્ય writeLogMessage(LogLevel logLevel, અચલ અક્ષર* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    જો (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        પાછા;
    }

    અચલ પૂર્ણાંક threadID = cv::utils::getThreadID();

    std::string message_id;
    પસંદગી (getShowTimestampMode())
    {
        સ્થિતિ 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); તોડો;
        સ્થિતિ 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned પૂર્ણાંક)getTimestampNS()); તોડો;
        મૂળભૂત: message_id = cv::format("%d", threadID); તોડો;
    }

    std::ostringstream ss;
    પસંદગી (logLevel)
    {
    સ્થિતિ LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_VERBOSE: ss << message << std::endl; તોડો;
    સ્થિતિ LOG_LEVEL_SILENT: પાછા;  // avoid compiler warning about incomplete switch
    સ્થિતિ ENUM_LOG_LEVEL_FORCE_INT: પાછા;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    પૂર્ણાંક android_logLevel = ANDROID_LOG_INFO;
    પસંદગી (logLevel)
    {
    સ્થિતિ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; તોડો;
    સ્થિતિ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; તોડો;
    સ્થિતિ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; તોડો;
    સ્થિતિ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; તોડો;
    સ્થિતિ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; તોડો;
    સ્થિતિ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; તોડો;
    મૂળભૂત:
        તોડો;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    જો (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

સ્થિર અચલ અક્ષર* stripSourceFilePathPrefix(અચલ અક્ષર* file)
{
    CV_Assert(file);
    અચલ અક્ષર* pos = file;
    અચલ અક્ષર* strip_pos = ખાલી;
    અક્ષર ch = 0;
    જ્યારે ((ch = pos[0]) != 0)
    {
        ++pos;
        જો (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    જો (strip_pos == ખાલી || strip_pos == pos/*eos*/)
        પાછા file;
    પાછા strip_pos;
}

શૂન્ય writeLogMessageEx(LogLevel logLevel, અચલ અક્ષર* tag, અચલ અક્ષર* file, પૂર્ણાંક લીટી, અચલ અક્ષર* func, અચલ અક્ષર* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    જો (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, લીટી, func, message);
        પાછા;
    }

    std::ostringstream strm;
    જો (tag)
    {
        strm << tag << ' ';
    }
    જો (file)
    {
        strm << stripSourceFilePathPrefix(file);
        જો (લીટી > 0)
        {
            strm << ':' << લીટી;
        }
        strm << ' ';
    }
    જો (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

શૂન્ય replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    જો (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

શૂન્ય replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    જો (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
