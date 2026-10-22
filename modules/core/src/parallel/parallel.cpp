// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "parallel.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NDEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>


#ਅਨ੍ਤਰ੍ਭਾਵ "registry_parallel.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "registry_parallel.impl.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "plugin_parallel_api.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "plugin_parallel_wrapper.impl.hpp"


ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

ਪੂਰਨ ਅੰਕ_ਧਾਗੇ = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ਸਥਿਰ
std::string& getParallelBackendName()
{
    ਸਥਿਰ std::string g_backendName = ਮੰਜ਼ਿਲ_ਉੱਤਲਾ_Case(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ਵਾਪਸ g_backendName;
}

ਸਥਿਰ ਤਾਰਕਿਕ g_initializedParallelForAPI = ਝੂਠ;

ਸਥਿਰ
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ਅਚਲ std::string& name = getParallelBackendName();
    ਤਾਰਕਿਕ is_ਗਿਆਤ = ਝੂਠ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ& backends = getParallelBackendsInfo();
    ਜੇ (!name.empty())
    {
        CV_LOG_INFO(ਨਿਰਾ, "core(parallel): requested backend name: " << name);
    }
    ਲਈ (size_t i = 0; i < backends.size(); i++)
    {
        ਅਚਲ ਸਵੈਚਾਲਿਤ& info = backends[i];
        ਜੇ (!name.empty())
        {
            ਜੇ (name != info.name)
            {
                ਜਾਰੀ;
            }
            is_ਗਿਆਤ = ਸੱਚ;
        }
        ਕੋਸ਼ਿਸ਼
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): trying backend: " << info.name << " (priority=" << info.ਤਰਜੀਹ << ")");
            ਜੇ (!info.backendFactory)
            {
                CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ਜਾਰੀ;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ਜੇ (!backend)
            {
                CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): not available: " << info.name);
                ਜਾਰੀ;
            }
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): using backend: " << info.name << " (priority=" << info.ਤਰਜੀਹ << ")");
            g_initializedParallelForAPI = ਸੱਚ;
            getParallelBackendName() = info.name;
            ਵਾਪਸ backend;
        }
        ਫੜੋ (ਅਚਲ std::exception& e)
        {
            CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        ਫੜੋ (...)
        {
            CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ਜੇ (name.empty())
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): fallback on builtin code");
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (!is_ਗਿਆਤ)
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = ਸੱਚ;
    getParallelBackendName() = std::string();
    ਵਾਪਸ std::shared_ptr<ParallelForAPI>();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): Initializing parallel backend...");
    ਵਾਪਸ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ਸਥਿਰ std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ਵਾਪਸ g_currentParallelForAPI;
}

ਖਾਲੀ ਸੈੱਟ_Parallel_For_Backend(ਅਚਲ std::shared_ptr<ParallelForAPI>& api, ਤਾਰਕਿਕ propagate_ਅੰਕ_ਧਾਗੇ)
{
    getCurrentParallelForAPI() = api;
    ਜੇ (propagate_ਅੰਕ_ਧਾਗੇ && api)
    {
        ਸੈੱਟ_ਅੰਕ_ਧਾਗੇ(ਅੰਕ_ਧਾਗੇ);
    }
}

ਤਾਰਕਿਕ ਸੈੱਟ_Parallel_For_Backend(ਅਚਲ std::string& backendName, ਤਾਰਕਿਕ propagate_ਅੰਕ_ਧਾਗੇ)
{
    CV_TRACE_FUNCTION();

    ਤਾਰਕਿਕ saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = ਮੰਜ਼ਿਲ_ਉੱਤਲਾ_Case(backendName);
    ਜੇ (g_initializedParallelForAPI)
    {
        // ... already initialized
        ਜੇ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ਵਾਪਸ ਸੱਚ;
        }
        ਨਹੀਂਤਾਂ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ਜੇ (!api)
    {
        ਜੇ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ਜੇ (saved_initialized)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = ਝੂਠ;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ਵਾਪਸ ਝੂਠ;
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ਜੇ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ਜੇ (propagate_ਅੰਕ_ਧਾਗੇ)
    {
        ਸੈੱਟ_ਅੰਕ_ਧਾਗੇ(ਅੰਕ_ਧਾਗੇ);
    }
    ਵਾਪਸ ਸੱਚ;
}

}}  // namespace
