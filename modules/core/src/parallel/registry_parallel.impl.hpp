// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#समावेश "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

namespace cv { namespace parallel {

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#घोषणा DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#अन्यथा
#घोषणा DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#पूर्ण_अगर

#घोषणा DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { वापस createBackendAPI(); }) \
},

अटल
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    अटल std::vector<ParallelBackendInfo> g_backends
    {
#अगर_घोषित HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#या_अगर defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#या_अगर defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#पूर्ण_अगर
    };
    वापस g_backends;
}

अटल
bool sortByPriority(स्थिर ParallelBackendInfo &lhs, स्थिर ParallelBackendInfo &rhs)
{
    वापस lhs.priority > rhs.priority;
}

/** @brief Manages list of enabled backends
 */
श्रेणी ParallelBackendRegistry
{
रक्षित:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        पूर्णांक N = (पूर्णांक)enabledBackends.size();
        क्रम (पूर्णांक i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.priority = 1000 - i * 10;
        }
        CV_LOG_DEBUG(शून्य, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        अगर (readPrioritySettings())
        {
            CV_LOG_INFO(शून्य, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (पूर्णांक)enabledBackends.size();
        }
        पूर्णांक enabled = 0;
        क्रम (पूर्णांक i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            अगर (enabled != i)
                info = enabledBackends[i];
            माप_प्रकार param_priority = utils::get_Configuration_Parameter_आकार_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (माप_प्रकार)info.priority);
            CV_Assert(param_priority == (माप_प्रकार)(पूर्णांक)param_priority); // overflow check
            अगर (param_priority > 0)
            {
                info.priority = (पूर्णांक)param_priority;
                enabled++;
            }
            अन्यथा
            {
                CV_LOG_INFO(शून्य, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(शून्य, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), sortByPriority);
        CV_LOG_INFO(शून्य, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    अटल std::vector<std::string> tokenize_string(स्थिर std::string& निवेश_2, अक्षर token)
    {
        std::vector<std::string> परिणाम;
        std::string::size_type पूर्व_pos = 0, pos = 0;
        जबतक((pos = निवेश_2.find(token, pos)) != std::string::npos)
        {
            परिणाम.push_back(निवेश_2.substr(पूर्व_pos, pos-पूर्व_pos));
            पूर्व_pos = ++pos;
        }
        परिणाम.push_back(निवेश_2.substr(पूर्व_pos));
        वापस परिणाम;
    }
    bool readPrioritySettings()
    {
        bool hasChanges = false;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        अगर (prioritized_backends.empty())
            वापस hasChanges;
        CV_LOG_INFO(शून्य, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        स्थिर std::vector<std::string> names = tokenize_string(prioritized_backends, ',');
        क्रम (माप_प्रकार i = 0; i < names.size(); i++)
        {
            स्थिर std::string& name = names[i];
            पूर्णांक priority = (पूर्णांक)(100000 + (names.size() - i) * 1000);
            bool found = false;
            क्रम (माप_प्रकार k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                अगर (name == info.name)
                {
                    info.priority = priority;
                    CV_LOG_DEBUG(शून्य, "core(parallel): New backend priority: '" << name << "' => " << info.priority);
                    found = true;
                    hasChanges = true;
                    अवरोध;
                }
            }
            अगर (!found)
            {
                CV_LOG_INFO(शून्य, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{priority, name, createPluginParallelBackendFactory(name)});
                hasChanges = true;
            }
        }
        वापस hasChanges;
    }
खुला:
    std::string dumpBackends() स्थिर
    {
        std::ostringstream os;
        क्रम (माप_प्रकार i = 0; i < enabledBackends.size(); i++)
        {
            अगर (i > 0) os << "; ";
            स्थिर ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.priority << ')';
        }
        वापस os.str();
    }

    अटल ParallelBackendRegistry& getInstance()
    {
        अटल ParallelBackendRegistry g_instance;
        वापस g_instance;
    }

    अंतरभूत स्थिर std::vector<ParallelBackendInfo>& getEnabledBackends() स्थिर { वापस enabledBackends; }
};


स्थिर std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    वापस cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
