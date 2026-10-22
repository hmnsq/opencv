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


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT

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
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

नामाकाश cv { नामाकाश utils { नामाकाश fs {

#ifdef _WIN32
स्थिर ध्रुव वर्ण native_separator = '\\';
#अन्यथा
स्थिर ध्रुव वर्ण native_separator = '/';
#endif

स्थिर अन्तःस्थ
तार्किक isPathSeparator(वर्ण c)
{
    प्रत्यागम c == '/' || c == '\\';
}

cv::String join(ध्रुव cv::String& base, ध्रुव cv::String& path)
{
    यदि (base.empty())
        प्रत्यागम path;
    यदि (path.empty())
        प्रत्यागम base;

    तार्किक baseSep = isPathSeparator(base[base.size() - 1]);
    तार्किक pathSep = isPathSeparator(path[0]);
    String result;
    यदि (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    अन्यथा यदि (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    अन्यथा
    {
        result = base + path;
    }
    प्रत्यागम result;
}

CV_EXPORTS cv::String getParent(ध्रुव cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    यदि (loc == std::string::npos)
        प्रत्यागम std::string();
    प्रत्यागम std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ध्रुव std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    यदि (loc == std::wstring::npos)
        प्रत्यागम std::wstring();
    प्रत्यागम std::wstring(path, 0, loc);
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ध्रुव cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ध्रुव वर्ण* result_str = _fullpath(रिक्त, path.c_str(), 0);
#अन्यथा
    ध्रुव वर्ण* result_str = realpath(path.c_str(), रिक्त);
#endif
    यदि (result_str)
    {
        result = cv::String(result_str);
        मुक्त((शून्य*)result_str);
    }
    प्रत्यागम result.empty() ? path : result;
}


तार्किक exists(ध्रुव cv::String& path)
{
    CV_INSTRUMENT_REGION();

#यदि defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    प्रत्यागम !!status;
#अन्यथा
    संरचना stat stat_buf;
    प्रत्यागम (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS शून्य remove_all(ध्रुव cv::String& path)
{
    यदि (!exists(path))
        प्रत्यागम;
    यदि (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, असत्य, सत्य);
        प्रति (size_t i = 0; i < entries.size(); i++)
        {
            ध्रुव String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        तार्किक result = _rmdir(path.c_str()) == 0;
#अन्यथा
        तार्किक result = rmdir(path.c_str()) == 0;
#endif
        यदि (!result)
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
        यदि (!result)
        {
            CV_LOG_ERROR(रिक्त, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<वर्ण, 4096> buf;
#यदि defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    प्रत्यागम cv::String();
#अन्यथा
    DWORD sz = GetCurrentDirectoryA(0, रिक्त);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    प्रत्यागम cv::String(buf.data(), (size_t)sz);
#endif
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    प्रति(;;)
    {
        वर्ण* p = ::getcwd(buf.data(), buf.size());
        यदि (p == रिक्त)
        {
            यदि (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                अनुवर्त;
            }
            प्रत्यागम cv::String();
        }
        भङ्ग;
    }
    प्रत्यागम cv::String(buf.data(), (size_t)strlen(buf.data()));
#अन्यथा
    प्रत्यागम cv::String();
#endif
}


तार्किक createDirectory(ध्रुव cv::String& path)
{
    CV_INSTRUMENT_REGION();
#यदि defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    पूर्णाङ्क result = CreateDirectoryA(wpath, रिक्त) ? 0 : -1;
#अन्यथा
    पूर्णाङ्क result = _mkdir(path.c_str());
#endif
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    पूर्णाङ्क result = mkdir(path.c_str(), 0777);
#अन्यथा
    पूर्णाङ्क result = -1;
#endif

    यदि (result == -1)
    {
        प्रत्यागम isDirectory(path);
    }
    प्रत्यागम सत्य;
}

तार्किक createDirectories(ध्रुव cv::String& path_)
{
    cv::String path = path_;
    प्रति (;;)
    {
        वर्ण last_char = path.empty() ? 0 : path[path.length() - 1];
        यदि (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            अनुवर्त;
        }
        भङ्ग;
    }

    यदि (path.empty() || path == "./" || path == ".\\" || path == ".")
        प्रत्यागम सत्य;
    यदि (isDirectory(path))
        प्रत्यागम सत्य;

    size_t pos = path.rfind('/');
    यदि (pos == cv::String::npos)
        pos = path.rfind('\\');
    यदि (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        यदि (!parent_directory.empty())
        {
            यदि (!createDirectories(parent_directory))
                प्रत्यागम असत्य;
        }
    }

    प्रत्यागम createDirectory(path);
}

#ifdef _WIN32

संरचना FileLock::Impl
{
    Impl(ध्रुव वर्ण* fname)
    {
        // http://support.microsoft.com/kb/316609
        पूर्णाङ्क अंक_Retries = 5;
        कुरु
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, रिक्त,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, रिक्त);
            यदि (INVALID_HANDLE_मूल्य == handle)
            {
                यदि (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    अंक_Retries--;
                    Sleep(250);
                    अनुवर्त;
                }
                अन्यथा
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            भङ्ग;
        } यावत् (अंक_Retries > 0);
    }
    ~Impl()
    {
        यदि (INVALID_HANDLE_मूल्य != handle)
        {
            ::CloseHandle(handle);
        }
    }

    तार्किक lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    तार्किक lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock_shared()
    {
        प्रत्यागम unlock();
    }

    HANDLE handle;

गुप्त:
    Impl(ध्रुव Impl&); // disabled
    Impl& operator=(ध्रुव Impl&); // disabled
};

#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

संरचना FileLock::Impl
{
    Impl(ध्रुव वर्ण* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        यदि (handle >= 0)
            ::close(handle);
    }

    तार्किक lock()
    {
        संरचना ::flock l;
        std::memset(&l, 0, परिमाण(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        प्रत्यागम res;
    }
    तार्किक unlock()
    {
        संरचना ::flock l;
        std::memset(&l, 0, परिमाण(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLK, &l);
        प्रत्यागम res;
    }

    तार्किक lock_shared()
    {
        संरचना ::flock l;
        std::memset(&l, 0, परिमाण(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        प्रत्यागम res;
    }
    तार्किक unlock_shared()
    {
        प्रत्यागम unlock();
    }

    पूर्णाङ्क handle;

गुप्त:
    Impl(ध्रुव Impl&); // disabled
    Impl& operator=(ध्रुव Impl&); // disabled
};

#endif

FileLock::FileLock(ध्रुव वर्ण* fname)
    : pImpl(नूतन Impl(fname))
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



cv::String getCacheDirectory(ध्रुव वर्ण* sub_directory_name, ध्रुव वर्ण* configuration_name)
{
    String cache_path;
    यदि (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    यदि (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        यदि (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            पूर्णाङ्क len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, रिक्त, 0, रिक्त, रिक्त);
            यदि (len > 0)
            {
                std::vector<वर्ण> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, रिक्त, रिक्त);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#नोचेद्यदि defined __ANDROID__
        // no defaults
#नोचेद्यदि defined __APPLE__
        ध्रुव std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        यदि (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        अन्यथा
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#नोचेद्यदि defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        यदि (default_cache_path.empty())
        {
            ध्रुव std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            यदि (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        यदि (default_cache_path.empty())
        {
            ध्रुव std::string home_env = utils::getConfigurationParameterString("HOME");
            यदि (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                यदि (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        यदि (default_cache_path.empty())
        {
            ध्रुव वर्ण* temp_path = "/var/tmp/";
            यदि (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        यदि (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#अन्यथा
        // no defaults
#endif
        CV_LOG_VERBOSE(रिक्त, 0, "default_cache_path = " << default_cache_path);
        यदि (!default_cache_path.empty())
        {
            यदि (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                यदि (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", सत्य)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    यत्न
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, असत्य, सत्य);
                    }
                    गृहाण (...)
                    {
                        // ignore
                    }
                    यदि (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(रिक्त, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(रिक्त, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(रिक्त, "Feel free to cleanup these unused directories:");
                        प्रति (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(रिक्त, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(रिक्त, "Note: This message is showed only once.");
                    }
                }
                यदि (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                यदि (!utils::fs::createDirectories(default_cache_path))
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
        यदि (cache_path == "disabled")
            प्रत्यागम cache_path;
        यदि (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(रिक्त, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            यदि (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(रिक्त, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    यदि (!cache_path.empty())
    {
        यदि (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    प्रत्यागम cache_path;
}

#अन्यथा
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ध्रुव cv::String& /*path*/) { NOT_IMPLEMENTED }
तार्किक exists(ध्रुव cv::String& /*path*/) { NOT_IMPLEMENTED }
शून्य remove_all(ध्रुव cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
तार्किक createDirectory(ध्रुव cv::String& /*path*/) { NOT_IMPLEMENTED }
तार्किक createDirectories(ध्रुव cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ध्रुव वर्ण* /*sub_directory_name*/, ध्रुव वर्ण* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
#अन्तर्भाव "plugin_loader.impl.hpp"
#endif
