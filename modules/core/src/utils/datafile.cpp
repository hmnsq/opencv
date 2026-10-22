// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "../precomp.hpp"

#समावेश "opencv_data_config.hpp"

#समावेश <vector>
#समावेश <fstream>

#समावेश <opencv2/core/utils/logger.defines.hpp>
#अघोषित CV_LOG_STRIP_LEVEL
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#समावेश "opencv2/core/utils/logger.hpp"
#समावेश "opencv2/core/utils/filesystem.hpp"

#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश "opencv2/core/utils/filesystem.private.hpp"

#अगर_घोषित _WIN32
#घोषणा WIN32_LEAN_AND_MEAN
#समावेश <windows.h>
#अघोषित small
#अघोषित min
#अघोषित max
#अघोषित असल
#या_अगर defined(__linux__)
#समावेश <dlfcn.h>  // requires -ldl
#या_अगर defined(__APPLE__)
#समावेश <TargetConditionals.h>
#अगर TARGET_OS_MAC
#समावेश <dlfcn.h>
#पूर्ण_अगर
#पूर्ण_अगर

namespace cv { namespace utils {

अटल cv::Ptr< std::vector<cv::String> > g_तथ्य_खोज_path;
अटल cv::Ptr< std::vector<cv::String> > g_तथ्य_खोज_subdir;

अटल std::vector<cv::String>& get_तथ्य_खोज_Path()
{
    अगर (g_तथ्य_खोज_path.empty())
        g_तथ्य_खोज_path.reset(नया std::vector<cv::String>());
    वापस *(g_तथ्य_खोज_path.लो());
}

अटल std::vector<cv::String>& get_तथ्य_खोज_Sub_Directory()
{
    अगर (g_तथ्य_खोज_subdir.empty())
    {
        g_तथ्य_खोज_subdir.reset(नया std::vector<cv::String>());
        g_तथ्य_खोज_subdir->push_back("data");
        g_तथ्य_खोज_subdir->push_back("");
    }
    वापस *(g_तथ्य_खोज_subdir.लो());
}


CV_EXPORTS व्योम add_तथ्य_खोज_Path(स्थिर cv::String& path)
{
    अगर (!path.empty() && utils::fs::isDirectory(path))
        get_तथ्य_खोज_Path().push_back(path);
}
CV_EXPORTS व्योम add_तथ्य_खोज_Sub_Directory(स्थिर cv::String& subdir)
{
    get_तथ्य_खोज_Sub_Directory().push_back(subdir);
}

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
अटल bool isPathSep(अक्षर c)
{
    वापस c == '/' || c == '\\';
}
अटल bool isSubDirectory_(स्थिर cv::String& base_path, स्थिर cv::String& path)
{
    माप_प्रकार N = base_path.size();
    अगर (N == 0)
        वापस false;
    अगर (isPathSep(base_path[N - 1]))
        N--;
    अगर (path.size() < N)
        वापस false;
    क्रम (माप_प्रकार i = 0; i < N; i++)
    {
        अगर (path[i] == base_path[i])
            जारी;
        अगर (isPathSep(path[i]) && isPathSep(base_path[i]))
            जारी;
        वापस false;
    }
    माप_प्रकार M = path.size();
    अगर (M > N)
    {
        अगर (!isPathSep(path[N]))
            वापस false;
    }
    वापस true;
}

अटल bool isSubDirectory(स्थिर cv::String& base_path, स्थिर cv::String& path)
{
    bool res = isSubDirectory_(base_path, path);
    CV_LOG_VERBOSE(शून्य, 0, "isSubDirectory(): base: " << base_path << "  path: " << path << "  => result: " << (res ? "TRUE" : "FALSE"));
    वापस res;
}
#पूर्ण_अगर //OPENCV_HAVE_FILESYSTEM_SUPPORT

अटल cv::String getModuleLocation(स्थिर व्योम* addr)
{
    CV_UNUSED(addr);
#अगर_घोषित _WIN32
    HMODULE m = 0;
#अगर _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#पूर्ण_अगर
    अगर (m)
    {
        TCHAR path[MAX_PATH];
        स्थिर माप_प्रकार path_आकार = माप(path) / माप(*path);
        माप_प्रकार sz = GetModuleFileName(m, path, path_आकार);
        अगर (sz > 0 && sz < path_आकार)
        {
            path[sz] = TCHAR('\0');
#अगर_घोषित _UNICODE
            अक्षर char_path[MAX_PATH];
            माप_प्रकार copied = wcstombs(char_path, path, MAX_PATH);
            CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
            वापस cv::String(char_path);
#अन्यथा
            वापस cv::String(path);
#पूर्ण_अगर
        }
    }
#या_अगर defined(__linux__)
    Dl_info info;
    अगर (0 != dladdr(addr, &info))
    {
        वापस cv::String(info.dli_fname);
    }
#या_अगर defined(__APPLE__)
# अगर TARGET_OS_MAC
    Dl_info info;
    अगर (0 != dladdr(addr, &info))
    {
        वापस cv::String(info.dli_fname);
    }
# endif
#अन्यथा
    // not supported, skip
#पूर्ण_अगर
    वापस cv::String();
}

bool getBinLocation(std::string& dst)
{
    dst = getModuleLocation((व्योम*)getModuleLocation); // using code address, doesn't work with static linkage!
    वापस !dst.empty();
}

#अगर_घोषित _WIN32
bool getBinLocation(std::wstring& dst)
{
    HMODULE m = 0;
#अगर _WIN32_WINNT >= 0x0501 && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
    व्योम* addr = (व्योम*)getModuleLocation; // using code address, doesn't work with static linkage!
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCTSTR>(addr),
        &m);
#पूर्ण_अगर
    अगर (m)
    {
        ब_अक्षर_प्रकार path[4096];
        स्थिर माप_प्रकार path_आकार = माप(path)/माप(*path);
        माप_प्रकार sz = GetModuleFileNameW(m, path, path_आकार);
        अगर (sz > 0 && sz < path_आकार)
        {
            path[sz] = '\0';
            dst.assign(path, sz);
            वापस true;
        }
    }
    वापस false;
}
#पूर्ण_अगर

cv::String find_तथ्य_File(स्थिर cv::String& relative_path,
                        स्थिर अक्षर* configuration_parameter,
                        स्थिर std::vector<String>* खोज_paths,
                        स्थिर std::vector<String>* subdir_paths)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    configuration_parameter = configuration_parameter ? configuration_parameter : "OPENCV_DATA_PATH";
    CV_LOG_DEBUG(शून्य, cv::format("utils::findDataFile('%s', %s)", relative_path.c_str(), configuration_parameter));

#घोषणा TRY_FILE_WITH_PREFIX(prefix) \
{ \
    cv::String path = utils::fs::join(prefix, relative_path); \
    CV_LOG_DEBUG(शून्य, cv::format("... Line %d: trying open '%s'", __LINE__, path.c_str())); \
    खाता* f = ख_खोलो(path.c_str(), "rb"); \
    अगर(f) { \
        ख_बंद(f); \
        वापस path; \
    } \
}


    // Step 0: check current directory or absolute path at first
    TRY_FILE_WITH_PREFIX("");


    // Step 1
    स्थिर std::vector<cv::String>& खोज_path = खोज_paths ? *खोज_paths : get_तथ्य_खोज_Path();
    क्रम(माप_प्रकार i = खोज_path.size(); i > 0; i--)
    {
        स्थिर cv::String& prefix = खोज_path[i - 1];
        TRY_FILE_WITH_PREFIX(prefix);
    }

    स्थिर std::vector<cv::String>& खोज_subdir = subdir_paths ? *subdir_paths : get_तथ्य_खोज_Sub_Directory();


    // Step 2
    स्थिर cv::String configuration_parameter_s(configuration_parameter ? configuration_parameter : "");
    स्थिर cv::utils::Paths& खोज_hint = configuration_parameter_s.empty() ? cv::utils::Paths()
                                          : getConfigurationParameterPaths((configuration_parameter_s + "_HINT").c_str());
    क्रम (माप_प्रकार k = 0; k < खोज_hint.size(); k++)
    {
        cv::String datapath = खोज_hint[k];
        अगर (datapath.empty())
            जारी;
        अगर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): trying " << configuration_parameter << "_HINT=" << datapath);
            क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                स्थिर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(शून्य, configuration_parameter << "_HINT is specified but it is not a directory: " << datapath);
        }
    }


    // Step 3
    स्थिर cv::utils::Paths& override_paths = configuration_parameter_s.empty() ? cv::utils::Paths()
                                           : getConfigurationParameterPaths(configuration_parameter);
    क्रम (माप_प्रकार k = 0; k < override_paths.size(); k++)
    {
        cv::String datapath = override_paths[k];
        अगर (datapath.empty())
            जारी;
        अगर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): trying " << configuration_parameter << "=" << datapath);
            क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                स्थिर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
        अन्यथा
        {
            CV_LOG_WARNING(शून्य, configuration_parameter << " is specified but it is not a directory: " << datapath);
        }
    }
    अगर (!override_paths.empty())
    {
        CV_LOG_INFO(शून्य, "utils::findDataFile(): can't find data file via " << configuration_parameter << " configuration override: " << relative_path);
        वापस cv::String();
    }


    // Steps: 4, 5, 6
    cv::String cwd = utils::fs::getcwd();
    cv::String build_dir(OPENCV_BUILD_DIR);
    bool has_tested_build_directory = false;
    अगर (isSubDirectory(build_dir, cwd) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(cwd)))
    {
        CV_LOG_DEBUG(शून्य, "utils::findDataFile(): the current directory is build sub-directory: " << cwd);
        स्थिर अक्षर* build_subdirs[] = { OPENCV_तथ्य_BUILD_DIR_खोज_PATHS };
        क्रम (माप_प्रकार k = 0; k < माप(build_subdirs)/माप(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            अगर (utils::fs::isDirectory(datapath))
            {
                क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    स्थिर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
        has_tested_build_directory = true;
    }

    cv::String source_dir;
    cv::String try_source_dir = cwd;
    क्रम (पूर्णांक levels = 0; levels < 3; ++levels)
    {
        अगर (utils::fs::exists(utils::fs::join(try_source_dir, "modules/core/include/opencv2/core/version.hpp")))
        {
            source_dir = try_source_dir;
            अवरोध;
        }
        try_source_dir = utils::fs::join(try_source_dir, "/..");
    }
    अगर (!source_dir.empty())
    {
        CV_LOG_DEBUG(शून्य, "utils::findDataFile(): the current directory is source sub-directory: " << source_dir);
        CV_LOG_DEBUG(शून्य, "utils::findDataFile(): <source>" << source_dir);
        cv::String datapath = source_dir;
        अगर (utils::fs::isDirectory(datapath))
        {
            क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                स्थिर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }

    cv::String module_path;
    अगर (getBinLocation(module_path))
    {
        CV_LOG_DEBUG(शून्य, "Detected module path: '" << module_path << '\'');
    }
    अन्यथा
    {
        CV_LOG_INFO(शून्य, "Can't detect module binaries location");
    }

    अगर (!has_tested_build_directory &&
        (isSubDirectory(build_dir, module_path) || isSubDirectory(utils::fs::canonical(build_dir), utils::fs::canonical(module_path)))
    )
    {
        CV_LOG_DEBUG(शून्य, "utils::findDataFile(): the binary module directory is build sub-directory: " << module_path);
        स्थिर अक्षर* build_subdirs[] = { OPENCV_तथ्य_BUILD_DIR_खोज_PATHS };
        क्रम (माप_प्रकार k = 0; k < माप(build_subdirs)/माप(build_subdirs[0]); k++)
        {
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): <build>/" << build_subdirs[k]);
            cv::String datapath = utils::fs::join(build_dir, build_subdirs[k]);
            अगर (utils::fs::isDirectory(datapath))
            {
                क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    स्थिर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
        }
    }

#अगर defined OPENCV_INSTALL_तथ्य_DIR_RELATIVE
    अगर (!module_path.empty())  // require module path
    {
        माप_प्रकार pos = module_path.rfind('/');
        अगर (pos == cv::String::npos)
            pos = module_path.rfind('\\');
        cv::String module_dir = (pos == cv::String::npos) ? module_path : module_path.substr(0, pos);
        स्थिर अक्षर* install_subdirs[] = { OPENCV_INSTALL_तथ्य_DIR_RELATIVE };
        क्रम (माप_प्रकार k = 0; k < माप(install_subdirs)/माप(install_subdirs[0]); k++)
        {
            cv::String datapath = utils::fs::join(module_dir, install_subdirs[k]);
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): trying install path (from binary path): " << datapath);
            अगर (utils::fs::isDirectory(datapath))
            {
                क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
                {
                    स्थिर cv::String& subdir = खोज_subdir[i - 1];
                    cv::String prefix = utils::fs::join(datapath, subdir);
                    TRY_FILE_WITH_PREFIX(prefix);
                }
            }
            अन्यथा
            {
                CV_LOG_DEBUG(शून्य, "utils::findDataFile(): ... skip, not a valid directory: " << datapath);
            }
        }
    }
#पूर्ण_अगर

#अगर defined OPENCV_INSTALL_PREFIX && defined OPENCV_तथ्य_INSTALL_PATH
    cv::String install_dir(OPENCV_INSTALL_PREFIX);
    // use core/world module path and verify that library is running from installation directory
    // It is necessary to avoid touching of unrelated common /usr/local path
    अगर (module_path.empty()) // can't determine
        module_path = install_dir;
    अगर (isSubDirectory(install_dir, module_path) || isSubDirectory(utils::fs::canonical(install_dir), utils::fs::canonical(module_path)))
    {
        cv::String datapath = utils::fs::join(install_dir, OPENCV_तथ्य_INSTALL_PATH);
        अगर (utils::fs::isDirectory(datapath))
        {
            CV_LOG_DEBUG(शून्य, "utils::findDataFile(): trying install path: " << datapath);
            क्रम(माप_प्रकार i = खोज_subdir.size(); i > 0; i--)
            {
                स्थिर cv::String& subdir = खोज_subdir[i - 1];
                cv::String prefix = utils::fs::join(datapath, subdir);
                TRY_FILE_WITH_PREFIX(prefix);
            }
        }
    }
#पूर्ण_अगर

    वापस cv::String();  // not found
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(configuration_parameter);
    CV_UNUSED(खोज_paths);
    CV_UNUSED(subdir_paths);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

cv::String find_तथ्य_File(स्थिर cv::String& relative_path, bool required, स्थिर अक्षर* configuration_parameter)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(शून्य, cv::format("cv::utils::findDataFile('%s', %s, %s)",
                                  relative_path.c_str(), required ? "true" : "false",
                                  configuration_parameter ? configuration_parameter : "NULL"));
    cv::String परिणाम = cv::utils::find_तथ्य_File(relative_path,
                                                configuration_parameter,
                                                शून्य,
                                                शून्य);
    अगर (परिणाम.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV: Can't find required data file: %s", relative_path.c_str()));
    वापस परिणाम;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(configuration_parameter);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

}} // namespace
