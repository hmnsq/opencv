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

পূর্ণাঙ্ক অঙ্ক_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

স্থির
std::string& getParallelBackendName()
{
    স্থির std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    প্রত্যাগম g_backendName;
}

স্থির তার্কিক g_initializedParallelForAPI = অসত্য;

স্থির
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ধ্রুব std::string& name = getParallelBackendName();
    তার্কিক isKnown = অসত্য;
    ধ্রুব auto& backends = getParallelBackendsInfo();
    যদি (!name.empty())
    {
        CV_LOG_INFO(রিক্ত, "core(parallel): requested backend name: " << name);
    }
    প্রতি (size_t i = 0; i < backends.size(); i++)
    {
        ধ্রুব auto& info = backends[i];
        যদি (!name.empty())
        {
            যদি (name != info.name)
            {
                অনুবর্ত;
            }
            isKnown = সত্য;
        }
        যত্ন
        {
            CV_LOG_DEBUG(রিক্ত, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            যদি (!info.backendFactory)
            {
                CV_LOG_DEBUG(রিক্ত, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                অনুবর্ত;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            যদি (!backend)
            {
                CV_LOG_DEBUG(রিক্ত, "core(parallel): not available: " << info.name);
                অনুবর্ত;
            }
            CV_LOG_INFO(রিক্ত, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = সত্য;
            getParallelBackendName() = info.name;
            প্রত্যাগম backend;
        }
        গৃহাণ (ধ্রুব std::exception& e)
        {
            CV_LOG_WARNING(রিক্ত, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        গৃহাণ (...)
        {
            CV_LOG_WARNING(রিক্ত, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    যদি (name.empty())
    {
        CV_LOG_DEBUG(রিক্ত, "core(parallel): fallback on builtin code");
    }
    অন্যথা
    {
        যদি (!isKnown)
        {
            CV_LOG_INFO(রিক্ত, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        অন্যথা
        {
            CV_LOG_INFO(রিক্ত, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = সত্য;
    getParallelBackendName() = std::string();
    প্রত্যাগম std::shared_ptr<ParallelForAPI>();
}

স্থির অন্তঃস্থ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(রিক্ত, "core(parallel): Initializing parallel backend...");
    প্রত্যাগম createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    স্থির std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    প্রত্যাগম g_currentParallelForAPI;
}

শূন্য setParallelForBackend(ধ্রুব std::shared_ptr<ParallelForAPI>& api, তার্কিক propagate_অঙ্ক_Threads)
{
    getCurrentParallelForAPI() = api;
    যদি (propagate_অঙ্ক_Threads && api)
    {
        set_অঙ্ক_Threads(অঙ্ক_Threads);
    }
}

তার্কিক setParallelForBackend(ধ্রুব std::string& backendName, তার্কিক propagate_অঙ্ক_Threads)
{
    CV_TRACE_FUNCTION();

    তার্কিক saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    যদি (g_initializedParallelForAPI)
    {
        // ... already initialized
        যদি (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(রিক্ত, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            প্রত্যাগম সত্য;
        }
        অন্যথা
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(রিক্ত, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
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
                CV_LOG_WARNING(রিক্ত, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            অন্যথা
            {
                CV_LOG_WARNING(রিক্ত, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = অসত্য;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            প্রত্যাগম অসত্য;
        }
        অন্যথা
        {
            CV_LOG_WARNING(রিক্ত, "core(parallel): switched to builtin code (legacy)");
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
    প্রত্যাগম সত্য;
}

}}  // namespace
