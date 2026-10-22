// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ "opencv_data_config.hpp"

#ಅನ್ತರ್ಭಾವ <vector>
#ಅನ್ತರ್ಭಾವ <fstream>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/logger.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ಅನ್ತರ್ಭಾವ <windows.h>
#undef small
#undef min
#undef max
#undef abs
#ನೋಚೇದ್ಯದಿ defined(__linux__)
#ಅನ್ತರ್ಭಾವ <dlfcn.h>  // requires -ldl
#ನೋಚೇದ್ಯದಿ defined(__APPLE__)
#ಅನ್ತರ್ಭಾವ <TargetConditionals.h>
#ಯದಿ TARGET_OS_MAC
#ಅನ್ತರ್ಭಾವ <dlfcn.h>
#endif
#endif

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils {

ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_data_ಹುಡುಕು_path;
ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_data_ಹುಡುಕು_subdir;

ಸ್ಥಿರ std::vector<cv::String>& get_Data_ಹುಡುಕು_Path()
{
    ಯದಿ (g_data_ಹುಡುಕು_path.empty())
        g_data_ಹುಡುಕು_path.reset(ನೂತನ std::vector<cv::String>());
    ಪ್ರತ್ಯಾಗಮ *(g_data_ಹುಡುಕು_path.get());
}

ಸ್ಥಿರ std::vector<cv::String>& get_Data_ಹುಡುಕು_Sub_Directory()
{
    ಯದಿ (g_data_ಹುಡುಕು_subdir.empty())
    {
        g_data_ಹುಡುಕು_subdir.reset(ನೂತನ std::vector<cv::String>());
        g_data_ಹುಡುಕು_subdir->push_back("data");
        g_data_ಹುಡುಕು_subdir->push_back("");
    }
    ಪ್ರತ್ಯಾಗಮ *(g_data_ಹುಡುಕು_subdir.get());
}


CV_EXPORTS ಶೂನ್ಯ add_Data_ಹುಡುಕು_Path(ಧ್ರುವ cv::String& path)
{
    ಯದಿ (!path.empty() && utils::fs::isDirectory(path))
        get_Data_ಹುಡುಕು_Path().push_back(path);
}
CV_EXPORTS ಶೂನ್ಯ add_Data_ಹುಡುಕು_Sub_Directory(ಧ್ರುವ cv::String& subdir)
{
    get_Data_ಹುಡುಕು_Sub_Directory().push_back(subdir);
}

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
ಸ್ಥಿರ ತಾರ್ಕಿಕ isPathSep(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ c == '/' || c == '\\';
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ isSubDirectory_(ಧ್ರುವ cv::String& base_path, ಧ್ರುವ cv::String& path)
{
    size_t N = base_path.size();
    ಯದಿ (N == 0)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (isPathSep(base_path[N - 1]))
        N--;
    ಯದಿ (path.size() < N)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (size_t i = 0; i < N; i++)
    {
        ಯದಿ (path[i] == base_path[i])
            ಅನುವರ್ತ;
        ಯದಿ (isPathSep(path[i]) && isPathSep(base_path[i]))
            ಅನುವರ್ತ;
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    size_t M = path.size();
    ಯದಿ (M > N)
    {
        ಯದಿ (!isPathSep(path[N]))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ isSubDirectory(ಧ್ರುವ cv::String& base_path, ಧ್ರುವ cv::String& path)
{
    ತಾರ್ಕಿಕ res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    ಪ್ರತ್ಯಾಗಮ res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

ಸ್ಥಿರ cv::String getModuleLocation(ಧ್ರುವ ಶೂನ್ಯ* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#ಯದಿ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ಯದಿ (m)
    {
        TCHAR path[MAX_PATH];
        ಧ್ರುವ size_t path_size = ಪರಿಮಾಣ(path) / ಪರಿಮಾಣ(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        ಯದಿ (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            ವರ್ಣ char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            ಪ್ರತ್ಯಾಗಮ cv::String(char_path);
#ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ cv::String(path);
#endif
        }
    }
#ನೋಚೇದ್ಯದಿ defined(__linux__)
    Dl_info info;
    ಯದಿ (0 != dladdr(addr, &info))
    {
        ಪ್ರತ್ಯಾಗಮ cv::String(info.dli_fname);
    }
#ನೋಚೇದ್ಯದಿ defined(__APPLE__)
# ಯದಿ TARGET_OS_MAC
    Dl_info info;
    ಯದಿ (0 != dladdr(addr, &info))
    {
        ಪ್ರತ್ಯಾಗಮ cv::String(info.dli_fname);
    }
# endif
#ಅನ್ಯಥಾ
    // not supported, skip
#endif
    ಪ್ರತ್ಯಾಗಮ cv::String();
}

ತಾರ್ಕಿಕ getBinLocation(std::string& dst)
{
    dst = getModuleLocation((ಶೂನ್ಯ*)getModuleLocation); // using code address, doesn't work with static linkage!
    ಪ್ರತ್ಯಾಗಮ !dst.empty();
}

#ifdef _WIN32
ತಾರ್ಕಿಕ getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ಯದಿ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ಶೂನ್ಯ* addr = (ಶೂನ್ಯ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ಯದಿ (m)
    {
        wchar_t path[4096];
        ಧ್ರುವ size_t path_size = ಪರಿಮಾಣ(path)/ಪರಿಮಾಣ(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        ಯದಿ (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
#endif

cv::String findDataFile(ಧ್ರುವ cv::String& relative_path,
                        ಧ್ರುವ ವರ್ಣ* configuration_parameter,
                        ಧ್ರುವ std::vector<String>* ಹುಡುಕು_paths,
                        ಧ್ರುವ std::vector<String>* subdir_paths)
{
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ರಿಕ್ತ, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(ರಿಕ್ತ, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    ಯದಿ(f) { \
        fclose(f); \
        ಪ್ರತ್ಯಾಗಮ path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ಧ್ರುವ std::vector<cv::String>& ಹುಡುಕು_path = ಹುಡುಕು_paths ? *ಹುಡುಕು_paths : get_Data_ಹುಡುಕು_Path();
    ಪ್ರತಿ(size_t i = ಹುಡುಕು_path.size(); i > 0; i--)
    {
        ಧ್ರುವ cv::String& prefix = ಹುಡುಕು_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ಧ್ರುವ std::vector<cv::String>& ಹುಡುಕು_subdir = subdir_paths ? *subdir_paths : get_Data_ಹುಡುಕು_Sub_Directory();


    // Step 2
    ಧ್ರುವ cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ಧ್ರುವ cv::utils::Paths& ಹುಡುಕು_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    ಪ್ರತಿ (size_t k = 0; k < ಹುಡುಕು_hint.size(); k++)
    {
        cv::String datapath = ಹುಡುಕು_hint[k];
        ಯದಿ (datapath.empty())
            ಅನುವರ್ತ;
        ಯದಿ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_WARNING(ರಿಕ್ತ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ಧ್ರುವ cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    ಪ್ರತಿ (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ಯದಿ (datapath.empty())
            ಅನುವರ್ತ;
        ಯದಿ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_WARNING(ರಿಕ್ತ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ಯದಿ (!override_paths.empty())
    {
        CV_LOG_INFO(ರಿಕ್ತ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        ಪ್ರತ್ಯಾಗಮ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    ತಾರ್ಕಿಕ has_tested_build_directory = ಅಸತ್ಯ;
    ಯದಿ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ಧ್ರುವ ವರ್ಣ* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_ಹುಡುಕು_PATHS };
        ಪ್ರತಿ (size_t k = 0; k < ಪರಿಮಾಣ(build_subdirs)/ಪರಿಮಾಣ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ಯದಿ (utils::fs::isDirectory(datapath))
            {
                ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = ಸತ್ಯ;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ levels = 0; levels < 3; ++levels)
    {
        ಯದಿ (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            ಭಙ್ಗ;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    ಯದಿ (!source_dir.empty())
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        ಯದಿ (utils::fs::isDirectory(datapath))
        {
            ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    ಯದಿ (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "Detected module path: '" << module_path << '\'');
    }
    ಅನ್ಯಥಾ
    {
        CV_LOG_INFO(ರಿಕ್ತ, "Can't detect module binaries location");
    }

    ಯದಿ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ಧ್ರುವ ವರ್ಣ* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_ಹುಡುಕು_PATHS };
        ಪ್ರತಿ (size_t k = 0; k < ಪರಿಮಾಣ(build_subdirs)/ಪರಿಮಾಣ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ಯದಿ (utils::fs::isDirectory(datapath))
            {
                ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#ಯದಿ defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    ಯದಿ (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        ಯದಿ (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ಧ್ರುವ ವರ್ಣ* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        ಪ್ರತಿ (size_t k = 0; k < ಪರಿಮಾಣ(install_subdirs)/ಪರಿಮಾಣ(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ಯದಿ (utils::fs::isDirectory(datapath))
            {
                ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#ಯದಿ defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ಯದಿ (module_path.empty()) // can't determine
        module_path = install_dir;
    ಯದಿ (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        ಯದಿ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "utils::findDataFile(): trying install path: " << datapath);
            ಪ್ರತಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಧ್ರುವ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    ಪ್ರತ್ಯಾಗಮ cv::String();  // not found
#ಅನ್ಯಥಾ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(ಹುಡುಕು_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ಧ್ರುವ cv::String& relative_path, ತಾರ್ಕಿಕ required, ಧ್ರುವ ವರ್ಣ* configuration_parameter)
{
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ರಿಕ್ತ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                ರಿಕ್ತ,
                                                ರಿಕ್ತ);
    ಯದಿ (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    ಪ್ರತ್ಯಾಗಮ result;
#ಅನ್ಯಥಾ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
