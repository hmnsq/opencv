// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#شامل "opencv2/core/utils/plugin_loader.private.hpp"

نامخلا cv { نامخلا impl {

استعمال نامخلا cv::parallel;

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

استعمال نامخلا cv::plugin::impl;  // plugin_loader.hpp

جماعت PluginParallelBackend CV_FINAL: عوامی std::enable_shared_from_this<PluginParallelBackend>
{
محفوظ:
    خالی initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_علامت(init_name));
        اگر (fn_init)
        {
            CV_LOG_DEBUG(تہی, "Found entry: '" << init_name << "'");
            برائے (صحیح supported_api_نسخہ = API_نسخہ; supported_api_نسخہ >= 0; supported_api_نسخہ--)
            {
                plugin_api_ = fn_init(ABI_نسخہ, supported_api_نسخہ, تہی);
                اگر (plugin_api_)
                    توڑو;
            }
            اگر (!plugin_api_)
            {
                CV_LOG_INFO(تہی, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                واپس;
            }
            اگر (!checkCompatibility(plugin_api_->api_سرخی, ABI_نسخہ, API_نسخہ, جھوٹ))
            {
                plugin_api_ = تہی;
                واپس;
            }
            CV_LOG_INFO(تہی, "core(parallel): plugin is ready to use '" << plugin_api_->api_سرخی.api_description << "'");
        }
        ورنہ
        {
            CV_LOG_INFO(تہی, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقی checkCompatibility(ثابت Open_CV_API_سرخی& api_سرخی, غیرعلامتی صحیح abi_نسخہ, غیرعلامتی صحیح api_نسخہ, منطقی checkMinorOpenCVVersion)
    {
        اگر (api_سرخی.opencv_نسخہ_major != CV_نسخہ_MAJOR)
        {
            CV_LOG_ERROR(تہی, "core(parallel): wrong OpenCV major version used by plugin '" << api_سرخی.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخہ "'", api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor))
            واپس جھوٹ;
        }
        اگر (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        ورنہ اگر (api_سرخی.opencv_نسخہ_minor != CV_نسخہ_MINOR)
        {
            CV_LOG_ERROR(تہی, "core(parallel): wrong OpenCV minor version used by plugin '" << api_سرخی.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخہ "'", api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor))
            واپس جھوٹ;
        }
        CV_LOG_DEBUG(تہی, "core(parallel): initialized '" << api_سرخی.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor,
                 api_سرخی.min_api_نسخہ, api_سرخی.api_نسخہ)
            << ", current OpenCV version is '" CV_نسخہ "' (ABI/API = " << abi_نسخہ << "/" << api_نسخہ << ")"
        );
        اگر (api_سرخی.min_api_نسخہ != abi_نسخہ)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(تہی, "core(parallel): plugin is not supported due to incompatible ABI = " << api_سرخی.min_api_نسخہ);
            واپس جھوٹ;
        }
        اگر (api_سرخی.api_نسخہ != api_نسخہ)
        {
            CV_LOG_INFO(تہی, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_سرخی.api_نسخہ, api_نسخہ));
            اگر (api_سرخی.api_نسخہ < api_نسخہ)
            {
                CV_LOG_INFO(تہی, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        واپس سچ;
    }

عوامی:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ثابت OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ثابت std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(تہی)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = تہی;

        اگر (plugin_api_->v0.getInstance)
        {
            اگر (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                واپس std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        واپس std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


جماعت PluginParallelBackendFactory CV_FINAL: عوامی IParallelBackendFactory
{
عوامی:
    std::string بنیاد_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقی initialized;
عوامی:
    PluginParallelBackendFactory(ثابت std::string& بنیاد_Name)
        : بنیاد_Name_2(بنیاد_Name)
        , initialized(جھوٹ)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        اگر (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(یہ)->initBackend();
        }
        اگر (backend)
            واپس backend->create();
        واپس std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
محفوظ:
    خالی initBackend()
    {
        Auto_تالا lock(getInitializationMutex());
        کوشش
        {
            اگر (!initialized)
                بھرو_Plugin();
        }
        پکڑو (...)
        {
            CV_LOG_INFO(تہی, "core(parallel): exception during plugin loading: " << بنیاد_Name_2 << ". SKIP");
        }
        initialized = سچ;
    }
    خالی بھرو_Plugin();
};

ساکن
std::vector<File_System_راستہ_t> getPluginCandidates(ثابت std::string& بنیاد_Name)
{
    استعمال نامخلا cv::utils;
    استعمال نامخلا cv::utils::fs;
    ثابت std::string بنیاد_Name_l = مقصد_زیریں_Case(بنیاد_Name);
    ثابت std::string بنیاد_Name_u = مقصد_بالائی_Case(بنیاد_Name);
    ثابت File_System_راستہ_t بنیاد_Name_l_fs = مقصد_File_System_راستہ(بنیاد_Name_l);
    std::vector<File_System_راستہ_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    اگر (paths_.size() != 0)
    {
        برائے (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(مقصد_File_System_راستہ(paths_[i]));
        }
    }
    ورنہ
    {
        File_System_راستہ_t ثنائی_Location;
        اگر (getBinLocation(ثنائی_Location))
        {
            ثنائی_Location = get_والد(ثنائی_Location);
#اگر_غیر_تعریف_شدہ CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ثنائی_Location);
#ورنہ
            paths.push_back(ثنائی_Location + مقصد_File_System_راستہ("/") + مقصد_File_System_راستہ(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#اگر_ختم
        }
    }
    ثابت std::string default_اظہاریہ = library_سابقہ() + "opencv_core_parallel_" + بنیاد_Name_l + "*" + librarySuffix();
    ثابت std::string plugin_اظہاریہ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + بنیاد_Name_u).c_str(), default_اظہاریہ.c_str());
    std::vector<File_System_راستہ_t> results;
#اگر_تعریف_شدہ _WIN32
    File_System_راستہ_t moduleName = مقصد_File_System_راستہ(library_سابقہ() + "opencv_core_parallel_" + بنیاد_Name_l + librarySuffix());
    اگر (plugin_اظہاریہ != default_اظہاریہ)
    {
        moduleName = مقصد_File_System_راستہ(plugin_اظہاریہ);
        results.push_back(moduleName);
    }
    برائے (ثابت File_System_راستہ_t& راستہ : paths)
    {
        results.push_back(راستہ + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#ورنہ
    CV_LOG_DEBUG(تہی, "core(parallel): " << بنیاد_Name << " plugin's glob is '" << plugin_اظہاریہ << "', " << paths.size() << " location(s)");
    برائے (ثابت std::string& راستہ : paths)
    {
        اگر (راستہ.empty())
            جاری;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(راستہ, plugin_اظہاریہ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(تہی, "    - " << راستہ << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#اگر_ختم
    CV_LOG_DEBUG(تہی, "Found " << results.size() << " plugin(s) for " << بنیاد_Name);
    واپس results;
}

خالی PluginParallelBackendFactory::بھرو_Plugin()
{
    برائے (ثابت File_System_راستہ_t& plugin : getPluginCandidates(بنیاد_Name_2))
    {
        خودکار lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        اگر (!lib->isLoaded())
        {
            جاری;
        }
        کوشش
        {
            خودکار pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            اگر (!pluginBackend)
            {
                جاری;
            }
            اگر (pluginBackend->plugin_api_ == تہی)
            {
                CV_LOG_ERROR(تہی, "core(parallel): no compatible plugin API for backend: " << بنیاد_Name_2 << " in " << مقصد_Printable_راستہ(plugin));
                جاری;
            }
#اگر !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#اگر_ختم
            backend = pluginBackend;
            واپس;
        }
        پکڑو (...)
        {
            CV_LOG_WARNING(تہی, "core(parallel): exception during plugin initialization: " << مقصد_Printable_راستہ(plugin) << ". SKIP");
        }
    }
}

#اگر_ختم  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نامخلا parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& بنیاد_Name)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    واپس std::make_shared<impl::PluginParallelBackendFactory>(بنیاد_Name);
#ورنہ
    CV_غیر_مستعمل(بنیاد_Name);
    واپس std::shared_ptr<IParallelBackendFactory>();
#اگر_ختم
}

}}  // namespace
