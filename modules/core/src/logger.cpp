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

#جيڪڏهن_وصف_ٿيل __ANDROID__
# include <android/log.h>
#جيڪڏهن_پورو

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
    ساكن ثابت منطقي m_isDebugBuild = سچ;
#جيڪڏهن_پورو

عام:
    ساكن LogLevel m_defaultUnconfiguredGlobalLevel;

عام:
    Log_ٽيگ_Manager log_ٽيگ_Manager;

    GlobalLoggingInitStruct()
        : log_ٽيگ_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (فراغ)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

خاص:
    فراغ applyConfigString()
    {
        log_ٽيگ_Manager.سيٽ_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    فراغ handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        إذا (m_isDebugBuild)
        {
            ثابت خودڪار& parser = log_ٽيگ_Manager.getConfigParser();
            إذا (parser.hasMalformed())
            {
                ثابت خودڪار& malformedList = parser.getMalformed();
                لكل (ثابت خودڪار& malformed : malformedList)
                {
                    std::ٻاهر << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::ٻاهر.flush();
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
        (فراغ)get_Global_Log_ٽيگ();  // complete initialization of logger structures
    }
};

ساكن GlobalLoggingInitCall globalLoggingInitCall;

ساكن Log_ٽيگ_Manager& get_Log_ٽيگ_Manager()
{
    ساكن Log_ٽيگ_Manager& log_ٽيگ_Manager_Instance = getGlobalLoggingInitStruct().log_ٽيگ_Manager;
    أرجع log_ٽيگ_Manager_Instance;
}

ساكن LogLevel& getLogLevelVariable()
{
    ساكن LogLevel& refGlobalLogLevel = get_Global_Log_ٽيگ()->level;
    أرجع refGlobalLogLevel;
}

Log_ٽيگ* get_Global_Log_ٽيگ()
{
    ساكن Log_ٽيگ* global_Log_ٽيگ_Ptr = getGlobalLoggingInitStruct().log_ٽيگ_Manager.get("global");
    أرجع global_Log_ٽيگ_Ptr;
}

} // namespace

فراغ رجسٽر_Log_ٽيگ(Log_ٽيگ* plogtag)
{
    إذا (!plogtag || !plogtag->name)
    {
        أرجع;
    }
    internal::get_Log_ٽيگ_Manager().assign(plogtag->name, plogtag);
}

فراغ سيٽ_Log_ٽيگ_Level(ثابت حرف* ٽيگ, LogLevel level)
{
    إذا (!ٽيگ)
    {
        أرجع;
    }
    internal::get_Log_ٽيگ_Manager().سيٽ_Level_By_ڀريل_Name(std::string(ٽيگ), level);
}

LogLevel get_Log_ٽيگ_Level(ثابت حرف* ٽيگ)
{
    إذا (!ٽيگ)
    {
        أرجع getLogLevel();
    }
    ثابت Log_ٽيگ* ptr = internal::get_Log_ٽيگ_Manager().get(std::string(ٽيگ));
    إذا (!ptr)
    {
        أرجع getLogLevel();
    }
    أرجع ptr->level;
}

LogLevel سيٽ_Log_Level(LogLevel logLevel)
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
    std::atomic<Write_Log_پيغام_Func_Type> stc_user_Write_Log_پيغام_Func{};
    std::atomic<Write_Log_پيغام_Ex_Func_Type> stc_user_Write_Log_پيغام_Ex_Func{};
} //unnamed

ساكن صحيح get_Show_Timestamp_گهڻائي()
{
    ساكن منطقي param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", سچ);
    ساكن منطقي param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", خطأ);
    أرجع (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

فراغ write_Log_پيغام(LogLevel logLevel, ثابت حرف* message)
{
    Write_Log_پيغام_Func_Type userFunc = stc_user_Write_Log_پيغام_Func.load();
    إذا (userFunc && userFunc != write_Log_پيغام)
    {
        (*userFunc)(logLevel, message);
        أرجع;
    }

    ثابت صحيح threadID = cv::utils::getThreadID();

    std::string پيغام_id;
    بدل (get_Show_Timestamp_گهڻائي())
    {
        حالة 1: پيغام_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); اكسر;
        حالة 1+2: پيغام_id = cv::format("%d@%llu", threadID, (ڊگهو ڊگهو بنانشان صحيح)getTimestampNS()); اكسر;
        افتراضي: پيغام_id = cv::format("%d", threadID); اكسر;
    }

    std::ostringstream ss;
    بدل (logLevel)
    {
    حالة LOG_LEVEL_FATAL:   ss << "[FATAL:" << پيغام_id << "] " << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_ERROR:   ss << "[ERROR:" << پيغام_id << "] " << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_WARNING: ss << "[ WARN:" << پيغام_id << "] " << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_INFO:    ss << "[ INFO:" << پيغام_id << "] " << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << پيغام_id << "] " << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_VERBOSE: ss << message << std::سٽ_پڄاڻي; اكسر;
    حالة LOG_LEVEL_SILENT: أرجع;  // avoid compiler warning about incomplete switch
    حالة ENUM_LOG_LEVEL_FORCE_INT: أرجع;  // avoid compiler warning about incomplete switch
    }
#جيڪڏهن_وصف_ٿيل __ANDROID__
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
    __android_log_print(android_logLevel, "OpenCV/" CV_نسخو, "%s", ss.str().c_str());
#جيڪڏهن_پورو
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::ٻاهر;
    (*out) << ss.str();
    إذا (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ساكن ثابت حرف* strip_ذريعو_File_رستو_اڳياڙي(ثابت حرف* file)
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

فراغ write_Log_پيغام_Ex(LogLevel logLevel, ثابت حرف* ٽيگ, ثابت حرف* file, صحيح سٽ, ثابت حرف* func, ثابت حرف* message)
{
    Write_Log_پيغام_Ex_Func_Type userFunc = stc_user_Write_Log_پيغام_Ex_Func.load();
    إذا (userFunc && userFunc != write_Log_پيغام_Ex)
    {
        (*userFunc)(logLevel, ٽيگ, file, سٽ, func, message);
        أرجع;
    }

    std::ostringstream strm;
    إذا (ٽيگ)
    {
        strm << ٽيگ << ' ';
    }
    إذا (file)
    {
        strm << strip_ذريعو_File_رستو_اڳياڙي(file);
        إذا (سٽ > 0)
        {
            strm << ':' << سٽ;
        }
        strm << ' ';
    }
    إذا (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_پيغام(logLevel, strm.str().c_str());
}

فراغ replace_Write_Log_پيغام(Write_Log_پيغام_Func_Type f)
{
    إذا (f == write_Log_پيغام)
    {
        f = خالي_اشارو;
    }
    stc_user_Write_Log_پيغام_Func.store(f);
}

فراغ replace_Write_Log_پيغام_Ex(Write_Log_پيغام_Ex_Func_Type f)
{
    إذا (f == write_Log_پيغام_Ex)
    {
        f = خالي_اشارو;
    }
    stc_user_Write_Log_پيغام_Ex_Func.store(f);
}

} // namespace

}}} // namespace
