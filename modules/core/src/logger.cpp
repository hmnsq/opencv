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
#यदि defined न_संशोधन
    स्थिर अचर तार्किक m_isDebugBuild = असत्य;
#अन्यथा
    स्थिर अचर तार्किक m_isDebugBuild = सत्य;
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
            अचर स्वतः& parser = logTagManager.getConfigParser();
            यदि (parser.hasMalformed())
            {
                अचर स्वतः& malformedList = parser.getMalformed();
                लागि (अचर स्वतः& malformed : malformedList)
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
    फर्काऊ logTagManagerInstance;
}

स्थिर LogLevel& getLogLevelVariable()
{
    स्थिर LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    फर्काऊ refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    स्थिर LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    फर्काऊ globalLogTagPtr;
}

} // namespace

शून्य registerLogTag(LogTag* plogtag)
{
    यदि (!plogtag || !plogtag->name)
    {
        फर्काऊ;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

शून्य setLogTagLevel(अचर वर्ण* tag, LogLevel level)
{
    यदि (!tag)
    {
        फर्काऊ;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(अचर वर्ण* tag)
{
    यदि (!tag)
    {
        फर्काऊ getLogLevel();
    }
    अचर LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    यदि (!ptr)
    {
        फर्काऊ getLogLevel();
    }
    फर्काऊ ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    अचर LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    फर्काऊ old;
}

LogLevel getLogLevel()
{
    फर्काऊ internal::getLogLevelVariable();
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
    फर्काऊ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

शून्य writeLogMessage(LogLevel logLevel, अचर वर्ण* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    यदि (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        फर्काऊ;
    }

    अचर पूर्णाङ्क threadID = cv::utils::getThreadID();

    std::string message_id;
    विकल्प (getShowTimestampMode())
    {
        पक्ष 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); रोक;
        पक्ष 1+2: message_id = cv::format("%d@%llu", threadID, (दीर्घ दीर्घ अचिन्हित पूर्णाङ्क)getTimestampNS()); रोक;
        स्वभाव: message_id = cv::format("%d", threadID); रोक;
    }

    std::ostringstream ss;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; रोक;
    पक्ष LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; रोक;
    पक्ष LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; रोक;
    पक्ष LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; रोक;
    पक्ष LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; रोक;
    पक्ष LOG_LEVEL_VERBOSE: ss << message << std::endl; रोक;
    पक्ष LOG_LEVEL_SILENT: फर्काऊ;  // avoid compiler warning about incomplete switch
    पक्ष ENUM_LOG_LEVEL_FORCE_INT: फर्काऊ;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    पूर्णाङ्क android_logLevel = ANDROID_LOG_INFO;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; रोक;
    पक्ष LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; रोक;
    पक्ष LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; रोक;
    पक्ष LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; रोक;
    पक्ष LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; रोक;
    पक्ष LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; रोक;
    स्वभाव:
        रोक;
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

स्थिर अचर वर्ण* stripSourceFilePathPrefix(अचर वर्ण* file)
{
    CV_Assert(file);
    अचर वर्ण* pos = file;
    अचर वर्ण* strip_pos = रिक्त;
    वर्ण ch = 0;
    जबसम्म ((ch = pos[0]) != 0)
    {
        ++pos;
        यदि (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    यदि (strip_pos == रिक्त || strip_pos == pos/*eos*/)
        फर्काऊ file;
    फर्काऊ strip_pos;
}

शून्य writeLogMessageEx(LogLevel logLevel, अचर वर्ण* tag, अचर वर्ण* file, पूर्णाङ्क पङ्क्ति, अचर वर्ण* func, अचर वर्ण* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    यदि (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, पङ्क्ति, func, message);
        फर्काऊ;
    }

    std::ostringstream strm;
    यदि (tag)
    {
        strm << tag << ' ';
    }
    यदि (file)
    {
        strm << stripSourceFilePathPrefix(file);
        यदि (पङ्क्ति > 0)
        {
            strm << ':' << पङ्क्ति;
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
