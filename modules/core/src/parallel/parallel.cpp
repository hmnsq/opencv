// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "parallel.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अण्णहा
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#अन्तर्भाव <opencv2/core/utils/logger.hpp>


#अन्तर्भाव "registry_parallel.hpp"
#अन्तर्भाव "registry_parallel.impl.hpp"

#अन्तर्भाव "plugin_parallel_api.hpp"
#अन्तर्भाव "plugin_parallel_wrapper.impl.hpp"


नामाकाश cv { नामाकाश parallel {

पूर्णाङ्क अङ्कः_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

स्थिर
std::string& getParallelBackendName()
{
    स्थिर std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    प्रत्यागम g_backendName;
}

स्थिर तार्किक g_initializedParallelForAPI = मिच्छ;

स्थिर
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ध्रुव std::string& name = getParallelBackendName();
    तार्किक isKnown = मिच्छ;
    ध्रुव auto& backends = getParallelBackendsInfo();
    जइ (!name.empty())
    {
        CV_LOG_INFO(रिक्त, "core(parallel): requested backend name: " << name);
    }
    प्रति (size_t i = 0; i < backends.size(); i++)
    {
        ध्रुव auto& info = backends[i];
        जइ (!name.empty())
        {
            जइ (name != info.name)
            {
                अनुवर्त;
            }
            isKnown = सच्च;
        }
        यत्न
        {
            CV_LOG_DEBUG(रिक्त, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            जइ (!info.backendFactory)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                अनुवर्त;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            जइ (!backend)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): not available: " << info.name);
                अनुवर्त;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = सच्च;
            getParallelBackendName() = info.name;
            प्रत्यागम backend;
        }
        गृहाण (ध्रुव std::exception& e)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        गृहाण (...)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    जइ (name.empty())
    {
        CV_LOG_DEBUG(रिक्त, "core(parallel): fallback on builtin code");
    }
    अण्णहा
    {
        जइ (!isKnown)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        अण्णहा
        {
            CV_LOG_INFO(रिक्त, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = सच्च;
    getParallelBackendName() = std::string();
    प्रत्यागम std::shared_ptr<ParallelForAPI>();
}

स्थिर अन्तःस्थ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(रिक्त, "core(parallel): Initializing parallel backend...");
    प्रत्यागम createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    स्थिर std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    प्रत्यागम g_currentParallelForAPI;
}

शून्य setParallelForBackend(ध्रुव std::shared_ptr<ParallelForAPI>& api, तार्किक propagate_अङ्कः_Threads)
{
    getCurrentParallelForAPI() = api;
    जइ (propagate_अङ्कः_Threads && api)
    {
        set_अङ्कः_Threads(अङ्कः_Threads);
    }
}

तार्किक setParallelForBackend(ध्रुव std::string& backendName, तार्किक propagate_अङ्कः_Threads)
{
    CV_TRACE_FUNCTION();

    तार्किक saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    जइ (g_initializedParallelForAPI)
    {
        // ... already initialized
        जइ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            प्रत्यागम सच्च;
        }
        अण्णहा
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(रिक्त, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    अण्णहा
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    जइ (!api)
    {
        जइ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            जइ (saved_initialized)
            {
                CV_LOG_WARNING(रिक्त, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            अण्णहा
            {
                CV_LOG_WARNING(रिक्त, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = मिच्छ;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            प्रत्यागम मिच्छ;
        }
        अण्णहा
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): switched to builtin code (legacy)");
        }
    }
    जइ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    जइ (propagate_अङ्कः_Threads)
    {
        set_अङ्कः_Threads(अङ्कः_Threads);
    }
    प्रत्यागम सच्च;
}

}}  // namespace
