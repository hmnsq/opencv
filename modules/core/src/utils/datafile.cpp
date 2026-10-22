// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "../precomp.hpp"

#అన్తర్భావ "opencv_data_config.hpp"

#అన్తర్భావ <vector>
#అన్తర్భావ <fstream>

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచనం_రద్దు CV_LOG_STRIP_LEVEL
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అన్తర్భావ "opencv2/core/utils/logger.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.hpp"

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"

#నిర్వచితమైతే _WIN32
#నిర్వచించు WIN32_LEAN_AND_సగటు
#అన్తర్భావ <windows.h>
#నిర్వచనం_రద్దు small
#నిర్వచనం_రద్దు min
#నిర్వచనం_రద్దు max
#నిర్వచనం_రద్దు నిరపేక్ష
#లేదావేళ defined(__linux__)
#అన్తర్భావ <dlfcn.h>  // requires -ldl
#లేదావేళ defined(__APPLE__)
#అన్తర్భావ <TargetConditionals.h>
#ఒకవేళ లక్ష్యం_OS_MAC
#అన్తర్భావ <dlfcn.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

నామాకాశ cv { నామాకాశ utils {

స్థిర cv::Ptr< std::vector<cv::String> > g_దత్తాంశం_శోధన_మార్గం;
స్థిర cv::Ptr< std::vector<cv::String> > g_దత్తాంశం_శోధన_subdir;

స్థిర std::vector<cv::String>& get_దత్తాంశం_శోధన_మార్గం()
{
    ఒకవేళ (g_దత్తాంశం_శోధన_మార్గం.empty())
        g_దత్తాంశం_శోధన_మార్గం.reset(కొత్త std::vector<cv::String>());
    తిరిగి *(g_దత్తాంశం_శోధన_మార్గం.get());
}

స్థిర std::vector<cv::String>& get_దత్తాంశం_శోధన_Sub_Directory()
{
    ఒకవేళ (g_దత్తాంశం_శోధన_subdir.empty())
    {
        g_దత్తాంశం_శోధన_subdir.reset(కొత్త std::vector<cv::String>());
        g_దత్తాంశం_శోధన_subdir->push_back("data");
        g_దత్తాంశం_శోధన_subdir->push_back("");
    }
    తిరిగి *(g_దత్తాంశం_శోధన_subdir.get());
}


CV_EXPORTS శూన్య కూడు_దత్తాంశం_శోధన_మార్గం(స్థిరరాశి cv::String& మార్గం)
{
    ఒకవేళ (!మార్గం.empty() && utils::fs::isDirectory(మార్గం))
        get_దత్తాంశం_శోధన_మార్గం().push_back(మార్గం);
}
CV_EXPORTS శూన్య కూడు_దత్తాంశం_శోధన_Sub_Directory(స్థిరరాశి cv::String& subdir)
{
    get_దత్తాంశం_శోధన_Sub_Directory().push_back(subdir);
}

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
స్థిర తార్కిక is_మార్గం_Sep(అక్షరం c)
{
    తిరిగి c == '/' || c == '\\';
}
స్థిర తార్కిక isSubDirectory_(స్థిరరాశి cv::String& పునాది_మార్గం, స్థిరరాశి cv::String& మార్గం)
{
    size_t N = పునాది_మార్గం.size();
    ఒకవేళ (N == 0)
        తిరిగి అబద్ధం;
    ఒకవేళ (is_మార్గం_Sep(పునాది_మార్గం[N - 1]))
        N--;
    ఒకవేళ (మార్గం.size() < N)
        తిరిగి అబద్ధం;
    కొరకు (size_t i = 0; i < N; i++)
    {
        ఒకవేళ (మార్గం[i] == పునాది_మార్గం[i])
            కొనసాగు;
        ఒకవేళ (is_మార్గం_Sep(మార్గం[i]) && is_మార్గం_Sep(పునాది_మార్గం[i]))
            కొనసాగు;
        తిరిగి అబద్ధం;
    }
    size_t M = మార్గం.size();
    ఒకవేళ (M > N)
    {
        ఒకవేళ (!is_మార్గం_Sep(మార్గం[N]))
            తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
}

స్థిర తార్కిక isSubDirectory(స్థిరరాశి cv::String& పునాది_మార్గం, స్థిరరాశి cv::String& మార్గం)
{
    తార్కిక res = isSubDirectory_(పునాది_మార్గం, మార్గం);
    CV_LOG_VERBOSE(ఖాళీ, 0, "isSubDirectory(): base: " << పునాది_మార్గం << "  path: " << మార్గం << "  => result: " << (res ? "TRUE" : "FALSE"));
    తిరిగి res;
}
#అయితే_ముగింపు //OPENCV_HAVE_FILESYSTEM_SUPPORT

స్థిర cv::String getModuleLocation(స్థిరరాశి శూన్య* addr)
{
    CV_వాడని(addr);
#నిర్వచితమైతే _WIN32
    HMODULE m = 0;
#ఒకవేళ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_చిరునామా | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#అయితే_ముగింపు
    ఒకవేళ (m)
    {
        TCHAR మార్గం[MAX_మార్గం];
        స్థిరరాశి size_t మార్గం_కొలత = పరిమాణం(మార్గం) / పరిమాణం(*మార్గం);
        size_t sz = GetModuleFileName(m, మార్గం, మార్గం_కొలత);
        ఒకవేళ (sz > 0 && sz < మార్గం_కొలత)
        {
            మార్గం[sz] = TCHAR('\0');
#నిర్వచితమైతే _UNICODE
            అక్షరం char_మార్గం[MAX_మార్గం];
            size_t copied = wcstombs(char_మార్గం, మార్గం, MAX_మార్గం);
            CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
            తిరిగి cv::String(char_మార్గం);
#లేకపోతే
            తిరిగి cv::String(మార్గం);
#అయితే_ముగింపు
        }
    }
#లేదావేళ defined(__linux__)
    Dl_info info;
    ఒకవేళ (0 != dladdr(addr, &info))
    {
        తిరిగి cv::String(info.dli_fname);
    }
#లేదావేళ defined(__APPLE__)
# ఒకవేళ లక్ష్యం_OS_MAC
    Dl_info info;
    ఒకవేళ (0 != dladdr(addr, &info))
    {
        తిరిగి cv::String(info.dli_fname);
    }
# endif
#లేకపోతే
    // not supported, skip
#అయితే_ముగింపు
    తిరిగి cv::String();
}

తార్కిక getBinLocation(std::string& dst)
{
    dst = getModuleLocation((శూన్య*)getModuleLocation); // using code address, doesn't work with static linkage!
    తిరిగి !dst.empty();
}

#నిర్వచితమైతే _WIN32
తార్కిక getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ఒకవేళ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    శూన్య* addr = (శూన్య*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_చిరునామా | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#అయితే_ముగింపు
    ఒకవేళ (m)
    {
        wchar_t మార్గం[4096];
        స్థిరరాశి size_t మార్గం_కొలత = పరిమాణం(మార్గం)/పరిమాణం(*మార్గం);
        size_t sz = GetModuleFileNameW(m, మార్గం, మార్గం_కొలత);
        ఒకవేళ (sz > 0 && sz < మార్గం_కొలత)
        {
            మార్గం[sz] = '\0';
            dst.assign(మార్గం, sz);
            తిరిగి నిజం;
        }
    }
    తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

cv::String కనుగొను_దత్తాంశం_File(స్థిరరాశి cv::String& relative_మార్గం,
                        స్థిరరాశి అక్షరం* configuration_parameter,
                        స్థిరరాశి std::vector<String>* శోధన_paths,
                        స్థిరరాశి std::vector<String>* subdir_paths)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ఖాళీ, cv::format("utils::findDataFile('%s', %s)", relative_మార్గం.c_str(), configuration_parameter));

#నిర్వచించు TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ) \
{ \
    cv::String మార్గం = utils::fs::join(ఉపసర్గ, relative_మార్గం); \
    CV_LOG_DEBUG(ఖాళీ, cv::format("... Line %d: trying open '%s'", __LINE__, మార్గం.c_str())); \
    FILE* f = fopen(మార్గం.c_str(), "rb"); \
    ఒకవేళ(f) { \
        fclose(f); \
        తిరిగి మార్గం; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_ఉపసర్గ("");


    // Step 1
    స్థిరరాశి std::vector<cv::String>& శోధన_మార్గం = శోధన_paths ? *శోధన_paths : get_దత్తాంశం_శోధన_మార్గం();
    కొరకు(size_t i = శోధన_మార్గం.size(); i > 0; i--)
    {
        స్థిరరాశి cv::String& ఉపసర్గ = శోధన_మార్గం[i - 1];
        TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
    }

    స్థిరరాశి std::vector<cv::String>& శోధన_subdir = subdir_paths ? *subdir_paths : get_దత్తాంశం_శోధన_Sub_Directory();


    // Step 2
    స్థిరరాశి cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    స్థిరరాశి cv::utils::Paths& శోధన_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    కొరకు (size_t k = 0; k < శోధన_hint.size(); k++)
    {
        cv::String datapath = శోధన_hint[k];
        ఒకవేళ (datapath.empty())
            కొనసాగు;
        ఒకవేళ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
            {
                స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
            }
        }
        లేకపోతే
        {
            CV_LOG_WARNING(ఖాళీ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    స్థిరరాశి cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    కొరకు (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ఒకవేళ (datapath.empty())
            కొనసాగు;
        ఒకవేళ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
            {
                స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
            }
        }
        లేకపోతే
        {
            CV_LOG_WARNING(ఖాళీ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ఒకవేళ (!override_paths.empty())
    {
        CV_LOG_INFO(ఖాళీ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_మార్గం);
        తిరిగి cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    తార్కిక has_tested_build_directory = అబద్ధం;
    ఒకవేళ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        స్థిరరాశి అక్షరం* build_subdirs[] = { OPENCV_దత్తాంశం_BUILD_DIR_శోధన_PATHS };
        కొరకు (size_t k = 0; k < పరిమాణం(build_subdirs)/పరిమాణం(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ఒకవేళ (utils::fs::isDirectory(datapath))
            {
                కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
                {
                    స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                    cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
                }
            }
        }
        has_tested_build_directory = నిజం;
    }

    cv::String ఆధారం_dir;
    cv::String try_ఆధారం_dir = cwd;
    కొరకు (పూర్ణాంకం స్థాయిలు = 0; స్థాయిలు < 3; ++స్థాయిలు)
    {
        ఒకవేళ (utils::fs::exists(utils::fs::join(try_ఆధారం_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            ఆధారం_dir = try_ఆధారం_dir;
            ఆపు;
        }
        try_ఆధారం_dir = utils::fs::join(try_ఆధారం_dir, "/..");
    }
    ఒకవేళ (!ఆధారం_dir.empty())
    {
        CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): the current directory is source sub-directory: " << ఆధారం_dir);
        CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): <source>" << ఆధారం_dir);
        cv::String datapath = ఆధారం_dir;
        ఒకవేళ (utils::fs::isDirectory(datapath))
        {
            కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
            {
                స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
            }
        }
    }

    cv::String module_మార్గం;
    ఒకవేళ (getBinLocation(module_మార్గం))
    {
        CV_LOG_DEBUG(ఖాళీ, "Detected module path: '" << module_మార్గం << '\'');
    }
    లేకపోతే
    {
        CV_LOG_INFO(ఖాళీ, "Can't detect module binaries location");
    }

    ఒకవేళ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_మార్గం) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_మార్గం)))
    )
    {
        CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_మార్గం);
        స్థిరరాశి అక్షరం* build_subdirs[] = { OPENCV_దత్తాంశం_BUILD_DIR_శోధన_PATHS };
        కొరకు (size_t k = 0; k < పరిమాణం(build_subdirs)/పరిమాణం(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ఒకవేళ (utils::fs::isDirectory(datapath))
            {
                కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
                {
                    స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                    cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
                }
            }
        }
    }

#ఒకవేళ defined OPENCV_INSTALL_దత్తాంశం_DIR_RELATIVE
    ఒకవేళ (!module_మార్గం.empty())  // require module path
    {
        size_t pos = module_మార్గం.rfind('/');
        ఒకవేళ (pos == cv::String::npos)
            pos = module_మార్గం.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_మార్గం : module_మార్గం.substr(0, pos);
        స్థిరరాశి అక్షరం* install_subdirs[] = { OPENCV_INSTALL_దత్తాంశం_DIR_RELATIVE };
        కొరకు (size_t k = 0; k < పరిమాణం(install_subdirs)/పరిమాణం(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ఒకవేళ (utils::fs::isDirectory(datapath))
            {
                కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
                {
                    స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                    cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
                }
            }
            లేకపోతే
            {
                CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#అయితే_ముగింపు

#ఒకవేళ defined OPENCV_INSTALL_ఉపసర్గ && defined OPENCV_దత్తాంశం_INSTALL_మార్గం
    cv::String install_dir(OPENCV_INSTALL_ఉపసర్గ);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ఒకవేళ (module_మార్గం.empty()) // can't determine
        module_మార్గం = install_dir;
    ఒకవేళ (isSubDirectory(install_dir, module_మార్గం) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_మార్గం)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_దత్తాంశం_INSTALL_మార్గం);
        ఒకవేళ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ఖాళీ, "utils::findDataFile(): trying install path: " << datapath);
            కొరకు(size_t i = శోధన_subdir.size(); i > 0; i--)
            {
                స్థిరరాశి cv::String& subdir = శోధన_subdir[i - 1];
                cv::String ఉపసర్గ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ఉపసర్గ(ఉపసర్గ);
            }
        }
    }
#అయితే_ముగింపు

    తిరిగి cv::String();  // not found
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(relative_మార్గం);
    CV_వాడని(configuration_parameter);
    CV_వాడని(శోధన_paths);
    CV_వాడని(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String కనుగొను_దత్తాంశం_File(స్థిరరాశి cv::String& relative_మార్గం, తార్కిక required, స్థిరరాశి అక్షరం* configuration_parameter)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ఖాళీ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_మార్గం.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String ఫలితం = cv::utils::కనుగొను_దత్తాంశం_File(relative_మార్గం,
                                                configuration_parameter,
                                                ఖాళీ,
                                                ఖాళీ);
    ఒకవేళ (ఫలితం.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_మార్గం.c_str()));
    తిరిగి ఫలితం;
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(relative_మార్గం);
    CV_వాడని(required);
    CV_వాడని(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
