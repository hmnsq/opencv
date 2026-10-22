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

#यदि_परिभाषितम् __ANDROID__
# include <android/log.h>
#यदि_अन्तः

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
#यदि defined NDEBUG
    स्थिर ध्रुव तार्किक m_isDebugBuild = असत्य;
#अन्यथा
    स्थिर ध्रुव तार्किक m_isDebugBuild = सत्य;
#यदि_अन्तः

सार्वजनिक:
    स्थिर LogLevel m_defaultUnconfiguredGlobalLevel;

सार्वजनिक:
    Log_चिह्नकम्_Manager log_चिह्नकम्_Manager;

    GlobalLoggingInitStruct()
        : log_चिह्नकम्_Manager(m_defaultUnconfiguredGlobalLevel)
    {
        (शून्य)getInitializationMutex();  // ensure initialization of global objects

        applyConfigString();
        handleMalformed();
    }

गुप्त:
    शून्य applyConfigString()
    {
        log_चिह्नकम्_Manager.समुच्चयः_Config_String(utils::getConfigurationParameterString("OPENCV_LOG_LEVEL", ""));
    }

    शून्य handleMalformed()
    {
        // need to print warning for malformed log tag config strings?
        यदि (m_isDebugBuild)
        {
            ध्रुव स्वचालित& parser = log_चिह्नकम्_Manager.getConfigParser();
            यदि (parser.hasMalformed())
            {
                ध्रुव स्वचालित& malformedList = parser.getMalformed();
                प्रति (ध्रुव स्वचालित& malformed : malformedList)
                {
                    std::निर्गमः << "Malformed log level config: \"" << malformed << "\"\n";
                }
                std::निर्गमः.flush();
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
        (शून्य)get_Global_Log_चिह्नकम्();  // complete initialization of logger structures
    }
};

स्थिर GlobalLoggingInitCall globalLoggingInitCall;

स्थिर Log_चिह्नकम्_Manager& get_Log_चिह्नकम्_Manager()
{
    स्थिर Log_चिह्नकम्_Manager& log_चिह्नकम्_Manager_Instance = getGlobalLoggingInitStruct().log_चिह्नकम्_Manager;
    प्रत्यागम log_चिह्नकम्_Manager_Instance;
}

स्थिर LogLevel& getLogLevelVariable()
{
    स्थिर LogLevel& refGlobalLogLevel = get_Global_Log_चिह्नकम्()->level;
    प्रत्यागम refGlobalLogLevel;
}

Log_चिह्नकम्* get_Global_Log_चिह्नकम्()
{
    स्थिर Log_चिह्नकम्* global_Log_चिह्नकम्_Ptr = getGlobalLoggingInitStruct().log_चिह्नकम्_Manager.get("global");
    प्रत्यागम global_Log_चिह्नकम्_Ptr;
}

} // namespace

शून्य पञ्जिकाकोशः_Log_चिह्नकम्(Log_चिह्नकम्* plogtag)
{
    यदि (!plogtag || !plogtag->name)
    {
        प्रत्यागम;
    }
    internal::get_Log_चिह्नकम्_Manager().assign(plogtag->name, plogtag);
}

शून्य समुच्चयः_Log_चिह्नकम्_Level(ध्रुव वर्ण* चिह्नकम्, LogLevel level)
{
    यदि (!चिह्नकम्)
    {
        प्रत्यागम;
    }
    internal::get_Log_चिह्नकम्_Manager().समुच्चयः_Level_By_पूर्णम्_Name(std::string(चिह्नकम्), level);
}

LogLevel get_Log_चिह्नकम्_Level(ध्रुव वर्ण* चिह्नकम्)
{
    यदि (!चिह्नकम्)
    {
        प्रत्यागम getLogLevel();
    }
    ध्रुव Log_चिह्नकम्* ptr = internal::get_Log_चिह्नकम्_Manager().get(std::string(चिह्नकम्));
    यदि (!ptr)
    {
        प्रत्यागम getLogLevel();
    }
    प्रत्यागम ptr->level;
}

LogLevel समुच्चयः_Log_Level(LogLevel logLevel)
{
    // note: not thread safe, use sparingly and do not critically depend on outcome
    LogLevel& refGlobalLevel = internal::getLogLevelVariable();
    ध्रुव LogLevel old = refGlobalLevel;
    refGlobalLevel = logLevel;
    प्रत्यागम old;
}

LogLevel getLogLevel()
{
    प्रत्यागम internal::getLogLevelVariable();
}

नामाकाश internal {

नामाकाश //unnamed
{
    std::atomic<Write_Log_सन्देशः_Func_Type> stc_user_Write_Log_सन्देशः_Func{};
    std::atomic<Write_Log_सन्देशः_Ex_Func_Type> stc_user_Write_Log_सन्देशः_Ex_Func{};
} //unnamed

स्थिर पूर्णाङ्क get_Show_Timestamp_बहुलकम्()
{
    स्थिर तार्किक param_timestamp_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP", सत्य);
    स्थिर तार्किक param_timestamp_ns_enable = utils::getConfigurationParameterBool("OPENCV_LOG_TIMESTAMP_NS", असत्य);
    प्रत्यागम (param_timestamp_enable ? 1 : 0) + (param_timestamp_ns_enable ? 2 : 0);
}

शून्य write_Log_सन्देशः(LogLevel logLevel, ध्रुव वर्ण* message)
{
    Write_Log_सन्देशः_Func_Type userFunc = stc_user_Write_Log_सन्देशः_Func.load();
    यदि (userFunc && userFunc != write_Log_सन्देशः)
    {
        (*userFunc)(logLevel, message);
        प्रत्यागम;
    }

    ध्रुव पूर्णाङ्क threadID = cv::utils::getThreadID();

    std::string सन्देशः_id;
    विकल्प (get_Show_Timestamp_बहुलकम्())
    {
        पक्ष 1: सन्देशः_id = cv::format("%d@%0.3f", threadID, getTimestampNS() * 1e-9); भङ्ग;
        पक्ष 1+2: सन्देशः_id = cv::format("%d@%llu", threadID, (दीर्घ दीर्घ अचिह्नित पूर्णाङ्क)getTimestampNS()); भङ्ग;
        स्वभाव: सन्देशः_id = cv::format("%d", threadID); भङ्ग;
    }

    std::ostringstream ss;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   ss << "[FATAL:" << सन्देशः_id << "] " << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_ERROR:   ss << "[ERROR:" << सन्देशः_id << "] " << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_WARNING: ss << "[ WARN:" << सन्देशः_id << "] " << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_INFO:    ss << "[ INFO:" << सन्देशः_id << "] " << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_DEBUG:   ss << "[DEBUG:" << सन्देशः_id << "] " << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_VERBOSE: ss << message << std::पङ्क्त्यन्तः; भङ्ग;
    पक्ष LOG_LEVEL_SILENT: प्रत्यागम;  // avoid compiler warning about incomplete switch
    पक्ष ENUM_LOG_LEVEL_FORCE_INT: प्रत्यागम;  // avoid compiler warning about incomplete switch
    }
#यदि_परिभाषितम् __ANDROID__
    पूर्णाङ्क android_logLevel = ANDROID_LOG_INFO;
    विकल्प (logLevel)
    {
    पक्ष LOG_LEVEL_FATAL:   android_logLevel = ANDROID_LOG_FATAL; भङ्ग;
    पक्ष LOG_LEVEL_ERROR:   android_logLevel = ANDROID_LOG_ERROR; भङ्ग;
    पक्ष LOG_LEVEL_WARNING: android_logLevel = ANDROID_LOG_WARN; भङ्ग;
    पक्ष LOG_LEVEL_INFO:    android_logLevel = ANDROID_LOG_INFO; भङ्ग;
    पक्ष LOG_LEVEL_DEBUG:   android_logLevel = ANDROID_LOG_DEBUG; भङ्ग;
    पक्ष LOG_LEVEL_VERBOSE: android_logLevel = ANDROID_LOG_VERBOSE; भङ्ग;
    स्वभाव:
        भङ्ग;
    }
    __android_log_print(android_logLevel, "OpenCV/" CV_संस्करणम्, "%s", ss.str().c_str());
#यदि_अन्तः
    std::ostream* out = (logLevel <= LOG_LEVEL_WARNING) ? &std::cerr : &std::निर्गमः;
    (*out) << ss.str();
    यदि (logLevel <= LOG_LEVEL_WARNING)
    {
        (*out) << std::flush;
    }
}

स्थिर ध्रुव वर्ण* strip_Source_File_मार्गः_उपसर्गः(ध्रुव वर्ण* file)
{
    CV_Assert(file);
    ध्रुव वर्ण* pos = file;
    ध्रुव वर्ण* strip_pos = रिक्त;
    वर्ण ch = 0;
    यावत् ((ch = pos[0]) != 0)
    {
        ++pos;
        यदि (ch == '/' || ch == '\\')
            strip_pos = pos;
    }
    यदि (strip_pos == रिक्त || strip_pos == pos/*eos*/)
        प्रत्यागम file;
    प्रत्यागम strip_pos;
}

शून्य write_Log_सन्देशः_Ex(LogLevel logLevel, ध्रुव वर्ण* चिह्नकम्, ध्रुव वर्ण* file, पूर्णाङ्क पङ्क्तिः, ध्रुव वर्ण* func, ध्रुव वर्ण* message)
{
    Write_Log_सन्देशः_Ex_Func_Type userFunc = stc_user_Write_Log_सन्देशः_Ex_Func.load();
    यदि (userFunc && userFunc != write_Log_सन्देशः_Ex)
    {
        (*userFunc)(logLevel, चिह्नकम्, file, पङ्क्तिः, func, message);
        प्रत्यागम;
    }

    std::ostringstream strm;
    यदि (चिह्नकम्)
    {
        strm << चिह्नकम् << ' ';
    }
    यदि (file)
    {
        strm << strip_Source_File_मार्गः_उपसर्गः(file);
        यदि (पङ्क्तिः > 0)
        {
            strm << ':' << पङ्क्तिः;
        }
        strm << ' ';
    }
    यदि (func)
    {
        strm << func << ' ';
    }
    strm << message;
    write_Log_सन्देशः(logLevel, strm.str().c_str());
}

शून्य replace_Write_Log_सन्देशः(Write_Log_सन्देशः_Func_Type f)
{
    यदि (f == write_Log_सन्देशः)
    {
        f = शून्यसूचकः;
    }
    stc_user_Write_Log_सन्देशः_Func.store(f);
}

शून्य replace_Write_Log_सन्देशः_Ex(Write_Log_सन्देशः_Ex_Func_Type f)
{
    यदि (f == write_Log_सन्देशः_Ex)
    {
        f = शून्यसूचकः;
    }
    stc_user_Write_Log_सन्देशः_Ex_Func.store(f);
}

} // namespace

}}} // namespace
