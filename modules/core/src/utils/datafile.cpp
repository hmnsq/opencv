// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव "opencv_data_config.hpp"

#अन्तर्भाव <vector>
#अन्तर्भाव <fstream>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अपरिभाषय CV_LOG_STRIP_LEVEL
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव "opencv2/core/utils/logger.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

#यदि_परिभाषितम् _WIN32
#परिभाषय WIN32_LEAN_AND_माध्यम्
#अन्तर्भाव <windows.h>
#अपरिभाषय small
#अपरिभाषय min
#अपरिभाषय max
#अपरिभाषय निरपेक्षम्
#नोचेद्यदि defined(__linux__)
#अन्तर्भाव <dlfcn.h>  // requires -ldl
#नोचेद्यदि defined(__APPLE__)
#अन्तर्भाव <TargetConditionals.h>
#यदि TARGET_OS_MAC
#अन्तर्भाव <dlfcn.h>
#यदि_अन्तः
#यदि_अन्तः

नामाकाश cv { नामाकाश utils {

स्थिर cv::Ptr< std::vector<cv::String> > g_दत्त_अन्वेषणम्_मार्गः;
स्थिर cv::Ptr< std::vector<cv::String> > g_दत्त_अन्वेषणम्_subdir;

स्थिर std::vector<cv::String>& get_दत्त_अन्वेषणम्_मार्गः()
{
    यदि (g_दत्त_अन्वेषणम्_मार्गः.empty())
        g_दत्त_अन्वेषणम्_मार्गः.reset(नूतन std::vector<cv::String>());
    प्रत्यागम *(g_दत्त_अन्वेषणम्_मार्गः.get());
}

स्थिर std::vector<cv::String>& get_दत्त_अन्वेषणम्_Sub_Directory()
{
    यदि (g_दत्त_अन्वेषणम्_subdir.empty())
    {
        g_दत्त_अन्वेषणम्_subdir.reset(नूतन std::vector<cv::String>());
        g_दत्त_अन्वेषणम्_subdir->push_back("data");
        g_दत्त_अन्वेषणम्_subdir->push_back("");
    }
    प्रत्यागम *(g_दत्त_अन्वेषणम्_subdir.get());
}


CV_EXPORTS शून्य सङ्कलय_दत्त_अन्वेषणम्_मार्गः(ध्रुव cv::String& मार्गः)
{
    यदि (!मार्गः.empty() && utils::fs::isDirectory(मार्गः))
        get_दत्त_अन्वेषणम्_मार्गः().push_back(मार्गः);
}
CV_EXPORTS शून्य सङ्कलय_दत्त_अन्वेषणम्_Sub_Directory(ध्रुव cv::String& subdir)
{
    get_दत्त_अन्वेषणम्_Sub_Directory().push_back(subdir);
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक is_मार्गः_Sep(वर्ण c)
{
    प्रत्यागम c == '/' || c == '\\';
}
स्थिर तार्किक isSubDirectory_(ध्रुव cv::String& आधारः_मार्गः, ध्रुव cv::String& मार्गः)
{
    size_t N = आधारः_मार्गः.size();
    यदि (N == 0)
        प्रत्यागम असत्य;
    यदि (is_मार्गः_Sep(आधारः_मार्गः[N - 1]))
        N--;
    यदि (मार्गः.size() < N)
        प्रत्यागम असत्य;
    प्रति (size_t i = 0; i < N; i++)
    {
        यदि (मार्गः[i] == आधारः_मार्गः[i])
            अनुवर्त;
        यदि (is_मार्गः_Sep(मार्गः[i]) && is_मार्गः_Sep(आधारः_मार्गः[i]))
            अनुवर्त;
        प्रत्यागम असत्य;
    }
    size_t M = मार्गः.size();
    यदि (M > N)
    {
        यदि (!is_मार्गः_Sep(मार्गः[N]))
            प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
}

स्थिर तार्किक isSubDirectory(ध्रुव cv::String& आधारः_मार्गः, ध्रुव cv::String& मार्गः)
{
    तार्किक res = isSubDirectory_(आधारः_मार्गः, मार्गः);
    CV_LOG_VERBOSE(रिक्त, 0, "isSubDirectory(): base: " << आधारः_मार्गः << "  path: " << मार्गः << "  => result: " << (res ? "TRUE" : "FALSE"));
    प्रत्यागम res;
}
#यदि_अन्तः //OPENCV_HAVE_FILESYSTEM_SUPPORT

स्थिर cv::String getModuleLocation(ध्रुव शून्य* addr)
{
    CV_अप्रयुक्तम्(addr);
#यदि_परिभाषितम् _WIN32
    HMODULE m = 0;
#यदि _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_स्थानाङ्कः | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#यदि_अन्तः
    यदि (m)
    {
        TCHAR मार्गः[MAX_मार्गः];
        ध्रुव size_t मार्गः_आकार = परिमाण(मार्गः) / परिमाण(*मार्गः);
        size_t sz = GetModuleFileName(m, मार्गः, मार्गः_आकार);
        यदि (sz > 0 && sz < मार्गः_आकार)
        {
            मार्गः[sz] = TCHAR('\0');
#यदि_परिभाषितम् _UNICODE
            वर्ण char_मार्गः[MAX_मार्गः];
            size_t copied = wcstombs(char_मार्गः, मार्गः, MAX_मार्गः);
            CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
            प्रत्यागम cv::String(char_मार्गः);
#अन्यथा
            प्रत्यागम cv::String(मार्गः);
#यदि_अन्तः
        }
    }
#नोचेद्यदि defined(__linux__)
    Dl_info info;
    यदि (0 != dladdr(addr, &info))
    {
        प्रत्यागम cv::String(info.dli_fname);
    }
#नोचेद्यदि defined(__APPLE__)
# यदि TARGET_OS_MAC
    Dl_info info;
    यदि (0 != dladdr(addr, &info))
    {
        प्रत्यागम cv::String(info.dli_fname);
    }
# endif
#अन्यथा
    // not supported, skip
#यदि_अन्तः
    प्रत्यागम cv::String();
}

तार्किक getBinLocation(std::string& dst)
{
    dst = getModuleLocation((शून्य*)getModuleLocation); // using code address, doesn't work with static linkage!
    प्रत्यागम !dst.empty();
}

#यदि_परिभाषितम् _WIN32
तार्किक getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#यदि _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    शून्य* addr = (शून्य*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_स्थानाङ्कः | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#यदि_अन्तः
    यदि (m)
    {
        wchar_t मार्गः[4096];
        ध्रुव size_t मार्गः_आकार = परिमाण(मार्गः)/परिमाण(*मार्गः);
        size_t sz = GetModuleFileNameW(m, मार्गः, मार्गः_आकार);
        यदि (sz > 0 && sz < मार्गः_आकार)
        {
            मार्गः[sz] = '\0';
            dst.assign(मार्गः, sz);
            प्रत्यागम सत्य;
        }
    }
    प्रत्यागम असत्य;
}
#यदि_अन्तः

cv::String find_दत्त_File(ध्रुव cv::String& relative_मार्गः,
                        ध्रुव वर्ण* configuration_parameter,
                        ध्रुव std::vector<String>* अन्वेषणम्_paths,
                        ध्रुव std::vector<String>* subdir_paths)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(रिक्त, cv::format("utils::findDataFile('%s', %s)", relative_मार्गः.c_str(), configuration_parameter));

#परिभाषय TRY_FILE_WITH_उपसर्गः(उपसर्गः) \
{ \
    cv::String मार्गः = utils::fs::join(उपसर्गः, relative_मार्गः); \
    CV_LOG_DEBUG(रिक्त, cv::format("... Line %d: trying open '%s'", __LINE__, मार्गः.c_str())); \
    FILE* f = fopen(मार्गः.c_str(), "rb"); \
    यदि(f) { \
        fclose(f); \
        प्रत्यागम मार्गः; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_उपसर्गः("");


    // Step 1
    ध्रुव std::vector<cv::String>& अन्वेषणम्_मार्गः = अन्वेषणम्_paths ? *अन्वेषणम्_paths : get_दत्त_अन्वेषणम्_मार्गः();
    प्रति(size_t i = अन्वेषणम्_मार्गः.size(); i > 0; i--)
    {
        ध्रुव cv::String& उपसर्गः = अन्वेषणम्_मार्गः[i - 1];
        TRY_FILE_WITH_उपसर्गः(उपसर्गः);
    }

    ध्रुव std::vector<cv::String>& अन्वेषणम्_subdir = subdir_paths ? *subdir_paths : get_दत्त_अन्वेषणम्_Sub_Directory();


    // Step 2
    ध्रुव cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    ध्रुव cv::utils::Paths& अन्वेषणम्_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    प्रति (size_t k = 0; k < अन्वेषणम्_hint.size(); k++)
    {
        cv::String datapath = अन्वेषणम्_hint[k];
        यदि (datapath.empty())
            अनुवर्त;
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_उपसर्गः(उपसर्गः);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    ध्रुव cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    प्रति (size_t k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        यदि (datapath.empty())
            अनुवर्त;
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_उपसर्गः(उपसर्गः);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(रिक्त, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    यदि (!override_paths.empty())
    {
        CV_LOG_INFO(रिक्त, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_मार्गः);
        प्रत्यागम cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    तार्किक has_tested_build_directory = असत्य;
    यदि (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_दत्त_BUILD_DIR_अन्वेषणम्_PATHS };
        प्रति (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            यदि (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_उपसर्गः(उपसर्गः);
                }
            }
        }
        has_tested_build_directory = सत्य;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    प्रति (पूर्णाङ्क स्तराः = 0; स्तराः < 3; ++स्तराः)
    {
        यदि (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            भङ्ग;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    यदि (!source_dir.empty())
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        यदि (utils::fs::isDirectory(datapath))
        {
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_उपसर्गः(उपसर्गः);
            }
        }
    }

    cv::String module_मार्गः;
    यदि (getBinLocation(module_मार्गः))
    {
        CV_LOG_DEBUG(रिक्त, "Detected module path: '" << module_मार्गः << '\'');
    }
    अन्यथा
    {
        CV_LOG_INFO(रिक्त, "Can't detect module binaries location");
    }

    यदि (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_मार्गः) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_मार्गः)))
    )
    {
        CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_मार्गः);
        ध्रुव वर्ण* build_subdirs[] = { OPENCV_दत्त_BUILD_DIR_अन्वेषणम्_PATHS };
        प्रति (size_t k = 0; k < परिमाण(build_subdirs)/परिमाण(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            यदि (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_उपसर्गः(उपसर्गः);
                }
            }
        }
    }

#यदि defined OPENCV_INSTALL_दत्त_DIR_RELATIVE
    यदि (!module_मार्गः.empty())  // require module path
    {
        size_t pos = module_मार्गः.rfind('/');
        यदि (pos == cv::String::npos)
            pos = module_मार्गः.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_मार्गः : module_मार्गः.substr(0, pos);
        ध्रुव वर्ण* install_subdirs[] = { OPENCV_INSTALL_दत्त_DIR_RELATIVE };
        प्रति (size_t k = 0; k < परिमाण(install_subdirs)/परिमाण(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            यदि (utils::fs::isDirectory(datapath))
            {
                प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
                {
                    ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                    cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_उपसर्गः(उपसर्गः);
                }
            }
            अन्यथा
            {
                CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#यदि_अन्तः

#यदि defined OPENCV_INSTALL_उपसर्गः && defined OPENCV_दत्त_INSTALL_मार्गः
    cv::String install_dir(OPENCV_INSTALL_उपसर्गः);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    यदि (module_मार्गः.empty()) // can't determine
        module_मार्गः = install_dir;
    यदि (isSubDirectory(install_dir, module_मार्गः) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_मार्गः)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_दत्त_INSTALL_मार्गः);
        यदि (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(रिक्त, "utils::findDataFile(): trying install path: " << datapath);
            प्रति(size_t i = अन्वेषणम्_subdir.size(); i > 0; i--)
            {
                ध्रुव cv::String& subdir = अन्वेषणम्_subdir[i - 1];
                cv::String उपसर्गः = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_उपसर्गः(उपसर्गः);
            }
        }
    }
#यदि_अन्तः

    प्रत्यागम cv::String();  // not found
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(relative_मार्गः);
    CV_अप्रयुक्तम्(configuration_parameter);
    CV_अप्रयुक्तम्(अन्वेषणम्_paths);
    CV_अप्रयुक्तम्(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String find_दत्त_File(ध्रुव cv::String& relative_मार्गः, तार्किक required, ध्रुव वर्ण* configuration_parameter)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_मार्गः.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String परिणाम = cv::utils::find_दत्त_File(relative_मार्गः,
                                                configuration_parameter,
                                                रिक्त,
                                                रिक्त);
    यदि (परिणाम.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_मार्गः.c_str()));
    प्रत्यागम परिणाम;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(relative_मार्गः);
    CV_अप्रयुक्तम्(required);
    CV_अप्रयुक्तम्(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
