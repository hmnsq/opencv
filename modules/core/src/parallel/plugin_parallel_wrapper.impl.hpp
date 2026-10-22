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

ورتوں نطاق cv::parallel;

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

ورتوں نطاق cv::plugin::impl;  // plugin_loader.hpp

صنف PluginParallelBackend CV_FINAL: عام std::enable_shared_from_this<PluginParallelBackend>
{
محمي:
    فراغ initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_علامت(init_name));
        إذا (fn_init)
        {
            CV_LOG_DEBUG(عدم, "Found entry: '" << init_name << "'");
            لكل (صحيح supported_api_نسخہ = API_نسخہ; supported_api_نسخہ >= 0; supported_api_نسخہ--)
            {
                plugin_api_ = fn_init(ABI_نسخہ, supported_api_نسخہ, عدم);
                إذا (plugin_api_)
                    اكسر;
            }
            إذا (!plugin_api_)
            {
                CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                أرجع;
            }
            إذا (!checkCompatibility(plugin_api_->api_سرخی, ABI_نسخہ, API_نسخہ, خطأ))
            {
                plugin_api_ = عدم;
                أرجع;
            }
            CV_LOG_INFO(عدم, "core(parallel): plugin is ready to use '" << plugin_api_->api_سرخی.api_description << "'");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقي checkCompatibility(ثابت Open_CV_API_سرخی& api_سرخی, بنانشانی صحيح abi_نسخہ, بنانشانی صحيح api_نسخہ, منطقي checkMinorOpenCVVersion)
    {
        إذا (api_سرخی.opencv_نسخہ_major != CV_نسخہ_MAJOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV major version used by plugin '" << api_سرخی.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخہ "'", api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor))
            أرجع خطأ;
        }
        إذا (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وإلا إذا (api_سرخی.opencv_نسخہ_minor != CV_نسخہ_MINOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV minor version used by plugin '" << api_سرخی.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخہ "'", api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor))
            أرجع خطأ;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): initialized '" << api_سرخی.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_سرخی.opencv_نسخہ_major, api_سرخی.opencv_نسخہ_minor,
                 api_سرخی.min_api_نسخہ, api_سرخی.api_نسخہ)
            << ", current OpenCV version is '" CV_نسخہ "' (ABI/API = " << abi_نسخہ << "/" << api_نسخہ << ")"
        );
        إذا (api_سرخی.min_api_نسخہ != abi_نسخہ)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(عدم, "core(parallel): plugin is not supported due to incompatible ABI = " << api_سرخی.min_api_نسخہ);
            أرجع خطأ;
        }
        إذا (api_سرخی.api_نسخہ != api_نسخہ)
        {
            CV_LOG_INFO(عدم, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_سرخی.api_نسخہ, api_نسخہ));
            إذا (api_سرخی.api_نسخہ < api_نسخہ)
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
    std::string ادھار_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقي initialized;
عام:
    PluginParallelBackendFactory(ثابت std::string& ادھار_Name)
        : ادھار_Name_2(ادھار_Name)
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
        Auto_جندرا lock(getInitializationMutex());
        حاول
        {
            إذا (!initialized)
                لدو_Plugin();
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "core(parallel): exception during plugin loading: " << ادھار_Name_2 << ". SKIP");
        }
        initialized = سچ;
    }
    فراغ لدو_Plugin();
};

ساكن
std::vector<File_System_رستہ_t> getPluginCandidates(ثابت std::string& ادھار_Name)
{
    ورتوں نطاق cv::utils;
    ورتوں نطاق cv::utils::fs;
    ثابت std::string ادھار_Name_l = منزل_ہیٹھلا_Case(ادھار_Name);
    ثابت std::string ادھار_Name_u = منزل_اتلا_Case(ادھار_Name);
    ثابت File_System_رستہ_t ادھار_Name_l_fs = منزل_File_System_رستہ(ادھار_Name_l);
    std::vector<File_System_رستہ_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    إذا (paths_.size() != 0)
    {
        لكل (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(منزل_File_System_رستہ(paths_[i]));
        }
    }
    وإلا
    {
        File_System_رستہ_t ثنائی_Location;
        إذا (getBinLocation(ثنائی_Location))
        {
            ثنائی_Location = get_ماپا(ثنائی_Location);
#جے_بنا_تعریف CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ثنائی_Location);
#وإلا
            paths.push_back(ثنائی_Location + منزل_File_System_رستہ("/") + منزل_File_System_رستہ(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#جے_انت
        }
    }
    ثابت std::string default_اظہاریہ = library_اگیتر() + "opencv_core_parallel_" + ادھار_Name_l + "*" + librarySuffix();
    ثابت std::string plugin_اظہاریہ = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + ادھار_Name_u).c_str(), default_اظہاریہ.c_str());
    std::vector<File_System_رستہ_t> results;
#جے_تعریف_شدہ _WIN32
    File_System_رستہ_t moduleName = منزل_File_System_رستہ(library_اگیتر() + "opencv_core_parallel_" + ادھار_Name_l + librarySuffix());
    إذا (plugin_اظہاریہ != default_اظہاریہ)
    {
        moduleName = منزل_File_System_رستہ(plugin_اظہاریہ);
        results.push_back(moduleName);
    }
    لكل (ثابت File_System_رستہ_t& رستہ : paths)
    {
        results.push_back(رستہ + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وإلا
    CV_LOG_DEBUG(عدم, "core(parallel): " << ادھار_Name << " plugin's glob is '" << plugin_اظہاریہ << "', " << paths.size() << " location(s)");
    لكل (ثابت std::string& رستہ : paths)
    {
        إذا (رستہ.empty())
            استمر;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(رستہ, plugin_اظہاریہ), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(عدم, "    - " << رستہ << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#جے_انت
    CV_LOG_DEBUG(عدم, "Found " << results.size() << " plugin(s) for " << ادھار_Name);
    أرجع results;
}

فراغ PluginParallelBackendFactory::لدو_Plugin()
{
    لكل (ثابت File_System_رستہ_t& plugin : getPluginCandidates(ادھار_Name_2))
    {
        خودکار lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        إذا (!lib->isLoaded())
        {
            استمر;
        }
        حاول
        {
            خودکار pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            إذا (!pluginBackend)
            {
                استمر;
            }
            إذا (pluginBackend->plugin_api_ == عدم)
            {
                CV_LOG_ERROR(عدم, "core(parallel): no compatible plugin API for backend: " << ادھار_Name_2 << " in " << منزل_Printable_رستہ(plugin));
                استمر;
            }
#إذا !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#جے_انت
            backend = pluginBackend;
            أرجع;
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): exception during plugin initialization: " << منزل_Printable_رستہ(plugin) << ". SKIP");
        }
    }
}

#جے_انت  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نطاق parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& ادھار_Name)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    أرجع std::make_shared<impl::PluginParallelBackendFactory>(ادھار_Name);
#وإلا
    CV_بن_ورتیا(ادھار_Name);
    أرجع std::shared_ptr<IParallelBackendFactory>();
#جے_انت
}

}}  // namespace
