// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#অন্তর্ভাৱ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

নামাকাশ cv { নামাকাশ parallel {

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#অন্যথা
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ঘূৰাই createBackendAPI(); }) \
},

স্থির
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    স্থির std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#নোচেদ্যদি defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#নোচেদ্যদি defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    ঘূৰাই g_backends;
}

স্থির
তার্কিক sortByPriority(ধ্রুৱ ParallelBackendInfo &lhs, ধ্রুৱ ParallelBackendInfo &rhs)
{
    ঘূৰাই lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
শ্ৰেণী ParallelBackendRegistry
{
রক্ষিত:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        পূর্ণাঙ্ক N = (পূর্ণাঙ্ক)enabledBackends.size();
        বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(রিক্ত, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        যদি (readPrioritySettings())
        {
            CV_LOG_INFO(রিক্ত, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (পূর্ণাঙ্ক)enabledBackends.size();
        }
        পূর্ণাঙ্ক enabled = 0;
        বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            যদি (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(পূর্ণাঙ্ক)param_priority); // overflow check
            যদি (param_priority > 0)
            {
                info.priority = (পূর্ণাঙ্ক)param_priority;
                enabled++;
            }
            অন্যথা
            {
                CV_LOG_INFO(রিক্ত, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(রিক্ত, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(রিক্ত, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    স্থির std::vector<std::string> tokenize_string(ধ্রুৱ std::string& প্ৰবেশ_2, ৱর্ণ token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        যেতিয়ালৈ((pos = প্ৰবেশ_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(প্ৰবেশ_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(প্ৰবেশ_2.substr(prev_pos));
        ঘূৰাই result;
    }
    তার্কিক readPrioritySettings()
    {
        তার্কিক hasChanges = মিছা;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        যদি (prioritized_backends.empty())
            ঘূৰাই hasChanges;
        CV_LOG_INFO(রিক্ত, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ধ্রুৱ std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        বাবে (size_t i = 0; i < names.size(); i++)
        {
            ধ্রুৱ std::string& name = names[i];
            পূর্ণাঙ্ক priority = (পূর্ণাঙ্ক)(100000 + (names.size() - i) * 1000);
            তার্কিক found = মিছা;
            বাবে (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                যদি (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(রিক্ত, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = সঁচা;
                    hasChanges = সঁচা;
                    ভঙ্গ;
                }
            }
            যদি (!found)
            {
                CV_LOG_INFO(রিক্ত, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = সঁচা;
            }
        }
        ঘূৰাই hasChanges;
    }
সার্ৱজনিক:
    std::string dumpBackends() ধ্রুৱ
    {
        std::ostringstream os;
        বাবে (size_t i = 0; i < enabledBackends.size(); i++)
        {
            যদি (i > 0) os << "; ";
            ধ্রুৱ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ঘূৰাই os.str();
    }

    স্থির ParallelBackendRegistry& getInstance()
    {
        স্থির ParallelBackendRegistry g_instance;
        ঘূৰাই g_instance;
    }

    অন্তঃস্থ ধ্রুৱ std::vector<ParallelBackendInfo>& getEnabledBackends() ধ্রুৱ { ঘূৰাই enabledBackends; }
};


ধ্রুৱ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ঘূৰাই cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
