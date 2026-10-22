// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "../precomp.hpp"

#અન્તર્ભાવ "opencv_data_config.hpp"

#અન્તર્ભાવ <vector>
#અન્તર્ભાવ <fstream>

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#અન્તર્ભાવ "opencv2/core/utils/logger.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.hpp"

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#અન્તર્ભાવ <windows.h>
#undef small
#undef min
#undef max
#undef abs
#અથવાજો defined(__linux__)
#અન્તર્ભાવ <dlfcn.h>  // requires -ldl
#અથવાજો defined(__APPLE__)
#અન્તર્ભાવ <TargetConditionals.h>
#જો TARGET_OS_MAC
#અન્તર્ભાવ <dlfcn.h>
#endif
#endif

નામાકાશ cv { નામાકાશ utils {

સ્થિર cv::Ptr< std::vector<cv::String> > g_data_શોધ_path;
સ્થિર cv::Ptr< std::vector<cv::String> > g_data_શોધ_subdir;

સ્થિર std::vector<cv::String>& get_Data_શોધ_Path()
{
    જો (g_data_શોધ_path.empty())
        g_data_શોધ_path.reset(નવું std::vector<cv::String>());
    પાછા *(g_data_શોધ_path.get());
}

સ્થિર std::vector<cv::String>& get_Data_શોધ_Sub_Directory()
{
    જો (g_data_શોધ_subdir.empty())
    {
        g_data_શોધ_subdir.reset(નવું std::vector<cv::String>());
        g_data_શોધ_subdir->push_back("data");
        g_data_શોધ_subdir->push_back("");
    }
    પાછા *(g_data_શોધ_subdir.get());
}


CV_EXPORTS શૂન્ય add_Data_શોધ_Path(અચલ cv::String& path)
{
    જો (!path.empty() && utils::fs::isDirectory(path))
        get_Data_શોધ_Path().push_back(path);
}
CV_EXPORTS શૂન્ય add_Data_શોધ_Sub_Directory(અચલ cv::String& subdir)
{
    get_Data_શોધ_Sub_Directory().push_back(subdir);
}

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
સ્થિર તાર્કિક isPathSep(અક્ષર c)
{
    પાછા c == '/' || c == '\\';
}
સ્થિર તાર્કિક isSubDirectory_(અચલ cv::String& base_path, અચલ cv::String& path)
{
    size_t N = base_path.size();
    જો (N == 0)
        પાછા ખોટું;
    જો (isPathSep(base_path[N - 1]))
        N--;
    જો (path.size() < N)
        પાછા ખોટું;
    માટે (size_t i = 0; i < N; i++)
    {
        જો (path[i] == base_path[i])
            ચાલુ;
        જો (isPathSep(path[i]) && isPathSep(base_path[i]))
            ચાલુ;
        પાછા ખોટું;
    }
    size_t M = path.size();
    જો (M > N)
    {
        જો (!isPathSep(path[N]))
            પાછા ખોટું;
    }
    પાછા સાચું;
}

સ્થિર તાર્કિક isSubDirectory(અચલ cv::String& base_path, અચલ cv::String& path)
{
    તાર્કિક res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(ખાલી, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    પાછા res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

સ્થિર cv::String getModuleLocation(અચલ શૂન્ય* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#જો _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    જો (m)
    {
        TCHAR path[MAX_PATH];
        અચલ size_t path_size = કદ(path) / કદ(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        જો (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            અક્ષર char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            પાછા cv::String(char_path);
#નહીંતર
            પાછા cv::String(path);
#endif
        }
    }
#અથવાજો defined(__linux__)
    Dl_info info;
    જો (0 != dladdr(addr, &info))
    {
        પાછા cv::String(info.dli_fname);
    }
#અથવાજો defined(__APPLE__)
# જો TARGET_OS_MAC
    Dl_info info;
    જો (0 != dladdr(addr, &info))
    {
        પાછા cv::String(info.dli_fname);
    }
# endif
#નહીંતર
    // not supported, skip
#endif
    પાછા cv::String();
}

તાર્કિક getBinLocation(std::string& dst)
{
    dst = getModuleLocation((શૂન્ય*)getModuleLocation); // using code address, doesn't work with static linkage!
    પાછા !dst.empty();
}

#ifdef _WIN32
તાર્કિક getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#જો _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    શૂન્ય* addr = (શૂન્ય*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    જો (m)
    {
        wchar_t path[4096];
        અચલ size_t path_size = કદ(path)/કદ(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        જો (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            પાછા સાચું;
        }
    }
    પાછા ખોટું;
}
#endif

cv::String findDataFile(અચલ cv::String& relative_path,
                        અચલ અક્ષર* configuration_parameter,
                        અચલ std::vector<String>* શોધ_paths,
                        અચલ std::vector<String>* subdir_paths)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ખાલી, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(ખાલી, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    જો(f) { \
        fclose(f); \
        પાછા path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    અચલ std::vector<cv::String>& શોધ_path = શોધ_paths ? *શોધ_paths : get_Data_શોધ_Path();
    માટે(size_t i = શોધ_path.size(); i > 0; i--)
    {
        અચલ cv::String& prefix = શોધ_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    અચલ std::vector<cv::String>& શોધ_subdir = subdir_paths ? *subdir_paths : get_Data_શોધ_Sub_Directory();


    // Step 2
    અચલ cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    અચલ cv::utils::Paths& શોધ_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    માટે (size_t k = 0; k < શોધ_hint.size(); k++)
    {
        cv::String datapath = શોધ_hint[k];
        જો (datapath.empty())
            ચાલુ;
        જો (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
            {
                અચલ cv::String& subdir = શોધ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        નહીંતર
        {
            CV_LOG_WARNING(ખાલી, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    અચલ cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    માટે (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        જો (datapath.empty())
            ચાલુ;
        જો (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
            {
                અચલ cv::String& subdir = શોધ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        નહીંતર
        {
            CV_LOG_WARNING(ખાલી, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    જો (!override_paths.empty())
    {
        CV_LOG_INFO(ખાલી, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        પાછા cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    તાર્કિક has_tested_build_directory = ખોટું;
    જો (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        અચલ અક્ષર* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_શોધ_PATHS };
        માટે (size_t k = 0; k < કદ(build_subdirs)/કદ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            જો (utils::fs::isDirectory(datapath))
            {
                માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
                {
                    અચલ cv::String& subdir = શોધ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = સાચું;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    માટે (પૂર્ણાંક levels = 0; levels < 3; ++levels)
    {
        જો (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            તોડો;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    જો (!source_dir.empty())
    {
        CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        જો (utils::fs::isDirectory(datapath))
        {
            માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
            {
                અચલ cv::String& subdir = શોધ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    જો (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(ખાલી, "Detected module path: '" << module_path << '\'');
    }
    નહીંતર
    {
        CV_LOG_INFO(ખાલી, "Can't detect module binaries location");
    }

    જો (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        અચલ અક્ષર* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_શોધ_PATHS };
        માટે (size_t k = 0; k < કદ(build_subdirs)/કદ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            જો (utils::fs::isDirectory(datapath))
            {
                માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
                {
                    અચલ cv::String& subdir = શોધ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#જો defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    જો (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        જો (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        અચલ અક્ષર* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        માટે (size_t k = 0; k < કદ(install_subdirs)/કદ(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            જો (utils::fs::isDirectory(datapath))
            {
                માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
                {
                    અચલ cv::String& subdir = શોધ_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            નહીંતર
            {
                CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#જો defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    જો (module_path.empty()) // can't determine
        module_path = install_dir;
    જો (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        જો (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ખાલી, "utils::findDataFile(): trying install path: " << datapath);
            માટે(size_t i = શોધ_subdir.size(); i > 0; i--)
            {
                અચલ cv::String& subdir = શોધ_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    પાછા cv::String();  // not found
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(શોધ_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(અચલ cv::String& relative_path, તાર્કિક required, અચલ અક્ષર* configuration_parameter)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ખાલી, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                ખાલી,
                                                ખાલી);
    જો (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    પાછા result;
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
