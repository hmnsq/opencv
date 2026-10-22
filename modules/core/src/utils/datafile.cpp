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
#إذا TARGET_OS_MAC
#ضمن <dlfcn.h>
#endif
#endif

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_data_بحث_path;
ساكن cv::Ptr< std::vector<cv::String> > g_data_بحث_subdir;

ساكن std::vector<cv::String>& get_Data_بحث_Path()
{
    إذا (g_data_بحث_path.empty())
        g_data_بحث_path.reset(جديد std::vector<cv::String>());
    أرجع *(g_data_بحث_path.get());
}

ساكن std::vector<cv::String>& get_Data_بحث_Sub_Directory()
{
    إذا (g_data_بحث_subdir.empty())
    {
        g_data_بحث_subdir.reset(جديد std::vector<cv::String>());
        g_data_بحث_subdir->push_back("data");
        g_data_بحث_subdir->push_back("");
    }
    أرجع *(g_data_بحث_subdir.get());
}


CV_EXPORTS فراغ add_Data_بحث_Path(ثابت cv::String& path)
{
    إذا (!path.empty() && utils::fs::isDirectory(path))
        get_Data_بحث_Path().push_back(path);
}
CV_EXPORTS فراغ add_Data_بحث_Sub_Directory(ثابت cv::String& subdir)
{
    get_Data_بحث_Sub_Directory().push_back(subdir);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isPathSep(حرف c)
{
    أرجع c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& base_path, ثابت cv::String& path)
{
    size_t N = base_path.size();
    إذا (N == 0)
        أرجع خطأ;
    إذا (isPathSep(base_path[N - 1]))
        N--;
    إذا (path.size() < N)
        أرجع خطأ;
    لكل (size_t i = 0; i < N; i++)
    {
        إذا (path[i] == base_path[i])
            استمر;
        إذا (isPathSep(path[i]) && isPathSep(base_path[i]))
            استمر;
        أرجع خطأ;
    }
    size_t M = path.size();
    إذا (M > N)
    {
        إذا (!isPathSep(path[N]))
            أرجع خطأ;
    }
    أرجع true;
}

ساكن منطقي isSubDirectory(ثابت cv::String& base_path, ثابت cv::String& path)
{
    منطقي res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    أرجع res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    إذا (m)
    {
        TCHAR path[MAX_PATH];
        ثابت size_t path_size = حجم(path) / حجم(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        إذا (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            حرف char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            أرجع cv::String(char_path);
#وإلا
            أرجع cv::String(path);
#endif
        }
    }
#وإلاإذا defined(__linux__)
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
#وإلاإذا defined(__APPLE__)
# إذا TARGET_OS_MAC
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
# endif
#وإلا
    // not supported, skip
#endif
    أرجع cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    أرجع !dst.empty();
}

#ifdef _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    إذا (m)
    {
        wchar_t path[4096];
        ثابت size_t path_size = حجم(path)/حجم(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        إذا (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            أرجع true;
        }
    }
    أرجع خطأ;
}
#endif

cv::String findDataFile(ثابت cv::String& relative_path,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* بحث_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    إذا(f) { \
        fclose(f); \
        أرجع path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ثابت std::vector<cv::String>& بحث_path = بحث_paths ? *بحث_paths : get_Data_بحث_Path();
    لكل(size_t i = بحث_path.size(); i > 0; i--)
    {
        ثابت cv::String& prefix = بحث_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ثابت std::vector<cv::String>& بحث_subdir = subdir_paths ? *subdir_paths : get_Data_بحث_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& بحث_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    لكل (size_t k = 0; k < بحث_hint.size(); k++)
    {
        cv::String datapath = بحث_hint[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            لكل(size_t i = بحث_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = بحث_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ثابت cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    لكل (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            لكل(size_t i = بحث_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = بحث_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    إذا (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        أرجع cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = خطأ;
    إذا (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_بحث_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = بحث_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = بحث_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = true;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    لكل (صحيح levels = 0; levels < 3; ++levels)
    {
        إذا (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            اكسر;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    إذا (!source_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        إذا (utils::fs::isDirectory(datapath))
        {
            لكل(size_t i = بحث_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = بحث_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    إذا (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_path << '\'');
    }
    وإلا
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    إذا (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ثابت حرف* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_بحث_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = بحث_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = بحث_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#إذا defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    إذا (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        إذا (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        لكل (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = بحث_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = بحث_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            وإلا
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#إذا defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    إذا (module_path.empty()) // can't determine
        module_path = install_dir;
    إذا (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            لكل(size_t i = بحث_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = بحث_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    أرجع cv::String();  // not found
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(بحث_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ثابت cv::String& relative_path, منطقي required, ثابت حرف* configuration_parameter)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    إذا (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    أرجع result;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
