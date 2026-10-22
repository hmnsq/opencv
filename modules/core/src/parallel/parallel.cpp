// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "parallel.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ଅନ୍ୟଥା
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>


#ଅନ୍ତର୍ଭାଵ "registry_parallel.hpp"
#ଅନ୍ତର୍ଭାଵ "registry_parallel.impl.hpp"

#ଅନ୍ତର୍ଭାଵ "plugin_parallel_api.hpp"
#ଅନ୍ତର୍ଭାଵ "plugin_parallel_wrapper.impl.hpp"


ନାମାକାଶ cv { ନାମାକାଶ parallel {

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ସ୍ଥିର
std::string& getParallelBackendName()
{
    ସ୍ଥିର std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    ଫେରନ୍ତୁ g_backendName;
}

ସ୍ଥିର ତାର୍କିକ g_initializedParallelForAPI = ମିଥ୍ୟା;

ସ୍ଥିର
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ଅଚଳ std::string& name = getParallelBackendName();
    ତାର୍କିକ isKnown = ମିଥ୍ୟା;
    ଅଚଳ auto& backends = getParallelBackendsInfo();
    ଯଦି (!name.empty())
    {
        CV_LOG_INFO(ଖାଲି, "core(parallel): requested backend name: " << name);
    }
    ପାଇଁ (size_t i = 0; i < backends.size(); i++)
    {
        ଅଚଳ auto& info = backends[i];
        ଯଦି (!name.empty())
        {
            ଯଦି (name != info.name)
            {
                ଜାରି;
            }
            isKnown = ସତ୍ୟ;
        }
        ଚେଷ୍ଟା
        {
            CV_LOG_DEBUG(ଖାଲି, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            ଯଦି (!info.backendFactory)
            {
                CV_LOG_DEBUG(ଖାଲି, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ଜାରି;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ଯଦି (!backend)
            {
                CV_LOG_DEBUG(ଖାଲି, "core(parallel): not available: " << info.name);
                ଜାରି;
            }
            CV_LOG_INFO(ଖାଲି, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = ସତ୍ୟ;
            getParallelBackendName() = info.name;
            ଫେରନ୍ତୁ backend;
        }
        ଧର (ଅଚଳ std::exception& e)
        {
            CV_LOG_WARNING(ଖାଲି, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        ଧର (...)
        {
            CV_LOG_WARNING(ଖାଲି, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ଯଦି (name.empty())
    {
        CV_LOG_DEBUG(ଖାଲି, "core(parallel): fallback on builtin code");
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (!isKnown)
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = ସତ୍ୟ;
    getParallelBackendName() = std::string();
    ଫେରନ୍ତୁ std::shared_ptr<ParallelForAPI>();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ଖାଲି, "core(parallel): Initializing parallel backend...");
    ଫେରନ୍ତୁ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ସ୍ଥିର std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    ଫେରନ୍ତୁ g_currentParallelForAPI;
}

ଶୂନ୍ୟ setParallelForBackend(ଅଚଳ std::shared_ptr<ParallelForAPI>& api, ତାର୍କିକ propagate_ଅଙ୍କ_Threads)
{
    getCurrentParallelForAPI() = api;
    ଯଦି (propagate_ଅଙ୍କ_Threads && api)
    {
        set_ଅଙ୍କ_Threads(ଅଙ୍କ_Threads);
    }
}

ତାର୍କିକ setParallelForBackend(ଅଚଳ std::string& backendName, ତାର୍କିକ propagate_ଅଙ୍କ_Threads)
{
    CV_TRACE_FUNCTION();

    ତାର୍କିକ saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    ଯଦି (g_initializedParallelForAPI)
    {
        // ... already initialized
        ଯଦି (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
        ଅନ୍ୟଥା
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ଖାଲି, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ଅନ୍ୟଥା
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ଯଦି (!api)
    {
        ଯଦି (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ଯଦି (saved_initialized)
            {
                CV_LOG_WARNING(ଖାଲି, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_WARNING(ଖାଲି, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = ମିଥ୍ୟା;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_WARNING(ଖାଲି, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ଯଦି (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ଯଦି (propagate_ଅଙ୍କ_Threads)
    {
        set_ଅଙ୍କ_Threads(ଅଙ୍କ_Threads);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

}}  // namespace
