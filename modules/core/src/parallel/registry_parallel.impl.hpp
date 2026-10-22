// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

નામાકાશ cv { નામાકાશ parallel {

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#નહીંતર
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { પાછા createBackendAPI(); }) \
},

સ્થિર
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    સ્થિર std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#અથવાજો defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#અથવાજો defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    પાછા g_backends;
}

સ્થિર
તાર્કિક sortByPriority(અચલ ParallelBackendInfo &lhs, અચલ ParallelBackendInfo &rhs)
{
    પાછા lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
વર્ગ ParallelBackendRegistry
{
સુરક્ષિત:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        પૂર્ણાંક N = (પૂર્ણાંક)enabledBackends.size();
        માટે (પૂર્ણાંક i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ખાલી, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        જો (readPrioritySettings())
        {
            CV_LOG_INFO(ખાલી, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (પૂર્ણાંક)enabledBackends.size();
        }
        પૂર્ણાંક enabled = 0;
        માટે (પૂર્ણાંક i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            જો (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(પૂર્ણાંક)param_priority); // overflow check
            જો (param_priority > 0)
            {
                info.priority = (પૂર્ણાંક)param_priority;
                enabled++;
            }
            નહીંતર
            {
                CV_LOG_INFO(ખાલી, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ખાલી, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(ખાલી, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    સ્થિર std::vector<std::string> tokenize_string(અચલ std::string& આગત_2, અક્ષર token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        જ્યારે((pos = આગત_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(આગત_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(આગત_2.substr(prev_pos));
        પાછા result;
    }
    તાર્કિક readPrioritySettings()
    {
        તાર્કિક hasChanges = ખોટું;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        જો (prioritized_backends.empty())
            પાછા hasChanges;
        CV_LOG_INFO(ખાલી, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        અચલ std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        માટે (size_t i = 0; i < names.size(); i++)
        {
            અચલ std::string& name = names[i];
            પૂર્ણાંક priority = (પૂર્ણાંક)(100000 + (names.size() - i) * 1000);
            તાર્કિક found = ખોટું;
            માટે (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                જો (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(ખાલી, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = સાચું;
                    hasChanges = સાચું;
                    તોડો;
                }
            }
            જો (!found)
            {
                CV_LOG_INFO(ખાલી, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = સાચું;
            }
        }
        પાછા hasChanges;
    }
જાહેર:
    std::string dumpBackends() અચલ
    {
        std::ostringstream os;
        માટે (size_t i = 0; i < enabledBackends.size(); i++)
        {
            જો (i > 0) os << "; ";
            અચલ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        પાછા os.str();
    }

    સ્થિર ParallelBackendRegistry& getInstance()
    {
        સ્થિર ParallelBackendRegistry g_instance;
        પાછા g_instance;
    }

    અંતઃરેખ અચલ std::vector<ParallelBackendInfo>& getEnabledBackends() અચલ { પાછા enabledBackends; }
};


અચલ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    પાછા cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
