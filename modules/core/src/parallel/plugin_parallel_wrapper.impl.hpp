// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#समावेश "opencv2/core/utils/plugin_loader.private.hpp"

namespace cv { namespace impl {

using namespace cv::parallel;

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using namespace cv::plugin::impl;  // plugin_loader.hpp

श्रेणी PluginParallelBackend CV_FINAL: खुला std::enable_shared_from_this<PluginParallelBackend>
{
रक्षित:
    व्योम initPluginAPI()
    {
        स्थिर अक्षर* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_प्रतीक(init_name));
        अगर (fn_init)
        {
            CV_LOG_DEBUG(शून्य, "Found entry: '" << init_name << "'");
            क्रम (पूर्णांक supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, शून्य);
                अगर (plugin_api_)
                    अवरोध;
            }
            अगर (!plugin_api_)
            {
                CV_LOG_INFO(शून्य, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                वापस;
            }
            अगर (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, false))
            {
                plugin_api_ = शून्य;
                वापस;
            }
            CV_LOG_INFO(शून्य, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        अन्यथा
        {
            CV_LOG_INFO(शून्य, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    bool checkCompatibility(स्थिर OpenCV_API_Header& api_header, अचिन्हित पूर्णांक abi_version, अचिन्हित पूर्णांक api_version, bool checkMinorOpenCVVersion)
    {
        अगर (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(शून्य, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            वापस false;
        }
        अगर (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        अन्यथा अगर (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(शून्य, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            वापस false;
        }
        CV_LOG_DEBUG(शून्य, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        अगर (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(शून्य, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            वापस false;
        }
        अगर (api_header.api_version != api_version)
        {
            CV_LOG_INFO(शून्य, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            अगर (api_header.api_version < api_version)
            {
                CV_LOG_INFO(शून्य, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        वापस true;
    }

खुला:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    स्थिर OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(स्थिर std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(शून्य)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिर
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = शून्य;

        अगर (plugin_api_->v0.getInstance)
        {
            अगर (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                वापस std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        वापस std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


श्रेणी PluginParallelBackendFactory CV_FINAL: खुला IParallelBackendFactory
{
खुला:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    bool initialized;
खुला:
    PluginParallelBackendFactory(स्थिर std::string& baseName)
        : baseName_(baseName)
        , initialized(false)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिर CV_OVERRIDE
    {
        अगर (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(यह)->initBackend();
        }
        अगर (backend)
            वापस backend->create();
        वापस std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
रक्षित:
    व्योम initBackend()
    {
        AutoLock lock(getInitializationMutex());
        प्रयास
        {
            अगर (!initialized)
                loadPlugin();
        }
        पकड़ो (...)
        {
            CV_LOG_INFO(शून्य, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = true;
    }
    व्योम loadPlugin();
};

अटल
std::vector<FileSystemPath_t> getPluginCandidates(स्थिर std::string& baseName)
{
    using namespace cv::utils;
    using namespace cv::utils::fs;
    स्थिर std::string baseName_l = toLowerCase(baseName);
    स्थिर std::string baseName_u = toUpperCase(baseName);
    स्थिर FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    स्थिर std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    अगर (paths_.size() != 0)
    {
        क्रम (माप_प्रकार i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    अन्यथा
    {
        FileSystemPath_t द्विआधारी_Location;
        अगर (getBinLocation(द्विआधारी_Location))
        {
            द्विआधारी_Location = getParent(द्विआधारी_Location);
#अगर_अघोषित CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(द्विआधारी_Location);
#अन्यथा
            paths.push_back(द्विआधारी_Location + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#पूर्ण_अगर
        }
    }
    स्थिर std::string default_व्यंजक = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    स्थिर std::string plugin_व्यंजक = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_व्यंजक.c_str());
    std::vector<FileSystemPath_t> results;
#अगर_घोषित _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    अगर (plugin_व्यंजक != default_व्यंजक)
    {
        moduleName = toFileSystemPath(plugin_व्यंजक);
        results.push_back(moduleName);
    }
    क्रम (स्थिर FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#अन्यथा
    CV_LOG_DEBUG(शून्य, "core(parallel): " << baseName << " plugin's glob is '" << plugin_व्यंजक << "', " << paths.size() << " location(s)");
    क्रम (स्थिर std::string& path : paths)
    {
        अगर (path.empty())
            जारी;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_व्यंजक), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(शून्य, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#पूर्ण_अगर
    CV_LOG_DEBUG(शून्य, "Found " << results.size() << " plugin(s) for " << baseName);
    वापस results;
}

व्योम PluginParallelBackendFactory::loadPlugin()
{
    क्रम (स्थिर FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        स्वतः lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        अगर (!lib->isLoaded())
        {
            जारी;
        }
        प्रयास
        {
            स्वतः pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            अगर (!pluginBackend)
            {
                जारी;
            }
            अगर (pluginBackend->plugin_api_ == शून्य)
            {
                CV_LOG_ERROR(शून्य, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                जारी;
            }
#अगर !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#पूर्ण_अगर
            backend = pluginBackend;
            वापस;
        }
        पकड़ो (...)
        {
            CV_LOG_WARNING(शून्य, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#पूर्ण_अगर  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

namespace parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(स्थिर std::string& baseName)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    वापस std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#अन्यथा
    CV_UNUSED(baseName);
    वापस std::shared_ptr<IParallelBackendFactory>();
#पूर्ण_अगर
}

}}  // namespace
