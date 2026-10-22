// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن "opencv_data_config.hpp"

#ضمن <vector>
#ضمن <fstream>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#لغو_تعریف CV_LOG_STRIP_LEVEL
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#اگر_تعریف_شده _WIN32
#تعریف_کن WIN32_LEAN_AND_میانگین
#ضمن <windows.h>
#لغو_تعریف small
#لغو_تعریف min
#لغو_تعریف max
#لغو_تعریف قدر_مطلق
#وإلاإذا defined(__linux__)
#ضمن <dlfcn.h>  // requires -ldl
#وإلاإذا defined(__APPLE__)
#ضمن <TargetConditionals.h>
#إذا هدف_OS_MAC
#ضمن <dlfcn.h>
#پایان_اگر
#پایان_اگر

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_داده_جستجو_مسیر;
ساكن cv::Ptr< std::vector<cv::String> > g_داده_جستجو_subdir;

ساكن std::vector<cv::String>& get_داده_جستجو_مسیر()
{
    إذا (g_داده_جستجو_مسیر.empty())
        g_داده_جستجو_مسیر.reset(جديد std::vector<cv::String>());
    أرجع *(g_داده_جستجو_مسیر.get());
}

ساكن std::vector<cv::String>& get_داده_جستجو_Sub_Directory()
{
    إذا (g_داده_جستجو_subdir.empty())
    {
        g_داده_جستجو_subdir.reset(جديد std::vector<cv::String>());
        g_داده_جستجو_subdir->push_back("data");
        g_داده_جستجو_subdir->push_back("");
    }
    أرجع *(g_داده_جستجو_subdir.get());
}


CV_EXPORTS فراغ جمع_کن_داده_جستجو_مسیر(ثابت cv::String& مسیر)
{
    إذا (!مسیر.empty() && utils::fs::isDirectory(مسیر))
        get_داده_جستجو_مسیر().push_back(مسیر);
}
CV_EXPORTS فراغ جمع_کن_داده_جستجو_Sub_Directory(ثابت cv::String& subdir)
{
    get_داده_جستجو_Sub_Directory().push_back(subdir);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي is_مسیر_Sep(حرف c)
{
    أرجع c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& پایه_مسیر, ثابت cv::String& مسیر)
{
    size_t N = پایه_مسیر.size();
    إذا (N == 0)
        أرجع خطأ;
    إذا (is_مسیر_Sep(پایه_مسیر[N - 1]))
        N--;
    إذا (مسیر.size() < N)
        أرجع خطأ;
    لكل (size_t i = 0; i < N; i++)
    {
        إذا (مسیر[i] == پایه_مسیر[i])
            استمر;
        إذا (is_مسیر_Sep(مسیر[i]) && is_مسیر_Sep(پایه_مسیر[i]))
            استمر;
        أرجع خطأ;
    }
    size_t M = مسیر.size();
    إذا (M > N)
    {
        إذا (!is_مسیر_Sep(مسیر[N]))
            أرجع خطأ;
    }
    أرجع درست;
}

ساكن منطقي isSubDirectory(ثابت cv::String& پایه_مسیر, ثابت cv::String& مسیر)
{
    منطقي res = isSubDirectory_(پایه_مسیر, مسیر);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << پایه_مسیر << "  path: " << مسیر << "  => result: " << (res ? "TRUE" : "FALSE"));
    أرجع res;
}
#پایان_اگر //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_استفادهنشده(addr);
#اگر_تعریف_شده _WIN32
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_آدرس | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#پایان_اگر
    إذا (m)
    {
        TCHAR مسیر[MAX_مسیر];
        ثابت size_t مسیر_اندازه = حجم(مسیر) / حجم(*مسیر);
        size_t sz = GetModuleFileName(m, مسیر, مسیر_اندازه);
        إذا (sz > 0 && sz < مسیر_اندازه)
        {
            مسیر[sz] = TCHAR('\0');
#اگر_تعریف_شده _UNICODE
            حرف char_مسیر[MAX_مسیر];
            size_t copied = wcstombs(char_مسیر, مسیر, MAX_مسیر);
            CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
            أرجع cv::String(char_مسیر);
#وإلا
            أرجع cv::String(مسیر);
#پایان_اگر
        }
    }
#وإلاإذا defined(__linux__)
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
#وإلاإذا defined(__APPLE__)
# إذا هدف_OS_MAC
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
# endif
#وإلا
    // not supported, skip
#پایان_اگر
    أرجع cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    أرجع !dst.empty();
}

#اگر_تعریف_شده _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_آدرس | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#پایان_اگر
    إذا (m)
    {
        wchar_t مسیر[4096];
        ثابت size_t مسیر_اندازه = حجم(مسیر)/حجم(*مسیر);
        size_t sz = GetModuleFileNameW(m, مسیر, مسیر_اندازه);
        إذا (sz > 0 && sz < مسیر_اندازه)
        {
            مسیر[sz] = '\0';
            dst.assign(مسیر, sz);
            أرجع درست;
        }
    }
    أرجع خطأ;
}
#پایان_اگر

cv::String پیدا_کن_داده_File(ثابت cv::String& relative_مسیر,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* جستجو_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_مسیر.c_str(), configuration_parameter));

#تعریف_کن TRY_FILE_WITH_پیشوند(پیشوند) \
{ \
    cv::String مسیر = utils::fs::join(پیشوند, relative_مسیر); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, مسیر.c_str())); \
    FILE* f = fopen(مسیر.c_str(), "rb"); \
    إذا(f) { \
        fclose(f); \
        أرجع مسیر; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_پیشوند("");


    // Step 1
    ثابت std::vector<cv::String>& جستجو_مسیر = جستجو_paths ? *جستجو_paths : get_داده_جستجو_مسیر();
    لكل(size_t i = جستجو_مسیر.size(); i > 0; i--)
    {
        ثابت cv::String& پیشوند = جستجو_مسیر[i - 1];
        TRY_FILE_WITH_پیشوند(پیشوند);
    }

    ثابت std::vector<cv::String>& جستجو_subdir = subdir_paths ? *subdir_paths : get_داده_جستجو_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& جستجو_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    لكل (size_t k = 0; k < جستجو_hint.size(); k++)
    {
        cv::String datapath = جستجو_hint[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String پیشوند = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_پیشوند(پیشوند);
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
            لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String پیشوند = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_پیشوند(پیشوند);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    إذا (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_مسیر);
        أرجع cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = خطأ;
    إذا (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_داده_BUILD_DIR_جستجو_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String پیشوند = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_پیشوند(پیشوند);
                }
            }
        }
        has_tested_build_directory = درست;
    }

    cv::String منبع_dir;
    cv::String try_منبع_dir = cwd;
    لكل (صحيح سطوح = 0; سطوح < 3; ++سطوح)
    {
        إذا (utils::fs::exists(utils::fs::join(try_منبع_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            منبع_dir = try_منبع_dir;
            اكسر;
        }
        try_منبع_dir = utils::fs::join(try_منبع_dir, "/..");
    }
    إذا (!منبع_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << منبع_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << منبع_dir);
        cv::String datapath = منبع_dir;
        إذا (utils::fs::isDirectory(datapath))
        {
            لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String پیشوند = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_پیشوند(پیشوند);
            }
        }
    }

    cv::String module_مسیر;
    إذا (getBinLocation(module_مسیر))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_مسیر << '\'');
    }
    وإلا
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    إذا (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_مسیر) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_مسیر)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_مسیر);
        ثابت حرف* build_subdirs[] = { OPENCV_داده_BUILD_DIR_جستجو_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String پیشوند = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_پیشوند(پیشوند);
                }
            }
        }
    }

#إذا defined OPENCV_INSTALL_داده_DIR_RELATIVE
    إذا (!module_مسیر.empty())  // require module path
    {
        size_t pos = module_مسیر.rfind('/');
        إذا (pos == cv::String::npos)
            pos = module_مسیر.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_مسیر : module_مسیر.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_داده_DIR_RELATIVE };
        لكل (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = جستجو_subdir[i - 1];
                    cv::String پیشوند = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_پیشوند(پیشوند);
                }
            }
            وإلا
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#پایان_اگر

#إذا defined OPENCV_INSTALL_پیشوند && defined OPENCV_داده_INSTALL_مسیر
    cv::String install_dir(OPENCV_INSTALL_پیشوند);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    إذا (module_مسیر.empty()) // can't determine
        module_مسیر = install_dir;
    إذا (isSubDirectory(install_dir, module_مسیر) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_مسیر)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_داده_INSTALL_مسیر);
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            لكل(size_t i = جستجو_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = جستجو_subdir[i - 1];
                cv::String پیشوند = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_پیشوند(پیشوند);
            }
        }
    }
#پایان_اگر

    أرجع cv::String();  // not found
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(relative_مسیر);
    CV_استفادهنشده(configuration_parameter);
    CV_استفادهنشده(جستجو_paths);
    CV_استفادهنشده(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String پیدا_کن_داده_File(ثابت cv::String& relative_مسیر, منطقي required, ثابت حرف* configuration_parameter)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_مسیر.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String نتیجه = cv::utils::پیدا_کن_داده_File(relative_مسیر,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    إذا (نتیجه.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_مسیر.c_str()));
    أرجع نتیجه;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(relative_مسیر);
    CV_استفادهنشده(required);
    CV_استفادهنشده(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
