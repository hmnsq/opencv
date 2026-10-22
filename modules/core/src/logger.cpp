// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/logger.hpp>
#شامل "utils/logtagmanager.hpp"
#شامل "utils/logtagconfigparser.hpp"

#شامل <sstream>
#شامل <iostream>
#شامل <fstream>
#شامل <atomic>

#اگر_تعریف_شدہ __ANDROID__
# include <android/log.h>
#اگر_ختم

نامخلا cv {
نامخلا utils {
نامخلا logging {

نامخلا internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
ساخت GlobalLoggingInitStruct
{
عوامی:
#اگر defined NDEBUG
    ساکن ثابت منطقی m_isDebugBuild = جھوٹ;
#ورنہ
    ساکن ثابت منطقی m_isDebugBuild = سچ;
#اگر_ختم

عوامی:
    ساکن LogLevel m_defaultUnconfiguredGlobalLevel;

عوامی:
    Log_لیبل_Manager log_لیبل_Manager;

    GlobalLoggingInitStruct()
        : log_لیبل_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (خالی)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

نجی:
    خالی applyConfigString()
    {
        log_لیبل_Manager.سیٹ_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    خالی handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        اگر (m_isDebugBuild)
        {
            ثابت خودکار& parser = log_لیبل_Manager.getConfigParser();
            اگر (parser.hasMalformed())
            {
                ثابت خودکار& malformedList = parser.getMalformed();
                برائے (ثابت خودکار& malformed : malformedList)
                {
                    std::خارج << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::خارج.flush();
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
ساکن GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, نیا GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
ساخت GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (خالی)get_Global_Log_لیبل();  // complete initialization of logger structures
    }
};

ساکن GlobalLoggingInitCall globalLoggingInitCall;

ساکن Log_لیبل_Manager& get_Log_لیبل_Manager()
{
    ساکن Log_لیبل_Manager& log_لیبل_Manager_Instance = getGlobalLoggingInitStruct().log_لیبل_Manager;
    واپس log_لیبل_Manager_Instance;
}

ساکن LogLevel& getLogLevelVariable()
{
    ساکن LogLevel& refGlobalLogLevel = get_Global_Log_لیبل()->level;
    واپس refGlobalLogLevel;
}

Log_لیبل* get_Global_Log_لیبل()
{
    ساکن Log_لیبل* global_Log_لیبل_Ptr = getGlobalLoggingInitStruct().log_لیبل_Manager.get("global");
    واپس global_Log_لیبل_Ptr;
}

} // namespace

خالی رجسٹر_Log_لیبل(Log_لیبل* plogtag)
{
    اگر (!plogtag || !plogtag->name)
    {
        واپس;
    }
    internal::get_Log_لیبل_Manager().assign(plogtag->name, plogtag);
}

خالی سیٹ_Log_لیبل_Level(ثابت حرف* لیبل, LogLevel level)
{
    اگر (!لیبل)
    {
        واپس;
    }
    internal::get_Log_لیبل_Manager().سیٹ_Level_By_بھرا_Name(std::string(لیبل), level);
}

LogLevel get_Log_لیبل_Level(ثابت حرف* لیبل)
{
    اگر (!لیبل)
    {
        واپس getLogLevel();
    }
    ثابت Log_لیبل* ptr = internal::get_Log_لیبل_Manager().get(std::string(لیبل));
    اگر (!ptr)
    {
        واپس getLogLevel();
    }
    واپس ptr->level;
}

LogLevel سیٹ_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ثابت LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    واپس old;
}

LogLevel getLogLevel()
{
    واپس internal::getLogLevelVariable();
}

نامخلا internal {

نامخلا //unnamed
{
    std::atomic<Write_Log_پیغام_Func_Type> stc_user_Write_Log_پیغام_Func{};
    std::atomic<Write_Log_پیغام_Ex_Func_Type> stc_user_Write_Log_پیغام_Ex_Func{};
} //unnamed

ساکن صحیح get_Show_Timestamp_منوال()
{
    ساکن منطقی param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", سچ);
    ساکن منطقی param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", جھوٹ);
    واپس (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

خالی write_Log_پیغام(LogLevel logLevel, ثابت حرف* message)
{
    Write_Log_پیغام_Func_Type userFunc = stc_user_Write_Log_پیغام_Func.load();
    اگر (userFunc && userFunc != write_Log_پیغام)
    {
        (*userFunc)(logLevel, message);
        واپس;
    }

    ثابت صحیح threadID = cv::utils::getThreadID();

    std::string پیغام_id;
    انتخاب (get_Show_Timestamp_منوال())
    {
        حالت 1: پیغام_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); توڑو;
        حالت 1+2: پیغام_id = cv::format("%d@%llu", threadID, (طویل طویل غیرعلامتی صحیح)getTimestampNS()); توڑو;
        طےشدہ: پیغام_id = cv::format("%d", threadID); توڑو;
    }

    std::ostringstream ss;
    انتخاب (logLevel)
    {
    حالت LOG_LEVEL_FATAL:   ss << "[FATAL:" << پیغام_id << "] " << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_ERROR:   ss << "[ERROR:" << پیغام_id << "] " << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_WARNING: ss << "[ WARN:" << پیغام_id << "] " << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_INFO:    ss << "[ INFO:" << پیغام_id << "] " << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << پیغام_id << "] " << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_VERBOSE: ss << message << std::سطر_ختم; توڑو;
    حالت LOG_LEVEL_SILENT: واپس;  // avoid compiler warning about incomplete switch
    حالت ENUM_LOG_LEVEL_FORCE_INT: واپس;  // avoid compiler warning about incomplete switch
    }
#اگر_تعریف_شدہ __ANDROID__
    صحیح android_logLevel = ANDROID_LOG_INFO;
    انتخاب (logLevel)
    {
    حالت LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; توڑو;
    حالت LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; توڑو;
    حالت LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; توڑو;
    حالت LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; توڑو;
    حالت LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; توڑو;
    حالت LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; توڑو;
    طےشدہ:
        توڑو;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_نسخہ, "%s", ss.str().c_str());
#اگر_ختم
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::خارج;
    (*out) << ss.str();
    اگر (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

ساکن ثابت حرف* strip_ماخذ_File_راستہ_سابقہ(ثابت حرف* file)
{
    CV_Assert(file);
    ثابت حرف* pos = file;
    ثابت حرف* strip_pos = تہی;
    حرف ch = 0;
    جب ((ch = pos[0]) != 0)
    {
        ++pos;
        اگر (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    اگر (strip_pos == تہی || strip_pos == pos/*eos*/)
        واپس file;
    واپس strip_pos;
}

خالی write_Log_پیغام_Ex(LogLevel logLevel, ثابت حرف* لیبل, ثابت حرف* file, صحیح سطر, ثابت حرف* func, ثابت حرف* message)
{
    Write_Log_پیغام_Ex_Func_Type userFunc = stc_user_Write_Log_پیغام_Ex_Func.load();
    اگر (userFunc && userFunc != write_Log_پیغام_Ex)
    {
        (*userFunc)(logLevel, لیبل, file, سطر, func, message);
        واپس;
    }

    std::ostringstream strm;
    اگر (لیبل)
    {
        strm << لیبل << ' ';
    }
    اگر (file)
    {
        strm << strip_ماخذ_File_راستہ_سابقہ(file);
        اگر (سطر > 0)
        {
            strm << ':' << سطر;
        }
        strm << ' ';
    }
    اگر (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_پیغام(logLevel, strm.str().c_str());
}

خالی replace_Write_Log_پیغام(Write_Log_پیغام_Func_Type f)
{
    اگر (f == write_Log_پیغام)
    {
        f = خالی_اشاریہ;
    }
    stc_user_Write_Log_پیغام_Func.store(f);
}

خالی replace_Write_Log_پیغام_Ex(Write_Log_پیغام_Ex_Func_Type f)
{
    اگر (f == write_Log_پیغام_Ex)
    {
        f = خالی_اشاریہ;
    }
    stc_user_Write_Log_پیغام_Ex_Func.store(f);
}

} // namespace

}}} // namespace
