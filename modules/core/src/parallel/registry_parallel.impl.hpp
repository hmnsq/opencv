// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

#شامل "opencv2/core/utils/filesystem.private.hpp"  // OPENCV_HAVE_FILESYSTEM_SUPPORT

نامخلا cv { نامخلا parallel {

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
#تعریف_کرو DECLARE_DYNAMIC_BACKEND(name) \
ParallelBackendInfo { \
    1000, name, createPluginParallelBackendFactory(name) \
},
#ورنہ
#تعریف_کرو DECLARE_DYNAMIC_BACKEND(name) /* nothing */
#اگر_ختم

#تعریف_کرو DECLARE_STATIC_BACKEND(name, createBackendAPI) \
ParallelBackendInfo { \
    1000, name, std::make_shared<cv::parallel::StaticBackendFactory>([=] () -> std::shared_ptr<cv::parallel::ParallelForAPI> { واپس createBackendAPI(); }) \
},

ساکن
std::vector<ParallelBackendInfo>& getBuiltinParallelBackendsInfo()
{
    ساکن std::vector<ParallelBackendInfo> g_backends
    {
#اگر_تعریف_شدہ HAVE_TBB
        DECLARE_STATIC_BACKEND("TBB", createParallelBackendTBB)
#وگرنہ_اگر defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("ONETBB")   // dedicated oneTBB plugin (interface >= 12000, binary incompatibe with TBB 2017-2020)
        DECLARE_DYNAMIC_BACKEND("TBB")      // generic TBB plugins
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENMP
        DECLARE_STATIC_BACKEND("OPENMP", createParallelBackendOpenMP)
#وگرنہ_اگر defined(PARALLEL_ENABLE_PLUGINS)
        DECLARE_DYNAMIC_BACKEND("OPENMP")  // TODO Intel OpenMP?
#اگر_ختم
    };
    واپس g_backends;
}

ساکن
منطقی ترتیب_دو_By_ترجیح(ثابت ParallelBackendInfo &lhs, ثابت ParallelBackendInfo &rhs)
{
    واپس lhs.ترجیح > rhs.ترجیح;
}

/** @brief Manages list of enabled backends
 */
جماعت ParallelBackendRegistry
{
محفوظ:
    std::vector<ParallelBackendInfo> enabledBackends;
    ParallelBackendRegistry()
    {
        enabledBackends = getBuiltinParallelBackendsInfo();
        صحیح N = (صحیح)enabledBackends.size();
        برائے (صحیح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[i];
            info.ترجیح = 1000 - i * 10;
        }
        CV_LOG_DEBUG(تہی, "core(parallel): Builtin backends(" << N << "): " << dumpBackends());
        اگر (read_ترجیح_Settings())
        {
            CV_LOG_INFO(تہی, "core(parallel): Updated backends priorities: " << dumpBackends());
            N = (صحیح)enabledBackends.size();
        }
        صحیح enabled = 0;
        برائے (صحیح i = 0; i < N; i++)
        {
            ParallelBackendInfo& info = enabledBackends[enabled];
            اگر (enabled != i)
                info = enabledBackends[i];
            size_t param_ترجیح = utils::get_Configuration_Parameter_سائز_T(cv::format("OPENCV_PARALLEL_PRIORITY_%s", info.name.c_str()).c_str(), (size_t)info.ترجیح);
            CV_Assert(param_ترجیح == (size_t)(صحیح)param_ترجیح); // overflow check
            اگر (param_ترجیح > 0)
            {
                info.ترجیح = (صحیح)param_ترجیح;
                enabled++;
            }
            ورنہ
            {
                CV_LOG_INFO(تہی, "core(parallel): Disable backend: " << info.name);
            }
        }
        enabledBackends.resize(enabled);
        CV_LOG_DEBUG(تہی, "core(parallel): Available backends(" << enabled << "): " << dumpBackends());
        std::sort(enabledBackends.begin(), enabledBackends.end(), ترتیب_دو_By_ترجیح);
        CV_LOG_INFO(تہی, "core(parallel): Enabled backends(" << enabled << ", sorted by priority): " << (enabledBackends.empty() ? std::string("N/A") : dumpBackends()));
    }

    ساکن std::vector<std::string> tokenize_string(ثابت std::string& داخلہ_2, حرف token)
    {
        std::vector<std::string> نتیجہ;
        std::string::size_type پچھلا_pos = 0, pos = 0;
        جب((pos = داخلہ_2.find(token, pos)) != std::string::npos)
        {
            نتیجہ.push_back(داخلہ_2.substr(پچھلا_pos, pos-پچھلا_pos));
            پچھلا_pos = ++pos;
        }
        نتیجہ.push_back(داخلہ_2.substr(پچھلا_pos));
        واپس نتیجہ;
    }
    منطقی read_ترجیح_Settings()
    {
        منطقی hasChanges = جھوٹ;
        cv::String prioritized_backends = utils::getConfigurationParameterString("OPENCV_PARALLEL_PRIORITY_LIST");
        اگر (prioritized_backends.empty())
            واپس hasChanges;
        CV_LOG_INFO(تہی, "core(parallel): Configured priority list (OPENCV_PARALLEL_PRIORITY_LIST): " << prioritized_backends);
        ثابت std::vector<std::string> نام_2 = tokenize_string(prioritized_backends, ',');
        برائے (size_t i = 0; i < نام_2.size(); i++)
        {
            ثابت std::string& name = نام_2[i];
            صحیح ترجیح = (صحیح)(100000 + (نام_2.size() - i) * 1000);
            منطقی ملا = جھوٹ;
            برائے (size_t k = 0; k < enabledBackends.size(); k++)
            {
                ParallelBackendInfo& info = enabledBackends[k];
                اگر (name == info.name)
                {
                    info.ترجیح = ترجیح;
                    CV_LOG_DEBUG(تہی, "core(parallel): New backend priority: '" << name << "' => " << info.ترجیح);
                    ملا = سچ;
                    hasChanges = سچ;
                    توڑو;
                }
            }
            اگر (!ملا)
            {
                CV_LOG_INFO(تہی, "core(parallel): Adding parallel backend (plugin): '" << name << "'");
                enabledBackends.push_back(ParallelBackendInfo{ترجیح, name, createPluginParallelBackendFactory(name)});
                hasChanges = سچ;
            }
        }
        واپس hasChanges;
    }
عوامی:
    std::string dumpBackends() ثابت
    {
        std::ostringstream os;
        برائے (size_t i = 0; i < enabledBackends.size(); i++)
        {
            اگر (i > 0) os << "; ";
            ثابت ParallelBackendInfo& info = enabledBackends[i];
            os << info.name << '(' << info.ترجیح << ')';
        }
        واپس os.str();
    }

    ساکن ParallelBackendRegistry& getInstance()
    {
        ساکن ParallelBackendRegistry g_instance;
        واپس g_instance;
    }

    اندرونی ثابت std::vector<ParallelBackendInfo>& getEnabledBackends() ثابت { واپس enabledBackends; }
};


ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo()
{
    واپس cv::parallel::ParallelBackendRegistry::getInstance().getEnabledBackends();
}

}} // namespace
