// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#അന്തര്ഭാവ "../precomp.hpp"
#അന്തര്ഭാവ "parallel.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#അല്ലെങ്കിൽ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>


#അന്തര്ഭാവ "registry_parallel.hpp"
#അന്തര്ഭാവ "registry_parallel.impl.hpp"

#അന്തര്ഭാവ "plugin_parallel_api.hpp"
#അന്തര്ഭാവ "plugin_parallel_wrapper.impl.hpp"


നാമാകാശ cv { നാമാകാശ parallel {

പൂർണ്ണസംഖ്യ അക്കം_Threads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

സ്ഥിതം
std::string& getParallelBackendName()
{
    സ്ഥിതം std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    തിരികെ g_backendName;
}

സ്ഥിതം തർക്കം g_initializedParallelForAPI = അസത്യം;

സ്ഥിതം
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    സ്ഥിരം std::string& name = getParallelBackendName();
    തർക്കം isKnown = അസത്യം;
    സ്ഥിരം auto& backends = getParallelBackendsInfo();
    എങ്കിൽ (!name.empty())
    {
        CV_LOG_INFO(ശൂന്യ, "core(parallel): requested backend name: " << name);
    }
    വേണ്ടി (size_t i = 0; i < backends.size(); i++)
    {
        സ്ഥിരം auto& info = backends[i];
        എങ്കിൽ (!name.empty())
        {
            എങ്കിൽ (name != info.name)
            {
                തുടരുക;
            }
            isKnown = സത്യം;
        }
        ശ്രമിക്കുക
        {
            CV_LOG_DEBUG(ശൂന്യ, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            എങ്കിൽ (!info.backendFactory)
            {
                CV_LOG_DEBUG(ശൂന്യ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                തുടരുക;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            എങ്കിൽ (!backend)
            {
                CV_LOG_DEBUG(ശൂന്യ, "core(parallel): not available: " << info.name);
                തുടരുക;
            }
            CV_LOG_INFO(ശൂന്യ, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = സത്യം;
            getParallelBackendName() = info.name;
            തിരികെ backend;
        }
        പിടിക്കുക (സ്ഥിരം std::exception& e)
        {
            CV_LOG_WARNING(ശൂന്യ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        പിടിക്കുക (...)
        {
            CV_LOG_WARNING(ശൂന്യ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    എങ്കിൽ (name.empty())
    {
        CV_LOG_DEBUG(ശൂന്യ, "core(parallel): fallback on builtin code");
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (!isKnown)
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = സത്യം;
    getParallelBackendName() = std::string();
    തിരികെ std::shared_ptr<ParallelForAPI>();
}

സ്ഥിതം വരിയിൽ
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ശൂന്യ, "core(parallel): Initializing parallel backend...");
    തിരികെ createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    സ്ഥിതം std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    തിരികെ g_currentParallelForAPI;
}

ശൂന്യം setParallelForBackend(സ്ഥിരം std::shared_ptr<ParallelForAPI>& api, തർക്കം propagate_അക്കം_Threads)
{
    getCurrentParallelForAPI() = api;
    എങ്കിൽ (propagate_അക്കം_Threads && api)
    {
        set_അക്കം_Threads(അക്കം_Threads);
    }
}

തർക്കം setParallelForBackend(സ്ഥിരം std::string& backendName, തർക്കം propagate_അക്കം_Threads)
{
    CV_TRACE_FUNCTION();

    തർക്കം saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    എങ്കിൽ (g_initializedParallelForAPI)
    {
        // ... already initialized
        എങ്കിൽ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            തിരികെ സത്യം;
        }
        അല്ലെങ്കിൽ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ശൂന്യ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    അല്ലെങ്കിൽ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    എങ്കിൽ (!api)
    {
        എങ്കിൽ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            എങ്കിൽ (saved_initialized)
            {
                CV_LOG_WARNING(ശൂന്യ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_WARNING(ശൂന്യ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = അസത്യം;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            തിരികെ അസത്യം;
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_WARNING(ശൂന്യ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    എങ്കിൽ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    എങ്കിൽ (propagate_അക്കം_Threads)
    {
        set_അക്കം_Threads(അക്കം_Threads);
    }
    തിരികെ സത്യം;
}

}}  // namespace
