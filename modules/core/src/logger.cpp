// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>
#അന്തര്ഭാവ "utils/logtagmanager.hpp"
#അന്തര്ഭാവ "utils/logtagconfigparser.hpp"

#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <iostream>
#അന്തര്ഭാവ <fstream>
#അന്തര്ഭാവ <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

നാമാകാശ cv {
നാമാകാശ utils {
നാമാകാശ logging {

നാമാകാശ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ഘടന GlobalLoggingInitStruct
{
പൊതു:
#എങ്കിൽ defined NDEBUG
    സ്ഥിതം സ്ഥിരം തർക്കം m_isDebugBuild = അസത്യം;
#അല്ലെങ്കിൽ
    സ്ഥിതം സ്ഥിരം തർക്കം m_isDebugBuild = സത്യം;
#endif

പൊതു:
    സ്ഥിതം LogLevel m_defaultUnconfiguredGlobalLevel;

പൊതു:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (ശൂന്യം)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

സ്വകാര്യ:
    ശൂന്യം applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    ശൂന്യം handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        എങ്കിൽ (m_isDebugBuild)
        {
            സ്ഥിരം auto& parser = logTagManager.getConfigParser();
            എങ്കിൽ (parser.hasMalformed())
            {
                സ്ഥിരം auto& malformedList = parser.getMalformed();
                വേണ്ടി (സ്ഥിരം auto& malformed : malformedList)
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
സ്ഥിതം GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, പുതിയ GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ഘടന GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (ശൂന്യം)getGlobalLogTag();  // complete initialization of logger structures
    }
};

സ്ഥിതം GlobalLoggingInitCall globalLoggingInitCall;

സ്ഥിതം LogTagManager& getLogTagManager()
{
    സ്ഥിതം LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    തിരികെ logTagManagerInstance;
}

സ്ഥിതം LogLevel& getLogLevelVariable()
{
    സ്ഥിതം LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    തിരികെ refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    സ്ഥിതം LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    തിരികെ globalLogTagPtr;
}

} // namespace

ശൂന്യം registerLogTag(LogTag* plogtag)
{
    എങ്കിൽ (!plogtag || !plogtag->name)
    {
        തിരികെ;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

ശൂന്യം setLogTagLevel(സ്ഥിരം അക്ഷരം* tag, LogLevel level)
{
    എങ്കിൽ (!tag)
    {
        തിരികെ;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(സ്ഥിരം അക്ഷരം* tag)
{
    എങ്കിൽ (!tag)
    {
        തിരികെ getLogLevel();
    }
    സ്ഥിരം LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    എങ്കിൽ (!ptr)
    {
        തിരികെ getLogLevel();
    }
    തിരികെ ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    സ്ഥിരം LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    തിരികെ old;
}

LogLevel getLogLevel()
{
    തിരികെ internal::getLogLevelVariable();
}

നാമാകാശ internal {

നാമാകാശ //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

സ്ഥിതം പൂർണ്ണസംഖ്യ getShowTimestampMode()
{
    സ്ഥിതം തർക്കം param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", സത്യം);
    സ്ഥിതം തർക്കം param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", അസത്യം);
    തിരികെ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

ശൂന്യം writeLogMessage(LogLevel logLevel, സ്ഥിരം അക്ഷരം* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    എങ്കിൽ (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        തിരികെ;
    }

    സ്ഥിരം പൂർണ്ണസംഖ്യ threadID = cv::utils::getThreadID();

    std::string message_id;
    തിരഞ്ഞെടുപ്പ് (getShowTimestampMode())
    {
        അവസ്ഥ 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); നിർത്തുക;
        അവസ്ഥ 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned പൂർണ്ണസംഖ്യ)getTimestampNS()); നിർത്തുക;
        സ്ഥിരസ്ഥിതി: message_id = cv::format("%d", threadID); നിർത്തുക;
    }

    std::ostringstream ss;
    തിരഞ്ഞെടുപ്പ് (logLevel)
    {
    അവസ്ഥ LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_VERBOSE: ss << message << std::endl; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_SILENT: തിരികെ;  // avoid compiler warning about incomplete switch
    അവസ്ഥ ENUM_LOG_LEVEL_FORCE_INT: തിരികെ;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    പൂർണ്ണസംഖ്യ android_logLevel = ANDROID_LOG_INFO;
    തിരഞ്ഞെടുപ്പ് (logLevel)
    {
    അവസ്ഥ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; നിർത്തുക;
    അവസ്ഥ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        നിർത്തുക;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    എങ്കിൽ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

സ്ഥിതം സ്ഥിരം അക്ഷരം* stripSourceFilePathPrefix(സ്ഥിരം അക്ഷരം* file)
{
    CV_Assert(file);
    സ്ഥിരം അക്ഷരം* pos = file;
    സ്ഥിരം അക്ഷരം* strip_pos = ശൂന്യ;
    അക്ഷരം ch = 0;
    ആകുമ്പോൾ ((ch = pos[0]) != 0)
    {
        ++pos;
        എങ്കിൽ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    എങ്കിൽ (strip_pos == ശൂന്യ || strip_pos == pos/*eos*/)
        തിരികെ file;
    തിരികെ strip_pos;
}

ശൂന്യം writeLogMessageEx(LogLevel logLevel, സ്ഥിരം അക്ഷരം* tag, സ്ഥിരം അക്ഷരം* file, പൂർണ്ണസംഖ്യ വരി, സ്ഥിരം അക്ഷരം* func, സ്ഥിരം അക്ഷരം* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    എങ്കിൽ (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, വരി, func, message);
        തിരികെ;
    }

    std::ostringstream strm;
    എങ്കിൽ (tag)
    {
        strm << tag << ' ';
    }
    എങ്കിൽ (file)
    {
        strm << stripSourceFilePathPrefix(file);
        എങ്കിൽ (വരി > 0)
        {
            strm << ':' << വരി;
        }
        strm << ' ';
    }
    എങ്കിൽ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

ശൂന്യം replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    എങ്കിൽ (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

ശൂന്യം replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    എങ്കിൽ (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
