// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل "opencv_data_config.hpp"

#شامل <vector>
#شامل <fstream>

#شامل <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل "opencv2/core/utils/logger.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#شامل <windows.h>
#undef small
#undef min
#undef max
#undef abs
#وگرنهاگر defined(__linux__)
#شامل <dlfcn.h>  // requires -ldl
#وگرنهاگر defined(__APPLE__)
#شامل <TargetConditionals.h>
#اگر TARGET_OS_MAC
#شامل <dlfcn.h>
#endif
#endif

فضاینام cv { فضاینام utils {

ایستا cv::Ptr< std::vector<cv::String> > g_data_جستجو_path;
ایستا cv::Ptr< std::vector<cv::String> > g_data_جستجو_subdir;

ایستا std::vector<cv::String>& get_Data_جستجو_Path()
{
    اگر (g_data_جستجو_path.empty())
        g_data_جستجو_path.reset(جدید std::vector<cv::String>());
    بازگشت *(g_data_جستجو_path.get());
}

ایستا std::vector<cv::String>& get_Data_جستجو_Sub_Directory()
{
    اگر (g_data_جستجو_subdir.empty())
    {
        g_data_جستجو_subdir.reset(جدید std::vector<cv::String>());
        g_data_جستجو_subdir->push_back("data");
        g_data_جستجو_subdir->push_back("");
    }
    بازگشت *(g_data_جستجو_subdir.get());
}


CV_EXPORTS تهی add_Data_جستجو_Path(ثابت cv::String& path)
{
    اگر (!path.empty() && utils::fs::isDirectory(path))
        get_Data_جستجو_Path().push_back(path);
}
CV_EXPORTS تهی add_Data_جستجو_Sub_Directory(ثابت cv::String& subdir)
{
    get_Data_جستجو_Sub_Directory().push_back(subdir);
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ایستا منطقی isPathSep(نویسه c)
{
    بازگشت c == '/' || c == '\\';
}
ایستا منطقی isSubDirectory_(ثابت cv::String& base_path, ثابت cv::String& path)
{
    size_t N = base_path.size();
    اگر (N == 0)
        بازگشت نادرست;
    اگر (isPathSep(base_path[N - 1]))
        N--;
    اگر (path.size() < N)
        بازگشت نادرست;
    برای (size_t i = 0; i < N; i++)
    {
        اگر (path[i] == base_path[i])
            ادامه;
        اگر (isPathSep(path[i]) && isPathSep(base_path[i]))
            ادامه;
        بازگشت نادرست;
    }
    size_t M = path.size();
    اگر (M > N)
    {
        اگر (!isPathSep(path[N]))
            بازگشت نادرست;
    }
    بازگشت درست;
}

ایستا منطقی isSubDirectory(ثابت cv::String& base_path, ثابت cv::String& path)
{
    منطقی res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(پوچ, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    بازگشت res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

ایستا cv::String getModuleLocation(ثابت تهی* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#اگر _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    اگر (m)
    {
        TCHAR path[MAX_PATH];
        ثابت size_t path_size = اندازه(path) / اندازه(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        اگر (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            نویسه char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            بازگشت cv::String(char_path);
#وگرنه
            بازگشت cv::String(path);
#endif
        }
    }
#وگرنهاگر defined(__linux__)
    Dl_info info;
    اگر (0 != dladdr(addr, &info))
    {
        بازگشت cv::String(info.dli_fname);
    }
#وگرنهاگر defined(__APPLE__)
# اگر TARGET_OS_MAC
    Dl_info info;
    اگر (0 != dladdr(addr, &info))
    {
        بازگشت cv::String(info.dli_fname);
    }
# endif
#وگرنه
    // not supported, skip
#endif
    بازگشت cv::String();
}

منطقی getBinLocation(std::string& dst)
{
    dst = getModuleLocation((تهی*)getModuleLocation); // using code address, doesn't work with static linkage!
    بازگشت !dst.empty();
}

#ifdef _WIN32
منطقی getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#اگر _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    تهی* addr = (تهی*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    اگر (m)
    {
        wchar_t path[4096];
        ثابت size_t path_size = اندازه(path)/اندازه(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        اگر (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            بازگشت درست;
        }
    }
    بازگشت نادرست;
}
#endif

cv::String findDataFile(ثابت cv::String& relative_path,
                        ثابت نویسه* configuration_parameter,
                        ثابت std::vector<String>* جستجو_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(پوچ, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(پوچ, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    اگر(f) { \
        fclose(f); \
        بازگشت path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ثابت std::vector<cv::String>& جستجو_path = جستجو_paths ? *جستجو_paths : get_Data_جستجو_Path();
    برای(size_t i = جستجو_path.size(); i > 0; i--)
    {
        ثابت cv::String& prefix = جستجو_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ثابت std::vector<cv::String>& جستجو_subdir = subdir_paths ? *subdir_paths : get_Data_جستجو_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& جستجو_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    برای (size_t k = 0; k < جستجو_hint.size(); k++)
    {
        cv::String datapath = جستجو_hint[k];
        اگر (datapath.empty())
            ادامه;
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            برای(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        وگرنه
        {
            CV_LOG_WARNING(پوچ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ثابت cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    برای (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        اگر (datapath.empty())
            ادامه;
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            برای(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        وگرنه
        {
            CV_LOG_WARNING(پوچ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    اگر (!override_paths.empty())
    {
        CV_LOG_INFO(پوچ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        بازگشت cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقی has_tested_build_directory = نادرست;
    اگر (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(پوچ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت نویسه* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_جستجو_PATHS };
        برای (size_t k = 0; k < اندازه(build_subdirs)/اندازه(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            اگر (utils::fs::isDirectory(datapath))
            {
                برای(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = درست;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    برای (صحیح levels = 0; levels < 3; ++levels)
    {
        اگر (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            بشکن;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    اگر (!source_dir.empty())
    {
        CV_LOG_DEBUG(پوچ, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(پوچ, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        اگر (utils::fs::isDirectory(datapath))
        {
            برای(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    اگر (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(پوچ, "Detected module path: '" << module_path << '\'');
    }
    وگرنه
    {
        CV_LOG_INFO(پوچ, "Can't detect module binaries location");
    }

    اگر (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(پوچ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ثابت نویسه* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_جستجو_PATHS };
        برای (size_t k = 0; k < اندازه(build_subdirs)/اندازه(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            اگر (utils::fs::isDirectory(datapath))
            {
                برای(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#اگر defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    اگر (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        اگر (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ثابت نویسه* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        برای (size_t k = 0; k < اندازه(install_subdirs)/اندازه(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            اگر (utils::fs::isDirectory(datapath))
            {
                برای(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            وگرنه
            {
                CV_LOG_DEBUG(پوچ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#اگر defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    اگر (module_path.empty()) // can't determine
        module_path = install_dir;
    اگر (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(پوچ, "utils::findDataFile(): trying install path: " << datapath);
            برای(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    بازگشت cv::String();  // not found
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(جستجو_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ثابت cv::String& relative_path, منطقی required, ثابت نویسه* configuration_parameter)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(پوچ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                پوچ,
                                                پوچ);
    اگر (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    بازگشت result;
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
