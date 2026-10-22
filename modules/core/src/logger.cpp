// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"

#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश <opencv2/core/utils/logger.hpp>
#समावेश "utils/logtagmanager.hpp"
#समावेश "utils/logtagconfigparser.hpp"

#समावेश <sstream>
#समावेश <iostream>
#समावेश <fstream>
#समावेश <atomic>

#अगर_घोषित __ANDROID__
# include <android/log.h>
#पूर्ण_अगर

namespace cv {
namespace utils {
namespace logging {

namespace internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
काष्ठा GlobalLoggingInitStruct
{
खुला:
#अगर defined न_संशोधन
    अटल स्थिर bool m_isDebugBuild = false;
#अन्यथा
    अटल स्थिर bool m_isDebugBuild = true;
#पूर्ण_अगर

खुला:
    अटल LogLevel m_defaultUnconfiguredGlobalLevel;

खुला:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (व्योम)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

निजी:
    व्योम applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    व्योम handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        अगर (m_isDebugBuild)
        {
            स्थिर स्वतः& parser = logTagManager.getConfigParser();
            अगर (parser.hasMalformed())
            {
                स्थिर स्वतः& malformedList = parser.getMalformed();
                क्रम (स्थिर स्वतः& malformed : malformedList)
                {
                    std::अ_बाहर << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::अ_बाहर.सफाई();
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
अटल GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, नया GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
काष्ठा GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (व्योम)getGlobalLogTag();  // complete initialization of logger structures
    }
};

अटल GlobalLoggingInitCall globalLoggingInitCall;

अटल LogTagManager& getLogTagManager()
{
    अटल LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    वापस logTagManagerInstance;
}

अटल LogLevel& getLogLevelVariable()
{
    अटल LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    वापस refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    अटल LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.लो("global");
    वापस globalLogTagPtr;
}

} // namespace

व्योम registerLogTag(LogTag* plogtag)
{
    अगर (!plogtag || !plogtag->name)
    {
        वापस;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

व्योम setLogTagLevel(स्थिर अक्षर* tag, LogLevel level)
{
    अगर (!tag)
    {
        वापस;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(स्थिर अक्षर* tag)
{
    अगर (!tag)
    {
        वापस getLogLevel();
    }
    स्थिर LogTag* ptr = internal::getLogTagManager().लो(std::string(tag));
    अगर (!ptr)
    {
        वापस getLogLevel();
    }
    वापस ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    स्थिर LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    वापस old;
}

LogLevel getLogLevel()
{
    वापस internal::getLogLevelVariable();
}

namespace internal {

namespace //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

अटल पूर्णांक getShowTimestampMode()
{
    अटल bool param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", true);
    अटल bool param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", false);
    वापस (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

व्योम writeLogMessage(LogLevel logLevel, स्थिर अक्षर* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    अगर (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        वापस;
    }

    स्थिर पूर्णांक threadID = cv::utils::getThreadID();

    std::string message_id;
    चयन (getShowTimestampMode())
    {
        हाल 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); अवरोध;
        हाल 1+2: message_id = cv::format("%d@%llu", threadID, (दीर्घ दीर्घ अचिन्हित पूर्णांक)getTimestampNS()); अवरोध;
        शेष: message_id = cv::format("%d", threadID); अवरोध;
    }

    std::ostringstream ss;
    चयन (logLevel)
    {
    हाल LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_VERBOSE: ss << message << std::प_पूर्ण; अवरोध;
    हाल LOG_LEVEL_SILENT: वापस;  // avoid compiler warning about incomplete switch
    हाल ENUM_LOG_LEVEL_FORCE_INT: वापस;  // avoid compiler warning about incomplete switch
    }
#अगर_घोषित __ANDROID__
    पूर्णांक android_logLevel = ANDROID_LOG_INFO;
    चयन (logLevel)
    {
    हाल LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; अवरोध;
    हाल LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; अवरोध;
    हाल LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; अवरोध;
    हाल LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; अवरोध;
    हाल LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; अवरोध;
    हाल LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; अवरोध;
    शेष:
        अवरोध;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#पूर्ण_अगर
    std::ब_प्रवाह* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::अ_त्रुटि : &std::अ_बाहर;
    (*out) << ss.str();
    अगर (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::सफाई;
    }
}

अटल स्थिर अक्षर* stripSourceFilePathPrefix(स्थिर अक्षर* file)
{
    CV_Assert(file);
    स्थिर अक्षर* pos = file;
    स्थिर अक्षर* strip_pos = शून्य;
    अक्षर ch = 0;
    जबतक ((ch = pos[0]) != 0)
    {
        ++pos;
        अगर (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    अगर (strip_pos == शून्य || strip_pos == pos/*eos*/)
        वापस file;
    वापस strip_pos;
}

व्योम writeLogMessageEx(LogLevel logLevel, स्थिर अक्षर* tag, स्थिर अक्षर* file, पूर्णांक पंक्ति, स्थिर अक्षर* func, स्थिर अक्षर* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    अगर (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, पंक्ति, func, message);
        वापस;
    }

    std::ostringstream strm;
    अगर (tag)
    {
        strm << tag << ' ';
    }
    अगर (file)
    {
        strm << stripSourceFilePathPrefix(file);
        अगर (पंक्ति > 0)
        {
            strm << ':' << पंक्ति;
        }
        strm << ' ';
    }
    अगर (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

व्योम replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    अगर (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

व्योम replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    अगर (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
