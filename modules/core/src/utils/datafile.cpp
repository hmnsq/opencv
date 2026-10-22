// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن "opencv_data_config.hpp"

#ضمن <vector>
#ضمن <fstream>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_لغوه CV_LOG_STRIP_LEVEL
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#که_تعریف_شوی _WIN32
#تعریف_کړه WIN32_LEAN_AND_منځنۍ
#ضمن <windows.h>
#تعریف_لغوه small
#تعریف_لغوه min
#تعریف_لغوه max
#تعریف_لغوه مطلق_قیمت
#وإلاإذا defined(__linux__)
#ضمن <dlfcn.h>  // requires -ldl
#وإلاإذا defined(__APPLE__)
#ضمن <TargetConditionals.h>
#إذا موخه_OS_MAC
#ضمن <dlfcn.h>
#د_که_پای
#د_که_پای

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_پلټنه_لار;
ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_پلټنه_subdir;

ساكن std::vector<cv::String>& get_معلومات_پلټنه_لار()
{
    إذا (g_معلومات_پلټنه_لار.empty())
        g_معلومات_پلټنه_لار.reset(جديد std::vector<cv::String>());
    أرجع *(g_معلومات_پلټنه_لار.get());
}

ساكن std::vector<cv::String>& get_معلومات_پلټنه_Sub_Directory()
{
    إذا (g_معلومات_پلټنه_subdir.empty())
    {
        g_معلومات_پلټنه_subdir.reset(جديد std::vector<cv::String>());
        g_معلومات_پلټنه_subdir->push_back("data");
        g_معلومات_پلټنه_subdir->push_back("");
    }
    أرجع *(g_معلومات_پلټنه_subdir.get());
}


CV_EXPORTS فراغ جمع_کړه_معلومات_پلټنه_لار(ثابت cv::String& لار)
{
    إذا (!لار.empty() && utils::fs::isDirectory(لار))
        get_معلومات_پلټنه_لار().push_back(لار);
}
CV_EXPORTS فراغ جمع_کړه_معلومات_پلټنه_Sub_Directory(ثابت cv::String& subdir)
{
    get_معلومات_پلټنه_Sub_Directory().push_back(subdir);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي is_لار_Sep(حرف c)
{
    أرجع c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& بنسټ_لار, ثابت cv::String& لار)
{
    size_t N = بنسټ_لار.size();
    إذا (N == 0)
        أرجع خطأ;
    إذا (is_لار_Sep(بنسټ_لار[N - 1]))
        N--;
    إذا (لار.size() < N)
        أرجع خطأ;
    لكل (size_t i = 0; i < N; i++)
    {
        إذا (لار[i] == بنسټ_لار[i])
            استمر;
        إذا (is_لار_Sep(لار[i]) && is_لار_Sep(بنسټ_لار[i]))
            استمر;
        أرجع خطأ;
    }
    size_t M = لار.size();
    إذا (M > N)
    {
        إذا (!is_لار_Sep(لار[N]))
            أرجع خطأ;
    }
    أرجع ریښتیا;
}

ساكن منطقي isSubDirectory(ثابت cv::String& بنسټ_لار, ثابت cv::String& لار)
{
    منطقي res = isSubDirectory_(بنسټ_لار, لار);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << بنسټ_لار << "  path: " << لار << "  => result: " << (res ? "TRUE" : "FALSE"));
    أرجع res;
}
#د_که_پای //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_ناکارول_شوی(addr);
#که_تعریف_شوی _WIN32
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پته | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#د_که_پای
    إذا (m)
    {
        TCHAR لار[MAX_لار];
        ثابت size_t لار_اندازه = حجم(لار) / حجم(*لار);
        size_t sz = GetModuleFileName(m, لار, لار_اندازه);
        إذا (sz > 0 && sz < لار_اندازه)
        {
            لار[sz] = TCHAR('\0');
#که_تعریف_شوی _UNICODE
            حرف char_لار[MAX_لار];
            size_t copied = wcstombs(char_لار, لار, MAX_لار);
            CV_Assert((copied != MAX_لار) && (copied != (size_t)-1));
            أرجع cv::String(char_لار);
#وإلا
            أرجع cv::String(لار);
#د_که_پای
        }
    }
#وإلاإذا defined(__linux__)
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
#وإلاإذا defined(__APPLE__)
# إذا موخه_OS_MAC
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
# endif
#وإلا
    // not supported, skip
#د_که_پای
    أرجع cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    أرجع !dst.empty();
}

#که_تعریف_شوی _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پته | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#د_که_پای
    إذا (m)
    {
        wchar_t لار[4096];
        ثابت size_t لار_اندازه = حجم(لار)/حجم(*لار);
        size_t sz = GetModuleFileNameW(m, لار, لار_اندازه);
        إذا (sz > 0 && sz < لار_اندازه)
        {
            لار[sz] = '\0';
            dst.assign(لار, sz);
            أرجع ریښتیا;
        }
    }
    أرجع خطأ;
}
#د_که_پای

cv::String پیدا_کړه_معلومات_File(ثابت cv::String& relative_لار,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* پلټنه_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_لار.c_str(), configuration_parameter));

#تعریف_کړه TRY_FILE_WITH_مختاړی(مختاړی) \
{ \
    cv::String لار = utils::fs::join(مختاړی, relative_لار); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, لار.c_str())); \
    FILE* f = fopen(لار.c_str(), "rb"); \
    إذا(f) { \
        fclose(f); \
        أرجع لار; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_مختاړی("");


    // Step 1
    ثابت std::vector<cv::String>& پلټنه_لار = پلټنه_paths ? *پلټنه_paths : get_معلومات_پلټنه_لار();
    لكل(size_t i = پلټنه_لار.size(); i > 0; i--)
    {
        ثابت cv::String& مختاړی = پلټنه_لار[i - 1];
        TRY_FILE_WITH_مختاړی(مختاړی);
    }

    ثابت std::vector<cv::String>& پلټنه_subdir = subdir_paths ? *subdir_paths : get_معلومات_پلټنه_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& پلټنه_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    لكل (size_t k = 0; k < پلټنه_hint.size(); k++)
    {
        cv::String datapath = پلټنه_hint[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                cv::String مختاړی = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_مختاړی(مختاړی);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ثابت cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    لكل (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                cv::String مختاړی = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_مختاړی(مختاړی);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    إذا (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_لار);
        أرجع cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = خطأ;
    إذا (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_معلومات_BUILD_DIR_پلټنه_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                    cv::String مختاړی = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_مختاړی(مختاړی);
                }
            }
        }
        has_tested_build_directory = ریښتیا;
    }

    cv::String سرچینه_dir;
    cv::String try_سرچینه_dir = cwd;
    لكل (صحيح پوړونه = 0; پوړونه < 3; ++پوړونه)
    {
        إذا (utils::fs::exists(utils::fs::join(try_سرچینه_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            سرچینه_dir = try_سرچینه_dir;
            اكسر;
        }
        try_سرچینه_dir = utils::fs::join(try_سرچینه_dir, "/..");
    }
    إذا (!سرچینه_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << سرچینه_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << سرچینه_dir);
        cv::String datapath = سرچینه_dir;
        إذا (utils::fs::isDirectory(datapath))
        {
            لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                cv::String مختاړی = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_مختاړی(مختاړی);
            }
        }
    }

    cv::String module_لار;
    إذا (getBinLocation(module_لار))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_لار << '\'');
    }
    وإلا
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    إذا (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_لار) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_لار)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_لار);
        ثابت حرف* build_subdirs[] = { OPENCV_معلومات_BUILD_DIR_پلټنه_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                    cv::String مختاړی = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_مختاړی(مختاړی);
                }
            }
        }
    }

#إذا defined OPENCV_INSTALL_معلومات_DIR_RELATIVE
    إذا (!module_لار.empty())  // require module path
    {
        size_t pos = module_لار.rfind('/');
        إذا (pos == cv::String::npos)
            pos = module_لار.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_لار : module_لار.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_معلومات_DIR_RELATIVE };
        لكل (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                    cv::String مختاړی = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_مختاړی(مختاړی);
                }
            }
            وإلا
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#د_که_پای

#إذا defined OPENCV_INSTALL_مختاړی && defined OPENCV_معلومات_INSTALL_لار
    cv::String install_dir(OPENCV_INSTALL_مختاړی);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    إذا (module_لار.empty()) // can't determine
        module_لار = install_dir;
    إذا (isSubDirectory(install_dir, module_لار) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_لار)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_معلومات_INSTALL_لار);
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            لكل(size_t i = پلټنه_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = پلټنه_subdir[i - 1];
                cv::String مختاړی = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_مختاړی(مختاړی);
            }
        }
    }
#د_که_پای

    أرجع cv::String();  // not found
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ناکارول_شوی(relative_لار);
    CV_ناکارول_شوی(configuration_parameter);
    CV_ناکارول_شوی(پلټنه_paths);
    CV_ناکارول_شوی(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String پیدا_کړه_معلومات_File(ثابت cv::String& relative_لار, منطقي required, ثابت حرف* configuration_parameter)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_لار.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String پایله = cv::utils::پیدا_کړه_معلومات_File(relative_لار,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    إذا (پایله.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_لار.c_str()));
    أرجع پایله;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ناکارول_شوی(relative_لار);
    CV_ناکارول_شوی(required);
    CV_ناکارول_شوی(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
