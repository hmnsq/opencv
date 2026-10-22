// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

నామాకాశ cv { నామాకాశ parallel {

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#నిర్వచించు DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#లేకపోతే
#నిర్వచించు DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#అయితే_ముగింపు

#నిర్వచించు DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { తిరిగి createBackendAPI(); }) \
},

స్థిర
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    స్థిర std::vector<ParallelBackendInfo> g_backends
    {
#నిర్వచితమైతే HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#లేదావేళ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#లేదావేళ defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#అయితే_ముగింపు
    };
    తిరిగి g_backends;
}

స్థిర
తార్కిక క్రమీకరించు_By_ప్రాధాన్యత(స్థిరరాశి ParallelBackendInfo &lhs, స్థిరరాశి ParallelBackendInfo &rhs)
{
    తిరిగి lhs.ప్రాధాన్యత > rhs.ప్రాధాన్యత;
}

/** @brief Manages list of enabled backends
 */
తరగతి ParallelBackendRegistry
{
రక్షిత:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        పూర్ణాంకం N = (పూర్ణాంకం)enabledBackends.size();
        కొరకు (పూర్ణాంకం i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.ప్రాధాన్యత = 1000 - i * 10;
        }
        CV_LOG_DEBUG(ఖాళీ, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        ఒకవేళ (read_ప్రాధాన్యత_Settings())
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (పూర్ణాంకం)enabledBackends.size();
        }
        పూర్ణాంకం enabled = 0;
        కొరకు (పూర్ణాంకం i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            ఒకవేళ (enabled != i)
                info = enabledBackends[i];
            size_t param_ప్రాధాన్యత = utils::get_Configuration_Parameter_కొలత_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.ప్రాధాన్యత);
            CV_Assert(param_ప్రాధాన్యత == (size_t)(పూర్ణాంకం)param_ప్రాధాన్యత); // overflow check
            ఒకవేళ (param_ప్రాధాన్యత > 0)
            {
                info.ప్రాధాన్యత = (పూర్ణాంకం)param_ప్రాధాన్యత;
                enabled++;
            }
            లేకపోతే
            {
                CV_LOG_INFO(ఖాళీ, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(ఖాళీ, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), క్రమీకరించు_By_ప్రాధాన్యత);
        CV_LOG_INFO(ఖాళీ, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    స్థిర std::vector<std::string> tokenize_string(స్థిరరాశి std::string& ఆదానం_2, అక్షరం token)
    {
        std::vector<std::string> ఫలితం;
        std::string::size_type prev_pos = 0, pos = 0;
        అయితే((pos = ఆదానం_2.find(token, pos)) != std::string::npos)
        {
            ఫలితం.push_back(ఆదానం_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        ఫలితం.push_back(ఆదానం_2.substr(prev_pos));
        తిరిగి ఫలితం;
    }
    తార్కిక read_ప్రాధాన్యత_Settings()
    {
        తార్కిక hasChanges = అబద్ధం;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        ఒకవేళ (prioritized_backends.empty())
            తిరిగి hasChanges;
        CV_LOG_INFO(ఖాళీ, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        స్థిరరాశి std::vector<std::string> పేర్లు_2 = tokenize_string(prioritized_backends, ',');
        కొరకు (size_t i = 0; i < పేర్లు_2.size(); i++)
        {
            స్థిరరాశి std::string& name = పేర్లు_2[i];
            పూర్ణాంకం ప్రాధాన్యత = (పూర్ణాంకం)(100000 + (పేర్లు_2.size() - i) * 1000);
            తార్కిక దొరికింది = అబద్ధం;
            కొరకు (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                ఒకవేళ (name == info.name)
                {
                    info.ప్రాధాన్యత = ప్రాధాన్యత;
                    CV_LOG_DEBUG(ఖాళీ, "core(parallel): New backend priority: '" << name << "' => " << info.ప్రాధాన్యత);
                    దొరికింది = నిజం;
                    hasChanges = నిజం;
                    ఆపు;
                }
            }
            ఒకవేళ (!దొరికింది)
            {
                CV_LOG_INFO(ఖాళీ, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{ప్రాధాన్యత, name, createPluginParallelBackendFactory(name)});
                hasChanges = నిజం;
            }
        }
        తిరిగి hasChanges;
    }
బహిరంగ:
    std::string dumpBackends() స్థిరరాశి
    {
        std::ostringstream os;
        కొరకు (size_t i = 0; i < enabledBackends.size(); i++)
        {
            ఒకవేళ (i > 0) os << "; ";
            స్థిరరాశి ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.ప్రాధాన్యత << ')';
        }
        తిరిగి os.str();
    }

    స్థిర ParallelBackendRegistry& getInstance()
    {
        స్థిర ParallelBackendRegistry g_instance;
        తిరిగి g_instance;
    }

    అంతఃవరుస స్థిరరాశి std::vector<ParallelBackendInfo>& getEnabledBackends() స్థిరరాశి { తిరిగి enabledBackends; }
};


స్థిరరాశి std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    తిరిగి cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
