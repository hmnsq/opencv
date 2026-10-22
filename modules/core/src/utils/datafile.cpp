// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن "opencv_data_config.hpp"

#ضمن <vector>
#ضمن <fstream>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#وصف_رد CV_LOG_STRIP_LEVEL
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#جيڪڏهن_وصف_ٿيل _WIN32
#وصف_ڪريو WIN32_LEAN_AND_سراسري
#ضمن <windows.h>
#وصف_رد small
#وصف_رد min
#وصف_رد max
#وصف_رد مطلق_قدر
#وإلاإذا defined(__linux__)
#ضمن <dlfcn.h>  // requires -ldl
#وإلاإذا defined(__APPLE__)
#ضمن <TargetConditionals.h>
#إذا هدف_OS_MAC
#ضمن <dlfcn.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_ڳولا_رستو;
ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_ڳولا_subdir;

ساكن std::vector<cv::String>& get_معلومات_ڳولا_رستو()
{
    إذا (g_معلومات_ڳولا_رستو.empty())
        g_معلومات_ڳولا_رستو.reset(جديد std::vector<cv::String>());
    أرجع *(g_معلومات_ڳولا_رستو.get());
}

ساكن std::vector<cv::String>& get_معلومات_ڳولا_Sub_Directory()
{
    إذا (g_معلومات_ڳولا_subdir.empty())
    {
        g_معلومات_ڳولا_subdir.reset(جديد std::vector<cv::String>());
        g_معلومات_ڳولا_subdir->push_back("data");
        g_معلومات_ڳولا_subdir->push_back("");
    }
    أرجع *(g_معلومات_ڳولا_subdir.get());
}


CV_EXPORTS فراغ جوڙيو_معلومات_ڳولا_رستو(ثابت cv::String& رستو)
{
    إذا (!رستو.empty() && utils::fs::isDirectory(رستو))
        get_معلومات_ڳولا_رستو().push_back(رستو);
}
CV_EXPORTS فراغ جوڙيو_معلومات_ڳولا_Sub_Directory(ثابت cv::String& subdir)
{
    get_معلومات_ڳولا_Sub_Directory().push_back(subdir);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي is_رستو_Sep(حرف c)
{
    أرجع c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& بنياد_رستو, ثابت cv::String& رستو)
{
    size_t N = بنياد_رستو.size();
    إذا (N == 0)
        أرجع خطأ;
    إذا (is_رستو_Sep(بنياد_رستو[N - 1]))
        N--;
    إذا (رستو.size() < N)
        أرجع خطأ;
    لكل (size_t i = 0; i < N; i++)
    {
        إذا (رستو[i] == بنياد_رستو[i])
            استمر;
        إذا (is_رستو_Sep(رستو[i]) && is_رستو_Sep(بنياد_رستو[i]))
            استمر;
        أرجع خطأ;
    }
    size_t M = رستو.size();
    إذا (M > N)
    {
        إذا (!is_رستو_Sep(رستو[N]))
            أرجع خطأ;
    }
    أرجع سچ;
}

ساكن منطقي isSubDirectory(ثابت cv::String& بنياد_رستو, ثابت cv::String& رستو)
{
    منطقي res = isSubDirectory_(بنياد_رستو, رستو);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << بنياد_رستو << "  path: " << رستو << "  => result: " << (res ? "TRUE" : "FALSE"));
    أرجع res;
}
#جيڪڏهن_پورو //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_اڻ_استعمال_ٿيل(addr);
#جيڪڏهن_وصف_ٿيل _WIN32
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتو | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#جيڪڏهن_پورو
    إذا (m)
    {
        TCHAR رستو[MAX_رستو];
        ثابت size_t رستو_سائيز = حجم(رستو) / حجم(*رستو);
        size_t sz = GetModuleFileName(m, رستو, رستو_سائيز);
        إذا (sz > 0 && sz < رستو_سائيز)
        {
            رستو[sz] = TCHAR('\0');
#جيڪڏهن_وصف_ٿيل _UNICODE
            حرف char_رستو[MAX_رستو];
            size_t copied = wcstombs(char_رستو, رستو, MAX_رستو);
            CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
            أرجع cv::String(char_رستو);
#وإلا
            أرجع cv::String(رستو);
#جيڪڏهن_پورو
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
#جيڪڏهن_پورو
    أرجع cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    أرجع !dst.empty();
}

#جيڪڏهن_وصف_ٿيل _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتو | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#جيڪڏهن_پورو
    إذا (m)
    {
        wchar_t رستو[4096];
        ثابت size_t رستو_سائيز = حجم(رستو)/حجم(*رستو);
        size_t sz = GetModuleFileNameW(m, رستو, رستو_سائيز);
        إذا (sz > 0 && sz < رستو_سائيز)
        {
            رستو[sz] = '\0';
            dst.assign(رستو, sz);
            أرجع سچ;
        }
    }
    أرجع خطأ;
}
#جيڪڏهن_پورو

cv::String ڳوليو_معلومات_File(ثابت cv::String& relative_رستو,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* ڳولا_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_رستو.c_str(), configuration_parameter));

#وصف_ڪريو TRY_FILE_WITH_اڳياڙي(اڳياڙي) \
{ \
    cv::String رستو = utils::fs::join(اڳياڙي, relative_رستو); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, رستو.c_str())); \
    FILE* f = fopen(رستو.c_str(), "rb"); \
    إذا(f) { \
        fclose(f); \
        أرجع رستو; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_اڳياڙي("");


    // Step 1
    ثابت std::vector<cv::String>& ڳولا_رستو = ڳولا_paths ? *ڳولا_paths : get_معلومات_ڳولا_رستو();
    لكل(size_t i = ڳولا_رستو.size(); i > 0; i--)
    {
        ثابت cv::String& اڳياڙي = ڳولا_رستو[i - 1];
        TRY_FILE_WITH_اڳياڙي(اڳياڙي);
    }

    ثابت std::vector<cv::String>& ڳولا_subdir = subdir_paths ? *subdir_paths : get_معلومات_ڳولا_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& ڳولا_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    لكل (size_t k = 0; k < ڳولا_hint.size(); k++)
    {
        cv::String datapath = ڳولا_hint[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اڳياڙي(اڳياڙي);
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
            لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اڳياڙي(اڳياڙي);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    إذا (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_رستو);
        أرجع cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = خطأ;
    إذا (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_معلومات_BUILD_DIR_ڳولا_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                    cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اڳياڙي(اڳياڙي);
                }
            }
        }
        has_tested_build_directory = سچ;
    }

    cv::String ذريعو_dir;
    cv::String try_ذريعو_dir = cwd;
    لكل (صحيح سطحون = 0; سطحون < 3; ++سطحون)
    {
        إذا (utils::fs::exists(utils::fs::join(try_ذريعو_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            ذريعو_dir = try_ذريعو_dir;
            اكسر;
        }
        try_ذريعو_dir = utils::fs::join(try_ذريعو_dir, "/..");
    }
    إذا (!ذريعو_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << ذريعو_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << ذريعو_dir);
        cv::String datapath = ذريعو_dir;
        إذا (utils::fs::isDirectory(datapath))
        {
            لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اڳياڙي(اڳياڙي);
            }
        }
    }

    cv::String module_رستو;
    إذا (getBinLocation(module_رستو))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_رستو << '\'');
    }
    وإلا
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    إذا (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_رستو) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_رستو)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_رستو);
        ثابت حرف* build_subdirs[] = { OPENCV_معلومات_BUILD_DIR_ڳولا_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                    cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اڳياڙي(اڳياڙي);
                }
            }
        }
    }

#إذا defined OPENCV_INSTALL_معلومات_DIR_RELATIVE
    إذا (!module_رستو.empty())  // require module path
    {
        size_t pos = module_رستو.rfind('/');
        إذا (pos == cv::String::npos)
            pos = module_رستو.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_رستو : module_رستو.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_معلومات_DIR_RELATIVE };
        لكل (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                    cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اڳياڙي(اڳياڙي);
                }
            }
            وإلا
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#جيڪڏهن_پورو

#إذا defined OPENCV_INSTALL_اڳياڙي && defined OPENCV_معلومات_INSTALL_رستو
    cv::String install_dir(OPENCV_INSTALL_اڳياڙي);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    إذا (module_رستو.empty()) // can't determine
        module_رستو = install_dir;
    إذا (isSubDirectory(install_dir, module_رستو) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_رستو)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_معلومات_INSTALL_رستو);
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            لكل(size_t i = ڳولا_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = ڳولا_subdir[i - 1];
                cv::String اڳياڙي = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اڳياڙي(اڳياڙي);
            }
        }
    }
#جيڪڏهن_پورو

    أرجع cv::String();  // not found
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_اڻ_استعمال_ٿيل(relative_رستو);
    CV_اڻ_استعمال_ٿيل(configuration_parameter);
    CV_اڻ_استعمال_ٿيل(ڳولا_paths);
    CV_اڻ_استعمال_ٿيل(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String ڳوليو_معلومات_File(ثابت cv::String& relative_رستو, منطقي required, ثابت حرف* configuration_parameter)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_رستو.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String نتيجو = cv::utils::ڳوليو_معلومات_File(relative_رستو,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    إذا (نتيجو.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_رستو.c_str()));
    أرجع نتيجو;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_اڻ_استعمال_ٿيل(relative_رستو);
    CV_اڻ_استعمال_ٿيل(required);
    CV_اڻ_استعمال_ٿيل(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
