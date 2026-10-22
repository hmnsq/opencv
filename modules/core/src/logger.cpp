// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/logger.hpp>
#ضمن "utils/logtagmanager.hpp"
#ضمن "utils/logtagconfigparser.hpp"

#ضمن <sstream>
#ضمن <iostream>
#ضمن <fstream>
#ضمن <atomic>

#که_تعریف_شوی __ANDROID__
# include <android/log.h>
#د_که_پای

نطاق cv {
نطاق utils {
نطاق logging {

نطاق internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
بنية GlobalLoggingInitStruct
{
عام:
#إذا defined NDEBUG
    ساكن ثابت منطقي m_isDebugBuild = خطأ;
#وإلا
    ساكن ثابت منطقي m_isDebugBuild = ریښتیا;
#د_که_پای

عام:
    ساكن LogLevel m_defaultUnconfiguredGlobalLevel;

عام:
    Log_نښان_Manager log_نښان_Manager;

    GlobalLoggingInitStruct()
        : log_نښان_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (فراغ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

خاص:
    فراغ applyConfigString()
    {
        log_نښان_Manager.سېټ_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    فراغ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        إذا (m_isDebugBuild)
        {
            ثابت خپلکاری& parser = log_نښان_Manager.getConfigParser();
            إذا (parser.hasMalformed())
            {
                ثابت خپلکاری& malformedList = parser.getMalformed();
                لكل (ثابت خپلکاری& malformed : malformedList)
                {
                    std::وتنه << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::وتنه.flush();
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
ساكن GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, جديد GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
بنية GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (فراغ)get_Global_Log_نښان();  // complete initialization of logger structures
    }
};

ساكن GlobalLoggingInitCall globalLoggingInitCall;

ساكن Log_نښان_Manager& get_Log_نښان_Manager()
{
    ساكن Log_نښان_Manager& log_نښان_Manager_Instance = getGlobalLoggingInitStruct().log_نښان_Manager;
    أرجع log_نښان_Manager_Instance;
}

ساكن LogLevel& getLogLevelVariable()
{
    ساكن LogLevel& refGlobalLogLevel = get_Global_Log_نښان()->level;
    أرجع refGlobalLogLevel;
}

Log_نښان* get_Global_Log_نښان()
{
    ساكن Log_نښان* global_Log_نښان_Ptr = getGlobalLoggingInitStruct().log_نښان_Manager.get("global");
    أرجع global_Log_نښان_Ptr;
}

} // namespace

فراغ ثبتوونکی_Log_نښان(Log_نښان* plogtag)
{
    إذا (!plogtag || !plogtag->name)
    {
        أرجع;
    }
    internal::get_Log_نښان_Manager().assign(plogtag->name, plogtag);
}

فراغ سېټ_Log_نښان_Level(ثابت حرف* نښان, LogLevel level)
{
    إذا (!نښان)
    {
        أرجع;
    }
    internal::get_Log_نښان_Manager().سېټ_Level_By_ډک_Name(std::string(نښان), level);
}

LogLevel get_Log_نښان_Level(ثابت حرف* نښان)
{
    إذا (!نښان)
    {
        أرجع getLogLevel();
    }
    ثابت Log_نښان* ptr = internal::get_Log_نښان_Manager().get(std::string(نښان));
    إذا (!ptr)
    {
        أرجع getLogLevel();
    }
    أرجع ptr->level;
}

LogLevel سېټ_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ثابت LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    أرجع old;
}

LogLevel getLogLevel()
{
    أرجع internal::getLogLevelVariable();
}

نطاق internal {

نطاق //unnamed
{
    std::atomic<Write_Log_پیغام_Func_Type> stc_user_Write_Log_پیغام_Func{};
    std::atomic<Write_Log_پیغام_Ex_Func_Type> stc_user_Write_Log_پیغام_Ex_Func{};
} //unnamed

ساكن صحيح get_Show_Timestamp_ډېرپېښ()
{
    ساكن منطقي param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", ریښتیا);
    ساكن منطقي param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", خطأ);
    أرجع (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

فراغ write_Log_پیغام(LogLevel logLevel, ثابت حرف* message)
{
    Write_Log_پیغام_Func_Type userFunc = stc_user_Write_Log_پیغام_Func.load();
    إذا (userFunc && userFunc != write_Log_پیغام)
    {
        (*userFunc)(logLevel, message);
        أرجع;
    }

    ثابت صحيح threadID = cv::utils::getThreadID();

    std::string پیغام_id;
    بدل (get_Show_Timestamp_ډېرپېښ())
    {
        حالة 1: پیغام_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); اكسر;
        حالة 1+2: پیغام_id = cv::format("%d@%llu", threadID, (اوږد اوږد بینښه صحيح)getTimestampNS()); اكسر;
        افتراضي: پیغام_id = cv::format("%d", threadID); اكسر;
    }

    std::ostringstream ss;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   ss << "[FATAL:" << پیغام_id << "] " << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_ERROR:   ss << "[ERROR:" << پیغام_id << "] " << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_WARNING: ss << "[ WARN:" << پیغام_id << "] " << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_INFO:    ss << "[ INFO:" << پیغام_id << "] " << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << پیغام_id << "] " << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_VERBOSE: ss << message << std::کرښه_پای; اكسر;
    حالة LOG_LEVEL_SILENT: أرجع;  // avoid compiler warning about incomplete switch
    حالة ENUM_LOG_LEVEL_FORCE_INT: أرجع;  // avoid compiler warning about incomplete switch
    }
#که_تعریف_شوی __ANDROID__
    صحيح android_logLevel = ANDROID_LOG_INFO;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; اكسر;
    حالة LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; اكسر;
    حالة LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; اكسر;
    حالة LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; اكسر;
    حالة LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; اكسر;
    حالة LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; اكسر;
    افتراضي:
        اكسر;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_نسخه, "%s", ss.str().c_str());
#د_که_پای
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::وتنه;
    (*out) << ss.str();
    إذا (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ساكن ثابت حرف* strip_سرچینه_File_لار_مختاړی(ثابت حرف* file)
{
    CV_Assert(file);
    ثابت حرف* pos = file;
    ثابت حرف* strip_pos = عدم;
    حرف ch = 0;
    طالما ((ch = pos[0]) != 0)
    {
        ++pos;
        إذا (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    إذا (strip_pos == عدم || strip_pos == pos/*eos*/)
        أرجع file;
    أرجع strip_pos;
}

فراغ write_Log_پیغام_Ex(LogLevel logLevel, ثابت حرف* نښان, ثابت حرف* file, صحيح کرښه, ثابت حرف* func, ثابت حرف* message)
{
    Write_Log_پیغام_Ex_Func_Type userFunc = stc_user_Write_Log_پیغام_Ex_Func.load();
    إذا (userFunc && userFunc != write_Log_پیغام_Ex)
    {
        (*userFunc)(logLevel, نښان, file, کرښه, func, message);
        أرجع;
    }

    std::ostringstream strm;
    إذا (نښان)
    {
        strm << نښان << ' ';
    }
    إذا (file)
    {
        strm << strip_سرچینه_File_لار_مختاړی(file);
        إذا (کرښه > 0)
        {
            strm << ':' << کرښه;
        }
        strm << ' ';
    }
    إذا (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_پیغام(logLevel, strm.str().c_str());
}

فراغ replace_Write_Log_پیغام(Write_Log_پیغام_Func_Type f)
{
    إذا (f == write_Log_پیغام)
    {
        f = تش_اشاره;
    }
    stc_user_Write_Log_پیغام_Func.store(f);
}

فراغ replace_Write_Log_پیغام_Ex(Write_Log_پیغام_Ex_Func_Type f)
{
    إذا (f == write_Log_پیغام_Ex)
    {
        f = تش_اشاره;
    }
    stc_user_Write_Log_پیغام_Ex_Func.store(f);
}

} // namespace

}}} // namespace
