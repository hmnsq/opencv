// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#شامل "opencv2/core/utils/plugin_loader.private.hpp"

فضاینام cv { فضاینام impl {

using فضاینام cv::parallel;

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using فضاینام cv::plugin::impl;  // plugin_loader.hpp

رده PluginParallelBackend CV_FINAL: عمومی std::enable_shared_from_this<PluginParallelBackend>
{
محافظتشده:
    تهی initPluginAPI()
    {
        ثابت نویسه* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_نماد(init_name));
        اگر (fn_init)
        {
            CV_LOG_DEBUG(پوچ, "Found entry: '" << init_name << "'");
            برای (صحیح supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, پوچ);
                اگر (plugin_api_)
                    بشکن;
            }
            اگر (!plugin_api_)
            {
                CV_LOG_INFO(پوچ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                بازگشت;
            }
            اگر (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, نادرست))
            {
                plugin_api_ = پوچ;
                بازگشت;
            }
            CV_LOG_INFO(پوچ, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        وگرنه
        {
            CV_LOG_INFO(پوچ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقی checkCompatibility(ثابت OpenCV_API_Header& api_header, unsigned صحیح abi_version, unsigned صحیح api_version, منطقی checkMinorOpenCVVersion)
    {
        اگر (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(پوچ, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            بازگشت نادرست;
        }
        اگر (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وگرنه اگر (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(پوچ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            بازگشت نادرست;
        }
        CV_LOG_DEBUG(پوچ, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        اگر (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(پوچ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            بازگشت نادرست;
        }
        اگر (api_header.api_version != api_version)
        {
            CV_LOG_INFO(پوچ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            اگر (api_header.api_version < api_version)
            {
                CV_LOG_INFO(پوچ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        بازگشت درست;
    }

عمومی:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ثابت OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ثابت std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(پوچ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = پوچ;

        اگر (plugin_api_->v0.getInstance)
        {
            اگر (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                بازگشت std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        بازگشت std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


رده PluginParallelBackendFactory CV_FINAL: عمومی IParallelBackendFactory
{
عمومی:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقی initialized;
عمومی:
    PluginParallelBackendFactory(ثابت std::string& baseName)
        : baseName_(baseName)
        , initialized(نادرست)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        اگر (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(این)->initBackend();
        }
        اگر (backend)
            بازگشت backend->create();
        بازگشت std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
محافظتشده:
    تهی initBackend()
    {
        AutoLock lock(getInitializationMutex());
        تلاش
        {
            اگر (!initialized)
                loadPlugin();
        }
        بگیر (...)
        {
            CV_LOG_INFO(پوچ, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = درست;
    }
    تهی loadPlugin();
};

ایستا
std::vector<FileSystemPath_t> getPluginCandidates(ثابت std::string& baseName)
{
    using فضاینام cv::utils;
    using فضاینام cv::utils::fs;
    ثابت std::string baseName_l = toLowerCase(baseName);
    ثابت std::string baseName_u = toUpperCase(baseName);
    ثابت FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    اگر (paths_.size() != 0)
    {
        برای (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    وگرنه
    {
        FileSystemPath_t دودویی_Location;
        اگر (getBinLocation(دودویی_Location))
        {
            دودویی_Location = getParent(دودویی_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(دودویی_Location);
#وگرنه
            paths.push_back(دودویی_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ثابت std::string default_عبارت = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ثابت std::string plugin_عبارت = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_عبارت.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    اگر (plugin_عبارت != default_عبارت)
    {
        moduleName = toFileSystemPath(plugin_عبارت);
        results.push_back(moduleName);
    }
    برای (ثابت FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وگرنه
    CV_LOG_DEBUG(پوچ, "core(parallel): " << baseName << " plugin's glob is '" << plugin_عبارت << "', " << paths.size() << " location(s)");
    برای (ثابت std::string& path : paths)
    {
        اگر (path.empty())
            ادامه;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_عبارت), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(پوچ, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(پوچ, "Found " << results.size() << " plugin(s) for " << baseName);
    بازگشت results;
}

تهی PluginParallelBackendFactory::loadPlugin()
{
    برای (ثابت FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        اگر (!lib->isLoaded())
        {
            ادامه;
        }
        تلاش
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            اگر (!pluginBackend)
            {
                ادامه;
            }
            اگر (pluginBackend->plugin_api_ == پوچ)
            {
                CV_LOG_ERROR(پوچ, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                ادامه;
            }
#اگر !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            بازگشت;
        }
        بگیر (...)
        {
            CV_LOG_WARNING(پوچ, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

فضاینام parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& baseName)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    بازگشت std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#وگرنه
    CV_UNUSED(baseName);
    بازگشت std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
