// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#अन्तर्भाव <iostream>
#define DBG(...) __VA_ARGS__
#अन्यथा
#define DBG(...)
#endif


#जर OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#अन्तर्भाव <windows.h>
#अन्तर्भाव <direct.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <io.h>
#अन्तर्भाव <stdio.h>
#किंवाजर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

नामाकाश cv { नामाकाश utils { नामाकाश fs {

#ifdef _WIN32
स्थिर स्थिरांक अक्षर native_separator = '\\';
#अन्यथा
स्थिर स्थिरांक अक्षर native_separator = '/';
#endif

स्थिर ओळीत
तार्किक isPathSeparator(अक्षर c)
{
    परत c == '/' || c == '\\';
}

cv::String join(स्थिरांक cv::String& base, स्थिरांक cv::String& path)
{
    जर (base.empty())
        परत path;
    जर (path.empty())
        परत base;

    तार्किक baseSep = isPathSeparator(base[base.size() - 1]);
    तार्किक pathSep = isPathSeparator(path[0]);
    String result;
    जर (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    अन्यथा जर (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    अन्यथा
    {
        result = base + path;
    }
    परत result;
}

CV_EXPORTS cv::String getParent(स्थिरांक cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    जर (loc == std::string::npos)
        परत std::string();
    परत std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(स्थिरांक std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    जर (loc == std::wstring::npos)
        परत std::wstring();
    परत std::wstring(path, 0, loc);
}

#जर OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(स्थिरांक cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    स्थिरांक अक्षर* result_str = _fullpath(रिक्त, path.c_str(), 0);
#अन्यथा
    स्थिरांक अक्षर* result_str = realpath(path.c_str(), रिक्त);
#endif
    जर (result_str)
    {
        result = cv::String(result_str);
        मुक्त((शून्य*)result_str);
    }
    परत result.empty() ? path : result;
}


तार्किक exists(स्थिरांक cv::String& path)
{
    CV_INSTRUMENT_REGION();

#जर defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        ब_अक्षर_प्रकार wpath[MAX_PATH];
        माप_प्रकार copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    परत !!status;
#अन्यथा
    संरचना stat stat_buf;
    परत (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS शून्य remove_all(स्थिरांक cv::String& path)
{
    जर (!exists(path))
        परत;
    जर (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, असत्य, सत्य);
        साठी (माप_प्रकार i = 0; i < entries.size(); i++)
        {
            स्थिरांक String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        तार्किक result = _rmdir(path.c_str()) == 0;
#अन्यथा
        तार्किक result = सूची_हटाओ(path.c_str()) == 0;
#endif
        जर (!result)
        {
            CV_LOG_ERROR(रिक्त, "Can't remove directory: " << path);
        }
    }
    अन्यथा
    {
#ifdef _MSC_VER
        तार्किक result = _unlink(path.c_str()) == 0;
#अन्यथा
        तार्किक result = unlink(path.c_str()) == 0;
#endif
        जर (!result)
        {
            CV_LOG_ERROR(रिक्त, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<अक्षर, 4096> buf;
#जर defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    परत cv::String();
#अन्यथा
    DWORD sz = GetCurrentDirectoryA(0, रिक्त);
    buf.allocate((माप_प्रकार)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    परत cv::String(buf.data(), (माप_प्रकार)sz);
#endif
#किंवाजर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    साठी(;;)
    {
        अक्षर* p = ::getcwd(buf.data(), buf.size());
        जर (p == रिक्त)
        {
            जर (त्रुटि_सं == दुस्फल)
            {
                buf.allocate(buf.size() * 2);
                सुरूठेवा;
            }
            परत cv::String();
        }
        थांबा;
    }
    परत cv::String(buf.data(), (माप_प्रकार)म_माप(buf.data()));
#अन्यथा
    परत cv::String();
#endif
}


तार्किक createDirectory(स्थिरांक cv::String& path)
{
    CV_INSTRUMENT_REGION();
#जर defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    ब_अक्षर_प्रकार wpath[MAX_PATH];
    माप_प्रकार copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
    पूर्णांक result = CreateDirectoryA(wpath, रिक्त) ? 0 : -1;
#अन्यथा
    पूर्णांक result = _mkdir(path.c_str());
#endif
#किंवाजर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    पूर्णांक result = सूची_गढ़ो(path.c_str(), 0777);
#अन्यथा
    पूर्णांक result = -1;
#endif

    जर (result == -1)
    {
        परत isDirectory(path);
    }
    परत सत्य;
}

तार्किक createDirectories(स्थिरांक cv::String& path_)
{
    cv::String path = path_;
    साठी (;;)
    {
        अक्षर last_char = path.empty() ? 0 : path[path.length() - 1];
        जर (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            सुरूठेवा;
        }
        थांबा;
    }

    जर (path.empty() || path == "./" || path == ".\\" || path == ".")
        परत सत्य;
    जर (isDirectory(path))
        परत सत्य;

    माप_प्रकार pos = path.rfind('/');
    जर (pos == cv::String::npos)
        pos = path.rfind('\\');
    जर (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        जर (!parent_directory.empty())
        {
            जर (!createDirectories(parent_directory))
                परत असत्य;
        }
    }

    परत createDirectory(path);
}

#ifdef _WIN32

संरचना FileLock::Impl
{
    Impl(स्थिरांक अक्षर* fname)
    {
        // http://support.microsoft.com/kb/316609
        पूर्णांक अंक_Retries = 5;
        करा
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, रिक्त,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, रिक्त);
            जर (INVALID_HANDLE_मूल्य == handle)
            {
                जर (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    अंक_Retries--;
                    Sleep(250);
                    सुरूठेवा;
                }
                अन्यथा
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            थांबा;
        } पर्यंत (अंक_Retries > 0);
    }
    ~Impl()
    {
        जर (INVALID_HANDLE_मूल्य != handle)
        {
            ::CloseHandle(handle);
        }
    }

    तार्किक lock()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, आकार(overlapped));
        परत !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, आकार(overlapped));
        परत !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    तार्किक lock_shared()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, आकार(overlapped));
        परत !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock_shared()
    {
        परत unlock();
    }

    HANDLE handle;

खाजगी:
    Impl(स्थिरांक Impl&); // disabled
    Impl& चालक=(स्थिरांक Impl&); // disabled
};

#किंवाजर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

संरचना FileLock::Impl
{
    Impl(स्थिरांक अक्षर* fname)
    {
        handle = ::खोलो(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        जर (handle >= 0)
            ::बंद(handle);
    }

    तार्किक lock()
    {
        संरचना ::flock l;
        std::स_रखो(&l, 0, आकार(l));
        l.l_type = F_WRLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        परत res;
    }
    तार्किक unlock()
    {
        संरचना ::flock l;
        std::स_रखो(&l, 0, आकार(l));
        l.l_type = F_UNLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLK, &l);
        परत res;
    }

    तार्किक lock_shared()
    {
        संरचना ::flock l;
        std::स_रखो(&l, 0, आकार(l));
        l.l_type = F_RDLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        परत res;
    }
    तार्किक unlock_shared()
    {
        परत unlock();
    }

    पूर्णांक handle;

खाजगी:
    Impl(स्थिरांक Impl&); // disabled
    Impl& चालक=(स्थिरांक Impl&); // disabled
};

#endif

FileLock::FileLock(स्थिरांक अक्षर* fname)
    : pImpl(नवीन Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = रिक्त;
}

शून्य FileLock::lock() { CV_Assert(pImpl->lock()); }
शून्य FileLock::unlock() { CV_Assert(pImpl->unlock()); }
शून्य FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
शून्य FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(स्थिरांक अक्षर* sub_directory_name, स्थिरांक अक्षर* configuration_name)
{
    String cache_path;
    जर (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    जर (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        ब_अक्षर_प्रकार tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        जर (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            पूर्णांक len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, रिक्त, 0, रिक्त, रिक्त);
            जर (len > 0)
            {
                std::vector<अक्षर> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, रिक्त, रिक्त);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#किंवाजर defined __ANDROID__
        // no defaults
#किंवाजर defined __APPLE__
        स्थिरांक std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        जर (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        अन्यथा
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#किंवाजर defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        जर (default_cache_path.empty())
        {
            स्थिरांक std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            जर (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        जर (default_cache_path.empty())
        {
            स्थिरांक std::string home_env = utils::getConfigurationParameterString("HOME");
            जर (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                जर (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        जर (default_cache_path.empty())
        {
            स्थिरांक अक्षर* temp_path = "/var/tmp/";
            जर (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        जर (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#अन्यथा
        // no defaults
#endif
        CV_LOG_VERBOSE(रिक्त, 0, "default_cache_path = " << default_cache_path);
        जर (!default_cache_path.empty())
        {
            जर (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                जर (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", सत्य)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    प्रयत्न
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, असत्य, सत्य);
                    }
                    पकडा (...)
                    {
                        // ignore
                    }
                    जर (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(रिक्त, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(रिक्त, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(रिक्त, "Feel free to cleanup these unused directories:");
                        साठी (माप_प्रकार i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(रिक्त, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(रिक्त, "Note: This message is showed only once.");
                    }
                }
                जर (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                जर (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(रिक्त, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                अन्यथा
                {
                    cache_path = default_cache_path;
                }
            }
            अन्यथा
            {
                CV_LOG_INFO(रिक्त, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        अन्यथा
        {
            CV_LOG_DEBUG(रिक्त, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    अन्यथा
    {
        जर (cache_path == "disabled")
            परत cache_path;
        जर (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(रिक्त, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            जर (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(रिक्त, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    जर (!cache_path.empty())
    {
        जर (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    परत cache_path;
}

#अन्यथा
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(स्थिरांक cv::String& /*path*/) { NOT_IMPLEMENTED }
तार्किक exists(स्थिरांक cv::String& /*path*/) { NOT_IMPLEMENTED }
शून्य remove_all(स्थिरांक cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
तार्किक createDirectory(स्थिरांक cv::String& /*path*/) { NOT_IMPLEMENTED }
तार्किक createDirectories(स्थिरांक cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(स्थिरांक अक्षर* /*sub_directory_name*/, स्थिरांक अक्षर* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
#अन्तर्भाव "plugin_loader.impl.hpp"
#endif
