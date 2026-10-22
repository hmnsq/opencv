// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"
#ضمن "parallel.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/logger.defines.hpp>
#که_تعریف_شوی NDEBUG
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#د_که_پای
#ضمن <opencv2/core/utils/logger.hpp>


#ضمن "registry_parallel.hpp"
#ضمن "registry_parallel.impl.hpp"

#ضمن "plugin_parallel_api.hpp"
#ضمن "plugin_parallel_wrapper.impl.hpp"


نطاق cv { نطاق parallel {

صحيح عدد_تارونه = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ساكن
std::string& getParallelBackendName()
{
    ساكن std::string g_backendName = منزل_پاسنی_Case(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    أرجع g_backendName;
}

ساكن منطقي g_initializedParallelForAPI = خطأ;

ساكن
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ثابت std::string& name = getParallelBackendName();
    منطقي is_معلوم = خطأ;
    ثابت خپلکاری& backends = getParallelBackendsInfo();
    إذا (!name.empty())
    {
        CV_LOG_INFO(عدم, "core(parallel): requested backend name: " << name);
    }
    لكل (size_t i = 0; i < backends.size(); i++)
    {
        ثابت خپلکاری& info = backends[i];
        إذا (!name.empty())
        {
            إذا (name != info.name)
            {
                استمر;
            }
            is_معلوم = ریښتیا;
        }
        حاول
        {
            CV_LOG_DEBUG(عدم, "core(parallel): trying backend: " << info.name << " (priority=" << info.لومړیتوب << ")");
            إذا (!info.backendFactory)
            {
                CV_LOG_DEBUG(عدم, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                استمر;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            إذا (!backend)
            {
                CV_LOG_DEBUG(عدم, "core(parallel): not available: " << info.name);
                استمر;
            }
            CV_LOG_INFO(عدم, "core(parallel): using backend: " << info.name << " (priority=" << info.لومړیتوب << ")");
            g_initializedParallelForAPI = ریښتیا;
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
    إذا (name.empty())
    {
        CV_LOG_DEBUG(عدم, "core(parallel): fallback on builtin code");
    }
    وإلا
    {
        إذا (!is_معلوم)
        {
            CV_LOG_INFO(عدم, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = ریښتیا;
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

فراغ سېټ_Parallel_For_Backend(ثابت std::shared_ptr<ParallelForAPI>& api, منطقي propagate_عدد_تارونه)
{
    getCurrentParallelForAPI() = api;
    إذا (propagate_عدد_تارونه && api)
    {
        سېټ_عدد_تارونه(عدد_تارونه);
    }
}

منطقي سېټ_Parallel_For_Backend(ثابت std::string& backendName, منطقي propagate_عدد_تارونه)
{
    CV_TRACE_FUNCTION();

    منطقي saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = منزل_پاسنی_Case(backendName);
    إذا (g_initializedParallelForAPI)
    {
        // ... already initialized
        إذا (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(عدم, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            أرجع ریښتیا;
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
    إذا (!api)
    {
        إذا (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            إذا (saved_initialized)
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
    إذا (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    إذا (propagate_عدد_تارونه)
    {
        سېټ_عدد_تارونه(عدد_تارونه);
    }
    أرجع ریښتیا;
}

}}  // namespace
