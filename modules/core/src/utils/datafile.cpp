// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن "opencv_data_config.hpp"

#ضمن <vector>
#ضمن <fstream>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_رد CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#جے_تعریف_شدہ _WIN32
#تعریف_کرو WIN32_LEAN_AND_اوسط
#ضمن <windows.h>
#تعریف_رد small
#تعریف_رد min
#تعریف_رد max
#تعریف_رد مطلق_قدر
#وإلاإذا defined(__linux__)
#ضمن <dlfcn.h>  // requires -ldl
#وإلاإذا defined(__APPLE__)
#ضمن <TargetConditionals.h>
#إذا نشانہ_OS_MAC
#ضمن <dlfcn.h>
#جے_انت
#جے_انت

نطاق cv { نطاق utils {

ساكن cv::Ptr< std::vector<cv::String> > g_انکڑے_تلاش_رستہ;
ساكن cv::Ptr< std::vector<cv::String> > g_انکڑے_تلاش_subdir;

ساكن std::vector<cv::String>& get_انکڑے_تلاش_رستہ()
{
    إذا (g_انکڑے_تلاش_رستہ.empty())
        g_انکڑے_تلاش_رستہ.reset(جديد std::vector<cv::String>());
    أرجع *(g_انکڑے_تلاش_رستہ.get());
}

ساكن std::vector<cv::String>& get_انکڑے_تلاش_Sub_Directory()
{
    إذا (g_انکڑے_تلاش_subdir.empty())
    {
        g_انکڑے_تلاش_subdir.reset(جديد std::vector<cv::String>());
        g_انکڑے_تلاش_subdir->push_back("data");
        g_انکڑے_تلاش_subdir->push_back("");
    }
    أرجع *(g_انکڑے_تلاش_subdir.get());
}


CV_EXPORTS فراغ جوڑو_انکڑے_تلاش_رستہ(ثابت cv::String& رستہ)
{
    إذا (!رستہ.empty() && utils::fs::isDirectory(رستہ))
        get_انکڑے_تلاش_رستہ().push_back(رستہ);
}
CV_EXPORTS فراغ جوڑو_انکڑے_تلاش_Sub_Directory(ثابت cv::String& subdir)
{
    get_انکڑے_تلاش_Sub_Directory().push_back(subdir);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي is_رستہ_Sep(حرف c)
{
    أرجع c == '/' || c == '\\';
}
ساكن منطقي isSubDirectory_(ثابت cv::String& ادھار_رستہ, ثابت cv::String& رستہ)
{
    size_t N = ادھار_رستہ.size();
    إذا (N == 0)
        أرجع خطأ;
    إذا (is_رستہ_Sep(ادھار_رستہ[N - 1]))
        N--;
    إذا (رستہ.size() < N)
        أرجع خطأ;
    لكل (size_t i = 0; i < N; i++)
    {
        إذا (رستہ[i] == ادھار_رستہ[i])
            استمر;
        إذا (is_رستہ_Sep(رستہ[i]) && is_رستہ_Sep(ادھار_رستہ[i]))
            استمر;
        أرجع خطأ;
    }
    size_t M = رستہ.size();
    إذا (M > N)
    {
        إذا (!is_رستہ_Sep(رستہ[N]))
            أرجع خطأ;
    }
    أرجع سچ;
}

ساكن منطقي isSubDirectory(ثابت cv::String& ادھار_رستہ, ثابت cv::String& رستہ)
{
    منطقي res = isSubDirectory_(ادھار_رستہ, رستہ);
    CV_LOG_VERBOSE(عدم, 0, "isSubDirectory(): base: " << ادھار_رستہ << "  path: " << رستہ << "  => result: " << (res ? "TRUE" : "FALSE"));
    أرجع res;
}
#جے_انت //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساكن cv::String getModuleLocation(ثابت فراغ* addr)
{
    CV_بن_ورتیا(addr);
#جے_تعریف_شدہ _WIN32
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتہ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#جے_انت
    إذا (m)
    {
        TCHAR رستہ[MAX_رستہ];
        ثابت size_t رستہ_ناپ = حجم(رستہ) / حجم(*رستہ);
        size_t sz = GetModuleFileName(m, رستہ, رستہ_ناپ);
        إذا (sz > 0 && sz < رستہ_ناپ)
        {
            رستہ[sz] = TCHAR('\0');
#جے_تعریف_شدہ _UNICODE
            حرف char_رستہ[MAX_رستہ];
            size_t copied = wcstombs(char_رستہ, رستہ, MAX_رستہ);
            CV_Assert((copied != MAX_رستہ) && (copied != (size_t)-1));
            أرجع cv::String(char_رستہ);
#وإلا
            أرجع cv::String(رستہ);
#جے_انت
        }
    }
#وإلاإذا defined(__linux__)
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
#وإلاإذا defined(__APPLE__)
# إذا نشانہ_OS_MAC
    Dl_info info;
    إذا (0 != dladdr(addr, &info))
    {
        أرجع cv::String(info.dli_fname);
    }
# endif
#وإلا
    // not supported, skip
#جے_انت
    أرجع cv::String();
}

منطقي getBinLocation(std::string& dst)
{
    dst = getModuleLocation((فراغ*)getModuleLocation); // using code address, doesn't work with static linkage!
    أرجع !dst.empty();
}

#جے_تعریف_شدہ _WIN32
منطقي getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#إذا _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    فراغ* addr = (فراغ*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتہ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#جے_انت
    إذا (m)
    {
        wchar_t رستہ[4096];
        ثابت size_t رستہ_ناپ = حجم(رستہ)/حجم(*رستہ);
        size_t sz = GetModuleFileNameW(m, رستہ, رستہ_ناپ);
        إذا (sz > 0 && sz < رستہ_ناپ)
        {
            رستہ[sz] = '\0';
            dst.assign(رستہ, sz);
            أرجع سچ;
        }
    }
    أرجع خطأ;
}
#جے_انت

cv::String لبھو_انکڑے_File(ثابت cv::String& relative_رستہ,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* تلاش_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(عدم, cv::format("utils::findDataFile('%s', %s)", relative_رستہ.c_str(), configuration_parameter));

#تعریف_کرو TRY_FILE_WITH_اگیتر(اگیتر) \
{ \
    cv::String رستہ = utils::fs::join(اگیتر, relative_رستہ); \
    CV_LOG_DEBUG(عدم, cv::format("... Line %d: trying open '%s'", __LINE__, رستہ.c_str())); \
    FILE* f = fopen(رستہ.c_str(), "rb"); \
    إذا(f) { \
        fclose(f); \
        أرجع رستہ; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_اگیتر("");


    // Step 1
    ثابت std::vector<cv::String>& تلاش_رستہ = تلاش_paths ? *تلاش_paths : get_انکڑے_تلاش_رستہ();
    لكل(size_t i = تلاش_رستہ.size(); i > 0; i--)
    {
        ثابت cv::String& اگیتر = تلاش_رستہ[i - 1];
        TRY_FILE_WITH_اگیتر(اگیتر);
    }

    ثابت std::vector<cv::String>& تلاش_subdir = subdir_paths ? *subdir_paths : get_انکڑے_تلاش_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& تلاش_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    لكل (size_t k = 0; k < تلاش_hint.size(); k++)
    {
        cv::String datapath = تلاش_hint[k];
        إذا (datapath.empty())
            استمر;
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String اگیتر = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اگیتر(اگیتر);
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
            لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String اگیتر = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اگیتر(اگیتر);
            }
        }
        وإلا
        {
            CV_LOG_WARNING(عدم, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    إذا (!override_paths.empty())
    {
        CV_LOG_INFO(عدم, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_رستہ);
        أرجع cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقي has_tested_build_directory = خطأ;
    إذا (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_انکڑے_BUILD_DIR_تلاش_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String اگیتر = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اگیتر(اگیتر);
                }
            }
        }
        has_tested_build_directory = سچ;
    }

    cv::String سروت_dir;
    cv::String try_سروت_dir = cwd;
    لكل (صحيح پدھراں = 0; پدھراں < 3; ++پدھراں)
    {
        إذا (utils::fs::exists(utils::fs::join(try_سروت_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            سروت_dir = try_سروت_dir;
            اكسر;
        }
        try_سروت_dir = utils::fs::join(try_سروت_dir, "/..");
    }
    إذا (!سروت_dir.empty())
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the current directory is source sub-directory: " << سروت_dir);
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): <source>" << سروت_dir);
        cv::String datapath = سروت_dir;
        إذا (utils::fs::isDirectory(datapath))
        {
            لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String اگیتر = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اگیتر(اگیتر);
            }
        }
    }

    cv::String module_رستہ;
    إذا (getBinLocation(module_رستہ))
    {
        CV_LOG_DEBUG(عدم, "Detected module path: '" << module_رستہ << '\'');
    }
    وإلا
    {
        CV_LOG_INFO(عدم, "Can't detect module binaries location");
    }

    إذا (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_رستہ) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_رستہ)))
    )
    {
        CV_LOG_DEBUG(عدم, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_رستہ);
        ثابت حرف* build_subdirs[] = { OPENCV_انکڑے_BUILD_DIR_تلاش_PATHS };
        لكل (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String اگیتر = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اگیتر(اگیتر);
                }
            }
        }
    }

#إذا defined OPENCV_INSTALL_انکڑے_DIR_RELATIVE
    إذا (!module_رستہ.empty())  // require module path
    {
        size_t pos = module_رستہ.rfind('/');
        إذا (pos == cv::String::npos)
            pos = module_رستہ.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_رستہ : module_رستہ.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_انکڑے_DIR_RELATIVE };
        لكل (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            إذا (utils::fs::isDirectory(datapath))
            {
                لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String اگیتر = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_اگیتر(اگیتر);
                }
            }
            وإلا
            {
                CV_LOG_DEBUG(عدم, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#جے_انت

#إذا defined OPENCV_INSTALL_اگیتر && defined OPENCV_انکڑے_INSTALL_رستہ
    cv::String install_dir(OPENCV_INSTALL_اگیتر);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    إذا (module_رستہ.empty()) // can't determine
        module_رستہ = install_dir;
    إذا (isSubDirectory(install_dir, module_رستہ) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_رستہ)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_انکڑے_INSTALL_رستہ);
        إذا (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(عدم, "utils::findDataFile(): trying install path: " << datapath);
            لكل(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String اگیتر = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_اگیتر(اگیتر);
            }
        }
    }
#جے_انت

    أرجع cv::String();  // not found
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_بن_ورتیا(relative_رستہ);
    CV_بن_ورتیا(configuration_parameter);
    CV_بن_ورتیا(تلاش_paths);
    CV_بن_ورتیا(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String لبھو_انکڑے_File(ثابت cv::String& relative_رستہ, منطقي required, ثابت حرف* configuration_parameter)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_رستہ.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String نتیجہ = cv::utils::لبھو_انکڑے_File(relative_رستہ,
                                                configuration_parameter,
                                                عدم,
                                                عدم);
    إذا (نتیجہ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_رستہ.c_str()));
    أرجع نتیجہ;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_بن_ورتیا(relative_رستہ);
    CV_بن_ورتیا(required);
    CV_بن_ورتیا(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
