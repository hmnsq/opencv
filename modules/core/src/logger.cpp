// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "utils/logtagmanager.hpp"
#include "utils/logtagconfigparser.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <atomic>

#ifdef __ANDROID__
# include <android/log.h>
#endif

namespace cv {
namespace utils {
namespace logging {

namespace internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
sa_nracanaa GlobalLoggingInitStruct
{
public:
#ប្រសិនបើ defined NDEBUG
    static const bool m_isDebugBuild = false;
#បើមិនដូច្នេះ
    static const bool m_isDebugBuild = true;
#endif

public:
    static LogLevel m_defaultUnconfiguredGlobalLevel;

public:
    LogTagManager logTagManager;

    GlobalLoggingInitStruct()
        : logTagManager(m_defaultUnconfiguredGlobalLevel)
    {
        (void)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

private:
    void applyConfigString()
    {
        logTagManager.setConfigString(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    void handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ប្រសិនបើ (m_isDebugBuild)
        {
            const auto& parser = logTagManager.getConfigParser();
            ប្រសិនបើ (parser.hasMalformed())
            {
                const auto& malformedList = parser.getMalformed();
                សម្រាប់ (const auto& malformed : malformedList)
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
static GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, new GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
sa_nracanaa GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (void)getGlobalLogTag();  // complete initialization of logger structures
    }
};

static GlobalLoggingInitCall globalLoggingInitCall;

static LogTagManager& getLogTagManager()
{
    static LogTagManager& logTagManagerInstance = getGlobalLoggingInitStruct().logTagManager;
    ត្រឡប់ logTagManagerInstance;
}

static LogLevel& getLogLevelVariable()
{
    static LogLevel& refGlobalLogLevel = getGlobalLogTag()->level;
    ត្រឡប់ refGlobalLogLevel;
}

LogTag* getGlobalLogTag()
{
    static LogTag* globalLogTagPtr = getGlobalLoggingInitStruct().logTagManager.get("global");
    ត្រឡប់ globalLogTagPtr;
}

} // namespace

void registerLogTag(LogTag* plogtag)
{
    ប្រសិនបើ (!plogtag || !plogtag->name)
    {
        ត្រឡប់;
    }
    internal::getLogTagManager().assign(plogtag->name, plogtag);
}

void setLogTagLevel(const var_na* tag, LogLevel level)
{
    ប្រសិនបើ (!tag)
    {
        ត្រឡប់;
    }
    internal::getLogTagManager().setLevelByFullName(std::string(tag), level);
}

LogLevel getLogTagLevel(const var_na* tag)
{
    ប្រសិនបើ (!tag)
    {
        ត្រឡប់ getLogLevel();
    }
    const LogTag* ptr = internal::getLogTagManager().get(std::string(tag));
    ប្រសិនបើ (!ptr)
    {
        ត្រឡប់ getLogLevel();
    }
    ត្រឡប់ ptr->level;
}

LogLevel setLogLevel(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    const LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ត្រឡប់ old;
}

LogLevel getLogLevel()
{
    ត្រឡប់ internal::getLogLevelVariable();
}

namespace internal {

namespace //unnamed
{
    std::atomic<WriteLogMessageFuncType> stc_userWriteLogMessageFunc{};
    std::atomic<WriteLogMessageExFuncType> stc_userWriteLogMessageExFunc{};
} //unnamed

static ចំនួនគត់ getShowTimestampMode()
{
    static bool param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", true);
    static bool param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", false);
    ត្រឡប់ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

void writeLogMessage(LogLevel logLevel, const var_na* message)
{
    WriteLogMessageFuncType userFunc = stc_userWriteLogMessageFunc.load();
    ប្រសិនបើ (userFunc && userFunc != writeLogMessage)
    {
        (*userFunc)(logLevel, message);
        ត្រឡប់;
    }

    const ចំនួនគត់ threadID = cv::utils::getThreadID();

    std::string message_id;
    switch (getShowTimestampMode())
    {
        pak_sa 1: message_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); bha_nga;
        pak_sa 1+2: message_id = cv::format("%d@%llu", threadID, (long long unsigned ចំនួនគត់)getTimestampNS()); bha_nga;
        default: message_id = cv::format("%d", threadID); bha_nga;
    }

    std::ostringstream ss;
    switch (logLevel)
    {
    pak_sa LOG_LEVEL_FATAL:   ss << "[FATAL:" << message_id << "] " << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_ERROR:   ss << "[ERROR:" << message_id << "] " << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_WARNING: ss << "[ WARN:" << message_id << "] " << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_INFO:    ss << "[ INFO:" << message_id << "] " << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << message_id << "] " << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_VERBOSE: ss << message << std::endl; bha_nga;
    pak_sa LOG_LEVEL_SILENT: ត្រឡប់;  // avoid compiler warning about incomplete switch
    pak_sa ENUM_LOG_LEVEL_FORCE_INT: ត្រឡប់;  // avoid compiler warning about incomplete switch
    }
#ifdef __ANDROID__
    ចំនួនគត់ android_logLevel = ANDROID_LOG_INFO;
    switch (logLevel)
    {
    pak_sa LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; bha_nga;
    pak_sa LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; bha_nga;
    pak_sa LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; bha_nga;
    pak_sa LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; bha_nga;
    pak_sa LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; bha_nga;
    pak_sa LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; bha_nga;
    default:
        bha_nga;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_VERSION, "%s", ss.str().c_str());
#endif
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::cout;
    (*out) << ss.str();
    ប្រសិនបើ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

static const var_na* stripSourceFilePathPrefix(const var_na* file)
{
    CV_Assert(file);
    const var_na* pos = file;
    const var_na* strip_pos = NULL;
    var_na ch = 0;
    ខណៈពេល ((ch = pos[0]) != 0)
    {
        ++pos;
        ប្រសិនបើ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ប្រសិនបើ (strip_pos == NULL || strip_pos == pos/*eos*/)
        ត្រឡប់ file;
    ត្រឡប់ strip_pos;
}

void writeLogMessageEx(LogLevel logLevel, const var_na* tag, const var_na* file, ចំនួនគត់ line, const var_na* func, const var_na* message)
{
    WriteLogMessageExFuncType userFunc = stc_userWriteLogMessageExFunc.load();
    ប្រសិនបើ (userFunc && userFunc != writeLogMessageEx)
    {
        (*userFunc)(logLevel, tag, file, line, func, message);
        ត្រឡប់;
    }

    std::ostringstream strm;
    ប្រសិនបើ (tag)
    {
        strm << tag << ' ';
    }
    ប្រសិនបើ (file)
    {
        strm << stripSourceFilePathPrefix(file);
        ប្រសិនបើ (line > 0)
        {
            strm << ':' << line;
        }
        strm << ' ';
    }
    ប្រសិនបើ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    writeLogMessage(logLevel, strm.str().c_str());
}

void replaceWriteLogMessage(WriteLogMessageFuncType f)
{
    ប្រសិនបើ (f == writeLogMessage)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageFunc.store(f);
}

void replaceWriteLogMessageEx(WriteLogMessageExFuncType f)
{
    ប្រសិនបើ (f == writeLogMessageEx)
    {
        f = nullptr;
    }
    stc_userWriteLogMessageExFunc.store(f);
}

} // namespace

}}} // namespace
