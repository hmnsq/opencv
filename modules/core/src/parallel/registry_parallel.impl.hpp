// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#شامل "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

فضاینام cv { فضاینام parallel {

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#وگرنه
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { بازگشت createBackendAPI(); }) \
},

ایستا
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ایستا std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#وگرنهاگر defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#وگرنهاگر defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    بازگشت g_backends;
}

ایستا
منطقی sortByPriority(ثابت ParallelBackendInfo &lhs, ثابت ParallelBackendInfo &rhs)
{
    بازگشت lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
رده ParallelBackendRegistry
{
محافظتشده:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        صحیح N = (صحیح)enabledBackends.size();
        برای (صحیح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(پوچ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        اگر (readPrioritySettings())
        {
            CV_LOG_INFO(پوچ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (صحیح)enabledBackends.size();
        }
        صحیح enabled = 0;
        برای (صحیح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            اگر (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(صحیح)param_priority); // overflow check
            اگر (param_priority > 0)
            {
                info.priority = (صحیح)param_priority;
                enabled++;
            }
            وگرنه
            {
                CV_LOG_INFO(پوچ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(پوچ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(پوچ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ایستا std::vector<std::string> tokenize_string(ثابت std::string& درون_داد_2, نویسه token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        وقتی((pos = درون_داد_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(درون_داد_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(درون_داد_2.substr(prev_pos));
        بازگشت result;
    }
    منطقی readPrioritySettings()
    {
        منطقی hasChanges = نادرست;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        اگر (prioritized_backends.empty())
            بازگشت hasChanges;
        CV_LOG_INFO(پوچ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ثابت std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        برای (size_t i = 0; i < names.size(); i++)
        {
            ثابت std::string& name = names[i];
            صحیح priority = (صحیح)(100000 + (names.size() - i) * 1000);
            منطقی found = نادرست;
            برای (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                اگر (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(پوچ, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = درست;
                    hasChanges = درست;
                    بشکن;
                }
            }
            اگر (!found)
            {
                CV_LOG_INFO(پوچ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = درست;
            }
        }
        بازگشت hasChanges;
    }
عمومی:
    std::string dumpBackends() ثابت
    {
        std::ostringstream os;
        برای (size_t i = 0; i < enabledBackends.size(); i++)
        {
            اگر (i > 0) os << "; ";
            ثابت ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        بازگشت os.str();
    }

    ایستا ParallelBackendRegistry& getInstance()
    {
        ایستا ParallelBackendRegistry g_instance;
        بازگشت g_instance;
    }

    درونخطی ثابت std::vector<ParallelBackendInfo>& getEnabledBackends() ثابت { بازگشت enabledBackends; }
};


ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    بازگشت cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
