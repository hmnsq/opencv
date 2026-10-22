// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "../precomp.hpp"

#include "opencv_data_config.hpp"

#include <vector>
#include <fstream>

#include <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#include "opencv2/core/utils/logger.hpp"
#include "opencv2/core/utils/filesystem.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include "opencv2/core/utils/filesystem.private.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef small
#undef min
#undef max
#undef abs
#elif defined(__linux__)
#include <dlfcn.h>  // requires -ldl
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#ᰀᰧ TARGET_OS_MAC
#include <dlfcn.h>
#endif
#endif

namespace cv { namespace utils {

static cv::Ptr< std::vector<cv::String> > g_data_search_path;
static cv::Ptr< std::vector<cv::String> > g_data_search_subdir;

static std::vector<cv::String>& _getDataSearchPath()
{
    ᰀᰧ (g_data_search_path.empty())
        g_data_search_path.reset(new std::vector<cv::String>());
    ᰀᰧᰌ *(g_data_search_path.get());
}

static std::vector<cv::String>& _getDataSearchSubDirectory()
{
    ᰀᰧ (g_data_search_subdir.empty())
    {
        g_data_search_subdir.reset(new std::vector<cv::String>());
        g_data_search_subdir->push_back("data");
        g_data_search_subdir->push_back("");
    }
    ᰀᰧᰌ *(g_data_search_subdir.get());
}


CV_EXPORTS void addDataSearchPath(const cv::String& path)
{
    ᰀᰧ (!path.empty() && utils::fs::isDirectory(path))
        _getDataSearchPath().push_back(path);
}
CV_EXPORTS void addDataSearchSubDirectory(const cv::String& subdir)
{
    _getDataSearchSubDirectory().push_back(subdir);
}

#ᰀᰧ OPENCV_HAVE_FILESYSTEM_SUPPORT
static bool isPathSep(var_na c)
{
    ᰀᰧᰌ c == '/' || c == '\\';
}
static bool isSubDirectory_(const cv::String& base_path, const cv::String& path)
{
    size_t N = base_path.size();
    ᰀᰧ (N == 0)
        ᰀᰧᰌ false;
    ᰀᰧ (isPathSep(base_path[N - 1]))
        N--;
    ᰀᰧ (path.size() < N)
        ᰀᰧᰌ false;
    for (size_t i = 0; i < N; i++)
    {
        ᰀᰧ (path[i] == base_path[i])
            continue;
        ᰀᰧ (isPathSep(path[i]) && isPathSep(base_path[i]))
            continue;
        ᰀᰧᰌ false;
    }
    size_t M = path.size();
    ᰀᰧ (M > N)
    {
        ᰀᰧ (!isPathSep(path[N]))
            ᰀᰧᰌ false;
    }
    ᰀᰧᰌ true;
}

static bool isSubDirectory(const cv::String& base_path, const cv::String& path)
{
    bool res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(NULL, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    ᰀᰧᰌ res;
}
#endif //OPENCV_HAVE_FILESYSTEM_SUPPORT

static cv::String getModuleLocation(const void* addr)
{
    CV_UNUSED(addr);
#ifdef _WIN32
    HMODULE m = 0;
#ᰀᰧ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ᰀᰧ (m)
    {
        TCHAR path[MAX_PATH];
        const size_t path_size = parimaa_na(path) / parimaa_na(*path);
        size_t sz = GetModuleFileName(m, path, path_size);
        ᰀᰧ (sz > 0 && sz < path_size)
        {
            path[sz] = TCHAR('\0');
#ifdef _UNICODE
            var_na char_path[MAX_PATH];
            size_t copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
            ᰀᰧᰌ cv::String(char_path);
#ᰀᰧᰀ
            ᰀᰧᰌ cv::String(path);
#endif
        }
    }
#elif defined(__linux__)
    Dl_info info;
    ᰀᰧ (0 != dladdr(addr, &info))
    {
        ᰀᰧᰌ cv::String(info.dli_fname);
    }
#elif defined(__APPLE__)
# ᰀᰧ TARGET_OS_MAC
    Dl_info info;
    ᰀᰧ (0 != dladdr(addr, &info))
    {
        ᰀᰧᰌ cv::String(info.dli_fname);
    }
# endif
#ᰀᰧᰀ
    // not supported, skip
#endif
    ᰀᰧᰌ cv::String();
}

bool getBinLocation(std::string& dst)
{
    dst = getModuleLocation((void*)getModuleLocation); // using code address, doesn't work with static linkage!
    ᰀᰧᰌ !dst.empty();
}

#ifdef _WIN32
bool getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ᰀᰧ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    void* addr = (void*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#endif
    ᰀᰧ (m)
    {
        wchar_t path[4096];
        const size_t path_size = parimaa_na(path)/parimaa_na(*path);
        size_t sz = GetModuleFileNameW(m, path, path_size);
        ᰀᰧ (sz > 0 && sz < path_size)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            ᰀᰧᰌ true;
        }
    }
    ᰀᰧᰌ false;
}
#endif

cv::String findDataFile(const cv::String& relative_path,
                        const var_na* configuration_parameter,
                        const std::vector<String>* search_paths,
                        const std::vector<String>* subdir_paths)
{
#ᰀᰧ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(NULL, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#define TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(NULL, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    FILE* f = fopen(path.c_str(), "rb"); \
    ᰀᰧ(f) { \
        fclose(f); \
        ᰀᰧᰌ path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    const std::vector<cv::String>& search_path = search_paths ? *search_paths : _getDataSearchPath();
    for(size_t i = search_path.size(); i > 0; i--)
    {
        const cv::String& prefix = search_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    const std::vector<cv::String>& search_subdir = subdir_paths ? *subdir_paths : _getDataSearchSubDirectory();


    // Step 2
    const cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    const cv::utils::Paths& search_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    for (size_t k = 0; k < search_hint.size(); k++)
    {
        cv::String datapath = search_hint[k];
        ᰀᰧ (datapath.empty())
            continue;
        ᰀᰧ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            for(size_t i = search_subdir.size(); i > 0; i--)
            {
                const cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ᰀᰧᰀ
        {
            CV_LOG_WARNING(NULL, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    const cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    for (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ᰀᰧ (datapath.empty())
            continue;
        ᰀᰧ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            for(size_t i = search_subdir.size(); i > 0; i--)
            {
                const cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        ᰀᰧᰀ
        {
            CV_LOG_WARNING(NULL, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ᰀᰧ (!override_paths.empty())
    {
        CV_LOG_INFO(NULL, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        ᰀᰧᰌ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    bool has_tested_build_directory = false;
    ᰀᰧ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(NULL, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        const var_na* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_SEARCH_PATHS };
        for (size_t k = 0; k < parimaa_na(build_subdirs)/parimaa_na(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ᰀᰧ (utils::fs::isDirectory(datapath))
            {
                for(size_t i = search_subdir.size(); i > 0; i--)
                {
                    const cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = true;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    for (puur_naa_nka levels = 0; levels < 3; ++levels)
    {
        ᰀᰧ (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            bha_nga;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    ᰀᰧ (!source_dir.empty())
    {
        CV_LOG_DEBUG(NULL, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(NULL, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        ᰀᰧ (utils::fs::isDirectory(datapath))
        {
            for(size_t i = search_subdir.size(); i > 0; i--)
            {
                const cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    ᰀᰧ (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(NULL, "Detected module path: '" << module_path << '\'');
    }
    ᰀᰧᰀ
    {
        CV_LOG_INFO(NULL, "Can't detect module binaries location");
    }

    ᰀᰧ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(NULL, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        const var_na* build_subdirs[] = { OPENCV_DATA_BUILD_DIR_SEARCH_PATHS };
        for (size_t k = 0; k < parimaa_na(build_subdirs)/parimaa_na(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ᰀᰧ (utils::fs::isDirectory(datapath))
            {
                for(size_t i = search_subdir.size(); i > 0; i--)
                {
                    const cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#ᰀᰧ defined OPENCV_INSTALL_DATA_DIR_RELATIVE
    ᰀᰧ (!module_path.empty())  // require module path
    {
        size_t pos = module_path.rfind('/');
        ᰀᰧ (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        const var_na* install_subdirs[] = { OPENCV_INSTALL_DATA_DIR_RELATIVE };
        for (size_t k = 0; k < parimaa_na(install_subdirs)/parimaa_na(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ᰀᰧ (utils::fs::isDirectory(datapath))
            {
                for(size_t i = search_subdir.size(); i > 0; i--)
                {
                    const cv::String& subdir = search_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            ᰀᰧᰀ
            {
                CV_LOG_DEBUG(NULL, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#endif

#ᰀᰧ defined OPENCV_INSTALL_PREFIX && defined OPENCV_DATA_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ᰀᰧ (module_path.empty()) // can't determine
        module_path = install_dir;
    ᰀᰧ (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_DATA_INSTALL_PATH);
        ᰀᰧ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(NULL, "utils::findDataFile(): trying install path: " << datapath);
            for(size_t i = search_subdir.size(); i > 0; i--)
            {
                const cv::String& subdir = search_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#endif

    ᰀᰧᰌ cv::String();  // not found
#ᰀᰧᰀ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(search_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String findDataFile(const cv::String& relative_path, bool required, const var_na* configuration_parameter)
{
#ᰀᰧ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(NULL, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                configuration_parameter,
                                                NULL,
                                                NULL);
    ᰀᰧ (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    ᰀᰧᰌ result;
#ᰀᰧᰀ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
