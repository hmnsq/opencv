// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#அந்தர்பாவ "../precomp.hpp"
#அந்தர்பாவ "parallel.hpp"

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#இல்லையெனில்
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>


#அந்தர்பாவ "registry_parallel.hpp"
#அந்தர்பாவ "registry_parallel.impl.hpp"

#அந்தர்பாவ "plugin_parallel_api.hpp"
#அந்தர்பாவ "plugin_parallel_wrapper.impl.hpp"


நாமாகாஶ cv { நாமாகாஶ parallel {

முழு இலக்கம்_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

நிலையான
std::string& getParallelBackendName()
{
    நிலையான std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    திரும்பு g_backendName;
}

நிலையான தருக்க g_initializedParallelForAPI = பொய்;

நிலையான
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    மாறிலி std::string& name = getParallelBackendName();
    தருக்க isKnown = பொய்;
    மாறிலி auto& backends = getParallelBackendsInfo();
    எனில் (!name.empty())
    {
        CV_LOG_INFO(வெற்று, "core(parallel): requested backend name: " << name);
    }
    க்கு (size_t i = 0; i < backends.size(); i++)
    {
        மாறிலி auto& info = backends[i];
        எனில் (!name.empty())
        {
            எனில் (name != info.name)
            {
                தொடர்;
            }
            isKnown = உண்மை;
        }
        முயல்
        {
            CV_LOG_DEBUG(வெற்று, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            எனில் (!info.backendFactory)
            {
                CV_LOG_DEBUG(வெற்று, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                தொடர்;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            எனில் (!backend)
            {
                CV_LOG_DEBUG(வெற்று, "core(parallel): not available: " << info.name);
                தொடர்;
            }
            CV_LOG_INFO(வெற்று, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = உண்மை;
            getParallelBackendName() = info.name;
            திரும்பு backend;
        }
        பிடி (மாறிலி std::exception& e)
        {
            CV_LOG_WARNING(வெற்று, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        பிடி (...)
        {
            CV_LOG_WARNING(வெற்று, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    எனில் (name.empty())
    {
        CV_LOG_DEBUG(வெற்று, "core(parallel): fallback on builtin code");
    }
    இல்லையெனில்
    {
        எனில் (!isKnown)
        {
            CV_LOG_INFO(வெற்று, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        இல்லையெனில்
        {
            CV_LOG_INFO(வெற்று, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = உண்மை;
    getParallelBackendName() = std::string();
    திரும்பு std::shared_ptr<ParallelForAPI>();
}

நிலையான உள்வரி
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(வெற்று, "core(parallel): Initializing parallel backend...");
    திரும்பு createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    நிலையான std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    திரும்பு g_currentParallelForAPI;
}

வெறுமை setParallelForBackend(மாறிலி std::shared_ptr<ParallelForAPI>& api, தருக்க propagate_இலக்கம்_Threads)
{
    getCurrentParallelForAPI() = api;
    எனில் (propagate_இலக்கம்_Threads && api)
    {
        set_இலக்கம்_Threads(இலக்கம்_Threads);
    }
}

தருக்க setParallelForBackend(மாறிலி std::string& backendName, தருக்க propagate_இலக்கம்_Threads)
{
    CV_TRACE_FUNCTION();

    தருக்க saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    எனில் (g_initializedParallelForAPI)
    {
        // ... already initialized
        எனில் (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(வெற்று, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            திரும்பு உண்மை;
        }
        இல்லையெனில்
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(வெற்று, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    இல்லையெனில்
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    எனில் (!api)
    {
        எனில் (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            எனில் (saved_initialized)
            {
                CV_LOG_WARNING(வெற்று, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            இல்லையெனில்
            {
                CV_LOG_WARNING(வெற்று, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = பொய்;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            திரும்பு பொய்;
        }
        இல்லையெனில்
        {
            CV_LOG_WARNING(வெற்று, "core(parallel): switched to builtin code (legacy)");
        }
    }
    எனில் (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    எனில் (propagate_இலக்கம்_Threads)
    {
        set_இலக்கம்_Threads(இலக்கம்_Threads);
    }
    திரும்பு உண்மை;
}

}}  // namespace
