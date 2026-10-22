// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of parallel.cpp
//

//==================================================================================================
// Dynamic backend implementation

#అన్తర్భావ "opencv2/core/utils/plugin_loader.private.hpp"

నామాకాశ cv { నామాకాశ impl {

ఉపయోగించి నామాకాశ cv::parallel;

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

ఉపయోగించి నామాకాశ cv::plugin::impl;  // plugin_loader.hpp

తరగతి PluginParallelBackend CV_FINAL: బహిరంగ std::enable_shared_from_this<PluginParallelBackend>
{
రక్షిత:
    శూన్య initPluginAPI()
    {
        స్థిరరాశి అక్షరం* init_name = "opencv_core_parallel_plugin_init_v0";
        FN_opencv_core_parallel_plugin_init_t fn_init = reinterpret_cast<FN_opencv_core_parallel_plugin_init_t>(lib_->get_చిహ్నం(init_name));
        ఒకవేళ (fn_init)
        {
            CV_LOG_DEBUG(ఖాళీ, "Found entry: '" << init_name << "'");
            కొరకు (పూర్ణాంకం supported_api_సంస్కరణ = API_సంస్కరణ; supported_api_సంస్కరణ >= 0; supported_api_సంస్కరణ--)
            {
                plugin_api_ = fn_init(ABI_సంస్కరణ, supported_api_సంస్కరణ, ఖాళీ);
                ఒకవేళ (plugin_api_)
                    ఆపు;
            }
            ఒకవేళ (!plugin_api_)
            {
                CV_LOG_INFO(ఖాళీ, "core(parallel): plugin is incompatible (can't be initialized): " << lib_->getName());
                తిరిగి;
            }
            ఒకవేళ (!checkCompatibility(plugin_api_->api_శీర్షిక, ABI_సంస్కరణ, API_సంస్కరణ, అబద్ధం))
            {
                plugin_api_ = ఖాళీ;
                తిరిగి;
            }
            CV_LOG_INFO(ఖాళీ, "core(parallel): plugin is ready to use '" << plugin_api_->api_శీర్షిక.api_description << "'");
        }
        లేకపోతే
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): plugin is incompatible, missing init function: '" << init_name << "', file: " << lib_->getName());
        }
    }


    తార్కిక checkCompatibility(స్థిరరాశి Open_CV_API_శీర్షిక& api_శీర్షిక, గుర్తులేని పూర్ణాంకం abi_సంస్కరణ, గుర్తులేని పూర్ణాంకం api_సంస్కరణ, తార్కిక checkMinorOpenCVVersion)
    {
        ఒకవేళ (api_శీర్షిక.opencv_సంస్కరణ_major != CV_సంస్కరణ_MAJOR)
        {
            CV_LOG_ERROR(ఖాళీ, "core(parallel): wrong OpenCV major version used by plugin '" << api_శీర్షిక.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_సంస్కరణ "'", api_శీర్షిక.opencv_సంస్కరణ_major, api_శీర్షిక.opencv_సంస్కరణ_minor))
            తిరిగి అబద్ధం;
        }
        ఒకవేళ (!checkMinorOpenCVVersion)
        {
            // no checks for OpenCV minor version
        }
        లేకపోతే ఒకవేళ (api_శీర్షిక.opencv_సంస్కరణ_minor != CV_సంస్కరణ_MINOR)
        {
            CV_LOG_ERROR(ఖాళీ, "core(parallel): wrong OpenCV minor version used by plugin '" << api_శీర్షిక.api_description << "': " <<
                cv::format("%d.%d, OpenCV version is '" CV_సంస్కరణ "'", api_శీర్షిక.opencv_సంస్కరణ_major, api_శీర్షిక.opencv_సంస్కరణ_minor))
            తిరిగి అబద్ధం;
        }
        CV_LOG_DEBUG(ఖాళీ, "core(parallel): initialized '" << api_శీర్షిక.api_description << "': built with "
            << cv::format("OpenCV %d.%d (ABI/API = %d/%d)",
                 api_శీర్షిక.opencv_సంస్కరణ_major, api_శీర్షిక.opencv_సంస్కరణ_minor,
                 api_శీర్షిక.min_api_సంస్కరణ, api_శీర్షిక.api_సంస్కరణ)
            << ", current OpenCV version is '" CV_సంస్కరణ "' (ABI/API = " << abi_సంస్కరణ << "/" << api_సంస్కరణ << ")"
        );
        ఒకవేళ (api_శీర్షిక.min_api_సంస్కరణ != abi_సంస్కరణ)  // future: range can be here
        {
            // actually this should never happen due to checks in plugin's init() function
            CV_LOG_ERROR(ఖాళీ, "core(parallel): plugin is not supported due to incompatible ABI = " << api_శీర్షిక.min_api_సంస్కరణ);
            తిరిగి అబద్ధం;
        }
        ఒకవేళ (api_శీర్షిక.api_సంస్కరణ != api_సంస్కరణ)
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): NOTE: plugin is supported, but there is API version mismath: "
                << cv::format("plugin API level (%d) != OpenCV API level (%d)", api_శీర్షిక.api_సంస్కరణ, api_సంస్కరణ));
            ఒకవేళ (api_శీర్షిక.api_సంస్కరణ < api_సంస్కరణ)
            {
                CV_LOG_INFO(ఖాళీ, "core(parallel): NOTE: some functionality may be unavailable due to lack of support by plugin implementation");
            }
        }
        తిరిగి నిజం;
    }

బహిరంగ:
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib_;
    స్థిరరాశి OpenCV_Core_Parallel_Plugin_API* plugin_api_;

    PluginParallelBackend(స్థిరరాశి std::shared_ptr<cv::plugin::impl::DynamicLib>& lib)
        : lib_(lib)
        , plugin_api_(ఖాళీ)
    {
        initPluginAPI();
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() స్థిరరాశి
    {
        CV_Assert(plugin_api_);

        CvPluginParallelBackendAPI instancePtr = ఖాళీ;

        ఒకవేళ (plugin_api_->v0.getInstance)
        {
            ఒకవేళ (CV_ERROR_OK == plugin_api_->v0.getInstance(&instancePtr))
            {
                CV_Assert(instancePtr);
                // TODO C++20 "aliasing constructor"
                తిరిగి std::shared_ptr<cv::parallel::ParallelForAPI>(instancePtr, [](cv::parallel::ParallelForAPI*){});  // empty deleter
            }
        }
        తిరిగి std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
};


తరగతి PluginParallelBackendFactory CV_FINAL: బహిరంగ IParallelBackendFactory
{
బహిరంగ:
    std::string పునాది_Name_2;
    std::shared_ptr<PluginParallelBackend> backend;
    తార్కిక initialized;
బహిరంగ:
    PluginParallelBackendFactory(స్థిరరాశి std::string& పునాది_Name)
        : పునాది_Name_2(పునాది_Name)
        , initialized(అబద్ధం)
    {
        // nothing, plugins are loaded on demand
    }

    std::shared_ptr<cv::parallel::ParallelForAPI> create() స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!initialized)
        {
            const_cast<PluginParallelBackendFactory*>(ఇది)->initBackend();
        }
        ఒకవేళ (backend)
            తిరిగి backend->create();
        తిరిగి std::shared_ptr<cv::parallel::ParallelForAPI>();
    }
రక్షిత:
    శూన్య initBackend()
    {
        Auto_తాళం lock(getInitializationMutex());
        ప్రయత్నించు
        {
            ఒకవేళ (!initialized)
                ఎక్కించు_Plugin();
        }
        పట్టుకో (...)
        {
            CV_LOG_INFO(ఖాళీ, "core(parallel): exception during plugin loading: " << పునాది_Name_2 << ". SKIP");
        }
        initialized = నిజం;
    }
    శూన్య ఎక్కించు_Plugin();
};

స్థిర
std::vector<File_System_మార్గం_t> getPluginCandidates(స్థిరరాశి std::string& పునాది_Name)
{
    ఉపయోగించి నామాకాశ cv::utils;
    ఉపయోగించి నామాకాశ cv::utils::fs;
    స్థిరరాశి std::string పునాది_Name_l = గమ్యం_క్రింది_Case(పునాది_Name);
    స్థిరరాశి std::string పునాది_Name_u = గమ్యం_పై_Case(పునాది_Name);
    స్థిరరాశి File_System_మార్గం_t పునాది_Name_l_fs = గమ్యం_File_System_మార్గం(పునాది_Name_l);
    std::vector<File_System_మార్గం_t> paths;
    // TODO OPENCV_PLUGIN_PATH
    స్థిరరాశి std::vector<std::string> paths_ = getConfigurationParameterPaths("OPENCV_CORE_PLUGIN_PATH", std::vector<std::string>());
    ఒకవేళ (paths_.size() != 0)
    {
        కొరకు (size_t i = 0; i < paths_.size(); i++)
        {
            paths.push_back(గమ్యం_File_System_మార్గం(paths_[i]));
        }
    }
    లేకపోతే
    {
        File_System_మార్గం_t ద్విమాన_Location;
        ఒకవేళ (getBinLocation(ద్విమాన_Location))
        {
            ద్విమాన_Location = get_మాతృ(ద్విమాన_Location);
#నిర్వచితం_కాకపోతే CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY
            paths.push_back(ద్విమాన_Location);
#లేకపోతే
            paths.push_back(ద్విమాన_Location + గమ్యం_File_System_మార్గం("/") + గమ్యం_File_System_మార్గం(CV_CORE_PARALLEL_PLUGIN_SUBDIRECTORY_STR));
#అయితే_ముగింపు
        }
    }
    స్థిరరాశి std::string default_సమాసం = library_ఉపసర్గ() + "opencv_core_parallel_" + పునాది_Name_l + "*" + librarySuffix();
    స్థిరరాశి std::string plugin_సమాసం = getConfigurationParameterString((std::string("OPENCV_CORE_PARALLEL_PLUGIN_") + పునాది_Name_u).c_str(), default_సమాసం.c_str());
    std::vector<File_System_మార్గం_t> results;
#నిర్వచితమైతే _WIN32
    File_System_మార్గం_t moduleName = గమ్యం_File_System_మార్గం(library_ఉపసర్గ() + "opencv_core_parallel_" + పునాది_Name_l + librarySuffix());
    ఒకవేళ (plugin_సమాసం != default_సమాసం)
    {
        moduleName = గమ్యం_File_System_మార్గం(plugin_సమాసం);
        results.push_back(moduleName);
    }
    కొరకు (స్థిరరాశి File_System_మార్గం_t& మార్గం : paths)
    {
        results.push_back(మార్గం + L"\\" + moduleName);
    }
    results.push_back(moduleName);
#లేకపోతే
    CV_LOG_DEBUG(ఖాళీ, "core(parallel): " << పునాది_Name << " plugin's glob is '" << plugin_సమాసం << "', " << paths.size() << " location(s)");
    కొరకు (స్థిరరాశి std::string& మార్గం : paths)
    {
        ఒకవేళ (మార్గం.empty())
            కొనసాగు;
        std::vector<std::string> candidates;
        cv::glob(utils::fs::join(మార్గం, plugin_సమాసం), candidates);
        // Prefer candisates with higher versions
        // TODO: implemented accurate versions-based comparator
        std::sort(candidates.begin(), candidates.end(), std::greater<std::string>());
        CV_LOG_DEBUG(ఖాళీ, "    - " << మార్గం << ": " << candidates.size());
        copy(candidates.begin(), candidates.end(), back_inserter(results));
    }
#అయితే_ముగింపు
    CV_LOG_DEBUG(ఖాళీ, "Found " << results.size() << " plugin(s) for " << పునాది_Name);
    తిరిగి results;
}

శూన్య PluginParallelBackendFactory::ఎక్కించు_Plugin()
{
    కొరకు (స్థిరరాశి File_System_మార్గం_t& plugin : getPluginCandidates(పునాది_Name_2))
    {
        స్వయంచాలక lib = std::make_shared<cv::plugin::impl::DynamicLib>(plugin);
        ఒకవేళ (!lib->isLoaded())
        {
            కొనసాగు;
        }
        ప్రయత్నించు
        {
            స్వయంచాలక pluginBackend = std::make_shared<PluginParallelBackend>(lib);
            ఒకవేళ (!pluginBackend)
            {
                కొనసాగు;
            }
            ఒకవేళ (pluginBackend->plugin_api_ == ఖాళీ)
            {
                CV_LOG_ERROR(ఖాళీ, "core(parallel): no compatible plugin API for backend: " << పునాది_Name_2 << " in " << గమ్యం_Printable_మార్గం(plugin));
                కొనసాగు;
            }
#ఒకవేళ !defined(_WIN32)
            // NB: we are going to use parallel backend, so prevent automatic library unloading
            // (avoid uncontrolled crashes in worker threads of underlying libraries: libgomp, libtbb)
            // details: https://github.com/opencv/opencv/pull/19470#pullrequestreview-589834777
            lib->disableAutomaticLibraryUnloading();
#అయితే_ముగింపు
            backend = pluginBackend;
            తిరిగి;
        }
        పట్టుకో (...)
        {
            CV_LOG_WARNING(ఖాళీ, "core(parallel): exception during plugin initialization: " << గమ్యం_Printable_మార్గం(plugin) << ". SKIP");
        }
    }
}

#అయితే_ముగింపు  // OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)

}  // namespace

నామాకాశ parallel {

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(స్థిరరాశి std::string& పునాది_Name)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT && defined(PARALLEL_ENABLE_PLUGINS)
    తిరిగి std::make_shared<impl::PluginParallelBackendFactory>(పునాది_Name);
#లేకపోతే
    CV_వాడని(పునాది_Name);
    తిరిగి std::shared_ptr<IParallelBackendFactory>();
#అయితే_ముగింపు
}

}}  // namespace
