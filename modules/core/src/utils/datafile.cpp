// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv_data_config.hpp"

#ଅନ୍ତର୍ଭାଵ <vector>
#ଅନ୍ତର୍ଭାଵ <fstream>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/logger.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ଅନ୍ତର୍ଭାଵ <windows.h>
#undef small
#undef min
#undef max
#undef abs
#ନଚେତଯଦି defined(__linux__)
#ଅନ୍ତର୍ଭାଵ <dlfcn.h>  // requires -ldl
#ନଚେତଯଦି defined(__APPLE__)
#ଅନ୍ତର୍ଭାଵ <TargetConditionals.h>
#ଯଦି TARGET_OS_MAC
#ଅନ୍ତର୍ଭାଵ <dlfcn.h>
#endif
#endif

ନାମାକାଶ cv { ନାମାକାଶ utils {

ସ୍ଥିର cv::Ptr< std::vector<cv::String> > g_data_ସନ୍ଧାନ_path;
ସ୍ଥିର cv::Ptr< std::vector<cv::String> > g_data_ସନ୍ଧାନ_subdir;

ସ୍ଥିର std::vector<cv::String>& get_Data_ସନ୍ଧାନ_Path()
{
    ଯଦି (g_data_ସନ୍ଧାନ_path.empty())
        g_data_ସନ୍ଧାନ_path.reset(ନୂତନ std::vector<cv::String>());
    ଫେରନ୍ତୁ *(g_data_ସନ୍ଧାନ_path.get());
}

ସ୍ଥିର std::vector<cv::String>& get_Data_ସନ୍ଧାନ_Sub_Directory()
{
    ଯଦି (g_data_ସନ୍ଧାନ_subdir.empty())
    {
        g_data_ସନ୍ଧାନ_subdir.reset(ନୂତନ std::vector<cv::String>());
        g_data_ସନ୍ଧାନ_subdir->push_back("data");
        g_data_ସନ୍ଧାନ_subdir->push_back("");
    }
    ଫେରନ୍ତୁ *(g_data_ସନ୍ଧାନ_subdir.get());
}


CV_EXPORTS ଶୂନ୍ୟ add_Data_ସନ୍ଧାନ_Path(ଅଚଳ cv::String& path)
{
    ଯଦି (!path.empty() && utils::fs::isDirectory(path))
        get_Data_ସନ୍ଧାନ_Path().push_back(path);
}
CV_EXPORTS ଶୂନ୍ୟ add_Data_ସନ୍ଧାନ_Sub_Directory(ଅଚଳ cv::String& subdir)
{
    get_Data_ସନ୍ଧାନ_Sub_Directory().push_back(subdir);
}

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
ସ୍ଥିର ତାର୍କିକ isPathSep(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ c == '/' || c == '\\';
}
ସ୍ଥିର ତାର୍କିକ isSubDirectory_(ଅଚଳ cv::String& base_path, ଅଚଳ cv::String& path)
{
    size_t N = base_path.size();
    ଯଦି (N == 0)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (isPathSep(base_path[N - 1]))
        N--;
    ଯଦି (path.size() < N)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (size_t i = 0; i < N; i++)
    {
        ଯଦି (path[i] == base_path[i])
            ଜାରି;
        ଯଦି (isPathSep(path[i]) && isPathSep(base_path[i]))
            ଜାରି;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    size_t M = path.size();
    ଯଦି (M > N)
    {
        ଯଦି (!isPathSep(path[N]))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ isSubDirectory(ଅଚଳ cv::String& base_path, ଅଚଳ cv::String& path)
{
    ତାର୍କିକ res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(ଖାଲି, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    ଫେରନ୍ତୁ res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

ସ୍ଥିର cv::String getModuleLocation(ଅଚଳ ଶୂନ୍ୟ* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#ଯଦି _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ଯଦି (m)
    {
        TCHAR path[MAX_PATH];
        ଅଚଳ size_t path_size = ଆକାର(path) / ଆକାର(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        ଯଦି (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            ଅକ୍ଷର char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            ଫେରନ୍ତୁ cv::String(char_path);
#ଅନ୍ୟଥା
            ଫେରନ୍ତୁ cv::String(path);
#endif
        }
    }
#ନଚେତଯଦି defined(__linux__)
    Dl_info info;
    ଯଦି (0 != dladdr(addr, &info))
    {
        ଫେରନ୍ତୁ cv::String(info.dli_fname);
    }
#ନଚେତଯଦି defined(__APPLE__)
# ଯଦି TARGET_OS_MAC
    Dl_info info;
    ଯଦି (0 != dladdr(addr, &info))
    {
        ଫେରନ୍ତୁ cv::String(info.dli_fname);
    }
# endif
#ଅନ୍ୟଥା
    // not supported, skip
#endif
    ଫେରନ୍ତୁ cv::String();
}

ତାର୍କିକ getBinLocation(std::string& dst)
{
    dst = getModuleLocation((ଶୂନ୍ୟ*)getModuleLocation); // using code address, doesn't work with static linkage!
    ଫେରନ୍ତୁ !dst.empty();
}

#ifdef _WIN32
ତାର୍କିକ getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ଯଦି _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ଶୂନ୍ୟ* addr = (ଶୂନ୍ୟ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ଯଦି (m)
    {
        wchar_t path[4096];
        ଅଚଳ size_t path_size = ଆକାର(path)/ଆକାର(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        ଯଦି (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

cv::String findDataFile(ଅଚଳ cv::String& relative_path,
                        ଅଚଳ ଅକ୍ଷର* configuration_parameter,
                        ଅଚଳ std::vector<String>* ସନ୍ଧାନ_paths,
                        ଅଚଳ std::vector<String>* subdir_paths)
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ଖାଲି, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(ଖାଲି, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    ଯଦି(f) { \
        fclose(f); \
        ଫେରନ୍ତୁ path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ଅଚଳ std::vector<cv::String>& ସନ୍ଧାନ_path = ସନ୍ଧାନ_paths ? *ସନ୍ଧାନ_paths : get_Data_ସନ୍ଧାନ_Path();
    ପାଇଁ(size_t i = ସନ୍ଧାନ_path.size(); i > 0; i--)
    {
        ଅଚଳ cv::String& prefix = ସନ୍ଧାନ_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ଅଚଳ std::vector<cv::String>& ସନ୍ଧାନ_subdir = subdir_paths ? *subdir_paths : get_Data_ସନ୍ଧାନ_Sub_Directory();


    // Step 2
    ଅଚଳ cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ଅଚଳ cv::utils::Paths& ସନ୍ଧାନ_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    ପାଇଁ (size_t k = 0; k < ସନ୍ଧାନ_hint.size(); k++)
    {
        cv::String datapath = ସନ୍ଧାନ_hint[k];
        ଯଦି (datapath.empty())
            ଜାରି;
        ଯଦି (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
            {
                ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_WARNING(ଖାଲି, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ଅଚଳ cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    ପାଇଁ (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ଯଦି (datapath.empty())
            ଜାରି;
        ଯଦି (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
            {
                ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_WARNING(ଖାଲି, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ଯଦି (!override_paths.empty())
    {
        CV_LOG_INFO(ଖାଲି, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        ଫେରନ୍ତୁ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    ତାର୍କିକ has_tested_build_directory = ମିଥ୍ୟା;
    ଯଦି (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ଅଚଳ ଅକ୍ଷର* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_ସନ୍ଧାନ_PATHS };
        ପାଇଁ (size_t k = 0; k < ଆକାର(build_subdirs)/ଆକାର(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ଯଦି (utils::fs::isDirectory(datapath))
            {
                ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
                {
                    ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = ସତ୍ୟ;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା levels = 0; levels < 3; ++levels)
    {
        ଯଦି (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            ଭାଙ୍ଗ;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    ଯଦି (!source_dir.empty())
    {
        CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        ଯଦି (utils::fs::isDirectory(datapath))
        {
            ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
            {
                ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    ଯଦି (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(ଖାଲି, "Detected module path: '" << module_path << '\'');
    }
    ଅନ୍ୟଥା
    {
        CV_LOG_INFO(ଖାଲି, "Can't detect module binaries location");
    }

    ଯଦି (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ଅଚଳ ଅକ୍ଷର* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_ସନ୍ଧାନ_PATHS };
        ପାଇଁ (size_t k = 0; k < ଆକାର(build_subdirs)/ଆକାର(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ଯଦି (utils::fs::isDirectory(datapath))
            {
                ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
                {
                    ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#ଯଦି defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    ଯଦି (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        ଯଦି (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ଅଚଳ ଅକ୍ଷର* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        ପାଇଁ (size_t k = 0; k < ଆକାର(install_subdirs)/ଆକାର(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ଯଦି (utils::fs::isDirectory(datapath))
            {
                ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
                {
                    ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#ଯଦି defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ଯଦି (module_path.empty()) // can't determine
        module_path = install_dir;
    ଯଦି (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        ଯଦି (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ଖାଲି, "utils::findDataFile(): trying install path: " << datapath);
            ପାଇଁ(size_t i = ସନ୍ଧାନ_subdir.size(); i > 0; i--)
            {
                ଅଚଳ cv::String& subdir = ସନ୍ଧାନ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    ଫେରନ୍ତୁ cv::String();  // not found
#ଅନ୍ୟଥା // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(ସନ୍ଧାନ_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ଅଚଳ cv::String& relative_path, ତାର୍କିକ required, ଅଚଳ ଅକ୍ଷର* configuration_parameter)
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ଖାଲି, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                ଖାଲି,
                                                ଖାଲି);
    ଯଦି (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
