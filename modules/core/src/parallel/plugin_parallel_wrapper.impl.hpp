// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/plugin_loader.private.hpp"

ନାମାକାଶ cv { ନାମାକାଶ impl {

using ନାମାକାଶ cv::parallel;

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using ନାମାକାଶ cv::plugin::impl;  // plugin_loader.hpp

ଶ୍ରେଣୀ PluginParallelBackend CV_FINAL: ସର୍ବସାଧାରଣ std::enable_shared_from_this<PluginParallelBackend>
{
ସୁରକ୍ଷିତ:
    ଶୂନ୍ୟ initPluginAPI()
    {
        ଅଚଳ ଅକ୍ଷର* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_ପ୍ରତୀକ(init_name));
        ଯଦି (fn_init)
        {
            CV_LOG_DEBUG(ଖାଲି, "Found entry: '" << init_name << "'");
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, ଖାଲି);
                ଯଦି (plugin_api_)
                    ଭାଙ୍ଗ;
            }
            ଯଦି (!plugin_api_)
            {
                CV_LOG_INFO(ଖାଲି, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ଫେରନ୍ତୁ;
            }
            ଯଦି (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, ମିଥ୍ୟା))
            {
                plugin_api_ = ଖାଲି;
                ଫେରନ୍ତୁ;
            }
            CV_LOG_INFO(ଖାଲି, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    ତାର୍କିକ checkCompatibility(ଅଚଳ OpenCV_API_Header& api_header, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା abi_version, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା api_version, ତାର୍କିକ checkMinorOpenCVVersion)
    {
        ଯଦି (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(ଖାଲି, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଯଦି (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        ଅନ୍ୟଥା ଯଦି (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(ଖାଲି, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        CV_LOG_DEBUG(ଖାଲି, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        ଯଦି (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ଖାଲି, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଯଦି (api_header.api_version != api_version)
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            ଯଦି (api_header.api_version < api_version)
            {
                CV_LOG_INFO(ଖାଲି, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

ସର୍ବସାଧାରଣ:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ଅଚଳ OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ଅଚଳ std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ଖାଲି)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ଅଚଳ
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ଖାଲି;

        ଯଦି (plugin_api_->v0.getInstance)
        {
            ଯଦି (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ଫେରନ୍ତୁ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ଫେରନ୍ତୁ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


ଶ୍ରେଣୀ PluginParallelBackendFactory CV_FINAL: ସର୍ବସାଧାରଣ IParallelBackendFactory
{
ସର୍ବସାଧାରଣ:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    ତାର୍କିକ initialized;
ସର୍ବସାଧାରଣ:
    PluginParallelBackendFactory(ଅଚଳ std::string& baseName)
        : baseName_(baseName)
        , initialized(ମିଥ୍ୟା)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ଏହି)->initBackend();
        }
        ଯଦି (backend)
            ଫେରନ୍ତୁ backend->create();
        ଫେରନ୍ତୁ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
ସୁରକ୍ଷିତ:
    ଶୂନ୍ୟ initBackend()
    {
        AutoLock lock(getInitializationMutex());
        ଚେଷ୍ଟା
        {
            ଯଦି (!initialized)
                loadPlugin();
        }
        ଧର (...)
        {
            CV_LOG_INFO(ଖାଲି, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = ସତ୍ୟ;
    }
    ଶୂନ୍ୟ loadPlugin();
};

ସ୍ଥିର
std::vector<FileSystemPath_t> getPluginCandidates(ଅଚଳ std::string& baseName)
{
    using ନାମାକାଶ cv::utils;
    using ନାମାକାଶ cv::utils::fs;
    ଅଚଳ std::string baseName_l = toLowerCase(baseName);
    ଅଚଳ std::string baseName_u = toUpperCase(baseName);
    ଅଚଳ FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ଅଚଳ std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ଯଦି (paths_.size() != 0)
    {
        ପାଇଁ (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    ଅନ୍ୟଥା
    {
        FileSystemPath_t ଦ୍ୱିମିକ_Location;
        ଯଦି (getBinLocation(ଦ୍ୱିମିକ_Location))
        {
            ଦ୍ୱିମିକ_Location = getParent(ଦ୍ୱିମିକ_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ଦ୍ୱିମିକ_Location);
#ଅନ୍ୟଥା
            paths.push_back(ଦ୍ୱିମିକ_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    ଅଚଳ std::string default_ପଦସମଷ୍ଟି = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    ଅଚଳ std::string plugin_ପଦସମଷ୍ଟି = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_ପଦସମଷ୍ଟି.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    ଯଦି (plugin_ପଦସମଷ୍ଟି != default_ପଦସମଷ୍ଟି)
    {
        moduleName = toFileSystemPath(plugin_ପଦସମଷ୍ଟି);
        results.push_back(moduleName);
    }
    ପାଇଁ (ଅଚଳ FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#ଅନ୍ୟଥା
    CV_LOG_DEBUG(ଖାଲି, "core(parallel): " << baseName << " plugin's glob is '" << plugin_ପଦସମଷ୍ଟି << "', " << paths.size() << " location(s)");
    ପାଇଁ (ଅଚଳ std::string& path : paths)
    {
        ଯଦି (path.empty())
            ଜାରି;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_ପଦସମଷ୍ଟି), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ଖାଲି, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(ଖାଲି, "Found " << results.size() << " plugin(s) for " << baseName);
    ଫେରନ୍ତୁ results;
}

ଶୂନ୍ୟ PluginParallelBackendFactory::loadPlugin()
{
    ପାଇଁ (ଅଚଳ FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ଯଦି (!lib->isLoaded())
        {
            ଜାରି;
        }
        ଚେଷ୍ଟା
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ଯଦି (!pluginBackend)
            {
                ଜାରି;
            }
            ଯଦି (pluginBackend->plugin_api_ == ଖାଲି)
            {
                CV_LOG_ERROR(ଖାଲି, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                ଜାରି;
            }
#ଯଦି !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            ଫେରନ୍ତୁ;
        }
        ଧର (...)
        {
            CV_LOG_WARNING(ଖାଲି, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

ନାମାକାଶ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ଅଚଳ std::string& baseName)
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ଫେରନ୍ତୁ std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#ଅନ୍ୟଥା
    CV_UNUSED(baseName);
    ଫେରନ୍ତୁ std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
