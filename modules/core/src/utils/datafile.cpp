// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل "opencv_data_config.hpp"

#شامل <vector>
#شامل <fstream>

#شامل <opencv2/core/utils/logger.defines.hpp>
#تعریف_منسوخ CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل "opencv2/core/utils/logger.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل "opencv2/core/utils/filesystem.private.hpp"

#اگر_تعریف_شدہ _WIN32
#تعریف_کرو WIN32_LEAN_AND_اوسط
#شامل <windows.h>
#تعریف_منسوخ small
#تعریف_منسوخ min
#تعریف_منسوخ max
#تعریف_منسوخ مطلق_قدر
#وگرنہ_اگر defined(__linux__)
#شامل <dlfcn.h>  // requires -ldl
#وگرنہ_اگر defined(__APPLE__)
#شامل <TargetConditionals.h>
#اگر ہدف_OS_MAC
#شامل <dlfcn.h>
#اگر_ختم
#اگر_ختم

نامخلا cv { نامخلا utils {

ساکن cv::Ptr< std::vector<cv::String> > g_ڈیٹا_تلاش_راستہ;
ساکن cv::Ptr< std::vector<cv::String> > g_ڈیٹا_تلاش_subdir;

ساکن std::vector<cv::String>& get_ڈیٹا_تلاش_راستہ()
{
    اگر (g_ڈیٹا_تلاش_راستہ.empty())
        g_ڈیٹا_تلاش_راستہ.reset(نیا std::vector<cv::String>());
    واپس *(g_ڈیٹا_تلاش_راستہ.get());
}

ساکن std::vector<cv::String>& get_ڈیٹا_تلاش_Sub_Directory()
{
    اگر (g_ڈیٹا_تلاش_subdir.empty())
    {
        g_ڈیٹا_تلاش_subdir.reset(نیا std::vector<cv::String>());
        g_ڈیٹا_تلاش_subdir->push_back("data");
        g_ڈیٹا_تلاش_subdir->push_back("");
    }
    واپس *(g_ڈیٹا_تلاش_subdir.get());
}


CV_EXPORTS خالی جوڑو_ڈیٹا_تلاش_راستہ(ثابت cv::String& راستہ)
{
    اگر (!راستہ.empty() && utils::fs::isDirectory(راستہ))
        get_ڈیٹا_تلاش_راستہ().push_back(راستہ);
}
CV_EXPORTS خالی جوڑو_ڈیٹا_تلاش_Sub_Directory(ثابت cv::String& subdir)
{
    get_ڈیٹا_تلاش_Sub_Directory().push_back(subdir);
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ساکن منطقی is_راستہ_Sep(حرف c)
{
    واپس c == '/' || c == '\\';
}
ساکن منطقی isSubDirectory_(ثابت cv::String& بنیاد_راستہ, ثابت cv::String& راستہ)
{
    size_t N = بنیاد_راستہ.size();
    اگر (N == 0)
        واپس جھوٹ;
    اگر (is_راستہ_Sep(بنیاد_راستہ[N - 1]))
        N--;
    اگر (راستہ.size() < N)
        واپس جھوٹ;
    برائے (size_t i = 0; i < N; i++)
    {
        اگر (راستہ[i] == بنیاد_راستہ[i])
            جاری;
        اگر (is_راستہ_Sep(راستہ[i]) && is_راستہ_Sep(بنیاد_راستہ[i]))
            جاری;
        واپس جھوٹ;
    }
    size_t M = راستہ.size();
    اگر (M > N)
    {
        اگر (!is_راستہ_Sep(راستہ[N]))
            واپس جھوٹ;
    }
    واپس سچ;
}

ساکن منطقی isSubDirectory(ثابت cv::String& بنیاد_راستہ, ثابت cv::String& راستہ)
{
    منطقی res = isSubDirectory_(بنیاد_راستہ, راستہ);
    CV_LOG_VERBOSE(تہی, 0, "isSubDirectory(): base: " << بنیاد_راستہ << "  path: " << راستہ << "  => result: " << (res ? "TRUE" : "FALSE"));
    واپس res;
}
#اگر_ختم //OPENCV_HAVE_FILESYSTEM_SUPPORT

ساکن cv::String getModuleLocation(ثابت خالی* addr)
{
    CV_غیر_مستعمل(addr);
#اگر_تعریف_شدہ _WIN32
    HMODULE m = 0;
#اگر _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتہ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#اگر_ختم
    اگر (m)
    {
        TCHAR راستہ[MAX_راستہ];
        ثابت size_t راستہ_سائز = حجم(راستہ) / حجم(*راستہ);
        size_t sz = GetModuleFileName(m, راستہ, راستہ_سائز);
        اگر (sz > 0 && sz < راستہ_سائز)
        {
            راستہ[sz] = TCHAR('\0');
#اگر_تعریف_شدہ _UNICODE
            حرف char_راستہ[MAX_راستہ];
            size_t copied = wcstombs(char_راستہ, راستہ, MAX_راستہ);
            CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
            واپس cv::String(char_راستہ);
#ورنہ
            واپس cv::String(راستہ);
#اگر_ختم
        }
    }
#وگرنہ_اگر defined(__linux__)
    Dl_info info;
    اگر (0 != dladdr(addr, &info))
    {
        واپس cv::String(info.dli_fname);
    }
#وگرنہ_اگر defined(__APPLE__)
# اگر ہدف_OS_MAC
    Dl_info info;
    اگر (0 != dladdr(addr, &info))
    {
        واپس cv::String(info.dli_fname);
    }
# endif
#ورنہ
    // not supported, skip
#اگر_ختم
    واپس cv::String();
}

منطقی getBinLocation(std::string& dst)
{
    dst = getModuleLocation((خالی*)getModuleLocation); // using code address, doesn't work with static linkage!
    واپس !dst.empty();
}

#اگر_تعریف_شدہ _WIN32
منطقی getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#اگر _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    خالی* addr = (خالی*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_پتہ | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#اگر_ختم
    اگر (m)
    {
        wchar_t راستہ[4096];
        ثابت size_t راستہ_سائز = حجم(راستہ)/حجم(*راستہ);
        size_t sz = GetModuleFileNameW(m, راستہ, راستہ_سائز);
        اگر (sz > 0 && sz < راستہ_سائز)
        {
            راستہ[sz] = '\0';
            dst.assign(راستہ, sz);
            واپس سچ;
        }
    }
    واپس جھوٹ;
}
#اگر_ختم

cv::String ڈھونڈو_ڈیٹا_File(ثابت cv::String& relative_راستہ,
                        ثابت حرف* configuration_parameter,
                        ثابت std::vector<String>* تلاش_paths,
                        ثابت std::vector<String>* subdir_paths)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(تہی, cv::format("utils::findDataFile('%s', %s)", relative_راستہ.c_str(), configuration_parameter));

#تعریف_کرو TRY_FILE_WITH_سابقہ(سابقہ) \
{ \
    cv::String راستہ = utils::fs::join(سابقہ, relative_راستہ); \
    CV_LOG_DEBUG(تہی, cv::format("... Line %d: trying open '%s'", __LINE__, راستہ.c_str())); \
    FILE* f = fopen(راستہ.c_str(), "rb"); \
    اگر(f) { \
        fclose(f); \
        واپس راستہ; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_سابقہ("");


    // Step 1
    ثابت std::vector<cv::String>& تلاش_راستہ = تلاش_paths ? *تلاش_paths : get_ڈیٹا_تلاش_راستہ();
    برائے(size_t i = تلاش_راستہ.size(); i > 0; i--)
    {
        ثابت cv::String& سابقہ = تلاش_راستہ[i - 1];
        TRY_FILE_WITH_سابقہ(سابقہ);
    }

    ثابت std::vector<cv::String>& تلاش_subdir = subdir_paths ? *subdir_paths : get_ڈیٹا_تلاش_Sub_Directory();


    // Step 2
    ثابت cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ثابت cv::utils::Paths& تلاش_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    برائے (size_t k = 0; k < تلاش_hint.size(); k++)
    {
        cv::String datapath = تلاش_hint[k];
        اگر (datapath.empty())
            جاری;
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String سابقہ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_سابقہ(سابقہ);
            }
        }
        ورنہ
        {
            CV_LOG_WARNING(تہی, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ثابت cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    برائے (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        اگر (datapath.empty())
            جاری;
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String سابقہ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_سابقہ(سابقہ);
            }
        }
        ورنہ
        {
            CV_LOG_WARNING(تہی, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    اگر (!override_paths.empty())
    {
        CV_LOG_INFO(تہی, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_راستہ);
        واپس cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    منطقی has_tested_build_directory = جھوٹ;
    اگر (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(تہی, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ثابت حرف* build_subdirs[] = { OPENCV_ڈیٹا_BUILD_DIR_تلاش_PATHS };
        برائے (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            اگر (utils::fs::isDirectory(datapath))
            {
                برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String سابقہ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_سابقہ(سابقہ);
                }
            }
        }
        has_tested_build_directory = سچ;
    }

    cv::String ماخذ_dir;
    cv::String try_ماخذ_dir = cwd;
    برائے (صحیح سطحیں = 0; سطحیں < 3; ++سطحیں)
    {
        اگر (utils::fs::exists(utils::fs::join(try_ماخذ_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            ماخذ_dir = try_ماخذ_dir;
            توڑو;
        }
        try_ماخذ_dir = utils::fs::join(try_ماخذ_dir, "/..");
    }
    اگر (!ماخذ_dir.empty())
    {
        CV_LOG_DEBUG(تہی, "utils::findDataFile(): the current directory is source sub-directory: " << ماخذ_dir);
        CV_LOG_DEBUG(تہی, "utils::findDataFile(): <source>" << ماخذ_dir);
        cv::String datapath = ماخذ_dir;
        اگر (utils::fs::isDirectory(datapath))
        {
            برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String سابقہ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_سابقہ(سابقہ);
            }
        }
    }

    cv::String module_راستہ;
    اگر (getBinLocation(module_راستہ))
    {
        CV_LOG_DEBUG(تہی, "Detected module path: '" << module_راستہ << '\'');
    }
    ورنہ
    {
        CV_LOG_INFO(تہی, "Can't detect module binaries location");
    }

    اگر (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_راستہ) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_راستہ)))
    )
    {
        CV_LOG_DEBUG(تہی, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_راستہ);
        ثابت حرف* build_subdirs[] = { OPENCV_ڈیٹا_BUILD_DIR_تلاش_PATHS };
        برائے (size_t k = 0; k < حجم(build_subdirs)/حجم(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            اگر (utils::fs::isDirectory(datapath))
            {
                برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String سابقہ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_سابقہ(سابقہ);
                }
            }
        }
    }

#اگر defined OPENCV_INSTALL_ڈیٹا_DIR_RELATIVE
    اگر (!module_راستہ.empty())  // require module path
    {
        size_t pos = module_راستہ.rfind('/');
        اگر (pos == cv::String::npos)
            pos = module_راستہ.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_راستہ : module_راستہ.substr(0, pos);
        ثابت حرف* install_subdirs[] = { OPENCV_INSTALL_ڈیٹا_DIR_RELATIVE };
        برائے (size_t k = 0; k < حجم(install_subdirs)/حجم(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            اگر (utils::fs::isDirectory(datapath))
            {
                برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
                {
                    ثابت cv::String& subdir = تلاش_subdir[i - 1];
                    cv::String سابقہ = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_سابقہ(سابقہ);
                }
            }
            ورنہ
            {
                CV_LOG_DEBUG(تہی, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#اگر_ختم

#اگر defined OPENCV_INSTALL_سابقہ && defined OPENCV_ڈیٹا_INSTALL_راستہ
    cv::String install_dir(OPENCV_INSTALL_سابقہ);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    اگر (module_راستہ.empty()) // can't determine
        module_راستہ = install_dir;
    اگر (isSubDirectory(install_dir, module_راستہ) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_راستہ)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_ڈیٹا_INSTALL_راستہ);
        اگر (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(تہی, "utils::findDataFile(): trying install path: " << datapath);
            برائے(size_t i = تلاش_subdir.size(); i > 0; i--)
            {
                ثابت cv::String& subdir = تلاش_subdir[i - 1];
                cv::String سابقہ = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_سابقہ(سابقہ);
            }
        }
    }
#اگر_ختم

    واپس cv::String();  // not found
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(relative_راستہ);
    CV_غیر_مستعمل(configuration_parameter);
    CV_غیر_مستعمل(تلاش_paths);
    CV_غیر_مستعمل(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String ڈھونڈو_ڈیٹا_File(ثابت cv::String& relative_راستہ, منطقی required, ثابت حرف* configuration_parameter)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(تہی, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_راستہ.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String نتیجہ = cv::utils::ڈھونڈو_ڈیٹا_File(relative_راستہ,
                                                configuration_parameter,
                                                تہی,
                                                تہی);
    اگر (نتیجہ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_راستہ.c_str()));
    واپس نتیجہ;
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(relative_راستہ);
    CV_غیر_مستعمل(required);
    CV_غیر_مستعمل(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
