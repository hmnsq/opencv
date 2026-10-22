// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "parallel.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#ifdef न_संशोधन
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

पूर्णांक अंक_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

स्थिर
std::string& getParallelBackendName()
{
    स्थिर std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    परत g_backendName;
}

स्थिर तार्किक g_initializedParallelForAPI = असत्य;

स्थिर
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    स्थिरांक std::string& name = getParallelBackendName();
    तार्किक isKnown = असत्य;
    स्थिरांक स्वतः& backends = getParallelBackendsInfo();
    जर (!name.empty())
    {
        CV_LOG_INFO(रिक्त, "core(parallel): requested backend name: " << name);
    }
    साठी (माप_प्रकार i = 0; i < backends.size(); i++)
    {
        स्थिरांक स्वतः& info = backends[i];
        जर (!name.empty())
        {
            जर (name != info.name)
            {
                सुरूठेवा;
            }
            isKnown = सत्य;
        }
        प्रयत्न
        {
            CV_LOG_DEBUG(रिक्त, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            जर (!info.backendFactory)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                सुरूठेवा;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            जर (!backend)
            {
                CV_LOG_DEBUG(रिक्त, "core(parallel): not available: " << info.name);
                सुरूठेवा;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = सत्य;
            getParallelBackendName() = info.name;
            परत backend;
        }
        पकडा (स्थिरांक std::exception& e)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        पकडा (...)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    जर (name.empty())
    {
        CV_LOG_DEBUG(रिक्त, "core(parallel): fallback on builtin code");
    }
    अन्यथा
    {
        जर (!isKnown)
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
    परत std::shared_ptr<ParallelForAPI>();
}

स्थिर ओळीत
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(रिक्त, "core(parallel): Initializing parallel backend...");
    परत createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    स्थिर std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    परत g_currentParallelForAPI;
}

शून्य setParallelForBackend(स्थिरांक std::shared_ptr<ParallelForAPI>& api, तार्किक propagate_अंक_Threads)
{
    getCurrentParallelForAPI() = api;
    जर (propagate_अंक_Threads && api)
    {
        set_अंक_Threads(अंक_Threads);
    }
}

तार्किक setParallelForBackend(स्थिरांक std::string& backendName, तार्किक propagate_अंक_Threads)
{
    CV_TRACE_FUNCTION();

    तार्किक saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    जर (g_initializedParallelForAPI)
    {
        // ... already initialized
        जर (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            परत सत्य;
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
    जर (!api)
    {
        जर (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            जर (saved_initialized)
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
            परत असत्य;
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): switched to builtin code (legacy)");
        }
    }
    जर (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    जर (propagate_अंक_Threads)
    {
        set_अंक_Threads(अंक_Threads);
    }
    परत सत्य;
}

}}  // namespace
