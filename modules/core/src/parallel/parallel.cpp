// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#అన్తర్భావ "../precomp.hpp"
#అన్తర్భావ "parallel.hpp"

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచితమైతే NDEBUG
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#లేకపోతే
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అయితే_ముగింపు
#అన్తర్భావ <opencv2/core/utils/logger.hpp>


#అన్తర్భావ "registry_parallel.hpp"
#అన్తర్భావ "registry_parallel.impl.hpp"

#అన్తర్భావ "plugin_parallel_api.hpp"
#అన్తర్భావ "plugin_parallel_wrapper.impl.hpp"


నామాకాశ cv { నామాకాశ parallel {

పూర్ణాంకం అంకె_దారాలు = -1;

ParallelForAPI::~ParallelForAPI()
{
    // nothing
}

స్థిర
std::string& getParallelBackendName()
{
    స్థిర std::string g_backendName = గమ్యం_పై_Case(cv::utils::getConfigurationParameterString("OPENCV_PARALLEL_BACKEND", ""));
    తిరిగి g_backendName;
}

స్థిర తార్కిక g_initializedParallelForAPI = అబద్ధం;

స్థిర
std::shared_ptr<ParallelForAPI> createParallelForAPI()
{
    స్థిరరాశి std::string& name = getParallelBackendName();
    తార్కిక is_తెలిసిన = అబద్ధం;
    స్థిరరాశి స్వయంచాలక& backends = getParallelBackendsInfo();
    ఒకవేళ (!name.empty())
    {
        CV_LOG_INFO(ఖాళీ, "core(parallel): requested backend name: " << name);
    }
    కొరకు (size_t i = 0; i < backends.size(); i++)
    {
        స్థిరరాశి స్వయంచాలక& info = backends[i];
        ఒకవేళ (!name.empty())
        {
            ఒకవేళ (name != info.name)
            {
                కొనసాగు;
            }
            is_తెలిసిన = నిజం;
        }
        ప్రయత్నించు
        {
            CV_LOG_DEBUG(ఖాళీ, "core(parallel): trying backend: " << info.name << " (priority=" << info.ప్రాధాన్యత << ")");
            ఒకవేళ (!info.backendFactory)
            {
                CV_LOG_DEBUG(ఖాళీ, "core(parallel): factory is not available (plugins require filesystem support): " << info.name);
                కొనసాగు;
            }
            std::shared_ptr<ParallelForAPI> backend = info.backendFactory->create();
            ఒకవేళ (!backend)
            {
                CV_LOG_DEBUG(ఖాళీ, "core(parallel): not available: " << info.name);
                కొనసాగు;
            }
            CV_LOG_INFO(ఖాళీ, "core(parallel): using backend: " << info.name << " (priority=" << info.ప్రాధాన్యత << ")");
            g_initializedParallelForAPI = నిజం;
            getParallelBackendName() = info.name;
            తిరిగి backend;
        }
        పట్టుకో (స్థిరరాశి std::exception& e)
        {
            CV_LOG_WARNING(ఖాళీ, "core(parallel): can't initialize " << info.name << " backend: " << e.what());
        }
        పట్టుకో (...)
        {
            CV_LOG_WARNING(ఖాళీ, "core(parallel): can't initialize " << info.name << " backend: Unknown C++ exception");
        }
    }
    ఒకవేళ (name.empty())
    {
        CV_LOG_DEBUG(ఖాళీ, "core(parallel): fallback on builtin code");
    }
    లేకపోతే
    {
        ఒకవేళ (!is_తెలిసిన)
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): unknown backend: " << name << ", fallback on builtin code");
        }
        లేకపోతే
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): backend=" << name << " is not available, fallback on builtin code");
        }
    }
    g_initializedParallelForAPI = నిజం;
    getParallelBackendName() = std::string();
    తిరిగి std::shared_ptr<ParallelForAPI>();
}

స్థిర అంతఃవరుస
std::shared_ptr<ParallelForAPI> createDefaultParallelForAPI()
{
    CV_LOG_DEBUG(ఖాళీ, "core(parallel): Initializing parallel backend...");
    తిరిగి createParallelForAPI();
}

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI()
{
    స్థిర std::shared_ptr<ParallelForAPI> g_currentParallelForAPI = createDefaultParallelForAPI();
    తిరిగి g_currentParallelForAPI;
}

శూన్య సమితి_Parallel_For_Backend(స్థిరరాశి std::shared_ptr<ParallelForAPI>& api, తార్కిక propagate_అంకె_దారాలు)
{
    getCurrentParallelForAPI() = api;
    ఒకవేళ (propagate_అంకె_దారాలు && api)
    {
        సమితి_అంకె_దారాలు(అంకె_దారాలు);
    }
}

తార్కిక సమితి_Parallel_For_Backend(స్థిరరాశి std::string& backendName, తార్కిక propagate_అంకె_దారాలు)
{
    CV_TRACE_FUNCTION();

    తార్కిక saved_initialized = g_initializedParallelForAPI;
    std::string saved_backendName = getParallelBackendName();
    std::shared_ptr<ParallelForAPI> saved_backend;  // don't call getCurrentParallelForAPI() if g_initializedParallelForAPI = false to avoid unnecessary "default" initialization

    std::string backendName_u = గమ్యం_పై_Case(backendName);
    ఒకవేళ (g_initializedParallelForAPI)
    {
        // ... already initialized
        ఒకవేళ (getParallelBackendName() == backendName_u)
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): backend is already activated: " << (backendName.empty() ? "builtin(legacy)" : backendName));
            తిరిగి నిజం;
        }
        లేకపోతే
        {
            saved_backend = getCurrentParallelForAPI();
            // ... re-create new
            CV_LOG_DEBUG(ఖాళీ, "core(parallel): replacing parallel backend (old=" << saved_backendName << " new=" << backendName << ")...");
            getParallelBackendName() = backendName_u;
            getCurrentParallelForAPI() = createParallelForAPI();
        }
    }
    లేకపోతే
    {
        // ... no backend exists, just specify the name (initialization is triggered by getCurrentParallelForAPI() call)
        getParallelBackendName() = backendName_u;
    }
    std::shared_ptr<ParallelForAPI> api = getCurrentParallelForAPI();
    ఒకవేళ (!api)
    {
        ఒకవేళ (!backendName.empty())
        {
            // restore previous backend or build default
            getParallelBackendName() = saved_backendName;
            ఒకవేళ (saved_initialized)
            {
                CV_LOG_WARNING(ఖాళీ, "core(parallel): backend is not available: " << backendName << " (keep previous)");
                getCurrentParallelForAPI() = saved_backend;
            }
            లేకపోతే
            {
                CV_LOG_WARNING(ఖాళీ, "core(parallel): backend is not available: " << backendName << " (use default)");
                g_initializedParallelForAPI = అబద్ధం;
                getCurrentParallelForAPI() = createDefaultParallelForAPI();
            }
            తిరిగి అబద్ధం;
        }
        లేకపోతే
        {
            CV_LOG_WARNING(ఖాళీ, "core(parallel): switched to builtin code (legacy)");
        }
    }
    ఒకవేళ (!backendName_u.empty())
    {
        CV_Assert(backendName_u == getParallelBackendName());  // data race?
    }

    ఒకవేళ (propagate_అంకె_దారాలు)
    {
        సమితి_అంకె_దారాలు(అంకె_దారాలు);
    }
    తిరిగి నిజం;
}

}}  // namespace
