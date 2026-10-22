// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ impl {

ಬಳಸಿ ನಾಮಾಕಾಶ cv::parallel;

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

ಬಳಸಿ ನಾಮಾಕಾಶ cv::plugin::impl;  // plugin_loader.hpp

ವರ್ಗ PluginParallelBackend CV_FINAL: ಸಾರ್ವಜನಿಕ std::enable_shared_from_this<PluginParallelBackend>
{
ಸಂರಕ್ಷಿತ:
    ಶೂನ್ಯ initPluginAPI()
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_ಚಿಹ್ನೆ(init_name));
        ಆದರೆ (fn_init)
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "Found entry: '" << init_name << "'");
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ supported_api_ಆವೃತ್ತಿ = API_ಆವೃತ್ತಿ; supported_api_ಆವೃತ್ತಿ >= 0; supported_api_ಆವೃತ್ತಿ--)
            {
                plugin_api_ = fn_init(ABI_ಆವೃತ್ತಿ, supported_api_ಆವೃತ್ತಿ, ಖಾಲಿ);
                ಆದರೆ (plugin_api_)
                    ನಿಲ್ಲಿಸು;
            }
            ಆದರೆ (!plugin_api_)
            {
                CV_LOG_INFO(ಖಾಲಿ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                ಹಿಂತಿರುಗಿ;
            }
            ಆದರೆ (!checkCompatibility(plugin_api_->api_ಶೀರ್ಷಿಕೆ, ABI_ಆವೃತ್ತಿ, API_ಆವೃತ್ತಿ, ಸುಳ್ಳು))
            {
                plugin_api_ = ಖಾಲಿ;
                ಹಿಂತಿರುಗಿ;
            }
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): plugin is ready to use '" << plugin_api_->api_ಶೀರ್ಷಿಕೆ.api_description << "'");
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    ತಾರ್ಕಿಕ checkCompatibility(ಸ್ಥಿರಾಂಕ Open_CV_API_ಶೀರ್ಷಿಕೆ& api_ಶೀರ್ಷಿಕೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ abi_ಆವೃತ್ತಿ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ api_ಆವೃತ್ತಿ, ತಾರ್ಕಿಕ checkMinorOpenCVVersion)
    {
        ಆದರೆ (api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_major != CV_ಆವೃತ್ತಿ_MAJOR)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "core(parallel): wrong OpenCV major version used by plugin '" << api_ಶೀರ್ಷಿಕೆ.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_ಆವೃತ್ತಿ "'", api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_major, api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_minor))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಆದರೆ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_minor != CV_ಆವೃತ್ತಿ_MINOR)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_ಶೀರ್ಷಿಕೆ.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_ಆವೃತ್ತಿ "'", api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_major, api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_minor))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): initialized '" << api_ಶೀರ್ಷಿಕೆ.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_major, api_ಶೀರ್ಷಿಕೆ.opencv_ಆವೃತ್ತಿ_minor,
                 api_ಶೀರ್ಷಿಕೆ.min_api_ಆವೃತ್ತಿ, api_ಶೀರ್ಷಿಕೆ.api_ಆವೃತ್ತಿ)
            << ", current OpenCV version is '" CV_ಆವೃತ್ತಿ "' (ABI/API = " << abi_ಆವೃತ್ತಿ << "/" << api_ಆವೃತ್ತಿ << ")"
        );
        ಆದರೆ (api_ಶೀರ್ಷಿಕೆ.min_api_ಆವೃತ್ತಿ != abi_ಆವೃತ್ತಿ)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ಖಾಲಿ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_ಶೀರ್ಷಿಕೆ.min_api_ಆವೃತ್ತಿ);
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಆದರೆ (api_ಶೀರ್ಷಿಕೆ.api_ಆವೃತ್ತಿ != api_ಆವೃತ್ತಿ)
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_ಶೀರ್ಷಿಕೆ.api_ಆವೃತ್ತಿ, api_ಆವೃತ್ತಿ));
            ಆದರೆ (api_ಶೀರ್ಷಿಕೆ.api_ಆವೃತ್ತಿ < api_ಆವೃತ್ತಿ)
            {
                CV_LOG_INFO(ಖಾಲಿ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

ಸಾರ್ವಜನಿಕ:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ಸ್ಥಿರಾಂಕ OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ಸ್ಥಿರಾಂಕ std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ಖಾಲಿ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ಖಾಲಿ;

        ಆದರೆ (plugin_api_->v0.getInstance)
        {
            ಆದರೆ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                ಹಿಂತಿರುಗಿ std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        ಹಿಂತಿರುಗಿ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


ವರ್ಗ PluginParallelBackendFactory CV_FINAL: ಸಾರ್ವಜನಿಕ IParallelBackendFactory
{
ಸಾರ್ವಜನಿಕ:
    std::string ತಳ_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    ತಾರ್ಕಿಕ initialized;
ಸಾರ್ವಜನಿಕ:
    PluginParallelBackendFactory(ಸ್ಥಿರಾಂಕ std::string& ತಳ_Name)
        : ತಳ_Name_2(ತಳ_Name)
        , initialized(ಸುಳ್ಳು)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ಇದು)->initBackend();
        }
        ಆದರೆ (backend)
            ಹಿಂತಿರುಗಿ backend->create();
        ಹಿಂತಿರುಗಿ std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
ಸಂರಕ್ಷಿತ:
    ಶೂನ್ಯ initBackend()
    {
        Auto_ಬೀಗ lock(getInitializationMutex());
        ಪ್ರಯತ್ನಿಸು
        {
            ಆದರೆ (!initialized)
                ತುಂಬು_Plugin();
        }
        ಹಿಡಿ (...)
        {
            CV_LOG_INFO(ಖಾಲಿ, "core(parallel): exception during plugin loading: " << ತಳ_Name_2 << ". SKIP");
        }
        initialized = ನಿಜ;
    }
    ಶೂನ್ಯ ತುಂಬು_Plugin();
};

ಸ್ಥಿರ
std::vector<File_System_ಮಾರ್ಗ_t> getPluginCandidates(ಸ್ಥಿರಾಂಕ std::string& ತಳ_Name)
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::utils;
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::utils::fs;
    ಸ್ಥಿರಾಂಕ std::string ತಳ_Name_l = ಗಮ್ಯ_ಅಧೋ_Case(ತಳ_Name);
    ಸ್ಥಿರಾಂಕ std::string ತಳ_Name_u = ಗಮ್ಯ_ಊರ್ಧ್ವ_Case(ತಳ_Name);
    ಸ್ಥಿರಾಂಕ File_System_ಮಾರ್ಗ_t ತಳ_Name_l_fs = ಗಮ್ಯ_File_System_ಮಾರ್ಗ(ತಳ_Name_l);
    std::vector<File_System_ಮಾರ್ಗ_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ಸ್ಥಿರಾಂಕ std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ಆದರೆ (paths_.size() != 0)
    {
        ಗಾಗಿ (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(ಗಮ್ಯ_File_System_ಮಾರ್ಗ(paths_[i]));
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        File_System_ಮಾರ್ಗ_t ದ್ವಿಮಾನ_Location;
        ಆದರೆ (getBinLocation(ದ್ವಿಮಾನ_Location))
        {
            ದ್ವಿಮಾನ_Location = get_ಪೋಷಕ(ದ್ವಿಮಾನ_Location);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ದ್ವಿಮಾನ_Location);
#ಇಲ್ಲದಿದ್ದರೆ
            paths.push_back(ದ್ವಿಮಾನ_Location + ಗಮ್ಯ_File_System_ಮಾರ್ಗ("/") + ಗಮ್ಯ_File_System_ಮಾರ್ಗ(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#ಆದರೆ_ಅಂತ್ಯ
        }
    }
    ಸ್ಥಿರಾಂಕ std::string default_ಅಭಿವ್ಯಕ್ತಿ = library_ಪೂರ್ವಪ್ರತ್ಯಯ() + "opencv_core_parallel_" + ತಳ_Name_l + "*" + librarySuffix();
    ಸ್ಥಿರಾಂಕ std::string plugin_ಅಭಿವ್ಯಕ್ತಿ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + ತಳ_Name_u).c_str(), default_ಅಭಿವ್ಯಕ್ತಿ.c_str());
    std::vector<File_System_ಮಾರ್ಗ_t> results;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    File_System_ಮಾರ್ಗ_t moduleName = ಗಮ್ಯ_File_System_ಮಾರ್ಗ(library_ಪೂರ್ವಪ್ರತ್ಯಯ() + "opencv_core_parallel_" + ತಳ_Name_l + librarySuffix());
    ಆದರೆ (plugin_ಅಭಿವ್ಯಕ್ತಿ != default_ಅಭಿವ್ಯಕ್ತಿ)
    {
        moduleName = ಗಮ್ಯ_File_System_ಮಾರ್ಗ(plugin_ಅಭಿವ್ಯಕ್ತಿ);
        results.push_back(moduleName);
    }
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ File_System_ಮಾರ್ಗ_t& ಮಾರ್ಗ : paths)
    {
        results.push_back(ಮಾರ್ಗ + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#ಇಲ್ಲದಿದ್ದರೆ
    CV_LOG_DEBUG(ಖಾಲಿ, "core(parallel): " << ತಳ_Name << " plugin's glob is '" << plugin_ಅಭಿವ್ಯಕ್ತಿ << "', " << paths.size() << " location(s)");
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ std::string& ಮಾರ್ಗ : paths)
    {
        ಆದರೆ (ಮಾರ್ಗ.empty())
            ಮುಂದುವರಿ;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(ಮಾರ್ಗ, plugin_ಅಭಿವ್ಯಕ್ತಿ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ಖಾಲಿ, "    - " << ಮಾರ್ಗ << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#ಆದರೆ_ಅಂತ್ಯ
    CV_LOG_DEBUG(ಖಾಲಿ, "Found " << results.size() << " plugin(s) for " << ತಳ_Name);
    ಹಿಂತಿರುಗಿ results;
}

ಶೂನ್ಯ PluginParallelBackendFactory::ತುಂಬು_Plugin()
{
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ File_System_ಮಾರ್ಗ_t& plugin : getPluginCandidates(ತಳ_Name_2))
    {
        ಸ್ವಯಂಚಾಲಿತ lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ಆದರೆ (!lib->isLoaded())
        {
            ಮುಂದುವರಿ;
        }
        ಪ್ರಯತ್ನಿಸು
        {
            ಸ್ವಯಂಚಾಲಿತ pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ಆದರೆ (!pluginBackend)
            {
                ಮುಂದುವರಿ;
            }
            ಆದರೆ (pluginBackend->plugin_api_ == ಖಾಲಿ)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "core(parallel): no compatible plugin API for backend: " << ತಳ_Name_2 << " in " << ಗಮ್ಯ_Printable_ಮಾರ್ಗ(plugin));
                ಮುಂದುವರಿ;
            }
#ಆದರೆ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#ಆದರೆ_ಅಂತ್ಯ
            backend = pluginBackend;
            ಹಿಂತಿರುಗಿ;
        }
        ಹಿಡಿ (...)
        {
            CV_LOG_WARNING(ಖಾಲಿ, "core(parallel): exception during plugin initialization: " << ಗಮ್ಯ_Printable_ಮಾರ್ಗ(plugin) << ". SKIP");
        }
    }
}

#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

ನಾಮಾಕಾಶ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ಸ್ಥಿರಾಂಕ std::string& ತಳ_Name)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    ಹಿಂತಿರುಗಿ std::make_shared<impl::PluginParallelBackendFactory>(ತಳ_Name);
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ತಳ_Name);
    ಹಿಂತಿರುಗಿ std::shared_ptr<IParallelBackendFactory>();
#ಆದರೆ_ಅಂತ್ಯ
}

}}  // namespace
