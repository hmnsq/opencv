// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#include "opencv2/core/utils/plugin_loader.private.hpp"

namespace cv { namespace impl {

using namespace cv::parallel;

#ប្រសិនបើ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

using namespace cv::plugin::impl;  // plugin_loader.hpp

class PluginParallelBackend CV_FINAL: public std::enable_shared_from_this<PluginParallelBackend>
{
rak_sita:
    void initPluginAPI()
    {
        const var_na* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->getSymbol(init_name));
        ប្រសិនបើ (fn_init)
        {
            CV_LOG_DEBUG(NULL, "Found entry: '" << init_name << "'");
            សម្រាប់ (ចំនួនគត់ supported_api_version = API_VERSION; supported_api_version >= 0; supported_api_version--)
            {
                plugin_api_ = fn_init(ABI_VERSION, supported_api_version, NULL);
                ប្រសិនបើ (plugin_api_)
                    bha_nga;
            }
            ប្រសិនបើ (!plugin_api_)
            {
                CV_LOG_INFO(NULL, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ត្រឡប់;
            }
            ប្រសិនបើ (!checkCompatibility(plugin_api_->api_header, ABI_VERSION, API_VERSION, false))
            {
                plugin_api_ = NULL;
                ត្រឡប់;
            }
            CV_LOG_INFO(NULL, "core(parallel): plugin is ready to use '" << plugin_api_->api_header.api_description << "'");
        }
        បើមិនដូច្នេះ
        {
            CV_LOG_INFO(NULL, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    bool checkCompatibility(const OpenCV_API_Header& api_header, unsigned ចំនួនគត់ abi_version, unsigned ចំនួនគត់ api_version, bool checkMinorOpenCVVersion)
    {
        ប្រសិនបើ (api_header.opencv_version_major != CV_VERSION_MAJOR)
        {
            CV_LOG_ERROR(NULL, "core(parallel): wrong OpenCV major version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ត្រឡប់ false;
        }
        ប្រសិនបើ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        បើមិនដូច្នេះ ប្រសិនបើ (api_header.opencv_version_minor != CV_VERSION_MINOR)
        {
            CV_LOG_ERROR(NULL, "core(parallel): wrong OpenCV minor version used by plugin '" << api_header.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_VERSION "'", api_header.opencv_version_major, api_header.opencv_version_minor))
            ត្រឡប់ false;
        }
        CV_LOG_DEBUG(NULL, "core(parallel): initialized '" << api_header.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_header.opencv_version_major, api_header.opencv_version_minor,
                 api_header.min_api_version, api_header.api_version)
            << ", current OpenCV version is '" CV_VERSION "' (ABI/API = " << abi_version << "/" << api_version << ")"
        );
        ប្រសិនបើ (api_header.min_api_version != abi_version)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(NULL, "core(parallel): plugin is not supported due to incompatible ABI = " << api_header.min_api_version);
            ត្រឡប់ false;
        }
        ប្រសិនបើ (api_header.api_version != api_version)
        {
            CV_LOG_INFO(NULL, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_header.api_version, api_version));
            ប្រសិនបើ (api_header.api_version < api_version)
            {
                CV_LOG_INFO(NULL, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ត្រឡប់ true;
    }

public:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    const OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(const std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(NULL)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() const
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = NULL;

        ប្រសិនបើ (plugin_api_->v0.getInstance)
        {
            ប្រសិនបើ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ត្រឡប់ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ត្រឡប់ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


class PluginParallelBackendFactory CV_FINAL: public IParallelBackendFactory
{
public:
    std::string baseName_;
    std::shared_ptr<PluginParallelBackend> backend;
    bool initialized;
public:
    PluginParallelBackendFactory(const std::string& baseName)
        : baseName_(baseName)
        , initialized(false)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() const CV_OVERRIDE
    {
        ប្រសិនបើ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(this)->initBackend();
        }
        ប្រសិនបើ (backend)
            ត្រឡប់ backend->create();
        ត្រឡប់ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
rak_sita:
    void initBackend()
    {
        AutoLock lock(getInitializationMutex());
        try
        {
            ប្រសិនបើ (!initialized)
                loadPlugin();
        }
        gr_haa_na (...)
        {
            CV_LOG_INFO(NULL, "core(parallel): exception during plugin loading: " << baseName_ << ". SKIP");
        }
        initialized = true;
    }
    void loadPlugin();
};

static
std::vector<FileSystemPath_t> getPluginCandidates(const std::string& baseName)
{
    using namespace cv::utils;
    using namespace cv::utils::fs;
    const std::string baseName_l = toLowerCase(baseName);
    const std::string baseName_u = toUpperCase(baseName);
    const FileSystemPath_t baseName_l_fs = toFileSystemPath(baseName_l);
    std::vector<FileSystemPath_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    const std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ប្រសិនបើ (paths_.size() != 0)
    {
        សម្រាប់ (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(toFileSystemPath(paths_[i]));
        }
    }
    បើមិនដូច្នេះ
    {
        FileSystemPath_t binaryLocation;
        ប្រសិនបើ (getBinLocation(binaryLocation))
        {
            binaryLocation = getParent(binaryLocation);
#ifndef CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(binaryLocation);
#បើមិនដូច្នេះ
            paths.push_back(binaryLocation + toFileSystemPath("/") + toFileSystemPath(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#endif
        }
    }
    const std::string default_expr = libraryPrefix() + "opencv_core_parallel_" + baseName_l + "*" + librarySuffix();
    const std::string plugin_expr = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + baseName_u).c_str(), default_expr.c_str());
    std::vector<FileSystemPath_t> results;
#ifdef _WIN32
    FileSystemPath_t moduleName = toFileSystemPath(libraryPrefix() + "opencv_core_parallel_" + baseName_l + librarySuffix());
    ប្រសិនបើ (plugin_expr != default_expr)
    {
        moduleName = toFileSystemPath(plugin_expr);
        results.push_back(moduleName);
    }
    សម្រាប់ (const FileSystemPath_t& path : paths)
    {
        results.push_back(path + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#បើមិនដូច្នេះ
    CV_LOG_DEBUG(NULL, "core(parallel): " << baseName << " plugin's glob is '" << plugin_expr << "', " << paths.size() << " location(s)");
    សម្រាប់ (const std::string& path : paths)
    {
        ប្រសិនបើ (path.empty())
            continue;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(path, plugin_expr), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(NULL, "    - " << path << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#endif
    CV_LOG_DEBUG(NULL, "Found " << results.size() << " plugin(s) for " << baseName);
    ត្រឡប់ results;
}

void PluginParallelBackendFactory::loadPlugin()
{
    សម្រាប់ (const FileSystemPath_t& plugin : getPluginCandidates(baseName_))
    {
        auto lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ប្រសិនបើ (!lib->isLoaded())
        {
            continue;
        }
        try
        {
            auto pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ប្រសិនបើ (!pluginBackend)
            {
                continue;
            }
            ប្រសិនបើ (pluginBackend->plugin_api_ == NULL)
            {
                CV_LOG_ERROR(NULL, "core(parallel): no compatible plugin API for backend: " << baseName_ << " in " << toPrintablePath(plugin));
                continue;
            }
#ប្រសិនបើ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#endif
            backend = pluginBackend;
            ត្រឡប់;
        }
        gr_haa_na (...)
        {
            CV_LOG_WARNING(NULL, "core(parallel): exception during plugin initialization: " << toPrintablePath(plugin) << ". SKIP");
        }
    }
}

#endif  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

namespace parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(const std::string& baseName)
{
#ប្រសិនបើ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ត្រឡប់ std::make_shared<impl::PluginParallelBackendFactory>(baseName);
#បើមិនដូច្នេះ
    CV_UNUSED(baseName);
    ត្រឡប់ std::shared_ptr<IParallelBackendFactory>();
#endif
}

}}  // namespace
