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
#undef असल
#अथवायदि defined(__linux__)
#अन्तर्भाव <dlfcn.h>  // requires -ldl
#अथवायदि defined(__APPLE__)
#अन्तर्भाव <TargetConditionals.h>
#यदि TARGET_OS_MAC
#अन्तर्भाव <dlfcn.h>
#endif
#endif

नामाकाश cv { नामाकाश utils {

स्थिर cv::Ptr< std::vector<cv::String> > g_data_खोज_path;
स्थिर cv::Ptr< std::vector<cv::String> > g_data_खोज_subdir;

स्थिर std::vector<cv::String>& get_Data_खोज_Path()
{
    यदि (g_data_खोज_path.empty())
        g_data_खोज_path.reset(नूतन std::vector<cv::String>());
    फर्काऊ *(g_data_खोज_path.get());
}

स्थिर std::vector<cv::String>& get_Data_खोज_Sub_Directory()
{
    यदि (g_data_खोज_subdir.empty())
    {
        g_data_खोज_subdir.reset(नूतन std::vector<cv::String>());
        g_data_खोज_subdir->push_back("data");
        g_data_खोज_subdir->push_back("");
    }
    फर्काऊ *(g_data_खोज_subdir.get());
}


CV_EXPORTS शून्य add_Data_खोज_Path(अचर cv::String& path)
{
    यदि (!path.empty() && utils::fs::isDirectory(path))
        get_Data_खोज_Path().push_back(path);
}
CV_EXPORTS शून्य add_Data_खोज_Sub_Directory(अचर cv::String& subdir)
{
    get_Data_खोज_Sub_Directory().push_back(subdir);
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isPathSep(वर्ण c)
{
    फर्काऊ c == '/' || c == '\\';
}
स्थिर तार्किक isSubDirectory_(अचर cv::String& base_path, अचर cv::String& path)
{
    माप_प्रकार N = base_path.size();
    यदि (N == 0)
        फर्काऊ असत्य;
    यदि (isPathSep(base_path[N - 1]))
        N--;
    यदि (path.size() < N)
        फर्काऊ असत्य;
    लागि (माप_प्रकार i = 0; i < N; i++)
    {
        यदि (path[i] == base_path[i])
            जारी;
        यदि (isPathSep(path[i]) && isPathSep(base_path[i]))
            जारी;
        फर्काऊ असत्य;
    }
    माप_प्रकार M = path.size();
    यदि (M > N)
    {
        यदि (!isPathSep(path[N]))
            फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
}

स्थिर तार्किक isSubDirectory(अचर cv::String& base_path, अचर cv::String& path)
{
    तार्किक res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(रिक्त, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    फर्काऊ res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

स्थिर cv::String getModuleLocation(अचर शून्य* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#यदि _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    यदि (m)
    {
        TCHAR path[MAX_PATH];
        अचर माप_प्रकार path_size = परिमाण(path) / परिमाण(*path);
        माप_प्रकार sz = GetModuleFileName(m, path, path_size);
        यदि (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            वर्ण char_path[MAX_PATH];
            माप_प्रकार copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
            फर्काऊ cv::String(char_path);
#अन्यथा
            फर्काऊ cv::String(path);
#endif
        }
    }
#अथवायदि defined(__linux__)
    Dl_info info;
    यदि (0 != dladdr(addr, &info))
    {
        फर्काऊ cv::String(info.dli_fname);
    }
#अथवायदि defined(__APPLE__)
# यदि TARGET_OS_MAC
    Dl_info info;
    यदि (0 != dladdr(addr, &info))
    {
        फर्काऊ cv::String(info.dli_fname);
    }
# endif
#अन्यथा
    // not supported, skip
#endif
    फर्काऊ cv::String();
}

तार्किक getBinLocation(std::string& dst)
{
    dst = getModuleLocation((शून्य*)getModuleLocation); // using code address, doesn't work with static linkage!
    फर्काऊ !dst.empty();
}

#ifdef _WIN32
तार्किक getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#यदि _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    शून्य* addr = (शून्य*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    यदि (m)
    {
        ब_अक्षर_प्रकार path[4096];
        अचर माप_प्रकार path_size = परिमाण(path)/परिमाण(*path);
        माप_प्रकार sz = GetModuleFileNameW(m, path, path_size);
        यदि (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            फर्काऊ सत्य;
        }
    }
    फर्काऊ असत्य;
}
#endif

cv::String findDataFile(अचर cv::String& relative_path,
                        अचर वर्ण* configuration_parameter,
                        अचर std::vector<String>* खोज_paths,
                        अचर std::vector<String>* subdir_paths)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(रिक्त, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(रिक्त, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    खाता* f = ख_खोलो(path.c_str(), "rb"); \
    यदि(f) { \
        ख_बंद(f); \
        फर्काऊ path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    अचर std::vector<cv::String>& खोज_path = खोज_paths ? *खोज_paths : get_Data_खोज_Path();
    लागि(माप_प्रकार i = खोज_path.size(); i > 0; i--)
    {
        अचर cv::String& prefix = खोज_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    अचर std::vector<cv::String>& खोज_subdir = subdir_paths ? *subdir_paths : get_Data_खोज_Sub_Directory();


    // Step 2
    अचर cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    अचर cv::utils::Paths& खोज_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    लागि (माप_प्रकार k = 0; k < खोज_hint.size(); k++)
    {
        cv::String datapath = खोज_hint[k];
        यदि (datapath.empty())
            जारी;
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                अचर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    अचर cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    लागि (माप_प्रकार k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        यदि (datapath.empty())
            जारी;
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                अचर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    यदि (!override_paths.empty())
    {
        CV_LOG_INFO(रिक्त, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        फर्काऊ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    तार्किक has_tested_build_directory = असत्य;
    यदि (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        अचर वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_खोज_PATHS };
        लागि (माप_प्रकार k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            यदि (utils::fs::isDirectory(datapath))
            {
                लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    अचर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = सत्य;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    लागि (पूर्णाङ्क levels = 0; levels < 3; ++levels)
    {
        यदि (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            रोक;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    यदि (!source_dir.empty())
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        यदि (utils::fs::isDirectory(datapath))
        {
            लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                अचर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    यदि (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(रिक्त, "Detected module path: '" << module_path << '\'');
    }
    अन्यथा
    {
        CV_LOG_INFO(रिक्त, "Can't detect module binaries location");
    }

    यदि (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        अचर वर्ण* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_खोज_PATHS };
        लागि (माप_प्रकार k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            यदि (utils::fs::isDirectory(datapath))
            {
                लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    अचर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#यदि defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    यदि (!module_path.empty())  // require module path
    {
        माप_प्रकार pos = module_path.rfind('/');
        यदि (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        अचर वर्ण* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        लागि (माप_प्रकार k = 0; k < परिमाण(install_subdirs)/परिमाण(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            यदि (utils::fs::isDirectory(datapath))
            {
                लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    अचर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            अन्यथा
            {
                CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#यदि defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    यदि (module_path.empty()) // can't determine
        module_path = install_dir;
    यदि (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path: " << datapath);
            लागि(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                अचर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    फर्काऊ cv::String();  // not found
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(खोज_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(अचर cv::String& relative_path, तार्किक required, अचर वर्ण* configuration_parameter)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                रिक्त,
                                                रिक्त);
    यदि (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    फर्काऊ result;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
