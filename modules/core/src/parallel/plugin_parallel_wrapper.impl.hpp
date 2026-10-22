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

उपयुज्य नामाकाश cv::parallel;

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

उपयुज्य नामाकाश cv::plugin::impl;  // plugin_loader.hpp

वर्ग PluginParallelBackend CV_FINAL: सार्वजनिक std::enable_shared_from_this<PluginParallelBackend>
{
रक्षित:
    शून्य initPluginAPI()
    {
        ध्रुव वर्ण* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_चिह्नम्(init_name));
        यदि (fn_init)
        {
            CV_LOG_DEBUG(रिक्त, "Found entry: '" << init_name << "'");
            प्रति (पूर्णाङ्क supported_api_संस्करणम् = API_संस्करणम्; supported_api_संस्करणम् >= 0; supported_api_संस्करणम्--)
            {
                plugin_api_ = fn_init(ABI_संस्करणम्, supported_api_संस्करणम्, रिक्त);
                यदि (plugin_api_)
                    भङ्ग;
            }
            यदि (!plugin_api_)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                प्रत्यागम;
            }
            यदि (!checkCompatibility(plugin_api_->api_शीर्षकम्, ABI_संस्करणम्, API_संस्करणम्, असत्य))
            {
                plugin_api_ = रिक्त;
                प्रत्यागम;
            }
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is ready to use '" << plugin_api_->api_शीर्षकम्.api_description << "'");
        }
        अन्यथा
        {
            CV_LOG_INFO(रिक्त, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    तार्किक checkCompatibility(ध्रुव Open_CV_API_शीर्षकम्& api_शीर्षकम्, अचिह्नित पूर्णाङ्क abi_संस्करणम्, अचिह्नित पूर्णाङ्क api_संस्करणम्, तार्किक checkMinorOpenCVVersion)
    {
        यदि (api_शीर्षकम्.opencv_संस्करणम्_major != CV_संस्करणम्_MAJOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV major version used by plugin '" << api_शीर्षकम्.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_संस्करणम् "'", api_शीर्षकम्.opencv_संस्करणम्_major, api_शीर्षकम्.opencv_संस्करणम्_minor))
            प्रत्यागम असत्य;
        }
        यदि (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        अन्यथा यदि (api_शीर्षकम्.opencv_संस्करणम्_minor != CV_संस्करणम्_MINOR)
        {
            CV_LOG_ERROR(रिक्त, "core(parallel): wrong OpenCV minor version used by plugin '" << api_शीर्षकम्.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_संस्करणम् "'", api_शीर्षकम्.opencv_संस्करणम्_major, api_शीर्षकम्.opencv_संस्करणम्_minor))
            प्रत्यागम असत्य;
        }
        CV_LOG_DEBUG(रिक्त, "core(parallel): initialized '" << api_शीर्षकम्.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_शीर्षकम्.opencv_संस्करणम्_major, api_शीर्षकम्.opencv_संस्करणम्_minor,
                 api_शीर्षकम्.min_api_संस्करणम्, api_शीर्षकम्.api_संस्करणम्)
            << ", current OpenCV version is '" CV_संस्करणम् "' (ABI/API = " << abi_संस्करणम् << "/" << api_संस्करणम् << ")"
        );
        यदि (api_शीर्षकम्.min_api_संस्करणम् != abi_संस्करणम्)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(रिक्त, "core(parallel): plugin is not supported due to incompatible ABI = " << api_शीर्षकम्.min_api_संस्करणम्);
            प्रत्यागम असत्य;
        }
        यदि (api_शीर्षकम्.api_संस्करणम् != api_संस्करणम्)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_शीर्षकम्.api_संस्करणम्, api_संस्करणम्));
            यदि (api_शीर्षकम्.api_संस्करणम् < api_संस्करणम्)
            {
                CV_LOG_INFO(रिक्त, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        प्रत्यागम सत्य;
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

        यदि (plugin_api_->v0.getInstance)
        {
            यदि (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                प्रत्यागम std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        प्रत्यागम std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


वर्ग PluginParallelBackendFactory CV_FINAL: सार्वजनिक IParallelBackendFactory
{
सार्वजनिक:
    std::string आधारः_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    तार्किक initialized;
सार्वजनिक:
    PluginParallelBackendFactory(ध्रुव std::string& आधारः_Name)
        : आधारः_Name_2(आधारः_Name)
        , initialized(असत्य)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ध्रुव CV_OVERRIDE
    {
        यदि (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(एतत्)->initBackend();
        }
        यदि (backend)
            प्रत्यागम backend->create();
        प्रत्यागम std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
रक्षित:
    शून्य initBackend()
    {
        Auto_तालकम् lock(getInitializationMutex());
        यत्न
        {
            यदि (!initialized)
                आरोपय_Plugin();
        }
        गृहाण (...)
        {
            CV_LOG_INFO(रिक्त, "core(parallel): exception during plugin loading: " << आधारः_Name_2 << ". SKIP");
        }
        initialized = सत्य;
    }
    शून्य आरोपय_Plugin();
};

स्थिर
std::vector<File_System_मार्गः_t> getPluginCandidates(ध्रुव std::string& आधारः_Name)
{
    उपयुज्य नामाकाश cv::utils;
    उपयुज्य नामाकाश cv::utils::fs;
    ध्रुव std::string आधारः_Name_l = गन्तव्यम्_अधस्तनम्_Case(आधारः_Name);
    ध्रुव std::string आधारः_Name_u = गन्तव्यम्_ऊर्ध्वस्थम्_Case(आधारः_Name);
    ध्रुव File_System_मार्गः_t आधारः_Name_l_fs = गन्तव्यम्_File_System_मार्गः(आधारः_Name_l);
    std::vector<File_System_मार्गः_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ध्रुव std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    यदि (paths_.size() != 0)
    {
        प्रति (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(गन्तव्यम्_File_System_मार्गः(paths_[i]));
        }
    }
    अन्यथा
    {
        File_System_मार्गः_t द्विमान_Location;
        यदि (getBinLocation(द्विमान_Location))
        {
            द्विमान_Location = getParent(द्विमान_Location);
#यदि_अपरिभाषितम् CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(द्विमान_Location);
#अन्यथा
            paths.push_back(द्विमान_Location + गन्तव्यम्_File_System_मार्गः("/") + गन्तव्यम्_File_System_मार्गः(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#यदि_अन्तः
        }
    }
    ध्रुव std::string default_व्यञ्जकम् = library_उपसर्गः() + "opencv_core_parallel_" + आधारः_Name_l + "*" + librarySuffix();
    ध्रुव std::string plugin_व्यञ्जकम् = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + आधारः_Name_u).c_str(), default_व्यञ्जकम्.c_str());
    std::vector<File_System_मार्गः_t> results;
#यदि_परिभाषितम् _WIN32
    File_System_मार्गः_t moduleName = गन्तव्यम्_File_System_मार्गः(library_उपसर्गः() + "opencv_core_parallel_" + आधारः_Name_l + librarySuffix());
    यदि (plugin_व्यञ्जकम् != default_व्यञ्जकम्)
    {
        moduleName = गन्तव्यम्_File_System_मार्गः(plugin_व्यञ्जकम्);
        results.push_back(moduleName);
    }
    प्रति (ध्रुव File_System_मार्गः_t& मार्गः : paths)
    {
        results.push_back(मार्गः + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#अन्यथा
    CV_LOG_DEBUG(रिक्त, "core(parallel): " << आधारः_Name << " plugin's glob is '" << plugin_व्यञ्जकम् << "', " << paths.size() << " location(s)");
    प्रति (ध्रुव std::string& मार्गः : paths)
    {
        यदि (मार्गः.empty())
            अनुवर्त;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(मार्गः, plugin_व्यञ्जकम्), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(रिक्त, "    - " << मार्गः << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#यदि_अन्तः
    CV_LOG_DEBUG(रिक्त, "Found " << results.size() << " plugin(s) for " << आधारः_Name);
    प्रत्यागम results;
}

शून्य PluginParallelBackendFactory::आरोपय_Plugin()
{
    प्रति (ध्रुव File_System_मार्गः_t& plugin : getPluginCandidates(आधारः_Name_2))
    {
        स्वचालित lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        यदि (!lib->isLoaded())
        {
            अनुवर्त;
        }
        यत्न
        {
            स्वचालित pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            यदि (!pluginBackend)
            {
                अनुवर्त;
            }
            यदि (pluginBackend->plugin_api_ == रिक्त)
            {
                CV_LOG_ERROR(रिक्त, "core(parallel): no compatible plugin API for backend: " << आधारः_Name_2 << " in " << गन्तव्यम्_Printable_मार्गः(plugin));
                अनुवर्त;
            }
#यदि !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#यदि_अन्तः
            backend = pluginBackend;
            प्रत्यागम;
        }
        गृहाण (...)
        {
            CV_LOG_WARNING(रिक्त, "core(parallel): exception during plugin initialization: " << गन्तव्यम्_Printable_मार्गः(plugin) << ". SKIP");
        }
    }
}

#यदि_अन्तः  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

नामाकाश parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ध्रुव std::string& आधारः_Name)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    प्रत्यागम std::make_shared<impl::PluginParallelBackendFactory>(आधारः_Name);
#अन्यथा
    CV_अप्रयुक्तम्(आधारः_Name);
    प्रत्यागम std::shared_ptr<IParallelBackendFactory>();
#यदि_अन्तः
}

}}  // namespace
