// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "../precomp.hpp"

#അന്തര്ഭാവ "opencv_data_config.hpp"

#അന്തര്ഭാവ <vector>
#അന്തര്ഭാവ <fstream>

#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#അന്തര്ഭാവ "opencv2/core/utils/logger.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#അന്തര്ഭാവ <windows.h>
#undef small
#undef min
#undef max
#undef abs
#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
#അന്തര്ഭാവ <dlfcn.h>  // requires -ldl
#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)
#അന്തര്ഭാവ <TargetConditionals.h>
#എങ്കിൽ TARGET_OS_MAC
#അന്തര്ഭാവ <dlfcn.h>
#endif
#endif

നാമാകാശ cv { നാമാകാശ utils {

സ്ഥിതം cv::Ptr< std::vector<cv::String> > g_data_തിരയൽ_path;
സ്ഥിതം cv::Ptr< std::vector<cv::String> > g_data_തിരയൽ_subdir;

സ്ഥിതം std::vector<cv::String>& get_Data_തിരയൽ_Path()
{
    എങ്കിൽ (g_data_തിരയൽ_path.empty())
        g_data_തിരയൽ_path.reset(പുതിയ std::vector<cv::String>());
    തിരികെ *(g_data_തിരയൽ_path.get());
}

സ്ഥിതം std::vector<cv::String>& get_Data_തിരയൽ_Sub_Directory()
{
    എങ്കിൽ (g_data_തിരയൽ_subdir.empty())
    {
        g_data_തിരയൽ_subdir.reset(പുതിയ std::vector<cv::String>());
        g_data_തിരയൽ_subdir->push_back("data");
        g_data_തിരയൽ_subdir->push_back("");
    }
    തിരികെ *(g_data_തിരയൽ_subdir.get());
}


CV_EXPORTS ശൂന്യം add_Data_തിരയൽ_Path(സ്ഥിരം cv::String& path)
{
    എങ്കിൽ (!path.empty() && utils::fs::isDirectory(path))
        get_Data_തിരയൽ_Path().push_back(path);
}
CV_EXPORTS ശൂന്യം add_Data_തിരയൽ_Sub_Directory(സ്ഥിരം cv::String& subdir)
{
    get_Data_തിരയൽ_Sub_Directory().push_back(subdir);
}

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
സ്ഥിതം തർക്കം isPathSep(അക്ഷരം c)
{
    തിരികെ c == '/' || c == '\\';
}
സ്ഥിതം തർക്കം isSubDirectory_(സ്ഥിരം cv::String& base_path, സ്ഥിരം cv::String& path)
{
    size_t N = base_path.size();
    എങ്കിൽ (N == 0)
        തിരികെ അസത്യം;
    എങ്കിൽ (isPathSep(base_path[N - 1]))
        N--;
    എങ്കിൽ (path.size() < N)
        തിരികെ അസത്യം;
    വേണ്ടി (size_t i = 0; i < N; i++)
    {
        എങ്കിൽ (path[i] == base_path[i])
            തുടരുക;
        എങ്കിൽ (isPathSep(path[i]) && isPathSep(base_path[i]))
            തുടരുക;
        തിരികെ അസത്യം;
    }
    size_t M = path.size();
    എങ്കിൽ (M > N)
    {
        എങ്കിൽ (!isPathSep(path[N]))
            തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം isSubDirectory(സ്ഥിരം cv::String& base_path, സ്ഥിരം cv::String& path)
{
    തർക്കം res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(ശൂന്യ, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    തിരികെ res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

സ്ഥിതം cv::String getModuleLocation(സ്ഥിരം ശൂന്യം* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#എങ്കിൽ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    എങ്കിൽ (m)
    {
        TCHAR path[MAX_PATH];
        സ്ഥിരം size_t path_size = വലുപ്പം(path) / വലുപ്പം(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        എങ്കിൽ (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            അക്ഷരം char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            തിരികെ cv::String(char_path);
#അല്ലെങ്കിൽ
            തിരികെ cv::String(path);
#endif
        }
    }
#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
    Dl_info info;
    എങ്കിൽ (0 != dladdr(addr, &info))
    {
        തിരികെ cv::String(info.dli_fname);
    }
#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)
# എങ്കിൽ TARGET_OS_MAC
    Dl_info info;
    എങ്കിൽ (0 != dladdr(addr, &info))
    {
        തിരികെ cv::String(info.dli_fname);
    }
# endif
#അല്ലെങ്കിൽ
    // not supported, skip
#endif
    തിരികെ cv::String();
}

തർക്കം getBinLocation(std::string& dst)
{
    dst = getModuleLocation((ശൂന്യം*)getModuleLocation); // using code address, doesn't work with static linkage!
    തിരികെ !dst.empty();
}

#ifdef _WIN32
തർക്കം getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#എങ്കിൽ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ശൂന്യം* addr = (ശൂന്യം*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    എങ്കിൽ (m)
    {
        wchar_t path[4096];
        സ്ഥിരം size_t path_size = വലുപ്പം(path)/വലുപ്പം(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        എങ്കിൽ (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            തിരികെ സത്യം;
        }
    }
    തിരികെ അസത്യം;
}
#endif

cv::String findDataFile(സ്ഥിരം cv::String& relative_path,
                        സ്ഥിരം അക്ഷരം* configuration_parameter,
                        സ്ഥിരം std::vector<String>* തിരയൽ_paths,
                        സ്ഥിരം std::vector<String>* subdir_paths)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ശൂന്യ, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(ശൂന്യ, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    എങ്കിൽ(f) { \
        fclose(f); \
        തിരികെ path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    സ്ഥിരം std::vector<cv::String>& തിരയൽ_path = തിരയൽ_paths ? *തിരയൽ_paths : get_Data_തിരയൽ_Path();
    വേണ്ടി(size_t i = തിരയൽ_path.size(); i > 0; i--)
    {
        സ്ഥിരം cv::String& prefix = തിരയൽ_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    സ്ഥിരം std::vector<cv::String>& തിരയൽ_subdir = subdir_paths ? *subdir_paths : get_Data_തിരയൽ_Sub_Directory();


    // Step 2
    സ്ഥിരം cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    സ്ഥിരം cv::utils::Paths& തിരയൽ_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    വേണ്ടി (size_t k = 0; k < തിരയൽ_hint.size(); k++)
    {
        cv::String datapath = തിരയൽ_hint[k];
        എങ്കിൽ (datapath.empty())
            തുടരുക;
        എങ്കിൽ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
            {
                സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_WARNING(ശൂന്യ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    സ്ഥിരം cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    വേണ്ടി (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        എങ്കിൽ (datapath.empty())
            തുടരുക;
        എങ്കിൽ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
            {
                സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_WARNING(ശൂന്യ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    എങ്കിൽ (!override_paths.empty())
    {
        CV_LOG_INFO(ശൂന്യ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        തിരികെ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    തർക്കം has_tested_build_directory = അസത്യം;
    എങ്കിൽ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        സ്ഥിരം അക്ഷരം* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_തിരയൽ_PATHS };
        വേണ്ടി (size_t k = 0; k < വലുപ്പം(build_subdirs)/വലുപ്പം(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            എങ്കിൽ (utils::fs::isDirectory(datapath))
            {
                വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
                {
                    സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = സത്യം;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    വേണ്ടി (പൂർണ്ണസംഖ്യ levels = 0; levels < 3; ++levels)
    {
        എങ്കിൽ (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            നിർത്തുക;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    എങ്കിൽ (!source_dir.empty())
    {
        CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        എങ്കിൽ (utils::fs::isDirectory(datapath))
        {
            വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
            {
                സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    എങ്കിൽ (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(ശൂന്യ, "Detected module path: '" << module_path << '\'');
    }
    അല്ലെങ്കിൽ
    {
        CV_LOG_INFO(ശൂന്യ, "Can't detect module binaries location");
    }

    എങ്കിൽ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        സ്ഥിരം അക്ഷരം* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_തിരയൽ_PATHS };
        വേണ്ടി (size_t k = 0; k < വലുപ്പം(build_subdirs)/വലുപ്പം(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            എങ്കിൽ (utils::fs::isDirectory(datapath))
            {
                വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
                {
                    സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#എങ്കിൽ defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    എങ്കിൽ (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        എങ്കിൽ (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        സ്ഥിരം അക്ഷരം* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        വേണ്ടി (size_t k = 0; k < വലുപ്പം(install_subdirs)/വലുപ്പം(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            എങ്കിൽ (utils::fs::isDirectory(datapath))
            {
                വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
                {
                    സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#എങ്കിൽ defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    എങ്കിൽ (module_path.empty()) // can't determine
        module_path = install_dir;
    എങ്കിൽ (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        എങ്കിൽ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ശൂന്യ, "utils::findDataFile(): trying install path: " << datapath);
            വേണ്ടി(size_t i = തിരയൽ_subdir.size(); i > 0; i--)
            {
                സ്ഥിരം cv::String& subdir = തിരയൽ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    തിരികെ cv::String();  // not found
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(തിരയൽ_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(സ്ഥിരം cv::String& relative_path, തർക്കം required, സ്ഥിരം അക്ഷരം* configuration_parameter)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ശൂന്യ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                ശൂന്യ,
                                                ശൂന്യ);
    എങ്കിൽ (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    തിരികെ result;
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
