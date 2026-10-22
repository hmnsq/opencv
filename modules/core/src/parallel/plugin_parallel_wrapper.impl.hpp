// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

نطاق cv { نطاق impl {

using نطاق cv::parallel;

#𐤀𐤌 OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using نطاق cv::plugin::impl;  // plugin_loader.hpp

صنف PluginParallelBackend CV_FINAL: عام std::enable_shared_from_this<PluginParallelBackend>
{
محمي:
    فراغ initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->getSymbol(init_name));
        𐤀𐤌 (fn_init)
        {
            CV_LOG_DEBUG(عدم, "Found entry: '" << init_name << "'");
            لكل (صحيح supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, عدم);
                𐤀𐤌 (plugin_api_)
                    اكسر;
            }
            𐤀𐤌 (!plugin_api_)
            {
                CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                أرجع;
            }
            𐤀𐤌 (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, خطأ))
            {
                plugin_api_ = عدم;
                أرجع;
            }
            CV_LOG_INFO(عدم, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقي checkCompatibility(ثابت OpenCV_API_Header& api_header, unsigned صحيح abi_version, unsigned صحيح api_version, منطقي checkMinorOpenCVVersion)
    {
        𐤀𐤌 (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            أرجع خطأ;
        }
        𐤀𐤌 (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وإلا 𐤀𐤌 (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            أرجع خطأ;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        𐤀𐤌 (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(عدم, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            أرجع خطأ;
        }
        𐤀𐤌 (api_header.api_version != api_version)
        {
            CV_LOG_INFO(عدم, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            𐤀𐤌 (api_header.api_version < api_version)
            {
                CV_LOG_INFO(عدم, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        أرجع true;
    }

عام:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ثابت OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ثابت std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(عدم)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = عدم;

        𐤀𐤌 (plugin_api_->v0.getInstance)
        {
            𐤀𐤌 (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                أرجع std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        أرجع std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


صنف PluginParallelBackendFactory CV_FINAL: عام IParallelBackendFactory
{
عام:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقي initialized;
عام:
    PluginParallelBackendFactory(ثابت std::string& baseName)
        : baseName_(baseName)
        , initialized(خطأ)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        𐤀𐤌 (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(هذا)->initBackend();
        }
        𐤀𐤌 (backend)
            أرجع backend->create();
        أرجع std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
محمي:
    فراغ initBackend()
    {
        AutoLock lock(getInitializationMutex());
        حاول
        {
            𐤀𐤌 (!initialized)
                loadPlugin();
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = true;
    }
    فراغ loadPlugin();
};

ساكن
std::vector<FileSystemPath_t> getPluginCandidates(ثابت std::string& baseName)
{
    using نطاق cv::utils;
    using نطاق cv::utils::fs;
    ثابت std::string baseName_l = toLowerCase(baseName);
    ثابت std::string baseName_u = toUpperCase(baseName);
    ثابت FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    𐤀𐤌 (paths_.size() != 0)
    {
        لكل (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    وإلا
    {
        FileSystemPath_t binaryLocation;
        𐤀𐤌 (getBinLocation(binaryLocation))
        {
            binaryLocation = getParent(binaryLocation);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(binaryLocation);
#وإلا
            paths.push_back(binaryLocation + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ثابت std::string default_expr = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ثابت std::string plugin_expr = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_expr.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    𐤀𐤌 (plugin_expr != default_expr)
    {
        moduleName = toFileSystemPath(plugin_expr);
        results.push_back(moduleName);
    }
    لكل (ثابت FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وإلا
    CV_LOG_DEBUG(عدم, "core(parallel): " << baseName << " plugin's glob is '" << plugin_expr << "', " << paths.size() << " location(s)");
    لكل (ثابت std::string& path : paths)
    {
        𐤀𐤌 (path.empty())
            استمر;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_expr), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(عدم, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(عدم, "Found " << results.size() << " plugin(s) for " << baseName);
    أرجع results;
}

فراغ PluginParallelBackendFactory::loadPlugin()
{
    لكل (ثابت FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        𐤀𐤌 (!lib->isLoaded())
        {
            استمر;
        }
        حاول
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            𐤀𐤌 (!pluginBackend)
            {
                استمر;
            }
            𐤀𐤌 (pluginBackend->plugin_api_ == عدم)
            {
                CV_LOG_ERROR(عدم, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                استمر;
            }
#𐤀𐤌 !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            أرجع;
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نطاق parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& baseName)
{
#𐤀𐤌 OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    أرجع std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#وإلا
    CV_UNUSED(baseName);
    أرجع std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
