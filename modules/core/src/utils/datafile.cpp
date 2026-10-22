// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن "opencv_data_config.hpp"

#ضمن <vector>
#ضمن <fstream>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ضمن <windows.h>
#undef small
#undef min
#undef max
#undef abs
#وإلاإذا defined(__linux__)
#ضمن <dlfcn.h>  // requires -ldl
#وإلاإذا defined(__APPLE__)
#ضمن <TargetConditionals.h>
#אם TARGET_OS_MAC
#ضمن <dlfcn.h>
#endif
#endif

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_data_search_path;
ساكن cv::Ptr< std::vector<cv::String> > g_data_search_subdir;

ساكن std::vector<cv::String>& _getDataSearchPath()
{
    אם (g_data_search_path.empty())
        g_data_search_path.reset(جديد std::vector<cv::String>());
    החזר *(g_data_search_path.get());
}

ساكن std::vector<cv::String>& _getDataSearchSubDirectory()
{
    אם (g_data_search_subdir.empty())
    {
        g_data_search_subdir.reset(جديد std::vector<cv::String>());
        g_data_search_subdir->push_back("data");
        g_data_search_subdir->push_back("");
    }
    החזר *(g_data_search_subdir.get());
}


CV_EXPORTS فراغ addDataSearchPath(ثابت cv::String& path)
{
    אם (!path.empty() && utils::fs::isDirectory(path))
        _getDataSearchPath().push_back(path);
}
CV_EXPORTS فراغ addDataSearchSubDirectory(ثابت cv::String& subdir)
{
    _getDataSearchSubDirectory().push_back(subdir);
}

#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isPathSep(حرف c)
{
    החזר c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& base_path, ثابت cv::String& path)
{
    size_t N = base_path.size();
    אם (N == 0)
        החזר שקר;
    אם (isPathSep(base_path[N - 1]))
        N--;
    אם (path.size() < N)
        החזר שקר;
    עבור (size_t i = 0; i < N; i++)
    {
        אם (path[i] == base_path[i])
            استمر;
        אם (isPathSep(path[i]) && isPathSep(base_path[i]))
            استمر;
        החזר שקר;
    }
    size_t M = path.size();
    אם (M > N)
    {
        אם (!isPathSep(path[N]))
            החזר שקר;
    }
    החזר אמת;
}

ساكن منطقي isSubDirectory(ثابت cv::String& base_path, ثابت cv::String& path)
{
    منطقي res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    החזר res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#אם _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    אם (m)
    {
        TCHAR path[MAX_PATH];
        ثابت size_t path_size = حجم(path) / حجم(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        אם (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            حرف char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            החזר cv::String(char_path);
#אחרת
            החזר cv::String(path);
#endif
        }
    }
#وإلاإذا defined(__linux__)
    Dl_info info;
    אם (0 != dladdr(addr, &info))
    {
        החזר cv::String(info.dli_fname);
    }
#وإلاإذا defined(__APPLE__)
# אם TARGET_OS_MAC
    Dl_info info;
    אם (0 != dladdr(addr, &info))
    {
        החזר cv::String(info.dli_fname);
    }
# endif
#אחרת
    // not supported, skip
#endif
    החזר cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    החזר !dst.empty();
}

#ifdef _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#אם _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    אם (m)
    {
        wchar_t path[4096];
        ثابت size_t path_size = حجم(path)/حجم(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        אם (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            החזר אמת;
        }
    }
    החזר שקר;
}
#endif

cv::String findDataFile(ثابت cv::String& relative_path,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* search_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    אם(f) { \
        fclose(f); \
        החזר path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ثابت std::vector<cv::String>& search_path = search_paths ? *search_paths : _getDataSearchPath();
    עבור(size_t i = search_path.size(); i > 0; i--)
    {
        ثابت cv::String& prefix = search_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ثابت std::vector<cv::String>& search_subdir = subdir_paths ? *subdir_paths : _getDataSearchSubDirectory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& search_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    עבור (size_t k = 0; k < search_hint.size(); k++)
    {
        cv::String datapath = search_hint[k];
        אם (datapath.empty())
            استمر;
        אם (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            עבור(size_t i = search_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        אחרת
        {
            CV_LOG_WARNING(عدم, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ثابت cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    עבור (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        אם (datapath.empty())
            استمر;
        אם (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            עבור(size_t i = search_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        אחרת
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    אם (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        החזר cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = שקר;
    אם (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_SEARCH_PATHS };
        עבור (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            אם (utils::fs::isDirectory(datapath))
            {
                עבור(size_t i = search_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = אמת;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    עבור (صحيح levels = 0; levels < 3; ++levels)
    {
        אם (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            اكسر;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    אם (!source_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        אם (utils::fs::isDirectory(datapath))
        {
            עבור(size_t i = search_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    אם (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_path << '\'');
    }
    אחרת
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    אם (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ثابت حرف* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_SEARCH_PATHS };
        עבור (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            אם (utils::fs::isDirectory(datapath))
            {
                עבור(size_t i = search_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#אם defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    אם (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        אם (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        עבור (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            אם (utils::fs::isDirectory(datapath))
            {
                עבור(size_t i = search_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            אחרת
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#אם defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    אם (module_path.empty()) // can't determine
        module_path = install_dir;
    אם (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        אם (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            עבור(size_t i = search_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    החזר cv::String();  // not found
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(search_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ثابت cv::String& relative_path, منطقي required, ثابت حرف* configuration_parameter)
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    אם (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    החזר result;
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
