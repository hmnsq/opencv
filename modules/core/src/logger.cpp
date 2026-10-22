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
#जर defined न_संशोधन
    स्थिर स्थिरांक तार्किक m_isDebugBuild = असत्य;
#अन्यथा
    स्थिर स्थिरांक तार्किक m_isDebugBuild = सत्य;
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

खाजगी:
    शून्य applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    शून्य handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        जर (m_isDebugBuild)
        {
            स्थिरांक स्वतः& parser = logTagManager.getConfigParser();
            जर (parser.hasMalformed())
            {
                स्थिरांक स्वतः& malformedList = parser.getMalformed();
                साठी (स्थिरांक स्वतः& malformed : malformedList)
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
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, नवीन GlobalLoggingInitStruct());
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
    परत logTagManagerInstance;
}

स्थिर LogLevel& getLogLevelVariable()
{
    स्थिर LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    परत refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    स्थिर LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    परत globalLogTagPtr;
}

} // namespace

शून्य registerLogTag(LogTag* plogtag)
{
    जर (!plogtag || !plogtag->name)
    {
        परत;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

शून्य setLogTagLevel(स्थिरांक अक्षर* tag, LogLevel level)
{
    जर (!tag)
    {
        परत;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(स्थिरांक अक्षर* tag)
{
    जर (!tag)
    {
        परत getLogLevel();
    }
    स्थिरांक LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    जर (!ptr)
    {
        परत getLogLevel();
    }
    परत ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    स्थिरांक LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    परत old;
}

LogLevel getLogLevel()
{
    परत internal::getLogLevelVariable();
}

नामाकाश internal {

नामाकाश //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

स्थिर पूर्णांक getShowTimestampMode()
{
    स्थिर तार्किक param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", सत्य);
    स्थिर तार्किक param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", असत्य);
    परत (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

शून्य writeLogMessage(LogLevel logLevel, स्थिरांक अक्षर* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    जर (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        परत;
    }

    स्थिरांक पूर्णांक threadID = cv::utils::getThreadID();

    std::string message_id;
    निवड (getShowTimestampMode())
    {
        स्थिती 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); थांबा;
        स्थिती 1+2: message_id = cv::format("%d@%llu", threadID, (दीर्घ दीर्घ अचिन्हित पूर्णांक)getTimestampNS()); थांबा;
        पूर्वनिर्धारित: message_id = cv::format("%d", threadID); थांबा;
    }

    std::ostringstream ss;
    निवड (logLevel)
    {
    स्थिती LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_VERBOSE: ss << message << std::endl; थांबा;
    स्थिती LOG_LEVEL_SILENT: परत;  // avoid compiler warning about incomplete switch
    स्थिती ENUM_LOG_LEVEL_FORCE_INT: परत;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    पूर्णांक android_logLevel = ANDROID_LOG_INFO;
    निवड (logLevel)
    {
    स्थिती LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; थांबा;
    स्थिती LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; थांबा;
    स्थिती LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; थांबा;
    स्थिती LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; थांबा;
    स्थिती LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; थांबा;
    स्थिती LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; थांबा;
    पूर्वनिर्धारित:
        थांबा;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    जर (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

स्थिर स्थिरांक अक्षर* stripSourceFilePathPrefix(स्थिरांक अक्षर* file)
{
    CV_Assert(file);
    स्थिरांक अक्षर* pos = file;
    स्थिरांक अक्षर* strip_pos = रिक्त;
    अक्षर ch = 0;
    पर्यंत ((ch = pos[0]) != 0)
    {
        ++pos;
        जर (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    जर (strip_pos == रिक्त || strip_pos == pos/*eos*/)
        परत file;
    परत strip_pos;
}

शून्य writeLogMessageEx(LogLevel logLevel, स्थिरांक अक्षर* tag, स्थिरांक अक्षर* file, पूर्णांक ओळ, स्थिरांक अक्षर* func, स्थिरांक अक्षर* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    जर (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, ओळ, func, message);
        परत;
    }

    std::ostringstream strm;
    जर (tag)
    {
        strm << tag << ' ';
    }
    जर (file)
    {
        strm << stripSourceFilePathPrefix(file);
        जर (ओळ > 0)
        {
            strm << ':' << ओळ;
        }
        strm << ' ';
    }
    जर (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

शून्य replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    जर (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

शून्य replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    जर (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
