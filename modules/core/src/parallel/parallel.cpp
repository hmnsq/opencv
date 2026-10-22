// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"
#ضمن "parallel.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/logger.defines.hpp>
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ضمن <opencv2/core/utils/logger.hpp>


#ضمن "registry_parallel.hpp"
#ضمن "registry_parallel.impl.hpp"

#ضمن "plugin_parallel_api.hpp"
#ضمن "plugin_parallel_wrapper.impl.hpp"


نطاق cv { نطاق parallel {

صحيح numThreads = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ساكن
std::string& getParallelBackendName()
{
    ساكن std::string g_backendName = toUpperCase(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    أرجع g_backendName;
}

ساكن منطقي g_initializedParallelForAPI = خطأ;

ساكن
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ثابت std::string& name = getParallelBackendName();
    منطقي isKnown = خطأ;
    ثابت auto& backends = getParallelBackendsInfo();
    𐡄𐡍 (!name.empty())
    {
        CV_LOG_INFO(عدم, "core(parallel): requested backend name: " << name);
    }
    لكل (size_t i = 0; i < backends.size(); i++)
    {
        ثابت auto& info = backends[i];
        𐡄𐡍 (!name.empty())
        {
            𐡄𐡍 (name != info.name)
            {
                استمر;
            }
            isKnown = 𐡒𐡔𐡈;
        }
        حاول
        {
            CV_LOG_DEBUG(عدم, "core(parallel): trying backend: " << info.name << " (priority=" << info.priority << ")");
            𐡄𐡍 (!info.backendFactory)
            {
                CV_LOG_DEBUG(عدم, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                استمر;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            𐡄𐡍 (!backend)
            {
                CV_LOG_DEBUG(عدم, "core(parallel): not available: " << info.name);
                استمر;
            }
            CV_LOG_INFO(عدم, "core(parallel): using backend: " << info.name << " (priority=" << info.priority << ")");
            g_initializedParallelForAPI = 𐡒𐡔𐡈;
            getParallelBackendName() = info.name;
            أرجع backend;
        }
        امسك (ثابت std::exception& e)
        {
            CV_LOG_WARNING(عدم, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    𐡄𐡍 (name.empty())
    {
        CV_LOG_DEBUG(عدم, "core(parallel): fallback on builtin code");
    }
    وإلا
    {
        𐡄𐡍 (!isKnown)
        {
            CV_LOG_INFO(عدم, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = 𐡒𐡔𐡈;
    getParallelBackendName() = std::string();
    أرجع std::shared_ptr<ParallelForAPI>();
}

ساكن مضمن
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(عدم, "core(parallel): Initializing parallel backend...");
    أرجع createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ساكن std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    أرجع g_currentParallelForAPI;
}

فراغ setParallelForBackend(ثابت std::shared_ptr<ParallelForAPI>& api, منطقي propagateNumThreads)
{
    getCurrentParallelForAPI() = api;
    𐡄𐡍 (propagateNumThreads && api)
    {
        setNumThreads(numThreads);
    }
}

منطقي setParallelForBackend(ثابت std::string& backendName, منطقي propagateNumThreads)
{
    CV_TRACE_FUNCTION();

    منطقي saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = toUpperCase(backendName);
    𐡄𐡍 (g_initializedParallelForAPI)
    {
        // ... already initialized
        𐡄𐡍 (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(عدم, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            أرجع 𐡒𐡔𐡈;
        }
        وإلا
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(عدم, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    وإلا
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    𐡄𐡍 (!api)
    {
        𐡄𐡍 (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            𐡄𐡍 (saved_initialized)
            {
                CV_LOG_WARNING(عدم, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            وإلا
            {
                CV_LOG_WARNING(عدم, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = خطأ;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            أرجع خطأ;
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, "core(parallel): switched to builtin code (legacy)");
        }
    }
    𐡄𐡍 (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    𐡄𐡍 (propagateNumThreads)
    {
        setNumThreads(numThreads);
    }
    أرجع 𐡒𐡔𐡈;
}

}}  // namespace
