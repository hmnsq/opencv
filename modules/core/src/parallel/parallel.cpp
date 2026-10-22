// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "../precomp.hpp"
#include "parallel.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#បើមិនដូច្នេះ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#include <opencv2/core/utils/logger.hpp>


#include "registry_parallel.hpp"
#include "registry_parallel.impl.hpp"

#include "plugin_parallel_api.hpp"
#include "plugin_parallel_wrapper.impl.hpp"


namespace cv { namespace parallel {

ចំនួនគត់ numThreads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

static
std::string& getParallelBackendName()
{
    static std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ត្រឡប់ g_backendName;
}

static bool g_initializedParallelForAPI = false;

static
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    const std::string& name = getParallelBackendName();
    bool isKnown = false;
    const auto& backends = getParallelBackendsInfo();
    ប្រសិនបើ (!name.empty())
    {
        CV_LOG_INFO(NULL, "core(parallel): requested backend name: " << name);
    }
    សម្រាប់ (size_t i = 0; i < backends.size(); i++)
    {
        const auto& info = backends[i];
        ប្រសិនបើ (!name.empty())
        {
            ប្រសិនបើ (name != info.name)
            {
                continue;
            }
            isKnown = true;
        }
        try
        {
            CV_LOG_DEBUG(NULL, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            ប្រសិនបើ (!info.backendFactory)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                continue;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ប្រសិនបើ (!backend)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): not available: " << info.name);
                continue;
            }
            CV_LOG_INFO(NULL, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = true;
            getParallelBackendName() = info.name;
            ត្រឡប់ backend;
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
    ប្រសិនបើ (name.empty())
    {
        CV_LOG_DEBUG(NULL, "core(parallel): fallback on builtin code");
    }
    បើមិនដូច្នេះ
    {
        ប្រសិនបើ (!isKnown)
        {
            CV_LOG_INFO(NULL, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        បើមិនដូច្នេះ
        {
            CV_LOG_INFO(NULL, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = true;
    getParallelBackendName() = std::string();
    ត្រឡប់ std::shared_ptr<ParallelForAPI>();
}

static anta_hstha
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(NULL, "core(parallel): Initializing parallel backend...");
    ត្រឡប់ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    static std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ត្រឡប់ g_currentParallelForAPI;
}

void setParallelForBackend(const std::shared_ptr<ParallelForAPI>& api, bool propagateNumThreads)
{
    getCurrentParallelForAPI() = api;
    ប្រសិនបើ (propagateNumThreads && api)
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
    ប្រសិនបើ (g_initializedParallelForAPI)
    {
        // ... already initialized
        ប្រសិនបើ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(NULL, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ត្រឡប់ true;
        }
        បើមិនដូច្នេះ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(NULL, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    បើមិនដូច្នេះ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ប្រសិនបើ (!api)
    {
        ប្រសិនបើ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ប្រសិនបើ (saved_initialized)
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            បើមិនដូច្នេះ
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = false;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ត្រឡប់ false;
        }
        បើមិនដូច្នេះ
        {
            CV_LOG_WARNING(NULL, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ប្រសិនបើ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ប្រសិនបើ (propagateNumThreads)
    {
        setNumThreads(numThreads);
    }
    ត្រឡប់ true;
}

}}  // namespace
