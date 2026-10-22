// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ضمن "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق parallel {

#אם OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#אחרת
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { החזר createBackendAPI(); }) \
},

ساكن
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ساكن std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#وإلاإذا defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#وإلاإذا defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    החזר g_backends;
}

ساكن
منطقي sortByPriority(ثابت ParallelBackendInfo &lhs, ثابت ParallelBackendInfo &rhs)
{
    החזר lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
מחלקה ParallelBackendRegistry
{
محمي:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        صحيح N = (صحيح)enabledBackends.size();
        עבור (صحيح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        אם (readPrioritySettings())
        {
            CV_LOG_INFO(عدم, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (صحيح)enabledBackends.size();
        }
        صحيح enabled = 0;
        עבור (صحيح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            אם (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(صحيح)param_priority); // overflow check
            אם (param_priority > 0)
            {
                info.priority = (صحيح)param_priority;
                enabled++;
            }
            אחרת
            {
                CV_LOG_INFO(عدم, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(عدم, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(عدم, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ساكن std::vector<std::string> tokenize_string(ثابت std::string& input, حرف token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        כלעוד((pos = input.find(token, pos)) != std::string::npos)
        {
            result.push_back(input.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(input.substr(prev_pos));
        החזר result;
    }
    منطقي readPrioritySettings()
    {
        منطقي hasChanges = שקר;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        אם (prioritized_backends.empty())
            החזר hasChanges;
        CV_LOG_INFO(عدم, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ثابت std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        עבור (size_t i = 0; i < names.size(); i++)
        {
            ثابت std::string& name = names[i];
            صحيح priority = (صحيح)(100000 + (names.size() - i) * 1000);
            منطقي found = שקר;
            עבור (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                אם (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(عدم, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = אמת;
                    hasChanges = אמת;
                    اكسر;
                }
            }
            אם (!found)
            {
                CV_LOG_INFO(عدم, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = אמת;
            }
        }
        החזר hasChanges;
    }
عام:
    std::string dumpBackends() ثابت
    {
        std::ostringstream os;
        עבור (size_t i = 0; i < enabledBackends.size(); i++)
        {
            אם (i > 0) os << "; ";
            ثابت ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        החזר os.str();
    }

    ساكن ParallelBackendRegistry& getInstance()
    {
        ساكن ParallelBackendRegistry g_instance;
        החזר g_instance;
    }

    مضمن ثابت std::vector<ParallelBackendInfo>& getEnabledBackends() ثابت { החזר enabledBackends; }
};


ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    החזר cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
