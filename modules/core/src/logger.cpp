// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#अन्तर्भाव "utils/logtagmanager.hpp"
#अन्तर्भाव "utils/logtagconfigparser.hpp"

#अन्तर्भाव <sstream>
#अन्तर्भाव <iostream>
#अन्तर्भाव <fstream>
#अन्तर्भाव <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {

नामाकाश internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
संरचना GlobalLoggingInitStruct
{
सार्वजनिक:
#यदि defined NDEBUG
    स्थिर ध्रुव तार्किक m_isDebugBuild = असत्य;
#अन्यथा
    स्थिर ध्रुव तार्किक m_isDebugBuild = सत्य;
#endif

सार्वजनिक:
    स्थिर LogLevel m_defaultUnconfiguredGlobalLevel;

सार्वजनिक:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (शून्य)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

गुप्त:
    शून्य applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    शून्य handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        यदि (m_isDebugBuild)
        {
            ध्रुव auto& parser = logTagManager.getConfigParser();
            यदि (parser.hasMalformed())
            {
                ध्रुव auto& malformedList = parser.getMalformed();
                प्रति (ध्रुव auto& malformed : malformedList)
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
स्थिर GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, नूतन GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
संरचना GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (शून्य)getGlobalLogTag();  // complete initialization of logger structures
    }
};

स्थिर GlobalLoggingInitCall globalLoggingInitCall;

स्थिर LogTagManager& getLogTagManager()
{
    स्थिर LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    प्रत्यागम logTagManagerInstance;
}

स्थिर LogLevel& getLogLevelVariable()
{
    स्थिर LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    प्रत्यागम refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    स्थिर LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    प्रत्यागम globalLogTagPtr;
}

} // namespace

शून्य registerLogTag(LogTag* plogtag)
{
    यदि (!plogtag || !plogtag->name)
    {
        प्रत्यागम;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

शून्य setLogTagLevel(ध्रुव वर्ण* tag, LogLevel level)
{
    यदि (!tag)
    {
        प्रत्यागम;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(ध्रुव वर्ण* tag)
{
    यदि (!tag)
    {
        प्रत्यागम getLogLevel();
    }
    ध्रुव LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    यदि (!ptr)
    {
        प्रत्यागम getLogLevel();
    }
    प्रत्यागम ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ध्रुव LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    प्रत्यागम old;
}

LogLevel getLogLevel()
{
    प्रत्यागम internal::getLogLevelVariable();
}

नामाकाश internal {

नामाकाश //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

स्थिर पूर्णाङ्क getShowTimestampMode()
{
    स्थिर तार्किक param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", सत्य);
    स्थिर तार्किक param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", असत्य);
    प्रत्यागम (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

शून्य writeLogMessage(LogLevel logLevel, ध्रुव वर्ण* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    यदि (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        प्रत्यागम;
    }

    ध्रुव पूर्णाङ्क threadID = cv::utils::getThreadID();

    std::string message_id;
    विकल्प (getShowTimestampMode())
    {
        पक्ष 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); भङ्ग;
        पक्ष 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned पूर्णाङ्क)getTimestampNS()); भङ्ग;
        स्वभाव: message_id = cv::format("%d", threadID); भङ्ग;
    }

    std::ostringstream ss;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_VERBOSE: ss << message << std::endl; भङ्ग;
    पक्ष LOG_LEVEL_SILENT: प्रत्यागम;  // avoid compiler warning about incomplete switch
    पक्ष ENUM_LOG_LEVEL_FORCE_INT: प्रत्यागम;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    पूर्णाङ्क android_logLevel = ANDROID_LOG_INFO;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; भङ्ग;
    पक्ष LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; भङ्ग;
    पक्ष LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; भङ्ग;
    पक्ष LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; भङ्ग;
    पक्ष LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; भङ्ग;
    पक्ष LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; भङ्ग;
    स्वभाव:
        भङ्ग;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    यदि (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

स्थिर ध्रुव वर्ण* stripSourceFilePathPrefix(ध्रुव वर्ण* file)
{
    CV_Assert(file);
    ध्रुव वर्ण* pos = file;
    ध्रुव वर्ण* strip_pos = रिक्त;
    वर्ण ch = 0;
    यावत् ((ch = pos[0]) != 0)
    {
        ++pos;
        यदि (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    यदि (strip_pos == रिक्त || strip_pos == pos/*eos*/)
        प्रत्यागम file;
    प्रत्यागम strip_pos;
}

शून्य writeLogMessageEx(LogLevel logLevel, ध्रुव वर्ण* tag, ध्रुव वर्ण* file, पूर्णाङ्क पंक्ति, ध्रुव वर्ण* func, ध्रुव वर्ण* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    यदि (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, पंक्ति, func, message);
        प्रत्यागम;
    }

    std::ostringstream strm;
    यदि (tag)
    {
        strm << tag << ' ';
    }
    यदि (file)
    {
        strm << stripSourceFilePathPrefix(file);
        यदि (पंक्ति > 0)
        {
            strm << ':' << पंक्ति;
        }
        strm << ' ';
    }
    यदि (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

शून्य replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    यदि (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

शून्य replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    यदि (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
