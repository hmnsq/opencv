// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "../precomp.hpp"
#include "parallel.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#မဟုတ်ပါက
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#include <opencv2/core/utils/logger.hpp>


#include "registry_parallel.hpp"
#include "registry_parallel.impl.hpp"

#include "plugin_parallel_api.hpp"
#include "plugin_parallel_wrapper.impl.hpp"


namespace cv { namespace parallel {

ကိန်းပြည့် numThreads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

static
std::string& getParallelBackendName()
{
    static std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ပြန်ပေး g_backendName;
}

static bool g_initializedParallelForAPI = false;

static
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    const std::string& name = getParallelBackendName();
    bool isKnown = false;
    const auto& backends = getParallelBackendsInfo();
    အကယ်၍ (!name.empty())
    {
        CV_LOG_INFO(NULL, "core(parallel): requested backend name: " << name);
    }
    အတွက် (size_t i = 0; i < backends.size(); i++)
    {
        const auto& info = backends[i];
        အကယ်၍ (!name.empty())
        {
            အကယ်၍ (name != info.name)
            {
                continue;
            }
            isKnown = true;
        }
        try
        {
            CV_LOG_DEBUG(NULL, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            အကယ်၍ (!info.backendFactory)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                continue;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            အကယ်၍ (!backend)
            {
                CV_LOG_DEBUG(NULL, "core(parallel): not available: " << info.name);
                continue;
            }
            CV_LOG_INFO(NULL, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = true;
            getParallelBackendName() = info.name;
            ပြန်ပေး backend;
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
    အကယ်၍ (name.empty())
    {
        CV_LOG_DEBUG(NULL, "core(parallel): fallback on builtin code");
    }
    မဟုတ်ပါက
    {
        အကယ်၍ (!isKnown)
        {
            CV_LOG_INFO(NULL, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        မဟုတ်ပါက
        {
            CV_LOG_INFO(NULL, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = true;
    getParallelBackendName() = std::string();
    ပြန်ပေး std::shared_ptr<ParallelForAPI>();
}

static anta_hstha
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(NULL, "core(parallel): Initializing parallel backend...");
    ပြန်ပေး createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    static std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ပြန်ပေး g_currentParallelForAPI;
}

void setParallelForBackend(const std::shared_ptr<ParallelForAPI>& api, bool propagateNumThreads)
{
    getCurrentParallelForAPI() = api;
    အကယ်၍ (propagateNumThreads && api)
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
    အကယ်၍ (g_initializedParallelForAPI)
    {
        // ... already initialized
        အကယ်၍ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(NULL, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ပြန်ပေး true;
        }
        မဟုတ်ပါက
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(NULL, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    မဟုတ်ပါက
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    အကယ်၍ (!api)
    {
        အကယ်၍ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            အကယ်၍ (saved_initialized)
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            မဟုတ်ပါက
            {
                CV_LOG_WARNING(NULL, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = false;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ပြန်ပေး false;
        }
        မဟုတ်ပါက
        {
            CV_LOG_WARNING(NULL, "core(parallel): switched to builtin code (legacy)");
        }
    }
    အကယ်၍ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    အကယ်၍ (propagateNumThreads)
    {
        setNumThreads(numThreads);
    }
    ပြန်ပေး true;
}

}}  // namespace
