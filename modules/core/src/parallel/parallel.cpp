// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#समावेश "../precomp.hpp"
#समावेश "parallel.hpp"

#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश <opencv2/core/utils/logger.defines.hpp>
#अगर_घोषित न_संशोधन
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्यथा
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#पूर्ण_अगर
#समावेश <opencv2/core/utils/logger.hpp>


#समावेश "registry_parallel.hpp"
#समावेश "registry_parallel.impl.hpp"

#समावेश "plugin_parallel_api.hpp"
#समावेश "plugin_parallel_wrapper.impl.hpp"


namespace cv { namespace parallel {

पूर्णांक अंक_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

अटल
std::string& getParallelBackendName()
{
    अटल std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    वापस g_backendName;
}

अटल bool g_initializedParallelForAPI = false;

अटल
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    स्थिर std::string& name = getParallelBackendName();
    bool isKnown = false;
    स्थिर स्वतः& backends = getParallelBackendsInfo();
    अगर (!name.empty())
    {
        CV_LOG_INFO(शून्य, "core(parallel): requested backend name: " << name);
    }
    क्रम (माप_प्रकार i = 0; i < backends.size(); i++)
    {
        स्थिर स्वतः& info = backends[i];
        अगर (!name.empty())
        {
            अगर (name != info.name)
            {
                जारी;
            }
            isKnown = true;
        }
        प्रयास
        {
            CV_LOG_DEBUG(शून्य, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            अगर (!info.backendFactory)
            {
                CV_LOG_DEBUG(शून्य, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                जारी;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            अगर (!backend)
            {
                CV_LOG_DEBUG(शून्य, "core(parallel): not available: " << info.name);
                जारी;
            }
            CV_LOG_INFO(शून्य, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = true;
            getParallelBackendName() = info.name;
            वापस backend;
        }
        पकड़ो (स्थिर std::exception& e)
        {
            CV_LOG_WARNING(शून्य, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        पकड़ो (...)
        {
            CV_LOG_WARNING(शून्य, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    अगर (name.empty())
    {
        CV_LOG_DEBUG(शून्य, "core(parallel): fallback on builtin code");
    }
    अन्यथा
    {
        अगर (!isKnown)
        {
            CV_LOG_INFO(शून्य, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        अन्यथा
        {
            CV_LOG_INFO(शून्य, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = true;
    getParallelBackendName() = std::string();
    वापस std::shared_ptr<ParallelForAPI>();
}

अटल अंतरभूत
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(शून्य, "core(parallel): Initializing parallel backend...");
    वापस createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    अटल std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    वापस g_currentParallelForAPI;
}

व्योम setParallelForBackend(स्थिर std::shared_ptr<ParallelForAPI>& api, bool propagate_अंक_Threads)
{
    getCurrentParallelForAPI() = api;
    अगर (propagate_अंक_Threads && api)
    {
        set_अंक_Threads(अंक_Threads);
    }
}

bool setParallelForBackend(स्थिर std::string& backendName, bool propagate_अंक_Threads)
{
    CV_TRACE_FUNCTION();

    bool saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    अगर (g_initializedParallelForAPI)
    {
        // ... already initialized
        अगर (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(शून्य, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            वापस true;
        }
        अन्यथा
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(शून्य, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
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
    अगर (!api)
    {
        अगर (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            अगर (saved_initialized)
            {
                CV_LOG_WARNING(शून्य, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            अन्यथा
            {
                CV_LOG_WARNING(शून्य, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = false;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            वापस false;
        }
        अन्यथा
        {
            CV_LOG_WARNING(शून्य, "core(parallel): switched to builtin code (legacy)");
        }
    }
    अगर (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    अगर (propagate_अंक_Threads)
    {
        set_अंक_Threads(अंक_Threads);
    }
    वापस true;
}

}}  // namespace
