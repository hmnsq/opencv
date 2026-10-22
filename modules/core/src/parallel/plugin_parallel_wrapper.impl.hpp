// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/plugin_loader.private.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ impl {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::parallel;

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::plugin::impl;  // plugin_loader.hpp

ਵਰਗ PluginParallelBackend CV_FINAL: ਜਨਤਕ std::enable_shared_from_this<PluginParallelBackend>
{
ਸੁਰੱਖਿਅਤ:
    ਖਾਲੀ initPluginAPI()
    {
        ਅਚਲ ਅੱਖਰ* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_ਚਿੰਨ੍ਹ(init_name));
        ਜੇ (fn_init)
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "Found entry: '" << init_name << "'");
            ਲਈ (ਪੂਰਨ supported_api_ਸੰਸਕਰਣ = API_ਸੰਸਕਰਣ; supported_api_ਸੰਸਕਰਣ >= 0; supported_api_ਸੰਸਕਰਣ--)
            {
                plugin_api_ = fn_init(ABI_ਸੰਸਕਰਣ, supported_api_ਸੰਸਕਰਣ, ਨਿਰਾ);
                ਜੇ (plugin_api_)
                    ਰੋਕੋ;
            }
            ਜੇ (!plugin_api_)
            {
                CV_LOG_INFO(ਨਿਰਾ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ਵਾਪਸ;
            }
            ਜੇ (!checkCompatibility(plugin_api_->api_ਸਿਰਲੇਖ, ABI_ਸੰਸਕਰਣ, API_ਸੰਸਕਰਣ, ਝੂਠ))
            {
                plugin_api_ = ਨਿਰਾ;
                ਵਾਪਸ;
            }
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): plugin is ready to use '" << plugin_api_->api_ਸਿਰਲੇਖ.api_description << "'");
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    ਤਾਰਕਿਕ checkCompatibility(ਅਚਲ Open_CV_API_ਸਿਰਲੇਖ& api_ਸਿਰਲੇਖ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ abi_ਸੰਸਕਰਣ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ api_ਸੰਸਕਰਣ, ਤਾਰਕਿਕ checkMinorOpenCVVersion)
    {
        ਜੇ (api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_major != CV_ਸੰਸਕਰਣ_MAJOR)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "core(parallel): wrong OpenCV major version used by plugin '" << api_ਸਿਰਲੇਖ.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_ਸੰਸਕਰਣ "'", api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_major, api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_minor))
            ਵਾਪਸ ਝੂਠ;
        }
        ਜੇ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        ਨਹੀਂਤਾਂ ਜੇ (api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_minor != CV_ਸੰਸਕਰਣ_MINOR)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_ਸਿਰਲੇਖ.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_ਸੰਸਕਰਣ "'", api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_major, api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_minor))
            ਵਾਪਸ ਝੂਠ;
        }
        CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): initialized '" << api_ਸਿਰਲੇਖ.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_major, api_ਸਿਰਲੇਖ.opencv_ਸੰਸਕਰਣ_minor,
                 api_ਸਿਰਲੇਖ.min_api_ਸੰਸਕਰਣ, api_ਸਿਰਲੇਖ.api_ਸੰਸਕਰਣ)
            << ", current OpenCV version is '" CV_ਸੰਸਕਰਣ "' (ABI/API = " << abi_ਸੰਸਕਰਣ << "/" << api_ਸੰਸਕਰਣ << ")"
        );
        ਜੇ (api_ਸਿਰਲੇਖ.min_api_ਸੰਸਕਰਣ != abi_ਸੰਸਕਰਣ)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ਨਿਰਾ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_ਸਿਰਲੇਖ.min_api_ਸੰਸਕਰਣ);
            ਵਾਪਸ ਝੂਠ;
        }
        ਜੇ (api_ਸਿਰਲੇਖ.api_ਸੰਸਕਰਣ != api_ਸੰਸਕਰਣ)
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_ਸਿਰਲੇਖ.api_ਸੰਸਕਰਣ, api_ਸੰਸਕਰਣ));
            ਜੇ (api_ਸਿਰਲੇਖ.api_ਸੰਸਕਰਣ < api_ਸੰਸਕਰਣ)
            {
                CV_LOG_INFO(ਨਿਰਾ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ਵਾਪਸ ਸੱਚ;
    }

ਜਨਤਕ:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ਅਚਲ OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ਅਚਲ std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ਨਿਰਾ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ਅਚਲ
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ਨਿਰਾ;

        ਜੇ (plugin_api_->v0.getInstance)
        {
            ਜੇ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ਵਾਪਸ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ਵਾਪਸ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


ਵਰਗ PluginParallelBackendFactory CV_FINAL: ਜਨਤਕ IParallelBackendFactory
{
ਜਨਤਕ:
    std::string ਅਧਾਰ_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    ਤਾਰਕਿਕ initialized;
ਜਨਤਕ:
    PluginParallelBackendFactory(ਅਚਲ std::string& ਅਧਾਰ_Name)
        : ਅਧਾਰ_Name_2(ਅਧਾਰ_Name)
        , initialized(ਝੂਠ)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ਇਹ)->initBackend();
        }
        ਜੇ (backend)
            ਵਾਪਸ backend->create();
        ਵਾਪਸ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
ਸੁਰੱਖਿਅਤ:
    ਖਾਲੀ initBackend()
    {
        Auto_ਤਾਲਾ lock(getInitializationMutex());
        ਕੋਸ਼ਿਸ਼
        {
            ਜੇ (!initialized)
                ਲੱਦੋ_Plugin();
        }
        ਫੜੋ (...)
        {
            CV_LOG_INFO(ਨਿਰਾ, "core(parallel): exception during plugin loading: " << ਅਧਾਰ_Name_2 << ". SKIP");
        }
        initialized = ਸੱਚ;
    }
    ਖਾਲੀ ਲੱਦੋ_Plugin();
};

ਸਥਿਰ
std::vector<File_System_ਰਸਤਾ_t> getPluginCandidates(ਅਚਲ std::string& ਅਧਾਰ_Name)
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::utils;
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::utils::fs;
    ਅਚਲ std::string ਅਧਾਰ_Name_l = ਮੰਜ਼ਿਲ_ਹੇਠਲਾ_Case(ਅਧਾਰ_Name);
    ਅਚਲ std::string ਅਧਾਰ_Name_u = ਮੰਜ਼ਿਲ_ਉੱਤਲਾ_Case(ਅਧਾਰ_Name);
    ਅਚਲ File_System_ਰਸਤਾ_t ਅਧਾਰ_Name_l_fs = ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ(ਅਧਾਰ_Name_l);
    std::vector<File_System_ਰਸਤਾ_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ਅਚਲ std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ਜੇ (paths_.size() != 0)
    {
        ਲਈ (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ(paths_[i]));
        }
    }
    ਨਹੀਂਤਾਂ
    {
        File_System_ਰਸਤਾ_t ਦੁਆਧਾਰੀ_Location;
        ਜੇ (getBinLocation(ਦੁਆਧਾਰੀ_Location))
        {
            ਦੁਆਧਾਰੀ_Location = get_ਮਾਪਾ(ਦੁਆਧਾਰੀ_Location);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ਦੁਆਧਾਰੀ_Location);
#ਨਹੀਂਤਾਂ
            paths.push_back(ਦੁਆਧਾਰੀ_Location + ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ("/") + ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#ਜੇ_ਅੰਤ
        }
    }
    ਅਚਲ std::string default_ਵਿਅੰਜਕ = library_ਅਗੇਤਰ() + "opencv_core_parallel_" + ਅਧਾਰ_Name_l + "*" + librarySuffix();
    ਅਚਲ std::string plugin_ਵਿਅੰਜਕ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + ਅਧਾਰ_Name_u).c_str(), default_ਵਿਅੰਜਕ.c_str());
    std::vector<File_System_ਰਸਤਾ_t> results;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    File_System_ਰਸਤਾ_t moduleName = ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ(library_ਅਗੇਤਰ() + "opencv_core_parallel_" + ਅਧਾਰ_Name_l + librarySuffix());
    ਜੇ (plugin_ਵਿਅੰਜਕ != default_ਵਿਅੰਜਕ)
    {
        moduleName = ਮੰਜ਼ਿਲ_File_System_ਰਸਤਾ(plugin_ਵਿਅੰਜਕ);
        results.push_back(moduleName);
    }
    ਲਈ (ਅਚਲ File_System_ਰਸਤਾ_t& ਰਸਤਾ : paths)
    {
        results.push_back(ਰਸਤਾ + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#ਨਹੀਂਤਾਂ
    CV_LOG_DEBUG(ਨਿਰਾ, "core(parallel): " << ਅਧਾਰ_Name << " plugin's glob is '" << plugin_ਵਿਅੰਜਕ << "', " << paths.size() << " location(s)");
    ਲਈ (ਅਚਲ std::string& ਰਸਤਾ : paths)
    {
        ਜੇ (ਰਸਤਾ.empty())
            ਜਾਰੀ;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(ਰਸਤਾ, plugin_ਵਿਅੰਜਕ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ਨਿਰਾ, "    - " << ਰਸਤਾ << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#ਜੇ_ਅੰਤ
    CV_LOG_DEBUG(ਨਿਰਾ, "Found " << results.size() << " plugin(s) for " << ਅਧਾਰ_Name);
    ਵਾਪਸ results;
}

ਖਾਲੀ PluginParallelBackendFactory::ਲੱਦੋ_Plugin()
{
    ਲਈ (ਅਚਲ File_System_ਰਸਤਾ_t& plugin : getPluginCandidates(ਅਧਾਰ_Name_2))
    {
        ਸਵੈਚਾਲਿਤ lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ਜੇ (!lib->isLoaded())
        {
            ਜਾਰੀ;
        }
        ਕੋਸ਼ਿਸ਼
        {
            ਸਵੈਚਾਲਿਤ pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ਜੇ (!pluginBackend)
            {
                ਜਾਰੀ;
            }
            ਜੇ (pluginBackend->plugin_api_ == ਨਿਰਾ)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "core(parallel): no compatible plugin API for backend: " << ਅਧਾਰ_Name_2 << " in " << ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(plugin));
                ਜਾਰੀ;
            }
#ਜੇ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#ਜੇ_ਅੰਤ
            backend = pluginBackend;
            ਵਾਪਸ;
        }
        ਫੜੋ (...)
        {
            CV_LOG_WARNING(ਨਿਰਾ, "core(parallel): exception during plugin initialization: " << ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(plugin) << ". SKIP");
        }
    }
}

#ਜੇ_ਅੰਤ  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

ਨਾਮਾਕਾਸ਼ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ਅਚਲ std::string& ਅਧਾਰ_Name)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ਵਾਪਸ std::make_shared<impl::PluginParallelBackendFactory>(ਅਧਾਰ_Name);
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ਅਧਾਰ_Name);
    ਵਾਪਸ std::shared_ptr<IParallelBackendFactory>();
#ਜੇ_ਅੰਤ
}

}}  // namespace
