// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "../precomp.hpp"
#شامل "parallel.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وگرنه
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#شامل <opencv2/core/utils/logger.hpp>


#شامل "registry_parallel.hpp"
#شامل "registry_parallel.impl.hpp"

#شامل "plugin_parallel_api.hpp"
#شامل "plugin_parallel_wrapper.impl.hpp"


فضاینام cv { فضاینام parallel {

صحیح رقم_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ایستا
std::string& getParallelBackendName()
{
    ایستا std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    بازگشت g_backendName;
}

ایستا منطقی g_initializedParallelForAPI = نادرست;

ایستا
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ثابت std::string& name = getParallelBackendName();
    منطقی isKnown = نادرست;
    ثابت auto& backends = getParallelBackendsInfo();
    اگر (!name.empty())
    {
        CV_LOG_INFO(پوچ, "core(parallel): requested backend name: " << name);
    }
    برای (size_t i = 0; i < backends.size(); i++)
    {
        ثابت auto& info = backends[i];
        اگر (!name.empty())
        {
            اگر (name != info.name)
            {
                ادامه;
            }
            isKnown = درست;
        }
        تلاش
        {
            CV_LOG_DEBUG(پوچ, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            اگر (!info.backendFactory)
            {
                CV_LOG_DEBUG(پوچ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ادامه;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            اگر (!backend)
            {
                CV_LOG_DEBUG(پوچ, "core(parallel): not available: " << info.name);
                ادامه;
            }
            CV_LOG_INFO(پوچ, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = درست;
            getParallelBackendName() = info.name;
            بازگشت backend;
        }
        بگیر (ثابت std::exception& e)
        {
            CV_LOG_WARNING(پوچ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        بگیر (...)
        {
            CV_LOG_WARNING(پوچ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    اگر (name.empty())
    {
        CV_LOG_DEBUG(پوچ, "core(parallel): fallback on builtin code");
    }
    وگرنه
    {
        اگر (!isKnown)
        {
            CV_LOG_INFO(پوچ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        وگرنه
        {
            CV_LOG_INFO(پوچ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = درست;
    getParallelBackendName() = std::string();
    بازگشت std::shared_ptr<ParallelForAPI>();
}

ایستا درونخطی
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(پوچ, "core(parallel): Initializing parallel backend...");
    بازگشت createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ایستا std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    بازگشت g_currentParallelForAPI;
}

تهی setParallelForBackend(ثابت std::shared_ptr<ParallelForAPI>& api, منطقی propagate_رقم_Threads)
{
    getCurrentParallelForAPI() = api;
    اگر (propagate_رقم_Threads && api)
    {
        set_رقم_Threads(رقم_Threads);
    }
}

منطقی setParallelForBackend(ثابت std::string& backendName, منطقی propagate_رقم_Threads)
{
    CV_TRACE_FUNCTION();

    منطقی saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    اگر (g_initializedParallelForAPI)
    {
        // ... already initialized
        اگر (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(پوچ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            بازگشت درست;
        }
        وگرنه
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(پوچ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    وگرنه
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    اگر (!api)
    {
        اگر (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            اگر (saved_initialized)
            {
                CV_LOG_WARNING(پوچ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            وگرنه
            {
                CV_LOG_WARNING(پوچ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = نادرست;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            بازگشت نادرست;
        }
        وگرنه
        {
            CV_LOG_WARNING(پوچ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    اگر (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    اگر (propagate_رقم_Threads)
    {
        set_رقم_Threads(رقم_Threads);
    }
    بازگشت درست;
}

}}  // namespace
