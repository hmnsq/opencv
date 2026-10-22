// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "../precomp.hpp"
#include "parallel.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ပေႃးဢမ်ႇ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#include <opencv2/core/utils/logger.hpp>


#include "registry_parallel.hpp"
#include "registry_parallel.impl.hpp"

#include "plugin_parallel_api.hpp"
#include "plugin_parallel_wrapper.impl.hpp"


namespace cv { namespace parallel {

puur_naa_nka numThreads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

static
std::string& getParallelBackendName()
{
    static std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ပွၵ်ႈ g_backendName;
}

static bool g_initializedParallelForAPI = false;

static
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    const std::string& name = getParallelBackendName();
    bool isKnown = false;
    const auto& backends = getParallelBackendsInfo();
    ပေႃး (!name.empty())
    {
        CV_LOG_INFO(NULL, "core(parallel): requested backend name: " << name);
    }
    for (size_t i = 0; i < backends.size(); i++)
    {
        const auto& info = backends[i];
        ပေႃး (!name.empty())
        {
            ပေႃး (name != info.name)
            {
                continue;
            }
            isKnown = true;
        }
        try
        {
            CV_LOG_DEBUG(NULL, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            ပေႃး (!info.backendFactory)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                continue;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ပေႃး (!backend)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): not available: " << info.name);
                continue;
            }
            CV_LOG_INFO(NULL, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = true;
            getParallelBackendName() = info.name;
            ပွၵ်ႈ backend;
        }
        gr_haa_na (const std::exception& e)
        {
            CV_LOG_WARNING(NULL, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        gr_haa_na (...)
        {
            CV_LOG_WARNING(NULL, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ပေႃး (name.empty())
    {
        CV_LOG_DEBUG(NULL, "core(parallel): fallback on builtin code");
    }
    ပေႃးဢမ်ႇ
    {
        ပေႃး (!isKnown)
        {
            CV_LOG_INFO(NULL, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ပေႃးဢမ်ႇ
        {
            CV_LOG_INFO(NULL, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = true;
    getParallelBackendName() = std::string();
    ပွၵ်ႈ std::shared_ptr<ParallelForAPI>();
}

static anta_hstha
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(NULL, "core(parallel): Initializing parallel backend...");
    ပွၵ်ႈ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    static std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ပွၵ်ႈ g_currentParallelForAPI;
}

void setParallelForBackend(const std::shared_ptr<ParallelForAPI>& api, bool propagateNumThreads)
{
    getCurrentParallelForAPI() = api;
    ပေႃး (propagateNumThreads && api)
    {
        setNumThreads(numThreads);
    }
}

bool setParallelForBackend(const std::string& backendName, bool propagateNumThreads)
{
    CV_TRACE_FUNCTION();

    bool saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    ပေႃး (g_initializedParallelForAPI)
    {
        // ... already initialized
        ပေႃး (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(NULL, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ပွၵ်ႈ true;
        }
        ပေႃးဢမ်ႇ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(NULL, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ပေႃးဢမ်ႇ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ပေႃး (!api)
    {
        ပေႃး (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ပေႃး (saved_initialized)
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ပေႃးဢမ်ႇ
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = false;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ပွၵ်ႈ false;
        }
        ပေႃးဢမ်ႇ
        {
            CV_LOG_WARNING(NULL, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ပေႃး (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ပေႃး (propagateNumThreads)
    {
        setNumThreads(numThreads);
    }
    ပွၵ်ႈ true;
}

}}  // namespace
