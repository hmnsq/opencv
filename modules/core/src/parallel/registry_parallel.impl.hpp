// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

नामाकाश cv { नामाकाश parallel {

#जर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#अन्यथा
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { परत createBackendAPI(); }) \
},

स्थिर
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    स्थिर std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#किंवाजर defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#किंवाजर defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    परत g_backends;
}

स्थिर
तार्किक sortByPriority(स्थिरांक ParallelBackendInfo &lhs, स्थिरांक ParallelBackendInfo &rhs)
{
    परत lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
वर्ग ParallelBackendRegistry
{
संरक्षित:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        पूर्णांक N = (पूर्णांक)enabledBackends.size();
        साठी (पूर्णांक i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(रिक्त, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        जर (readPrioritySettings())
        {
            CV_LOG_INFO(रिक्त, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (पूर्णांक)enabledBackends.size();
        }
        पूर्णांक enabled = 0;
        साठी (पूर्णांक i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            जर (enabled != i)
                info = enabledBackends[i];
            माप_प्रकार param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (माप_प्रकार)info.priority);
            CV_Assert(param_priority == (माप_प्रकार)(पूर्णांक)param_priority); // overflow check
            जर (param_priority > 0)
            {
                info.priority = (पूर्णांक)param_priority;
                enabled++;
            }
            अन्यथा
            {
                CV_LOG_INFO(रिक्त, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(रिक्त, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(रिक्त, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    स्थिर std::vector<std::string> tokenize_string(स्थिरांक std::string& आदान_2, अक्षर token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        पर्यंत((pos = आदान_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(आदान_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(आदान_2.substr(prev_pos));
        परत result;
    }
    तार्किक readPrioritySettings()
    {
        तार्किक hasChanges = असत्य;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        जर (prioritized_backends.empty())
            परत hasChanges;
        CV_LOG_INFO(रिक्त, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        स्थिरांक std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        साठी (माप_प्रकार i = 0; i < names.size(); i++)
        {
            स्थिरांक std::string& name = names[i];
            पूर्णांक priority = (पूर्णांक)(100000 + (names.size() - i) * 1000);
            तार्किक found = असत्य;
            साठी (माप_प्रकार k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                जर (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(रिक्त, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = सत्य;
                    hasChanges = सत्य;
                    थांबा;
                }
            }
            जर (!found)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = सत्य;
            }
        }
        परत hasChanges;
    }
सार्वजनिक:
    std::string dumpBackends() स्थिरांक
    {
        std::ostringstream os;
        साठी (माप_प्रकार i = 0; i < enabledBackends.size(); i++)
        {
            जर (i > 0) os << "; ";
            स्थिरांक ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        परत os.str();
    }

    स्थिर ParallelBackendRegistry& getInstance()
    {
        स्थिर ParallelBackendRegistry g_instance;
        परत g_instance;
    }

    ओळीत स्थिरांक std::vector<ParallelBackendInfo>& getEnabledBackends() स्थिरांक { परत enabledBackends; }
};


स्थिरांक std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    परत cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
