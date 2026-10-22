// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ಹಿಂತಿರುಗಿ createBackendAPI(); }) \
},

ಸ್ಥಿರ
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ಸ್ಥಿರ std::vector<ParallelBackendInfo> g_backends
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#ಅಥವಾಆದರೆ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#ಅಥವಾಆದರೆ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#ಆದರೆ_ಅಂತ್ಯ
    };
    ಹಿಂತಿರುಗಿ g_backends;
}

ಸ್ಥಿರ
ತಾರ್ಕಿಕ ವಿಂಗಡಿಸು_By_ಆದ್ಯತೆ(ಸ್ಥಿರಾಂಕ ParallelBackendInfo &lhs, ಸ್ಥಿರಾಂಕ ParallelBackendInfo &rhs)
{
    ಹಿಂತಿರುಗಿ lhs.ಆದ್ಯತೆ > rhs.ಆದ್ಯತೆ;
}

/** @brief Manages list of enabled backends
 */
ವರ್ಗ ParallelBackendRegistry
{
ಸಂರಕ್ಷಿತ:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        ಪೂರ್ಣಾಂಕ N = (ಪೂರ್ಣಾಂಕ)enabledBackends.size();
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.ಆದ್ಯತೆ = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ಆದರೆ (read_ಆದ್ಯತೆ_Settings())
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (ಪೂರ್ಣಾಂಕ)enabledBackends.size();
        }
        ಪೂರ್ಣಾಂಕ enabled = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ಆದರೆ (enabled != i)
                info = enabledBackends[i];
            size_t param_ಆದ್ಯತೆ = utils::get_Configuration_Parameter_ಗಾತ್ರ_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.ಆದ್ಯತೆ);
            CV_Assert(param_ಆದ್ಯತೆ == (size_t)(ಪೂರ್ಣಾಂಕ)param_ಆದ್ಯತೆ); // overflow check
            ಆದರೆ (param_ಆದ್ಯತೆ > 0)
            {
                info.ಆದ್ಯತೆ = (ಪೂರ್ಣಾಂಕ)param_ಆದ್ಯತೆ;
                enabled++;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_INFO(ಖಾಲಿ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), ವಿಂಗಡಿಸು_By_ಆದ್ಯತೆ);
        CV_LOG_INFO(ಖಾಲಿ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ಸ್ಥಿರ std::vector<std::string> tokenize_string(ಸ್ಥಿರಾಂಕ std::string& ಒಳಹರಿವು_2, ಅಕ್ಷರ token)
    {
        std::vector<std::string> ಫಲಿತಾಂಶ;
        std::string::size_type prev_pos = 0, pos = 0;
        ಆಗ((pos = ಒಳಹರಿವು_2.find(token, pos)) != std::string::npos)
        {
            ಫಲಿತಾಂಶ.push_back(ಒಳಹರಿವು_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        ಫಲಿತಾಂಶ.push_back(ಒಳಹರಿವು_2.substr(prev_pos));
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
    ತಾರ್ಕಿಕ read_ಆದ್ಯತೆ_Settings()
    {
        ತಾರ್ಕಿಕ hasChanges = ಸುಳ್ಳು;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ಆದರೆ (prioritized_backends.empty())
            ಹಿಂತಿರುಗಿ hasChanges;
        CV_LOG_INFO(ಖಾಲಿ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ಸ್ಥಿರಾಂಕ std::vector<std::string> ಹೆಸರುಗಳು_2 = tokenize_string(prioritized_backends, ',');
        ಗಾಗಿ (size_t i = 0; i < ಹೆಸರುಗಳು_2.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ std::string& name = ಹೆಸರುಗಳು_2[i];
            ಪೂರ್ಣಾಂಕ ಆದ್ಯತೆ = (ಪೂರ್ಣಾಂಕ)(100000 + (ಹೆಸರುಗಳು_2.size() - i) * 1000);
            ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
            ಗಾಗಿ (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ಆದರೆ (name == info.name)
                {
                    info.ಆದ್ಯತೆ = ಆದ್ಯತೆ;
                    CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): New backend priority: '" << name << "' => " << info.ಆದ್ಯತೆ);
                    ಸಿಕ್ಕಿತು = ನಿಜ;
                    hasChanges = ನಿಜ;
                    ನಿಲ್ಲಿಸು;
                }
            }
            ಆದರೆ (!ಸಿಕ್ಕಿತು)
            {
                CV_LOG_INFO(ಖಾಲಿ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{ಆದ್ಯತೆ, name, createPluginParallelBackendFactory(name)});
                hasChanges = ನಿಜ;
            }
        }
        ಹಿಂತಿರುಗಿ hasChanges;
    }
ಸಾರ್ವಜನಿಕ:
    std::string dumpBackends() ಸ್ಥಿರಾಂಕ
    {
        std::ostringstream os;
        ಗಾಗಿ (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ಆದರೆ (i > 0) os << "; ";
            ಸ್ಥಿರಾಂಕ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.ಆದ್ಯತೆ << ')';
        }
        ಹಿಂತಿರುಗಿ os.str();
    }

    ಸ್ಥಿರ ParallelBackendRegistry& getInstance()
    {
        ಸ್ಥಿರ ParallelBackendRegistry g_instance;
        ಹಿಂತಿರುಗಿ g_instance;
    }

    ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ std::vector<ParallelBackendInfo>& getEnabledBackends() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ enabledBackends; }
};


ಸ್ಥಿರಾಂಕ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ಹಿಂತಿರುಗಿ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
