// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ಅನ್ತರ್ಭಾವ "../precomp.hpp"
#ಅನ್ತರ್ಭಾವ "parallel.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NDEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>


#ಅನ್ತರ್ಭಾವ "registry_parallel.hpp"
#ಅನ್ತರ್ಭಾವ "registry_parallel.impl.hpp"

#ಅನ್ತರ್ಭಾವ "plugin_parallel_api.hpp"
#ಅನ್ತರ್ಭಾವ "plugin_parallel_wrapper.impl.hpp"


ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ಸ್ಥಿರ
std::string& getParallelBackendName()
{
    ಸ್ಥಿರ std::string g_backendName = ಗಮ್ಯ_ಊರ್ಧ್ವ_Case(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ಹಿಂತಿರುಗಿ g_backendName;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ g_initializedParallelForAPI = ಸುಳ್ಳು;

ಸ್ಥಿರ
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ಸ್ಥಿರಾಂಕ std::string& name = getParallelBackendName();
    ತಾರ್ಕಿಕ is_ತಿಳಿದ = ಸುಳ್ಳು;
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& backends = getParallelBackendsInfo();
    ಆದರೆ (!name.empty())
    {
        CV_LOG_INFO(ಖಾಲಿ, "core(parallel): requested backend name: " << name);
    }
    ಗಾಗಿ (size_t i = 0; i < backends.size(); i++)
    {
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& info = backends[i];
        ಆದರೆ (!name.empty())
        {
            ಆದರೆ (name != info.name)
            {
                ಮುಂದುವರಿ;
            }
            is_ತಿಳಿದ = ನಿಜ;
        }
        ಪ್ರಯತ್ನಿಸು
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): trying backend: " << info.name << " (priority=" << info.ಆದ್ಯತೆ << ")");
            ಆದರೆ (!info.backendFactory)
            {
                CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ಮುಂದುವರಿ;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ಆದರೆ (!backend)
            {
                CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): not available: " << info.name);
                ಮುಂದುವರಿ;
            }
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): using backend: " << info.name << " (priority=" << info.ಆದ್ಯತೆ << ")");
            g_initializedParallelForAPI = ನಿಜ;
            getParallelBackendName() = info.name;
            ಹಿಂತಿರುಗಿ backend;
        }
        ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
        {
            CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        ಹಿಡಿ (...)
        {
            CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ಆದರೆ (name.empty())
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): fallback on builtin code");
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (!is_ತಿಳಿದ)
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = ನಿಜ;
    getParallelBackendName() = std::string();
    ಹಿಂತಿರುಗಿ std::shared_ptr<ParallelForAPI>();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): Initializing parallel backend...");
    ಹಿಂತಿರುಗಿ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ಸ್ಥಿರ std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ಹಿಂತಿರುಗಿ g_currentParallelForAPI;
}

ಶೂನ್ಯ ಗಣ_Parallel_For_Backend(ಸ್ಥಿರಾಂಕ std::shared_ptr<ParallelForAPI>& api, ತಾರ್ಕಿಕ propagate_ಅಂಕೆ_ಎಳೆಗಳು)
{
    getCurrentParallelForAPI() = api;
    ಆದರೆ (propagate_ಅಂಕೆ_ಎಳೆಗಳು && api)
    {
        ಗಣ_ಅಂಕೆ_ಎಳೆಗಳು(ಅಂಕೆ_ಎಳೆಗಳು);
    }
}

ತಾರ್ಕಿಕ ಗಣ_Parallel_For_Backend(ಸ್ಥಿರಾಂಕ std::string& backendName, ತಾರ್ಕಿಕ propagate_ಅಂಕೆ_ಎಳೆಗಳು)
{
    CV_TRACE_FUNCTION();

    ತಾರ್ಕಿಕ saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = ಗಮ್ಯ_ಊರ್ಧ್ವ_Case(backendName);
    ಆದರೆ (g_initializedParallelForAPI)
    {
        // ... already initialized
        ಆದರೆ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ಆದರೆ (!api)
    {
        ಆದರೆ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ಆದರೆ (saved_initialized)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = ಸುಳ್ಳು;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ಆದರೆ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ಆದರೆ (propagate_ಅಂಕೆ_ಎಳೆಗಳು)
    {
        ಗಣ_ಅಂಕೆ_ಎಳೆಗಳು(ಅಂಕೆ_ಎಳೆಗಳು);
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

}}  // namespace
