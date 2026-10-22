// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#અન્તર્ભાવ "opencv2/core/utils/plugin_loader.private.hpp"

નામાકાશ cv { નામાકાશ impl {

using નામાકાશ cv::parallel;

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using નામાકાશ cv::plugin::impl;  // plugin_loader.hpp

વર્ગ PluginParallelBackend CV_FINAL: જાહેર std::enable_shared_from_this<PluginParallelBackend>
{
સુરક્ષિત:
    શૂન્ય initPluginAPI()
    {
        અચલ અક્ષર* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_પ્રતીક(init_name));
        જો (fn_init)
        {
            CV_LOG_DEBUG(ખાલી, "Found entry: '" << init_name << "'");
            માટે (પૂર્ણાંક supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, ખાલી);
                જો (plugin_api_)
                    તોડો;
            }
            જો (!plugin_api_)
            {
                CV_LOG_INFO(ખાલી, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                પાછા;
            }
            જો (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, ખોટું))
            {
                plugin_api_ = ખાલી;
                પાછા;
            }
            CV_LOG_INFO(ખાલી, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        નહીંતર
        {
            CV_LOG_INFO(ખાલી, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    તાર્કિક checkCompatibility(અચલ OpenCV_API_Header& api_header, unsigned પૂર્ણાંક abi_version, unsigned પૂર્ણાંક api_version, તાર્કિક checkMinorOpenCVVersion)
    {
        જો (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(ખાલી, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            પાછા ખોટું;
        }
        જો (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        નહીંતર જો (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(ખાલી, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            પાછા ખોટું;
        }
        CV_LOG_DEBUG(ખાલી, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        જો (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ખાલી, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            પાછા ખોટું;
        }
        જો (api_header.api_version != api_version)
        {
            CV_LOG_INFO(ખાલી, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            જો (api_header.api_version < api_version)
            {
                CV_LOG_INFO(ખાલી, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        પાછા સાચું;
    }

જાહેર:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    અચલ OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(અચલ std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ખાલી)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() અચલ
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ખાલી;

        જો (plugin_api_->v0.getInstance)
        {
            જો (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                પાછા std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        પાછા std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


વર્ગ PluginParallelBackendFactory CV_FINAL: જાહેર IParallelBackendFactory
{
જાહેર:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    તાર્કિક initialized;
જાહેર:
    PluginParallelBackendFactory(અચલ std::string& baseName)
        : baseName_(baseName)
        , initialized(ખોટું)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() અચલ CV_OVERRIDE
    {
        જો (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(આ)->initBackend();
        }
        જો (backend)
            પાછા backend->create();
        પાછા std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
સુરક્ષિત:
    શૂન્ય initBackend()
    {
        AutoLock lock(getInitializationMutex());
        પ્રયાસ
        {
            જો (!initialized)
                loadPlugin();
        }
        પકડો (...)
        {
            CV_LOG_INFO(ખાલી, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = સાચું;
    }
    શૂન્ય loadPlugin();
};

સ્થિર
std::vector<FileSystemPath_t> getPluginCandidates(અચલ std::string& baseName)
{
    using નામાકાશ cv::utils;
    using નામાકાશ cv::utils::fs;
    અચલ std::string baseName_l = toLowerCase(baseName);
    અચલ std::string baseName_u = toUpperCase(baseName);
    અચલ FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    અચલ std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    જો (paths_.size() != 0)
    {
        માટે (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    નહીંતર
    {
        FileSystemPath_t દ્વિઅંકી_Location;
        જો (getBinLocation(દ્વિઅંકી_Location))
        {
            દ્વિઅંકી_Location = getParent(દ્વિઅંકી_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(દ્વિઅંકી_Location);
#નહીંતર
            paths.push_back(દ્વિઅંકી_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    અચલ std::string default_પદાવલિ = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    અચલ std::string plugin_પદાવલિ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_પદાવલિ.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    જો (plugin_પદાવલિ != default_પદાવલિ)
    {
        moduleName = toFileSystemPath(plugin_પદાવલિ);
        results.push_back(moduleName);
    }
    માટે (અચલ FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#નહીંતર
    CV_LOG_DEBUG(ખાલી, "core(parallel): " << baseName << " plugin's glob is '" << plugin_પદાવલિ << "', " << paths.size() << " location(s)");
    માટે (અચલ std::string& path : paths)
    {
        જો (path.empty())
            ચાલુ;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_પદાવલિ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ખાલી, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(ખાલી, "Found " << results.size() << " plugin(s) for " << baseName);
    પાછા results;
}

શૂન્ય PluginParallelBackendFactory::loadPlugin()
{
    માટે (અચલ FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        જો (!lib->isLoaded())
        {
            ચાલુ;
        }
        પ્રયાસ
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            જો (!pluginBackend)
            {
                ચાલુ;
            }
            જો (pluginBackend->plugin_api_ == ખાલી)
            {
                CV_LOG_ERROR(ખાલી, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                ચાલુ;
            }
#જો !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            પાછા;
        }
        પકડો (...)
        {
            CV_LOG_WARNING(ખાલી, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

નામાકાશ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(અચલ std::string& baseName)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    પાછા std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#નહીંતર
    CV_UNUSED(baseName);
    પાછા std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
