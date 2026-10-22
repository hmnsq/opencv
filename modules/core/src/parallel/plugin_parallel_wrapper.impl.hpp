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

استفاده نطاق cv::parallel;

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

استفاده نطاق cv::plugin::impl;  // plugin_loader.hpp

صنف PluginParallelBackend CV_FINAL: عام std::enable_shared_from_this<PluginParallelBackend>
{
محمي:
    فراغ initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_نماد(init_name));
        إذا (fn_init)
        {
            CV_LOG_DEBUG(عدم, "Found entry: '" << init_name << "'");
            لكل (صحيح supported_api_نسخه = API_نسخه; supported_api_نسخه >= 0; supported_api_نسخه--)
            {
                plugin_api_ = fn_init(ABI_نسخه, supported_api_نسخه, عدم);
                إذا (plugin_api_)
                    اكسر;
            }
            إذا (!plugin_api_)
            {
                CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                أرجع;
            }
            إذا (!checkCompatibility(plugin_api_->api_سرآیند, ABI_نسخه, API_نسخه, خطأ))
            {
                plugin_api_ = عدم;
                أرجع;
            }
            CV_LOG_INFO(عدم, "core(parallel): plugin is ready to use '" << plugin_api_->api_سرآیند.api_description << "'");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقي checkCompatibility(ثابت Open_CV_API_سرآیند& api_سرآیند, بیعلامت صحيح abi_نسخه, بیعلامت صحيح api_نسخه, منطقي checkMinorOpenCVVersion)
    {
        إذا (api_سرآیند.opencv_نسخه_major != CV_نسخه_MAJOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV major version used by plugin '" << api_سرآیند.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخه "'", api_سرآیند.opencv_نسخه_major, api_سرآیند.opencv_نسخه_minor))
            أرجع خطأ;
        }
        إذا (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وإلا إذا (api_سرآیند.opencv_نسخه_minor != CV_نسخه_MINOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV minor version used by plugin '" << api_سرآیند.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخه "'", api_سرآیند.opencv_نسخه_major, api_سرآیند.opencv_نسخه_minor))
            أرجع خطأ;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): initialized '" << api_سرآیند.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_سرآیند.opencv_نسخه_major, api_سرآیند.opencv_نسخه_minor,
                 api_سرآیند.min_api_نسخه, api_سرآیند.api_نسخه)
            << ", current OpenCV version is '" CV_نسخه "' (ABI/API = " << abi_نسخه << "/" << api_نسخه << ")"
        );
        إذا (api_سرآیند.min_api_نسخه != abi_نسخه)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(عدم, "core(parallel): plugin is not supported due to incompatible ABI = " << api_سرآیند.min_api_نسخه);
            أرجع خطأ;
        }
        إذا (api_سرآیند.api_نسخه != api_نسخه)
        {
            CV_LOG_INFO(عدم, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_سرآیند.api_نسخه, api_نسخه));
            إذا (api_سرآیند.api_نسخه < api_نسخه)
            {
                CV_LOG_INFO(عدم, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        أرجع درست;
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
    std::string پایه_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقي initialized;
عام:
    PluginParallelBackendFactory(ثابت std::string& پایه_Name)
        : پایه_Name_2(پایه_Name)
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
        Auto_قفل lock(getInitializationMutex());
        حاول
        {
            إذا (!initialized)
                بارگیری_Plugin();
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "core(parallel): exception during plugin loading: " << پایه_Name_2 << ". SKIP");
        }
        initialized = درست;
    }
    فراغ بارگیری_Plugin();
};

ساكن
std::vector<File_System_مسیر_t> getPluginCandidates(ثابت std::string& پایه_Name)
{
    استفاده نطاق cv::utils;
    استفاده نطاق cv::utils::fs;
    ثابت std::string پایه_Name_l = مقصد_پایینی_Case(پایه_Name);
    ثابت std::string پایه_Name_u = مقصد_فوقانی_Case(پایه_Name);
    ثابت File_System_مسیر_t پایه_Name_l_fs = مقصد_File_System_مسیر(پایه_Name_l);
    std::vector<File_System_مسیر_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    إذا (paths_.size() != 0)
    {
        لكل (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(مقصد_File_System_مسیر(paths_[i]));
        }
    }
    وإلا
    {
        File_System_مسیر_t دودویی_Location;
        إذا (getBinLocation(دودویی_Location))
        {
            دودویی_Location = get_والد(دودویی_Location);
#اگر_تعریف_نشده CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(دودویی_Location);
#وإلا
            paths.push_back(دودویی_Location + مقصد_File_System_مسیر("/") + مقصد_File_System_مسیر(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#پایان_اگر
        }
    }
    ثابت std::string default_عبارت = library_پیشوند() + "opencv_core_parallel_" + پایه_Name_l + "*" + librarySuffix();
    ثابت std::string plugin_عبارت = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + پایه_Name_u).c_str(), default_عبارت.c_str());
    std::vector<File_System_مسیر_t> results;
#اگر_تعریف_شده _WIN32
    File_System_مسیر_t moduleName = مقصد_File_System_مسیر(library_پیشوند() + "opencv_core_parallel_" + پایه_Name_l + librarySuffix());
    إذا (plugin_عبارت != default_عبارت)
    {
        moduleName = مقصد_File_System_مسیر(plugin_عبارت);
        results.push_back(moduleName);
    }
    لكل (ثابت File_System_مسیر_t& مسیر : paths)
    {
        results.push_back(مسیر + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وإلا
    CV_LOG_DEBUG(عدم, "core(parallel): " << پایه_Name << " plugin's glob is '" << plugin_عبارت << "', " << paths.size() << " location(s)");
    لكل (ثابت std::string& مسیر : paths)
    {
        إذا (مسیر.empty())
            استمر;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(مسیر, plugin_عبارت), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(عدم, "    - " << مسیر << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#پایان_اگر
    CV_LOG_DEBUG(عدم, "Found " << results.size() << " plugin(s) for " << پایه_Name);
    أرجع results;
}

فراغ PluginParallelBackendFactory::بارگیری_Plugin()
{
    لكل (ثابت File_System_مسیر_t& plugin : getPluginCandidates(پایه_Name_2))
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
                CV_LOG_ERROR(عدم, "core(parallel): no compatible plugin API for backend: " << پایه_Name_2 << " in " << مقصد_Printable_مسیر(plugin));
                استمر;
            }
#إذا !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#پایان_اگر
            backend = pluginBackend;
            أرجع;
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): exception during plugin initialization: " << مقصد_Printable_مسیر(plugin) << ". SKIP");
        }
    }
}

#پایان_اگر  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نطاق parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& پایه_Name)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    أرجع std::make_shared<impl::PluginParallelBackendFactory>(پایه_Name);
#وإلا
    CV_استفادهنشده(پایه_Name);
    أرجع std::shared_ptr<IParallelBackendFactory>();
#پایان_اگر
}

}}  // namespace
