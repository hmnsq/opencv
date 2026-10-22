// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv_data_config.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <vector>
#ਅਨ੍ਤਰ੍ਭਾਵ <fstream>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_LOG_STRIP_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/logger.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIN32_LEAN_AND_ਔਸਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ small
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ min
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ max
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਨਿਰਪੇਖ
#ਜਾਂਜੇ defined(__linux__)
#ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>  // requires -ldl
#ਜਾਂਜੇ defined(__APPLE__)
#ਅਨ੍ਤਰ੍ਭਾਵ <TargetConditionals.h>
#ਜੇ ਨਿਸ਼ਾਨਾ_OS_MAC
#ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ utils {

ਸਥਿਰ cv::Ptr< std::vector<cv::String> > g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ;
ਸਥਿਰ cv::Ptr< std::vector<cv::String> > g_ਅੰਕੜੇ_ਖੋਜ_subdir;

ਸਥਿਰ std::vector<cv::String>& get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ()
{
    ਜੇ (g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.empty())
        g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.reset(ਨਵਾਂ std::vector<cv::String>());
    ਵਾਪਸ *(g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.get());
}

ਸਥਿਰ std::vector<cv::String>& get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory()
{
    ਜੇ (g_ਅੰਕੜੇ_ਖੋਜ_subdir.empty())
    {
        g_ਅੰਕੜੇ_ਖੋਜ_subdir.reset(ਨਵਾਂ std::vector<cv::String>());
        g_ਅੰਕੜੇ_ਖੋਜ_subdir->push_back("data");
        g_ਅੰਕੜੇ_ਖੋਜ_subdir->push_back("");
    }
    ਵਾਪਸ *(g_ਅੰਕੜੇ_ਖੋਜ_subdir.get());
}


CV_EXPORTS ਖਾਲੀ ਜੋੜੋ_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ(ਅਚਲ cv::String& ਰਸਤਾ)
{
    ਜੇ (!ਰਸਤਾ.empty() && utils::fs::isDirectory(ਰਸਤਾ))
        get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ().push_back(ਰਸਤਾ);
}
CV_EXPORTS ਖਾਲੀ ਜੋੜੋ_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory(ਅਚਲ cv::String& subdir)
{
    get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory().push_back(subdir);
}

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
ਸਥਿਰ ਤਾਰਕਿਕ is_ਰਸਤਾ_Sep(ਅੱਖਰ c)
{
    ਵਾਪਸ c == '/' || c == '\\';
}
ਸਥਿਰ ਤਾਰਕਿਕ isSubDirectory_(ਅਚਲ cv::String& ਅਧਾਰ_ਰਸਤਾ, ਅਚਲ cv::String& ਰਸਤਾ)
{
    size_t N = ਅਧਾਰ_ਰਸਤਾ.size();
    ਜੇ (N == 0)
        ਵਾਪਸ ਝੂਠ;
    ਜੇ (is_ਰਸਤਾ_Sep(ਅਧਾਰ_ਰਸਤਾ[N - 1]))
        N--;
    ਜੇ (ਰਸਤਾ.size() < N)
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (size_t i = 0; i < N; i++)
    {
        ਜੇ (ਰਸਤਾ[i] == ਅਧਾਰ_ਰਸਤਾ[i])
            ਜਾਰੀ;
        ਜੇ (is_ਰਸਤਾ_Sep(ਰਸਤਾ[i]) && is_ਰਸਤਾ_Sep(ਅਧਾਰ_ਰਸਤਾ[i]))
            ਜਾਰੀ;
        ਵਾਪਸ ਝੂਠ;
    }
    size_t M = ਰਸਤਾ.size();
    ਜੇ (M > N)
    {
        ਜੇ (!is_ਰਸਤਾ_Sep(ਰਸਤਾ[N]))
            ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ isSubDirectory(ਅਚਲ cv::String& ਅਧਾਰ_ਰਸਤਾ, ਅਚਲ cv::String& ਰਸਤਾ)
{
    ਤਾਰਕਿਕ res = isSubDirectory_(ਅਧਾਰ_ਰਸਤਾ, ਰਸਤਾ);
    CV_LOG_VERBOSE(ਨਿਰਾ, 0, "isSubDirectory(): base: " << ਅਧਾਰ_ਰਸਤਾ << "  path: " << ਰਸਤਾ << "  => result: " << (res ? "TRUE" : "FALSE"));
    ਵਾਪਸ res;
}
#ਜੇ_ਅੰਤ //OPENCV_HAVE_FILESYSTEM_SUPPORT

ਸਥਿਰ cv::String getModuleLocation(ਅਚਲ ਖਾਲੀ* addr)
{
    CV_ਅਣਵਰਤਿਆ(addr);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    HMODULE m = 0;
#ਜੇ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ਪਤਾ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#ਜੇ_ਅੰਤ
    ਜੇ (m)
    {
        TCHAR ਰਸਤਾ[MAX_ਰਸਤਾ];
        ਅਚਲ size_t ਰਸਤਾ_ਮਾਪ = ਆਕਾਰ(ਰਸਤਾ) / ਆਕਾਰ(*ਰਸਤਾ);
        size_t sz = GetModuleFileName(m, ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ);
        ਜੇ (sz > 0 && sz < ਰਸਤਾ_ਮਾਪ)
        {
            ਰਸਤਾ[sz] = TCHAR('\0');
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _UNICODE
            ਅੱਖਰ char_ਰਸਤਾ[MAX_ਰਸਤਾ];
            size_t copied = wcstombs(char_ਰਸਤਾ, ਰਸਤਾ, MAX_ਰਸਤਾ);
            CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
            ਵਾਪਸ cv::String(char_ਰਸਤਾ);
#ਨਹੀਂਤਾਂ
            ਵਾਪਸ cv::String(ਰਸਤਾ);
#ਜੇ_ਅੰਤ
        }
    }
#ਜਾਂਜੇ defined(__linux__)
    Dl_info info;
    ਜੇ (0 != dladdr(addr, &info))
    {
        ਵਾਪਸ cv::String(info.dli_fname);
    }
#ਜਾਂਜੇ defined(__APPLE__)
# ਜੇ ਨਿਸ਼ਾਨਾ_OS_MAC
    Dl_info info;
    ਜੇ (0 != dladdr(addr, &info))
    {
        ਵਾਪਸ cv::String(info.dli_fname);
    }
# endif
#ਨਹੀਂਤਾਂ
    // not supported, skip
#ਜੇ_ਅੰਤ
    ਵਾਪਸ cv::String();
}

ਤਾਰਕਿਕ getBinLocation(std::string& dst)
{
    dst = getModuleLocation((ਖਾਲੀ*)getModuleLocation); // using code address, doesn't work with static linkage!
    ਵਾਪਸ !dst.empty();
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
ਤਾਰਕਿਕ getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ਜੇ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ਖਾਲੀ* addr = (ਖਾਲੀ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ਪਤਾ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#ਜੇ_ਅੰਤ
    ਜੇ (m)
    {
        wchar_t ਰਸਤਾ[4096];
        ਅਚਲ size_t ਰਸਤਾ_ਮਾਪ = ਆਕਾਰ(ਰਸਤਾ)/ਆਕਾਰ(*ਰਸਤਾ);
        size_t sz = GetModuleFileNameW(m, ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ);
        ਜੇ (sz > 0 && sz < ਰਸਤਾ_ਮਾਪ)
        {
            ਰਸਤਾ[sz] = '\0';
            dst.assign(ਰਸਤਾ, sz);
            ਵਾਪਸ ਸੱਚ;
        }
    }
    ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

cv::String ਲੱਭੋ_ਅੰਕੜੇ_File(ਅਚਲ cv::String& relative_ਰਸਤਾ,
                        ਅਚਲ ਅੱਖਰ* configuration_parameter,
                        ਅਚਲ std::vector<String>* ਖੋਜ_paths,
                        ਅਚਲ std::vector<String>* subdir_paths)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ਨਿਰਾ, cv::format("utils::findDataFile('%s', %s)", relative_ਰਸਤਾ.c_str(), configuration_parameter));

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ) \
{ \
    cv::String ਰਸਤਾ = utils::fs::join(ਅਗੇਤਰ, relative_ਰਸਤਾ); \
    CV_LOG_DEBUG(ਨਿਰਾ, cv::format("... Line %d: trying open '%s'", __LINE__, ਰਸਤਾ.c_str())); \
    FILE* f = fopen(ਰਸਤਾ.c_str(), "rb"); \
    ਜੇ(f) { \
        fclose(f); \
        ਵਾਪਸ ਰਸਤਾ; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_ਅਗੇਤਰ("");


    // Step 1
    ਅਚਲ std::vector<cv::String>& ਖੋਜ_ਰਸਤਾ = ਖੋਜ_paths ? *ਖੋਜ_paths : get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ();
    ਲਈ(size_t i = ਖੋਜ_ਰਸਤਾ.size(); i > 0; i--)
    {
        ਅਚਲ cv::String& ਅਗੇਤਰ = ਖੋਜ_ਰਸਤਾ[i - 1];
        TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
    }

    ਅਚਲ std::vector<cv::String>& ਖੋਜ_subdir = subdir_paths ? *subdir_paths : get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory();


    // Step 2
    ਅਚਲ cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ਅਚਲ cv::utils::Paths& ਖੋਜ_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    ਲਈ (size_t k = 0; k < ਖੋਜ_hint.size(); k++)
    {
        cv::String datapath = ਖੋਜ_hint[k];
        ਜੇ (datapath.empty())
            ਜਾਰੀ;
        ਜੇ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
            {
                ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
            }
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_WARNING(ਨਿਰਾ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ਅਚਲ cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    ਲਈ (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ਜੇ (datapath.empty())
            ਜਾਰੀ;
        ਜੇ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
            {
                ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
            }
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_WARNING(ਨਿਰਾ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ਜੇ (!override_paths.empty())
    {
        CV_LOG_INFO(ਨਿਰਾ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_ਰਸਤਾ);
        ਵਾਪਸ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    ਤਾਰਕਿਕ has_tested_build_directory = ਝੂਠ;
    ਜੇ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ਅਚਲ ਅੱਖਰ* build_subdirs[] = { OPENCV_ਅੰਕੜੇ_BUILD_DIR_ਖੋਜ_PATHS };
        ਲਈ (size_t k = 0; k < ਆਕਾਰ(build_subdirs)/ਆਕਾਰ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ਜੇ (utils::fs::isDirectory(datapath))
            {
                ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
                {
                    ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                    cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
                }
            }
        }
        has_tested_build_directory = ਸੱਚ;
    }

    cv::String ਸਰੋਤ_dir;
    cv::String try_ਸਰੋਤ_dir = cwd;
    ਲਈ (ਪੂਰਨ ਪੱਧਰ = 0; ਪੱਧਰ < 3; ++ਪੱਧਰ)
    {
        ਜੇ (utils::fs::exists(utils::fs::join(try_ਸਰੋਤ_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            ਸਰੋਤ_dir = try_ਸਰੋਤ_dir;
            ਰੋਕੋ;
        }
        try_ਸਰੋਤ_dir = utils::fs::join(try_ਸਰੋਤ_dir, "/..");
    }
    ਜੇ (!ਸਰੋਤ_dir.empty())
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): the current directory is source sub-directory: " << ਸਰੋਤ_dir);
        CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): <source>" << ਸਰੋਤ_dir);
        cv::String datapath = ਸਰੋਤ_dir;
        ਜੇ (utils::fs::isDirectory(datapath))
        {
            ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
            {
                ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
            }
        }
    }

    cv::String module_ਰਸਤਾ;
    ਜੇ (getBinLocation(module_ਰਸਤਾ))
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "Detected module path: '" << module_ਰਸਤਾ << '\'');
    }
    ਨਹੀਂਤਾਂ
    {
        CV_LOG_INFO(ਨਿਰਾ, "Can't detect module binaries location");
    }

    ਜੇ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_ਰਸਤਾ) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_ਰਸਤਾ)))
    )
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_ਰਸਤਾ);
        ਅਚਲ ਅੱਖਰ* build_subdirs[] = { OPENCV_ਅੰਕੜੇ_BUILD_DIR_ਖੋਜ_PATHS };
        ਲਈ (size_t k = 0; k < ਆਕਾਰ(build_subdirs)/ਆਕਾਰ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ਜੇ (utils::fs::isDirectory(datapath))
            {
                ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
                {
                    ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                    cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
                }
            }
        }
    }

#ਜੇ defined OPENCV_INSTALL_ਅੰਕੜੇ_DIR_RELATIVE
    ਜੇ (!module_ਰਸਤਾ.empty())  // require module path
    {
        size_t pos = module_ਰਸਤਾ.rfind('/');
        ਜੇ (pos == cv::String::npos)
            pos = module_ਰਸਤਾ.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_ਰਸਤਾ : module_ਰਸਤਾ.substr(0, pos);
        ਅਚਲ ਅੱਖਰ* install_subdirs[] = { OPENCV_INSTALL_ਅੰਕੜੇ_DIR_RELATIVE };
        ਲਈ (size_t k = 0; k < ਆਕਾਰ(install_subdirs)/ਆਕਾਰ(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ਜੇ (utils::fs::isDirectory(datapath))
            {
                ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
                {
                    ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                    cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
                }
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#ਜੇ_ਅੰਤ

#ਜੇ defined OPENCV_INSTALL_ਅਗੇਤਰ && defined OPENCV_ਅੰਕੜੇ_INSTALL_ਰਸਤਾ
    cv::String install_dir(OPENCV_INSTALL_ਅਗੇਤਰ);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ਜੇ (module_ਰਸਤਾ.empty()) // can't determine
        module_ਰਸਤਾ = install_dir;
    ਜੇ (isSubDirectory(install_dir, module_ਰਸਤਾ) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_ਰਸਤਾ)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_ਅੰਕੜੇ_INSTALL_ਰਸਤਾ);
        ਜੇ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "utils::findDataFile(): trying install path: " << datapath);
            ਲਈ(size_t i = ਖੋਜ_subdir.size(); i > 0; i--)
            {
                ਅਚਲ cv::String& subdir = ਖੋਜ_subdir[i - 1];
                cv::String ਅਗੇਤਰ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ਅਗੇਤਰ(ਅਗੇਤਰ);
            }
        }
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ cv::String();  // not found
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(relative_ਰਸਤਾ);
    CV_ਅਣਵਰਤਿਆ(configuration_parameter);
    CV_ਅਣਵਰਤਿਆ(ਖੋਜ_paths);
    CV_ਅਣਵਰਤਿਆ(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String ਲੱਭੋ_ਅੰਕੜੇ_File(ਅਚਲ cv::String& relative_ਰਸਤਾ, ਤਾਰਕਿਕ required, ਅਚਲ ਅੱਖਰ* configuration_parameter)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ਨਿਰਾ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_ਰਸਤਾ.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String ਨਤੀਜਾ = cv::utils::ਲੱਭੋ_ਅੰਕੜੇ_File(relative_ਰਸਤਾ,
                                                configuration_parameter,
                                                ਨਿਰਾ,
                                                ਨਿਰਾ);
    ਜੇ (ਨਤੀਜਾ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_ਰਸਤਾ.c_str()));
    ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(relative_ਰਸਤਾ);
    CV_ਅਣਵਰਤਿਆ(required);
    CV_ਅਣਵਰਤਿਆ(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
