// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "parallel.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्यथा
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#अन्तर्भाव <opencv2/core/utils/logger.hpp>


#अन्तर्भाव "registry_parallel.hpp"
#अन्तर्भाव "registry_parallel.impl.hpp"

#अन्तर्भाव "plugin_parallel_api.hpp"
#अन्तर्भाव "plugin_parallel_wrapper.impl.hpp"


नामाकाश cv { नामाकाश parallel {

पूर्णाङ्क अंक_Threads = -1;

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

स्थिर तार्किक g_initializedParallelForAPI = असत्य;

स्थिर
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ध्रुव std::string& name = getParallelBackendName();
    तार्किक isKnown = असत्य;
    ध्रुव auto& backends = getParallelBackendsInfo();
    यदि (!name.empty())
    {
        CV_LOG_INFO(रिक्त, "core(parallel): requested backend name: " << name);
    }
    प्रति (size_t i = 0; i < backends.size(); i++)
    {
        ध्रुव auto& info = backends[i];
        यदि (!name.empty())
        {
            यदि (name != info.name)
            {
                अनुवर्त;
            }
            isKnown = सत्य;
        }
        यत्न
        {
            CV_LOG_DEBUG(रिक्त, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            यदि (!info.backendFactory)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                अनुवर्त;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            यदि (!backend)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): not available: " << info.name);
                अनुवर्त;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = सत्य;
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
    यदि (name.empty())
    {
        CV_LOG_DEBUG(रिक्त, "core(parallel): fallback on builtin code");
    }
    अन्यथा
    {
        यदि (!isKnown)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        अन्यथा
        {
            CV_LOG_INFO(रिक्त, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = सत्य;
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

शून्य setParallelForBackend(ध्रुव std::shared_ptr<ParallelForAPI>& api, तार्किक propagate_अंक_Threads)
{
    getCurrentParallelForAPI() = api;
    यदि (propagate_अंक_Threads && api)
    {
        set_अंक_Threads(अंक_Threads);
    }
}

तार्किक setParallelForBackend(ध्रुव std::string& backendName, तार्किक propagate_अंक_Threads)
{
    CV_TRACE_FUNCTION();

    तार्किक saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    यदि (g_initializedParallelForAPI)
    {
        // ... already initialized
        यदि (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            प्रत्यागम सत्य;
        }
        अन्यथा
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(रिक्त, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    अन्यथा
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    यदि (!api)
    {
        यदि (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            यदि (saved_initialized)
            {
                CV_LOG_WARNING(रिक्त, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            अन्यथा
            {
                CV_LOG_WARNING(रिक्त, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = असत्य;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            प्रत्यागम असत्य;
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): switched to builtin code (legacy)");
        }
    }
    यदि (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    यदि (propagate_अंक_Threads)
    {
        set_अंक_Threads(अंक_Threads);
    }
    प्रत्यागम सत्य;
}

}}  // namespace
