// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#અન્તર્ભાવ "../precomp.hpp"
#અન્તર્ભાવ "parallel.hpp"

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#નહીંતર
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>


#અન્તર્ભાવ "registry_parallel.hpp"
#અન્તર્ભાવ "registry_parallel.impl.hpp"

#અન્તર્ભાવ "plugin_parallel_api.hpp"
#અન્તર્ભાવ "plugin_parallel_wrapper.impl.hpp"


નામાકાશ cv { નામાકાશ parallel {

પૂર્ણાંક અંક_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

સ્થિર
std::string& getParallelBackendName()
{
    સ્થિર std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    પાછા g_backendName;
}

સ્થિર તાર્કિક g_initializedParallelForAPI = ખોટું;

સ્થિર
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    અચલ std::string& name = getParallelBackendName();
    તાર્કિક isKnown = ખોટું;
    અચલ auto& backends = getParallelBackendsInfo();
    જો (!name.empty())
    {
        CV_LOG_INFO(ખાલી, "core(parallel): requested backend name: " << name);
    }
    માટે (size_t i = 0; i < backends.size(); i++)
    {
        અચલ auto& info = backends[i];
        જો (!name.empty())
        {
            જો (name != info.name)
            {
                ચાલુ;
            }
            isKnown = સાચું;
        }
        પ્રયાસ
        {
            CV_LOG_DEBUG(ખાલી, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            જો (!info.backendFactory)
            {
                CV_LOG_DEBUG(ખાલી, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                ચાલુ;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            જો (!backend)
            {
                CV_LOG_DEBUG(ખાલી, "core(parallel): not available: " << info.name);
                ચાલુ;
            }
            CV_LOG_INFO(ખાલી, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = સાચું;
            getParallelBackendName() = info.name;
            પાછા backend;
        }
        પકડો (અચલ std::exception& e)
        {
            CV_LOG_WARNING(ખાલી, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        પકડો (...)
        {
            CV_LOG_WARNING(ખાલી, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    જો (name.empty())
    {
        CV_LOG_DEBUG(ખાલી, "core(parallel): fallback on builtin code");
    }
    નહીંતર
    {
        જો (!isKnown)
        {
            CV_LOG_INFO(ખાલી, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        નહીંતર
        {
            CV_LOG_INFO(ખાલી, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = સાચું;
    getParallelBackendName() = std::string();
    પાછા std::shared_ptr<ParallelForAPI>();
}

સ્થિર અંતઃરેખ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ખાલી, "core(parallel): Initializing parallel backend...");
    પાછા createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    સ્થિર std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    પાછા g_currentParallelForAPI;
}

શૂન્ય setParallelForBackend(અચલ std::shared_ptr<ParallelForAPI>& api, તાર્કિક propagate_અંક_Threads)
{
    getCurrentParallelForAPI() = api;
    જો (propagate_અંક_Threads && api)
    {
        set_અંક_Threads(અંક_Threads);
    }
}

તાર્કિક setParallelForBackend(અચલ std::string& backendName, તાર્કિક propagate_અંક_Threads)
{
    CV_TRACE_FUNCTION();

    તાર્કિક saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    જો (g_initializedParallelForAPI)
    {
        // ... already initialized
        જો (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ખાલી, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            પાછા સાચું;
        }
        નહીંતર
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ખાલી, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    નહીંતર
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    જો (!api)
    {
        જો (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            જો (saved_initialized)
            {
                CV_LOG_WARNING(ખાલી, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            નહીંતર
            {
                CV_LOG_WARNING(ખાલી, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = ખોટું;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            પાછા ખોટું;
        }
        નહીંતર
        {
            CV_LOG_WARNING(ખાલી, "core(parallel): switched to builtin code (legacy)");
        }
    }
    જો (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    જો (propagate_અંક_Threads)
    {
        set_અંક_Threads(અંક_Threads);
    }
    પાછા સાચું;
}

}}  // namespace
