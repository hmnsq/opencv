// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "../precomp.hpp"

#অন্তর্ভাব "opencv_data_config.hpp"

#অন্তর্ভাব <vector>
#অন্তর্ভাব <fstream>

#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#অন্তর্ভাব "opencv2/core/utils/logger.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.hpp"

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#অন্তর্ভাব <windows.h>
#undef small
#undef min
#undef max
#undef abs
#নোচেদ্যদি defined(__linux__)
#অন্তর্ভাব <dlfcn.h>  // requires -ldl
#নোচেদ্যদি defined(__APPLE__)
#অন্তর্ভাব <TargetConditionals.h>
#যদি TARGET_OS_MAC
#অন্তর্ভাব <dlfcn.h>
#endif
#endif

নামাকাশ cv { নামাকাশ utils {

স্থির cv::Ptr< std::vector<cv::String> > g_data_অনুসন্ধান_path;
স্থির cv::Ptr< std::vector<cv::String> > g_data_অনুসন্ধান_subdir;

স্থির std::vector<cv::String>& get_Data_অনুসন্ধান_Path()
{
    যদি (g_data_অনুসন্ধান_path.empty())
        g_data_অনুসন্ধান_path.reset(নূতন std::vector<cv::String>());
    প্রত্যাগম *(g_data_অনুসন্ধান_path.get());
}

স্থির std::vector<cv::String>& get_Data_অনুসন্ধান_Sub_Directory()
{
    যদি (g_data_অনুসন্ধান_subdir.empty())
    {
        g_data_অনুসন্ধান_subdir.reset(নূতন std::vector<cv::String>());
        g_data_অনুসন্ধান_subdir->push_back("data");
        g_data_অনুসন্ধান_subdir->push_back("");
    }
    প্রত্যাগম *(g_data_অনুসন্ধান_subdir.get());
}


CV_EXPORTS শূন্য add_Data_অনুসন্ধান_Path(ধ্রুব cv::String& path)
{
    যদি (!path.empty() && utils::fs::isDirectory(path))
        get_Data_অনুসন্ধান_Path().push_back(path);
}
CV_EXPORTS শূন্য add_Data_অনুসন্ধান_Sub_Directory(ধ্রুব cv::String& subdir)
{
    get_Data_অনুসন্ধান_Sub_Directory().push_back(subdir);
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
স্থির তার্কিক isPathSep(বর্ণ c)
{
    প্রত্যাগম c == '/' || c == '\\';
}
স্থির তার্কিক isSubDirectory_(ধ্রুব cv::String& base_path, ধ্রুব cv::String& path)
{
    size_t N = base_path.size();
    যদি (N == 0)
        প্রত্যাগম অসত্য;
    যদি (isPathSep(base_path[N - 1]))
        N--;
    যদি (path.size() < N)
        প্রত্যাগম অসত্য;
    প্রতি (size_t i = 0; i < N; i++)
    {
        যদি (path[i] == base_path[i])
            অনুবর্ত;
        যদি (isPathSep(path[i]) && isPathSep(base_path[i]))
            অনুবর্ত;
        প্রত্যাগম অসত্য;
    }
    size_t M = path.size();
    যদি (M > N)
    {
        যদি (!isPathSep(path[N]))
            প্রত্যাগম অসত্য;
    }
    প্রত্যাগম সত্য;
}

স্থির তার্কিক isSubDirectory(ধ্রুব cv::String& base_path, ধ্রুব cv::String& path)
{
    তার্কিক res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(রিক্ত, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    প্রত্যাগম res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

স্থির cv::String getModuleLocation(ধ্রুব শূন্য* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#যদি _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    যদি (m)
    {
        TCHAR path[MAX_PATH];
        ধ্রুব size_t path_size = পরিমাণ(path) / পরিমাণ(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        যদি (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            বর্ণ char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            প্রত্যাগম cv::String(char_path);
#অন্যথা
            প্রত্যাগম cv::String(path);
#endif
        }
    }
#নোচেদ্যদি defined(__linux__)
    Dl_info info;
    যদি (0 != dladdr(addr, &info))
    {
        প্রত্যাগম cv::String(info.dli_fname);
    }
#নোচেদ্যদি defined(__APPLE__)
# যদি TARGET_OS_MAC
    Dl_info info;
    যদি (0 != dladdr(addr, &info))
    {
        প্রত্যাগম cv::String(info.dli_fname);
    }
# endif
#অন্যথা
    // not supported, skip
#endif
    প্রত্যাগম cv::String();
}

তার্কিক getBinLocation(std::string& dst)
{
    dst = getModuleLocation((শূন্য*)getModuleLocation); // using code address, doesn't work with static linkage!
    প্রত্যাগম !dst.empty();
}

#ifdef _WIN32
তার্কিক getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#যদি _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    শূন্য* addr = (শূন্য*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    যদি (m)
    {
        wchar_t path[4096];
        ধ্রুব size_t path_size = পরিমাণ(path)/পরিমাণ(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        যদি (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            প্রত্যাগম সত্য;
        }
    }
    প্রত্যাগম অসত্য;
}
#endif

cv::String findDataFile(ধ্রুব cv::String& relative_path,
                        ধ্রুব বর্ণ* configuration_parameter,
                        ধ্রুব std::vector<String>* অনুসন্ধান_paths,
                        ধ্রুব std::vector<String>* subdir_paths)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(রিক্ত, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(রিক্ত, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    যদি(f) { \
        fclose(f); \
        প্রত্যাগম path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    ধ্রুব std::vector<cv::String>& অনুসন্ধান_path = অনুসন্ধান_paths ? *অনুসন্ধান_paths : get_Data_অনুসন্ধান_Path();
    প্রতি(size_t i = অনুসন্ধান_path.size(); i > 0; i--)
    {
        ধ্রুব cv::String& prefix = অনুসন্ধান_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    ধ্রুব std::vector<cv::String>& অনুসন্ধান_subdir = subdir_paths ? *subdir_paths : get_Data_অনুসন্ধান_Sub_Directory();


    // Step 2
    ধ্রুব cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ধ্রুব cv::utils::Paths& অনুসন্ধান_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    প্রতি (size_t k = 0; k < অনুসন্ধান_hint.size(); k++)
    {
        cv::String datapath = অনুসন্ধান_hint[k];
        যদি (datapath.empty())
            অনুবর্ত;
        যদি (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
            {
                ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        অন্যথা
        {
            CV_LOG_WARNING(রিক্ত, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ধ্রুব cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    প্রতি (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        যদি (datapath.empty())
            অনুবর্ত;
        যদি (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
            {
                ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        অন্যথা
        {
            CV_LOG_WARNING(রিক্ত, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    যদি (!override_paths.empty())
    {
        CV_LOG_INFO(রিক্ত, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        প্রত্যাগম cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    তার্কিক has_tested_build_directory = অসত্য;
    যদি (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ধ্রুব বর্ণ* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_অনুসন্ধান_PATHS };
        প্রতি (size_t k = 0; k < পরিমাণ(build_subdirs)/পরিমাণ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            যদি (utils::fs::isDirectory(datapath))
            {
                প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
                {
                    ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = সত্য;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    প্রতি (পূর্ণাঙ্ক levels = 0; levels < 3; ++levels)
    {
        যদি (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            ভঙ্গ;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    যদি (!source_dir.empty())
    {
        CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        যদি (utils::fs::isDirectory(datapath))
        {
            প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
            {
                ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    যদি (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(রিক্ত, "Detected module path: '" << module_path << '\'');
    }
    অন্যথা
    {
        CV_LOG_INFO(রিক্ত, "Can't detect module binaries location");
    }

    যদি (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        ধ্রুব বর্ণ* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_অনুসন্ধান_PATHS };
        প্রতি (size_t k = 0; k < পরিমাণ(build_subdirs)/পরিমাণ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            যদি (utils::fs::isDirectory(datapath))
            {
                প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
                {
                    ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#যদি defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    যদি (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        যদি (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        ধ্রুব বর্ণ* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        প্রতি (size_t k = 0; k < পরিমাণ(install_subdirs)/পরিমাণ(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            যদি (utils::fs::isDirectory(datapath))
            {
                প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
                {
                    ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            অন্যথা
            {
                CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#যদি defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    যদি (module_path.empty()) // can't determine
        module_path = install_dir;
    যদি (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        যদি (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(রিক্ত, "utils::findDataFile(): trying install path: " << datapath);
            প্রতি(size_t i = অনুসন্ধান_subdir.size(); i > 0; i--)
            {
                ধ্রুব cv::String& subdir = অনুসন্ধান_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    প্রত্যাগম cv::String();  // not found
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(অনুসন্ধান_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(ধ্রুব cv::String& relative_path, তার্কিক required, ধ্রুব বর্ণ* configuration_parameter)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(রিক্ত, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                রিক্ত,
                                                রিক্ত);
    যদি (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    প্রত্যাগম result;
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
