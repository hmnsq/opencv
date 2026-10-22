// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

ନାମାକାଶ cv { ନାମାକାଶ parallel {

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#define DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ଅନ୍ୟଥା
#define DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#endif

#define DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { ଫେରନ୍ତୁ createBackendAPI(); }) \
},

ସ୍ଥିର
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ସ୍ଥିର std::vector<ParallelBackendInfo> g_backends
    {
#ifdef HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#ନଚେତଯଦି defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#endif

#ifdef HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#ନଚେତଯଦି defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#endif
    };
    ଫେରନ୍ତୁ g_backends;
}

ସ୍ଥିର
ତାର୍କିକ sortByPriority(ଅଚଳ ParallelBackendInfo &lhs, ଅଚଳ ParallelBackendInfo &rhs)
{
    ଫେରନ୍ତୁ lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
ଶ୍ରେଣୀ ParallelBackendRegistry
{
ସୁରକ୍ଷିତ:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)enabledBackends.size();
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ଖାଲି, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ଯଦି (readPrioritySettings())
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)enabledBackends.size();
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା enabled = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ଯଦି (enabled != i)
                info = enabledBackends[i];
            size_t param_priority = utils::getConfigurationParameterSizeT(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.priority);
            CV_Assert(param_priority == (size_t)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)param_priority); // overflow check
            ଯଦି (param_priority > 0)
            {
                info.priority = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)param_priority;
                enabled++;
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_INFO(ଖାଲି, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ଖାଲି, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(ଖାଲି, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ସ୍ଥିର std::vector<std::string> tokenize_string(ଅଚଳ std::string& ନିବେଶ_2, ଅକ୍ଷର token)
    {
        std::vector<std::string> result;
        std::string::size_type prev_pos = 0, pos = 0;
        ଯେତେବେଳେ((pos = ନିବେଶ_2.find(token, pos)) != std::string::npos)
        {
            result.push_back(ନିବେଶ_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        result.push_back(ନିବେଶ_2.substr(prev_pos));
        ଫେରନ୍ତୁ result;
    }
    ତାର୍କିକ readPrioritySettings()
    {
        ତାର୍କିକ hasChanges = ମିଥ୍ୟା;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ଯଦି (prioritized_backends.empty())
            ଫେରନ୍ତୁ hasChanges;
        CV_LOG_INFO(ଖାଲି, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ଅଚଳ std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        ପାଇଁ (size_t i = 0; i < names.size(); i++)
        {
            ଅଚଳ std::string& name = names[i];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା priority = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(100000 + (names.size() - i) * 1000);
            ତାର୍କିକ found = ମିଥ୍ୟା;
            ପାଇଁ (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ଯଦି (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(ଖାଲି, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = ସତ୍ୟ;
                    hasChanges = ସତ୍ୟ;
                    ଭାଙ୍ଗ;
                }
            }
            ଯଦି (!found)
            {
                CV_LOG_INFO(ଖାଲି, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = ସତ୍ୟ;
            }
        }
        ଫେରନ୍ତୁ hasChanges;
    }
ସର୍ବସାଧାରଣ:
    std::string dumpBackends() ଅଚଳ
    {
        std::ostringstream os;
        ପାଇଁ (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ଯଦି (i > 0) os << "; ";
            ଅଚଳ ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        ଫେରନ୍ତୁ os.str();
    }

    ସ୍ଥିର ParallelBackendRegistry& getInstance()
    {
        ସ୍ଥିର ParallelBackendRegistry g_instance;
        ଫେରନ୍ତୁ g_instance;
    }

    ଧାଡ଼ିଭିତରେ ଅଚଳ std::vector<ParallelBackendInfo>& getEnabledBackends() ଅଚଳ { ଫେରନ୍ତୁ enabledBackends; }
};


ଅଚଳ std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    ଫେରନ୍ତୁ cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
