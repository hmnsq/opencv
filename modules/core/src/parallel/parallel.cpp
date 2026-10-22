// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ಅನ್ತರ್ಭಾವ "../precomp.hpp"
#ಅನ್ತರ್ಭಾವ "parallel.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ಯಥಾ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>


#ಅನ್ತರ್ಭಾವ "registry_parallel.hpp"
#ಅನ್ತರ್ಭಾವ "registry_parallel.impl.hpp"

#ಅನ್ತರ್ಭಾವ "plugin_parallel_api.hpp"
#ಅನ್ತರ್ಭಾವ "plugin_parallel_wrapper.impl.hpp"


ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ಸ್ಥಿರ
std::string& getParallelBackendName()
{
    ಸ್ಥಿರ std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ಪ್ರತ್ಯಾಗಮ g_backendName;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ g_initializedParallelForAPI = ಅಸತ್ಯ;

ಸ್ಥಿರ
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ಧ್ರುವ std::string& name = getParallelBackendName();
    ತಾರ್ಕಿಕ isKnown = ಅಸತ್ಯ;
    ಧ್ರುವ auto& backends = getParallelBackendsInfo();
    ಯದಿ (!name.empty())
    {
        CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): requested backend name: " << name);
    }
    ಪ್ರತಿ (size_t i = 0; i < backends.size(); i++)
    {
        ಧ್ರುವ auto& info = backends[i];
        ಯದಿ (!name.empty())
        {
            ಯದಿ (name != info.name)
            {
                ಅನುವರ್ತ;
            }
            isKnown = ಸತ್ಯ;
        }
        ಯತ್ನ
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            ಯದಿ (!info.backendFactory)
            {
                CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ಅನುವರ್ತ;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ಯದಿ (!backend)
            {
                CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): not available: " << info.name);
                ಅನುವರ್ತ;
            }
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = ಸತ್ಯ;
            getParallelBackendName() = info.name;
            ಪ್ರತ್ಯಾಗಮ backend;
        }
        ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
        {
            CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        ಗೃಹಾಣ (...)
        {
            CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ಯದಿ (name.empty())
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): fallback on builtin code");
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (!isKnown)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = ಸತ್ಯ;
    getParallelBackendName() = std::string();
    ಪ್ರತ್ಯಾಗಮ std::shared_ptr<ParallelForAPI>();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): Initializing parallel backend...");
    ಪ್ರತ್ಯಾಗಮ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ಸ್ಥಿರ std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ಪ್ರತ್ಯಾಗಮ g_currentParallelForAPI;
}

ಶೂನ್ಯ setParallelForBackend(ಧ್ರುವ std::shared_ptr<ParallelForAPI>& api, ತಾರ್ಕಿಕ propagate_ಅಂಕೆ_Threads)
{
    getCurrentParallelForAPI() = api;
    ಯದಿ (propagate_ಅಂಕೆ_Threads && api)
    {
        set_ಅಂಕೆ_Threads(ಅಂಕೆ_Threads);
    }
}

ತಾರ್ಕಿಕ setParallelForBackend(ಧ್ರುವ std::string& backendName, ತಾರ್ಕಿಕ propagate_ಅಂಕೆ_Threads)
{
    CV_TRACE_FUNCTION();

    ತಾರ್ಕಿಕ saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    ಯದಿ (g_initializedParallelForAPI)
    {
        // ... already initialized
        ಯದಿ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
        ಅನ್ಯಥಾ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ಅನ್ಯಥಾ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ಯದಿ (!api)
    {
        ಯದಿ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ಯದಿ (saved_initialized)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = ಅಸತ್ಯ;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ಯದಿ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ಯದಿ (propagate_ಅಂಕೆ_Threads)
    {
        set_ಅಂಕೆ_Threads(ಅಂಕೆ_Threads);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

}}  // namespace
