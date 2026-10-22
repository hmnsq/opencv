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

#اگر_تعریف_شده __ANDROID__
# include <android/log.h>
#پایان_اگر

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
    ساكن ثابت منطقي m_isDebugBuild = درست;
#پایان_اگر

عام:
    ساكن LogLevel m_defaultUnconfiguredGlobalLevel;

عام:
    Log_برچسب_Manager log_برچسب_Manager;

    GlobalLoggingInitStruct()
        : log_برچسب_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (فراغ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

خاص:
    فراغ applyConfigString()
    {
        log_برچسب_Manager.مجموعه_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    فراغ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        إذا (m_isDebugBuild)
        {
            ثابت خودکار& parser = log_برچسب_Manager.getConfigParser();
            إذا (parser.hasMalformed())
            {
                ثابت خودکار& malformedList = parser.getMalformed();
                لكل (ثابت خودکار& malformed : malformedList)
                {
                    std::خروجی << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::خروجی.flush();
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
        (فراغ)get_Global_Log_برچسب();  // complete initialization of logger structures
    }
};

ساكن GlobalLoggingInitCall globalLoggingInitCall;

ساكن Log_برچسب_Manager& get_Log_برچسب_Manager()
{
    ساكن Log_برچسب_Manager& log_برچسب_Manager_Instance = getGlobalLoggingInitStruct().log_برچسب_Manager;
    أرجع log_برچسب_Manager_Instance;
}

ساكن LogLevel& getLogLevelVariable()
{
    ساكن LogLevel& refGlobalLogLevel = get_Global_Log_برچسب()->level;
    أرجع refGlobalLogLevel;
}

Log_برچسب* get_Global_Log_برچسب()
{
    ساكن Log_برچسب* global_Log_برچسب_Ptr = getGlobalLoggingInitStruct().log_برچسب_Manager.get("global");
    أرجع global_Log_برچسب_Ptr;
}

} // namespace

فراغ ثبات_Log_برچسب(Log_برچسب* plogtag)
{
    إذا (!plogtag || !plogtag->name)
    {
        أرجع;
    }
    internal::get_Log_برچسب_Manager().assign(plogtag->name, plogtag);
}

فراغ مجموعه_Log_برچسب_Level(ثابت حرف* برچسب, LogLevel level)
{
    إذا (!برچسب)
    {
        أرجع;
    }
    internal::get_Log_برچسب_Manager().مجموعه_Level_By_پر_Name(std::string(برچسب), level);
}

LogLevel get_Log_برچسب_Level(ثابت حرف* برچسب)
{
    إذا (!برچسب)
    {
        أرجع getLogLevel();
    }
    ثابت Log_برچسب* ptr = internal::get_Log_برچسب_Manager().get(std::string(برچسب));
    إذا (!ptr)
    {
        أرجع getLogLevel();
    }
    أرجع ptr->level;
}

LogLevel مجموعه_Log_Level(LogLevel logLevel)
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
    std::atomic<Write_Log_پیام_Func_Type> stc_user_Write_Log_پیام_Func{};
    std::atomic<Write_Log_پیام_Ex_Func_Type> stc_user_Write_Log_پیام_Ex_Func{};
} //unnamed

ساكن صحيح get_Show_Timestamp_نما()
{
    ساكن منطقي param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", درست);
    ساكن منطقي param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", خطأ);
    أرجع (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

فراغ write_Log_پیام(LogLevel logLevel, ثابت حرف* message)
{
    Write_Log_پیام_Func_Type userFunc = stc_user_Write_Log_پیام_Func.load();
    إذا (userFunc && userFunc != write_Log_پیام)
    {
        (*userFunc)(logLevel, message);
        أرجع;
    }

    ثابت صحيح threadID = cv::utils::getThreadID();

    std::string پیام_id;
    بدل (get_Show_Timestamp_نما())
    {
        حالة 1: پیام_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); اكسر;
        حالة 1+2: پیام_id = cv::format("%d@%llu", threadID, (دراز دراز بیعلامت صحيح)getTimestampNS()); اكسر;
        افتراضي: پیام_id = cv::format("%d", threadID); اكسر;
    }

    std::ostringstream ss;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   ss << "[FATAL:" << پیام_id << "] " << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_ERROR:   ss << "[ERROR:" << پیام_id << "] " << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_WARNING: ss << "[ WARN:" << پیام_id << "] " << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_INFO:    ss << "[ INFO:" << پیام_id << "] " << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << پیام_id << "] " << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_VERBOSE: ss << message << std::پایان_سطر; اكسر;
    حالة LOG_LEVEL_SILENT: أرجع;  // avoid compiler warning about incomplete switch
    حالة ENUM_LOG_LEVEL_FORCE_INT: أرجع;  // avoid compiler warning about incomplete switch
    }
#اگر_تعریف_شده __ANDROID__
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
#پایان_اگر
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::خروجی;
    (*out) << ss.str();
    إذا (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ساكن ثابت حرف* strip_منبع_File_مسیر_پیشوند(ثابت حرف* file)
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

فراغ write_Log_پیام_Ex(LogLevel logLevel, ثابت حرف* برچسب, ثابت حرف* file, صحيح سطر, ثابت حرف* func, ثابت حرف* message)
{
    Write_Log_پیام_Ex_Func_Type userFunc = stc_user_Write_Log_پیام_Ex_Func.load();
    إذا (userFunc && userFunc != write_Log_پیام_Ex)
    {
        (*userFunc)(logLevel, برچسب, file, سطر, func, message);
        أرجع;
    }

    std::ostringstream strm;
    إذا (برچسب)
    {
        strm << برچسب << ' ';
    }
    إذا (file)
    {
        strm << strip_منبع_File_مسیر_پیشوند(file);
        إذا (سطر > 0)
        {
            strm << ':' << سطر;
        }
        strm << ' ';
    }
    إذا (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_پیام(logLevel, strm.str().c_str());
}

فراغ replace_Write_Log_پیام(Write_Log_پیام_Func_Type f)
{
    إذا (f == write_Log_پیام)
    {
        f = اشاره_تهی;
    }
    stc_user_Write_Log_پیام_Func.store(f);
}

فراغ replace_Write_Log_پیام_Ex(Write_Log_پیام_Ex_Func_Type f)
{
    إذا (f == write_Log_پیام_Ex)
    {
        f = اشاره_تهی;
    }
    stc_user_Write_Log_پیام_Ex_Func.store(f);
}

} // namespace

}}} // namespace
