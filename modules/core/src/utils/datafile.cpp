// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ "opencv_data_config.hpp"

#ಅನ್ತರ್ಭಾವ <vector>
#ಅನ್ತರ್ಭಾವ <fstream>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_LOG_STRIP_LEVEL
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/logger.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿಸು WIN32_LEAN_AND_ಸರಾಸರಿ
#ಅನ್ತರ್ಭಾವ <windows.h>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು small
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು min
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು max
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ನಿರಪೇಕ್ಷ
#ಅಥವಾಆದರೆ defined(__linux__)
#ಅನ್ತರ್ಭಾವ <dlfcn.h>  // requires -ldl
#ಅಥವಾಆದರೆ defined(__APPLE__)
#ಅನ್ತರ್ಭಾವ <TargetConditionals.h>
#ಆದರೆ ಗುರಿ_OS_MAC
#ಅನ್ತರ್ಭಾವ <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils {

ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ;
ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir;

ಸ್ಥಿರ std::vector<cv::String>& get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ()
{
    ಆದರೆ (g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.empty())
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.reset(ಹೊಸ std::vector<cv::String>());
    ಹಿಂತಿರುಗಿ *(g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.get());
}

ಸ್ಥಿರ std::vector<cv::String>& get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory()
{
    ಆದರೆ (g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.empty())
    {
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.reset(ಹೊಸ std::vector<cv::String>());
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir->push_back("data");
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir->push_back("");
    }
    ಹಿಂತಿರುಗಿ *(g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.get());
}


CV_EXPORTS ಶೂನ್ಯ ಕೂಡಿಸು_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    ಆದರೆ (!ಮಾರ್ಗ.empty() && utils::fs::isDirectory(ಮಾರ್ಗ))
        get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ().push_back(ಮಾರ್ಗ);
}
CV_EXPORTS ಶೂನ್ಯ ಕೂಡಿಸು_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory(ಸ್ಥಿರಾಂಕ cv::String& subdir)
{
    get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory().push_back(subdir);
}

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
ಸ್ಥಿರ ತಾರ್ಕಿಕ is_ಮಾರ್ಗ_Sep(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ c == '/' || c == '\\';
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ isSubDirectory_(ಸ್ಥಿರಾಂಕ cv::String& ತಳ_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    size_t N = ತಳ_ಮಾರ್ಗ.size();
    ಆದರೆ (N == 0)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ (is_ಮಾರ್ಗ_Sep(ತಳ_ಮಾರ್ಗ[N - 1]))
        N--;
    ಆದರೆ (ಮಾರ್ಗ.size() < N)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (size_t i = 0; i < N; i++)
    {
        ಆದರೆ (ಮಾರ್ಗ[i] == ತಳ_ಮಾರ್ಗ[i])
            ಮುಂದುವರಿ;
        ಆದರೆ (is_ಮಾರ್ಗ_Sep(ಮಾರ್ಗ[i]) && is_ಮಾರ್ಗ_Sep(ತಳ_ಮಾರ್ಗ[i]))
            ಮುಂದುವರಿ;
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    size_t M = ಮಾರ್ಗ.size();
    ಆದರೆ (M > N)
    {
        ಆದರೆ (!is_ಮಾರ್ಗ_Sep(ಮಾರ್ಗ[N]))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ isSubDirectory(ಸ್ಥಿರಾಂಕ cv::String& ತಳ_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    ತಾರ್ಕಿಕ res = isSubDirectory_(ತಳ_ಮಾರ್ಗ, ಮಾರ್ಗ);
    CV_LOG_VERBOSE(ಖಾಲಿ, 0, "isSubDirectory(): base: " << ತಳ_ಮಾರ್ಗ << "  path: " << ಮಾರ್ಗ << "  => result: " << (res ? "TRUE" : "FALSE"));
    ಹಿಂತಿರುಗಿ res;
}
#ಆದರೆ_ಅಂತ್ಯ //OPENCV_HAVE_FILESYSTEM_SUPPORT

ಸ್ಥಿರ cv::String getModuleLocation(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* addr)
{
    CV_ಬಳಸದ(addr);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    HMODULE m = 0;
#ಆದರೆ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ವಿಳಾಸ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (m)
    {
        TCHAR ಮಾರ್ಗ[MAX_ಮಾರ್ಗ];
        ಸ್ಥಿರಾಂಕ size_t ಮಾರ್ಗ_ಗಾತ್ರ = ಗಾತ್ರ(ಮಾರ್ಗ) / ಗಾತ್ರ(*ಮಾರ್ಗ);
        size_t sz = GetModuleFileName(m, ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ);
        ಆದರೆ (sz > 0 && sz < ಮಾರ್ಗ_ಗಾತ್ರ)
        {
            ಮಾರ್ಗ[sz] = TCHAR('\0');
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _UNICODE
            ಅಕ್ಷರ char_ಮಾರ್ಗ[MAX_ಮಾರ್ಗ];
            size_t copied = wcstombs(char_ಮಾರ್ಗ, ಮಾರ್ಗ, MAX_ಮಾರ್ಗ);
            CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
            ಹಿಂತಿರುಗಿ cv::String(char_ಮಾರ್ಗ);
#ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ cv::String(ಮಾರ್ಗ);
#ಆದರೆ_ಅಂತ್ಯ
        }
    }
#ಅಥವಾಆದರೆ defined(__linux__)
    Dl_info info;
    ಆದರೆ (0 != dladdr(addr, &info))
    {
        ಹಿಂತಿರುಗಿ cv::String(info.dli_fname);
    }
#ಅಥವಾಆದರೆ defined(__APPLE__)
# ಆದರೆ ಗುರಿ_OS_MAC
    Dl_info info;
    ಆದರೆ (0 != dladdr(addr, &info))
    {
        ಹಿಂತಿರುಗಿ cv::String(info.dli_fname);
    }
# endif
#ಇಲ್ಲದಿದ್ದರೆ
    // not supported, skip
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ cv::String();
}

ತಾರ್ಕಿಕ getBinLocation(std::string& dst)
{
    dst = getModuleLocation((ಶೂನ್ಯ*)getModuleLocation); // using code address, doesn't work with static linkage!
    ಹಿಂತಿರುಗಿ !dst.empty();
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
ತಾರ್ಕಿಕ getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#ಆದರೆ _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ಶೂನ್ಯ* addr = (ಶೂನ್ಯ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ವಿಳಾಸ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (m)
    {
        wchar_t ಮಾರ್ಗ[4096];
        ಸ್ಥಿರಾಂಕ size_t ಮಾರ್ಗ_ಗಾತ್ರ = ಗಾತ್ರ(ಮಾರ್ಗ)/ಗಾತ್ರ(*ಮಾರ್ಗ);
        size_t sz = GetModuleFileNameW(m, ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ);
        ಆದರೆ (sz > 0 && sz < ಮಾರ್ಗ_ಗಾತ್ರ)
        {
            ಮಾರ್ಗ[sz] = '\0';
            dst.assign(ಮಾರ್ಗ, sz);
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}
#ಆದರೆ_ಅಂತ್ಯ

cv::String ಕಂಡುಹಿಡಿ_ದತ್ತಾಂಶ_File(ಸ್ಥಿರಾಂಕ cv::String& relative_ಮಾರ್ಗ,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* configuration_parameter,
                        ಸ್ಥಿರಾಂಕ std::vector<String>* ಹುಡುಕು_paths,
                        ಸ್ಥಿರಾಂಕ std::vector<String>* subdir_paths)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(ಖಾಲಿ, cv::format("utils::findDataFile('%s', %s)", relative_ಮಾರ್ಗ.c_str(), configuration_parameter));

#ವ್ಯಾಖ್ಯಾನಿಸು TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ) \
{ \
    cv::String ಮಾರ್ಗ = utils::fs::join(ಪೂರ್ವಪ್ರತ್ಯಯ, relative_ಮಾರ್ಗ); \
    CV_LOG_DEBUG(ಖಾಲಿ, cv::format("... Line %d: trying open '%s'", __LINE__, ಮಾರ್ಗ.c_str())); \
    FILE* f = fopen(ಮಾರ್ಗ.c_str(), "rb"); \
    ಆದರೆ(f) { \
        fclose(f); \
        ಹಿಂತಿರುಗಿ ಮಾರ್ಗ; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ("");


    // Step 1
    ಸ್ಥಿರಾಂಕ std::vector<cv::String>& ಹುಡುಕು_ಮಾರ್ಗ = ಹುಡುಕು_paths ? *ಹುಡುಕು_paths : get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ();
    ಗಾಗಿ(size_t i = ಹುಡುಕು_ಮಾರ್ಗ.size(); i > 0; i--)
    {
        ಸ್ಥಿರಾಂಕ cv::String& ಪೂರ್ವಪ್ರತ್ಯಯ = ಹುಡುಕು_ಮಾರ್ಗ[i - 1];
        TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
    }

    ಸ್ಥಿರಾಂಕ std::vector<cv::String>& ಹುಡುಕು_subdir = subdir_paths ? *subdir_paths : get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory();


    // Step 2
    ಸ್ಥಿರಾಂಕ cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ಸ್ಥಿರಾಂಕ cv::utils::Paths& ಹುಡುಕು_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    ಗಾಗಿ (size_t k = 0; k < ಹುಡುಕು_hint.size(); k++)
    {
        cv::String datapath = ಹುಡುಕು_hint[k];
        ಆದರೆ (datapath.empty())
            ಮುಂದುವರಿ;
        ಆದರೆ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_WARNING(ಖಾಲಿ, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ಸ್ಥಿರಾಂಕ cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    ಗಾಗಿ (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        ಆದರೆ (datapath.empty())
            ಮುಂದುವರಿ;
        ಆದರೆ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_WARNING(ಖಾಲಿ, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    ಆದರೆ (!override_paths.empty())
    {
        CV_LOG_INFO(ಖಾಲಿ, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_ಮಾರ್ಗ);
        ಹಿಂತಿರುಗಿ cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    ತಾರ್ಕಿಕ has_tested_build_directory = ಸುಳ್ಳು;
    ಆದರೆ (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* build_subdirs[] = { OPENCV_ದತ್ತಾಂಶ_BUILD_DIR_ಹುಡುಕು_PATHS };
        ಗಾಗಿ (size_t k = 0; k < ಗಾತ್ರ(build_subdirs)/ಗಾತ್ರ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ಆದರೆ (utils::fs::isDirectory(datapath))
            {
                ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
                }
            }
        }
        has_tested_build_directory = ನಿಜ;
    }

    cv::String ಮೂಲ_dir;
    cv::String try_ಮೂಲ_dir = cwd;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಮಟ್ಟಗಳು = 0; ಮಟ್ಟಗಳು < 3; ++ಮಟ್ಟಗಳು)
    {
        ಆದರೆ (utils::fs::exists(utils::fs::join(try_ಮೂಲ_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            ಮೂಲ_dir = try_ಮೂಲ_dir;
            ನಿಲ್ಲಿಸು;
        }
        try_ಮೂಲ_dir = utils::fs::join(try_ಮೂಲ_dir, "/..");
    }
    ಆದರೆ (!ಮೂಲ_dir.empty())
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): the current directory is source sub-directory: " << ಮೂಲ_dir);
        CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): <source>" << ಮೂಲ_dir);
        cv::String datapath = ಮೂಲ_dir;
        ಆದರೆ (utils::fs::isDirectory(datapath))
        {
            ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
            }
        }
    }

    cv::String module_ಮಾರ್ಗ;
    ಆದರೆ (getBinLocation(module_ಮಾರ್ಗ))
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "Detected module path: '" << module_ಮಾರ್ಗ << '\'');
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_LOG_INFO(ಖಾಲಿ, "Can't detect module binaries location");
    }

    ಆದರೆ (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_ಮಾರ್ಗ) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_ಮಾರ್ಗ)))
    )
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_ಮಾರ್ಗ);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* build_subdirs[] = { OPENCV_ದತ್ತಾಂಶ_BUILD_DIR_ಹುಡುಕು_PATHS };
        ಗಾಗಿ (size_t k = 0; k < ಗಾತ್ರ(build_subdirs)/ಗಾತ್ರ(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            ಆದರೆ (utils::fs::isDirectory(datapath))
            {
                ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
                }
            }
        }
    }

#ಆದರೆ defined OPENCV_INSTALL_ದತ್ತಾಂಶ_DIR_RELATIVE
    ಆದರೆ (!module_ಮಾರ್ಗ.empty())  // require module path
    {
        size_t pos = module_ಮಾರ್ಗ.rfind('/');
        ಆದರೆ (pos == cv::String::npos)
            pos = module_ಮಾರ್ಗ.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_ಮಾರ್ಗ : module_ಮಾರ್ಗ.substr(0, pos);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* install_subdirs[] = { OPENCV_INSTALL_ದತ್ತಾಂಶ_DIR_RELATIVE };
        ಗಾಗಿ (size_t k = 0; k < ಗಾತ್ರ(install_subdirs)/ಗಾತ್ರ(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            ಆದರೆ (utils::fs::isDirectory(datapath))
            {
                ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
                {
                    ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                    cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined OPENCV_INSTALL_ಪೂರ್ವಪ್ರತ್ಯಯ && defined OPENCV_ದತ್ತಾಂಶ_INSTALL_ಮಾರ್ಗ
    cv::String install_dir(OPENCV_INSTALL_ಪೂರ್ವಪ್ರತ್ಯಯ);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    ಆದರೆ (module_ಮಾರ್ಗ.empty()) // can't determine
        module_ಮಾರ್ಗ = install_dir;
    ಆದರೆ (isSubDirectory(install_dir, module_ಮಾರ್ಗ) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_ಮಾರ್ಗ)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_ದತ್ತಾಂಶ_INSTALL_ಮಾರ್ಗ);
        ಆದರೆ (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "utils::findDataFile(): trying install path: " << datapath);
            ಗಾಗಿ(size_t i = ಹುಡುಕು_subdir.size(); i > 0; i--)
            {
                ಸ್ಥಿರಾಂಕ cv::String& subdir = ಹುಡುಕು_subdir[i - 1];
                cv::String ಪೂರ್ವಪ್ರತ್ಯಯ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ);
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ cv::String();  // not found
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(relative_ಮಾರ್ಗ);
    CV_ಬಳಸದ(configuration_parameter);
    CV_ಬಳಸದ(ಹುಡುಕು_paths);
    CV_ಬಳಸದ(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String ಕಂಡುಹಿಡಿ_ದತ್ತಾಂಶ_File(ಸ್ಥಿರಾಂಕ cv::String& relative_ಮಾರ್ಗ, ತಾರ್ಕಿಕ required, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* configuration_parameter)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ಖಾಲಿ, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_ಮಾರ್ಗ.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String ಫಲಿತಾಂಶ = cv::utils::ಕಂಡುಹಿಡಿ_ದತ್ತಾಂಶ_File(relative_ಮಾರ್ಗ,
                                                configuration_parameter,
                                                ಖಾಲಿ,
                                                ಖಾಲಿ);
    ಆದರೆ (ಫಲಿತಾಂಶ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_ಮಾರ್ಗ.c_str()));
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(relative_ಮಾರ್ಗ);
    CV_ಬಳಸದ(required);
    CV_ಬಳಸದ(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
