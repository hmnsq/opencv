// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव "opencv_data_config.hpp"

#अन्तर्भाव <vector>
#अन्तर्भाव <fstream>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव "opencv2/core/utils/logger.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#अन्तर्भाव <windows.h>
#undef small
#undef min
#undef max
#undef abs
#नोचेद्यदि defined(__linux__)
#अन्तर्भाव <dlfcn.h>  // requires -ldl
#नोचेद्यदि defined(__APPLE__)
#अन्तर्भाव <TargetConditionals.h>
#सचे TARGET_OS_MAC
#अन्तर्भाव <dlfcn.h>
#endif
#endif

नामाकाश cv { नामाकाश utils {

स्थिर cv::Ptr< std::vector<cv::String> > g_data_अन्वेषणम्_path;
स्थिर cv::Ptr< std::vector<cv::String> > g_data_अन्वेषणम्_subdir;

स्थिर std::vector<cv::String>& get_Data_अन्वेषणम्_Path()
{
    सचे (g_data_अन्वेषणम्_path.empty())
        g_data_अन्वेषणम्_path.reset(नूतन std::vector<cv::String>());
    पटिनिवत्त *(g_data_अन्वेषणम्_path.get());
}

स्थिर std::vector<cv::String>& get_Data_अन्वेषणम्_Sub_Directory()
{
    सचे (g_data_अन्वेषणम्_subdir.empty())
    {
        g_data_अन्वेषणम्_subdir.reset(नूतन std::vector<cv::String>());
        g_data_अन्वेषणम्_subdir->push_back("data");
        g_data_अन्वेषणम्_subdir->push_back("");
    }
    पटिनिवत्त *(g_data_अन्वेषणम्_subdir.get());
}


CV_EXPORTS शून्य add_Data_अन्वेषणम्_Path(ध्रुव cv::String& path)
{
    सचे (!path.empty() && utils::fs::isDirectory(path))
        get_Data_अन्वेषणम्_Path().push_back(path);
}
CV_EXPORTS शून्य add_Data_अन्वेषणम्_Sub_Directory(ध्रुव cv::String& subdir)
{
    get_Data_अन्वेषणम्_Sub_Directory().push_back(subdir);
}

#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isPathSep(वर्ण c)
{
    पटिनिवत्त c == '/' || c == '\\';
}
स्थिर तार्किक isSubDirectory_(ध्रुव cv::String& base_path, ध्रुव cv::String& path)
{
    size_t N = base_path.size();
    सचे (N == 0)
        पटिनिवत्त मिच्छा;
    सचे (isPathSep(base_path[N - 1]))
        N--;
    सचे (path.size() < N)
        पटिनिवत्त मिच्छा;
    अत्थाय (size_t i = 0; i < N; i++)
    {
        सचे (path[i] == base_path[i])
            अनुवर्त;
        सचे (isPathSep(path[i]) && isPathSep(base_path[i]))
            अनुवर्त;
        पटिनिवत्त मिच्छा;
    }
    size_t M = path.size();
    सचे (M > N)
    {
        सचे (!isPathSep(path[N]))
            पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
}

स्थिर तार्किक isSubDirectory(ध्रुव cv::String& base_path, ध्रुव cv::String& path)
{
    तार्किक res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(रिक्त, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    पटिनिवत्त res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

स्थिर cv::String getModuleLocation(ध्रुव शून्य* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#सचे _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    सचे (m)
    {
        TCHAR path[MAX_PATH];
        ध्रुव size_t path_size = परिमाण(path) / परिमाण(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        सचे (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            वर्ण char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            पटिनिवत्त cv::String(char_path);
#अञ्ञथा
            पटिनिवत्त cv::String(path);
#endif
        }
    }
#नोचेद्यदि defined(__linux__)
    Dl_info info;
    सचे (0 != dladdr(addr, &info))
    {
        पटिनिवत्त cv::String(info.dli_fname);
    }
#नोचेद्यदि defined(__APPLE__)
# सचे TARGET_OS_MAC
    Dl_info info;
    सचे (0 != dladdr(addr, &info))
    {
        पटिनिवत्त cv::String(info.dli_fname);
    }
# endif
#अञ्ञथा
    // not supported, skip
#endif
    पटिनिवत्त cv::String();
}

तार्किक getBinLocation(std::string& dst)
{
    dst = getModuleLocation((शून्य*)getModuleLocation); // using code address, doesn't work with static linkage!
    पटिनिवत्त !dst.empty();
}

#ifdef _WIN32
तार्किक getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#सचे _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    शून्य* addr = (शून्य*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    सचे (m)
    {
        wchar_t path[4096];
        ध्रुव size_t path_size = परिमाण(path)/परिमाण(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        सचे (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            पटिनिवत्त सच्च;
        }
    }
    पटिनिवत्त मिच्छा;
}
#endif

cv::String findDataFile(ध्रुव cv::String& relative_path,
                        ध्रुव वर्ण* configuration_parameter,
                        ध्रुव std::vector<String>* अन्वेषणम्_paths,
                        ध्रुव std::vector<String>* subdir_paths)
{
#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(रिक्त, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(रिक्त, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    सचे(f) { \
        fclose(f); \
        पटिनिवत्त path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ध्रुव std::vector<cv::String>& अन्वेषणम्_path = अन्वेषणम्_paths ? *अन्वेषणम्_paths : get_Data_अन्वेषणम्_Path();
    अत्थाय(size_t i = अन्वेषणम्_path.size(); i > 0; i--)
    {
        ध्रुव cv::String& prefix = अन्वेषणम्_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ध्रुव std::vector<cv::String>& अन्वेषणम्_subdir = subdir_paths ? *subdir_paths : get_Data_अन्वेषणम्_Sub_Directory();


    // Step 2
    ध्रुव cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ध्रुव cv::utils::Paths& अन्वेषणम्_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    अत्थाय (size_t k = 0; k < अन्वेषणम्_hint.size(); k++)
    {
        cv::String datapath = अन्वेषणम्_hint[k];
        सचे (datapath.empty())
            अनुवर्त;
        सचे (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अञ्ञथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ध्रुव cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    अत्थाय (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        सचे (datapath.empty())
            अनुवर्त;
        सचे (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अञ्ञथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    सचे (!override_paths.empty())
    {
        CV_LOG_INFO(रिक्त, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        पटिनिवत्त cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    तार्किक has_tested_build_directory = मिच्छा;
    सचे (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_अन्वेषणम्_PATHS };
        अत्थाय (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            सचे (utils::fs::isDirectory(datapath))
            {
                अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = सच्च;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    अत्थाय (पूर्णाङ्क levels = 0; levels < 3; ++levels)
    {
        सचे (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            भङ्ग;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    सचे (!source_dir.empty())
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        सचे (utils::fs::isDirectory(datapath))
        {
            अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    सचे (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(रिक्त, "Detected module path: '" << module_path << '\'');
    }
    अञ्ञथा
    {
        CV_LOG_INFO(रिक्त, "Can't detect module binaries location");
    }

    सचे (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_अन्वेषणम्_PATHS };
        अत्थाय (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            सचे (utils::fs::isDirectory(datapath))
            {
                अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#सचे defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    सचे (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        सचे (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ध्रुव वर्ण* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        अत्थाय (size_t k = 0; k < परिमाण(install_subdirs)/परिमाण(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            सचे (utils::fs::isDirectory(datapath))
            {
                अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            अञ्ञथा
            {
                CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#सचे defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    सचे (module_path.empty()) // can't determine
        module_path = install_dir;
    सचे (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        सचे (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path: " << datapath);
            अत्थाय(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    पटिनिवत्त cv::String();  // not found
#अञ्ञथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(अन्वेषणम्_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ध्रुव cv::String& relative_path, तार्किक required, ध्रुव वर्ण* configuration_parameter)
{
#सचे OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                रिक्त,
                                                रिक्त);
    सचे (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    पटिनिवत्त result;
#अञ्ञथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
