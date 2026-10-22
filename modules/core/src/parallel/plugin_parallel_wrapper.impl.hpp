// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#অন্তর্ভাব "opencv2/core/utils/plugin_loader.private.hpp"

নামাকাশ cv { নামাকাশ impl {

using নামাকাশ cv::parallel;

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using নামাকাশ cv::plugin::impl;  // plugin_loader.hpp

শ্রেণী PluginParallelBackend CV_FINAL: সর্বজনীন std::enable_shared_from_this<PluginParallelBackend>
{
সুরক্ষিত:
    শূন্য initPluginAPI()
    {
        ধ্রুবক অক্ষর* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_প্রতীক(init_name));
        যদি (fn_init)
        {
            CV_LOG_DEBUG(ফাঁকা, "Found entry: '" << init_name << "'");
            জন্য (পূর্ণসংখ্যা supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, ফাঁকা);
                যদি (plugin_api_)
                    বিরতি;
            }
            যদি (!plugin_api_)
            {
                CV_LOG_INFO(ফাঁকা, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ফেরত;
            }
            যদি (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, মিথ্যা))
            {
                plugin_api_ = ফাঁকা;
                ফেরত;
            }
            CV_LOG_INFO(ফাঁকা, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        অন্যথা
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    যৌক্তিক checkCompatibility(ধ্রুবক OpenCV_API_Header& api_header, unsigned পূর্ণসংখ্যা abi_version, unsigned পূর্ণসংখ্যা api_version, যৌক্তিক checkMinorOpenCVVersion)
    {
        যদি (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(ফাঁকা, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ফেরত মিথ্যা;
        }
        যদি (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        অন্যথা যদি (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(ফাঁকা, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ফেরত মিথ্যা;
        }
        CV_LOG_DEBUG(ফাঁকা, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        যদি (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ফাঁকা, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            ফেরত মিথ্যা;
        }
        যদি (api_header.api_version != api_version)
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            যদি (api_header.api_version < api_version)
            {
                CV_LOG_INFO(ফাঁকা, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ফেরত সত্য;
    }

সর্বজনীন:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ধ্রুবক OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ধ্রুবক std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ফাঁকা)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ধ্রুবক
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ফাঁকা;

        যদি (plugin_api_->v0.getInstance)
        {
            যদি (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ফেরত std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ফেরত std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


শ্রেণী PluginParallelBackendFactory CV_FINAL: সর্বজনীন IParallelBackendFactory
{
সর্বজনীন:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    যৌক্তিক initialized;
সর্বজনীন:
    PluginParallelBackendFactory(ধ্রুবক std::string& baseName)
        : baseName_(baseName)
        , initialized(মিথ্যা)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ধ্রুবক CV_OVERRIDE
    {
        যদি (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(এই)->initBackend();
        }
        যদি (backend)
            ফেরত backend->create();
        ফেরত std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
সুরক্ষিত:
    শূন্য initBackend()
    {
        AutoLock lock(getInitializationMutex());
        চেষ্টা
        {
            যদি (!initialized)
                loadPlugin();
        }
        ধরো (...)
        {
            CV_LOG_INFO(ফাঁকা, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = সত্য;
    }
    শূন্য loadPlugin();
};

স্থির
std::vector<FileSystemPath_t> getPluginCandidates(ধ্রুবক std::string& baseName)
{
    using নামাকাশ cv::utils;
    using নামাকাশ cv::utils::fs;
    ধ্রুবক std::string baseName_l = toLowerCase(baseName);
    ধ্রুবক std::string baseName_u = toUpperCase(baseName);
    ধ্রুবক FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ধ্রুবক std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    যদি (paths_.size() != 0)
    {
        জন্য (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    অন্যথা
    {
        FileSystemPath_t দ্বিমিক_Location;
        যদি (getBinLocation(দ্বিমিক_Location))
        {
            দ্বিমিক_Location = getParent(দ্বিমিক_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(দ্বিমিক_Location);
#অন্যথা
            paths.push_back(দ্বিমিক_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ধ্রুবক std::string default_রাশিমালা = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ধ্রুবক std::string plugin_রাশিমালা = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_রাশিমালা.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    যদি (plugin_রাশিমালা != default_রাশিমালা)
    {
        moduleName = toFileSystemPath(plugin_রাশিমালা);
        results.push_back(moduleName);
    }
    জন্য (ধ্রুবক FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#অন্যথা
    CV_LOG_DEBUG(ফাঁকা, "core(parallel): " << baseName << " plugin's glob is '" << plugin_রাশিমালা << "', " << paths.size() << " location(s)");
    জন্য (ধ্রুবক std::string& path : paths)
    {
        যদি (path.empty())
            চালু;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_রাশিমালা), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ফাঁকা, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(ফাঁকা, "Found " << results.size() << " plugin(s) for " << baseName);
    ফেরত results;
}

শূন্য PluginParallelBackendFactory::loadPlugin()
{
    জন্য (ধ্রুবক FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        যদি (!lib->isLoaded())
        {
            চালু;
        }
        চেষ্টা
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            যদি (!pluginBackend)
            {
                চালু;
            }
            যদি (pluginBackend->plugin_api_ == ফাঁকা)
            {
                CV_LOG_ERROR(ফাঁকা, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                চালু;
            }
#যদি !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            ফেরত;
        }
        ধরো (...)
        {
            CV_LOG_WARNING(ফাঁকা, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

নামাকাশ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ধ্রুবক std::string& baseName)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ফেরত std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#অন্যথা
    CV_UNUSED(baseName);
    ফেরত std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
