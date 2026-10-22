// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

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
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ফেরত createBackendAPI(); }) \
},

স্থির
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    স্থির std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#নতুবা_যদি defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#নতুবা_যদি defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    ফেরত g_backends;
}

স্থির
যৌক্তিক sortByPriority(ধ্রুবক ParallelBackendInfo &lhs, ধ্রুবক ParallelBackendInfo &rhs)
{
    ফেরত lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
শ্রেণী ParallelBackendRegistry
{
সুরক্ষিত:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        পূর্ণসংখ্যা N = (পূর্ণসংখ্যা)enabledBackends.size();
        জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ফাঁকা, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        যদি (readPrioritySettings())
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (পূর্ণসংখ্যা)enabledBackends.size();
        }
        পূর্ণসংখ্যা enabled = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            যদি (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::get_Configuration_Parameter_আকার_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(পূর্ণসংখ্যা)param_priority); // overflow check
            যদি (param_priority > 0)
            {
                info.priority = (পূর্ণসংখ্যা)param_priority;
                enabled++;
            }
            অন্যথা
            {
                CV_LOG_INFO(ফাঁকা, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ফাঁকা, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(ফাঁকা, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    স্থির std::vector<std::string> tokenize_string(ধ্রুবক std::string& প্রবেশ_2, অক্ষর token)
    {
        std::vector<std::string> ফলাফল;
        std::string::size_type পূর্ব_pos = 0, pos = 0;
        যখন((pos = প্রবেশ_2.find(token, pos)) != std::string::npos)
        {
            ফলাফল.push_back(প্রবেশ_2.substr(পূর্ব_pos, pos-পূর্ব_pos));
            পূর্ব_pos = ++pos;
        }
        ফলাফল.push_back(প্রবেশ_2.substr(পূর্ব_pos));
        ফেরত ফলাফল;
    }
    যৌক্তিক readPrioritySettings()
    {
        যৌক্তিক hasChanges = মিথ্যা;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        যদি (prioritized_backends.empty())
            ফেরত hasChanges;
        CV_LOG_INFO(ফাঁকা, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ধ্রুবক std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        জন্য (size_t i = 0; i < names.size(); i++)
        {
            ধ্রুবক std::string& name = names[i];
            পূর্ণসংখ্যা priority = (পূর্ণসংখ্যা)(100000 + (names.size() - i) * 1000);
            যৌক্তিক found = মিথ্যা;
            জন্য (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                যদি (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(ফাঁকা, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = সত্য;
                    hasChanges = সত্য;
                    বিরতি;
                }
            }
            যদি (!found)
            {
                CV_LOG_INFO(ফাঁকা, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = সত্য;
            }
        }
        ফেরত hasChanges;
    }
সর্বজনীন:
    std::string dumpBackends() ধ্রুবক
    {
        std::ostringstream os;
        জন্য (size_t i = 0; i < enabledBackends.size(); i++)
        {
            যদি (i > 0) os << "; ";
            ধ্রুবক ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ফেরত os.str();
    }

    স্থির ParallelBackendRegistry& getInstance()
    {
        স্থির ParallelBackendRegistry g_instance;
        ফেরত g_instance;
    }

    অন্তঃরেখ ধ্রুবক std::vector<ParallelBackendInfo>& getEnabledBackends() ধ্রুবক { ফেরত enabledBackends; }
};


ধ্রুবক std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ফেরত cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
