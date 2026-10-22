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
#जइ TARGET_OS_MAC
#अन्तर्भाव <dlfcn.h>
#endif
#endif

नामाकाश cv { नामाकाश utils {

स्थिर cv::Ptr< std::vector<cv::String> > g_data_अन्वेषणम्_path;
स्थिर cv::Ptr< std::vector<cv::String> > g_data_अन्वेषणम्_subdir;

स्थिर std::vector<cv::String>& get_Data_अन्वेषणम्_Path()
{
    जइ (g_data_अन्वेषणम्_path.empty())
        g_data_अन्वेषणम्_path.reset(नूतन std::vector<cv::String>());
    प्रत्यागम *(g_data_अन्वेषणम्_path.get());
}

स्थिर std::vector<cv::String>& get_Data_अन्वेषणम्_Sub_Directory()
{
    जइ (g_data_अन्वेषणम्_subdir.empty())
    {
        g_data_अन्वेषणम्_subdir.reset(नूतन std::vector<cv::String>());
        g_data_अन्वेषणम्_subdir->push_back("data");
        g_data_अन्वेषणम्_subdir->push_back("");
    }
    प्रत्यागम *(g_data_अन्वेषणम्_subdir.get());
}


CV_EXPORTS शून्य add_Data_अन्वेषणम्_Path(ध्रुव cv::String& path)
{
    जइ (!path.empty() && utils::fs::isDirectory(path))
        get_Data_अन्वेषणम्_Path().push_back(path);
}
CV_EXPORTS शून्य add_Data_अन्वेषणम्_Sub_Directory(ध्रुव cv::String& subdir)
{
    get_Data_अन्वेषणम्_Sub_Directory().push_back(subdir);
}

#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isPathSep(वर्ण c)
{
    प्रत्यागम c == '/' || c == '\\';
}
स्थिर तार्किक isSubDirectory_(ध्रुव cv::String& base_path, ध्रुव cv::String& path)
{
    size_t N = base_path.size();
    जइ (N == 0)
        प्रत्यागम मिच्छ;
    जइ (isPathSep(base_path[N - 1]))
        N--;
    जइ (path.size() < N)
        प्रत्यागम मिच्छ;
    प्रति (size_t i = 0; i < N; i++)
    {
        जइ (path[i] == base_path[i])
            अनुवर्त;
        जइ (isPathSep(path[i]) && isPathSep(base_path[i]))
            अनुवर्त;
        प्रत्यागम मिच्छ;
    }
    size_t M = path.size();
    जइ (M > N)
    {
        जइ (!isPathSep(path[N]))
            प्रत्यागम मिच्छ;
    }
    प्रत्यागम सच्च;
}

स्थिर तार्किक isSubDirectory(ध्रुव cv::String& base_path, ध्रुव cv::String& path)
{
    तार्किक res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(रिक्त, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    प्रत्यागम res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

स्थिर cv::String getModuleLocation(ध्रुव शून्य* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#जइ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    जइ (m)
    {
        TCHAR path[MAX_PATH];
        ध्रुव size_t path_size = परिमाण(path) / परिमाण(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        जइ (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            वर्ण char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            प्रत्यागम cv::String(char_path);
#अण्णहा
            प्रत्यागम cv::String(path);
#endif
        }
    }
#नोचेद्यदि defined(__linux__)
    Dl_info info;
    जइ (0 != dladdr(addr, &info))
    {
        प्रत्यागम cv::String(info.dli_fname);
    }
#नोचेद्यदि defined(__APPLE__)
# जइ TARGET_OS_MAC
    Dl_info info;
    जइ (0 != dladdr(addr, &info))
    {
        प्रत्यागम cv::String(info.dli_fname);
    }
# endif
#अण्णहा
    // not supported, skip
#endif
    प्रत्यागम cv::String();
}

तार्किक getBinLocation(std::string& dst)
{
    dst = getModuleLocation((शून्य*)getModuleLocation); // using code address, doesn't work with static linkage!
    प्रत्यागम !dst.empty();
}

#ifdef _WIN32
तार्किक getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#जइ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    शून्य* addr = (शून्य*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    जइ (m)
    {
        wchar_t path[4096];
        ध्रुव size_t path_size = परिमाण(path)/परिमाण(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        जइ (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            प्रत्यागम सच्च;
        }
    }
    प्रत्यागम मिच्छ;
}
#endif

cv::String findDataFile(ध्रुव cv::String& relative_path,
                        ध्रुव वर्ण* configuration_parameter,
                        ध्रुव std::vector<String>* अन्वेषणम्_paths,
                        ध्रुव std::vector<String>* subdir_paths)
{
#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(रिक्त, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(रिक्त, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    जइ(f) { \
        fclose(f); \
        प्रत्यागम path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ध्रुव std::vector<cv::String>& अन्वेषणम्_path = अन्वेषणम्_paths ? *अन्वेषणम्_paths : get_Data_अन्वेषणम्_Path();
    प्रति(size_t i = अन्वेषणम्_path.size(); i > 0; i--)
    {
        ध्रुव cv::String& prefix = अन्वेषणम्_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ध्रुव std::vector<cv::String>& अन्वेषणम्_subdir = subdir_paths ? *subdir_paths : get_Data_अन्वेषणम्_Sub_Directory();


    // Step 2
    ध्रुव cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ध्रुव cv::utils::Paths& अन्वेषणम्_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    प्रति (size_t k = 0; k < अन्वेषणम्_hint.size(); k++)
    {
        cv::String datapath = अन्वेषणम्_hint[k];
        जइ (datapath.empty())
            अनुवर्त;
        जइ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अण्णहा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ध्रुव cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    प्रति (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        जइ (datapath.empty())
            अनुवर्त;
        जइ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अण्णहा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    जइ (!override_paths.empty())
    {
        CV_LOG_INFO(रिक्त, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        प्रत्यागम cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    तार्किक has_tested_build_directory = मिच्छ;
    जइ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_अन्वेषणम्_PATHS };
        प्रति (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            जइ (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
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
    प्रति (पूर्णाङ्क levels = 0; levels < 3; ++levels)
    {
        जइ (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            भङ्ग;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    जइ (!source_dir.empty())
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        जइ (utils::fs::isDirectory(datapath))
        {
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    जइ (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(रिक्त, "Detected module path: '" << module_path << '\'');
    }
    अण्णहा
    {
        CV_LOG_INFO(रिक्त, "Can't detect module binaries location");
    }

    जइ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_अन्वेषणम्_PATHS };
        प्रति (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            जइ (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#जइ defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    जइ (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        जइ (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ध्रुव वर्ण* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        प्रति (size_t k = 0; k < परिमाण(install_subdirs)/परिमाण(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            जइ (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            अण्णहा
            {
                CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#जइ defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    जइ (module_path.empty()) // can't determine
        module_path = install_dir;
    जइ (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        जइ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path: " << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    प्रत्यागम cv::String();  // not found
#अण्णहा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(अन्वेषणम्_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ध्रुव cv::String& relative_path, तार्किक required, ध्रुव वर्ण* configuration_parameter)
{
#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                रिक्त,
                                                रिक्त);
    जइ (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    प्रत्यागम result;
#अण्णहा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
