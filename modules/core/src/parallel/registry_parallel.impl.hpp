// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#அந்தர்பாவ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

நாமாகாஶ cv { நாமாகாஶ parallel {

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#இல்லையெனில்
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { திரும்பு createBackendAPI(); }) \
},

நிலையான
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    நிலையான std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#அல்லதெனில் defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#அல்லதெனில் defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    திரும்பு g_backends;
}

நிலையான
தருக்க sortByPriority(மாறிலி ParallelBackendInfo &lhs, மாறிலி ParallelBackendInfo &rhs)
{
    திரும்பு lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
வகுப்பு ParallelBackendRegistry
{
பாதுகாக்கப்பட்ட:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        முழு N = (முழு)enabledBackends.size();
        க்கு (முழு i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(வெற்று, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        எனில் (readPrioritySettings())
        {
            CV_LOG_INFO(வெற்று, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (முழு)enabledBackends.size();
        }
        முழு enabled = 0;
        க்கு (முழு i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            எனில் (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(முழு)param_priority); // overflow check
            எனில் (param_priority > 0)
            {
                info.priority = (முழு)param_priority;
                enabled++;
            }
            இல்லையெனில்
            {
                CV_LOG_INFO(வெற்று, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(வெற்று, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(வெற்று, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    நிலையான std::vector<std::string> tokenize_string(மாறிலி std::string& உள்ளீடு_2, எழுத்து token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        போது((pos = உள்ளீடு_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(உள்ளீடு_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(உள்ளீடு_2.substr(prev_pos));
        திரும்பு result;
    }
    தருக்க readPrioritySettings()
    {
        தருக்க hasChanges = பொய்;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        எனில் (prioritized_backends.empty())
            திரும்பு hasChanges;
        CV_LOG_INFO(வெற்று, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        மாறிலி std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        க்கு (size_t i = 0; i < names.size(); i++)
        {
            மாறிலி std::string& name = names[i];
            முழு priority = (முழு)(100000 + (names.size() - i) * 1000);
            தருக்க found = பொய்;
            க்கு (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                எனில் (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(வெற்று, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = உண்மை;
                    hasChanges = உண்மை;
                    நிறுத்து;
                }
            }
            எனில் (!found)
            {
                CV_LOG_INFO(வெற்று, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = உண்மை;
            }
        }
        திரும்பு hasChanges;
    }
பொது:
    std::string dumpBackends() மாறிலி
    {
        std::ostringstream os;
        க்கு (size_t i = 0; i < enabledBackends.size(); i++)
        {
            எனில் (i > 0) os << "; ";
            மாறிலி ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        திரும்பு os.str();
    }

    நிலையான ParallelBackendRegistry& getInstance()
    {
        நிலையான ParallelBackendRegistry g_instance;
        திரும்பு g_instance;
    }

    உள்வரி மாறிலி std::vector<ParallelBackendInfo>& getEnabledBackends() மாறிலி { திரும்பு enabledBackends; }
};


மாறிலி std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    திரும்பு cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
