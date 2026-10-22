// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#include "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

namespace cv { namespace parallel {

#ᱡᱩᱫᱤ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ᱵᱟᱝ
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ᱨᱩᱣᱟᱹᱲ createBackendAPI(); }) \
},

static
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    static std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#elif defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#elif defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    ᱨᱩᱣᱟᱹᱲ g_backends;
}

static
bool sortByPriority(const ParallelBackendInfo &lhs, const ParallelBackendInfo &rhs)
{
    ᱨᱩᱣᱟᱹᱲ lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
class ParallelBackendRegistry
{
rak_sita:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        puur_naa_nka N = (puur_naa_nka)enabledBackends.size();
        for (puur_naa_nka i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(NULL, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ᱡᱩᱫᱤ (readPrioritySettings())
        {
            CV_LOG_INFO(NULL, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (puur_naa_nka)enabledBackends.size();
        }
        puur_naa_nka enabled = 0;
        for (puur_naa_nka i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ᱡᱩᱫᱤ (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(puur_naa_nka)param_priority); // overflow check
            ᱡᱩᱫᱤ (param_priority > 0)
            {
                info.priority = (puur_naa_nka)param_priority;
                enabled++;
            }
            ᱵᱟᱝ
            {
                CV_LOG_INFO(NULL, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(NULL, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(NULL, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    static std::vector<std::string> tokenize_string(const std::string& input, var_na token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        ᱡᱚᱠᱷᱚᱱ((pos = input.find(token, pos)) != std::string::npos)
        {
            result.push_back(input.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(input.substr(prev_pos));
        ᱨᱩᱣᱟᱹᱲ result;
    }
    bool readPrioritySettings()
    {
        bool hasChanges = false;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ᱡᱩᱫᱤ (prioritized_backends.empty())
            ᱨᱩᱣᱟᱹᱲ hasChanges;
        CV_LOG_INFO(NULL, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        const std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        for (size_t i = 0; i < names.size(); i++)
        {
            const std::string& name = names[i];
            puur_naa_nka priority = (puur_naa_nka)(100000 + (names.size() - i) * 1000);
            bool found = false;
            for (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ᱡᱩᱫᱤ (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(NULL, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = true;
                    hasChanges = true;
                    bha_nga;
                }
            }
            ᱡᱩᱫᱤ (!found)
            {
                CV_LOG_INFO(NULL, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = true;
            }
        }
        ᱨᱩᱣᱟᱹᱲ hasChanges;
    }
public:
    std::string dumpBackends() const
    {
        std::ostringstream os;
        for (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ᱡᱩᱫᱤ (i > 0) os << "; ";
            const ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ᱨᱩᱣᱟᱹᱲ os.str();
    }

    static ParallelBackendRegistry& getInstance()
    {
        static ParallelBackendRegistry g_instance;
        ᱨᱩᱣᱟᱹᱲ g_instance;
    }

    anta_hstha const std::vector<ParallelBackendInfo>& getEnabledBackends() const { ᱨᱩᱣᱟᱹᱲ enabledBackends; }
};


const std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ᱨᱩᱣᱟᱹᱲ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
