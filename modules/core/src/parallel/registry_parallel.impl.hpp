// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ಅನ್ಯಥಾ
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ಪ್ರತ್ಯಾಗಮ createBackendAPI(); }) \
},

ಸ್ಥಿರ
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ಸ್ಥಿರ std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#ನೋಚೇದ್ಯದಿ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#ನೋಚೇದ್ಯದಿ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    ಪ್ರತ್ಯಾಗಮ g_backends;
}

ಸ್ಥಿರ
ತಾರ್ಕಿಕ sortByPriority(ಧ್ರುವ ParallelBackendInfo &lhs, ಧ್ರುವ ParallelBackendInfo &rhs)
{
    ಪ್ರತ್ಯಾಗಮ lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
ವರ್ಗ ParallelBackendRegistry
{
ರಕ್ಷಿತ:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        ಪೂರ್ಣಾಙ್ಕ N = (ಪೂರ್ಣಾಙ್ಕ)enabledBackends.size();
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ಯದಿ (readPrioritySettings())
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (ಪೂರ್ಣಾಙ್ಕ)enabledBackends.size();
        }
        ಪೂರ್ಣಾಙ್ಕ enabled = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ಯದಿ (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(ಪೂರ್ಣಾಙ್ಕ)param_priority); // overflow check
            ಯದಿ (param_priority > 0)
            {
                info.priority = (ಪೂರ್ಣಾಙ್ಕ)param_priority;
                enabled++;
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ಸ್ಥಿರ std::vector<std::string> tokenize_string(ಧ್ರುವ std::string& ಒಳಹರಿವು_2, ವರ್ಣ token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        ಯಾವತ್((pos = ಒಳಹರಿವು_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(ಒಳಹರಿವು_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(ಒಳಹರಿವು_2.substr(prev_pos));
        ಪ್ರತ್ಯಾಗಮ result;
    }
    ತಾರ್ಕಿಕ readPrioritySettings()
    {
        ತಾರ್ಕಿಕ hasChanges = ಅಸತ್ಯ;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ಯದಿ (prioritized_backends.empty())
            ಪ್ರತ್ಯಾಗಮ hasChanges;
        CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ಧ್ರುವ std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        ಪ್ರತಿ (size_t i = 0; i < names.size(); i++)
        {
            ಧ್ರುವ std::string& name = names[i];
            ಪೂರ್ಣಾಙ್ಕ priority = (ಪೂರ್ಣಾಙ್ಕ)(100000 + (names.size() - i) * 1000);
            ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
            ಪ್ರತಿ (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ಯದಿ (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = ಸತ್ಯ;
                    hasChanges = ಸತ್ಯ;
                    ಭಙ್ಗ;
                }
            }
            ಯದಿ (!found)
            {
                CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = ಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ hasChanges;
    }
ಸಾರ್ವಜನಿಕ:
    std::string dumpBackends() ಧ್ರುವ
    {
        std::ostringstream os;
        ಪ್ರತಿ (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ಯದಿ (i > 0) os << "; ";
            ಧ್ರುವ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ಪ್ರತ್ಯಾಗಮ os.str();
    }

    ಸ್ಥಿರ ParallelBackendRegistry& getInstance()
    {
        ಸ್ಥಿರ ParallelBackendRegistry g_instance;
        ಪ್ರತ್ಯಾಗಮ g_instance;
    }

    ಅನ್ತಃಸ್ಥ ಧ್ರುವ std::vector<ParallelBackendInfo>& getEnabledBackends() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ enabledBackends; }
};


ಧ್ರುವ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ಪ್ರತ್ಯಾಗಮ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
