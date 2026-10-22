// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#അന്തര്ഭാവ "opencv2/core/utils/plugin_loader.private.hpp"

നാമാകാശ cv { നാമാകാശ impl {

using നാമാകാശ cv::parallel;

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using നാമാകാശ cv::plugin::impl;  // plugin_loader.hpp

വർഗ്ഗം PluginParallelBackend CV_FINAL: പൊതു std::enable_shared_from_this<PluginParallelBackend>
{
സംരക്ഷിത:
    ശൂന്യം initPluginAPI()
    {
        സ്ഥിരം അക്ഷരം* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_ചിഹ്നം(init_name));
        എങ്കിൽ (fn_init)
        {
            CV_LOG_DEBUG(ശൂന്യ, "Found entry: '" << init_name << "'");
            വേണ്ടി (പൂർണ്ണസംഖ്യ supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, ശൂന്യ);
                എങ്കിൽ (plugin_api_)
                    നിർത്തുക;
            }
            എങ്കിൽ (!plugin_api_)
            {
                CV_LOG_INFO(ശൂന്യ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                തിരികെ;
            }
            എങ്കിൽ (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, അസത്യം))
            {
                plugin_api_ = ശൂന്യ;
                തിരികെ;
            }
            CV_LOG_INFO(ശൂന്യ, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    തർക്കം checkCompatibility(സ്ഥിരം OpenCV_API_Header& api_header, unsigned പൂർണ്ണസംഖ്യ abi_version, unsigned പൂർണ്ണസംഖ്യ api_version, തർക്കം checkMinorOpenCVVersion)
    {
        എങ്കിൽ (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(ശൂന്യ, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            തിരികെ അസത്യം;
        }
        എങ്കിൽ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        അല്ലെങ്കിൽ എങ്കിൽ (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(ശൂന്യ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            തിരികെ അസത്യം;
        }
        CV_LOG_DEBUG(ശൂന്യ, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        എങ്കിൽ (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ശൂന്യ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            തിരികെ അസത്യം;
        }
        എങ്കിൽ (api_header.api_version != api_version)
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            എങ്കിൽ (api_header.api_version < api_version)
            {
                CV_LOG_INFO(ശൂന്യ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        തിരികെ സത്യം;
    }

പൊതു:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    സ്ഥിരം OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(സ്ഥിരം std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ശൂന്യ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() സ്ഥിരം
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ശൂന്യ;

        എങ്കിൽ (plugin_api_->v0.getInstance)
        {
            എങ്കിൽ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                തിരികെ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        തിരികെ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


വർഗ്ഗം PluginParallelBackendFactory CV_FINAL: പൊതു IParallelBackendFactory
{
പൊതു:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    തർക്കം initialized;
പൊതു:
    PluginParallelBackendFactory(സ്ഥിരം std::string& baseName)
        : baseName_(baseName)
        , initialized(അസത്യം)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ഇത്)->initBackend();
        }
        എങ്കിൽ (backend)
            തിരികെ backend->create();
        തിരികെ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
സംരക്ഷിത:
    ശൂന്യം initBackend()
    {
        AutoLock lock(getInitializationMutex());
        ശ്രമിക്കുക
        {
            എങ്കിൽ (!initialized)
                loadPlugin();
        }
        പിടിക്കുക (...)
        {
            CV_LOG_INFO(ശൂന്യ, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = സത്യം;
    }
    ശൂന്യം loadPlugin();
};

സ്ഥിതം
std::vector<FileSystemPath_t> getPluginCandidates(സ്ഥിരം std::string& baseName)
{
    using നാമാകാശ cv::utils;
    using നാമാകാശ cv::utils::fs;
    സ്ഥിരം std::string baseName_l = toLowerCase(baseName);
    സ്ഥിരം std::string baseName_u = toUpperCase(baseName);
    സ്ഥിരം FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    സ്ഥിരം std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    എങ്കിൽ (paths_.size() != 0)
    {
        വേണ്ടി (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    അല്ലെങ്കിൽ
    {
        FileSystemPath_t ദ്വിമാന_Location;
        എങ്കിൽ (getBinLocation(ദ്വിമാന_Location))
        {
            ദ്വിമാന_Location = getParent(ദ്വിമാന_Location);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ദ്വിമാന_Location);
#അല്ലെങ്കിൽ
            paths.push_back(ദ്വിമാന_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    സ്ഥിരം std::string default_വ്യഞ്ജകം = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    സ്ഥിരം std::string plugin_വ്യഞ്ജകം = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_വ്യഞ്ജകം.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    എങ്കിൽ (plugin_വ്യഞ്ജകം != default_വ്യഞ്ജകം)
    {
        moduleName = toFileSystemPath(plugin_വ്യഞ്ജകം);
        results.push_back(moduleName);
    }
    വേണ്ടി (സ്ഥിരം FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#അല്ലെങ്കിൽ
    CV_LOG_DEBUG(ശൂന്യ, "core(parallel): " << baseName << " plugin's glob is '" << plugin_വ്യഞ്ജകം << "', " << paths.size() << " location(s)");
    വേണ്ടി (സ്ഥിരം std::string& path : paths)
    {
        എങ്കിൽ (path.empty())
            തുടരുക;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_വ്യഞ്ജകം), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ശൂന്യ, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(ശൂന്യ, "Found " << results.size() << " plugin(s) for " << baseName);
    തിരികെ results;
}

ശൂന്യം PluginParallelBackendFactory::loadPlugin()
{
    വേണ്ടി (സ്ഥിരം FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        എങ്കിൽ (!lib->isLoaded())
        {
            തുടരുക;
        }
        ശ്രമിക്കുക
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            എങ്കിൽ (!pluginBackend)
            {
                തുടരുക;
            }
            എങ്കിൽ (pluginBackend->plugin_api_ == ശൂന്യ)
            {
                CV_LOG_ERROR(ശൂന്യ, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                തുടരുക;
            }
#എങ്കിൽ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            തിരികെ;
        }
        പിടിക്കുക (...)
        {
            CV_LOG_WARNING(ശൂന്യ, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

നാമാകാശ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(സ്ഥിരം std::string& baseName)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    തിരികെ std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#അല്ലെങ്കിൽ
    CV_UNUSED(baseName);
    തിരികെ std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
