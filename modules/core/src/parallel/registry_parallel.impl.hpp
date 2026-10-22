// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#അന്തര്ഭാവ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

നാമാകാശ cv { നാമാകാശ parallel {

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#അല്ലെങ്കിൽ
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { തിരികെ createBackendAPI(); }) \
},

സ്ഥിതം
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    സ്ഥിതം std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#അല്ലെങ്കിലെങ്കിൽ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#അല്ലെങ്കിലെങ്കിൽ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    തിരികെ g_backends;
}

സ്ഥിതം
തർക്കം sortByPriority(സ്ഥിരം ParallelBackendInfo &lhs, സ്ഥിരം ParallelBackendInfo &rhs)
{
    തിരികെ lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
വർഗ്ഗം ParallelBackendRegistry
{
സംരക്ഷിത:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        പൂർണ്ണസംഖ്യ N = (പൂർണ്ണസംഖ്യ)enabledBackends.size();
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ശൂന്യ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        എങ്കിൽ (readPrioritySettings())
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (പൂർണ്ണസംഖ്യ)enabledBackends.size();
        }
        പൂർണ്ണസംഖ്യ enabled = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            എങ്കിൽ (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(പൂർണ്ണസംഖ്യ)param_priority); // overflow check
            എങ്കിൽ (param_priority > 0)
            {
                info.priority = (പൂർണ്ണസംഖ്യ)param_priority;
                enabled++;
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_INFO(ശൂന്യ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ശൂന്യ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(ശൂന്യ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    സ്ഥിതം std::vector<std::string> tokenize_string(സ്ഥിരം std::string& നിവേശം_2, അക്ഷരം token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        ആകുമ്പോൾ((pos = നിവേശം_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(നിവേശം_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(നിവേശം_2.substr(prev_pos));
        തിരികെ result;
    }
    തർക്കം readPrioritySettings()
    {
        തർക്കം hasChanges = അസത്യം;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        എങ്കിൽ (prioritized_backends.empty())
            തിരികെ hasChanges;
        CV_LOG_INFO(ശൂന്യ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        സ്ഥിരം std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        വേണ്ടി (size_t i = 0; i < names.size(); i++)
        {
            സ്ഥിരം std::string& name = names[i];
            പൂർണ്ണസംഖ്യ priority = (പൂർണ്ണസംഖ്യ)(100000 + (names.size() - i) * 1000);
            തർക്കം found = അസത്യം;
            വേണ്ടി (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                എങ്കിൽ (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(ശൂന്യ, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = സത്യം;
                    hasChanges = സത്യം;
                    നിർത്തുക;
                }
            }
            എങ്കിൽ (!found)
            {
                CV_LOG_INFO(ശൂന്യ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = സത്യം;
            }
        }
        തിരികെ hasChanges;
    }
പൊതു:
    std::string dumpBackends() സ്ഥിരം
    {
        std::ostringstream os;
        വേണ്ടി (size_t i = 0; i < enabledBackends.size(); i++)
        {
            എങ്കിൽ (i > 0) os << "; ";
            സ്ഥിരം ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        തിരികെ os.str();
    }

    സ്ഥിതം ParallelBackendRegistry& getInstance()
    {
        സ്ഥിതം ParallelBackendRegistry g_instance;
        തിരികെ g_instance;
    }

    വരിയിൽ സ്ഥിരം std::vector<ParallelBackendInfo>& getEnabledBackends() സ്ഥിരം { തിരികെ enabledBackends; }
};


സ്ഥിരം std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    തിരികെ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
