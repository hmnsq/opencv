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
#किंवाजर defined(__linux__)
#अन्तर्भाव <dlfcn.h>  // requires -ldl
#किंवाजर defined(__APPLE__)
#अन्तर्भाव <TargetConditionals.h>
#जर TARGET_OS_MAC
#अन्तर्भाव <dlfcn.h>
#endif
#endif

नामाकाश cv { नामाकाश utils {

स्थिर cv::Ptr< std::vector<cv::String> > g_data_शोध_path;
स्थिर cv::Ptr< std::vector<cv::String> > g_data_शोध_subdir;

स्थिर std::vector<cv::String>& get_Data_शोध_Path()
{
    जर (g_data_शोध_path.empty())
        g_data_शोध_path.reset(नवीन std::vector<cv::String>());
    परत *(g_data_शोध_path.get());
}

स्थिर std::vector<cv::String>& get_Data_शोध_Sub_Directory()
{
    जर (g_data_शोध_subdir.empty())
    {
        g_data_शोध_subdir.reset(नवीन std::vector<cv::String>());
        g_data_शोध_subdir->push_back("data");
        g_data_शोध_subdir->push_back("");
    }
    परत *(g_data_शोध_subdir.get());
}


CV_EXPORTS शून्य add_Data_शोध_Path(स्थिरांक cv::String& path)
{
    जर (!path.empty() && utils::fs::isDirectory(path))
        get_Data_शोध_Path().push_back(path);
}
CV_EXPORTS शून्य add_Data_शोध_Sub_Directory(स्थिरांक cv::String& subdir)
{
    get_Data_शोध_Sub_Directory().push_back(subdir);
}

#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isPathSep(अक्षर c)
{
    परत c == '/' || c == '\\';
}
स्थिर तार्किक isSubDirectory_(स्थिरांक cv::String& base_path, स्थिरांक cv::String& path)
{
    माप_प्रकार N = base_path.size();
    जर (N == 0)
        परत असत्य;
    जर (isPathSep(base_path[N - 1]))
        N--;
    जर (path.size() < N)
        परत असत्य;
    साठी (माप_प्रकार i = 0; i < N; i++)
    {
        जर (path[i] == base_path[i])
            सुरूठेवा;
        जर (isPathSep(path[i]) && isPathSep(base_path[i]))
            सुरूठेवा;
        परत असत्य;
    }
    माप_प्रकार M = path.size();
    जर (M > N)
    {
        जर (!isPathSep(path[N]))
            परत असत्य;
    }
    परत सत्य;
}

स्थिर तार्किक isSubDirectory(स्थिरांक cv::String& base_path, स्थिरांक cv::String& path)
{
    तार्किक res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(रिक्त, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    परत res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

स्थिर cv::String getModuleLocation(स्थिरांक शून्य* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#जर _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    जर (m)
    {
        TCHAR path[MAX_PATH];
        स्थिरांक माप_प्रकार path_size = आकार(path) / आकार(*path);
        माप_प्रकार sz = GetModuleFileName(m, path, path_size);
        जर (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            अक्षर char_path[MAX_PATH];
            माप_प्रकार copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
            परत cv::String(char_path);
#अन्यथा
            परत cv::String(path);
#endif
        }
    }
#किंवाजर defined(__linux__)
    Dl_info info;
    जर (0 != dladdr(addr, &info))
    {
        परत cv::String(info.dli_fname);
    }
#किंवाजर defined(__APPLE__)
# जर TARGET_OS_MAC
    Dl_info info;
    जर (0 != dladdr(addr, &info))
    {
        परत cv::String(info.dli_fname);
    }
# endif
#अन्यथा
    // not supported, skip
#endif
    परत cv::String();
}

तार्किक getBinLocation(std::string& dst)
{
    dst = getModuleLocation((शून्य*)getModuleLocation); // using code address, doesn't work with static linkage!
    परत !dst.empty();
}

#ifdef _WIN32
तार्किक getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#जर _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    शून्य* addr = (शून्य*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    जर (m)
    {
        ब_अक्षर_प्रकार path[4096];
        स्थिरांक माप_प्रकार path_size = आकार(path)/आकार(*path);
        माप_प्रकार sz = GetModuleFileNameW(m, path, path_size);
        जर (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            परत सत्य;
        }
    }
    परत असत्य;
}
#endif

cv::String findDataFile(स्थिरांक cv::String& relative_path,
                        स्थिरांक अक्षर* configuration_parameter,
                        स्थिरांक std::vector<String>* शोध_paths,
                        स्थिरांक std::vector<String>* subdir_paths)
{
#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(रिक्त, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(रिक्त, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    खाता* f = ख_खोलो(path.c_str(), "rb"); \
    जर(f) { \
        ख_बंद(f); \
        परत path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    स्थिरांक std::vector<cv::String>& शोध_path = शोध_paths ? *शोध_paths : get_Data_शोध_Path();
    साठी(माप_प्रकार i = शोध_path.size(); i > 0; i--)
    {
        स्थिरांक cv::String& prefix = शोध_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    स्थिरांक std::vector<cv::String>& शोध_subdir = subdir_paths ? *subdir_paths : get_Data_शोध_Sub_Directory();


    // Step 2
    स्थिरांक cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    स्थिरांक cv::utils::Paths& शोध_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    साठी (माप_प्रकार k = 0; k < शोध_hint.size(); k++)
    {
        cv::String datapath = शोध_hint[k];
        जर (datapath.empty())
            सुरूठेवा;
        जर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
            {
                स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
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
    स्थिरांक cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    साठी (माप_प्रकार k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        जर (datapath.empty())
            सुरूठेवा;
        जर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
            {
                स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    जर (!override_paths.empty())
    {
        CV_LOG_INFO(रिक्त, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        परत cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    तार्किक has_tested_build_directory = असत्य;
    जर (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        स्थिरांक अक्षर* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_शोध_PATHS };
        साठी (माप_प्रकार k = 0; k < आकार(build_subdirs)/आकार(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            जर (utils::fs::isDirectory(datapath))
            {
                साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
                {
                    स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = सत्य;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    साठी (पूर्णांक levels = 0; levels < 3; ++levels)
    {
        जर (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            थांबा;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    जर (!source_dir.empty())
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        जर (utils::fs::isDirectory(datapath))
        {
            साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
            {
                स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    जर (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(रिक्त, "Detected module path: '" << module_path << '\'');
    }
    अन्यथा
    {
        CV_LOG_INFO(रिक्त, "Can't detect module binaries location");
    }

    जर (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        स्थिरांक अक्षर* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_शोध_PATHS };
        साठी (माप_प्रकार k = 0; k < आकार(build_subdirs)/आकार(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            जर (utils::fs::isDirectory(datapath))
            {
                साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
                {
                    स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#जर defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    जर (!module_path.empty())  // require module path
    {
        माप_प्रकार pos = module_path.rfind('/');
        जर (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        स्थिरांक अक्षर* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        साठी (माप_प्रकार k = 0; k < आकार(install_subdirs)/आकार(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            जर (utils::fs::isDirectory(datapath))
            {
                साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
                {
                    स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
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

#जर defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    जर (module_path.empty()) // can't determine
        module_path = install_dir;
    जर (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        जर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path: " << datapath);
            साठी(माप_प्रकार i = शोध_subdir.size(); i > 0; i--)
            {
                स्थिरांक cv::String& subdir = शोध_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    परत cv::String();  // not found
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(शोध_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(स्थिरांक cv::String& relative_path, तार्किक required, स्थिरांक अक्षर* configuration_parameter)
{
#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                रिक्त,
                                                रिक्त);
    जर (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    परत result;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
