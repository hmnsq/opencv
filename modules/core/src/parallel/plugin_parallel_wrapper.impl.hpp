// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"

नामाकाश cv { नामाकाश impl {

using नामाकाश cv::parallel;

#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using नामाकाश cv::plugin::impl;  // plugin_loader.hpp

वग्ग PluginParallelBackend CV_FINAL: सार्वजनिक std::enable_shared_from_this<PluginParallelBackend>
{
रक्षित:
    शून्य initPluginAPI()
    {
        ध्रुव वर्ण* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_चिह्नम्(init_name));
        सचे (fn_init)
        {
            CV_LOG_DEBUG(रिक्त, "Found entry: '" << init_name << "'");
            अत्थाय (पूर्णाङ्क supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, रिक्त);
                सचे (plugin_api_)
                    भङ्ग;
            }
            सचे (!plugin_api_)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                पटिनिवत्त;
            }
            सचे (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, मिच्छा))
            {
                plugin_api_ = रिक्त;
                पटिनिवत्त;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        अञ्ञथा
        {
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    तार्किक checkCompatibility(ध्रुव OpenCV_API_Header& api_header, unsigned पूर्णाङ्क abi_version, unsigned पूर्णाङ्क api_version, तार्किक checkMinorOpenCVVersion)
    {
        सचे (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            पटिनिवत्त मिच्छा;
        }
        सचे (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        अञ्ञथा सचे (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            पटिनिवत्त मिच्छा;
        }
        CV_LOG_DEBUG(रिक्त, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        सचे (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(रिक्त, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            पटिनिवत्त मिच्छा;
        }
        सचे (api_header.api_version != api_version)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            सचे (api_header.api_version < api_version)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        पटिनिवत्त सच्च;
    }

सार्वजनिक:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ध्रुव OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ध्रुव std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(रिक्त)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ध्रुव
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = रिक्त;

        सचे (plugin_api_->v0.getInstance)
        {
            सचे (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                पटिनिवत्त std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        पटिनिवत्त std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


वग्ग PluginParallelBackendFactory CV_FINAL: सार्वजनिक IParallelBackendFactory
{
सार्वजनिक:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    तार्किक initialized;
सार्वजनिक:
    PluginParallelBackendFactory(ध्रुव std::string& baseName)
        : baseName_(baseName)
        , initialized(मिच्छा)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ध्रुव CV_OVERRIDE
    {
        सचे (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(एतत्)->initBackend();
        }
        सचे (backend)
            पटिनिवत्त backend->create();
        पटिनिवत्त std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
रक्षित:
    शून्य initBackend()
    {
        AutoLock lock(getInitializationMutex());
        यत्न
        {
            सचे (!initialized)
                loadPlugin();
        }
        गृहाण (...)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = सच्च;
    }
    शून्य loadPlugin();
};

स्थिर
std::vector<FileSystemPath_t> getPluginCandidates(ध्रुव std::string& baseName)
{
    using नामाकाश cv::utils;
    using नामाकाश cv::utils::fs;
    ध्रुव std::string baseName_l = toLowerCase(baseName);
    ध्रुव std::string baseName_u = toUpperCase(baseName);
    ध्रुव FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ध्रुव std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    सचे (paths_.size() != 0)
    {
        अत्थाय (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    अञ्ञथा
    {
        FileSystemPath_t द्विमान_Location;
        सचे (getBinLocation(द्विमान_Location))
        {
            द्विमान_Location = getParent(द्विमान_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(द्विमान_Location);
#अञ्ञथा
            paths.push_back(द्विमान_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ध्रुव std::string default_व्यञ्जकम् = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ध्रुव std::string plugin_व्यञ्जकम् = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_व्यञ्जकम्.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    सचे (plugin_व्यञ्जकम् != default_व्यञ्जकम्)
    {
        moduleName = toFileSystemPath(plugin_व्यञ्जकम्);
        results.push_back(moduleName);
    }
    अत्थाय (ध्रुव FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#अञ्ञथा
    CV_LOG_DEBUG(रिक्त, "core(parallel): " << baseName << " plugin's glob is '" << plugin_व्यञ्जकम् << "', " << paths.size() << " location(s)");
    अत्थाय (ध्रुव std::string& path : paths)
    {
        सचे (path.empty())
            अनुवर्त;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_व्यञ्जकम्), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(रिक्त, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(रिक्त, "Found " << results.size() << " plugin(s) for " << baseName);
    पटिनिवत्त results;
}

शून्य PluginParallelBackendFactory::loadPlugin()
{
    अत्थाय (ध्रुव FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        सचे (!lib->isLoaded())
        {
            अनुवर्त;
        }
        यत्न
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            सचे (!pluginBackend)
            {
                अनुवर्त;
            }
            सचे (pluginBackend->plugin_api_ == रिक्त)
            {
                CV_LOG_ERROR(रिक्त, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                अनुवर्त;
            }
#सचे !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            पटिनिवत्त;
        }
        गृहाण (...)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

नामाकाश parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ध्रुव std::string& baseName)
{
#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    पटिनिवत्त std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#अञ्ञथा
    CV_UNUSED(baseName);
    पटिनिवत्त std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
