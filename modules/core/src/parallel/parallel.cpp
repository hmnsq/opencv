// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "../precomp.hpp"
#شامل "parallel.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/logger.defines.hpp>
#اگر_تعریف_شدہ NDEBUG
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ورنہ
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#اگر_ختم
#شامل <opencv2/core/utils/logger.hpp>


#شامل "registry_parallel.hpp"
#شامل "registry_parallel.impl.hpp"

#شامل "plugin_parallel_api.hpp"
#شامل "plugin_parallel_wrapper.impl.hpp"


نامخلا cv { نامخلا parallel {

صحیح عدد_دھاگے = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

ساکن
std::string& getParallelBackendName()
{
    ساکن std::string g_backendName = مقصد_بالائی_Case(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    واپس g_backendName;
}

ساکن منطقی g_initializedParallelForAPI = جھوٹ;

ساکن
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    ثابت std::string& name = getParallelBackendName();
    منطقی is_معلوم = جھوٹ;
    ثابت خودکار& backends = getParallelBackendsInfo();
    اگر (!name.empty())
    {
        CV_LOG_INFO(تہی, "core(parallel): requested backend name: " << name);
    }
    برائے (size_t i = 0; i < backends.size(); i++)
    {
        ثابت خودکار& info = backends[i];
        اگر (!name.empty())
        {
            اگر (name != info.name)
            {
                جاری;
            }
            is_معلوم = سچ;
        }
        کوشش
        {
            CV_LOG_DEBUG(تہی, "core(parallel): trying backend: " << info.name << " (priority=" << info.ترجیح << ")");
            اگر (!info.backendFactory)
            {
                CV_LOG_DEBUG(تہی, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                جاری;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            اگر (!backend)
            {
                CV_LOG_DEBUG(تہی, "core(parallel): not available: " << info.name);
                جاری;
            }
            CV_LOG_INFO(تہی, "core(parallel): using backend: " << info.name << " (priority=" << info.ترجیح << ")");
            g_initializedParallelForAPI = سچ;
            getParallelBackendName() = info.name;
            واپس backend;
        }
        پکڑو (ثابت std::exception& e)
        {
            CV_LOG_WARNING(تہی, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        پکڑو (...)
        {
            CV_LOG_WARNING(تہی, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    اگر (name.empty())
    {
        CV_LOG_DEBUG(تہی, "core(parallel): fallback on builtin code");
    }
    ورنہ
    {
        اگر (!is_معلوم)
        {
            CV_LOG_INFO(تہی, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        ورنہ
        {
            CV_LOG_INFO(تہی, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = سچ;
    getParallelBackendName() = std::string();
    واپس std::shared_ptr<ParallelForAPI>();
}

ساکن اندرونی
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(تہی, "core(parallel): Initializing parallel backend...");
    واپس createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    ساکن std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    واپس g_currentParallelForAPI;
}

خالی سیٹ_Parallel_For_Backend(ثابت std::shared_ptr<ParallelForAPI>& api, منطقی propagate_عدد_دھاگے)
{
    getCurrentParallelForAPI() = api;
    اگر (propagate_عدد_دھاگے && api)
    {
        سیٹ_عدد_دھاگے(عدد_دھاگے);
    }
}

منطقی سیٹ_Parallel_For_Backend(ثابت std::string& backendName, منطقی propagate_عدد_دھاگے)
{
    CV_TRACE_FUNCTION();

    منطقی saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = مقصد_بالائی_Case(backendName);
    اگر (g_initializedParallelForAPI)
    {
        // ... already initialized
        اگر (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(تہی, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            واپس سچ;
        }
        ورنہ
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(تہی, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    ورنہ
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    اگر (!api)
    {
        اگر (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            اگر (saved_initialized)
            {
                CV_LOG_WARNING(تہی, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            ورنہ
            {
                CV_LOG_WARNING(تہی, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = جھوٹ;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            واپس جھوٹ;
        }
        ورنہ
        {
            CV_LOG_WARNING(تہی, "core(parallel): switched to builtin code (legacy)");
        }
    }
    اگر (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    اگر (propagate_عدد_دھاگے)
    {
        سیٹ_عدد_دھاگے(عدد_دھاگے);
    }
    واپس سچ;
}

}}  // namespace
