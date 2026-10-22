// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#include "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

namespace cv { namespace parallel {

#ຖ້າ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ບໍ່ດັ່ງນັ້ນ
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ສົ່ງຄືນ createBackendAPI(); }) \
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
    ສົ່ງຄືນ g_backends;
}

static
bool sortByPriority(const ParallelBackendInfo &lhs, const ParallelBackendInfo &rhs)
{
    ສົ່ງຄືນ lhs.priority > rhs.priority;
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
        ຈຳນວນເຕັມ N = (ຈຳນວນເຕັມ)enabledBackends.size();
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(NULL, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ຖ້າ (readPrioritySettings())
        {
            CV_LOG_INFO(NULL, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (ຈຳນວນເຕັມ)enabledBackends.size();
        }
        ຈຳນວນເຕັມ enabled = 0;
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ຖ້າ (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(ຈຳນວນເຕັມ)param_priority); // overflow check
            ຖ້າ (param_priority > 0)
            {
                info.priority = (ຈຳນວນເຕັມ)param_priority;
                enabled++;
            }
            ບໍ່ດັ່ງນັ້ນ
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
        ໃນຂະນະທີ່((pos = input.find(token, pos)) != std::string::npos)
        {
            result.push_back(input.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(input.substr(prev_pos));
        ສົ່ງຄືນ result;
    }
    bool readPrioritySettings()
    {
        bool hasChanges = false;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ຖ້າ (prioritized_backends.empty())
            ສົ່ງຄືນ hasChanges;
        CV_LOG_INFO(NULL, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        const std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        ສຳລັບ (size_t i = 0; i < names.size(); i++)
        {
            const std::string& name = names[i];
            ຈຳນວນເຕັມ priority = (ຈຳນວນເຕັມ)(100000 + (names.size() - i) * 1000);
            bool found = false;
            ສຳລັບ (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ຖ້າ (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(NULL, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = true;
                    hasChanges = true;
                    bha_nga;
                }
            }
            ຖ້າ (!found)
            {
                CV_LOG_INFO(NULL, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = true;
            }
        }
        ສົ່ງຄືນ hasChanges;
    }
public:
    std::string dumpBackends() const
    {
        std::ostringstream os;
        ສຳລັບ (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ຖ້າ (i > 0) os << "; ";
            const ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ສົ່ງຄືນ os.str();
    }

    static ParallelBackendRegistry& getInstance()
    {
        static ParallelBackendRegistry g_instance;
        ສົ່ງຄືນ g_instance;
    }

    anta_hstha const std::vector<ParallelBackendInfo>& getEnabledBackends() const { ສົ່ງຄືນ enabledBackends; }
};


const std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ສົ່ງຄືນ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
