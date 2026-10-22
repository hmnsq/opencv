// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/logtagmanager.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/logtagconfigparser.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <fstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
# include <android/log.h>
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ logging {

ਨਾਮਾਕਾਸ਼ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ਬਣਤਰ GlobalLoggingInitStruct
{
ਜਨਤਕ:
#ਜੇ defined NDEBUG
    ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ m_isDebugBuild = ਝੂਠ;
#ਨਹੀਂਤਾਂ
    ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ m_isDebugBuild = ਸੱਚ;
#ਜੇ_ਅੰਤ

ਜਨਤਕ:
    ਸਥਿਰ LogLevel m_defaultUnconfiguredGlobalLevel;

ਜਨਤਕ:
    Log_ਟੈਗ_Manager log_ਟੈਗ_Manager;

    GlobalLoggingInitStruct()
        : log_ਟੈਗ_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (ਖਾਲੀ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

ਨਿਜੀ:
    ਖਾਲੀ applyConfigString()
    {
        log_ਟੈਗ_Manager.ਸੈੱਟ_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    ਖਾਲੀ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ਜੇ (m_isDebugBuild)
        {
            ਅਚਲ ਸਵੈਚਾਲਿਤ& parser = log_ਟੈਗ_Manager.getConfigParser();
            ਜੇ (parser.hasMalformed())
            {
                ਅਚਲ ਸਵੈਚਾਲਿਤ& malformedList = parser.getMalformed();
                ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& malformed : malformedList)
                {
                    std::ਬਾਹਰ << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::ਬਾਹਰ.flush();
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
ਸਥਿਰ GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, ਨਵਾਂ GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ਬਣਤਰ GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (ਖਾਲੀ)get_Global_Log_ਟੈਗ();  // complete initialization of logger structures
    }
};

ਸਥਿਰ GlobalLoggingInitCall globalLoggingInitCall;

ਸਥਿਰ Log_ਟੈਗ_Manager& get_Log_ਟੈਗ_Manager()
{
    ਸਥਿਰ Log_ਟੈਗ_Manager& log_ਟੈਗ_Manager_Instance = getGlobalLoggingInitStruct().log_ਟੈਗ_Manager;
    ਵਾਪਸ log_ਟੈਗ_Manager_Instance;
}

ਸਥਿਰ LogLevel& getLogLevelVariable()
{
    ਸਥਿਰ LogLevel& refGlobalLogLevel = get_Global_Log_ਟੈਗ()->level;
    ਵਾਪਸ refGlobalLogLevel;
}

Log_ਟੈਗ* get_Global_Log_ਟੈਗ()
{
    ਸਥਿਰ Log_ਟੈਗ* global_Log_ਟੈਗ_Ptr = getGlobalLoggingInitStruct().log_ਟੈਗ_Manager.get("global");
    ਵਾਪਸ global_Log_ਟੈਗ_Ptr;
}

} // namespace

ਖਾਲੀ ਰਜਿਸਟਰ_Log_ਟੈਗ(Log_ਟੈਗ* plogtag)
{
    ਜੇ (!plogtag || !plogtag->name)
    {
        ਵਾਪਸ;
    }
    internal::get_Log_ਟੈਗ_Manager().assign(plogtag->name, plogtag);
}

ਖਾਲੀ ਸੈੱਟ_Log_ਟੈਗ_Level(ਅਚਲ ਅੱਖਰ* ਟੈਗ, LogLevel level)
{
    ਜੇ (!ਟੈਗ)
    {
        ਵਾਪਸ;
    }
    internal::get_Log_ਟੈਗ_Manager().ਸੈੱਟ_Level_By_ਭਰਿਆ_Name(std::string(ਟੈਗ), level);
}

LogLevel get_Log_ਟੈਗ_Level(ਅਚਲ ਅੱਖਰ* ਟੈਗ)
{
    ਜੇ (!ਟੈਗ)
    {
        ਵਾਪਸ getLogLevel();
    }
    ਅਚਲ Log_ਟੈਗ* ptr = internal::get_Log_ਟੈਗ_Manager().get(std::string(ਟੈਗ));
    ਜੇ (!ptr)
    {
        ਵਾਪਸ getLogLevel();
    }
    ਵਾਪਸ ptr->level;
}

LogLevel ਸੈੱਟ_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ਅਚਲ LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    ਵਾਪਸ old;
}

LogLevel getLogLevel()
{
    ਵਾਪਸ internal::getLogLevelVariable();
}

ਨਾਮਾਕਾਸ਼ internal {

ਨਾਮਾਕਾਸ਼ //unnamed
{
    std::atomic<Write_Log_ਸੁਨੇਹਾ_Func_Type> stc_user_Write_Log_ਸੁਨੇਹਾ_Func{};
    std::atomic<Write_Log_ਸੁਨੇਹਾ_Ex_Func_Type> stc_user_Write_Log_ਸੁਨੇਹਾ_Ex_Func{};
} //unnamed

ਸਥਿਰ ਪੂਰਨ get_Show_Timestamp_ਬਹੁਲਕ()
{
    ਸਥਿਰ ਤਾਰਕਿਕ param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", ਸੱਚ);
    ਸਥਿਰ ਤਾਰਕਿਕ param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", ਝੂਠ);
    ਵਾਪਸ (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

ਖਾਲੀ write_Log_ਸੁਨੇਹਾ(LogLevel logLevel, ਅਚਲ ਅੱਖਰ* message)
{
    Write_Log_ਸੁਨੇਹਾ_Func_Type userFunc = stc_user_Write_Log_ਸੁਨੇਹਾ_Func.load();
    ਜੇ (userFunc && userFunc != write_Log_ਸੁਨੇਹਾ)
    {
        (*userFunc)(logLevel, message);
        ਵਾਪਸ;
    }

    ਅਚਲ ਪੂਰਨ threadID = cv::utils::getThreadID();

    std::string ਸੁਨੇਹਾ_id;
    ਚੋਣ (get_Show_Timestamp_ਬਹੁਲਕ())
    {
        ਹਾਲਤ 1: ਸੁਨੇਹਾ_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ਰੋਕੋ;
        ਹਾਲਤ 1+2: ਸੁਨੇਹਾ_id = cv::format("%d@%llu", threadID, (ਲੰਮਾ ਲੰਮਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)getTimestampNS()); ਰੋਕੋ;
        ਮੂਲ: ਸੁਨੇਹਾ_id = cv::format("%d", threadID); ਰੋਕੋ;
    }

    std::ostringstream ss;
    ਚੋਣ (logLevel)
    {
    ਹਾਲਤ LOG_LEVEL_FATAL:   ss << "[FATAL:" << ਸੁਨੇਹਾ_id << "] " << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_ERROR:   ss << "[ERROR:" << ਸੁਨੇਹਾ_id << "] " << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_WARNING: ss << "[ WARN:" << ਸੁਨੇਹਾ_id << "] " << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_INFO:    ss << "[ INFO:" << ਸੁਨੇਹਾ_id << "] " << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << ਸੁਨੇਹਾ_id << "] " << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_VERBOSE: ss << message << std::ਸਤਰ_ਅੰਤ; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_SILENT: ਵਾਪਸ;  // avoid compiler warning about incomplete switch
    ਹਾਲਤ ENUM_LOG_LEVEL_FORCE_INT: ਵਾਪਸ;  // avoid compiler warning about incomplete switch
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    ਪੂਰਨ android_logLevel = ANDROID_LOG_INFO;
    ਚੋਣ (logLevel)
    {
    ਹਾਲਤ LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ਰੋਕੋ;
    ਹਾਲਤ LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ਰੋਕੋ;
    ਮੂਲ:
        ਰੋਕੋ;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_ਸੰਸਕਰਣ, "%s", ss.str().c_str());
#ਜੇ_ਅੰਤ
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::ਬਾਹਰ;
    (*out) << ss.str();
    ਜੇ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* strip_ਸਰੋਤ_File_ਰਸਤਾ_ਅਗੇਤਰ(ਅਚਲ ਅੱਖਰ* file)
{
    CV_Assert(file);
    ਅਚਲ ਅੱਖਰ* pos = file;
    ਅਚਲ ਅੱਖਰ* strip_pos = ਨਿਰਾ;
    ਅੱਖਰ ch = 0;
    ਜਦੋਂ ((ch = pos[0]) != 0)
    {
        ++pos;
        ਜੇ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ਜੇ (strip_pos == ਨਿਰਾ || strip_pos == pos/*eos*/)
        ਵਾਪਸ file;
    ਵਾਪਸ strip_pos;
}

ਖਾਲੀ write_Log_ਸੁਨੇਹਾ_Ex(LogLevel logLevel, ਅਚਲ ਅੱਖਰ* ਟੈਗ, ਅਚਲ ਅੱਖਰ* file, ਪੂਰਨ ਸਤਰ, ਅਚਲ ਅੱਖਰ* func, ਅਚਲ ਅੱਖਰ* message)
{
    Write_Log_ਸੁਨੇਹਾ_Ex_Func_Type userFunc = stc_user_Write_Log_ਸੁਨੇਹਾ_Ex_Func.load();
    ਜੇ (userFunc && userFunc != write_Log_ਸੁਨੇਹਾ_Ex)
    {
        (*userFunc)(logLevel, ਟੈਗ, file, ਸਤਰ, func, message);
        ਵਾਪਸ;
    }

    std::ostringstream strm;
    ਜੇ (ਟੈਗ)
    {
        strm << ਟੈਗ << ' ';
    }
    ਜੇ (file)
    {
        strm << strip_ਸਰੋਤ_File_ਰਸਤਾ_ਅਗੇਤਰ(file);
        ਜੇ (ਸਤਰ > 0)
        {
            strm << ':' << ਸਤਰ;
        }
        strm << ' ';
    }
    ਜੇ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_ਸੁਨੇਹਾ(logLevel, strm.str().c_str());
}

ਖਾਲੀ replace_Write_Log_ਸੁਨੇਹਾ(Write_Log_ਸੁਨੇਹਾ_Func_Type f)
{
    ਜੇ (f == write_Log_ਸੁਨੇਹਾ)
    {
        f = ਖਾਲੀ_ਸੂਚਕ;
    }
    stc_user_Write_Log_ਸੁਨੇਹਾ_Func.store(f);
}

ਖਾਲੀ replace_Write_Log_ਸੁਨੇਹਾ_Ex(Write_Log_ਸੁਨੇਹਾ_Ex_Func_Type f)
{
    ਜੇ (f == write_Log_ਸੁਨੇਹਾ_Ex)
    {
        f = ਖਾਲੀ_ਸੂਚਕ;
    }
    stc_user_Write_Log_ਸੁਨੇਹਾ_Ex_Func.store(f);
}

} // namespace

}}} // namespace
