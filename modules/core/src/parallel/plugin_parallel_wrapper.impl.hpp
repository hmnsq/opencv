// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ impl {

using ನಾಮಾಕಾಶ cv::parallel;

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using ನಾಮಾಕಾಶ cv::plugin::impl;  // plugin_loader.hpp

ವರ್ಗ PluginParallelBackend CV_FINAL: ಸಾರ್ವಜನಿಕ std::enable_shared_from_this<PluginParallelBackend>
{
ರಕ್ಷಿತ:
    ಶೂನ್ಯ initPluginAPI()
    {
        ಧ್ರುವ ವರ್ಣ* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_ಚಿಹ್ನೆ(init_name));
        ಯದಿ (fn_init)
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "Found entry: '" << init_name << "'");
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, ರಿಕ್ತ);
                ಯದಿ (plugin_api_)
                    ಭಙ್ಗ;
            }
            ಯದಿ (!plugin_api_)
            {
                CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ಪ್ರತ್ಯಾಗಮ;
            }
            ಯದಿ (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, ಅಸತ್ಯ))
            {
                plugin_api_ = ರಿಕ್ತ;
                ಪ್ರತ್ಯಾಗಮ;
            }
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    ತಾರ್ಕಿಕ checkCompatibility(ಧ್ರುವ OpenCV_API_Header& api_header, unsigned ಪೂರ್ಣಾಙ್ಕ abi_version, unsigned ಪೂರ್ಣಾಙ್ಕ api_version, ತಾರ್ಕಿಕ checkMinorOpenCVVersion)
    {
        ಯದಿ (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಯದಿ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        ಅನ್ಯಥಾ ಯದಿ (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        ಯದಿ (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ರಿಕ್ತ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಯದಿ (api_header.api_version != api_version)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            ಯದಿ (api_header.api_version < api_version)
            {
                CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

ಸಾರ್ವಜನಿಕ:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ಧ್ರುವ OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ಧ್ರುವ std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ರಿಕ್ತ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ಧ್ರುವ
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ರಿಕ್ತ;

        ಯದಿ (plugin_api_->v0.getInstance)
        {
            ಯದಿ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ಪ್ರತ್ಯಾಗಮ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ಪ್ರತ್ಯಾಗಮ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


ವರ್ಗ PluginParallelBackendFactory CV_FINAL: ಸಾರ್ವಜನಿಕ IParallelBackendFactory
{
ಸಾರ್ವಜನಿಕ:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    ತಾರ್ಕಿಕ initialized;
ಸಾರ್ವಜನಿಕ:
    PluginParallelBackendFactory(ಧ್ರುವ std::string& baseName)
        : baseName_(baseName)
        , initialized(ಅಸತ್ಯ)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ಏತತ್)->initBackend();
        }
        ಯದಿ (backend)
            ಪ್ರತ್ಯಾಗಮ backend->create();
        ಪ್ರತ್ಯಾಗಮ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
ರಕ್ಷಿತ:
    ಶೂನ್ಯ initBackend()
    {
        AutoLock lock(getInitializationMutex());
        ಯತ್ನ
        {
            ಯದಿ (!initialized)
                loadPlugin();
        }
        ಗೃಹಾಣ (...)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = ಸತ್ಯ;
    }
    ಶೂನ್ಯ loadPlugin();
};

ಸ್ಥಿರ
std::vector<FileSystemPath_t> getPluginCandidates(ಧ್ರುವ std::string& baseName)
{
    using ನಾಮಾಕಾಶ cv::utils;
    using ನಾಮಾಕಾಶ cv::utils::fs;
    ಧ್ರುವ std::string baseName_l = toLowerCase(baseName);
    ಧ್ರುವ std::string baseName_u = toUpperCase(baseName);
    ಧ್ರುವ FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ಧ್ರುವ std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ಯದಿ (paths_.size() != 0)
    {
        ಪ್ರತಿ (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    ಅನ್ಯಥಾ
    {
        FileSystemPath_t ದ್ವಿಮಾನ_Location;
        ಯದಿ (getBinLocation(ದ್ವಿಮಾನ_Location))
        {
            ದ್ವಿಮಾನ_Location = getParent(ದ್ವಿಮಾನ_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ದ್ವಿಮಾನ_Location);
#ಅನ್ಯಥಾ
            paths.push_back(ದ್ವಿಮಾನ_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ಧ್ರುವ std::string default_ಅಭಿವ್ಯಕ್ತಿ = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ಧ್ರುವ std::string plugin_ಅಭಿವ್ಯಕ್ತಿ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_ಅಭಿವ್ಯಕ್ತಿ.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    ಯದಿ (plugin_ಅಭಿವ್ಯಕ್ತಿ != default_ಅಭಿವ್ಯಕ್ತಿ)
    {
        moduleName = toFileSystemPath(plugin_ಅಭಿವ್ಯಕ್ತಿ);
        results.push_back(moduleName);
    }
    ಪ್ರತಿ (ಧ್ರುವ FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#ಅನ್ಯಥಾ
    CV_LOG_DEBUG(ರಿಕ್ತ, "core(parallel): " << baseName << " plugin's glob is '" << plugin_ಅಭಿವ್ಯಕ್ತಿ << "', " << paths.size() << " location(s)");
    ಪ್ರತಿ (ಧ್ರುವ std::string& path : paths)
    {
        ಯದಿ (path.empty())
            ಅನುವರ್ತ;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_ಅಭಿವ್ಯಕ್ತಿ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ರಿಕ್ತ, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(ರಿಕ್ತ, "Found " << results.size() << " plugin(s) for " << baseName);
    ಪ್ರತ್ಯಾಗಮ results;
}

ಶೂನ್ಯ PluginParallelBackendFactory::loadPlugin()
{
    ಪ್ರತಿ (ಧ್ರುವ FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ಯದಿ (!lib->isLoaded())
        {
            ಅನುವರ್ತ;
        }
        ಯತ್ನ
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ಯದಿ (!pluginBackend)
            {
                ಅನುವರ್ತ;
            }
            ಯದಿ (pluginBackend->plugin_api_ == ರಿಕ್ತ)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                ಅನುವರ್ತ;
            }
#ಯದಿ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಗೃಹಾಣ (...)
        {
            CV_LOG_WARNING(ರಿಕ್ತ, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

ನಾಮಾಕಾಶ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ಧ್ರುವ std::string& baseName)
{
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ಪ್ರತ್ಯಾಗಮ std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#ಅನ್ಯಥಾ
    CV_UNUSED(baseName);
    ಪ್ರತ್ಯಾಗಮ std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
