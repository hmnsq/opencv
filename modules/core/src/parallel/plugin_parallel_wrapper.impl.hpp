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

#जर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using नामाकाश cv::plugin::impl;  // plugin_loader.hpp

वर्ग PluginParallelBackend CV_FINAL: सार्वजनिक std::enable_shared_from_this<PluginParallelBackend>
{
संरक्षित:
    शून्य initPluginAPI()
    {
        स्थिरांक अक्षर* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_चिन्ह(init_name));
        जर (fn_init)
        {
            CV_LOG_DEBUG(रिक्त, "Found entry: '" << init_name << "'");
            साठी (पूर्णांक supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, रिक्त);
                जर (plugin_api_)
                    थांबा;
            }
            जर (!plugin_api_)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                परत;
            }
            जर (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, असत्य))
            {
                plugin_api_ = रिक्त;
                परत;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        अन्यथा
        {
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    तार्किक checkCompatibility(स्थिरांक OpenCV_API_Header& api_header, अचिन्हित पूर्णांक abi_version, अचिन्हित पूर्णांक api_version, तार्किक checkMinorOpenCVVersion)
    {
        जर (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            परत असत्य;
        }
        जर (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        अन्यथा जर (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            परत असत्य;
        }
        CV_LOG_DEBUG(रिक्त, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        जर (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(रिक्त, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            परत असत्य;
        }
        जर (api_header.api_version != api_version)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            जर (api_header.api_version < api_version)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        परत सत्य;
    }

सार्वजनिक:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    स्थिरांक OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(स्थिरांक std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(रिक्त)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिरांक
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = रिक्त;

        जर (plugin_api_->v0.getInstance)
        {
            जर (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                परत std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        परत std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


वर्ग PluginParallelBackendFactory CV_FINAL: सार्वजनिक IParallelBackendFactory
{
सार्वजनिक:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    तार्किक initialized;
सार्वजनिक:
    PluginParallelBackendFactory(स्थिरांक std::string& baseName)
        : baseName_(baseName)
        , initialized(असत्य)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिरांक CV_OVERRIDE
    {
        जर (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(हे)->initBackend();
        }
        जर (backend)
            परत backend->create();
        परत std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
संरक्षित:
    शून्य initBackend()
    {
        AutoLock lock(getInitializationMutex());
        प्रयत्न
        {
            जर (!initialized)
                loadPlugin();
        }
        पकडा (...)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = सत्य;
    }
    शून्य loadPlugin();
};

स्थिर
std::vector<FileSystemPath_t> getPluginCandidates(स्थिरांक std::string& baseName)
{
    using नामाकाश cv::utils;
    using नामाकाश cv::utils::fs;
    स्थिरांक std::string baseName_l = toLowerCase(baseName);
    स्थिरांक std::string baseName_u = toUpperCase(baseName);
    स्थिरांक FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    स्थिरांक std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    जर (paths_.size() != 0)
    {
        साठी (माप_प्रकार i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    अन्यथा
    {
        FileSystemPath_t द्विमान_Location;
        जर (getBinLocation(द्विमान_Location))
        {
            द्विमान_Location = getParent(द्विमान_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(द्विमान_Location);
#अन्यथा
            paths.push_back(द्विमान_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    स्थिरांक std::string default_अभिव्यक्ती = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    स्थिरांक std::string plugin_अभिव्यक्ती = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_अभिव्यक्ती.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    जर (plugin_अभिव्यक्ती != default_अभिव्यक्ती)
    {
        moduleName = toFileSystemPath(plugin_अभिव्यक्ती);
        results.push_back(moduleName);
    }
    साठी (स्थिरांक FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#अन्यथा
    CV_LOG_DEBUG(रिक्त, "core(parallel): " << baseName << " plugin's glob is '" << plugin_अभिव्यक्ती << "', " << paths.size() << " location(s)");
    साठी (स्थिरांक std::string& path : paths)
    {
        जर (path.empty())
            सुरूठेवा;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_अभिव्यक्ती), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(रिक्त, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(रिक्त, "Found " << results.size() << " plugin(s) for " << baseName);
    परत results;
}

शून्य PluginParallelBackendFactory::loadPlugin()
{
    साठी (स्थिरांक FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        स्वतः lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        जर (!lib->isLoaded())
        {
            सुरूठेवा;
        }
        प्रयत्न
        {
            स्वतः pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            जर (!pluginBackend)
            {
                सुरूठेवा;
            }
            जर (pluginBackend->plugin_api_ == रिक्त)
            {
                CV_LOG_ERROR(रिक्त, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                सुरूठेवा;
            }
#जर !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            परत;
        }
        पकडा (...)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

नामाकाश parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(स्थिरांक std::string& baseName)
{
#जर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    परत std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#अन्यथा
    CV_UNUSED(baseName);
    परत std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
