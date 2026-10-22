// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

नामाकाश cv { नामाकाश parallel {

#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#अञ्ञथा
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { पटिनिवत्त createBackendAPI(); }) \
},

स्थिर
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    स्थिर std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#नोचेद्यदि defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#नोचेद्यदि defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    पटिनिवत्त g_backends;
}

स्थिर
तार्किक sortByPriority(ध्रुव ParallelBackendInfo &lhs, ध्रुव ParallelBackendInfo &rhs)
{
    पटिनिवत्त lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
वग्ग ParallelBackendRegistry
{
रक्षित:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        पूर्णाङ्क N = (पूर्णाङ्क)enabledBackends.size();
        अत्थाय (पूर्णाङ्क i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(रिक्त, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        सचे (readPrioritySettings())
        {
            CV_LOG_INFO(रिक्त, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (पूर्णाङ्क)enabledBackends.size();
        }
        पूर्णाङ्क enabled = 0;
        अत्थाय (पूर्णाङ्क i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            सचे (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(पूर्णाङ्क)param_priority); // overflow check
            सचे (param_priority > 0)
            {
                info.priority = (पूर्णाङ्क)param_priority;
                enabled++;
            }
            अञ्ञथा
            {
                CV_LOG_INFO(रिक्त, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(रिक्त, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(रिक्त, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    स्थिर std::vector<std::string> tokenize_string(ध्रुव std::string& निवेशः_2, वर्ण token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        याव((pos = निवेशः_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(निवेशः_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(निवेशः_2.substr(prev_pos));
        पटिनिवत्त result;
    }
    तार्किक readPrioritySettings()
    {
        तार्किक hasChanges = मिच्छा;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        सचे (prioritized_backends.empty())
            पटिनिवत्त hasChanges;
        CV_LOG_INFO(रिक्त, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ध्रुव std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        अत्थाय (size_t i = 0; i < names.size(); i++)
        {
            ध्रुव std::string& name = names[i];
            पूर्णाङ्क priority = (पूर्णाङ्क)(100000 + (names.size() - i) * 1000);
            तार्किक found = मिच्छा;
            अत्थाय (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                सचे (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(रिक्त, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = सच्च;
                    hasChanges = सच्च;
                    भङ्ग;
                }
            }
            सचे (!found)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = सच्च;
            }
        }
        पटिनिवत्त hasChanges;
    }
सार्वजनिक:
    std::string dumpBackends() ध्रुव
    {
        std::ostringstream os;
        अत्थाय (size_t i = 0; i < enabledBackends.size(); i++)
        {
            सचे (i > 0) os << "; ";
            ध्रुव ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        पटिनिवत्त os.str();
    }

    स्थिर ParallelBackendRegistry& getInstance()
    {
        स्थिर ParallelBackendRegistry g_instance;
        पटिनिवत्त g_instance;
    }

    अन्तःस्थ ध्रुव std::vector<ParallelBackendInfo>& getEnabledBackends() ध्रुव { पटिनिवत्त enabledBackends; }
};


ध्रुव std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    पटिनिवत्त cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
