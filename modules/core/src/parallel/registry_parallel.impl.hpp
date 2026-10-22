// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#ضمن "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق parallel {

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#تعریف_کرو DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#وإلا
#تعریف_کرو DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#جے_انت

#تعریف_کرو DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { أرجع createBackendAPI(); }) \
},

ساكن
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ساكن std::vector<ParallelBackendInfo> g_backends
    {
#جے_تعریف_شدہ HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#وإلاإذا defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#جے_انت

#جے_تعریف_شدہ HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#وإلاإذا defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#جے_انت
    };
    أرجع g_backends;
}

ساكن
منطقي ترتیب_دیو_By_ترجیح(ثابت ParallelBackendInfo &lhs, ثابت ParallelBackendInfo &rhs)
{
    أرجع lhs.ترجیح > rhs.ترجیح;
}

/** @brief Manages list of enabled backends
 */
صنف ParallelBackendRegistry
{
محمي:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        صحيح N = (صحيح)enabledBackends.size();
        لكل (صحيح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.ترجیح = 1000 - i * 10;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        إذا (read_ترجیح_Settings())
        {
            CV_LOG_INFO(عدم, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (صحيح)enabledBackends.size();
        }
        صحيح enabled = 0;
        لكل (صحيح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            إذا (enabled != i)
                info = enabledBackends[i];
            size_t param_ترجیح = utils::get_Configuration_Parameter_ناپ_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.ترجیح);
            CV_Assert(param_ترجیح == (size_t)(صحيح)param_ترجیح); // overflow check
            إذا (param_ترجیح > 0)
            {
                info.ترجیح = (صحيح)param_ترجیح;
                enabled++;
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(عدم, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), ترتیب_دیو_By_ترجیح);
        CV_LOG_INFO(عدم, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ساكن std::vector<std::string> tokenize_string(ثابت std::string& داخلہ_2, حرف token)
    {
        std::vector<std::string> نتیجہ;
        std::string::size_type prev_pos = 0, pos = 0;
        طالما((pos = داخلہ_2.find(token, pos)) != std::string::npos)
        {
            نتیجہ.push_back(داخلہ_2.substr(prev_pos, pos-prev_pos));
            prev_pos = ++pos;
        }
        نتیجہ.push_back(داخلہ_2.substr(prev_pos));
        أرجع نتیجہ;
    }
    منطقي read_ترجیح_Settings()
    {
        منطقي hasChanges = خطأ;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        إذا (prioritized_backends.empty())
            أرجع hasChanges;
        CV_LOG_INFO(عدم, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ثابت std::vector<std::string> ناں_2 = tokenize_string(prioritized_backends, ',');
        لكل (size_t i = 0; i < ناں_2.size(); i++)
        {
            ثابت std::string& name = ناں_2[i];
            صحيح ترجیح = (صحيح)(100000 + (ناں_2.size() - i) * 1000);
            منطقي لبھیا = خطأ;
            لكل (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                إذا (name == info.name)
                {
                    info.ترجیح = ترجیح;
                    CV_LOG_DEBUG(عدم, "core(parallel): New backend priority: '" << name << "' => " << info.ترجیح);
                    لبھیا = سچ;
                    hasChanges = سچ;
                    اكسر;
                }
            }
            إذا (!لبھیا)
            {
                CV_LOG_INFO(عدم, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{ترجیح, name, createPluginParallelBackendFactory(name)});
                hasChanges = سچ;
            }
        }
        أرجع hasChanges;
    }
عام:
    std::string dumpBackends() ثابت
    {
        std::ostringstream os;
        لكل (size_t i = 0; i < enabledBackends.size(); i++)
        {
            إذا (i > 0) os << "; ";
            ثابت ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.ترجیح << ')';
        }
        أرجع os.str();
    }

    ساكن ParallelBackendRegistry& getInstance()
    {
        ساكن ParallelBackendRegistry g_instance;
        أرجع g_instance;
    }

    مضمن ثابت std::vector<ParallelBackendInfo>& getEnabledBackends() ثابت { أرجع enabledBackends; }
};


ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    أرجع cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
