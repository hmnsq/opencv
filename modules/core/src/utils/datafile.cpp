// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "../precomp.hpp"

#அந்தர்பாவ "opencv_data_config.hpp"

#அந்தர்பாவ <vector>
#அந்தர்பாவ <fstream>

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#அந்தர்பாவ "opencv2/core/utils/logger.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.hpp"

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#அந்தர்பாவ <windows.h>
#undef small
#undef min
#undef max
#undef abs
#அல்லதெனில் defined(__linux__)
#அந்தர்பாவ <dlfcn.h>  // requires -ldl
#அல்லதெனில் defined(__APPLE__)
#அந்தர்பாவ <TargetConditionals.h>
#எனில் TARGET_OS_MAC
#அந்தர்பாவ <dlfcn.h>
#endif
#endif

நாமாகாஶ cv { நாமாகாஶ utils {

நிலையான cv::Ptr< std::vector<cv::String> > g_data_தேடல்_path;
நிலையான cv::Ptr< std::vector<cv::String> > g_data_தேடல்_subdir;

நிலையான std::vector<cv::String>& get_Data_தேடல்_Path()
{
    எனில் (g_data_தேடல்_path.empty())
        g_data_தேடல்_path.reset(புதிய std::vector<cv::String>());
    திரும்பு *(g_data_தேடல்_path.get());
}

நிலையான std::vector<cv::String>& get_Data_தேடல்_Sub_Directory()
{
    எனில் (g_data_தேடல்_subdir.empty())
    {
        g_data_தேடல்_subdir.reset(புதிய std::vector<cv::String>());
        g_data_தேடல்_subdir->push_back("data");
        g_data_தேடல்_subdir->push_back("");
    }
    திரும்பு *(g_data_தேடல்_subdir.get());
}


CV_EXPORTS வெறுமை add_Data_தேடல்_Path(மாறிலி cv::String& path)
{
    எனில் (!path.empty() && utils::fs::isDirectory(path))
        get_Data_தேடல்_Path().push_back(path);
}
CV_EXPORTS வெறுமை add_Data_தேடல்_Sub_Directory(மாறிலி cv::String& subdir)
{
    get_Data_தேடல்_Sub_Directory().push_back(subdir);
}

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
நிலையான தருக்க isPathSep(எழுத்து c)
{
    திரும்பு c == '/' || c == '\\';
}
நிலையான தருக்க isSubDirectory_(மாறிலி cv::String& base_path, மாறிலி cv::String& path)
{
    size_t N = base_path.size();
    எனில் (N == 0)
        திரும்பு பொய்;
    எனில் (isPathSep(base_path[N - 1]))
        N--;
    எனில் (path.size() < N)
        திரும்பு பொய்;
    க்கு (size_t i = 0; i < N; i++)
    {
        எனில் (path[i] == base_path[i])
            தொடர்;
        எனில் (isPathSep(path[i]) && isPathSep(base_path[i]))
            தொடர்;
        திரும்பு பொய்;
    }
    size_t M = path.size();
    எனில் (M > N)
    {
        எனில் (!isPathSep(path[N]))
            திரும்பு பொய்;
    }
    திரும்பு உண்மை;
}

நிலையான தருக்க isSubDirectory(மாறிலி cv::String& base_path, மாறிலி cv::String& path)
{
    தருக்க res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(வெற்று, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    திரும்பு res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

நிலையான cv::String getModuleLocation(மாறிலி வெறுமை* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#எனில் _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    எனில் (m)
    {
        TCHAR path[MAX_PATH];
        மாறிலி size_t path_size = அளவு(path) / அளவு(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        எனில் (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            எழுத்து char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            திரும்பு cv::String(char_path);
#இல்லையெனில்
            திரும்பு cv::String(path);
#endif
        }
    }
#அல்லதெனில் defined(__linux__)
    Dl_info info;
    எனில் (0 != dladdr(addr, &info))
    {
        திரும்பு cv::String(info.dli_fname);
    }
#அல்லதெனில் defined(__APPLE__)
# எனில் TARGET_OS_MAC
    Dl_info info;
    எனில் (0 != dladdr(addr, &info))
    {
        திரும்பு cv::String(info.dli_fname);
    }
# endif
#இல்லையெனில்
    // not supported, skip
#endif
    திரும்பு cv::String();
}

தருக்க getBinLocation(std::string& dst)
{
    dst = getModuleLocation((வெறுமை*)getModuleLocation); // using code address, doesn't work with static linkage!
    திரும்பு !dst.empty();
}

#ifdef _WIN32
தருக்க getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#எனில் _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    வெறுமை* addr = (வெறுமை*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    எனில் (m)
    {
        wchar_t path[4096];
        மாறிலி size_t path_size = அளவு(path)/அளவு(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        எனில் (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            திரும்பு உண்மை;
        }
    }
    திரும்பு பொய்;
}
#endif

cv::String findDataFile(மாறிலி cv::String& relative_path,
                        மாறிலி எழுத்து* configuration_parameter,
                        மாறிலி std::vector<String>* தேடல்_paths,
                        மாறிலி std::vector<String>* subdir_paths)
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(வெற்று, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(வெற்று, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    எனில்(f) { \
        fclose(f); \
        திரும்பு path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    மாறிலி std::vector<cv::String>& தேடல்_path = தேடல்_paths ? *தேடல்_paths : get_Data_தேடல்_Path();
    க்கு(size_t i = தேடல்_path.size(); i > 0; i--)
    {
        மாறிலி cv::String& prefix = தேடல்_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    மாறிலி std::vector<cv::String>& தேடல்_subdir = subdir_paths ? *subdir_paths : get_Data_தேடல்_Sub_Directory();


    // Step 2
    மாறிலி cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    மாறிலி cv::utils::Paths& தேடல்_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    க்கு (size_t k = 0; k < தேடல்_hint.size(); k++)
    {
        cv::String datapath = தேடல்_hint[k];
        எனில் (datapath.empty())
            தொடர்;
        எனில் (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
            {
                மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        இல்லையெனில்
        {
            CV_LOG_WARNING(வெற்று, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    மாறிலி cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    க்கு (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        எனில் (datapath.empty())
            தொடர்;
        எனில் (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
            {
                மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        இல்லையெனில்
        {
            CV_LOG_WARNING(வெற்று, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    எனில் (!override_paths.empty())
    {
        CV_LOG_INFO(வெற்று, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        திரும்பு cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    தருக்க has_tested_build_directory = பொய்;
    எனில் (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        மாறிலி எழுத்து* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_தேடல்_PATHS };
        க்கு (size_t k = 0; k < அளவு(build_subdirs)/அளவு(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            எனில் (utils::fs::isDirectory(datapath))
            {
                க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
                {
                    மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = உண்மை;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    க்கு (முழு levels = 0; levels < 3; ++levels)
    {
        எனில் (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            நிறுத்து;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    எனில் (!source_dir.empty())
    {
        CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        எனில் (utils::fs::isDirectory(datapath))
        {
            க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
            {
                மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    எனில் (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(வெற்று, "Detected module path: '" << module_path << '\'');
    }
    இல்லையெனில்
    {
        CV_LOG_INFO(வெற்று, "Can't detect module binaries location");
    }

    எனில் (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        மாறிலி எழுத்து* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_தேடல்_PATHS };
        க்கு (size_t k = 0; k < அளவு(build_subdirs)/அளவு(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            எனில் (utils::fs::isDirectory(datapath))
            {
                க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
                {
                    மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#எனில் defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    எனில் (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        எனில் (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        மாறிலி எழுத்து* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        க்கு (size_t k = 0; k < அளவு(install_subdirs)/அளவு(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            எனில் (utils::fs::isDirectory(datapath))
            {
                க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
                {
                    மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            இல்லையெனில்
            {
                CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#எனில் defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    எனில் (module_path.empty()) // can't determine
        module_path = install_dir;
    எனில் (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        எனில் (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(வெற்று, "utils::findDataFile(): trying install path: " << datapath);
            க்கு(size_t i = தேடல்_subdir.size(); i > 0; i--)
            {
                மாறிலி cv::String& subdir = தேடல்_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    திரும்பு cv::String();  // not found
#இல்லையெனில் // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(தேடல்_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(மாறிலி cv::String& relative_path, தருக்க required, மாறிலி எழுத்து* configuration_parameter)
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(வெற்று, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                வெற்று,
                                                வெற்று);
    எனில் (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    திரும்பு result;
#இல்லையெனில் // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
