// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

نطاق cv { نطاق impl {

استعمال نطاق cv::parallel;

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

استعمال نطاق cv::plugin::impl;  // plugin_loader.hpp

صنف PluginParallelBackend CV_FINAL: عام std::enable_shared_from_this<PluginParallelBackend>
{
محمي:
    فراغ initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_نشان(init_name));
        إذا (fn_init)
        {
            CV_LOG_DEBUG(عدم, "Found entry: '" << init_name << "'");
            لكل (صحيح supported_api_نسخو = API_نسخو; supported_api_نسخو >= 0; supported_api_نسخو--)
            {
                plugin_api_ = fn_init(ABI_نسخو, supported_api_نسخو, عدم);
                إذا (plugin_api_)
                    اكسر;
            }
            إذا (!plugin_api_)
            {
                CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                أرجع;
            }
            إذا (!checkCompatibility(plugin_api_->api_سرو, ABI_نسخو, API_نسخو, خطأ))
            {
                plugin_api_ = عدم;
                أرجع;
            }
            CV_LOG_INFO(عدم, "core(parallel): plugin is ready to use '" << plugin_api_->api_سرو.api_description << "'");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقي checkCompatibility(ثابت Open_CV_API_سرو& api_سرو, بنانشان صحيح abi_نسخو, بنانشان صحيح api_نسخو, منطقي checkMinorOpenCVVersion)
    {
        إذا (api_سرو.opencv_نسخو_major != CV_نسخو_MAJOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV major version used by plugin '" << api_سرو.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخو "'", api_سرو.opencv_نسخو_major, api_سرو.opencv_نسخو_minor))
            أرجع خطأ;
        }
        إذا (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وإلا إذا (api_سرو.opencv_نسخو_minor != CV_نسخو_MINOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV minor version used by plugin '" << api_سرو.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخو "'", api_سرو.opencv_نسخو_major, api_سرو.opencv_نسخو_minor))
            أرجع خطأ;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): initialized '" << api_سرو.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_سرو.opencv_نسخو_major, api_سرو.opencv_نسخو_minor,
                 api_سرو.min_api_نسخو, api_سرو.api_نسخو)
            << ", current OpenCV version is '" CV_نسخو "' (ABI/API = " << abi_نسخو << "/" << api_نسخو << ")"
        );
        إذا (api_سرو.min_api_نسخو != abi_نسخو)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(عدم, "core(parallel): plugin is not supported due to incompatible ABI = " << api_سرو.min_api_نسخو);
            أرجع خطأ;
        }
        إذا (api_سرو.api_نسخو != api_نسخو)
        {
            CV_LOG_INFO(عدم, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_سرو.api_نسخو, api_نسخو));
            إذا (api_سرو.api_نسخو < api_نسخو)
            {
                CV_LOG_INFO(عدم, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        أرجع سچ;
    }

عام:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    ثابت OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(ثابت std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(عدم)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = عدم;

        إذا (plugin_api_->v0.getInstance)
        {
            إذا (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                أرجع std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        أرجع std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


صنف PluginParallelBackendFactory CV_FINAL: عام IParallelBackendFactory
{
عام:
    std::string بنياد_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقي initialized;
عام:
    PluginParallelBackendFactory(ثابت std::string& بنياد_Name)
        : بنياد_Name_2(بنياد_Name)
        , initialized(خطأ)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        إذا (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(هذا)->initBackend();
        }
        إذا (backend)
            أرجع backend->create();
        أرجع std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
محمي:
    فراغ initBackend()
    {
        Auto_تالو lock(getInitializationMutex());
        حاول
        {
            إذا (!initialized)
                ڀريو_Plugin();
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "core(parallel): exception during plugin loading: " << بنياد_Name_2 << ". SKIP");
        }
        initialized = سچ;
    }
    فراغ ڀريو_Plugin();
};

ساكن
std::vector<File_System_رستو_t> getPluginCandidates(ثابت std::string& بنياد_Name)
{
    استعمال نطاق cv::utils;
    استعمال نطاق cv::utils::fs;
    ثابت std::string بنياد_Name_l = منزل_هيٺاهون_Case(بنياد_Name);
    ثابت std::string بنياد_Name_u = منزل_مٿانهون_Case(بنياد_Name);
    ثابت File_System_رستو_t بنياد_Name_l_fs = منزل_File_System_رستو(بنياد_Name_l);
    std::vector<File_System_رستو_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    إذا (paths_.size() != 0)
    {
        لكل (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(منزل_File_System_رستو(paths_[i]));
        }
    }
    وإلا
    {
        File_System_رستو_t ٻٽو_Location;
        إذا (getBinLocation(ٻٽو_Location))
        {
            ٻٽو_Location = get_والد(ٻٽو_Location);
#جيڪڏهن_وصف_نه_ٿيل CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ٻٽو_Location);
#وإلا
            paths.push_back(ٻٽو_Location + منزل_File_System_رستو("/") + منزل_File_System_رستو(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#جيڪڏهن_پورو
        }
    }
    ثابت std::string default_اظهار = library_اڳياڙي() + "opencv_core_parallel_" + بنياد_Name_l + "*" + librarySuffix();
    ثابت std::string plugin_اظهار = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + بنياد_Name_u).c_str(), default_اظهار.c_str());
    std::vector<File_System_رستو_t> results;
#جيڪڏهن_وصف_ٿيل _WIN32
    File_System_رستو_t moduleName = منزل_File_System_رستو(library_اڳياڙي() + "opencv_core_parallel_" + بنياد_Name_l + librarySuffix());
    إذا (plugin_اظهار != default_اظهار)
    {
        moduleName = منزل_File_System_رستو(plugin_اظهار);
        results.push_back(moduleName);
    }
    لكل (ثابت File_System_رستو_t& رستو : paths)
    {
        results.push_back(رستو + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وإلا
    CV_LOG_DEBUG(عدم, "core(parallel): " << بنياد_Name << " plugin's glob is '" << plugin_اظهار << "', " << paths.size() << " location(s)");
    لكل (ثابت std::string& رستو : paths)
    {
        إذا (رستو.empty())
            استمر;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(رستو, plugin_اظهار), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(عدم, "    - " << رستو << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#جيڪڏهن_پورو
    CV_LOG_DEBUG(عدم, "Found " << results.size() << " plugin(s) for " << بنياد_Name);
    أرجع results;
}

فراغ PluginParallelBackendFactory::ڀريو_Plugin()
{
    لكل (ثابت File_System_رستو_t& plugin : getPluginCandidates(بنياد_Name_2))
    {
        خودڪار lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        إذا (!lib->isLoaded())
        {
            استمر;
        }
        حاول
        {
            خودڪار pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            إذا (!pluginBackend)
            {
                استمر;
            }
            إذا (pluginBackend->plugin_api_ == عدم)
            {
                CV_LOG_ERROR(عدم, "core(parallel): no compatible plugin API for backend: " << بنياد_Name_2 << " in " << منزل_Printable_رستو(plugin));
                استمر;
            }
#إذا !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#جيڪڏهن_پورو
            backend = pluginBackend;
            أرجع;
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): exception during plugin initialization: " << منزل_Printable_رستو(plugin) << ". SKIP");
        }
    }
}

#جيڪڏهن_پورو  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نطاق parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& بنياد_Name)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    أرجع std::make_shared<impl::PluginParallelBackendFactory>(بنياد_Name);
#وإلا
    CV_اڻ_استعمال_ٿيل(بنياد_Name);
    أرجع std::shared_ptr<IParallelBackendFactory>();
#جيڪڏهن_پورو
}

}}  // namespace
