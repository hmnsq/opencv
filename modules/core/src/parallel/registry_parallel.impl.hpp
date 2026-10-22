// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ਵਾਪਸ createBackendAPI(); }) \
},

ਸਥਿਰ
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ਸਥਿਰ std::vector<ParallelBackendInfo> g_backends
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#ਜਾਂਜੇ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#ਜਾਂਜੇ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#ਜੇ_ਅੰਤ
    };
    ਵਾਪਸ g_backends;
}

ਸਥਿਰ
ਤਾਰਕਿਕ ਛਾਂਟੋ_By_ਤਰਜੀਹ(ਅਚਲ ParallelBackendInfo &lhs, ਅਚਲ ParallelBackendInfo &rhs)
{
    ਵਾਪਸ lhs.ਤਰਜੀਹ > rhs.ਤਰਜੀਹ;
}

/** @brief Manages list of enabled backends
 */
ਵਰਗ ParallelBackendRegistry
{
ਸੁਰੱਖਿਅਤ:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        ਪੂਰਨ N = (ਪੂਰਨ)enabledBackends.size();
        ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.ਤਰਜੀਹ = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ਜੇ (read_ਤਰਜੀਹ_Settings())
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (ਪੂਰਨ)enabledBackends.size();
        }
        ਪੂਰਨ enabled = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ਜੇ (enabled != i)
                info = enabledBackends[i];
            size_t param_ਤਰਜੀਹ = utils::get_Configuration_Parameter_ਮਾਪ_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.ਤਰਜੀਹ);
            CV_Assert(param_ਤਰਜੀਹ == (size_t)(ਪੂਰਨ)param_ਤਰਜੀਹ); // overflow check
            ਜੇ (param_ਤਰਜੀਹ > 0)
            {
                info.ਤਰਜੀਹ = (ਪੂਰਨ)param_ਤਰਜੀਹ;
                enabled++;
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_INFO(ਨਿਰਾ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), ਛਾਂਟੋ_By_ਤਰਜੀਹ);
        CV_LOG_INFO(ਨਿਰਾ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ਸਥਿਰ std::vector<std::string> tokenize_string(ਅਚਲ std::string& ਨਿਵੇਸ਼_2, ਅੱਖਰ token)
    {
        std::vector<std::string> ਨਤੀਜਾ;
        std::string::size_type prev_pos = 0, pos = 0;
        ਜਦੋਂ((pos = ਨਿਵੇਸ਼_2.find(token, pos)) != std::string::npos)
        {
            ਨਤੀਜਾ.push_back(ਨਿਵੇਸ਼_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        ਨਤੀਜਾ.push_back(ਨਿਵੇਸ਼_2.substr(prev_pos));
        ਵਾਪਸ ਨਤੀਜਾ;
    }
    ਤਾਰਕਿਕ read_ਤਰਜੀਹ_Settings()
    {
        ਤਾਰਕਿਕ hasChanges = ਝੂਠ;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ਜੇ (prioritized_backends.empty())
            ਵਾਪਸ hasChanges;
        CV_LOG_INFO(ਨਿਰਾ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ਅਚਲ std::vector<std::string> ਨਾਂ_2 = tokenize_string(prioritized_backends, ',');
        ਲਈ (size_t i = 0; i < ਨਾਂ_2.size(); i++)
        {
            ਅਚਲ std::string& name = ਨਾਂ_2[i];
            ਪੂਰਨ ਤਰਜੀਹ = (ਪੂਰਨ)(100000 + (ਨਾਂ_2.size() - i) * 1000);
            ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
            ਲਈ (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ਜੇ (name == info.name)
                {
                    info.ਤਰਜੀਹ = ਤਰਜੀਹ;
                    CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): New backend priority: '" << name << "' => " << info.ਤਰਜੀਹ);
                    ਮਿਲਿਆ = ਸੱਚ;
                    hasChanges = ਸੱਚ;
                    ਰੋਕੋ;
                }
            }
            ਜੇ (!ਮਿਲਿਆ)
            {
                CV_LOG_INFO(ਨਿਰਾ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{ਤਰਜੀਹ, name, createPluginParallelBackendFactory(name)});
                hasChanges = ਸੱਚ;
            }
        }
        ਵਾਪਸ hasChanges;
    }
ਜਨਤਕ:
    std::string dumpBackends() ਅਚਲ
    {
        std::ostringstream os;
        ਲਈ (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ਜੇ (i > 0) os << "; ";
            ਅਚਲ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.ਤਰਜੀਹ << ')';
        }
        ਵਾਪਸ os.str();
    }

    ਸਥਿਰ ParallelBackendRegistry& getInstance()
    {
        ਸਥਿਰ ParallelBackendRegistry g_instance;
        ਵਾਪਸ g_instance;
    }

    ਸਤਰਅੰਦਰ ਅਚਲ std::vector<ParallelBackendInfo>& getEnabledBackends() ਅਚਲ { ਵਾਪਸ enabledBackends; }
};


ਅਚਲ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ਵਾਪਸ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
