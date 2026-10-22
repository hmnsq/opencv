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

کارول نطاق cv::parallel;

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

کارول نطاق cv::plugin::impl;  // plugin_loader.hpp

صنف PluginParallelBackend CV_FINAL: عام std::enable_shared_from_this<PluginParallelBackend>
{
محمي:
    فراغ initPluginAPI()
    {
        ثابت حرف* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_نښه(init_name));
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
            إذا (!checkCompatibility(plugin_api_->api_سرلیک, ABI_نسخه, API_نسخه, خطأ))
            {
                plugin_api_ = عدم;
                أرجع;
            }
            CV_LOG_INFO(عدم, "core(parallel): plugin is ready to use '" << plugin_api_->api_سرلیک.api_description << "'");
        }
        وإلا
        {
            CV_LOG_INFO(عدم, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    منطقي checkCompatibility(ثابت Open_CV_API_سرلیک& api_سرلیک, بینښه صحيح abi_نسخه, بینښه صحيح api_نسخه, منطقي checkMinorOpenCVVersion)
    {
        إذا (api_سرلیک.opencv_نسخه_major != CV_نسخه_MAJOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV major version used by plugin '" << api_سرلیک.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخه "'", api_سرلیک.opencv_نسخه_major, api_سرلیک.opencv_نسخه_minor))
            أرجع خطأ;
        }
        إذا (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        وإلا إذا (api_سرلیک.opencv_نسخه_minor != CV_نسخه_MINOR)
        {
            CV_LOG_ERROR(عدم, "core(parallel): wrong OpenCV minor version used by plugin '" << api_سرلیک.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_نسخه "'", api_سرلیک.opencv_نسخه_major, api_سرلیک.opencv_نسخه_minor))
            أرجع خطأ;
        }
        CV_LOG_DEBUG(عدم, "core(parallel): initialized '" << api_سرلیک.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_سرلیک.opencv_نسخه_major, api_سرلیک.opencv_نسخه_minor,
                 api_سرلیک.min_api_نسخه, api_سرلیک.api_نسخه)
            << ", current OpenCV version is '" CV_نسخه "' (ABI/API = " << abi_نسخه << "/" << api_نسخه << ")"
        );
        إذا (api_سرلیک.min_api_نسخه != abi_نسخه)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(عدم, "core(parallel): plugin is not supported due to incompatible ABI = " << api_سرلیک.min_api_نسخه);
            أرجع خطأ;
        }
        إذا (api_سرلیک.api_نسخه != api_نسخه)
        {
            CV_LOG_INFO(عدم, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_سرلیک.api_نسخه, api_نسخه));
            إذا (api_سرلیک.api_نسخه < api_نسخه)
            {
                CV_LOG_INFO(عدم, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        أرجع ریښتیا;
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
    std::string بنسټ_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    منطقي initialized;
عام:
    PluginParallelBackendFactory(ثابت std::string& بنسټ_Name)
        : بنسټ_Name_2(بنسټ_Name)
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
        Auto_قلف lock(getInitializationMutex());
        حاول
        {
            إذا (!initialized)
                بارول_Plugin();
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "core(parallel): exception during plugin loading: " << بنسټ_Name_2 << ". SKIP");
        }
        initialized = ریښتیا;
    }
    فراغ بارول_Plugin();
};

ساكن
std::vector<File_System_لار_t> getPluginCandidates(ثابت std::string& بنسټ_Name)
{
    کارول نطاق cv::utils;
    کارول نطاق cv::utils::fs;
    ثابت std::string بنسټ_Name_l = منزل_ښکتنی_Case(بنسټ_Name);
    ثابت std::string بنسټ_Name_u = منزل_پاسنی_Case(بنسټ_Name);
    ثابت File_System_لار_t بنسټ_Name_l_fs = منزل_File_System_لار(بنسټ_Name_l);
    std::vector<File_System_لار_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    ثابت std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    إذا (paths_.size() != 0)
    {
        لكل (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(منزل_File_System_لار(paths_[i]));
        }
    }
    وإلا
    {
        File_System_لار_t دوګونی_Location;
        إذا (getBinLocation(دوګونی_Location))
        {
            دوګونی_Location = get_پلار(دوګونی_Location);
#که_نه_تعریف_شوی CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(دوګونی_Location);
#وإلا
            paths.push_back(دوګونی_Location + منزل_File_System_لار("/") + منزل_File_System_لار(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#د_که_پای
        }
    }
    ثابت std::string default_بیان = library_مختاړی() + "opencv_core_parallel_" + بنسټ_Name_l + "*" + librarySuffix();
    ثابت std::string plugin_بیان = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + بنسټ_Name_u).c_str(), default_بیان.c_str());
    std::vector<File_System_لار_t> results;
#که_تعریف_شوی _WIN32
    File_System_لار_t moduleName = منزل_File_System_لار(library_مختاړی() + "opencv_core_parallel_" + بنسټ_Name_l + librarySuffix());
    إذا (plugin_بیان != default_بیان)
    {
        moduleName = منزل_File_System_لار(plugin_بیان);
        results.push_back(moduleName);
    }
    لكل (ثابت File_System_لار_t& لار : paths)
    {
        results.push_back(لار + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#وإلا
    CV_LOG_DEBUG(عدم, "core(parallel): " << بنسټ_Name << " plugin's glob is '" << plugin_بیان << "', " << paths.size() << " location(s)");
    لكل (ثابت std::string& لار : paths)
    {
        إذا (لار.empty())
            استمر;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(لار, plugin_بیان), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(عدم, "    - " << لار << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#د_که_پای
    CV_LOG_DEBUG(عدم, "Found " << results.size() << " plugin(s) for " << بنسټ_Name);
    أرجع results;
}

فراغ PluginParallelBackendFactory::بارول_Plugin()
{
    لكل (ثابت File_System_لار_t& plugin : getPluginCandidates(بنسټ_Name_2))
    {
        خپلکاری lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        إذا (!lib->isLoaded())
        {
            استمر;
        }
        حاول
        {
            خپلکاری pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            إذا (!pluginBackend)
            {
                استمر;
            }
            إذا (pluginBackend->plugin_api_ == عدم)
            {
                CV_LOG_ERROR(عدم, "core(parallel): no compatible plugin API for backend: " << بنسټ_Name_2 << " in " << منزل_Printable_لار(plugin));
                استمر;
            }
#إذا !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#د_که_پای
            backend = pluginBackend;
            أرجع;
        }
        امسك (...)
        {
            CV_LOG_WARNING(عدم, "core(parallel): exception during plugin initialization: " << منزل_Printable_لار(plugin) << ". SKIP");
        }
    }
}

#د_که_پای  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

نطاق parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& بنسټ_Name)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    أرجع std::make_shared<impl::PluginParallelBackendFactory>(بنسټ_Name);
#وإلا
    CV_ناکارول_شوی(بنسټ_Name);
    أرجع std::shared_ptr<IParallelBackendFactory>();
#د_که_پای
}

}}  // namespace
