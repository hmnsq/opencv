// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ <opencv2/core/utils/logger.hpp>
#అన్తర్భావ "utils/logtagmanager.hpp"
#అన్తర్భావ "utils/logtagconfigparser.hpp"

#అన్తర్భావ <sstream>
#అన్తర్భావ <iostream>
#అన్తర్భావ <fstream>
#అన్తర్భావ <atomic>

#నిర్వచితమైతే __ANDROID__
# include <android/log.h>
#అయితే_ముగింపు

నామాకాశ cv {
నామాకాశ utils {
నామాకాశ logging {

నామాకాశ internal
{

// Combining several things that require static dynamic initialization in a
// well-defined order into a struct.
//
నిర్మాణం GlobalLoggingInitStruct
{
బహిరంగ:
#ఒకవేళ defined NDEBUG
    స్థిర స్థిరరాశి తార్కిక m_isDebugBuild = అబద్ధం;
#లేకపోతే
    స్థిర స్థిరరాశి తార్కిక m_isDebugBuild = నిజం;
#అయితే_ముగింపు

బహిరంగ:
    స్థిర LogLevel m_defaultUnconfiguredGlobalLevel;

బహిరంగ:
    Log_ట్యాగ్_Manager log_ట్యాగ్_Manager;

    GlobalLoggingInitStruct()
        : log_ట్యాగ్_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (శూన్య)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

అంతరంగ:
    శూన్య applyConfigString()
    {
        log_ట్యాగ్_Manager.సమితి_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    శూన్య handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        ఒకవేళ (m_isDebugBuild)
        {
            స్థిరరాశి స్వయంచాలక& parser = log_ట్యాగ్_Manager.getConfigParser();
            ఒకవేళ (parser.hasMalformed())
            {
                స్థిరరాశి స్వయంచాలక& malformedList = parser.getMalformed();
                కొరకు (స్థిరరాశి స్వయంచాలక& malformed : malformedList)
                {
                    std::బయటకు << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::బయటకు.flush();
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
స్థిర GlobalLoggingInitStruct& getGlobalLoggingInitStruct()
{
    CV_SINGLETON_LAZY_INIT_REF(GlobalLoggingInitStruct, కొత్త GlobalLoggingInitStruct());
}

// To ensure that the combined struct defined above is initialized even
// if the initialization guard function wasn't called, a dummy static
// instance of a struct is defined below, which will call the
// initialization guard function.
//
నిర్మాణం GlobalLoggingInitCall
{
    GlobalLoggingInitCall()
    {
        getGlobalLoggingInitStruct();
        (శూన్య)get_Global_Log_ట్యాగ్();  // complete initialization of logger structures
    }
};

స్థిర GlobalLoggingInitCall globalLoggingInitCall;

స్థిర Log_ట్యాగ్_Manager& get_Log_ట్యాగ్_Manager()
{
    స్థిర Log_ట్యాగ్_Manager& log_ట్యాగ్_Manager_Instance = getGlobalLoggingInitStruct().log_ట్యాగ్_Manager;
    తిరిగి log_ట్యాగ్_Manager_Instance;
}

స్థిర LogLevel& getLogLevelVariable()
{
    స్థిర LogLevel& refGlobalLogLevel = get_Global_Log_ట్యాగ్()->level;
    తిరిగి refGlobalLogLevel;
}

Log_ట్యాగ్* get_Global_Log_ట్యాగ్()
{
    స్థిర Log_ట్యాగ్* global_Log_ట్యాగ్_Ptr = getGlobalLoggingInitStruct().log_ట్యాగ్_Manager.get("global");
    తిరిగి global_Log_ట్యాగ్_Ptr;
}

} // namespace

శూన్య రిజిస్టరు_Log_ట్యాగ్(Log_ట్యాగ్* plogtag)
{
    ఒకవేళ (!plogtag || !plogtag->name)
    {
        తిరిగి;
    }
    internal::get_Log_ట్యాగ్_Manager().assign(plogtag->name, plogtag);
}

శూన్య సమితి_Log_ట్యాగ్_Level(స్థిరరాశి అక్షరం* ట్యాగ్, LogLevel level)
{
    ఒకవేళ (!ట్యాగ్)
    {
        తిరిగి;
    }
    internal::get_Log_ట్యాగ్_Manager().సమితి_Level_By_నిండిన_Name(std::string(ట్యాగ్), level);
}

LogLevel get_Log_ట్యాగ్_Level(స్థిరరాశి అక్షరం* ట్యాగ్)
{
    ఒకవేళ (!ట్యాగ్)
    {
        తిరిగి getLogLevel();
    }
    స్థిరరాశి Log_ట్యాగ్* ptr = internal::get_Log_ట్యాగ్_Manager().get(std::string(ట్యాగ్));
    ఒకవేళ (!ptr)
    {
        తిరిగి getLogLevel();
    }
    తిరిగి ptr->level;
}

LogLevel సమితి_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    స్థిరరాశి LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    తిరిగి old;
}

LogLevel getLogLevel()
{
    తిరిగి internal::getLogLevelVariable();
}

నామాకాశ internal {

నామాకాశ //unnamed
{
    std::atomic<Write_Log_సందేశం_Func_Type> stc_user_Write_Log_సందేశం_Func{};
    std::atomic<Write_Log_సందేశం_Ex_Func_Type> stc_user_Write_Log_సందేశం_Ex_Func{};
} //unnamed

స్థిర పూర్ణాంకం get_Show_Timestamp_బహుళకం()
{
    స్థిర తార్కిక param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", నిజం);
    స్థిర తార్కిక param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", అబద్ధం);
    తిరిగి (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

శూన్య write_Log_సందేశం(LogLevel logLevel, స్థిరరాశి అక్షరం* message)
{
    Write_Log_సందేశం_Func_Type userFunc = stc_user_Write_Log_సందేశం_Func.load();
    ఒకవేళ (userFunc && userFunc != write_Log_సందేశం)
    {
        (*userFunc)(logLevel, message);
        తిరిగి;
    }

    స్థిరరాశి పూర్ణాంకం threadID = cv::utils::getThreadID();

    std::string సందేశం_id;
    ఎంపిక (get_Show_Timestamp_బహుళకం())
    {
        స్థితి 1: సందేశం_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); ఆపు;
        స్థితి 1+2: సందేశం_id = cv::format("%d@%llu", threadID, (దీర్ఘ దీర్ఘ గుర్తులేని పూర్ణాంకం)getTimestampNS()); ఆపు;
        అప్రమేయ: సందేశం_id = cv::format("%d", threadID); ఆపు;
    }

    std::ostringstream ss;
    ఎంపిక (logLevel)
    {
    స్థితి LOG_LEVEL_FATAL:   ss << "[FATAL:" << సందేశం_id << "] " << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_ERROR:   ss << "[ERROR:" << సందేశం_id << "] " << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_WARNING: ss << "[ WARN:" << సందేశం_id << "] " << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_INFO:    ss << "[ INFO:" << సందేశం_id << "] " << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << సందేశం_id << "] " << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_VERBOSE: ss << message << std::పంక్తి_ముగింపు; ఆపు;
    స్థితి LOG_LEVEL_SILENT: తిరిగి;  // avoid compiler warning about incomplete switch
    స్థితి ENUM_LOG_LEVEL_FORCE_INT: తిరిగి;  // avoid compiler warning about incomplete switch
    }
#నిర్వచితమైతే __ANDROID__
    పూర్ణాంకం android_logLevel = ANDROID_LOG_INFO;
    ఎంపిక (logLevel)
    {
    స్థితి LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; ఆపు;
    స్థితి LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; ఆపు;
    స్థితి LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; ఆపు;
    స్థితి LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; ఆపు;
    స్థితి LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; ఆపు;
    స్థితి LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; ఆపు;
    అప్రమేయ:
        ఆపు;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_సంస్కరణ, "%s", ss.str().c_str());
#అయితే_ముగింపు
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::బయటకు;
    (*out) << ss.str();
    ఒకవేళ (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

స్థిర స్థిరరాశి అక్షరం* strip_ఆధారం_File_మార్గం_ఉపసర్గ(స్థిరరాశి అక్షరం* file)
{
    CV_Assert(file);
    స్థిరరాశి అక్షరం* pos = file;
    స్థిరరాశి అక్షరం* strip_pos = ఖాళీ;
    అక్షరం ch = 0;
    అయితే ((ch = pos[0]) != 0)
    {
        ++pos;
        ఒకవేళ (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    ఒకవేళ (strip_pos == ఖాళీ || strip_pos == pos/*eos*/)
        తిరిగి file;
    తిరిగి strip_pos;
}

శూన్య write_Log_సందేశం_Ex(LogLevel logLevel, స్థిరరాశి అక్షరం* ట్యాగ్, స్థిరరాశి అక్షరం* file, పూర్ణాంకం పంక్తి, స్థిరరాశి అక్షరం* func, స్థిరరాశి అక్షరం* message)
{
    Write_Log_సందేశం_Ex_Func_Type userFunc = stc_user_Write_Log_సందేశం_Ex_Func.load();
    ఒకవేళ (userFunc && userFunc != write_Log_సందేశం_Ex)
    {
        (*userFunc)(logLevel, ట్యాగ్, file, పంక్తి, func, message);
        తిరిగి;
    }

    std::ostringstream strm;
    ఒకవేళ (ట్యాగ్)
    {
        strm << ట్యాగ్ << ' ';
    }
    ఒకవేళ (file)
    {
        strm << strip_ఆధారం_File_మార్గం_ఉపసర్గ(file);
        ఒకవేళ (పంక్తి > 0)
        {
            strm << ':' << పంక్తి;
        }
        strm << ' ';
    }
    ఒకవేళ (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_సందేశం(logLevel, strm.str().c_str());
}

శూన్య replace_Write_Log_సందేశం(Write_Log_సందేశం_Func_Type f)
{
    ఒకవేళ (f == write_Log_సందేశం)
    {
        f = శూన్యసూచిక;
    }
    stc_user_Write_Log_సందేశం_Func.store(f);
}

శూన్య replace_Write_Log_సందేశం_Ex(Write_Log_సందేశం_Ex_Func_Type f)
{
    ఒకవేళ (f == write_Log_సందేశం_Ex)
    {
        f = శూన్యసూచిక;
    }
    stc_user_Write_Log_సందేశం_Ex_Func.store(f);
}

} // namespace

}}} // namespace
