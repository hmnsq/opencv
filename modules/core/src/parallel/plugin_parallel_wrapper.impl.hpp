// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#அந்தர்பாவ "opencv2/core/utils/plugin_loader.private.hpp"

நாமாகாஶ cv { நாமாகாஶ impl {

using நாமாகாஶ cv::parallel;

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using நாமாகாஶ cv::plugin::impl;  // plugin_loader.hpp

வகுப்பு PluginParallelBackend CV_FINAL: பொது std::enable_shared_from_this<PluginParallelBackend>
{
பாதுகாக்கப்பட்ட:
    வெறுமை initPluginAPI()
    {
        மாறிலி எழுத்து* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_குறியீடு(init_name));
        எனில் (fn_init)
        {
            CV_LOG_DEBUG(வெற்று, "Found entry: '" << init_name << "'");
            க்கு (முழு supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, வெற்று);
                எனில் (plugin_api_)
                    நிறுத்து;
            }
            எனில் (!plugin_api_)
            {
                CV_LOG_INFO(வெற்று, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                திரும்பு;
            }
            எனில் (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, பொய்))
            {
                plugin_api_ = வெற்று;
                திரும்பு;
            }
            CV_LOG_INFO(வெற்று, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        இல்லையெனில்
        {
            CV_LOG_INFO(வெற்று, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    தருக்க checkCompatibility(மாறிலி OpenCV_API_Header& api_header, unsigned முழு abi_version, unsigned முழு api_version, தருக்க checkMinorOpenCVVersion)
    {
        எனில் (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(வெற்று, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            திரும்பு பொய்;
        }
        எனில் (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        இல்லையெனில் எனில் (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(வெற்று, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            திரும்பு பொய்;
        }
        CV_LOG_DEBUG(வெற்று, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        எனில் (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(வெற்று, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            திரும்பு பொய்;
        }
        எனில் (api_header.api_version != api_version)
        {
            CV_LOG_INFO(வெற்று, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            எனில் (api_header.api_version < api_version)
            {
                CV_LOG_INFO(வெற்று, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        திரும்பு உண்மை;
    }

பொது:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    மாறிலி OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(மாறிலி std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(வெற்று)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() மாறிலி
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = வெற்று;

        எனில் (plugin_api_->v0.getInstance)
        {
            எனில் (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                திரும்பு std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        திரும்பு std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


வகுப்பு PluginParallelBackendFactory CV_FINAL: பொது IParallelBackendFactory
{
பொது:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    தருக்க initialized;
பொது:
    PluginParallelBackendFactory(மாறிலி std::string& baseName)
        : baseName_(baseName)
        , initialized(பொய்)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() மாறிலி CV_OVERRIDE
    {
        எனில் (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(இது)->initBackend();
        }
        எனில் (backend)
            திரும்பு backend->create();
        திரும்பு std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
பாதுகாக்கப்பட்ட:
    வெறுமை initBackend()
    {
        AutoLock lock(getInitializationMutex());
        முயல்
        {
            எனில் (!initialized)
                loadPlugin();
        }
        பிடி (...)
        {
            CV_LOG_INFO(வெற்று, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = உண்மை;
    }
    வெறுமை loadPlugin();
};

நிலையான
std::vector<FileSystemPath_t> getPluginCandidates(மாறிலி std::string& baseName)
{
    using நாமாகாஶ cv::utils;
    using நாமாகாஶ cv::utils::fs;
    மாறிலி std::string baseName_l = toLowerCase(baseName);
    மாறிலி std::string baseName_u = toUpperCase(baseName);
    மாறிலி FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    மாறிலி std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    எனில் (paths_.size() != 0)
    {
        க்கு (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    இல்லையெனில்
    {
        FileSystemPath_t இருமம்_Location;
        எனில் (getBinLocation(இருமம்_Location))
        {
            இருமம்_Location = getParent(இருமம்_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(இருமம்_Location);
#இல்லையெனில்
            paths.push_back(இருமம்_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    மாறிலி std::string default_கோவை = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    மாறிலி std::string plugin_கோவை = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_கோவை.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    எனில் (plugin_கோவை != default_கோவை)
    {
        moduleName = toFileSystemPath(plugin_கோவை);
        results.push_back(moduleName);
    }
    க்கு (மாறிலி FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#இல்லையெனில்
    CV_LOG_DEBUG(வெற்று, "core(parallel): " << baseName << " plugin's glob is '" << plugin_கோவை << "', " << paths.size() << " location(s)");
    க்கு (மாறிலி std::string& path : paths)
    {
        எனில் (path.empty())
            தொடர்;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_கோவை), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(வெற்று, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(வெற்று, "Found " << results.size() << " plugin(s) for " << baseName);
    திரும்பு results;
}

வெறுமை PluginParallelBackendFactory::loadPlugin()
{
    க்கு (மாறிலி FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        எனில் (!lib->isLoaded())
        {
            தொடர்;
        }
        முயல்
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            எனில் (!pluginBackend)
            {
                தொடர்;
            }
            எனில் (pluginBackend->plugin_api_ == வெற்று)
            {
                CV_LOG_ERROR(வெற்று, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                தொடர்;
            }
#எனில் !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            திரும்பு;
        }
        பிடி (...)
        {
            CV_LOG_WARNING(வெற்று, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

நாமாகாஶ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(மாறிலி std::string& baseName)
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    திரும்பு std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#இல்லையெனில்
    CV_UNUSED(baseName);
    திரும்பு std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
