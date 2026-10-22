// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#অন্তর্ভাব "../precomp.hpp"
#অন্তর্ভাব "parallel.hpp"

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্যথা
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>


#অন্তর্ভাব "registry_parallel.hpp"
#অন্তর্ভাব "registry_parallel.impl.hpp"

#অন্তর্ভাব "plugin_parallel_api.hpp"
#অন্তর্ভাব "plugin_parallel_wrapper.impl.hpp"


নামাকাশ cv { নামাকাশ parallel {

পূর্ণসংখ্যা অঙ্ক_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

স্থির
std::string& getParallelBackendName()
{
    স্থির std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ফেরত g_backendName;
}

স্থির যৌক্তিক g_initializedParallelForAPI = মিথ্যা;

স্থির
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ধ্রুবক std::string& name = getParallelBackendName();
    যৌক্তিক isKnown = মিথ্যা;
    ধ্রুবক auto& backends = getParallelBackendsInfo();
    যদি (!name.empty())
    {
        CV_LOG_INFO(ফাঁকা, "core(parallel): requested backend name: " << name);
    }
    জন্য (size_t i = 0; i < backends.size(); i++)
    {
        ধ্রুবক auto& info = backends[i];
        যদি (!name.empty())
        {
            যদি (name != info.name)
            {
                চালু;
            }
            isKnown = সত্য;
        }
        চেষ্টা
        {
            CV_LOG_DEBUG(ফাঁকা, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            যদি (!info.backendFactory)
            {
                CV_LOG_DEBUG(ফাঁকা, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                চালু;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            যদি (!backend)
            {
                CV_LOG_DEBUG(ফাঁকা, "core(parallel): not available: " << info.name);
                চালু;
            }
            CV_LOG_INFO(ফাঁকা, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = সত্য;
            getParallelBackendName() = info.name;
            ফেরত backend;
        }
        ধরো (ধ্রুবক std::exception& e)
        {
            CV_LOG_WARNING(ফাঁকা, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        ধরো (...)
        {
            CV_LOG_WARNING(ফাঁকা, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    যদি (name.empty())
    {
        CV_LOG_DEBUG(ফাঁকা, "core(parallel): fallback on builtin code");
    }
    অন্যথা
    {
        যদি (!isKnown)
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        অন্যথা
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = সত্য;
    getParallelBackendName() = std::string();
    ফেরত std::shared_ptr<ParallelForAPI>();
}

স্থির অন্তঃরেখ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ফাঁকা, "core(parallel): Initializing parallel backend...");
    ফেরত createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    স্থির std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ফেরত g_currentParallelForAPI;
}

শূন্য setParallelForBackend(ধ্রুবক std::shared_ptr<ParallelForAPI>& api, যৌক্তিক propagate_অঙ্ক_Threads)
{
    getCurrentParallelForAPI() = api;
    যদি (propagate_অঙ্ক_Threads && api)
    {
        set_অঙ্ক_Threads(অঙ্ক_Threads);
    }
}

যৌক্তিক setParallelForBackend(ধ্রুবক std::string& backendName, যৌক্তিক propagate_অঙ্ক_Threads)
{
    CV_TRACE_FUNCTION();

    যৌক্তিক saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    যদি (g_initializedParallelForAPI)
    {
        // ... already initialized
        যদি (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ফেরত সত্য;
        }
        অন্যথা
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ফাঁকা, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    অন্যথা
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    যদি (!api)
    {
        যদি (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            যদি (saved_initialized)
            {
                CV_LOG_WARNING(ফাঁকা, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            অন্যথা
            {
                CV_LOG_WARNING(ফাঁকা, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = মিথ্যা;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ফেরত মিথ্যা;
        }
        অন্যথা
        {
            CV_LOG_WARNING(ফাঁকা, "core(parallel): switched to builtin code (legacy)");
        }
    }
    যদি (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    যদি (propagate_অঙ্ক_Threads)
    {
        set_অঙ্ক_Threads(অঙ্ক_Threads);
    }
    ফেরত সত্য;
}

}}  // namespace
